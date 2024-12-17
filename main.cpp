#include "card.hpp"
#include <CLI/CLI.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

// Function to generate a GUID
std::string generate_guid() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

  std::ostringstream guid;

  guid << std::hex << std::setfill('0');
  guid << std::setw(8) << dis(gen) << '-';
  guid << std::setw(4) << (dis(gen) & 0xFFFF) << '-';
  guid << std::setw(4) << ((dis(gen) & 0x0FFF) | 0x4000)
       << '-'; // UUID version 4
  guid << std::setw(4) << ((dis(gen) & 0x3FFF) | 0x8000) << '-'; // UUID variant
  guid << std::setw(12) << dis(gen);

  return guid.str();
}

// Function to prompt the user for card details and create a new Card object
Card create_new_card() {
  Card new_card;

  // Automatically set the id to a GUID
  new_card.id = generate_guid();

  std::cout << "Generated ID: " << new_card.id << "\n";

  std::cout << "Enter lines for the front side (type 'DONE' to finish):\n";
  while (true) {
    std::string line;
    std::getline(std::cin, line);
    if (line == "DONE")
      break;
    new_card.front.lines.push_back(line);
  }

  std::cout << "Enter lines for the back side (type 'DONE' to finish):\n";
  while (true) {
    std::string line;
    std::getline(std::cin, line);
    if (line == "DONE")
      break;
    new_card.back.lines.push_back(line);
  }

  return new_card;
}

// Function to save the updated JSON to a file
void save_to_file(const std::string &filename, const json &json_data) {
  try {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
      throw std::ios_base::failure("Failed to open file for writing");
    }
    outputFile << json_data.dump(4); // Pretty print with 4 spaces
    std::cout << "Saved to file: " << filename << "\n";
  } catch (const std::exception &e) {
    std::cerr << "Error saving to file: " << e.what() << "\n";
  }
}

enum class Action { AddNewCard, CountCards };

int main(int argc, char **argv) {
  CLI::App app{"Flashcards CLI"};

  argv = app.ensure_utf8(argv);

  Action action = Action::CountCards;

  std::string filename = "data.json";

  app.add_option("-a,--action", action, "program action")
      ->transform(CLI::CheckedTransformer(
          std::map<std::string, Action>{{"add", Action::AddNewCard},
                                        {"count", Action::CountCards}},
          CLI::ignore_case));
  app.add_option("-f,--file", filename, "flash cards data file")
      ->check(CLI::ExistingFile);

  CLI11_PARSE(app, argc, argv);

  std::cout << "Using file: " << filename << "\n";

  // Parse the JSON file into a json object
  json json_data;
  try {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
      throw std::ios_base::failure("File not found or cannot be opened");
    }
    inputFile >> json_data;
  } catch (const json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << "\n";
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  // Check if the JSON is an array
  if (!json_data.is_array()) {
    std::cerr << "Error: Expected a JSON array.\n";
    return 1;
  }

  switch (action) {
  case Action::CountCards: {
    std::cout << "There are " << json_data.size() << " cards in the file.\n";
    break;
  }
  case Action::AddNewCard: {
    std::cout << "Add a new card:\n";
    Card new_card = create_new_card();

    // Convert the new card to JSON and add it to the existing JSON array
    json new_card_json = new_card;
    json_data.push_back(new_card_json);

    // Save updated JSON data to file
    save_to_file(filename, json_data);
    break;
  }
  }

  return 0;
}
