# WAAC - Web-enabled Arduino Aquarium Controller

There are a few things I need to finish but if you would like to contribute, please do.
Here are videos of the development
https://www.youtube.com/playlist?list=PLBa21usjmOFKCHJmGBnlkjP8v2rVpoFGZ

1. Add more devices like pH sensor, simple push button, any device that can interface with the Arduino.
2. Add data logging capabilities. The data would be saved onto the SD cards and displayed on a highchart framework.

I decided not to do back up to settings. It's too complicated. It's best you stick a 9v battery to the arduino in case of a power outtage :)

<h2>ABOUT</h2>
This is a plug and play system where you add and control devices through a browser, basically, no coding involved. It's developed by Minh Truong and copyrighted under 2noodles llc. It is free to use with Copyright restrictions.


<h2><a href="https://github.com/mistergreen/WAAC/tree/master/Arduino_Due">Required Equipments for the Arduino version</a></h2>
1. Arduino Due
2. Arduino Ethernet Shield (W5100) with SD card slot. Make sure the W5100 or whatever supports at least 4 simultaneous/concurrent socket connections. A UDP connection is used to update time. I hear the new WIFI shield (HDG204) supports 4 sockets. You'll have to change the Ethernet code to support the WIFI shield.
3. SD card

<h2><a href="https://github.com/mistergreen/WAAC/tree/master/Arduino_Due">Required Equipments for the ESP8266 version</a></h2>
1. WEMOS D1 mini
2. WEMOS SD shield
3. SD card
4. Bidirectional logic converter
5. (optional) Arduino Mega, 5v to 3.3V voltage step down


<h2>Setup (Arduino)</h2>
1. Copy all .html, .js, .css files onto the SD card
2. Library files go into the Arduino Library folder

<h2>Setup (ESP8266)</h2>
1. Copy all .html, .js, .css files onto the SD card
2. Library files go into the Arduino Library folder


<h2>Running</h2>
1. Wait 10 seconds for the Aduino to setup before browsing
2. Find the local network IP, (default) 192.168.1.177:8080 in a browser
3. Login (default) user: admin pass: waac

<h2>Quick Links</h2>
* <a href="http://aquatictechtank.net/viewforum.php?f=48">Support</a>

<h2>Donation</h2>
<a href='https://pledgie.com/campaigns/32780'><img alt='Click here to lend your support to: Support WAAC and make a donation at pledgie.com !' src='https://pledgie.com/campaigns/32780.png?skin_name=chrome' border='0' ></a>
