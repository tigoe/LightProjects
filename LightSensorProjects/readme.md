# Light Sensor Data Logging

This project shows how to log data from a light sensor on a newtorked server. Thanks to Don Coleman and Mathura Govindarajan for this excellent [mySQL and node.js server](https://github.com/don/itp-connected-devices) for datalogging. 

The sensor is a [TCS34725 color sensor](https://ams.com/tcs34725) from AMS using [Adafruit's library](https://github.com/adafruit/Adafruit_TCS34725) for it. AMS has some pretty good [application notes](https://ams.com/tcs34725#tab/documents) on this sensor too.


The sensor sends data to a server every five minutes. 
