//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_GENERATE_RANDOM_SHOT_H
#define GLVIEW_GENERATE_RANDOM_SHOT_H

#include <random>

#include "./RandomLayoutParams.h"
#include "common/shots/TableLayout.h"
#include "common/config/TableDimensions.h"

namespace billiards::layout {

	inline
	void create_random_layout(
		std::default_random_engine& engine,
		const RandomLayoutParams& params,
		const config::TableDimensions& dims,
		layout::TableLayout& layout
	) {
		layout.balls.clear();
		
		// Should come from somewhere
		double BALL_DIAM = 2.26;

		std::uniform_real_distribution<double> dist{0.0, 1.0};
		for (const config::BallInfo& info : config::balls::ALL_BALLS()) {
			bool intersects;
			geometry::Point point;
			do {
				// TODO: This allows balls too close to the edge...
				point = geometry::Point{dims.width * dist(engine), dims.height * dist(engine)};
				intersects = false;
				for (const auto& other : layout.balls) {
					double dist = (point - other.location).norm();
					if (dist < BALL_DIAM) {
						intersects = true;
					}
				}
			} while (intersects);
			
			layout.balls.push_back(layout::LocatedBall{info, point});
		}
	}
}



#endif //GLVIEW_GENERATE_RANDOM_SHOT_H
