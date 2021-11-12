//
// Created by thallock on 11/7/21.
//

#ifndef IDEA_FILELAYOUTMANAGER_H
#define IDEA_FILELAYOUTMANAGER_H


#include <utility>
#include <set>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>

#include "LayoutsManager.h"
#include "billiards_common/utils/fs.h"
#include "billiards_common/utils/SerializableCache.h"
#include "billiards_common/utils/dump.h"

#include <filesystem>

namespace billiards::layout {

    class FileLayoutManager : public LayoutManager, json::Serializable {
        std::string directory;
        std::set<LayoutEntry> entries;
        boost::uuids::random_generator gen;

    public:
        explicit FileLayoutManager(std::string directory)
            : directory{std::move(directory)}
            , gen{}
        {}

        ~FileLayoutManager() override = default;

        void save() const {
            const std::string output_str = json::pretty_dump(*this);
            std::ofstream out_file{get_entries_path()};
            out_file << output_str;
        }

        void reload() const {
            // TODO:
        }

        void list_layouts(const LayoutEntryReceiver& receiver) override {
            /*utils::list_directory(
                directory,
                [&](const std::string& path) {
                    if (!boost::algorithm::ends_with(path, ".json")) {
                        return;
                    }

                    json::SerializableCache<Layout> layoutCache{path};
                    layoutCache.reload();

                    auto entry = layoutCache.cached.get_entry();
                    receiver(entry);
                }
            ); */

            for (const auto& entry : entries) {
                receiver(entry);
            }
        }

        std::string get_path(const boost::uuids::uuid& uuid) const {
            std::filesystem::path p{directory};

            std::stringstream filename;
            filename << boost::lexical_cast<std::string>(entry.uuid) << ".json";

            p.append(filename.str());

            return p.string();
        }

        std::string get_entries_path() const {
            const std::string output_str = json::pretty_dump(obj);
            std::filesystem::path p{directory};
            p.append("entries.json");
            return p.string();
        }

        void get_layout(const LayoutEntry& entry, const LayoutReceiver& receiver) override {
            json::SerializableCache<Layout> layoutCache{get_path(entry.uuid)};
            layoutCache.reload();

            receiver(layoutCache.cached);
        }

        LayoutEntry create_layout() override {
            const auto ret = entries.emplace(gen());
            save();
            return *ret.first;
            //while (true) {
            //    auto ret = entries.emplace(gen());
            //    if (ret.second) {
            //        return ret.first;
            //    }
            //}
        }

        bool remove_layout(const boost::uuids::uuid& uuid) override {
            //std::filesystem::remove(std::filesystem::path{get_path(uuid)});
            std::cout << "rm " << get_path(uuid) << std::enld;
            entries.erase(LayoutEntry{uuid});
            save();
            return true; // TODO
        }

        bool update_layout(const boost::uuids::uuid& uuid, const Layout& layout) override {
            const auto& it = entries.find(LayoutEntry{uuid});
            if (!it) {
                return false;
            }

            it->last_modified = std::ctime(nullptr);
            save();

            json::SerializableCache<Layout> layoutCache{get_path(entry.uuid)};
            layoutCache.cached = layout;
            layoutCache.save();

            return true;
        }

        void parse(const nlohmann::json& value, json::ParseResult& status) override {
            ENSURE_STRING(status, value, "directory", "Manager must have a directory");
            uuid = boost::lexical_cast<boost::uuids::uuid>(value["directory"].get<std::string>());
            ENSURE_ARRAY(status, value, "entries", "Manager must have entries.");
            for (const auto& e : value["entries"]) {
                LayoutEntry entry;
                entry.parse(e, status);
                if (!status.success) {
                    return;
                }

                entries.insert(entry);
            }
        }

        void to_json(json::SaxWriter& writer) const override {
            writer.begin_object();
            writer.field("directory", directory);
            writer.key("entries");
            writer.begin_array();
            for (const auto& entry : entries) {
                entry.to_json(writer);
            }
            writer.end_array();
            writer.end_object();
        }
    };

}




#endif //IDEA_FILELAYOUTMANAGER_H
