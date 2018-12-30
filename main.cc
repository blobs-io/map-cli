#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <regex>
#include <iomanip>
static nlohmann::json* map = new nlohmann::json {};
static std::string* input = new std::string{""};

void show_editor_menu(std::vector<nlohmann::json>*);

int main() {
    std::cout << "blobs.io map generator" << std::endl
              << "======================" << std::endl
              << "1 - Start from scratch" << std::endl
              << "2 - Use template" << std::endl
              << "3 - Exit" << std::endl
              << ">";
    std::cin >> *input;
    switch (std::stoi(*input)) {
        case 1: 
            {
                std::vector<nlohmann::json> wallArray {};
                show_editor_menu(&wallArray);
            }
        break;
        case 2:
            {
                std::string* filecontents = new std::string {""};
                std::cout << "Path to JSON file: ";
                std::cin >> *input;
                std::ifstream* file = new std::ifstream {*input};
                if(!(*file)) {
                    std::cout << "An error occured while trying to open file.";
                    delete file;
                    delete input;
                    delete filecontents;
                    delete map;
                    exit(1);
                }
                nlohmann::json* json = new nlohmann::json {};
                *file >> *json;

                std::vector<nlohmann::json> wallArray = (*json)["objects"]["walls"];
                
                std::cout << "File loaded.\n\n";
                show_editor_menu(&wallArray);


                delete json;
                delete file;
                delete filecontents;
            }
        break;
        case 3:
            exit(0);
        break;
    }
    delete input;
    delete map;
    return 0;
}

void show_editor_menu(std::vector<nlohmann::json>* walls) {
        std::cout << "Amount of walls: " << walls->size() << std::endl;
    std::cout << "1 - Find wall(s)" << std::endl
              << "2 - Add wall" << std::endl
              << "3 - Remove wall" << std::endl
              << "4 - Export map" << std::endl
              << "5 - Back to main menu" << std::endl
              << ">";
    std::cin >> *input;
    switch (std::stoi(*input)) {
        case 1:
            {
                std::cout << "Input search type and value to find wall (e.g. 'x100', 'y30'): ";
                std::cin >> *input;
                std::vector<nlohmann::json> matches;
                if (input->at(0) == 'x') {
                    for (std::size_t i=0;i < walls->size(); ++i) {
                        if ((int)(*walls)[i]["x"] == std::stoi(input->substr(1))) {
                            matches.push_back(nlohmann::json((*walls)[i]));
                        }
                    }
                } else if (input->at(0) == 'y') {
                    for (std::size_t i=0;i < walls->size(); ++i) {
                        if ((int)(*walls)[i]["y"] == std::stoi(input->substr(1))) {
                            matches.push_back(nlohmann::json((*walls)[i]));
                        }
                    }
                }
                std::cout << "Found " << matches.size() << " walls: " << std::endl;
                for (const nlohmann::json& wall : matches) {
                    std::cout << "- X: " << wall["x"] << " | Y: " << wall["y"] << " | Type: " << wall["type"] << std::endl;
                }
                show_editor_menu(walls);
            }
        break;
        case 2:
            {
                std::cout << "Please input values (x,y,type), example: 10,10,0. Type anything else to exit this menu." << std::endl;
                while(std::cin >> *input) {
                    if (!std::regex_match(*input, std::regex("\\d+,\\d+,\\d"))) break;
                    nlohmann::json tempobj;
                    int x, y, type;
                    x = std::stoi(input->substr(0, input->find(',')));
                    y = std::stoi(input->substr(input->find(',') + 1, input->substr(input->find(',') + 1).find(',')));
                    type = std::stoi(input->substr(input->rfind(',')+1));
                    tempobj = nlohmann::json::parse("{\"x\": " + std::to_string(x) + ", \"y\": " + std::to_string(y) +", \"type\": " + std::to_string(type) + "}");
                    walls->push_back(tempobj);
                }
                show_editor_menu(walls);
            }
        break;
        case 3:
            {
                std::cout << "Input search type and value to delete wall (e.g. 'x100', 'y30'): ";
                std::cin >> *input;
                int counter = 0;
                if (input->at(0) == 'x') {
                    for (std::size_t i=0;i < walls->size(); ++i) {
                        if ((int)(*walls)[i]["x"] == std::stoi(input->substr(1))) {
                            walls->erase(walls->begin() + i);
                            counter++;
                        }
                    }
                } else if (input->at(0) == 'y') {
                    for (std::size_t i=0;i < walls->size(); ++i) {
                        if ((int)(*walls)[i]["y"] == std::stoi(input->substr(1))) {
                            walls->erase(walls->begin() + i);
                            counter++;
                        }
                    }
                }
                std::cout << "Deleted " << std::to_string(counter) << " walls." << std::endl;
                show_editor_menu(walls);
            }
        break;
        case 4: 
        {
            (*map)["objects"]["walls"] = *walls;
            std::cout << "Path to output file: ";
            std::cin >> *input;
            std::ofstream* file = new std::ofstream {*input};
            if (!(*file)) {
                std::cout << "An error occured while trying to open output file." << std::endl;
            }
            *file << std::setw(4) << *map;
            delete file;
        }
        break;
    }
}
