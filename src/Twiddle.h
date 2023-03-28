#ifndef TWIDDLE
#define TWIDDLE

#include <vector>

class Twiddle {
    private:
    int count_ ;
    int twiddle_phase_ ;
    int param_num_ ;
    double total_cte_ ;
    double best_cte_ ;
    double tolerance_ ;

    std::vector<double> params_ ;
    std::vector<double> param_deltas_ ;

    public:

    /**
    Constructor
    */
    Twiddle() ;

    /**
    Destructor
    */
    virtual ~Twiddle() ;

    /**
    Initialize Twiddle
    */
    void init(double Kp, double Ki, double Kd);

    void incrementCount(double cte);

    int getCount();

    double getTolerance();

    std::vector<double> updateParams();


};
#endif