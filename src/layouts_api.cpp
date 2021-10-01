//
// Created by thallock on 9/14/21.
//

#include <random>


#include "billiards_common/3rd_party/crow_all.h"
#include "billiards_common/utils/crow_common.h"
#include "billiards_common/config/ports.h"

#include "./create_random_layout.h"


int main(int argc, char **argv) {
	std::random_device ran;
	crow::SimpleApp app;

	DO_STATUS_ENDPOINT();

	CROW_ROUTE(app, "/random/locations/")
		.methods("POST"_method, "OPTIONS"_method)
		([&ran](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "POST"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);
				
				billiards::layout::RandomPositionsParams params;
				billiards::json::ParseResult result;
				if (HAS_OBJECT(value, "params")) {
					params.parse(value["params"], result);
				} else {
					RETURN_ERROR("No params provided");
				}
				if (!result.success) {
					RETURN_ERROR("Unable to parse params");
				}

				billiards::layout::Locations layout;
				billiards::layout::generate_ball_locations(ran, params, layout);
				
				RETURN_SUCCESS_WITH_DATA("Generated random locations", "locations", layout);
			} else {
				return crow::response(404);
			}
		});
	// Add graphics...

	app.port(billiards::config::ports::LAYOUTS_API_PORT).run();
	return 0;
}
