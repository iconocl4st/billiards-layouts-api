//
// Created by thallock on 1/1/22.
//

#ifndef IDEA_MISC_SHOTS_H
#define IDEA_MISC_SHOTS_H

#include "random_practice/practice_common.h"

namespace billiards::layout {

	inline
	void generate_bank_shot(
		const RandomPracticeParams& params,
		std::default_random_engine& engine,
		Layout& layout,
		GenResult& result
	) {
		SimpleGeneration gen{params, engine, layout, result};

		auto rail_step = std::make_shared<shots::RailStep>(0);
		auto pocket_step = std::make_shared<shots::PocketStep>(0);

		gen.shot().steps.push_back(std::make_shared<shots::CueStep>(gen.cue_index));
		gen.shot().steps.push_back(std::make_shared<shots::StrikeStep>(gen.obj_index));
		gen.shot().steps.push_back(rail_step);
		gen.shot().steps.push_back(pocket_step);

		std::uniform_int_distribution<int> bool_dist{0, 1};
		std::uniform_real_distribution<double> uniform_dist{0, 1};
		auto uniform = [&](double a, double b) {
			return a + (b - a) * uniform_dist(engine);
		};
		bool isLongRail = bool_dist(engine);

		for (int att = 0; att < MAX_ATTEMPTS; att++) {
			if (isLongRail) {
				rail_step->rail = config::constants::RIGHT_RAIL;
				if (bool_dist(engine)) {
					pocket_step->pocket = config::constants::LEFT_LOWER_POCKET;
				} else {
					pocket_step->pocket = config::constants::LEFT_UPPER_POCKET;
				}
				double percent_1 = 0.1;
				double divider = percent_1 * gen.dims().width;
				gen.obj_loc().x = uniform(std::max(gen.obj_r, divider), gen.dims().width - gen.obj_r);
				gen.obj_loc().y = uniform(gen.obj_r, gen.dims().height - gen.obj_r);

				gen.cue_loc().x = uniform(gen.cue_r, std::max(divider, 0.9 * gen.obj_loc().x));
				gen.cue_loc().y = uniform(gen.cue_r, gen.dims().height - gen.cue_r);
			} else {
				rail_step->rail = config::constants::UPPER_LEFT_RAIL;
				if (bool_dist(engine)) {
					pocket_step->pocket = config::constants::MIDDLE_LOWER_POCKET;
				} else {
					pocket_step->pocket = config::constants::LEFT_LOWER_POCKET;
				}
				double percent_1 = 0.25;
				double divider = percent_1 * gen.dims().height;
				gen.obj_loc().x = uniform(gen.obj_r, 0.5 * gen.dims().width / 2);
				gen.obj_loc().y = uniform(std::max(gen.obj_r, divider), gen.dims().height);

				gen.cue_loc().x = uniform(gen.cue_r, 0.5 * gen.dims().width / 2);
				gen.cue_loc().y = uniform(gen.cue_r, std::max(divider, 0.9 * gen.obj_loc().y));
			}

			if (balls_intersect(params.config, layout.locations, gen.cue_index, gen.obj_index)) {
				result.reason << "[intersected]";
				continue;
			}

			result.success = true;
			return;
		}

		result.reason << "[made maximum attempts]";
	}

	inline
	void generate_kick_shot(
		const RandomPracticeParams& params,
		std::default_random_engine& engine,
		Layout& layout,
		GenResult& result
	) {
		SimpleGeneration gen{params, engine, layout, result};

		auto rail_step = std::make_shared<shots::RailStep>(0);
		auto pocket_step = std::make_shared<shots::PocketStep>(0);

		gen.shot().steps.push_back(std::make_shared<shots::CueStep>(gen.cue_index));
		gen.shot().steps.push_back(rail_step);
		gen.shot().steps.push_back(std::make_shared<shots::StrikeStep>(gen.obj_index));
		gen.shot().steps.push_back(pocket_step);

		std::uniform_int_distribution<int> bool_dist{0, 1};
		std::uniform_int_distribution<int> rail_dist{0, config::constants::NUM_RAILS - 1};
		std::uniform_real_distribution<double> uniform_dist{0, 1};
		auto uniform = [&](double a, double b) {
			return a + (b - a) * uniform_dist(engine);
		};
		bool isLongRail = bool_dist(engine);
		isLongRail = false;

		const auto pockets = params.config.pockets();
		for (int att = 0; att < MAX_ATTEMPTS; att++) {
			gen.cue_loc() = random_position(engine, gen.dims(), gen.cue_r);
			gen.obj_loc() = random_position(engine, gen.dims(), gen.obj_r);
			rail_step->rail = rail_dist(engine);

			auto num_pockets = config::constants::NUM_POCKETS;
			std::vector<int> possible_pockets;
			for (int i = 0; i < pockets.size(); i++) {
				if (same_direction(
					params.config.rail(rail_step->rail),
					gen.obj_loc(),
					pockets[i]
				)) {
					possible_pockets.push_back(i);
				}
			}
			if (possible_pockets.empty()) {
				result.reason << "No possible pockets";
				continue;
			}
			pocket_step->pocket = random_selection(engine, possible_pockets);

			if (balls_intersect(params.config, layout.locations, gen.cue_index, gen.obj_index)) {
				result.reason << "[intersected]";
				continue;
			}

			result.success = true;
			return;
		}

		result.reason << "[made maximum attempts]";
	}
}

#endif //IDEA_MISC_SHOTS_H
