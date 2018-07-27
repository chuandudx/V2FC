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


#define ARM_SERVO PB8   
#define ARM_POT PB1
#define GRABBER_SERVO PB9
#define GRABBER_SWITCH PB12 

//#define NORMAL_SPEED 230
#define NORMAL_SPEED 255
#define SECOND_GAP_SPEED 230
#define EWOK_SPEED 50
#define TURN_SPEED 200
#define FULL_CLIFF_DISTANCE 17
#define GAP_CLIFF_DISTANCE 10
#define METERS_PER_SECOND 1
#define DEGREES_PER_SECOND 1
#define CLOSEST_DISTANCE_TO_EWOK 10
#define LINE_DISTANCE 6.0
#define EWOK_LONG_DISTANCE_DETECTION 25 //interval in ms to scan for the ewok

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
  void move_toward_ewok();
  void calibrate_meters_per_second(int seconds);
  void calibrate_degrees_per_second(int seconds);
  void follow_right_edge_until_ewok();
  void line_follow_meters(float meters);
  static void delay_update(long ms);
  static void check_sensors();

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
};