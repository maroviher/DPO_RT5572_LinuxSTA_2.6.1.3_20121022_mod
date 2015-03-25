Improved Linux Driver for Railnk USB WiFi Cards RT8070 RT3070 RT3370 RT3572 RT5370 RT5372 RT5572
Based on original Ralink Driver v2.6.1.3 (10/22/2012)

<b>Features:</b><br>
* fixed compilation error on kernels > 3.5<br>
* LED. On original driver if you compile without LED support (HAS_LED_CONTROL_SUPPORT in os/linux/config.mk) than your LED stays on all the time the WiFi card switched on. In this driver the LED will be switched off after you "ifconfig ra0 up". If you just recompiled the driver without LED support, its not enought to just re-insmod the new compiled driver, you must unpulg your card and plugin again, in order the changes to take effect.<br>

<b>Reverse engineered undocumented features:</b>
* change MAC through configuration file RT2870STA.dat (MacAddress=12:34:45:67:ad:be as first parameter)
* turn off annoying debug messages in kernel log (set debug level, run "iwpriv ra0 set Debug=0", possible levels 0~5)


<b><br>internal, raspi cross compile:<br></b>
git clone https://github.com/maroviher/DPO_RT5572_LinuxSTA_2.6.1.3_20121022_mod<br>
cd DPO_RT5572_LinuxSTA_2.6.1.3_20121022_mod/DPO_RT5572_LinuxSTA_2.6.1.3_20121022/raspi/<br>
./cross_compile.sh 3070 arm-linux-gnueabihf- /media/root/967cec13-9afa-45fb-b861-cb3255f6275d/rpi_kernel_23.03.2015/linux
