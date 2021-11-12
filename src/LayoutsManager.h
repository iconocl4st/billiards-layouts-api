//
// Created by thallock on 11/7/21.
//

#ifndef IDEA_LAYOUTSMANAGER_H
#define IDEA_LAYOUTSMANAGER_H

#include "billiards_common/layouts/Layout.h"
#include "billiards_common/layouts/LayoutListItem.h"
#include "billiards_common/unql/Record.h"

namespace billiards::layout {

	typedef unql::Record<Layout> LayoutRecord;
	typedef unql::Record<LayoutListItem> ListItemRecord;
    typedef std::function<bool(const ListItemRecord& record)> ListItemReceiver;

    class LayoutManager {
    public:
        LayoutManager() = default;
        virtual ~LayoutManager() = default;

		virtual bool create(LayoutRecord& result) = 0;
        // Could have a page...
        virtual bool fetch(const boost::uuids::uuid& uuid, LayoutRecord& result) = 0;
		virtual bool update(const boost::uuids::uuid& uuid, const nlohmann::json& value, LayoutRecord& result) = 0;
		virtual bool list(ListItemReceiver& receiver) = 0;
        virtual bool remove(const boost::uuids::uuid& uuid) = 0;
    };
}



#endif //IDEA_LAYOUTSMANAGER_H
