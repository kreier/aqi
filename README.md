# AQI - Air Quality Index

The air in Ho Chi Minh City is getting more and more polluted in recent years. Can we add some valid data to the smog and fog we see more often?

## Measure and educate

In order to understand the situation we want to build our own sensors and collect data. Building helps understand _WHAT_ AQI, PM10 and PM2.5 are and what the relation to our health is. The same time we learn _HOW_ to measure the quality of air

## Simple Arduino, Sharp sensor and 1602 display

You can get a finished sensor with Arduino, 1602 display and the sharp sensor GP2Y1010AU0F for 20 dollars or 499,000 VND:

## Luftdaten.info

Measure the AQI at AISVN and publish the data

## Database

Our Sensors:
- [12779556](https://www.madavi.de/sensor/graph.php?sensor=esp8266-12779556-sds011) Outdoor sensor in Nha Be at the campus of the AISVN
- [416796](https://www.madavi.de/sensor/graph.php?sensor=esp8266-1129419-sds011) Second sensor at AISVN, indoor in front of 407B as display
- [1129419](https://www.madavi.de/sensor/graph.php?sensor=esp8266-416796-sds011#l_week) Outdoor sensor in Quận 7, at Nội khu Hưng Phước 1, Tân Phong, Hồ Chí Minh, Vietnam

The measurements of the last year(s) are stored in the database as zip filed. Further project: import (automatically?) into a sqlite database and analyze/visualize with jupyter.
