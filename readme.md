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

* make smaller window 
* add more theses for kicks
* test system sounds, maybe add more 
* feature: add option to make the after shutup
* feature: add option ot make it stop flashing when after alert 
* provide some cool sounds 
* Options messages during aLERT
* 
