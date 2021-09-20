//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_RANDOM_LAYOUT_PARAMS_H
#define GLVIEW_RANDOM_LAYOUT_PARAMS_H

#include <optional>

#include "common/shots/Layout.h"

namespace billiards::layout {

	namespace locals {
		std::random_device ran;
	}

	class RandomLayoutParams : public json::Serializable {
	public:
		// optional?
		std::optional<uint64_t> seed;
		double ball_radius;
		geometry::Dimensions dimensions;

		RandomLayoutParams()
			: seed{1776}
			, ball_radius{2.26 / 2}
			, dimensions{92, 46}
		{}

		~RandomLayoutParams() override = default;
		
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			if (seed) {
				writer.field("seed", *seed);
			}
			writer.field("ball-radius", ball_radius);
			writer.key("dimensions");
			dimensions.to_json(writer);
			writer.end_object();
		}
		
		void parse(const nlohmann::json& value) override {
			if (value.contains("seed") && value["seed"].is_number()) {
				seed = value["seed"].get<uint64_t>();
			} else {
				seed = locals::ran();
			}
			if (value.contains("ball-radius") && value["ball-radius"].is_number()) {
				ball_radius = value["ball-radius"].get<double>();
			}
			if (value.contains("dimensions") && value["dimensions"].is_object()) {
				dimensions.parse(value["dimensions"]);
			}
		}
	};
}


#endif //GLVIEW_RANDOM_LAYOUT_PARAMS_H
