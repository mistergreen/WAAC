<h2><a href="https://github.com/mistergreen/WAAC/tree/master/Arduino_Due">Required Equipments for the Arduino version</a></h2>
<ol>
<li>Arduino Due</li>
<li>Arduino Ethernet Shield (W5100) with SD card slot. Make sure the W5100 or whatever supports at least 4 simultaneous/concurrent socket connections. A UDP connection is used to update time. I hear the new WIFI shield (HDG204) supports 4 sockets. You'll have to change the Ethernet code to support the WIFI shield.</li>
<li>SD card</li>
</ol>

<h2>Setup (Arduino)</h2>
<ol>
<li>Copy all .html, .js, .css files onto the SD card</li>
<li>Library files go into the Arduino Library folder</li>
</ol>

<h2>Running</h2>
<ol>
<li>Wait 10 seconds for the Aduino to setup before browsing</li>
<li>Find the local network IP, (default) 192.168.1.177:8080 in a browser</li>
<li>Login (default) user: admin pass: waac</li>
</ol>
