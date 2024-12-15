#include <nlohmann/json.hpp> // Include the nlohmann/json header

using json = nlohmann::json;

struct Side {
  std::string header;
  std::string main;
  std::string footer;
};

struct Card {
  std::string id;
  Side front{};
  Side back{};
};

void from_json(const json &j, Side &p) {
  j.at("header").get_to(p.header);
  j.at("main").get_to(p.main);
  j.at("footer").get_to(p.footer);
}

void to_json(json &j, const Side &p) {
  j = json{{"header", p.header}, {"main", p.main}, {"footer", p.footer}};
}

void from_json(const json &j, Card &c) {
  j.at("id").get_to(c.id);
  j.at("front").get_to(c.front); // Call from_json for Side
  j.at("back").get_to(c.back);   // Call from_json for Side
}

void to_json(json &j, const Card &c) {
  j = json{
      {"id", c.id},
      {"front", c.front}, // Call to_json for Side
      {"back", c.back}    // Call to_json for Side
  };
}
