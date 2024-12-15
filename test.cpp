#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

#include "card.hpp" // Include your Card and Side definitions

using json = nlohmann::json;

// Test fixture class for shared setup/teardown (optional)
class CardTest : public ::testing::Test {
protected:
  Card card;
  json valid_json;

  void SetUp() override {
    // Set up a valid Card instance
    card.id = "Test Card GUID";
    card.front = {"Front Header", "Front Main", "Front Footer"};
    card.back = {"Back Header", "Back Main", "Back Footer"};

    // Set up a matching JSON representation
    valid_json = json{{"id", "Test Card GUID"},
                      {"front",
                       {{"header", "Front Header"},
                        {"main", "Front Main"},
                        {"footer", "Front Footer"}}},
                      {"back",
                       {{"header", "Back Header"},
                        {"main", "Back Main"},
                        {"footer", "Back Footer"}}}};
  }
};

TEST_F(CardTest, DeserializeValidJSON) {
  Card parsed_card = valid_json.get<Card>();
  EXPECT_EQ(parsed_card.id, card.id);
  EXPECT_EQ(parsed_card.front.header, card.front.header);
  EXPECT_EQ(parsed_card.front.main, card.front.main);
  EXPECT_EQ(parsed_card.front.footer, card.front.footer);
  EXPECT_EQ(parsed_card.back.header, card.back.header);
  EXPECT_EQ(parsed_card.back.main, card.back.main);
  EXPECT_EQ(parsed_card.back.footer, card.back.footer);
}

TEST_F(CardTest, SerializeToJSON) {
  json serialized_json = card;
  EXPECT_EQ(serialized_json, valid_json);
}

TEST_F(CardTest, RoundTripSerialization) {
  // Serialize the Card to JSON and back
  json serialized_json = card;
  Card parsed_card = serialized_json.get<Card>();

  // Ensure the parsed card matches the original
  EXPECT_EQ(parsed_card.id, card.id);
  EXPECT_EQ(parsed_card.front.header, card.front.header);
  EXPECT_EQ(parsed_card.front.main, card.front.main);
  EXPECT_EQ(parsed_card.front.footer, card.front.footer);
  EXPECT_EQ(parsed_card.back.header, card.back.header);
  EXPECT_EQ(parsed_card.back.main, card.back.main);
  EXPECT_EQ(parsed_card.back.footer, card.back.footer);
}

TEST_F(CardTest, MissingFieldInJSON) {
  // Remove a field from the valid JSON
  valid_json["front"].erase("footer");

  // Check for exception or expected fallback behavior
  EXPECT_THROW(valid_json.get<Card>(), json::out_of_range);
}

TEST_F(CardTest, ExtraFieldInJSON) {
  // Add an extra field
  valid_json["extra_field"] = "Unexpected";

  // Ensure deserialization still works (ignoring the extra field)
  Card parsed_card = valid_json.get<Card>();
  EXPECT_EQ(parsed_card.id, card.id);
}

TEST_F(CardTest, InvalidJSON) {
  // Create an invalid JSON object
  json invalid_json = {{"id", 12345}, {"front", "not_a_side"}};

  // Check for an exception during deserialization
  EXPECT_THROW(invalid_json.get<Card>(), json::type_error);
}
