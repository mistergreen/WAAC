This is still work in progress. I haven’t fully debug this. I'll try to add more device components to the core code.
<h2>Features</h2>
<ol>
<li>User Login</li>
<li>NTP time sync</li>
<li>Email Alerts</li>
<li>Youtube streaming video connection</li>
<li>Public IP Management</li>
</ol>

<h2>Supported Device</h2>
<ol>
<li>Analog Input</li>
<li>Digital Output & Input</li>
<li>PCA9685 Digital Output</li>
<li>MCP23017 Digital Output & Input</li>
<li>Flow Sensor (Hall sensor)</li>
<li>PWM (12 bit)</li>
<li>PWM (PCA9685 I2C Servo Driver - 12 bit)</li>
<li>One Wire Sensor (DS18B20 temperature probe)</li>
</ol>

<h2>Required Equipments for the ESP32</h2>
<ol>
<li>ESP32 - I’m using WEMOS lolin32</li>
<li>SD card reader</li>
<li>SD card</li>
</ol>

<h2>Setup</h2>
<ol>
<li>Copy all .html, .js, .css files onto the SD card</li>
<li>Library files go into the Arduino Library folder</li>
</ol>

<h2>Running</h2>
<ol>
<li>Change the network name and password to your network in the main .ino file</li>
<li>Browse to 192.168.1.177. Static IP works with the 3/2018 ESP32 master</li>
<li>Login (default) user: admin pass: waac</li>
</ol>
