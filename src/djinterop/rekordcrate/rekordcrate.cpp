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

#include <djinterop/rekordcrate/rekordcrate.hpp>

#include <cmath>
#include <memory>
#include <stdexcept>

#include "database_impl.hpp"
#include "rekordcrate_writer.hpp"

namespace djinterop::rekordcrate
{
namespace
{
/// Detail columns per second (PWV3/PWV5/PWV7). Pinned by the ANLZ format.
/// Mirrors rekordcrate's `anlz_build::DETAIL_HZ` — pure math, intentionally
/// duplicated rather than crossing FFI for two divisions.
constexpr double DETAIL_HZ = 150.0;
/// Preview columns per second (PWAV/PWV2/PWV4/PWV6). Heuristic.
constexpr double PREVIEW_HZ = 6.667;

waveform_extents extents(
    unsigned long long sample_count, double sample_rate, double hz)
{
    if (sample_rate <= 0.0)
    {
        throw std::invalid_argument{"sample_rate must be positive"};
    }
    waveform_extents e{};
    e.samples_per_entry = sample_rate / hz;
    e.size = static_cast<unsigned long long>(
        std::round(static_cast<double>(sample_count) / e.samples_per_entry));
    return e;
}
}  // namespace

database create_database(const std::string& directory)
{
    auto writer = std::make_shared<rekordcrate_writer>(directory);
    auto impl = std::make_shared<database_impl>(std::move(writer), directory);
    return database{impl};
}

waveform_extents calculate_detail_waveform_extents(
    unsigned long long sample_count, double sample_rate)
{
    return extents(sample_count, sample_rate, DETAIL_HZ);
}

waveform_extents calculate_preview_waveform_extents(
    unsigned long long sample_count, double sample_rate)
{
    return extents(sample_count, sample_rate, PREVIEW_HZ);
}

}  // namespace djinterop::rekordcrate
