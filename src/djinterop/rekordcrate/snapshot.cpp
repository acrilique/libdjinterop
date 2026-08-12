/*
    This file is part of libdjinterop.

    libdjinterop is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libdjinterop is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libdjinterop.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "snapshot.hpp"

#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <string>

#include <djinterop/exceptions.hpp>
#include <djinterop/musical_key.hpp>
#include <djinterop/track_snapshot.hpp>

namespace djinterop::rekordcrate
{
namespace
{
/// Map a `musical_key` to the canonical key name rekordcrate's PDB writer folds
/// for dedup (e.g. "Cmaj", "Amin"). ponytail: ceiling — this is a fixed
/// 24-entry table mirroring rekordbox's own key list; Camelot wheel numbers are
/// not emitted (the PDB stores the name string). A caller wanting raw control
/// can leave `snapshot.key` unset.
std::string key_to_string(musical_key k)
{
    switch (k)
    {
        case musical_key::c_major: return "Cmaj";
        case musical_key::a_minor: return "Amin";
        case musical_key::g_major: return "Gmaj";
        case musical_key::e_minor: return "Emin";
        case musical_key::d_major: return "Dmaj";
        case musical_key::b_minor: return "Bmin";
        case musical_key::a_major: return "Amaj";
        case musical_key::f_sharp_minor: return "F#min";
        case musical_key::e_major: return "Emaj";
        case musical_key::d_flat_minor: return "Dbmin";
        case musical_key::b_major: return "Bmaj";
        case musical_key::a_flat_minor: return "Abmin";
        case musical_key::f_sharp_major: return "F#maj";
        case musical_key::e_flat_minor: return "Ebmin";
        case musical_key::d_flat_major: return "Dbmaj";
        case musical_key::b_flat_minor: return "Bbmin";
        case musical_key::a_flat_major: return "Abmaj";
        case musical_key::f_minor: return "Fmin";
        case musical_key::e_flat_major: return "Ebmaj";
        case musical_key::c_minor: return "Cmin";
        case musical_key::b_flat_major: return "Bbmaj";
        case musical_key::g_minor: return "Gmin";
        case musical_key::f_major: return "Fmaj";
        case musical_key::d_minor: return "Dmin";
    }
    throw std::invalid_argument{"unknown musical_key"};
}

/// Pull the basename (filename) off a device-relative path. rekordcrate's PDB
/// stores both the full `file_path` and the `filename` separately.
std::string basename_of(const std::string& path)
{
    auto pos = path.find_last_of('/');
    return pos == std::string::npos ? path : path.substr(pos + 1);
}

/// Map a `track_snapshot`'s waveform (one entry per detail column at the rate
/// the caller produced them) into the `ffi_performance_data` shape. Opacity is
/// dropped — rekordcrate's ANLZ mapping uses only the 3-band amplitude. No
/// downsample happens here; rekordcrate owns the 150 Hz → 6.667 Hz preview
/// step.
void map_waveform(
    const std::vector<waveform_entry>& waveform, ffi_performance_data& out)
{
    out.waveform_detail.clear();
    out.waveform_detail.reserve(waveform.size());
    for (auto& e : waveform)
    {
        out.waveform_detail.push_back({e.low.value, e.mid.value, e.high.value});
    }
    // Height is not separately carried by `waveform_entry`; derive 0-31 from
    // the band max so PWAV/PWV2/PWV3/PWV5 get a plausible peak. ponytail:
    // ceiling — the true peak amplitude isn't recoverable from 3-band energies,
    // so the derived height is a lower bound on the real envelope.
    out.waveform_height.clear();
    out.waveform_height.reserve(waveform.size());
    for (auto& e : waveform)
    {
        auto m = static_cast<int>(e.low.value);
        m = std::max(m, static_cast<int>(e.mid.value));
        m = std::max(m, static_cast<int>(e.high.value));
        out.waveform_height.push_back(static_cast<uint8_t>(m * 31 / 255));
    }
}

/// Map hot_cues + loops into the ffi lists. Hot-cue/loop slot index comes from
/// the container position (1-based), matching how Engine v2 stores them.
void map_cues(
    const std::vector<std::optional<hot_cue>>& hot_cues,
    const std::vector<std::optional<loop>>& loops, ffi_performance_data& out)
{
    out.hot_cues.clear();
    for (size_t i = 0; i < hot_cues.size(); ++i)
    {
        if (hot_cues[i])
        {
            out.hot_cues.push_back(
                ffi_hot_cue{
                    static_cast<uint32_t>(i + 1),
                    hot_cues[i]->label,
                    hot_cues[i]->sample_offset,
                    hot_cues[i]->color.r,
                    hot_cues[i]->color.g,
                    hot_cues[i]->color.b,
                    hot_cues[i]->color.a,
                });
        }
    }
    out.loops.clear();
    for (size_t i = 0; i < loops.size(); ++i)
    {
        if (loops[i])
        {
            out.loops.push_back(
                ffi_loop{
                    static_cast<uint32_t>(i + 1),
                    loops[i]->label,
                    loops[i]->start_sample_offset,
                    loops[i]->end_sample_offset,
                    loops[i]->color.r,
                    loops[i]->color.g,
                    loops[i]->color.b,
                    loops[i]->color.a,
                });
        }
    }
}
}  // namespace

track_args snapshot_to_track(const track_snapshot& s)
{
    if (!s.relative_path)
    {
        throw invalid_track_snapshot{
            "Snapshot does not contain a populated `relative_path` field, "
            "which "
            "is required to create a track"};
    }

    track_args a;
    a.file_path = *s.relative_path;
    a.filename = basename_of(*s.relative_path);
    a.title = s.title.value_or("");
    a.artist = s.artist.value_or("");
    a.album = s.album.value_or("");
    a.genre = s.genre.value_or("");
    a.comment = s.comment.value_or("");
    a.sample_rate = s.sample_rate ? static_cast<uint32_t>(*s.sample_rate) : 0u;
    a.bitrate = s.bitrate ? static_cast<uint32_t>(*s.bitrate) : 0u;
    a.sample_count = s.sample_count ? *s.sample_count : 0ull;
    a.key = s.key ? key_to_string(*s.key) : "";

    // Performance data is built when the caller supplied a waveform OR a
    // beatgrid. rekordcrate decides ANLZ emission from non-empty
    // `waveform_detail`, so a beatgrid-only snapshot still encodes its beats if
    // paired with at least a stub waveform — but the honest contract is:
    // waveform drives ANLZ.
    if (!s.waveform.empty())
    {
        ffi_performance_data p{};
        map_waveform(s.waveform, p);
        p.bpm = s.bpm.value_or(0.0);
        p.beatgrid.reserve(s.beatgrid.size());
        for (auto& m : s.beatgrid)
        {
            p.beatgrid.push_back(
                ffi_beatgrid_marker{
                    static_cast<int32_t>(m.index), m.sample_offset});
        }
        map_cues(s.hot_cues, s.loops, p);
        p.main_cue_sample = s.main_cue.value_or(-1.0);
        a.performance = std::move(p);
    }
    else
    {
        a.performance = std::nullopt;
    }

    return a;
}

}  // namespace djinterop::rekordcrate
