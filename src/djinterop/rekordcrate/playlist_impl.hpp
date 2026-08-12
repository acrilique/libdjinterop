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
#include <optional>

#include <djinterop/database.hpp>
#include <djinterop/track.hpp>

#include "../impl/playlist_impl.hpp"
#include "rekordcrate_writer.hpp"

namespace djinterop::rekordcrate
{
/// Write-only playlist handle. `add_track_back` delegates; the rest throw.
/// `id()` is stored here (the base has no id accessor for playlists).
class playlist_impl : public djinterop::playlist_impl
{
public:
    playlist_impl(std::shared_ptr<rekordcrate_writer> writer, int64_t id) :
        writer_{std::move(writer)}, id_{id}
    {
    }

    int64_t id() const noexcept { return id_; }

    void add_track_back(const djinterop::track_impl& tr) override;
    void add_track_after(
        const djinterop::track_impl& tr,
        const djinterop::track_impl& after) override;
    std::vector<playlist> children() override;
    void clear_tracks() override;
    playlist create_sub_playlist(const std::string& name) override;
    playlist create_sub_playlist_after(
        const std::string& name,
        const djinterop::playlist_impl& after) override;
    database db() const override;
    std::string name() const override;
    std::optional<playlist> parent() override;
    void remove_track(const djinterop::track_impl& tr) override;
    void set_name(const std::string& name) override;
    void set_parent(const djinterop::playlist_impl* parent_maybe) override;
    std::optional<playlist> sub_playlist_by_name(
        const std::string& name) override;
    std::vector<track> tracks() const override;

    bool operator==(
        const djinterop::playlist_impl& other) const noexcept override
    {
        // Cross-type compare only resolves for sibling rekordbox playlists;
        // other adapters compare unequal (no shared id space).
        const auto* o =
            dynamic_cast<const playlist_impl*>(std::addressof(other));
        return o != nullptr && id_ == o->id_;
    }

private:
    std::shared_ptr<rekordcrate_writer> writer_;
    int64_t id_;
};

}  // namespace djinterop::rekordcrate
