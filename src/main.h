#ifndef _MAIN_H_
#define _MAIN_H_

#include "EGCDResult.h"

#include <string>

namespace EGCD {
[[nodiscard]] EGCDResult extended_gcd(int, int);
[[nodiscard]] EGCDResult extended_binary_gcd(int, int);
[[nodiscard]] EGCDResult extended_truncated_gcd(int, int);

void print_result(std::string, EGCDResult);
}

int main(int, char**);

#endif // _MAIN_H_
