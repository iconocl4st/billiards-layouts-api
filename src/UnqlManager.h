
//
// Created by thallock on 11/7/21.
//

#ifndef IDEA_UNQLMANAGER_H
#define IDEA_UNQLMANAGER_H


#include "LayoutsManager.h"
#include "billiards_common/unql/Store.h"

namespace billiards::layout {

	class UnqlLayoutManager : public LayoutManager {
	public:
		unql::UnqlStore<Layout> store;

		explicit UnqlLayoutManager(const std::string& db_file) : store{db_file} {}

		~UnqlLayoutManager() override = default;

		bool create(LayoutRecord& result) override {
			return store.create(result);
		}
		bool fetch(const boost::uuids::uuid& uuid, LayoutRecord& result) override {
			return store.fetch(uuid, result);
		}
		bool update(const boost::uuids::uuid& uuid, const nlohmann::json& updates, LayoutRecord& result) override {
			return store.update(uuid, updates, result);
		}
		bool list(LayoutReceiver& receiver) override {
			return store.list(receiver);
		}
		bool remove(const boost::uuids::uuid& uuid) override {
			return store.remove(uuid);
		}
	};
}

#endif //IDEA_UNQLMANAGER_H
