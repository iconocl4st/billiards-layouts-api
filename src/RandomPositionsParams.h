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
		
		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			if (HAS_NUMBER(value, "seed")) {
				seed = value["seed"].get<uint64_t>();
			} else {
				seed = {};
			}
			ENSURE_NUMBER(status, value, "ball-radius", "Must include a ball radius");
			ball_radius = value["ball-radius"].get<double>();
			REQUIRE_CHILD(status, value, "dimensions", dimensions, "must include dimensions");
			ENSURE_ARRAY(status, value, "balls", "must include balls");
			balls.clear();
			for (auto& it: value["balls"]) {
				balls.emplace_back();
				balls.back().parse(it, status);
				if (!status.success) {
					return;
				}
			}
		}
	};
}

#endif //GLVIEW_RANDOM_LAYOUT_PARAMS_H
