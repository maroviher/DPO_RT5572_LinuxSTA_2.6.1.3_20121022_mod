Improved Linux Driver for Railnk USB WiFi Cards RT8070 RT3070 RT3370 RT3572 RT5370 RT5372 RT5572
Based on original Ralink Driver v2.6.1.3 (10/22/2012)

<b>Features:</b><br>
* fixed compilation error on kernels > 3.5<br>
* LED. AWUS036NH. On original driver if you compile without LED support (HAS_LED_CONTROL_SUPPORT in os/linux/config.mk) than your LED stays on all the time the WiFi card switched on. In this driver the LED will be switched off after you "ifconfig ra0 up". If you just recompiled the driver without LED support, its not enought to just re-insmod the new compiled driver, you must unpulg your card and plugin again, in order the changes to take effect.<br>

<b>Reverse engineered undocumented features:</b>
* change MAC through configuration file RT2870STA.dat (MacAddress=12:34:45:67:ad:be as first parameter)
* turn off annoying debug messages in kernel log (set debug level, run "iwpriv ra0 set Debug=0", possible levels 0~5)


<b>internal, raspi cross compile:<br></b>
* git clone https://github.com/maroviher/DPO_RT5572_LinuxSTA_2.6.1.3_20121022_mod<br>
* cd DPO_RT5572_LinuxSTA_2.6.1.3_20121022_mod/DPO_RT5572_LinuxSTA_2.6.1.3_20121022<br>
* # in Makefile uncomment PLATFORM = RASPI. in os/linux/config.mk remove -DRTMP_INTERNAL_TX_ALC in "ifneq ($(findstring 5370,$(CHIPSET)),)" section, otherweise you will get problems with rt5370 made in China ministick<br>
* # set path to toolchain in your $PATH
* cd raspi/<br>
* # compile
* cards="2870 3070 5370" ; for card in $cards ; do ./cross_compile.sh $card arm-linux-gnueabihf- /media/root/967cec13-9afa-45fb-b861-cb3255f6275d/rpi_kernel_23.03.2015/linux ; done<br>
* # here are the drivers
* ls ../os/linux/*.ko
* # to clean all
* make clean ; rm -f ap/.* ; rm -f chips/.* ; rm -f common/.* ; rm -f os/linux/.* ; rm -f os/linux/*.mod.c ; rm -f os/linux/*.o ; rm -f os/linux/*.mod.o ; rm -f os/linux/*.ko ; rm -f rate_ctrl/.* ; rm -f sta/.* ; rm -f tools/bin2h
