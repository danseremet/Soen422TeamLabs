

## BeagleBone Black Setup

### 1.0 Hardware Setup

- Connect BBB to PC.
- SSH into the BBB.
- `sudo su`.
- Run the configure SPI pins script.
- `sudo x11vnc -xkb -noxrecord -noxfixes -noxdamage -display :0 -auth /var/run/lightdm/root/:0 -usepw`

- http://www.ofitselfso.com/BeagleNotes/Beaglebone_Black_And_Device_Tree_Overlays.php
- http://ulasdikme.com/projects/arduino/BBB_Arduino_SPI.php
- https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#U-Boot_Capes_added:

### Enable BBB wifi to PC
* Have your computer connected to Ethernet (since wifi can't be used when it is a transmitter for the Wifi Hotspot)
* `connmanctl`
* `agent on`
* Make sure your wifi Hotspot is active.
* `scan wifi` Wait ~10 seconds.
* `services` Look for your wifi hotspot id that looks like `wifi_"some numbers"_managed_psk`.
* `connect wifi_"some numbers"_managed_psk`. 
* Test your connection: `ping www.google.com`
* Note: It can take some time before it has a connection.

### 2.0 Software setup
-  main program server

## Arduino Nano Setup

### 1.0 Hardware Setup

#### Left motor

- Connect GND and 5V to HV GND and 5V on the Level shifter.
- Connect SCK, MISO, MOSI, CS to the Level Shifter 4 HV (5V) channels.
##### LEFT MOTOR H-bridge configuration
     * Add 5V and GND sinks to H-Bridge
     * PD6 - OC0A out     -- 1,2 EN
     * PD4 - digital out  -- 1A CW    -- Backwards
     * PD2 - digital out  -- 2A CCKW  -- Forward
- Connect SCK, MISO, MOSI, CS from the Level Shifter 4 LV (3.3V) channels to the BBB pins
    * P9.17     cs      to CS
    * P9.18     d1      to MOSI
    * P9.21     d0      to MISO
    * P9.22     clk     to CLK    


### Run
In project folder:
Terminal 1:
'./connect-to-BBB'
'cd bbbRobotServer'
'./start'

Terminal 2:
'./build-robotGUI-Desktop-Release/robotGUI'