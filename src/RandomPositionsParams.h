//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_RANDOM_LAYOUT_PARAMS_H
#define GLVIEW_RANDOM_LAYOUT_PARAMS_H

#include <optional>

#include "common/shots/Locations.h"

namespace billiards::layout {

	class RandomPositionsParams : public json::Serializable {
	public:
		// optional?
		std::optional<uint64_t> seed;
		double ball_radius;
		geometry::Dimensions dimensions;
		std::list<vball::VirtualBall> balls;

		RandomPositionsParams()
			: seed{1776}
			, ball_radius{2.26 / 2}
			, dimensions{92, 46}
			, balls{}
		{}

		~RandomPositionsParams() override = default;
		
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			if (seed) {
				writer.field("seed", *seed);
			}
			writer.field("ball-radius", ball_radius);
			writer.key("dimensions");
			dimensions.to_json(writer);
			writer.key("balls");
			writer.begin_array();
			for (const auto& ball: balls) {
				ball.to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}
		
		void parse(const nlohmann::json& value) override {
			if (value.contains("seed") && value["seed"].is_number()) {
				seed = value["seed"].get<uint64_t>();
			} else {
				seed = {};
			}
			if (value.contains("ball-radius") && value["ball-radius"].is_number()) {
				ball_radius = value["ball-radius"].get<double>();
			}
			if (value.contains("dimensions") && value["dimensions"].is_object()) {
				dimensions.parse(value["dimensions"]);
			}
			if (value.contains("balls") && value["balls"].is_array()) {
				balls.clear();
				for (auto& it: value["balls"]) {
					balls.emplace_back();
					balls.back().parse(it);
				}
			}
		}
	};
}

#endif //GLVIEW_RANDOM_LAYOUT_PARAMS_H
