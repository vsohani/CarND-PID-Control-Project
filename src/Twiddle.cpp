#include "Twiddle.h"
#include <iostream>
#include <vector>
#include <cmath>
Twiddle::Twiddle() {}

Twiddle::~Twiddle() {}

void Twiddle::init(double Kp, double Ki, double Kd) {

    params_ = {Kp, Ki, Kd};
    param_deltas_ = {Kp/50 , Ki/50 , Kd / 50};
    count_ = 0;
    twiddle_phase_= 0;
    param_num_ = 0;
    best_cte_ = 1;
    total_cte_ = 0.0 ;
    tolerance_ = 0.0 ;

}

void Twiddle::incrementCount(double cte){
   
}

int Twiddle::getCount(){
    return count_ ;
}

double Twiddle::getTolerance(){

    return tolerance_;
}

std::vector<double> Twiddle::updateParams(){
    double curr_cte = total_cte_ / count_ ;
    count_ = 0;
    total_cte_ = 0;
    tolerance_ = param_deltas_[0] + param_deltas_[1] + param_deltas_[2];

    printf("\u0394Kp=%.5f, \u0394Ki=%.7f, \u0394Kd=%.4f, tolerance=%.3f\n", param_deltas_[0], param_deltas_[1], param_deltas_[2], tolerance_);

  bool update_params = false;

    
    if(twiddle_phase_ == 0){
        params_[param_num_] += param_deltas_[param_num_];
        twiddle_phase_ = 1 ;
    }
    else if (twiddle_phase_ == 1) {
        if(curr_cte < best_cte_){
            best_cte_ = curr_cte ;
            param_deltas_[param_num_] *= 1.1 ;
            twiddle_phase_ = 0;
            update_params = true;
        }
        else{
            params_[param_num_] -= 2 * param_deltas_[param_num_] ;
            twiddle_phase_ = 2 ;
            }
    } 
    else if (twiddle_phase_ == 2) {
        if(curr_cte < best_cte_){
            best_cte_ = curr_cte ;
            param_deltas_[param_num_] *= 1.1 ;
        }
        else{
            params_[param_num_] += param_deltas_[param_num_];
            param_deltas_[param_num_] *= 0.9 ;
        }
        twiddle_phase_ = 0;
        update_params = true;

    }
    if (update_params && (++param_num_ == 3)){
        param_num_ =0;
    }
    return params_ ;

}