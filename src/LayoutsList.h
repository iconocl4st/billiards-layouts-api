//
// Created by thallock on 10/29/21.
//

#ifndef IDEA_LAYOUTSLIST_H
#define IDEA_LAYOUTSLIST_H

#include "billiards_common/utils/Serializable.h"

namespace billiards::layouts {

    class LayoutEntry : public json::Serializable {
    public:
        boost::uuids::uuid uuid;
        std::string name;
        long lastModified;

        explicit LayoutEntry(boost::uuids::uuid uuid) : uuid{uuid} {}
        ~LayoutEntry() override = default;
    };

    class LayoutsList : public json::Serializable {
    public:
        std::map<boost::uuids::uuid, std::string> layouts;
        boost::uuids::random_generator gen;





    };
}




#endif //IDEA_LAYOUTSLIST_H
