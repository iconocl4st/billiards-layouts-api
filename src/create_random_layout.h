//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_GENERATE_RANDOM_SHOT_H
#define GLVIEW_GENERATE_RANDOM_SHOT_H

#include <random>

#include "./RandomPositionsParams.h"
#include "common/shots/Locations.h"

namespace billiards::layout {

	inline
	void generate_ball_locations(
		std::random_device& ran,
		const RandomPositionsParams& params,
		layout::Locations& layout
	) {
		layout.balls.clear();

		unsigned seed;
		if (params.seed) {
			seed = params.seed.value();
		} else {
			seed = ran();
		}
		std::default_random_engine engine{seed};
		
		std::uniform_real_distribution<double> xdist{
			params.ball_radius, params.dimensions.width - params.ball_radius};
		std::uniform_real_distribution<double> ydist{
			params.ball_radius, params.dimensions.height - params.ball_radius};

		for (const vball::VirtualBall& info : params.balls) {
			bool intersects;
			geometry::Point point;
			do {
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

		layout.table_dims = params.dimensions;
	}
}



#endif //GLVIEW_GENERATE_RANDOM_SHOT_H
