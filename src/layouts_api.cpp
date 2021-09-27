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


int main(int argc, char **argv) {
	std::random_device ran;
	crow::SimpleApp app;

	DO_STATUS_ENDPOINT();

	CROW_ROUTE(app, "/random/")
		.methods("POST"_method, "OPTIONS"_method)
		([&ran](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "POST"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);
				
				billiards::layout::RandomLayoutParams params;
				if (value.contains("params") && value["params"].is_object()) {
					params.parse(value["params"]);
				}

				billiards::layout::Layout layout;
				billiards::layout::create_random_layout(ran, params, layout);
				
				RETURN_SUCCESS_WITH_DATA("Generated random layout", "layout", layout);
			} else {
				return crow::response(404);
			}
		});
	// Add graphics...

	app.port(billiards::config::ports::LAYOUTS_API_PORT).run();
	return 0;
}
