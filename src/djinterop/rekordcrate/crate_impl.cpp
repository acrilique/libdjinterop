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

#include "crate_impl.hpp"

#include <djinterop/exceptions.hpp>

namespace djinterop::rekordcrate
{
namespace
{
[[noreturn]] void unsupported(const char* what)
{
    throw unsupported_operation{
        std::string{"rekordbox crate is write-only: "} + what +
        " is not implemented"};
}
}  // namespace

void crate_impl::add_track(int64_t track_id)
{
    // Tracks attach to the crate's child leaf playlist, not the folder itself —
    // Rekordbox folders can only hold child nodes, never tracks.
    writer_->add_track_to_playlist(
        static_cast<uint32_t>(playlist_id_), static_cast<uint32_t>(track_id));
}

void crate_impl::add_track(track tr)
{
    add_track(tr.id());
}

crate crate_impl::create_sub_crate(const std::string& name)
{
    auto folder_id = writer_->create_playlist_folder(
        name, static_cast<uint32_t>(this->id()));
    auto playlist_id =
        writer_->create_playlist(name, static_cast<uint32_t>(folder_id));
    return crate{std::make_shared<crate_impl>(writer_, folder_id, playlist_id)};
}

crate crate_impl::create_sub_crate_after(
    const std::string& name, const crate& /*after*/)
{
    // ponytail: "after" ordering not modeled; appended to parent.
    return create_sub_crate(name);
}

// ---- Write-only: read-back unsupported ----

std::vector<crate> crate_impl::children()
{
    unsupported("children");
}
void crate_impl::clear_tracks()
{
    unsupported("clear_tracks");
}
database crate_impl::db()
{
    unsupported("db");
}
std::vector<crate> crate_impl::descendants()
{
    unsupported("descendants");
}
bool crate_impl::is_valid()
{
    unsupported("is_valid");
}
std::string crate_impl::name()
{
    unsupported("name");
}
std::optional<crate> crate_impl::parent()
{
    unsupported("parent");
}
void crate_impl::remove_track(track)
{
    unsupported("remove_track");
}
std::optional<crate> crate_impl::sub_crate_by_name(const std::string&)
{
    unsupported("sub_crate_by_name");
}
void crate_impl::set_name(std::string)
{
    unsupported("set_name");
}
void crate_impl::set_parent(std::optional<crate>)
{
    unsupported("set_parent");
}
std::vector<track> crate_impl::tracks()
{
    unsupported("tracks");
}

}  // namespace djinterop::rekordcrate
