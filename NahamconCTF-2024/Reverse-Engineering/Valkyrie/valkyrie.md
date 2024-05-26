# Valkyrie

## Getting started
I had just solved the first two challenges in the series, Basics and Rhinegold. These followed a similar pattern where you enter a passphrase, and if it is correct, then it spits out the flag. It was helpful to complete these two beforehand, as their code follows a similar pattern, with each level having more obfuscation than the last.

Let's try running the binary to see if it is the same.
```
$ ./valkyrie
What is the passphrase of the vault?
> passphrase
Wrong passphrase!
```
Yup! Looks to follow the same format as the other two. The first thing I tried on all the reversing challenges was strings. The previous two challenges contained strings "What is the passphrase", "Wrong passphrase", and "flag{" (without the actual flag). Unfortunately, we are not so lucky with this one. There weren't any strings that were terribly helpful.

## Ghidra
Opening it up in Ghidra, at first glance it was much more confusing than the previous two challneges. There were a ton of variables and lot's of binary operations chained together. After skimming over the `main` function, I eventually noticed this part:
```
  cVar13 = o___76(local_68);
  if (cVar13 == '\0') {
    o___60(6,o___47);
    printf(o___47);
  }
```
This looked similar to a section from the previous challenges which evaluates the user input, and returns 1 if it matches the expected value, or 0 if not. If 1 is received, then it prints the flag. 

## The Comparison Function
Let's take a look at this comparison function (labeled o___76 in Ghidra, and a slightly deobfuscated version available here in valkyrie.c).