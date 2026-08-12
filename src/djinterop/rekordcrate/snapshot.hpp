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
#include <optional>
#include <string>

#include "rekordcrate_writer.hpp"

namespace djinterop
{
struct track_snapshot;
}

namespace djinterop::rekordcrate
{
/// The arguments `rekordcrate_writer::add_track` needs, derived from a
/// `track_snapshot` by [`snapshot_to_track`]. A plain struct so the
/// database/track impls can call the writer without re-deriving fields.
struct track_args
{
    std::string file_path;
    std::string filename;
    std::string title;
    std::string artist;
    std::string album;
    std::string genre;
    std::string comment;
    std::string key;
    uint32_t sample_rate = 0;
    uint32_t bitrate = 0;
    uint64_t sample_count = 0;
    std::optional<ffi_performance_data> performance;
};

/// Transform a format-agnostic `track_snapshot` into the args the rekordcrate
/// writer consumes. `relative_path` is mandatory; throws
/// `invalid_track_snapshot` if absent (mirrors Engine v2's `snapshot_to_row`).
/// Performance data (waveform, beatgrid, cues, loops) is carried across
/// verbatim — rekordcrate owns all ANLZ expansion.
track_args snapshot_to_track(const track_snapshot& s);

}  // namespace djinterop::rekordcrate
