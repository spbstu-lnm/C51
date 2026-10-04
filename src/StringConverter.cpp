#include "StringConverter.h"
#include <iostream>
#include <stdexcept>
#include <string>


int StringConverter::to_int(char* str) {
    int result = 0;

    try {
        result = std::stoi(str);
    } catch (const std::invalid_argument& e) {
        std::cerr << "FAIL: Invalid argument.\n";
    } catch (const std::out_of_range& e) {
        std::cerr << "FAIL: Out of range.\n";
    }

    return result;
}
