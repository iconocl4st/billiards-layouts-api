//
// Created by thallock on 11/7/21.
//

#ifndef IDEA_LAYOUTSMANAGER_H
#define IDEA_LAYOUTSMANAGER_H

#include "billiards_common/layouts/Layout.h"
#include "billiards_common/unql/Record.h"

namespace billiards::layout {

	typedef unql::Record<Layout> LayoutRecord;
    typedef std::function<bool(const LayoutRecord& record)> LayoutReceiver;

    class LayoutManager {
    public:
        LayoutManager() = default;
        virtual ~LayoutManager() = default;

		virtual bool create(LayoutRecord& result) = 0;
        // Could have a page...
        virtual bool fetch(const boost::uuids::uuid& uuid, LayoutRecord& result) = 0;
		virtual bool update(const boost::uuids::uuid& uuid, const nlohmann::json& value, LayoutRecord& result) = 0;
		virtual bool list(LayoutReceiver& receiver) = 0;
        virtual bool remove(const boost::uuids::uuid& uuid) = 0;
    };
}



#endif //IDEA_LAYOUTSMANAGER_H
