# WAAC - Web-enabled Arduino Aquarium Controller

This version of WAAC starts from this project https://github.com/mistergreen/WAAC and tries to develop some code specifically for ESP32.

In particular:
<ul>
  <li>Internal storage has been added, so all configuration can be stored in the internal flash of the ESP32.</li>
  <li>EzTime library is used instead of the default time library. This allows to use timezones instead of fixed time shifts.</li>
  <li>Servo motor library has been added to potentially handle an automated fish feeder (phisically under development).</li>
</ul>


<h2>ABOUT</h2>
This is a plug and play system where you add and control devices through a browser, basically, no coding involved for the <strong>Arduino</strong> and <strong>ESP</strong> micro controllers. It's developed by Minh Truong and copyrighted under 2noodles llc. Additional parts in this fork have been implemented by Andrea Villa. It is free to use with Copyright restrictions.

