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
<ol>
<li>Arduino Due</li>
<li>Arduino Ethernet Shield (W5100) with SD card slot. Make sure the W5100 or whatever supports at least 4 simultaneous/concurrent socket connections. A UDP connection is used to update time. I hear the new WIFI shield (HDG204) supports 4 sockets. You'll have to change the Ethernet code to support the WIFI shield.</li>
<li>SD card</li>
</ol>

<h2><a href="https://github.com/mistergreen/WAAC/tree/master/ESP8266">Required Equipments for the ESP8266 version</a></h2>
<ol>
<li>WEMOS D1 mini</li>
<li>WEMOS SD shield</li>
<li>SD card</li>
<li>(optional for additional digial output) Arduino Mega, Bidirectional logic converter, 5v to 3.3V voltage step down</li>
</ol>

<h2>Setup (Arduino)</h2>
<ol>
<li>Copy all .html, .js, .css files onto the SD card</li>
<li>Library files go into the Arduino Library folder</li>
</ol>

<h2>Setup (ESP8266)</h2>
<ol>
<li>Copy all .html, .js, .css files onto the SD card</li>
<li>Library files go into the Arduino Library folder</li>
<li>Install the hardware package describe here https://www.wemos.cc/tutorial/get-started-arduino.html
<li>Install WAAC_8266 onto the WEMOS</li>
<li>If you want additional digital output pins, combine the WEMOS with an Arduino Mega. The WEMOS communicates with the Arduino via Serial3.</li>
</ol>

<h2>Running</h2>
<ol>
<li>Wait 10 seconds for the Aduino to setup before browsing</li>
<li>Find the local network IP, (default) 192.168.1.177:8080 in a browser</li>
<li>Login (default) user: admin pass: waac</li>
</ol>

<h2>Quick Links</h2>
* <a href="http://aquatictechtank.net/viewforum.php?f=48">Support</a>

<h2>Donation</h2>
<a href='https://pledgie.com/campaigns/32780'><img alt='Click here to lend your support to: Support WAAC and make a donation at pledgie.com !' src='https://pledgie.com/campaigns/32780.png?skin_name=chrome' border='0' ></a>
