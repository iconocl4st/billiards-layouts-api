//
// Created by thallock on 12/31/21.
//

#ifndef IDEA_GENERATIONSTATUS_H
#define IDEA_GENERATIONSTATUS_H

namespace billiards::layout {

	class GenResult {
	public:
		bool success;
		std::stringstream reason;

		GenResult() : success{false}, reason{} {}

		virtual ~GenResult() = default;
	};

}

#endif //IDEA_GENERATIONSTATUS_H
