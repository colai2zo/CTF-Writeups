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
Let's take a look at this comparison function (labeled `o___76` in Ghidra, and a slightly deobfuscated version available here in valkyrie.c). Yikes! Over 100 local variables and a wall of calculations even more disgusting than in `main`. Since it would have been terrible to figure out what all of this does by hand, I opened it up in GDB and started stepping through the code. 

### How the Scramble the Passphrase
Eventually I got to this section:
```
  for (local_d4 = 0; local_d4 < 0x1a;
      local_d4 = local_d4 + local_128 * local_168 + local_1e8 * local_1a8 + 1) {
    iVar4 = rand();
    local_88[local_d4] = (byte)iVar4 ^ *(byte *)(param_1 + local_d4);
    uVar5 = (uint)o___56;
    uVar3 = (uint)o___66;
    local_128 = uVar3 & (uVar5 ^ 0xfffffff6) + (uVar5 | 9) * 2 + 1;
    local_168 = uVar3 | (uVar5 ^ 0xfffffff6) + (uVar5 | 9) * 2 + 1;
    local_1a8 = uVar3 & -((uVar5 ^ 0xfffffff6) + (uVar5 | 9) * 2) - 2;
    local_1e8 = ~uVar3 & (uVar5 ^ 0xfffffff6) + (uVar5 | 9) * 2 + 1;
  }
```
It looks a little complicated but it interested me because it was this first piece of code that references `param_1`, which is the user provided passphrase string. All of that gobledygook with the local variables is a bunch of nonsense, because at the end of the day, they are just used to add to the counter in the for loop, but they always evaluate to 0. So a simplified version of this loop is:
```
for (int i = 0; i < 0x1a; i++) {
    iVar4 = rand();
    local_88[i] = (byte)iVar4 ^ *(byte *)(param_1 + i);
}
```
So basically, it takes the first 25 characters of the string we provide, xor's them with a pseudo random number provided by `rand`, and stores the result in `local_88`. Immediately below this was another interesting loop that appears to do operations on that resulting string:
```
  local_b8 = (ulong)((uint)(&o___64)[(((int)(o___56 + 1 >> 0x3f) + (int)o___56 + 1U ^
                                      (uint)(o___56 + 1 >> 0x3f)) % 10) * 3 + 1] % DAT_0010a0cc) +
             0x14;
  if (o___71 == o___77) goto LAB_00104289;
  uVar3 = (uint)o___77 & 0xe976f10a;
  *(char *)(ulong)uVar3 = *(char *)(ulong)uVar3 + (char)uVar3;
  do {
    iVar4 = rand();
    uVar9 = (ulong)(long)iVar4 % ((ulong)(o___71 != o___77) + (ulong)(o___71 == o___77) + local_b8);
    bVar1 = local_88[local_b8];
    local_88[local_b8] = local_88[uVar9];
    local_88[uVar9] = bVar1;
    local_b8 = local_b8 + ((ulong)(o___71 != o___77) - (ulong)(o___71 == o___77));
LAB_00104289:
    uVar3 = (uint)o___56;
  } while ((ulong)((uint)(&o___64)[(((uVar3 - ((uVar3 - 5) * 2 & (uint)(o___56 + -5 >> 0x3f))) - 5)
                                   % 10) * 3 + 1] % DAT_0010a0cc) - 5 < local_b8);
```
Again, a bunch of nonsense, but the core logic here is:
```
int i = 0x19;
do {
    iVar4 = rand();
    uVar9 = r % (i + 1);
    bVar1 = local_88[local_b8];
    local_88[local_b8] = local_88[uVar9];
    local_88[uVar9] = bVar1;
    i++;
} (while 0 < i);
```
So basically, it performs swaps for each character with a random index. So our passphrase is first xor'd, then scrambled. But what is it being compared with?
### How 