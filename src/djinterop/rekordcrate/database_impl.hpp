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

#include <memory>
#include <string>

#include "../impl/database_impl.hpp"
#include "rekordcrate_writer.hpp"

namespace djinterop::rekordcrate
{
class database_impl : public djinterop::database_impl
{
public:
    database_impl(
        std::shared_ptr<rekordcrate_writer> writer, std::string directory) :
        djinterop::database_impl{
            {feature::supports_nested_playlists,
             feature::playlists_and_crates_are_distinct}},
        writer_{std::move(writer)}, directory_{std::move(directory)}
    {
    }

    std::optional<crate> crate_by_id(int64_t id) override;
    playlist create_root_playlist(const std::string& name) override;
    playlist create_root_playlist_after(
        const std::string& name,
        const djinterop::playlist_impl& after) override;
    crate create_root_crate(const std::string& name) override;
    crate create_root_crate_after(
        const std::string& name, const crate& after) override;
    track create_track(const track_snapshot& snapshot) override;
    std::string directory() override;
    void verify() override;
    void remove_crate(crate cr) override;
    void remove_playlist(const playlist_impl& pl) override;
    void remove_track(track tr) override;
    std::vector<crate> root_crates() override;
    std::optional<crate> root_crate_by_name(const std::string& name) override;
    std::vector<playlist> root_playlists() override;
    std::optional<playlist> root_playlist_by_name(
        const std::string& name) override;
    std::optional<track> track_by_id(int64_t id) override;
    std::vector<track> tracks() override;
    std::vector<track> tracks_by_relative_path(
        const std::string& relative_path) override;
    std::string uuid() override;
    std::string version_name() override;

    /// Accessor used by track/crate/playlist impls to reach the writer.
    std::shared_ptr<rekordcrate_writer> writer() const noexcept
    {
        return writer_;
    }

private:
    std::shared_ptr<rekordcrate_writer> writer_;
    std::string directory_;
    /// Next id handed to track_impl handles. rekordcrate assigns its own ids;
    /// we mirror the last returned one so the handle's `id()` is stable.
    int64_t next_track_id_ = 1;
    int64_t next_playlist_id_ = 1;
};

}  // namespace djinterop::rekordcrate
