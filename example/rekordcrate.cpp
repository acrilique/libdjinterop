// Rekordbox device-export primer.
//
// Mirrors example/engine_prime.cpp: build a track_snapshot with a 3-band
// waveform, a 2-marker beatgrid, and a hot cue, then write a Rekordbox device
// export via djinterop::rekordcrate::create_database. rekordcrate owns all ANLZ
// expansion; libdjinterop only carries format-agnostic performance data.
//
// ponytail: this example links against the rekordcrate cxx static lib via the
// adapter's rekordcrate_writer.cpp. Until the CMake↔Cargo wiring lands, it
// compiles but does not link. Run after that target exists.

#include <chrono>
#include <iostream>
#include <string>

#include <djinterop/djinterop.hpp>

namespace r = djinterop::rekordcrate;

int main()
{
    using namespace std::string_literals;

    auto dir = "PIONEER"s;
    auto db = r::create_database(dir);
    std::cout << "Created Rekordbox export in directory " << db.directory()
              << std::endl;

    djinterop::track_snapshot td;
    td.relative_path = "/Contents/01 - Some Artist - Some Song.mp3"s;
    td.title = "Some Song"s;
    td.artist = "Some Artist"s;
    td.album = "Some Album"s;
    td.genre = "House"s;
    td.bpm = 120.0;
    td.bitrate = 320;
    td.sample_rate = 44100.0;
    td.sample_count = 16140600;
    td.key = djinterop::musical_key::a_minor;
    td.duration = std::chrono::milliseconds{366000};

    // Beatgrid: two markers bracket a constant-tempo segment.
    td.beatgrid.push_back({-4, -83316.78});
    td.beatgrid.push_back({812, 17470734.439});
    td.main_cue = 2732.0;

    // Hot cue on pad 1.
    td.hot_cues.resize(8);
    td.hot_cues[0] = djinterop::hot_cue{
        "Cue 1", 1377924.5, djinterop::pad_color{0xEA, 0xC5, 0x32, 0xFF}};

    // One-vector waveform at the 150 Hz detail rate. rekordcrate downsamples to
    // ~6.667 Hz for the preview sections internally.
    auto extents =
        r::calculate_detail_waveform_extents(*td.sample_count, *td.sample_rate);
    td.waveform.reserve(extents.size);
    for (unsigned long long i = 0; i < extents.size; ++i)
    {
        td.waveform.push_back({{40, 255}, {80, 255}, {120, 255}});
    }

    auto tr = db.create_track(td);
    std::cout << "Created track id " << tr.id() << std::endl;

    auto cr = db.create_root_crate("My Crate");
    cr.add_track(tr);
    auto pl = db.create_root_playlist("My Playlist");
    pl.add_track_back(tr);

    std::cout << "Done. Export written under " << dir << "/" << std::endl;
    return 0;
}
