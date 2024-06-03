# So Many Flags

## Challenge Description
I heard you like flags, so I launched Chrome with a lot of flags so you can get your flag!
The flag is in /flag.txt, and the bot will visit the HTML file you uploaded!

## Getting Started
Looking at the website, there is a file upload form and a submit button. When you submit a file, it uploads it to the server, and then runs it in Google Chrome. Each and every possible flag is passed to Google Chrome. 

Looking at the code, my first instinct is that it might be vulnerable to command injection in the file name, since Chrome is being launched [from bash](server.js#L51) within `exec`. However, upon uploading my first file it became clear that this is not possible, since the name of the file is changed before executing it.

## Solution
Given the name of the challenge "so many flags", my next thought was that it might have something to do with all the different flags being passed to chrome, some of which may disable security features. I started skimming through the flags, and the first one that looked interesting was `--allow-file-access-from-files`. I googled that flag, and the first [StackOverflow](https://stackoverflow.com/questions/18586921/how-to-launch-html-using-chrome-at-allow-file-access-from-files-mode) thread referencing it said

> That flag is dangerous!! Leaves your file system open for access. Documents originating from anywhere, local or web, should not, by default, have any access to local file:/// resources.

Cool, so I should be able to write a script that will have chrome fetch the file flag and then send the result to me. The html file I uploaded was [index.html](index.html).

## Flag
I got the following request on my request catcher site.
```
GET /GPNCTF%7BCL1_FL4G5_4R3_FL4G5_T00%7D
```