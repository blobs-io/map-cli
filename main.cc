#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <regex>
#include <iomanip>
static nlohmann::json* map = new nlohmann::json {};
static std::string* input = new std::string{""};
static std::string* mapname = new std::string{"default"};

class wall {
    private:
        int x;
        int y;
        int type;
        static int count;
    public:
        wall(int, int, int);
        std::string to_string();
        nlohmann::json to_json();
        wall();
        int operator[] (std::string);
};
int wall::count = 0;

wall::wall(int _x, int _y, int _type) : x(_x), y(_y), type(_type) {
    wall::count++;
};
wall::wall() {
    this->x = 0;
    this->y = 0;
    this->type = 0;
    wall::count++;
}
std::string wall::to_string() {
    return "{\"x\": " + std::to_string((this->x)) + ", \"y\": "  + std::to_string((this->y)) + ", \"type\": " + std::to_string((this->type)) + "}";
}

nlohmann::json wall::to_json() {
    return nlohmann::json {this->to_string()};
}
int wall::operator[] (std::string accessor) {
    int retVal;
    if (accessor == "x") retVal = (this->x);
    else if (accessor == "y") retVal = (this->y);
    else if (accessor == "type") retVal = (this->type);
    else retVal = std::string::npos;
    return retVal;
}

void show_editor_menu(std::vector<wall>*);

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
                std::vector<wall> wallArray {};
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

                std::vector<wall> wallArray;
                for (std::size_t i = 0; i < (*json)["objects"]["walls"].size(); ++i) {
                    wallArray.push_back(wall((*json)["objects"]["walls"][i]["x"], (*json)["objects"]["walls"][i]["x"], (*json)["objects"]["walls"][i]["x"]));
                }

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

void show_editor_menu(std::vector<wall>* walls) {
    std::cout << "Amount of walls: " << walls->size()
              << "1 - Find wall(s)" << std::endl
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
                std::vector<wall> matches;
                if (input->at(0) == 'x') {
                    for (std::size_t i=0;i < walls->size(); ++i) {
                        if ((*walls)[i]["x"] == std::stoi(input->substr(1))) {
                            matches.push_back(wall((*walls)[i]["x"], (*walls)[i]["y"], (*walls)[i]["type"]));
                        }
                    }
                } else if (input->at(0) == 'y') {
                    for (std::size_t i=0;i < walls->size(); ++i) {
                        if ((*walls)[i]["y"] == std::stoi(input->substr(1))) {
                            matches.push_back(wall((*walls)[i]["x"], (*walls)[i]["y"], (*walls)[i]["type"]));
                        }
                    }
                }
                std::cout << "Found " << matches.size() << " walls: " << std::endl;
                for (std::size_t i = 0; i < matches.size(); ++i) {
                    std::cout << "- X: " << matches[i]["x"] << " | Y: " << matches[i]["y"] << " | Type: " << matches[i]["type"] << std::endl;
                }
                show_editor_menu(walls);
            }
        break;
        case 2:
            {
                std::cout << "Please input values (x,y,type), example: 10,10,0. Type anything else to exit this menu." << std::endl;
                while(std::cin >> *input) {
                    if (!std::regex_match(*input, std::regex("\\d+,\\d+,\\d"))) break;
                    int x, y, type;
                    x = std::stoi(input->substr(0, input->find(',')));
                    y = std::stoi(input->substr(input->find(',') + 1, input->substr(input->find(',') + 1).find(',')));
                    type = std::stoi(input->substr(input->rfind(',')+1));
                    wall tempobj (x, y, type);
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
                        if ((*walls)[i]["x"] == std::stoi(input->substr(1))) {
                            walls->erase(walls->begin() + i);
                            counter++;
                        }
                    }
                } else if (input->at(0) == 'y') {
                    for (std::size_t i=0;i < walls->size(); ++i) {
                        if ((*walls)[i]["y"] == std::stoi(input->substr(1))) {
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
            std::vector<nlohmann::json> _tmpwalls {};
            for(std::size_t i = 0; i < walls->size(); ++i) {
                _tmpwalls.push_back((*walls)[i].to_json());
            }
            (*map)["objects"]["walls"] = _tmpwalls;
            std::cout << "Path to output file (type '.stdout' for console-only output): ";
            std::cin >> *input;
            if (*input == ".stdout") {
                std::cout << map->dump(4);
            } else {
                std::ofstream* file = new std::ofstream {*input};
                if (!(*file)) {
                    std::cout << "An error occured while trying to open output file." << std::endl;
                } else {
                    *file << std::setw(4) << *map;
                }
                delete file;
            }
        }
        break;
    }
}
