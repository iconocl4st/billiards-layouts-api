//
// Created by thallock on 12/31/21.
//

#ifndef IDEA_GENERATE_RANDOM_PRACTICE_H
#define IDEA_GENERATE_RANDOM_PRACTICE_H

#include "random_practice/strike_shots.h"
#include "random_practice/misc_shots.h"
#include "random_practice/run_out.h"

namespace billiards::layout {

	void generate_random_practice(
		std::random_device& ran,
		const RandomPracticeParams& params,
		Layout& layout,
		GenResult& result
	) {
		layout.name = "randomly-generated"; // TODO: add the seed....
		layout.locations.table_dims = params.config.dims;

		unsigned seed;
		if (params.seed) {
			seed = params.seed.value();
		} else {
			seed = ran();
		}
		std::default_random_engine engine{seed};

		if (params.practice_type == "uniform-simple") {
			generate_uniform_simple_shot(params, engine, layout, result);
		} else if (params.practice_type == "grid-simple") {
			generate_uniform_grid_shot(params, engine, layout, result);
		} else if (params.practice_type == "short-shot") {
			generate_close_shot(params, engine, layout, result);
		} else if (params.practice_type == "obj-on-rail") {
			generate_object_on_rail(params, engine, layout, result);
		} else if (params.practice_type == "cue-on-rail") {
			generate_cue_on_rail(params, engine, layout, result);
		} else if (params.practice_type == "bank") {
			generate_bank_shot(params, engine, layout, result);
		} else if (params.practice_type == "kick") {
			generate_kick_shot(params, engine, layout, result);
		} else if (params.practice_type == "run-out") {
			generate_run_out(params, engine, layout, result);
		} else if (params.practice_type == "combo") {

		} else if (params.practice_type == "kiss") {

		} else if (params.practice_type == "carom") {

		} else if (params.practice_type == "snookered") {

		} else if (params.practice_type == "snooker") {

		} else {
			result.reason << "No practice type " << params.practice_type;
		}
	}
}



#endif //IDEA_GENERATE_RANDOM_PRACTICE_H
