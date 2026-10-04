#ifndef _EGCDRESULT_H_
#define _EGCDRESULT_H_

class EGCDResult {
public:
    [[nodiscard]] int get_i();

    [[nodiscard]] int get_a();
    [[nodiscard]] int get_b();

    [[nodiscard]] int get_r();
    
    [[nodiscard]] int get_x();
    [[nodiscard]] int get_y();

    EGCDResult() = default;
    EGCDResult(int, int, int, int, int, int);
    
    EGCDResult& operator=(const EGCDResult&) = default;


private:
    int i_;
    int a_, b_;    
    int r_;
    int x_, y_;
};

#endif // _EGCDRESULT_H_
