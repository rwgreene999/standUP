# What this project is about

THIS IS A WORK IN PROGRESS, NOT COMPLETE
  
This is a small Qt desktop app, built around main.cpp, called “Chair Reminder.” Its purpose is to help someone avoid sitting too long by setting a timer and reminding them to take a break or stand up.

## this is Version 0.1 
This is the first prototype of this program for linux.  After switching to linux, I 
abandoned TimeToy a rich windows app, also on Github. I wanted to move back to my roots, with cpp.  

## Build and run: 

This is build under vs-code, but you can build it without vs-code also. 

It uses cmake which you might need to install: ( sudo apt install cmake ) 

```  
cd /home/rwg/projects/standUP
rm -rf build
cmake -S . -B build
cmake --build build 
--parallel
```
or in one line 
```
rm -rf build && cmake -S . -B build && cmake --build build --parallel && ./build/standUP
```
 




## to play music 
If you choose to play music when the standUP alert happens, 
this app tries some standard methods 
The app tries to find an installed player such as:

    ffplay
    vlc
    cvlc
    mpg123
    mpg321
    mplayer

Also I had to install and use pavucontrol to see what I was playing music to a device that was not connected to my machine and force it to use the headset which was the external speakers.  

## where this  is going 

* DONE: make smaller window 
* DONE: On flash alert, click anywhere to end flash
* DONE: Added settings savings in config file
* DONE: VersionInfo add version info to title bar, using cmake process
* DONE: MusicTest 0.2.0 option to make the after shutup
* DONE: FixElapse 0.3.0 fix the elapse time to update when user pauses and restarts 
* DONE: ScreenUpdate 0.4.0 move manual set time below the buttons 
* DONE: SettingsTest 0.5.0 provide ability to change settings without saving for testing 
* DONE: SettingsUpdate 0.6.0 add 2 themes change settings to show color changes before save 
* DONE: AudioUpdate 0.7.0 added updates to qt6 playback as suggested by Gemini, tested multiple machines 
* DONE: AudioSounds 0.8.0 Added some extra beeps
* DONE: AudioSounds2 0.9.0 Changed the order of the should dropdown list 
* 
* Feature: add flash color choices 
* Bug: Bring to foreground did not work on my computer called Owl. 
* Feature: AlertMessage In settings, allow user options for alert 
* Feature: RepeatAlert Add a repeat button, stop current alert and start new one 
* 
* 
