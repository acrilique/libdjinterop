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

#include "playlist_impl.hpp"

#include <djinterop/exceptions.hpp>

#include "../impl/track_impl.hpp"  // full definition for tr.id()

namespace djinterop::rekordcrate
{
namespace
{
[[noreturn]] void unsupported(const char* what)
{
    throw unsupported_operation{
        std::string{"rekordbox playlist is write-only: "} + what +
        " is not implemented"};
}
}  // namespace

void playlist_impl::add_track_back(const djinterop::track_impl& tr)
{
    writer_->add_track_to_playlist(
        static_cast<uint32_t>(id_), static_cast<uint32_t>(tr.id()));
}

void playlist_impl::add_track_after(
    const djinterop::track_impl& tr, const djinterop::track_impl& /*after*/)
{
    // ponytail: "after" ordering not modeled; appended to end.
    add_track_back(tr);
}

playlist playlist_impl::create_sub_playlist(const std::string& name)
{
    auto id = writer_->create_playlist(name, static_cast<uint32_t>(id_));
    return playlist{std::make_shared<playlist_impl>(writer_, id)};
}

playlist playlist_impl::create_sub_playlist_after(
    const std::string& name, const djinterop::playlist_impl& /*after*/)
{
    return create_sub_playlist(name);
}

// ---- Write-only: read-back unsupported ----

std::vector<playlist> playlist_impl::children()
{
    unsupported("children");
}
void playlist_impl::clear_tracks()
{
    unsupported("clear_tracks");
}
database playlist_impl::db() const
{
    unsupported("db");
}
std::string playlist_impl::name() const
{
    unsupported("name");
}
std::optional<playlist> playlist_impl::parent()
{
    unsupported("parent");
}
void playlist_impl::remove_track(const djinterop::track_impl&)
{
    unsupported("remove_track");
}
void playlist_impl::set_name(const std::string&)
{
    unsupported("set_name");
}
void playlist_impl::set_parent(const djinterop::playlist_impl*)
{
    unsupported("set_parent");
}
std::optional<playlist> playlist_impl::sub_playlist_by_name(const std::string&)
{
    unsupported("sub_playlist_by_name");
}
std::vector<track> playlist_impl::tracks() const
{
    unsupported("tracks");
}

}  // namespace djinterop::rekordcrate
