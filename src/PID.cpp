#include "PID.h"
#include "iostream"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */
using namespace std;

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
   PID::Kp = Kp_ ;
   PID::Ki = Ki_ ;
   PID::Kd = Kd_ ;
   
   PID::p_error = 0;
   PID::i_error = 0;
   PID::d_error = 0;

   PID::prev_cte = 0;
   
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  PID::p_error = cte ;
  PID::i_error += cte;
  PID::d_error = cte - prev_cte;

  prev_cte = cte;
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return p_error*Kp + i_error*Ki + d_error*Kd;  // TODO: Add your total error calc here!
}

