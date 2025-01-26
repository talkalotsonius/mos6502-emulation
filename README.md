# mos6502-emulation
Emulation of the classic video game console chip MOS Technology 6502 (mostly just called "the 6502") used in numerous home devices during the 1980s
Like the: Atari 2600, Apple II, Nintendo Entertainment System (NES), Commodore 64 (C64), BBC Micro and many others.
Some of this code was written by me back in the end 00s, and then somewhat abandoned due to may problems with insufficient testing. Later it was re-written.
This emulation was inspired by my love of retro consoles and wanted to focus on the inner-workings of CPUs (at the time). And understanding as well as checking if the crude approach of just recreating the technical / operational logic of a given processing unit is 1:1 depictable by, lets call it a long string of ifs and arrays.

## Description
We want to emulate the function of the 6502 using the C++ programming language and also have a valid way of testing it.
As a technical reference for all operations and registers we are working off of [Andrew Jacobs' (BitWise) work](http://www.6502.org/users/obelisk/) and supported by [the NES development wiki](https://wiki.nesdev.com/w/index.php/Status_flags) & [the C64 development wiki](https://www.c64-wiki.com/wiki/BRK) .
Furthermore we are making use of [Klaus2m5's work](https://github.com/Klaus2m5/6502_65C02_functional_tests) in the field of 6502 assembly code to test all of the chip's functions.
This is, or will, by all means not be a full and entire emulation environment to run i.e. a C64 game or program. For this to work we need more than just the logical units of a CPU. Graphics, Sound, general I/O (like data storage, user input, ...) all together make the emulation a running system. But of course, the CPU is the beating heart of any computer / console.

## Compilation & Installation
```
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH= <path-to-gcc> -DCMAKE_CXX_COMPILER:FILEPATH= <path-to-g++> --no-warn-unused-cli -S<path-to-PROJECT_ROOT>/mos6502-emulation -B<path-to-PROJECT_ROOT>/mos6502-emulation/build -G "Unix Makefiles"
```
```
cmake --build <path-to-PROJECT_ROOT>/mos6502-emulation/build --config Debug --target all -j 12 --
```
The executable of the tests will be located here:
```
mos6502-emulation/build/6502/Test/MOS6502Test
```

### Integrating [6502_65C02_functional_tests](https://github.com/Klaus2m5/6502_65C02_functional_tests)
On Linux/Unix, if not already installed, get the i386 libs for libc & libstdc++ since it is a 32bit program.
Run the following command:
```
./as65 -l -m -w -h0 6502_functional_test.a65
```
This will yield you: `6502_functional_test.bin` & `6502_functional_test.lst`
Based on the information from the Symbols file `6502_functional_test.lst`, we obtain the information:
```
Wrote binary from address $000a through $ffff.
Total size 65526 bytes.
Program start address is at $0400 (1024).
```
This will be an entry hook for the test: `TEST_F(LoadPrgTest, LoadThe6502TestPrg)` under `LoadPrgTest.cc`.
Direct to / or copy the `6502_functional_test.bin` to a desired location and modify the `fopen_s()`- function accordingly.

## Learning exercise
Now, what did we learn from this?

* Some, if not many, features of logical hardware are undocumented. Or, dependent on age, may have gone lost.
* Writing a CPU emulator is time consuming and in the end it just turns out to be just big array bytes ;)
* Emulating a CPU is harder you than you think! Accounting for electrical specifications like the clock, will result in even more complexity built on top of the cumbersome logical units.
* If you may ever start questioning tests, especially unit tests, of the smallest most stupid things in (coding) life, this will teach you!
* Should we really use switch statements for this? I don't want to even think of any x86 recreation like this! :D

## Acknowledgements
Some of the re-writes of this code are loosely based and (re-)inspired by @davepoo's "Emulating a CPU in C++" https://youtu.be/qJgsuQoy9bc