//
// Created by thallock on 9/14/21.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <random>


#include "common/3rd_party/crow_all.h"
#include "common/utils/crow_common.h"
#include "common/config/ports.h"

#include "./create_random_layout.h"


namespace locals {
	std::default_random_engine engine;
}






int main(int argc, char **argv) {
	crow::SimpleApp app;

	CROW_ROUTE(app, "/random/")
		.methods("POST"_method, "OPTIONS"_method)
		([](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "POST"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);
				
				billiards::layout::RandomLayoutParams params;
				if (value.contains("params")) {
					params.parse(value["params"]);
				}
				
				billiards::config::TableDimensions dims;
				if (value.contains("dims")) {
					dims.parse(value["dims"]);
				}
				
				billiards::layout::TableLayout layout;
				billiards::layout::create_random_layout(
					locals::engine, params, dims, layout);
				
				RETURN_SUCCESS_WITH_DATA("Generated random layout", layout);
			} else {
				return crow::response(404);
			}
		});
	// Add graphics...

	app.port(billiards::config::ports::LAYOUTS_API_PORT).run();
	return 0;
}
