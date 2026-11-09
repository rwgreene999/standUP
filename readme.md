# What this project is about

This is working code!
  
This is a small Qt desktop app, built around main.cpp, called “Chair Reminder.” Its purpose is to help someone avoid sitting too long by setting a timer and reminding them to take a break and standUP.

* Runit with: standUP 
* Push a button to get notified in 30 minutes or one hour or other options  
* Get to work, this will notify you when the time is up
* for fun, go to settings, pick sound for the for your notification, or colors 

## this is Version 0.11
A working Linux program.  After switching to linux, I 
abandoned TimeToy a rich Windows app, also on Github. 
I wanted to move back to my roots, with cpp.  

## Build and run: 

This is build under vs-code, but you can build it without vs-code also. 

It uses cmake which you might need to install: ( sudo apt install cmake ) 

```  
cd /home/rwg/projects/standUP
rm -rf build
cmake -S . -B build
cmake --build build --parallel
```
or in one line 
```
rm -rf build && cmake -S . -B build && cmake --build build --parallel && ./build/standUP
```
one line to build and also make deb file ...
```
rm -rf build && cmake -S . -B build && cmake --build build --parallel && (cd build && cpack -G DEB) && ./build/standUP
```



## to play music 
Not sure I understand Linux sound processing, 
but I use qt6 which is supposed to handle this for me.  
Before switching sound to qt6, the app would tries to find an installed player such as:

    ffplay
    vlc
    cvlc
    mpg123
    mpg321
    mplayer

Also I had to install and use pavucontrol to see what I was playing music to a device that was not connected to my machine and force it to use the headset which was the external speakers.  


## where this has gone 

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
* DONE: Installer "0.10.0 updated cmake build package to create deb installer" 
* DONE: readme "0.11.0 updated readme" 
* DONE: ChangeTimes "0.12.0 change time buttons to more meaningful data"
* DONE: AddStopwatch "0.13.0 Add stopwatch functionality, Buttons and second panel  "
* Done: RazzleDazzle "0.14.0 Add more sounds and colors" 
* DONE: Icon "0.15.0 first use of opencode-Big Pickle in code to create icon"
* DONE: RazzleDazzle2 "0.16.0 option of random for color or sound"
* DONE: LayoutCleanup "0.17.0 move settings to a header gear and separate stopwatch controls"

## where this is going 
* Feature: add flash color choices 
* Bug: Bring to foreground done not always work
* Feature: AlertMessage In settings, allow user options for alert 
* Feature: RepeatAlert Add a repeat button, stop current alert and start new one 
* 
* 
