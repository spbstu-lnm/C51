#include "EGCDResult.h"


int EGCDResult::get_i() {
    return i_;
}

int EGCDResult::get_a() {
    return a_;
}

int EGCDResult::get_b() {
    return b_;
}

int EGCDResult::get_r() {
    return r_;
}

int EGCDResult::get_x() {
    return x_;
}

int EGCDResult::get_y() {
    return y_;
}


EGCDResult::EGCDResult(int i, int a, int b, int r, int x, int y) 
    : i_(i), a_(a), b_(b), r_(r), x_(x), y_(y) {}
