I'm going to drop this project. The arduino was used as an add-on to expand the ESP8266 digital pins. The ESP32 is more useable now and I'll go that direction instead.

<h2>Features</h2>
<ol>
<li>NTP time sync</li>
<li>Email Alerts</li>
<li>Youtube streaming video connection</li>
<li>Public IP Management</li>
</ol>

<h2>Supported Device</h2>
<ol>
<li>Analog Input</li>
<li>Digital Relay</li>
<li>PWM (10 bit)</li>
<li>PWM (PCA9685 I2C Servo Driver - 12 bit)</li>
<li>One Wire Sensor (DS18B20 temperature probe)</li>
</ol>


<h2><a href="https://github.com/mistergreen/WAAC/tree/master/ESP8266">Required Equipments for the ESP8266 version</a></h2>
<ol>
<li>WEMOS D1 mini</li>
<li>WEMOS SD shield</li>
<li>SD card</li>
<li>(optional for additional digial output) Arduino Mega, Bidirectional logic converter, 5v to 3.3V voltage step down</li>
</ol>

<h2>Setup (ESP8266)</h2>
<ol>
<li>Copy all .html, .js, .css files onto the SD card</li>
<li>Library files go into the Arduino Library folder</li>
<li>Install the hardware package describe here https://www.wemos.cc/tutorial/get-started-arduino.html
<li>Install <a href="https://github.com/mistergreen/WAAC/tree/master/ESP8266/WAAC_8266">WAAC_8266</a> onto the WEMOS</li>
<li>If you want additional digital output pins, combine the WEMOS with an Arduino Mega. The WEMOS communicates with the Arduino via Serial3. Install <a href="https://github.com/mistergreen/WAAC/tree/master/ESP8266/WAAC_Arduino_addon">WAAC_Arduino_addon</a> onto the Arduino</li>
</ol>

<h2>Running</h2>
<ol>
<li>Wait 10 seconds to setup before browsing</li>
<li>Find the local network IP, (default) 192.168.1.177:8080 in a browser</li>
<li>Login (default) user: admin pass: waac</li>
</ol>

