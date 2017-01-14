# WAAC - Web-enabled Arduino Aquarium Controller

There are a few things I need to finish but if you would like to contribute, please do.
Here are videos of the development
https://www.youtube.com/playlist?list=PLBa21usjmOFKCHJmGBnlkjP8v2rVpoFGZ

1. Add more devices like pH sensor, simple push button, any device that can interface with the Arduino.

I decided not to do back up to settings. It's too complicated. It's best you stick a 9v battery to the arduino in case of a power outtage :)

#ABOUT
This a plug and play system where you add and control devices through a browser, basically, no coding involved. It's developed by Minh Truong and copyrighted under 2noodles llc. It is free to use with Copyright restrictions.


#Required Equipments
1. Arduino Due
2. Arduino Ethernet Shield (W5100) with SD card slot. Make sure the W5100 or whatever supports at least 4 simultaneous socket connections. A UDP connection is used to update time.
3. SD card

#Setup
1. Copy all .html, .js, .css files onto the SD card
2. Library files go into the Arduino Library folder

#Running
1. Wait 10 seconds for the Aduino to setup before browsing
2. Find the local network IP, (default) 192.168.1.177:8080 in a browser
3. Login (default) user: admin pass: waac

#Quick Links
* <a href="http://aquatictechtank.net/viewforum.php?f=48">Support</a>

#Donation
<a href='https://pledgie.com/campaigns/32780'><img alt='Click here to lend your support to: Support WAAC and make a donation at pledgie.com !' src='https://pledgie.com/campaigns/32780.png?skin_name=chrome' border='0' ></a>
