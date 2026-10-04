#include "main.h"
#include "EGCDResult.h"
#include "StringConverter.h"

#include <cstdlib>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

namespace {

struct Step {
    int number;
    int64_t a;
    int64_t b;
    int64_t remainder;
    int64_t x;
    int64_t y;
};

struct Calculation {
    int iterations = 0;
    int64_t gcd = 0;
    int64_t x = 0;
    int64_t y = 0;
    std::vector<Step> steps;
};


// Nearest quotient produces signed remainders with smaller absolute values.
Calculation run_euclid(int a, int b, bool truncated, bool log) {
    int64_t old_r = std::abs(static_cast<int64_t>(a));
    int64_t r = std::abs(static_cast<int64_t>(b));
    int64_t old_x = 1, x = 0, old_y = 0, y = 1;

    if (a < 0) old_x = -old_x;
    if (b < 0) y = -y;

    Calculation result;

    while (r != 0) {
        int64_t q = old_r / r;

        if (truncated && std::abs(old_r % r) * 2 >= std::abs(r))
            q += ((old_r < 0) == (r < 0)) ? 1 : -1;
        
        const int64_t next_r = old_r - q * r;
        const int64_t next_x = old_x - q * x;
        const int64_t next_y = old_y - q * y;
        
        ++result.iterations;
        
        if (log) result.steps.push_back({result.iterations, old_r, r, next_r, next_x, next_y});
        
        old_r = r; r = next_r;
        old_x = x; x = next_x;
        old_y = y; y = next_y;
    }
    result.gcd = old_r;
    result.x = old_x;
    result.y = old_y;
    
    return result;
}

Calculation run_binary_gcd(int a, int b, bool log) {
    const int64_t abs_a = std::abs(static_cast<int64_t>(a));
    const int64_t abs_b = std::abs(static_cast<int64_t>(b));
    
    int64_t u = abs_a, v = abs_b;
    int64_t A = 1, B = 0, C = 0, D = 1;
    
    Calculation result;

    if (u == 0 || v == 0) {
        result.gcd = u == 0 ? v : u;
        result.x = a == 0 ? 0 : (a < 0 ? -1 : 1);
        result.y = b == 0 ? 0 : (b < 0 ? -1 : 1);
        return result;
    }

    int shift = 0;
    while ((u % 2 == 0) && (v % 2 == 0)) {
        u /= 2;
        v /= 2;
        ++shift;
    }
    const int64_t coefficient_a = u;
    const int64_t coefficient_b = v;

    auto halve = [coefficient_a, coefficient_b](int64_t& value, int64_t& x, int64_t& y) {
        if ((x % 2 == 0) && (y % 2 == 0)) {
            x /= 2; y /= 2;
        } else {
            x = (x + coefficient_b) / 2;
            y = (y - coefficient_a) / 2;
        }
        
        value /= 2;
    };

    while (u != v) {
        const int64_t previous_u = u, previous_v = v;
        if (u % 2 == 0) halve(u, A, B);
        else if (v % 2 == 0) halve(v, C, D);
        else if (u > v) {
            u -= v;
            A -= C; B -= D;
            
            halve(u, A, B);
        } else {
            v -= u;
            C -= A; D -= B;
            
            halve(v, C, D);
        }
        
        ++result.iterations;
        
        if (log) result.steps.push_back({result.iterations,
            previous_u << shift, previous_v << shift, u << shift,
            (a < 0 ? -1 : 1) * (u <= v ? A : C),
            (b < 0 ? -1 : 1) * (u <= v ? B : D)});
    }
    
    result.gcd = u << shift;
    result.x = a < 0 ? -A : A;
    result.y = b < 0 ? -B : B;
    
    return result;
}

void print_log(const char* name, const Calculation& result) {
    std::println("{} iterations:", name);
    
    const auto& steps = result.steps;
    
    auto print_step = [](const Step& step) {
        std::println("  {}: ({}, {}) -> {}; ({}, {})",
                     step.number, step.a, step.b, step.remainder, step.x, step.y);
    };
    
    if (steps.size() <= 20) {
        for (const auto& step : steps) print_step(step);
    } else {
        for (std::size_t i = 0; i < 5; ++i) print_step(steps[i]);
        
        std::println("  ... {} iterations omitted ...", steps.size() - 10);
        
        for (std::size_t i = steps.size() - 5; i < steps.size(); ++i) print_step(steps[i]);
    }
}

EGCDResult to_result(int a, int b, const Calculation& c) {
    return EGCDResult(c.iterations, a, b, static_cast<int>(c.gcd),
                      static_cast<int>(c.x), static_cast<int>(c.y));
}

template <typename Algorithm>
void benchmark(const char* name, Algorithm algorithm, int a, int b) {
    constexpr int repetitions = 100000;
    
    const auto start = std::chrono::steady_clock::now();
    
    int64_t checksum = 0;
    
    for (int i = 0; i < repetitions; ++i) {
        const auto result = algorithm(a, b, false);
        checksum += result.gcd + result.x + result.y;
    }
    
    const auto elapsed = std::chrono::duration<double, std::nano>(
        std::chrono::steady_clock::now() - start).count();
    
    std::println("{}: {:.2f} ns per call ({} repetitions, checksum {})",
                 name, elapsed / repetitions, repetitions, checksum);
}

}

EGCDResult EGCD::extended_gcd(int a, int b) {
    const auto calculation = run_euclid(a, b, false, true);
    
    print_log("EGCD", calculation);
    
    return to_result(a, b, calculation);
}


EGCDResult EGCD::extended_binary_gcd(int a, int b) {
    const auto calculation = run_binary_gcd(a, b, true);
    
    print_log("EBGCD", calculation);
    
    return to_result(a, b, calculation);
}


EGCDResult EGCD::extended_truncated_gcd(int a, int b) {
    const auto calculation = run_euclid(a, b, true, true);
    
    print_log("ETGCD", calculation);
    
    return to_result(a, b, calculation);
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

    std::println("Benchmark ({} repetitions per algorithm):", 100000);
    benchmark("EGCD", 
        [](int x, int y, bool log) { return run_euclid(x, y, false, log); }, a, b);
    
    benchmark("EBGCD", 
        [](int x, int y, bool log) { return run_binary_gcd(x, y, log); }, a, b);
    
    benchmark("ETGCD", 
        [](int x, int y, bool log) { return run_euclid(x, y, true, log); }, a, b);

    std::cout << std::endl;
    res = EGCD::extended_gcd(a, b);
    std::cout << std::endl;

    EGCD::print_result("=== EGCD", res);

    std::cout << std::endl;
    res = EGCD::extended_binary_gcd(a, b);
    std::cout << std::endl;

    EGCD::print_result("=== EBGCD", res);

    std::cout << std::endl;
    res = EGCD::extended_truncated_gcd(a, b);
    std::cout << std::endl;

    EGCD::print_result("=== ETGCD", res);
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
