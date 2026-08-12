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

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace djinterop::rekordcrate
{
/// A performance-data cue carried across the FFI boundary, mirroring
/// rekordcrate's `HotCueFfi`. Kept as a plain struct so `snapshot_to_track` can
/// build the vector without depending on the cxx-generated types directly.
struct ffi_hot_cue
{
    uint32_t index;  ///< Hot-cue slot 1-8 (0 = memory cue).
    std::string label;
    double sample_offset;
    uint8_t r, g, b, a;
};

/// A performance-data loop, mirroring rekordcrate's `LoopFfi`.
struct ffi_loop
{
    uint32_t index;
    std::string label;
    double start;
    double end;
    uint8_t r, g, b, a;
};

/// One 3-band waveform column at the 150 Hz detail rate.
struct ffi_waveform_entry
{
    uint8_t low, mid, high;
};

/// A sparse beatgrid anchor.
struct ffi_beatgrid_marker
{
    int32_t index;
    double sample_offset;
};

/// All format-agnostic performance data for a track, in the shape rekordcrate's
/// cxx bridge expects. `snapshot_to_track` fills this; `writer::add_track`
/// consumes it.
struct ffi_performance_data
{
    std::vector<ffi_waveform_entry> waveform_detail;
    std::vector<uint8_t> waveform_height;
    std::vector<ffi_beatgrid_marker> beatgrid;
    double bpm;  ///< <= 0 means unknown.
    std::vector<ffi_hot_cue> hot_cues;
    std::vector<ffi_loop> loops;
    double main_cue_sample;  ///< < 0 means none.
};

/// Result of adding a track: the assigned id and whether it was newly inserted
/// (false = deduped against an existing `file_path`).
struct ffi_add_track_outcome
{
    uint32_t id;
    bool is_new;
};

/// Opaque handle to a rekordcrate device-export writer. The implementation is
/// the cxx bridge generated from `rekordcrate::src/cpp.rs`; this header lets
/// the adapter compile without pulling in the cxx-generated headers directly.
/// ponytail: ceiling — the actual cxx `rekordcrate::Writer` is forward-declared
/// here and defined in `rekordcrate_writer.cpp`, which links against the
/// rekordcrate static lib. CMake↔Cargo wiring is the follow-up.
class rekordcrate_writer
{
public:
    explicit rekordcrate_writer(const std::string& directory);
    ~rekordcrate_writer();
    rekordcrate_writer(const rekordcrate_writer&) = delete;
    rekordcrate_writer& operator=(const rekordcrate_writer&) = delete;

    /// Insert (or dedup-return) a track. `file_path` is device-relative.
    ffi_add_track_outcome add_track(
        const std::string& file_path, const std::string& filename,
        const std::string& title, const std::string& artist,
        const std::string& album, const std::string& genre,
        const std::string& comment, uint32_t sample_rate, uint32_t bitrate,
        std::optional<ffi_performance_data> performance, uint64_t sample_count);

    /// Create a playlist folder under `parent_id` (0 = root). Returns its id.
    uint32_t create_playlist_folder(
        const std::string& name, uint32_t parent_id);
    /// Create a playlist under `parent_id` (0 = root). Returns its id.
    uint32_t create_playlist(const std::string& name, uint32_t parent_id);
    /// Append a track to a playlist.
    void add_track_to_playlist(uint32_t playlist_id, uint32_t track_id);

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

}  // namespace djinterop::rekordcrate
