/*
The loop tests the following functions of the robot
in the order they are shown:
    Arm
        -horizontal
        -search position
        -ewok position
        -ewok grab and swi tch satisfaction (press manually to continue program)
        -arm vertical
        -arm to basket position
        -ewok release
    Scissor Lift
        -check if scissor lift is in bottom position (should be true if scissor lift is down)
        -check if up position limit switch works (person presses this)
        -check if basket limit switch works
        -move scissor lift into up position
        -check that stay function works
        -move scissor lift to down position
    Tracks
        -move left track forwards
        -move left track backwards
        -move right track forwards
        -move right track backards
        -move both tracks forwards
        -move both tracks backwards
        -move them in opposite directions
        -switch the opposite directions
        -full stop
    Sensor Arrays
        -PID array
        -ewok array
        -left edge array
        -right edge array
    Beacon Sensor
*/

#include "HBRIDGE.h"
#include "armControl.h"
#include "SLIFT.h"
#include "IRBEACON.h"
#include "irsensor.h"
#include <Arduino.h>
#include <Wire.h>

#define DELAY_VAL 1000

#define EWOK_DETECTION 0x48
#define LINE_FOLLOWING 0x49
#define EDGE_DETECTION_LEFT 0x4A
#define EDGE_DETECTION_RIGHT 0x4B

#define ROW 20
#define COL 9

unsigned long current_time;

//The lookup_arrays for the IR sensors
int data_1[ROW][COL] = {{1, 131, 125, 134, 132, 129, 133, 135, 140},
                        {3, 140, 134, 145, 143, 140, 143, 149, 150},
                        {5, 150, 145, 155, 151, 147, 152, 157, 159},
                        {7, 160, 156, 163, 159, 156, 160, 165, 168},
                        {10, 174, 169, 176, 170, 166, 171, 176, 179},
                        {12, 182, 177, 183, 176, 172, 177, 184, 187},
                        {15, 196, 188, 196, 188, 182, 188, 195, 198},
                        {17, 207, 195, 205, 196, 189, 195, 204, 206},
                        {20, 227, 207, 222, 210, 200, 206, 218, 219},
                        {23, 373, 229, 277, 243, 220, 225, 255, 251},
                        {25, 417, 237, 337, 268, 229, 235, 281, 267},
                        {28, 629, 305, 613, 527, 291, 301, 551, 489},
                        {30, 768, 429, 764, 703, 445, 459, 745, 685},
                        {33, 856, 540, 900, 860, 601, 617, 883, 818},
                        {36, 960, 659, 1038, 1014, 757, 779, 1038, 971},
                        {39, 1029, 736, 1131, 1122, 866, 891, 1142, 1077},
                        {42, 1081, 785, 1198, 1202, 947, 976, 1223, 1155},
                        {44, 1097, 809, 1227, 1236, 981, 1015, 1255, 1188},
                        {46, 1092, 810, 1233, 1247, 991, 1026, 1258, 1192},
                        {50, 1156, 889, 1314, 1337, 1081, 1123, 1351, 1291}};

