#include <Wire.h>
#include <irsensor.h>

//detect ewoks
int lookup_table_1[20][9] = {{1, 131, 125, 134, 132, 129, 133, 135, 140},
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

//line following
int lookup_table_2[20][9] = {{1, 125, 121, 128, 125, 124, 131, 134, 148},
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
                             {50, 1660, 1640, 2240, 1999, 1590, 2055, 2255, 2780}};

//sense left edge
int lookup_table_3[20][9] = {{1, 139, 136, 144, 132, 133, 134, 138, 145},
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

//sense right edge
int lookup_table_4[20][9] = {{1, 137, 126, 124, 120, 126, 124, 136, 136},
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




irsensor front_sensor = irsensor(0x48, lookup_table_1);
irsensor bottom_sensor = irsensor(0x49, lookup_table_2);
irsensor left_sensor = irsensor(0x4A, lookup_table_3);
irsensor right_sensor = irsensor(0x4B, lookup_table_4);

void setup()
{
  Serial.begin(9600); // Start Serial
  Serial.println("Sketch begin");
  Wire.begin();
}

void loop()
{
  bottom_sensor.update();
  for (int i = 0; i < 8; i++)
  {
    Serial.print(bottom_sensor.distance_readings[i]);
    Serial.print(",");
  }
  Serial.println();
  Serial.print("Inverse weighted mean:");
  Serial.print(bottom_sensor.inverse_weighted_mean());
  Serial.println();
  Serial.print("Mean:");
  Serial.print(bottom_sensor.mean());
  Serial.println();
  Serial.print("Weighted Mean:");
  Serial.print(bottom_sensor.weighted_mean());
  Serial.println();
  delay(1000); // Wait for 1 second
}