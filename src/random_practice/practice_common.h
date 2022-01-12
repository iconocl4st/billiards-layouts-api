//
// Created by thallock on 1/1/22.
//

#ifndef IDEA_PRACTICE_COMMON_H
#define IDEA_PRACTICE_COMMON_H

#include "random_practice/helpers.h"
#include "random_practice/RandomPracticeParams.h"
#include "random_practice/GenerationStatus.h"

#define MAX_ATTEMPTS 10

namespace billiards::layout {

	class SimpleGeneration {
	public:
		const RandomPracticeParams& params;
		std::default_random_engine& engine;
		Layout& layout;
		GenResult& result;

		int cue_index;
		int obj_index;
		unsigned long shot_index;
		std::shared_ptr<shots::PocketStep> pocket_step;
		double cue_r;
		double obj_r;

		SimpleGeneration(
			const RandomPracticeParams& params,
			std::default_random_engine& engine,
			Layout& layout,
			GenResult& result
		)
			: params{params}
			, engine{engine}
			, layout{layout}
			, result{result}
			, cue_index{add_cue(layout)}
			, obj_index{add_num(layout, 1)}
			, shot_index{layout.shots.size()}
			, pocket_step{nullptr}
			, cue_r{params.config.balls[0].radius}
			, obj_r{params.config.balls[1].radius}
		{
			layout.shots.push_back(shots::Shot{});
		}

		virtual ~SimpleGeneration() = default;

		[[nodiscard]] inline
		geometry::Point& cue_loc() {
			return layout.locations.balls[cue_index].location;
		}

		[[nodiscard]] inline
		geometry::Point& obj_loc() {
			return layout.locations.balls[obj_index].location;
		}

		[[nodiscard]] inline
		const config::TableDimensions& dims() const {
			return params.config.dims;
		}

		[[nodiscard]] inline
		shots::Shot& shot() {
			return layout.shots[shot_index].shot;
		}

		[[nodiscard]] inline
		bool validate() {
			if (balls_intersect(params.config, layout.locations, cue_index, obj_index)) {
				result.reason << "[intersected]";
				return false;
			}
			bool possible = all_shots_are_possible(params, layout);
			if (!possible) {
				result.reason << "[shot was not possible]";
				return false;
			}
			return true;
		}

		[[nodiscard]] inline
		bool assign_strike_pocket() {
			const auto num_pockets = config::constants::NUM_POCKETS;
			std::vector<int> possible_pockets{}; // num_pockets;
			const auto pockets = params.config.pockets();
			for (int i = 0; i < pockets.size(); i++) {
				if (same_direction(cue_loc(), obj_loc(), pockets[i])) {
					possible_pockets.push_back(i);
				}
			}
			if (possible_pockets.empty()) {
				result.reason << "[no pockets possible]";
				return false;
			}
			pocket_step->pocket = random_selection(engine, possible_pockets);
			return true;
		}

		inline
		void generate_strike() {
			shot().steps.push_back(std::make_shared<shots::CueStep>(cue_index));
			shot().steps.push_back(std::make_shared<shots::StrikeStep>(obj_index));
			pocket_step = std::make_shared<shots::PocketStep>(0);
			shot().steps.push_back(pocket_step);
		}
	};
}

#endif //IDEA_PRACTICE_COMMON_H
