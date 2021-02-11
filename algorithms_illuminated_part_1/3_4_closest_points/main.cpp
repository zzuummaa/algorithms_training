#include <iostream>
#include <optional>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>

struct point;
typedef std::pair<point, point> point_pair;

struct point {
	float x;
	float y;

	[[nodiscard]] float dist_square(const point& other) const {
		return std::pow(this->x - other.x, 2.f) + std::pow(this->y - other.y, 2.f);
	}

	static float dist_square(const point_pair& pair) {
		return pair.first.dist_square(pair.second);
	}

	static float dist_square(const std::optional<point_pair>& pair) {
		return std::numeric_limits<typeof(point::x)>::max();
	}
};

template<typename RandomAccessIterator>
std::optional<point_pair> closest_split_pair(RandomAccessIterator first_y,
											 RandomAccessIterator last_y,
											 const point& median,
											 typeof(point::x) nearest_dist) {

	std::vector<point> filtered_y;
	std::copy_if(first_y, last_y, filtered_y.begin(), [&](auto p){ return std::abs(p.x - median.x) < nearest_dist; });

	std::optional<point_pair> nearest_pair = std::nullopt;
	for (auto it_p1 = first_y; it_p1 < last_y; it_p1++) {
		for (auto it_p2 = it_p1 + 1; it_p2 < std::min(it_p1 + 7, last_y); it_p2++) {
			auto dist = it_p1->dist_square(*it_p2);
			if (dist < nearest_dist) {
				nearest_dist = dist;
				nearest_pair = std::make_pair(*it_p1, *it_p2);
			}
		}
	}

	return nearest_pair;
}

template<typename RandomAccessIterator>
std::optional<point_pair> closest_pair_internal(RandomAccessIterator first_x,
												RandomAccessIterator last_x,
												RandomAccessIterator first_y) {

	if (last_x - first_x < 2) return std::nullopt;
	if (last_x - first_x < 3) return std::make_pair(*first_x, *(first_x + 1));

	auto half_size = (last_x - first_x) / 2;
	auto left_nears = closest_pair_internal(first_x, first_x + half_size, first_y).value();
	auto right_nears = closest_pair_internal(first_x + half_size, last_x, first_y + half_size).value();

	auto left_nearest_dist = point::dist_square(left_nears);
	auto right_nearest_dist = point::dist_square(right_nears);

	typeof(point::x) nearest_dist;
	point_pair nears_points;
	if (left_nearest_dist < right_nearest_dist) {
		nearest_dist = left_nearest_dist;
		nears_points = left_nears;
	} else {
		nearest_dist = right_nearest_dist;
		nears_points = right_nears;
	}

	const auto& median = *(first_x + half_size);
	auto split_nears = closest_split_pair(first_y, first_y + (last_x - first_x), median, nearest_dist);

	return point::dist_square(split_nears) < nearest_dist ? split_nears : nears_points;
}

template<typename RandomAccessIterator>
std::optional<point_pair> closest_pair(RandomAccessIterator first, RandomAccessIterator last) {
	std::vector<point> sorted_x(first, last);
	std::vector<point> sorted_y(first, last);

	std::sort(sorted_x.begin(), sorted_x.end(), [](auto a, auto b){ return a.x < b.x; });
	std::sort(sorted_y.begin(), sorted_y.end(), [](auto a, auto b){ return a.y < b.y; });

	return closest_pair_internal(sorted_x.begin(), sorted_x.end(), sorted_y.begin());
}

int main() {
	std::vector<point> a;
	closest_pair(a.begin(), a.end());
	return 0;
}
