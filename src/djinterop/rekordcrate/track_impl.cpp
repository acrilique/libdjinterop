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

#include "track_impl.hpp"

#include <djinterop/crate.hpp>
#include <djinterop/database.hpp>
#include <djinterop/exceptions.hpp>
#include <djinterop/track_snapshot.hpp>

#include "snapshot.hpp"

namespace djinterop::rekordcrate
{
namespace
{
[[noreturn]] void unsupported(const char* what)
{
    throw unsupported_operation{
        std::string{"rekordbox adapter is write-only: "} + what +
        " is not implemented"};
}
}  // namespace

track_snapshot track_impl::snapshot() const
{
    unsupported("track::snapshot");
}

void track_impl::update(const track_snapshot& snapshot)
{
    // Re-derive and re-add. rekordcrate dedups on file_path, so this is
    // idempotent when only mutable fields changed; a path change inserts a new
    // track.
    auto args = snapshot_to_track(snapshot);
    writer_->add_track(
        args.file_path, args.filename, args.title, args.artist, args.album,
        args.genre, args.comment, args.sample_rate, args.bitrate,
        args.performance, args.sample_count);
}

// ---- All accessors/setters: write-only contract ----

std::optional<std::string> track_impl::album()
{
    unsupported("album");
}
void track_impl::set_album(std::optional<std::string>)
{
    unsupported("set_album");
}
std::optional<std::string> track_impl::artist()
{
    unsupported("artist");
}
void track_impl::set_artist(std::optional<std::string>)
{
    unsupported("set_artist");
}
std::optional<double> track_impl::average_loudness()
{
    unsupported("average_loudness");
}
void track_impl::set_average_loudness(std::optional<double>)
{
    unsupported("set_average_loudness");
}
std::vector<beatgrid_marker> track_impl::beatgrid()
{
    unsupported("beatgrid");
}
void track_impl::set_beatgrid(std::vector<beatgrid_marker>)
{
    unsupported("set_beatgrid");
}
std::optional<int> track_impl::bitrate()
{
    unsupported("bitrate");
}
void track_impl::set_bitrate(std::optional<int>)
{
    unsupported("set_bitrate");
}
std::optional<double> track_impl::bpm()
{
    unsupported("bpm");
}
void track_impl::set_bpm(std::optional<double>)
{
    unsupported("set_bpm");
}
std::optional<std::string> track_impl::comment()
{
    unsupported("comment");
}
void track_impl::set_comment(std::optional<std::string>)
{
    unsupported("set_comment");
}
std::optional<std::string> track_impl::composer()
{
    unsupported("composer");
}
void track_impl::set_composer(std::optional<std::string>)
{
    unsupported("set_composer");
}
std::vector<crate> track_impl::containing_crates()
{
    unsupported("containing_crates");
}
database track_impl::db()
{
    unsupported("db");
}
std::optional<std::chrono::milliseconds> track_impl::duration()
{
    unsupported("duration");
}
void track_impl::set_duration(std::optional<std::chrono::milliseconds>)
{
    unsupported("set_duration");
}
std::string track_impl::file_extension()
{
    unsupported("file_extension");
}
std::string track_impl::filename()
{
    unsupported("filename");
}
std::optional<std::string> track_impl::genre()
{
    unsupported("genre");
}
void track_impl::set_genre(std::optional<std::string>)
{
    unsupported("set_genre");
}
std::optional<hot_cue> track_impl::hot_cue_at(int)
{
    unsupported("hot_cue_at");
}
void track_impl::set_hot_cue_at(int, std::optional<hot_cue>)
{
    unsupported("set_hot_cue_at");
}
std::vector<std::optional<hot_cue>> track_impl::hot_cues()
{
    unsupported("hot_cues");
}
void track_impl::set_hot_cues(std::vector<std::optional<hot_cue>>)
{
    unsupported("set_hot_cues");
}
bool track_impl::is_valid()
{
    unsupported("is_valid");
}
std::optional<musical_key> track_impl::key()
{
    unsupported("key");
}
void track_impl::set_key(std::optional<musical_key>)
{
    unsupported("set_key");
}
std::optional<std::chrono::system_clock::time_point>
track_impl::last_played_at()
{
    unsupported("last_played_at");
}
void track_impl::set_last_played_at(
    std::optional<std::chrono::system_clock::time_point>)
{
    unsupported("set_last_played_at");
}
std::optional<loop> track_impl::loop_at(int)
{
    unsupported("loop_at");
}
void track_impl::set_loop_at(int, std::optional<loop>)
{
    unsupported("set_loop_at");
}
std::vector<std::optional<loop>> track_impl::loops()
{
    unsupported("loops");
}
void track_impl::set_loops(std::vector<std::optional<loop>>)
{
    unsupported("set_loops");
}
std::optional<double> track_impl::main_cue()
{
    unsupported("main_cue");
}
void track_impl::set_main_cue(std::optional<double>)
{
    unsupported("set_main_cue");
}
std::optional<std::string> track_impl::publisher()
{
    unsupported("publisher");
}
void track_impl::set_publisher(std::optional<std::string>)
{
    unsupported("set_publisher");
}
std::optional<int> track_impl::rating()
{
    unsupported("rating");
}
void track_impl::set_rating(std::optional<int>)
{
    unsupported("set_rating");
}
std::string track_impl::relative_path()
{
    unsupported("relative_path");
}
void track_impl::set_relative_path(std::string)
{
    unsupported("set_relative_path");
}
std::optional<unsigned long long> track_impl::sample_count()
{
    unsupported("sample_count");
}
void track_impl::set_sample_count(std::optional<unsigned long long>)
{
    unsupported("set_sample_count");
}
std::optional<double> track_impl::sample_rate()
{
    unsupported("sample_rate");
}
void track_impl::set_sample_rate(std::optional<double>)
{
    unsupported("set_sample_rate");
}
std::optional<std::string> track_impl::title()
{
    unsupported("title");
}
void track_impl::set_title(std::optional<std::string>)
{
    unsupported("set_title");
}
std::optional<int> track_impl::track_number()
{
    unsupported("track_number");
}
void track_impl::set_track_number(std::optional<int>)
{
    unsupported("set_track_number");
}
std::vector<waveform_entry> track_impl::waveform()
{
    unsupported("waveform");
}
void track_impl::set_waveform(std::vector<waveform_entry>)
{
    unsupported("set_waveform");
}
std::optional<int> track_impl::year()
{
    unsupported("year");
}
void track_impl::set_year(std::optional<int>)
{
    unsupported("set_year");
}

}  // namespace djinterop::rekordcrate