int data_2[ROW][COL] = {{1, 125, 121, 128, 125, 124, 131, 134, 148},
                        {3, 132, 128, 143, 134, 132, 140, 142, 157},
                        {5, 145, 140, 151, 144, 142, 151, 153, 171},
                        {7, 155, 149, 158, 152, 150, 159, 163, 185},
                        {10, 170, 164, 171, 165, 162, 173, 180, 231},
                        {12, 181, 175, 182, 174, 170, 183, 193, 586},
                        {15, 202, 194, 207, 191, 183, 201, 227, 1272},
                        {17, 232, 212, 252, 207, 194, 221, 369, 1615},
                        {20, 493, 350, 733, 326, 220, 436, 902, 1966},
                        {23, 772, 671, 1107, 739, 318, 826, 1240, 2171},
                        {25, 932, 856, 1321, 985, 549, 1058, 1436, 2284},
                        {28, 1102, 1047, 1546, 1245, 808, 1310, 1645, 2406},
                        {30, 1254, 1208, 1725, 1440, 1006, 1500, 1806, 2515},
                        {33, 1311, 1269, 1809, 1538, 1105, 1595, 1878, 2545},
                        {36, 1422, 1387, 1944, 1689, 1261, 1743, 2001, 2622},
                        {39, 1433, 1403, 1974, 1724, 1300, 1778, 2023, 2625},
                        {42, 1572, 1548, 2130, 1895, 1473, 1947, 2166, 2727},
                        {44, 1614, 1587, 2171, 1939, 1521, 1990, 2203, 2750},
                        {46, 1658, 1630, 2223, 1992, 1579, 2045, 2245, 2775},
                        {50, 1619, 1589, 2191, 1959, 1547, 2015, 2214, 2745}};

int data_3[ROW][COL] = {{1, 139, 136, 144, 132, 133, 134, 138, 145},
                        {3, 145, 144, 152, 140, 140, 141, 145, 151},
                        {5, 156, 154, 161, 150, 149, 152, 155, 161},
                        {7, 166, 163, 172, 158, 158, 161, 165, 172},
                        {10, 185, 177, 189, 170, 170, 174, 180, 187},
                        {12, 200, 188, 206, 179, 179, 184, 192, 199},
                        {15, 238, 204, 253, 190, 191, 198, 211, 218},
                        {17, 462, 225, 604, 203, 203, 213, 239, 248},
                        {20, 842, 369, 11085, 231, 231, 255, 476, 481},
                        {23, 1106, 662, 1416, 364, 358, 510, 804, 757},
                        {25, 1217, 792, 1563, 537, 531, 679, 950, 880},
                        {28, 1399, 992, 1767, 786, 784, 923, 1165, 1070},
                        {30, 1487, 1095, 1871, 925, 927, 1062, 1289, 1179},
                        {33, 1569, 1189, 1973, 1059, 1064, 1192, 1398, 1273},
                        {36, 1720, 1344, 2102, 1224, 1229, 1351, 1549, 1414},
                        {39, 1786, 1417, 2175, 1323, 1329, 1448, 1638, 1496},
                        {42, 1870, 1504, 2253, 1424, 1429, 1548, 1730, 1584},
                        {44, 1968, 1603, 2337, 1528, 1537, 1648, 1827, 1679},
                        {46, 2080, 1715, 2434, 1651, 1661, 1770, 1947, 1795},
                        {50, 2200, 1839, 2531, 1779, 1788, 1893, 2068, 1917}};

int data_4[ROW][COL] = {{1, 137, 126, 124, 120, 126, 124, 136, 136},
                        {3, 152, 138, 137, 131, 140, 135, 151, 146},
                        {5, 169, 148, 148, 142, 151, 146, 161, 161},
                        {7, 184, 158, 155, 150, 159, 154, 171, 171},
                        {10, 243, 177, 169, 161, 172, 167, 192, 188},
                        {12, 702, 195, 180, 170, 182, 177, 217, 203},
                        {15, 1259, 273, 200, 184, 199, 193, 595, 254},
                        {17, 1612, 728, 241, 199, 223, 212, 1091, 605},
                        {20, 1878, 1139, 627, 231, 424, 312, 1525, 1005},
                        {23, 2099, 1447, 1017, 449, 800, 690, 1829, 1306},
                        {25, 2182, 1587, 1214, 668, 1002, 909, 1992, 1466},
                        {28, 2354, 1818, 1505, 985, 1287, 1213, 2225, 1711},
                        {30, 2442, 1944, 1673, 1167, 1452, 1392, 2355, 1849},
                        {33, 2504, 2039, 1803, 1312, 1586, 1538, 2461, 1967},
                        {36, 2611, 2173, 1969, 1490, 1747, 1712, 2594, 2121},
                        {39, 2694, 2283, 2107, 1638, 1880, 1856, 2704, 2254},
                        {42, 2752, 2358, 2205, 1746, 1979, 1965, 2786, 2354},
                        {44, 2792, 2379, 2234, 1778, 2007, 1996, 2801, 2373},
                        {46, 2761, 2384, 2246, 1794, 2002, 2016, 2816, 2393},
                        {50, 2805, 2440, 2320, 1874, 2096, 2098, 2877, 2469}};

