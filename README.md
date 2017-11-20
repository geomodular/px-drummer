# PX drummer
Simple drum machine with 1bit graphics and SID sound samples

Features:

 * Tempo range 40 - 240 bpm
 * 3/4 and 4/4 time signatures
 * 6 channels
 * Maximum of 16 patterns
 * Channel mute
 * Easy to use interface

![Example of use](doc/usage.gif)

## Motivation
I play the bass and I was in need of some drums to study rhythms.

## Build instructions
You need SDL2 library together with SDL2 mixer and SDL2 image. Project is buildable using c99 standard.

```
cd px-drummer
make
```

After compilation the binary should be in bin folder.
