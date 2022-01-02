//
// Created by thallock on 1/1/22.
//

#ifndef IDEA_RUN_OUT_H
#define IDEA_RUN_OUT_H

#include "random_practice/practice_common.h"

namespace billiards::layout {

	inline
	void generate_run_out(
		const RandomPracticeParams& params,
		std::default_random_engine& engine,
		Layout& layout,
		GenResult& result
	) {
		int num_balls = 3;
		if (HAS_NUMBER(params.options, "num-balls")) {
			num_balls = params.options["num-balls"].get<int>();
		}
		if (num_balls < 1) {
			result.reason << "[number of balls is too small]";
			return;
		}
		if (num_balls > 15) {
			result.reason << "[number of balls is too large]";
			return;
		}
		bool half_table = false;
		if (HAS_BOOL(params.options, "half-table")) {
			half_table = params.options["half-table"].get<bool>();
		}

		std::vector<int> ball_indices;

		int cue_index = add_cue(layout);
		ball_indices.push_back(cue_index);
		std::vector<int> obj_indices;
		for (int i = 0; i < num_balls; i++) {
			int idx = add_num(layout, i + 1);
			obj_indices.push_back(idx);
			ball_indices.push_back(idx);
		}

		std::uniform_real_distribution<double> uniform_dist{0, 1};
		auto uniform = [&](double a, double b) {
			return a + (b - a) * uniform_dist(engine);
		};

		const double w = (half_table ? 0.5 : 1.0) * params.config.dims.width;
		const double h = params.config.dims.height;
		const double cue_r = shots::get_ball_type(params.config, layout.locations, cue_index)->radius;
		for (int att = 0; att < MAX_ATTEMPTS; att++) {
			layout.locations.balls[cue_index].location = geometry::Point{
				uniform(cue_r, w - cue_r),
				uniform(cue_r, h - cue_r)};
			for (const auto obj_idx : obj_indices) {
				const double radius = shots::get_ball_type(params.config, layout.locations, obj_idx)->radius;
				layout.locations.balls[obj_idx].location = geometry::Point{
					uniform(radius, w - radius),
					uniform(radius, h - radius)};
			}

			const auto num_gen = ball_indices.size();
			bool intersect = false;
			for (auto i = 0; i < num_gen && !intersect; i++) {
				for (auto j = i + 1; j < num_gen && !intersect; j++) {
					if (balls_intersect(params.config, layout.locations, ball_indices[i], ball_indices[j])) {
						result.reason << "Balls intersected" << std::endl;
						intersect = true;
					}
				}
			}
			if (intersect) {
				continue;
			}

			// Should ensure there is a shot on the one...
			result.success = true;
			return;
		}

		result.reason << "[made maximum attempts]";
	}
}

#endif //IDEA_RUN_OUT_H
