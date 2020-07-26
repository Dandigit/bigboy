# Bigboy
A work-in-progress Gameboy emulator. See the write-up on [my blog](https://dandigit.com/posts/bigboy-writing-a-gameboy-emulator)!

## Screenshots
<p>
<img src="https://dandigit.com/assets/img/pokemon-1.png" width="25%" height="25%" style="display:inline-block;" />
<img src="https://dandigit.com/assets/img/pokemon-2.png" width="25%" height="25%" style="display:inline-block;" />
<img src="https://dandigit.com/assets/img/pokemon-3.jpeg" width="25%" height="25%" style="display:inline-block;" />
<p>

## Features
- Plays Tetris and Pokemon
- Faithful green screen tint
- (Hopefully) multiplatform
- Supports MBC1/MBC3/MBC5
- Supports real-time clock
- Passes Blargg's CPU tests

## Still working on it
- Dr. Mario gets to the title screen and menu, before disabling the display and rendering the game unplayable
- No audio support (that's what all the "No memory device registered for ..." are)
- No support for other MBC chips
- No graphical interface

## Usage
```
$ bigboy [gameboy-rom-path]
```

## Building
Unfortunately, I've only tested this on macOS so far. In theory, it should work fine on Windows and Linux too, but theory is theory. If you'd like to have a crack it should be pretty easy:
```
$ git clone https://github.com/Dandigit/bigboy
$ cmake .. .
$ make bigboy-bin
$ ./bigboy-bin ./tests/cpu-instrs.gb
```
