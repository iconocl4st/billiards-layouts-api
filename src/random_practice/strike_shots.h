//
// Created by thallock on 12/31/21.
//

#ifndef IDEA_STRIKE_SHOTS_H
#define IDEA_STRIKE_SHOTS_H

#include "random_practice/practice_common.h"

namespace billiards::layout {

	inline
	void generate_uniform_simple_shot(
		const RandomPracticeParams& params,
		std::default_random_engine& engine,
		Layout& layout,
		GenResult& result
	) {
		SimpleGeneration gen{params, engine, layout, result};
		gen.generate_strike();

		for (int att = 0; att < MAX_ATTEMPTS; att++) {
			gen.cue_loc() = random_position(engine, gen.dims(), gen.cue_r);
			gen.obj_loc() = random_position(engine, gen.dims(), gen.obj_r);

			if (!gen.assign_strike_pocket()) {
				continue;
			}
			if (!gen.validate()) {
				continue;
			}
			result.success = true;
			return;
		}

		result.reason << "[made maximum attempts]";
	}

	inline
	void generate_uniform_grid_shot(
		const RandomPracticeParams& params,
		std::default_random_engine& engine,
		Layout& layout,
		GenResult& result
	) {
		int n_x_grid = 7;
		if (HAS_NUMBER(params.options, "n-grid-x")) {
			n_x_grid = params.options["n-grid-x"].get<int>();
		}
		if (n_x_grid < 1) {
			result.reason << "[number of x grid lines is too small]";
			return;
		}
		int n_y_grid = 3;
		if (HAS_NUMBER(params.options, "n-grid-y")) {
			n_y_grid = params.options["n-grid-y"].get<int>();
		}
		if (n_y_grid < 1) {
			result.reason << "[number of y grid lines is too small]";
			return;
		}
		bool include_rails = false;
		if (HAS_BOOL(params.options, "include-rails")) {
			include_rails = params.options["include-rails"].get<bool>();
		}

		int mn = include_rails ? 0 : 1;
		int mx = include_rails ? 1 : 0;
		std::uniform_int_distribution<> x_dist{mn, n_x_grid + mx};
		std::uniform_int_distribution<> y_dist{mn, n_y_grid + mx};

		SimpleGeneration gen{params, engine, layout, result};
		gen.generate_strike();

		for (int att = 0; att < MAX_ATTEMPTS; att++) {
			int cue_idx_x =  x_dist(engine);
			int cue_idx_y =  y_dist(engine);
			int obj_idx_x =  x_dist(engine);
			int obj_idx_y =  y_dist(engine);

			if (cue_idx_x == obj_idx_x && cue_idx_y == obj_idx_y) {
				result.reason << "[same grid point]";
				continue;
			}

			gen.cue_loc() = geometry::Point{
					params.config.dims.width * cue_idx_x / (double) (n_x_grid + 1),
					params.config.dims.height * cue_idx_y / (double) (n_y_grid + 1)};
			gen.obj_loc() = geometry::Point{
					params.config.dims.width * obj_idx_x / (double) (n_x_grid + 1),
					params.config.dims.height * obj_idx_y / (double) (n_y_grid + 1)};

			if (!gen.assign_strike_pocket()) {
				continue;
			}
			if (!gen.validate()) {
				continue;
			}
			result.success = true;
			return;
		}

		result.reason << "[maximum attempts]";
	}

