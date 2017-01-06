# WAAC - Web-enabled Arduino Aquarium Controller
Alpha 0.01

There are a few things I need to finish but if you would like to contribute, please do.

1. Add simple device interface like digital devices like buttons
2. Possible use NTP (Network Time Protocol) instead of RTC to keep time

I decided not to do back up to settings. It's too complicated. It's best you stick a 9v battery to the arduino in case of a power outtage :)

#Required Equipments
1. Arduino Due
2. DS1307 RTC
3. Arduino Ethernet Shield (W5100) with SD card slot. Make sure the W5100 or whatever supports at least 4 simultaneous socket connections. A UDP connection is used to update time.
4. SD card

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
