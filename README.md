# powerupp
**Simple GUI for UPP (https://github.com/sibradzic/upp)**

![Screenshot](http://bufonaturvard.se/pics/powerupp2.png)

This is a tool for easy GUI adjustments (of a selected number) of values in the AMD PowerPlay table on Linux. For now only Navi 10 (Radeon 5000 series*) graphic cards are supported. If you can provide data for other graphic cards, please do so!

The reason for creating this is that the OverDrive settings were still not working for Navi 10 cards when I started working on it (kernel <5.5). Thanks to sibradzic and his work with UPP it is possible to adjust the settings anyway (and no need to set `ppfeaturemask` kernel options), but it can be hard to interpret the vast number of settings from the `pp_table` file, hence this GUI. 

For those of you who own a Navi 10 card and are on a kernel <5.5 and want to overclock/undervolt etc., or if you want to tinker with values not available in the OverDrive settings for more extreme or precise overclocking - this application can make the process easier.

Despite the simple appearence, this is a powerful tool that can potentially damage your graphics card if care is not taken. It is in early development and my first attempt at coding in C (probably a whole lot of bad code examples, but I will try to improve the code base over time). On my system it is fully functional in its current state, but use at your own risk!

If you think some important settings are missing/should be implemented (for fan control check out CoreCtrl or radeon-profile, won't bother with that) or have any other problems, please file an issue. Personally I have little experience with overclocking, but I think the included settings should be the most important ones. For optimal settings you will need to look elsewhere or experiment on your own, I only use it to undervolt my 5700 XT for lower noise and power usage.

* The AMD 5600 XT card has got some strict firmware limitations, which seems to prevent the Gfx clock to be set higher than stock settings via the `pp_table`. It is, however, possible to adjust the other settings and then overclock up to the OverDrive limit using other (OverDrive) tools. For a lengthier discussion regarding this, see issue [#1](https://github.com/azeam/powerupp/issues/1).

**Note**  
Due to resolving issue [#4](https://github.com/azeam/powerupp/issues/4) and moving the UPP dependencies to a pip package instead of a git submodule it is best to clean up a couple of files that are no longer used if you have installed an old version of PowerUPP (since before starting to use UPP as a pip package). The makefile will help to clean them up, after cloning the git repository do:

`sudo make uninstall`  

And then reboot before starting the new PowerUPP install if you have any settings applied, in order to load the stock settings.

**Note 2**  
If you have trouble adjusting the power limit, this may be caused by a firmware bug, see [#3](https://github.com/azeam/powerupp/issues/3) for a workaround.

**Dependencies**  
GTK3 (dev)  
GCC  
Python 3.6+  
pip  
UPP 0.0.7+, installed as pip package: `pip3 install upp`  

For Ubuntu:  
1. `sudo apt-get install libgtk-3-dev build-essential python3 python3-pip`  
2. `pip3 install upp`

**Installation**  
`git clone https://github.com/azeam/powerupp.git`  
`cd powerupp`  
`make`  
`sudo make install`

**Uninstall**  
`sudo make uninstall`  

**Usage**  
The install command will install a .desktop launcher file, which should be picked up by Gnome etc. If you experience problems and need a little more detailed output you can also run the application from terminal (`powerupp`).

PowerUPP supports multiple graphic cards, select the card you want to adjust from the dropdown menu. "Load active" settings the first time you use the application, this will read and translate the contents of your `pp_table` file(s). These values will be saved as "defaults" in the folder `/home/USER/.config/powerupp/` and can later be used to reset the values to stock. I recommend not to edit this file manually. 

If you have set custom powerplay values before using this program or delete the defaults file after creating it, *beware that the default "safe" limits may not be correct*. The "safe" limits are set to the values present in AMD:s OverDrive table (same as in Wattman etc.), if available.

When you apply your settings they will immediately take effect, but will not persist a reboot. Note that *all* the displayed values will be written to the `pp_table` when pressing "Apply current", altering one value at a time is advisable. 

To keep the current settings active after a reboot, use the "Persistent save" option. This will create a bash script and a udev rules file that loads the script on system startup. Note that "Persistent save" will read and save the values displayed in the application and not the currently active settings. Uninstalling the application using `sudo make uninstall` from the git directory will also disable and remove the startup scripts if they are enabled, but not the saved profiles or default values.