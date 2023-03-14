#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <vector>
#include <Book.cpp>
#include <memory>


bool isBookExisted(std::string);
void pushBookToCollection(std::string, std::map<std::string, int>, std::map<std::string, int>);
