//
// Created by thallock on 9/14/21.
//

#include <random>


#include "billiards_common/3rd_party/crow_all.h"
#include "billiards_common/utils/crow_common.h"
#include "billiards_common/config/ports.h"

#include "random_practice/generate_random_practice.h"

#include "layout_manager_impl.h"
#include "LayoutsManager.h"
#include "create_random_layout.h"


int main(int argc, char **argv) {
	std::random_device ran;
	crow::SimpleApp app;

	auto manager/*_impl*/ = std::make_unique<billiards::layout::LayoutsManagerImpl>("./layouts.db");
//	manager->store.clear();
//	auto manager = std::dynamic_pointer_cast<billiards::layout::LayoutManager>(manager_impl);

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

	CROW_ROUTE(app, "/random/practice/")
		.methods("POST"_method, "OPTIONS"_method)
			([&ran](const crow::request& req) {
				if (req.method == "OPTIONS"_method) {
					HANDLE_OPTIONS;
				} else if (req.method == "POST"_method) {
					nlohmann::json value = nlohmann::json::parse(req.body);

					billiards::layout::RandomPracticeParams params;
					billiards::json::ParseResult result;
					if (HAS_OBJECT(value, "params")) {
						params.parse(value["params"], result);
					} else {
						RETURN_ERROR("No params provided");
					}
					if (!result.success) {
						RETURN_ERROR("Unable to parse params");
					}

					billiards::layout::Layout layout;
					billiards::layout::GenResult gen_result;
					billiards::layout::generate_random_practice(ran, params, layout, gen_result);
					if (!gen_result.success) {
						std::stringstream ss;
						ss << "Unable to generate practice: ";
						ss << gen_result.reason.str();
						RETURN_ERROR(ss.str());
					}
					RETURN_SUCCESS_WITH_DATA("Generated a random practice layout", "layout", layout);
				} else {
					return crow::response(404);
				}
			});

	CROW_ROUTE(app, "/layouts/")
		.methods("POST"_method, "GET"_method, "OPTIONS"_method)
		([&](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "GET"_method) {
				auto callable = [&](billiards::json::SaxWriter& writer) {
					billiards::layout::ListItemReceiver r{
						[&](const billiards::layout::ListItemRecord& record) {
							record.to_json(writer);
							return true;
						}
					};
					writer.begin_array();
					bool successful = manager->list(r);
					writer.end_array();

					if (!successful) {
						std::cerr << "Listing was not successful" << std::endl;
					}
				};
				billiards::utils::DefaultResponse def_resp{
					"Listed layouts", true, "layouts", callable};

				RETURN_RESPONSE(def_resp);
			} else if (req.method == "POST"_method) {
				billiards::layout::LayoutRecord record;

				nlohmann::json value = nlohmann::json::parse(req.body);
				if (HAS_OBJECT(value, "input")) {
					billiards::json::ParseResult status;
					record.obj.parse(value["input"], status);
					if (!status.success) {
						RETURN_ERROR("Could not parse input value");
					}
				}

				bool success = manager->create(record);
				if (!success) {
					RETURN_ERROR("Unable to create new layout");
				}

				RETURN_SUCCESS_WITH_DATA("Created a new layout", "layout", record);
			} else {
				return crow::response(404);
			}
		});

	CROW_ROUTE(app, "/layout/<str>")
		.methods("PUT"_method, "DELETE"_method, "GET"_method, "OPTIONS"_method)
		([&](const crow::request& req, const std::string& id_str) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "GET"_method) {
				auto uuid = boost::lexical_cast<boost::uuids::uuid>(id_str);

				billiards::layout::LayoutRecord record;
				bool success = manager->fetch(uuid, record);
				if (!success) {
					RETURN_ERROR("Unable to retrieve layout");
				}
				RETURN_SUCCESS_WITH_DATA("Found layout", "layout", record);
			} else if (req.method == "DELETE"_method) {
				auto uuid = boost::lexical_cast<boost::uuids::uuid>(id_str);

				bool success = manager->remove(uuid);
				if (!success) {
					RETURN_ERROR("Unable to remove layout");
				}
				RETURN_SUCCESS("Successfully removed layout");
			} else if (req.method == "PUT"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);

				if (!HAS_OBJECT(value, "updates")) {
					RETURN_ERROR("No layout updates provided");
				}

				auto uuid = boost::lexical_cast<boost::uuids::uuid>(id_str);

				billiards::layout::LayoutRecord record;
				bool success = manager->update(uuid, value["updates"], record);
				if (!success) {
					RETURN_ERROR("Unable to update layout");
				}
				RETURN_SUCCESS_WITH_DATA("Updated layout", "layout", record);
			} else {
				return crow::response(404);
			}
		});

	app.port(billiards::config::ports::LAYOUTS_API_PORT).run();
	return 0;
}
