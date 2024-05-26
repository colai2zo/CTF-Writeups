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
Let's take a look at this comparison function (labeled `o___76` in Ghidra, and a slightly deobfuscated version available here in [valkyrie.c](valkyrie.c)). Yikes! Over 100 local variables and a wall of calculations even more disgusting than in `main`. Since it would have been terrible to figure out what all of this does by hand, I opened it up in GDB and started stepping through the code. 

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

### What The Scrambled Passphrase Is Compared With
The next 150 or so lines in the decompiled program are all setting those 100+ local variables to values based on a series of complex binary computations. After spending a little while trying to understand it, I ended up jumping down to the bottom to find the code that actually evaluates whether the passphrase is correct. The loop looks like this (simplified for brevity):
```
  comp_counter = 0;
  do {
    if ((long)0x19 < (long)comp_counter) {
      uVar8 = 1;
LAB_00107508:
      return uVar8;
    }
    if (local_88[comp_counter] != local_68[comp_counter]) {
      uVar8 = 0;
      goto LAB_00107508;
    }
    comp_counter = comp_counter + 1;
  } while( true );
```
So it evaluates the first 25 characters of local_88 (where our scrambled passphrase is stored), and compares it against local_68. So now we just need to know what is stored in local_68. Looking at where local_68 was defined, it is only an array of 4 bytes, so our scrambled passphrase will also be compared against the values next to local_68 in memory. I determined these values by setting breakpoints when each variable is assigned and noting the value in the register when it was assigned, though I realized later it probably would have been more efficient to just dump memory at the point of comparison in gdb (I am very new to reverse engineering). These values can be found in bytes.txt.

## Unscrambling the Passphrase
Now that we know the bytes our scrambled passphrase is being compared against and the method used to scramble them, all that is left to do is unscramble those bytes to get the required input. To do this we will first perform the same swaps they did, but in reverse order. Then, xor all of the bytes by the same amount they did. I broke this down into two stages

### Generating the XOR values and Swap Indices
In order to make sure our values match with the one used, I used C to generate the random numbers. I used `srand` with the [same value](valkyrie.c#L169) that is used in the program in order to ensure the same sequence would be generated. I then outputed this sequence in a format that I could easily bring over to python to do the operations. The program to do this is [generate.c](generate.c), and this is what is outputs:
```
$ ./generate
**** XOR (index, value) ****
(0, 0x67),(1, 0xc6),(2, 0x69),(3, 0x73),(4, 0x51),(5, 0xff),(6, 0x4a),(7, 0xec),(8, 0x29),(9, 0xcd),(10, 0xba),(11, 0xab),(12, 0xf2),(13, 0xfb),(14, 0xe3),(15, 0x46),(16, 0x7c),(17, 0xc2),(18, 0x54),(19, 0xf8),(20, 0x1b),(21, 0xe8),(22, 0xe7),(23, 0x8d),(24, 0x76),(25, 0x5a),

**** Swap Indices ****
(0x19, 0x2),(0x18, 0x17),(0x17, 0x13),(0x16, 0x4),(0x15, 0x15),(0x14, 0x0),(0x13, 0x2),(0x12, 0x6),(0x11, 0x5),(0x10, 0x10),(0xf, 0x1),(0xe, 0x1),(0xd, 0x9),(0xc, 0xb),(0xb, 0x9),(0xa, 0xa),(0x9, 0x1),(0x8, 0x2),(0x7, 0x0),(0x6, 0x3),(0x5, 0x2),(0x4, 0x4),(0x3, 0x3),(0x2, 0x0),(0x1, 0x1),
```

### Using Python to Generate the Passphrase
I wrote [reverse.py](reverse.py) to take the scrambled bytes, xor values, and swap indices from the C program, and actually unscramble them to get our passphrase. I ran it, and it gave me this:
```
$ python3 reverse.py
You've been thunderstÜuck
```
Uh-Oh, looks like I made a mistake reading one of the bytes as it was assigned. Oh well, it is pretty clear that the passphrase is `You've been thunderstruck`

## Flag
```
What is the passphrase of the vault?
> You've been thunderstruck

               -=*{ VALKYRIE }*=-  Co-sysop Valhalla BBS
                                   PH: +64-03-455-8584
  .       +       *          |\    .                    .
                              \\         *          .       +         .
       *       .        +      \\
                                \\      .      +                  *
  .     .      +    .          {====}
                                 (\\  ,,,,.         *      .           +
    +       *        .     *      \(),~`~`~~,
                                   \ ('_'|)))`  .             +
   .    .       .      +      .     \ \=,((((                             /
                                     \ !  )))),     +    .        *    +_/
            +      *       .        ,/_Y_(( \)                        _/
  --_.                             (  (   )\ \   *       ./\       . /~\~/v
      \_.     .     T_________T     \     / ) )        ./   \       /
         \,       '//////|\\\\\\'    )    \/ /       ./      \     /
           \,     /IT--T--T--T-I\   /     (_]      ,/         \_  /
             \,    I   U v U   I   |       |     ./             \/
               \,  I U.==.==.U I   !__,    |    /                \
                 \,I  |o=|=o|  I_.__|  \_~~/__,;                  \
                  ,I__|=o|o=|__I_   |  /|  |                       \.
                 /: ' : ' : ' : '\  |  ||  |
  --------------;   :   :   :   : '.(_-(|__(-------------------------------
  -----------------------------------\+/|\/:-------------------------------
               `'                     )\|/\;                 `'
       `'                            {,/ )\.]     `'
                            `'           {,/                          `'
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

flag{0f98ac306a8b3dab1b933121cd3f56a3}
```