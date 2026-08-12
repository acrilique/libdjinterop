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

#include "database_impl.hpp"

#include <utility>

#include <djinterop/exceptions.hpp>

#include "crate_impl.hpp"
#include "playlist_impl.hpp"
#include "snapshot.hpp"
#include "track_impl.hpp"

namespace djinterop::rekordcrate
{
namespace
{
/// The write-only contract: read-back is not supported. The adapter creates a
/// Rekordbox export but never reads one, matching how Mixxx primes an export.
[[noreturn]] void unsupported(const char* what)
{
    throw unsupported_operation{
        std::string{"rekordbox adapter is write-only: "} + what +
        " is not implemented"};
}
}  // namespace

track database_impl::create_track(const track_snapshot& snapshot)
{
    auto args = snapshot_to_track(snapshot);
    auto outcome = writer_->add_track(
        args.file_path, args.filename, args.title, args.artist, args.album,
        args.genre, args.comment, args.sample_rate, args.bitrate,
        args.performance, args.sample_count);
    return track{std::make_shared<track_impl>(writer_, outcome.id)};
}

crate database_impl::create_root_crate(const std::string& name)
{
    auto folder_id = writer_->create_playlist_folder(name, 0);
    auto playlist_id =
        writer_->create_playlist(name, static_cast<uint32_t>(folder_id));
    return crate{std::make_shared<crate_impl>(writer_, folder_id, playlist_id)};
}

crate database_impl::create_root_crate_after(
    const std::string& name, const crate& /*after*/)
{
    // ponytail: ceiling — "after" ordering isn't modeled; the new crate is
    // appended to the root. Upgrade path: thread the after-id into the writer.
    return create_root_crate(name);
}

playlist database_impl::create_root_playlist(const std::string& name)
{
    auto id = writer_->create_playlist(name, 0);
    return playlist{std::make_shared<playlist_impl>(writer_, id)};
}

playlist database_impl::create_root_playlist_after(
    const std::string& name, const djinterop::playlist_impl& /*after*/)
{
    return create_root_playlist(name);
}

std::string database_impl::directory()
{
    return directory_;
}
void database_impl::verify()
{ /* no SQL schema to validate */
}

std::string database_impl::uuid()
{
    // No PDB uuid is read back (write-only). Return a stable placeholder.
    return "rekordbox-export";
}

std::string database_impl::version_name()
{
    return "rekordbox";
}

// ---- Write-only: all read-back throws unsupported_operation ----

std::optional<crate> database_impl::crate_by_id(int64_t)
{
    unsupported("crate_by_id");
}
void database_impl::remove_crate(crate)
{
    unsupported("remove_crate");
}
void database_impl::remove_playlist(const djinterop::playlist_impl&)
{
    unsupported("remove_playlist");
}
void database_impl::remove_track(track)
{
    unsupported("remove_track");
}
std::vector<crate> database_impl::root_crates()
{
    unsupported("root_crates");
}
std::optional<crate> database_impl::root_crate_by_name(const std::string&)
{
    unsupported("root_crate_by_name");
}
std::vector<playlist> database_impl::root_playlists()
{
    unsupported("root_playlists");
}
std::optional<playlist> database_impl::root_playlist_by_name(const std::string&)
{
    unsupported("root_playlist_by_name");
}
std::optional<track> database_impl::track_by_id(int64_t)
{
    unsupported("track_by_id");
}
std::vector<track> database_impl::tracks()
{
    unsupported("tracks");
}
std::vector<track> database_impl::tracks_by_relative_path(const std::string&)
{
    unsupported("tracks_by_relative_path");
}

}  // namespace djinterop::rekordcrate
