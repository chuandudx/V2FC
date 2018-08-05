#pragma once

#include <Arduino.h>
#include <armControl.h>
#include <SLIFT.h>
#include <HTHING.h>
#include <irsensor.h>
#include <IRBEACON.h>
#include <pid.h>
#include <HBRIDGE.h>
#include <linefollower.h>
#include <SCLAW.h>

#define SUPPORT_SERVO PB8
#define ARM_POT PB1
#define PINCER_SERVO PB9
#define GRABBER_SWITCH PB12

// #define NORMAL_SPEED 150
#define NORMAL_SPEED 150
#define FIRST_GAP_SPEED 150
#define SECOND_GAP_SPEED 230
#define EWOK_SPEED 135
#define SWEEP_SPEED 200
#define TURN_SPEED 255
#define FAST_TURN_SPEED 255
#define METERS_PER_SECOND 0.46333333 //NORMAL_SPEED is 130 -> 0.46333333m/s
#define DEGREES_PER_SECOND 86        //TURN_SPEED is 200 -> 86 deg/sec
#define CLOSEST_DISTANCE_TO_CHEWY 6.0
#define LINE_DISTANCE 6.0
#define EWOK_LONG_DISTANCE_DETECTION 25 //interval in ms to scan for the ewok
#define RIGHT -1
#define LEFT 1
#define ONCE 1
#define TWICE 2

//CALIBRATE THESE VALUES:
#define CLOSEST_DISTANCE_TO_EWOK 14.0                                                                                                                                                                                                                                                                                                                          
#define BRIGHT_CLOSEST_DISTANCE_TO_EWOK 12.0
//gaps
#define FULL_CLIFF_DISTANCE 17
#define GAP_CLIFF_DISTANCE 10
#define TURN_TABLE_GAP 10.3

//ewok distances should be all the same but sometimes one ewok is in front of a window.
//these defines are implemented so we can calibrate accordingly. This is hopefully not needed for
//competition day
#define FIRST_EWOK_DISTANCE 14.0
#define SECOND_EWOK_DISTANCE 14.0
#define THIRD_EWOK_DISTANCE 17.0
#define FOURTH_EWOK_DISTANCE 14.0
#define CHEWIE_DISTANCE 14.0



class robot
{
public:
  //methods and constructor
  robot();
  void init();
  void drive_until_cliff();
  void drive_until_black_line();
  void turn_degrees(float degrees);
  void move_meters(float meters);
  void sweep_for_ewok(float angle);
  void move_toward_ewok(double distance_to_ewok);
  void calibrate_meters_per_second(int seconds);
  void calibrate_degrees_per_second(int seconds);
  void follow_right_edge_until_ewok();
  void line_follow_until_right_ewok();
  void ram_meters(float meters);
  void line_follow_meters(float meters);
  static void delay_update(long ms);
  static void check_sensors();
  void drive_until_cliff_early();
  void sensor_info();
  void grab_ewok();
  void turn_until_black_line(int turn_dir);
  void find_gap_one();
  void line_follow_until_second_ewok();
  void line_follow_until_second_ewok_2(float milliseconds);
  void wait_for_10khz();
  void line_follow_until_beacon();
  void sweep_ewok(int turn_dir);
  void line_follow_until_third_ewok();
  void find_second_edge();
  void sweep_for_zipline(int turn_dir);
  void follow_left_wall_until_ewok();
  void forward_until_wall();
  void turn_table_detect(int num);
  void third_ewok_pick_up_old();
  void second_gap_auto();

  //competition surface stages
  void first_ewok_pick_up();
  void second_ewok_pick_up();
  void archway_crossing();
  void third_ewok_pick_up();
  void edge_follow_to_chewy();
  // void fourth_ewok_pick_up();
  void chewbacca_pick_up();
  void zipline_follow();
  void compass_turn_degrees(float degrees);
  void second_gap_crossing();
  void zipline_finish();  
  void zipline_for_three_ewoks();

  //info
  void sensor_mean();
  void sensor_inverse_mean();
  void sensor_min();

  //different parts of the robot
  //initialized in robot constructor
  HBRIDGE *left_motor;
  HBRIDGE *right_motor;
  irsensor *bottom_sensor;
  irsensor *front_sensor; //only sensor that is upside down
  irsensor *left_sensor;
  irsensor *right_sensor;
  SLIFT *lift;
  pid *pid_controller;
  linefollower *line_follower;
  SCLAW *right_claw;
};