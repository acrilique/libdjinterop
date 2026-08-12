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
#ifndef DJINTEROP_REKORDCRATE_REKORDCRATE_HPP
#define DJINTEROP_REKORDCRATE_REKORDCRATE_HPP

#include <string>

#include <djinterop/config.hpp>
#include <djinterop/database.hpp>
#include <djinterop/performance_data.hpp>

namespace djinterop
{
class database;

namespace rekordcrate
{
/// Default top-level directory name for a device export.
constexpr const char* default_database_dir_name = "PIONEER";

/// Create a new, empty Rekordbox device export at `directory` (a folder that
/// will gain a `PIONEER/` subtree). Write-only: the returned database can
/// create tracks, crates, and playlists, but cannot read them back. Callers
/// that need read-back should open the export directly via the rekordcrate
/// reader.
///
/// `directory` is the *device root* — the parent of `PIONEER/`, matching how
/// the PDB layout expects paths. `track_snapshot::relative_path` becomes the
/// device-relative `file_path` (e.g. `/Contents/Artist - Title.mp3`).
DJINTEROP_PUBLIC database create_database(const std::string& directory);

/// Detail-waveform extents at 150 Hz (PWV3/PWV5/PWV7). Pure math, no FFI:
/// reuses the same rate as rekordcrate's `anlz_build::DETAIL_HZ`.
DJINTEROP_PUBLIC waveform_extents calculate_detail_waveform_extents(
    unsigned long long sample_count, double sample_rate);

/// Preview-waveform extents at ~6.667 Hz (PWAV/PWV2/PWV4/PWV6). Pure math.
DJINTEROP_PUBLIC waveform_extents calculate_preview_waveform_extents(
    unsigned long long sample_count, double sample_rate);

}  // namespace rekordcrate

}  // namespace djinterop

#endif  // DJINTEROP_REKORDCRATE_REKORDCRATE_HPP