	inline
	void generate_close_shot(
		const RandomPracticeParams& params,
		std::default_random_engine& engine,
		Layout& layout,
		GenResult& result
	) {
		SimpleGeneration gen{params, engine, layout, result};
		gen.generate_strike();

		const double min_dist = 1.1 * (gen.cue_r + gen.obj_r);
		std::uniform_real_distribution<double> radius{min_dist, 2 * min_dist};
		std::uniform_real_distribution<double> theta{-M_PI, M_PI};

		for (int att = 0; att < MAX_ATTEMPTS; att++) {
			gen.cue_loc() = random_position(engine, gen.dims(), gen.cue_r);
			const double r = radius(engine);
			const double t = theta(engine);
			gen.obj_loc() = geometry::Point{
				gen.cue_loc().x + r * std::cos(t),
				gen.cue_loc().y + r * std::sin(t),
			};
			if (gen.obj_loc().x < gen.obj_r || gen.obj_loc().x > params.config.dims.width - gen.obj_r) {
				result.reason << "[obj.x out of bounds]";
				continue;
			}
			if (gen.obj_loc().y < gen.obj_r || gen.obj_loc().y > params.config.dims.height - gen.obj_r) {
				result.reason << "[obj.y out of bounds]";
				continue;
			}

			if (!gen.assign_strike_pocket()) {
				continue;
			}
			if (!gen.validate()) {
				continue;
			}
			result.success = true;
			return;
		}

		result.reason << "[made maximum attempts]";
	}

	void generate_object_on_rail(
			const RandomPracticeParams& params,
			std::default_random_engine& engine,
			Layout& layout,
			GenResult& result
	) {
		SimpleGeneration gen{params, engine, layout, result};
		gen.generate_strike();

		const double max_dist_from_rail = gen.obj_r;
		std::uniform_real_distribution<double> dist_to_rail{0, max_dist_from_rail};
		std::uniform_int_distribution<int> bool_dist{0, 1};

		for (int att = 0; att < MAX_ATTEMPTS; att++) {
			gen.cue_loc() = random_position(engine, gen.dims(), gen.cue_r);
			gen.obj_loc() = random_position(engine, gen.dims(), gen.obj_r);
			const double d = dist_to_rail(engine);
			if (bool_dist(engine)) {
				if (bool_dist(engine)) {
					gen.obj_loc().x = gen.obj_r + d;
				} else {
					gen.obj_loc().x = gen.dims().width - gen.obj_r - d;
				}
			} else {
				if (bool_dist(engine)) {
					gen.obj_loc().y = gen.obj_r + d;
				} else {
					gen.obj_loc().y = gen.dims().height - gen.obj_r - d;
				}
			}
			if (!gen.assign_strike_pocket()) {
				continue;
			}
			if (!gen.validate()) {
				continue;
			}
			result.success = true;
			return;
		}

		result.reason << "[made maximum attempts]";
	}

	inline
	void generate_cue_on_rail(
		const RandomPracticeParams& params,
		std::default_random_engine& engine,
		Layout& layout,
		GenResult& result
	) {
		SimpleGeneration gen{params, engine, layout, result};
		gen.generate_strike();

		const double max_dist_from_rail = gen.cue_r;
		std::uniform_real_distribution<double> dist_to_rail{0, max_dist_from_rail};
		std::uniform_int_distribution<int> bool_dist{0, 1};

		for (int att = 0; att < MAX_ATTEMPTS; att++) {
			gen.cue_loc() = random_position(engine, gen.dims(), gen.cue_r);
			gen.obj_loc() = random_position(engine, gen.dims(), gen.obj_r);
			const double d = dist_to_rail(engine);
			if (bool_dist(engine)) {
				if (bool_dist(engine)) {
					gen.cue_loc().x = gen.cue_r + d;
				} else {
					gen.cue_loc().x = gen.dims().width - gen.cue_r - d;
				}
			} else {
				if (bool_dist(engine)) {
					gen.cue_loc().y = gen.cue_r + d;
				} else {
					gen.cue_loc().y = gen.dims().height - gen.cue_r - d;
				}
			}
			if (!gen.assign_strike_pocket()) {
				continue;
			}
			if (!gen.validate()) {
				continue;
			}
			result.success = true;
			return;
		}

		result.reason << "[made maximum attempts]";
	}
}

#endif //IDEA_STRIKE_SHOTS_H
