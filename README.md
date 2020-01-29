# powerupp
**Simple GUI for UPP (https://github.com/sibradzic/upp)**

![Screenshot](http://bufonaturvard.se/pics/poweruppscreenshot.png)

This is a tool for easy GUI adjustments (of a selected number) of values in the AMD PowerPlay table. For now only Navi 10 (Radeon 5000 series) graphic cards are supported. If you can provide data for other graphic cards, please do so!

The reason for creating this is that the pp_od_clk_voltage settings were still not working for Navi 10 cards when I started working on it (now, it is supposedly implemented in kernel 5.5, have yet to test it). Thanks to sibradzic and his work with UPP it is possible to adjust the settings anyway (and no need to set ppfeaturemask boot options), but it can be hard to interpret the vast number of settings from the pp_table file, hence this GUI. The UPP files are included in this git repository and will be "installed" with the "make install" command.

For those of you who own a Navi 10 card and are on a kernel <5.5 and want to overclock/undervolt etc., or if you want to mess around with values not available in the OverDrive settings for more extreme or precise overclocking - this application can make the process easier.

Despite the simple appearence, this is a powerful tool that can potentially damage your graphics card if care is not taken. It is in early development and my first attempt at coding in C (probably a whole lot of bad code examples, but I will try to improve the code base over time). On my system it is fully functional in its current state, but use at your own risk!

If you think some important settings are missing/should be implemented (for fan control check out CoreCtrl or radeon-profile, won't bother with that) or have any other problems, please file an issue. Personally I have little experience with overclocking, but I think the included settings should be the most important ones. For optimal settings you will need to look elsewhere or experiment on your own, I only use it to undervolt my 5700 XT for lower noise and power usage.

**Build dependencies**  
GTK3 (for Ubuntu: apt-get install libgtk-3-dev)

**Runtime dependencies (for UPP)**  
Python 2.7 or 3.6+, codecs, collections, struct, click. (for Ubuntu this should be enough: apt-get install python3 python3-click)

**Installation**  
git clone --recurse-submodules https://github.com/azeam/powerupp.git  
cd powerupp  
make  
sudo make install  

**Uninstall**
sudo make uninstall  

**Usage**  
The install command will place a .desktop launcher file in /usr/share/applications, which should be picked up by Gnome etc. If you experience problems and need a little more detailed output you can also run the application from terminal (powerupp).

PowerUPP supports multiple graphic cards, select the card you want to adjust from the dropdown menu. "Load active" settings the first time you use the application, this will read and translate the contents of your pp_table file(s). These values will be saved as "defaults" in the folder /home/USER/.config/powerupp/ and can later be used to reset the values to stock. Do not edit these files manually. 

If you have set custom values elsewhere before using this program or delete the default files after creating them, *beware that the default "safe" values may not be correct*. The "safe" limits are set to the values present in AMD:s OverDrive table (same as in Wattman etc.), if available.

When you apply your settings they will immediately take effect, but will not persist a reboot. Note that *all* the displayed values will be written to the pp_table when pressing "Apply current", altering one value at a time is advisable. 

To keep the current settings active after a reboot, press the "Persistent save" button. This will create a bash script and a systemd file that loads the script on system startup. Note that "Persistent save" will read and save the values displayed in the application and not the currently active settings.
