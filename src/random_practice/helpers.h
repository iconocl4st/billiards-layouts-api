//
// Created by thallock on 12/31/21.
//

#ifndef IDEA_HELPERS_H
#define IDEA_HELPERS_H


#include <random>

#include "RandomPracticeParams.h"

#include "billiards_common/layouts/Layout.h"
#include "billiards_common/shots/shot_calculation/shot.h"
#include "billiards_common/shots/shot_is_possible.h"
#include "billiards_common/shots/shot_helpers.h"


namespace billiards::layout {

	[[nodiscard]] inline
	bool all_shots_are_possible(const RandomPracticeParams& params, const Layout& layout) {
		shots::ShotInfoParams info_params;
		info_params.locations = layout.locations;
		info_params.config = params.config;
		for (const auto& lshot : layout.shots) {
			info_params.shot = lshot.shot;
			shots::ShotInformation info{info_params.shot};
			bool success = shots::calculate_shot(info_params, info);
			if (!success) {
				return false;
			}
			bool possible = shots::shot_info_is_possible(
				info_params.config, layout.locations, info);
			if (!possible) {
				return false;
			}
		}
		return true;
	}


	[[nodiscard]] inline
	int add_cue(Layout& layout) {
		int cue_index = layout.locations.balls.size();
		layout.locations.balls.push_back(LocatedBall{
				vball::VirtualBall{vball::virtual_type::CUE, -1},
				geometry::Point{}});
		return cue_index;
	}

	[[nodiscard]] inline
	int add_num(Layout& layout, int num) {
		int obj_index = layout.locations.balls.size();
		layout.locations.balls.push_back(LocatedBall{
				vball::VirtualBall{vball::virtual_type::NUMBER, num},
				geometry::Point{}});
		return obj_index;
	}

	[[nodiscard]] inline
	double random_val(std::default_random_engine& engine, const double min, const double max) {
		std::uniform_real_distribution<double> dist{min, max};
		return dist(engine);
	}

	[[nodiscard]] inline
	geometry::Point random_position(std::default_random_engine& engine,
									const double min_x, const double max_x, const double min_y, const double max_y)
	{
		return geometry::Point{random_val(engine, min_x, max_x), random_val(engine, min_y, max_y)};
	}

	[[nodiscard]] inline
	geometry::Point random_position(std::default_random_engine& engine, double w, double h, const double radius) {
		return random_position(engine, radius, w - radius, radius, h - radius);
	}

	[[nodiscard]] inline
	geometry::Point random_position(std::default_random_engine& engine, const config::TableDimensions& dims, const double radius) {
		return random_position(engine, dims.width, dims.height, radius);
	}


	template <typename T> [[nodiscard]] inline
	T random_selection(std::default_random_engine& engine, const std::vector<T>& possibles) {
		if (possibles.size() == 1) {
			return possibles[0];
		}
		std::uniform_int_distribution<> dist(0, possibles.size() - 1);
		return possibles[dist(engine)];
	}

	[[nodiscard]] inline
	bool balls_intersect(
		const config::PoolConfiguration& config,
		const layout::Locations& locations,
		const int ball1, const int ball2
	) {
		const double r1 = shots::get_ball_type(config, locations, ball1)->radius;
		const double r2 = shots::get_ball_type(config, locations, ball2)->radius;
		const auto& l1 = locations.balls[ball1].location;
		const auto& l2 = locations.balls[ball2].location;
		return (l1 - l2).norm() < r1 + r2;
	}

	[[nodiscard]] inline
	bool same_direction(
		const geometry::Point& cue,
		const geometry::Point& obj,
		const config::Pocket& pocket
	) {
		const auto dir = obj - cue;
		return dir.dot(pocket.outer_segment_1 - obj) >= 0 &&
			   dir.dot(pocket.outer_segment_2 - obj) >= 0;
	}

	[[nodiscard]] inline
	bool same_direction(
			const config::Rail& rail,
			const geometry::Point& obj,
			const config::Pocket& pocket
	) {
		auto pocket_dir_1 = pocket.outer_segment_1 - obj;
		auto pocket_dir_2 = pocket.outer_segment_2 - obj;
		auto rail_dir_1 = obj - rail.segment1;
		auto rail_dir_2 = obj - rail.segment2;
		return (
			pocket_dir_1.dot(rail_dir_1) >= 0 &&
			pocket_dir_1.dot(rail_dir_2) >= 0 &&
			pocket_dir_2.dot(rail_dir_1) >= 0 &&
			pocket_dir_2.dot(rail_dir_2) >= 0
		);
	}
}
#endif //IDEA_HELPERS_H
