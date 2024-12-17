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
    card.front.lines = {"Front Line 1", "Front Line 2", "Front Line 3"};
    card.back.lines = {"Back Line 1", "Back Line 2"};

    // Set up a matching JSON representation
    valid_json =
        json{{"id", "Test Card GUID"},
             {"front",
              {{"lines", {"Front Line 1", "Front Line 2", "Front Line 3"}}}},
             {"back", {{"lines", {"Back Line 1", "Back Line 2"}}}}};
  }
};

TEST_F(CardTest, DeserializeValidJSON) {
  Card parsed_card = valid_json.get<Card>();
  EXPECT_EQ(parsed_card.id, card.id);
  EXPECT_EQ(parsed_card.front.lines.size(), card.front.lines.size());
  EXPECT_EQ(parsed_card.back.lines.size(), card.back.lines.size());

  for (size_t i = 0; i < card.front.lines.size(); ++i) {
    EXPECT_EQ(parsed_card.front.lines[i], card.front.lines[i]);
  }
  for (size_t i = 0; i < card.back.lines.size(); ++i) {
    EXPECT_EQ(parsed_card.back.lines[i], card.back.lines[i]);
  }
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
  EXPECT_EQ(parsed_card.front.lines.size(), card.front.lines.size());
  EXPECT_EQ(parsed_card.back.lines.size(), card.back.lines.size());

  for (size_t i = 0; i < card.front.lines.size(); ++i) {
    EXPECT_EQ(parsed_card.front.lines[i], card.front.lines[i]);
  }
  for (size_t i = 0; i < card.back.lines.size(); ++i) {
    EXPECT_EQ(parsed_card.back.lines[i], card.back.lines[i]);
  }
}

TEST_F(CardTest, MissingFieldInJSON) {
  // Remove a field from the valid JSON
  valid_json.erase("front");

  // Check for exception or expected fallback behavior
  EXPECT_THROW(valid_json.get<Card>(), json::out_of_range);
}

TEST_F(CardTest, ExtraFieldInJSON) {
  // Add an extra field
  valid_json["extra_field"] = "Unexpected";

  // Ensure deserialization still works (ignoring the extra field)
  Card parsed_card = valid_json.get<Card>();
  EXPECT_EQ(parsed_card.id, card.id);
  EXPECT_EQ(parsed_card.front.lines.size(), card.front.lines.size());
  EXPECT_EQ(parsed_card.back.lines.size(), card.back.lines.size());
}

TEST_F(CardTest, InvalidJSON) {
  // Create an invalid JSON object
  json invalid_json = {{"id", 12345}, {"front", "not_a_side"}};

  // Check for an exception during deserialization
  EXPECT_THROW(invalid_json.get<Card>(), json::type_error);
}
