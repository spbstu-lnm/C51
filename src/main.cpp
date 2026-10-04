#include "main.h"
#include "EGCDResult.h"
#include "StringConverter.h"

#include <cstdlib>
#include <iostream>
#include <print>


EGCDResult EGCD::extended_gcd(int a, int b) {
    int i = 0, r = 0, x = 0, y = 0;

    // TODO: perform iterations, print log for each iteration to screen
    
    return EGCDResult(i, a, b, r, x, y);
}


EGCDResult EGCD::extended_binary_gcd(int a, int b) {
    int i = 0, r = 0, x = 0, y = 0;

    // TODO: perform iterations, print log for each iteration to screen
    
    return EGCDResult(i, a, b, r, x, y);
}


EGCDResult EGCD::extended_truncated_gcd(int a, int b) {
    int i = 0, r = 0, x = 0, y = 0;

    // TODO: perform iterations, print log for each iteration to screen
    
    return EGCDResult(i, a, b, r, x, y);
}


void EGCD::print_result(const std::string prefix, EGCDResult res) {
    std::println("{} at {}: ({}, {}), {}, ({}, {})",
        prefix, res.get_i(), 
        res.get_a(), res.get_b(), 
        res.get_r(),
        res.get_x(), res.get_y()
    );
}


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "FAIL: Not enough arguments provided.\n";
        
        return EXIT_FAILURE;
    }

    int a = StringConverter::to_int(argv[1]);
    int b = StringConverter::to_int(argv[2]);
    
    if (a == 0 || b == 0) {
        std::cerr << "FAIL: Division by zero.\n";

        return EXIT_FAILURE;
    }
    
    EGCDResult res;

    std::cout << std::endl;
    res = EGCD::extended_gcd(a, b);
    std::cout << std::endl;

    EGCD::print_result("EGCD", res);

    std::cout << std::endl;
    res = EGCD::extended_binary_gcd(a, b);
    std::cout << std::endl;

    EGCD::print_result("EBGCD", res);

    std::cout << std::endl;
    res = EGCD::extended_truncated_gcd(a, b);
    std::cout << std::endl;

    EGCD::print_result("ETGCD", res);

    return EXIT_SUCCESS;
}
