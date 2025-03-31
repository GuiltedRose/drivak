#pragma once
#include <string>
#include <unordered_map>
#include <vector>

using Row = std::unordered_map<std::string, std::string>;

class Table {
public:
    void insert(const std::string& id, const Row& row);
    const Row* get(const std::string& id) const;
    void set(const std::string& id, const std::string& field, const std::string& value);
    std::vector<std::string> listIds() const;
private:
    std::unordered_map<std::string, Row> rows;
};