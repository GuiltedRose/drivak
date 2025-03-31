#include "gamedb/Table.h"

void Table::insert(const std::string& id, const Row& row) {
    rows[id] = row;
}

const Row* Table::get(const std::string& id) const {
    auto it = rows.find(id);
    if (it != rows.end()) {
        return &it->second;
    }
    return nullptr;
}

void Table::set(const std::string& id, const std::string& field, const std::string& value) {
    rows[id][field] = value;
}

std::vector<std::string> Table::listIds() const {
    std::vector<std::string> ids;
    for (const auto& pair : rows) {
        ids.push_back(pair.first);
    }
    return ids;
}