# WAAC - Web-enabled Arduino Aquarium Controller

Thanks to https://github.com/Avilla83/WAAC for setting up the new event structure and the JSON serialization mechanism.
This version is ESP32 only.

<strong>Features:</strong>
<ul>
  <li>Internal storage has been added, so all configuration can be stored in the internal flash of the ESP32.</li>
  <li>EzTime library is used instead of the default time library. This allows to use timezones instead of fixed time shifts.</li>
  <li>Servo motor library has been added to potentially handle an automated fish feeder (physically under development).</li>
  <li>The HTML configuration pages send JSON back to the app to deserialize. Cleaner code and flow but make sure not send back wrong type or NULL or the app will crash.</li>
</ul>

<strong>Bug Fixes:</strong>
<ul>
  <li>Updated libraries</li>
  <li>LEDC API v3 updates</li>
  <li>Hall flow sensor crashing fix</li>
  <li>Time event midnight time rollover fix. Events didn't trigger when scheduled to the current day and the duration takes it to the next day.</li>
</ul>

<strong>Setting up:</strong>
In file WAAC_ESP32_JSON.ino
<ul>
  <li>line 19  #define USE_SPIFFS  true</li>
  Set true to use SPIFF or false to use an SD reader
  <li>Upload the all the contents of Html_esp32 and Json folder to the SPIFF or SD card</li>
  <li>You can change the web GUI user/pass on line 167 & 168</li>
  <li>Setting up the server network static IP or setting a hostname on lines 241 through 250. Use one or the other.</li>
  <li>Set up your Wifi ssid & password in settings.json. If you change routers, you can change the ssid/pass on the json instead of recompile the app</li>
  <li>If you want to set up your own email script, check out the PHPscript folder</li>
</ul>

<strong>Run the App:</strong>
<ul>
  <li>Log into the webGUI with the static IP or hostname you assigned.</li>
  <li>Set your timezone in settings</li>
  <li>You can set your email if you want alerts from the app. You can use SMS email if your cellphone service allows.</li>
  <li>If you want to play with public IP for WWW access, play with the Public IP management</li>
</ul>

You're set to add a relay or sensor. Use pin numbers only, no alias like A10, A16.


<h2>ABOUT</h2>
This is a plug and play system where you add and control devices through a browser, basically, no coding involved for the <strong>ESP32</strong> micro controllers. It's developed by Minh Truong and copyrighted under 2noodles llc. Additional parts in this fork have been implemented by Andrea Villa. It is free to use with Copyright restrictions.

