# Ring Cycle 1: Basics
This is the first in a series of five reverse engineering challenges.

## Getting Started
Let's start by running the program and seeing what it does.
```
$ ./basics                          
What is the passphrase of the vault?
> passphrase
Wrong passphrase!
```

Alright, it looks like it is validating a passphrase. Let's run strings and see if there is anything interesting
```
$ strings basics
...
What is the passphrase of the vault?
basics.txt
flag{
%02x
Wrong passphrase!
...
```
Neat, so it looks like we can see the wrapper for the flag as well as the prompts we saw when running the program. Let's open it up in Ghidra and take a closer look.

## Ghidra
