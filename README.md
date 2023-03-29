
# CarND-Controls-PID

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```



## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 



=======
# CarND-PID-Control-Project
Speed, Throttle and Steering control using PID algorithm in a self driving car.

Goal 1 : Control the Cross Track Error (CTE)

The CTE value is taken from the json message via websocket from the simulator. We use the principal of feedback control system to calculate the cross-track-error and predict input to the steering wheel. We exploit PID controller to calculate our set point.

Goal 2 : Control Throttle and speed

I controlled the throttle value based on the the steering angle and the cross-track error. I prevented the car from going too fast on turns and while it is trying to correct its CTE. This is what we do in real-life. My control action looks like :

 throttle = (target_speed - curr_speeed) - K1*steer_angle - K2*CTE (Where K1 and K2 are gains. Too high values might result in the car stopping all-togther or moving in the reverse direction)

Goal 3 : Tune the P, I and D gains

I used brute force method to tune the parameters before implementing Twiddle. I used the following steps to arrive at my gain values :

1. Set Ki and Kd to zero. Start increasing values of Kp until system starts reacting quickly to the changes.
2. Start increasing value of Kd. Kd should not be too high as it will slow down the system and will prevent it from changing quickly. It shouldnt be too low either as it will not damp the oscillations.
3. The integral value is used to eliminiate the steady state error.  A high value might cause the system to overshoot.

## Improve the performance

1. Implement new control strategy all together. Use control laws like LQR, MPC, Kanayama.
2. Consider the rate of updating error. This depends upon the rate at which data is being received from the simulator. Higher grpahics might slow the update rate and hence will create a latency in the control action.

