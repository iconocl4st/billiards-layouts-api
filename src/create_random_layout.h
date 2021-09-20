//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_GENERATE_RANDOM_SHOT_H
#define GLVIEW_GENERATE_RANDOM_SHOT_H

#include <random>

#include "./RandomLayoutParams.h"
#include "common/shots/Layout.h"

namespace billiards::layout {

	inline
	void create_random_layout(const RandomLayoutParams& params, layout::Layout& layout) {
		layout.balls.clear();

		std::default_random_engine engine{*params.seed};
		
		std::uniform_real_distribution<double> xdist{
			params.ball_radius, params.dimensions.width - params.ball_radius};
		std::uniform_real_distribution<double> ydist{
			params.ball_radius, params.dimensions.height - params.ball_radius};

		// Should come from the parameters...
		for (const config::BallInfo& info : config::balls::ALL_BALLS()) {
			bool intersects;
			geometry::Point point;
			do {
				// TODO: This allows balls too close to the edge...
				point = geometry::Point{xdist(engine), ydist(engine)};
				intersects = false;
				for (const auto& other : layout.balls) {
					double dist = (point - other.location).norm();
					if (dist < 2 * params.ball_radius) {
						intersects = true;
						break;
					}
				}
			} while (intersects);
			
			layout.balls.emplace_back(info, point);
		}
	}
}



#endif //GLVIEW_GENERATE_RANDOM_SHOT_H