//Create left and right tracks
HBRIDGE left_motor = HBRIDGE(PB0, PA1);
HBRIDGE right_motor = HBRIDGE(PA7, PA3);
//Create scissor lift
SLIFT atb_slift = SLIFT(PA8);
//Create arm
//ARMCONTROL atb_arm = ARMCONTROL(PB8,PB9,PB12,PB1);
//
irsensor ewok_sensor = irsensor(EWOK_DETECTION, data_1);
irsensor line_sensor = irsensor(LINE_FOLLOWING, data_2);
irsensor edge_sensor_left = irsensor(EDGE_DETECTION_LEFT, data_3);
irsensor edge_sensor_right = irsensor(EDGE_DETECTION_RIGHT, data_4);

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    //Initialize the tracks, scissor lift, and arm
    left_motor.init();
    right_motor.init();
    atb_slift.init();
    ARMCONTROL::init(PB8, PB9, PB12, PB1);
}

void loop(void)
{
    //Arm Testing
    Serial.println("ARM HORIZONTAL");
    ARMCONTROL::armHorizontal();
    delay(DELAY_VAL);
    Serial.println("ARM SEARCH");
    ARMCONTROL::armSearch();
    delay(DELAY_VAL);
    Serial.println("ARM PICKUP");
    ARMCONTROL::armPickup();
    delay(DELAY_VAL);
    Serial.println("GRABBER OPEN");
    ARMCONTROL::grabberOpen();
    Serial.println("PRESS GRABBER SWITCH TO CONTINUE");
    while (!ARMCONTROL::switchStatus())
    {
        delay(10);
    }
    Serial.println("GRABBER CLOSING");
    ARMCONTROL::grabberHug();
    delay(DELAY_VAL);
    Serial.println("ARM VERTICAL");
     ARMCONTROL::armVertical();
    delay(DELAY_VAL);
    Serial.println("ARM DROP OFF");
     ARMCONTROL::armDropoff();
    delay(DELAY_VAL);
    Serial.println("GRABBER OPEN");
     ARMCONTROL::grabberOpen();
    delay(DELAY_VAL);
    Serial.println("ARM HORIZONTAL");
     ARMCONTROL::armHorizontal();
    Serial.println("ARM TESTING DONE");

    //Scissor lift Testing
    Serial.println("PRESS DOWN POSITION LIMIT SWITCH TO CONITNUE");
    while (!atb_slift.atDownLimit())
    {
        delay(10);
    }
    Serial.println("PRESS UP POSITION LIMIT SWITCH TO CONITNUE");
    while (!atb_slift.atUpLimit())
    {
        delay(10);
    }
    Serial.println("PRESS BASKET LIMIT SWITCH TO CONITNUE");
    while (!atb_slift.atBasketLimit())
    {
        delay(10);
    }
    Serial.println("MOVING TO UP POSITION");
    atb_slift.moveUp();
    delay(DELAY_VAL);
    Serial.println("STAING IN POSITION - we never actually need to use this function");
    atb_slift.stay();
    delay(DELAY_VAL);
    Serial.println("MOVING TO UP POSITION");
    atb_slift.moveDown();
    delay(DELAY_VAL);
    Serial.println("SCISSOR LIFT TESTING DONE");

    //Motor Testing
    Serial.println("LEFT MOTOR FORWARD");
    left_motor.run(180);
    delay(3000);
    Serial.println("STOP");
    left_motor.stop();
    delay(DELAY_VAL);
    Serial.println("LEFT MOTOR REVERSE");
    left_motor.run(-180);
    delay(3000);
    Serial.println("STOP");
    left_motor.stop();
    delay(DELAY_VAL);

    Serial.println("RIGHT MOTOR FORWARD");
    right_motor.run(180);
    delay(3000);
    Serial.println("STOP");
    right_motor.stop();
    delay(DELAY_VAL);
    Serial.println("RIGHT MOTOR REVERSE");
    right_motor.run(-180);
    delay(3000);
    Serial.println("STOP");
    right_motor.stop();
    delay(DELAY_VAL);

    Serial.println("BOTH MOTOR FORWARD");
    left_motor.run(180);
    right_motor.run(180);
    delay(3000);
    Serial.println("STOP");
    left_motor.stop();
    right_motor.stop();
    delay(DELAY_VAL);
    Serial.println("BOTH MOTOR REVERSE");
    left_motor.run(-180);
    right_motor.run(-180);
    delay(3000);
    Serial.println("STOP");
    left_motor.stop();
    right_motor.stop();
    delay(DELAY_VAL);

    Serial.println("LEFT MOTOR FORWARD RIGHT MOTOR REVERSE");
    left_motor.run(180);
    right_motor.run(-180);
    delay(3000);
    Serial.println("STOP");
    left_motor.stop();
    right_motor.stop();
    delay(DELAY_VAL);
    Serial.println("RIGHT MOTOR FORWARD LEFT MOTOR REVERSE");
    left_motor.run(-180);
    right_motor.run(180);
    delay(3000);
    Serial.println("STOP");
    left_motor.stop();
    right_motor.stop();
    delay(DELAY_VAL);

    //Sensor Array Tests
    Serial.println("PID SENSOR");
    delay(DELAY_VAL);
    current_time = millis();
    while (millis() - current_time < 15000)
    {
        line_sensor.update();
        for (int i = 0; i < 8; i++)
        {
            Serial.print(line_sensor.distance_readings[i]);
            Serial.print(",");
        }
        Serial.println("");
    }

    Serial.println("EWOK SENSOR");
    delay(DELAY_VAL);
    current_time = millis();
    while (millis() - current_time < 15000)
    {
        ewok_sensor.update();
        for (int i = 0; i < 8; i++)
        {
            Serial.print(ewok_sensor.distance_readings[i]);
            Serial.print(",");
        }
        Serial.println("");
    }

    Serial.println("LEFT EDGE SENSOR");
    delay(DELAY_VAL);
    current_time = millis();
    while (millis() - current_time < 15000)
    {
        edge_sensor_left.update();
        for (int i = 0; i < 8; i++)
        {
            Serial.print(edge_sensor_left.distance_readings[i]);
            Serial.print(",");
        }
        Serial.println("");
    }

    Serial.println("RIGHT EDGE SENSOR");
    delay(DELAY_VAL);
    current_time = millis();
    while (millis() - current_time < 15000)
    {
        edge_sensor_right.update();
        for (int i = 0; i < 8; i++)
        {
            Serial.print(edge_sensor_right.distance_readings[i]);
            Serial.print(",");
        }
        Serial.println("");
    }

    //Beacon Sensor Test
    Serial.println("TESTING BEACON DETECTION");
    current_time = millis();
    while (millis() - current_time < 15000)
    {
        int value = IRBEACON::read(PA5);
        if (value == 0)
            Serial.println("No Beacon Detected!");
        if (value == 1)
            Serial.println("1khz Beacon detected!");
        if (value == 2)
            Serial.println("10khz Beacon detected!");
        delay(50);
    }
    Serial.println("BEACON DETECTION DONE");

    Serial.println("TESTING COMPLETE, PROGRAM WILL RESTART IN 30 SECONDS");
    delay(30000);
}