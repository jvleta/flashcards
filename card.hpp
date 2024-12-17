#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

// Side struct updated to use a vector of strings for flexible line entries
struct Side {
  std::vector<std::string> lines;
};

struct Card {
  std::string id;
  Side front;
  Side back;
};

// Serialization and deserialization for Side
void from_json(const json &j, Side &s) { j.at("lines").get_to(s.lines); }

void to_json(json &j, const Side &s) { j = json{{"lines", s.lines}}; }

// Serialization and deserialization for Card
void from_json(const json &j, Card &c) {
  j.at("id").get_to(c.id);
  j.at("front").get_to(c.front);
  j.at("back").get_to(c.back);
}

void to_json(json &j, const Card &c) {
  j = json{{"id", c.id}, {"front", c.front}, {"back", c.back}};
}
