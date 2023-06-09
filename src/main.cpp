#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include <vector>
#include "Twiddle.h"
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

static int count_ = 0;
static const int sample_size = 100 ;
static const double threshold = 0.1;
static bool achieved_tolerance = false;

//static double KP = 0.11;
//static double KD = 2.2;
//static double KI = 0.00006;

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}


int main() {
  uWS::Hub h;

  PID pid_cte;
  PID pid_speed;
  Twiddle twiddle;

  /**
   * TODO: Initialize the pid variable.
   */
  pid_cte.Init(0.125,0.000,6.8);
  pid_speed.Init(0.2,0.000,1);

  twiddle.init(0.125,0.0000,6.8);


  
  
  h.onMessage([&pid_cte,&pid_speed,&twiddle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value = 0.0;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */

          pid_cte.UpdateError(cte);
          steer_value -= pid_cte.TotalError();

          pid_speed.UpdateError((45-speed) - angle*5 - cte*2);
           

          bool is_sample_period = (++count_ % sample_size == 0) ;


          



          if(!achieved_tolerance){
            twiddle.incrementCount(cte);
            if(is_sample_period){
              std::vector<double> params = twiddle.updateParams();
              std::cout<<"Param Updated";
              if(twiddle.getTolerance() < threshold){
                achieved_tolerance = true;
              }else{
                pid_cte.Init(params[0], params[1], params[2]);
              }
            }





          }
           
          
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;
        
          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = pid_speed.TotalError();
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}