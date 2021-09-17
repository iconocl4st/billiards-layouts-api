//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_RANDOM_LAYOUT_PARAMS_H
#define GLVIEW_RANDOM_LAYOUT_PARAMS_H

#include "common/shots/TableLayout.h"

namespace billiards::layout {

	class RandomLayoutParams : public json::Serializable {
	public:
		int seed;
		RandomLayoutParams() = default;
		virtual ~RandomLayoutParams() = default;
		
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("seed", seed);
			writer.end_object();
		}
		
		void parse(const nlohmann::json& value) override {
			// TODO
		}
	};
}


#endif //GLVIEW_RANDOM_LAYOUT_PARAMS_H
