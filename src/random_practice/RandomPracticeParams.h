//
// Created by thallock on 12/31/21.
//

#ifndef IDEA_RANDOMPRACTICEGLOBALOPTIONS_H
#define IDEA_RANDOMPRACTICEGLOBALOPTIONS_H


#include "billiards_common/config/PoolConfiguration.h"

namespace billiards::layout {

	class RandomPracticeParams : public json::Serializable {
	public:
		// optional?
		std::optional<uint64_t> seed;
//		double ball_radius;
		config::PoolConfiguration config;
		std::string practice_type;
		nlohmann::json options;

		RandomPracticeParams()
			: seed{1776}
//			, ball_radius{2.26 / 2}
			, config{}
			, practice_type{"uniform-simple"}
			, options{}
		{}

		~RandomPracticeParams() override = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			if (seed) {
				writer.field("seed", *seed);
			}
//			writer.field("ball-radius", ball_radius);
			writer.key("config");
			config.to_json(writer);
			writer.key("balls");
			writer.field("practice-type", practice_type);

			// write out options...

			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			if (HAS_NUMBER(value, "seed")) {
				seed = value["seed"].get<uint64_t>();
			} else {
				seed = {};
			}
//			ENSURE_NUMBER(status, value, "ball-radius", "Must include a ball radius");
//			ball_radius = value["ball-radius"].get<double>();
			REQUIRE_CHILD(status, value, "config", config, "Must have a pool configuration");
			ENSURE_STRING(status, value, "practice-type", "Must have a practice type");
			practice_type = value["practice-type"].get<std::string>();
			if (value.contains("options")) {
				options = value["options"];
			}
		}
	};

}

#endif //IDEA_RANDOMPRACTICEGLOBALOPTIONS_H
