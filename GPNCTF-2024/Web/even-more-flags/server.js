const express = require('express');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');
const app = express();

const port = 1337;

const flags = fs.readFileSync('flags.txt', 'utf8')

const flag = fs.readFileSync('/flag', 'utf8')
fs.unlinkSync('/flag') // No peeking!

app.use(express.urlencoded({ extended: true }));
app.use(express.json());

// logging
app.use((req, res, next) => {
  console.log(req.method, req.url);
  next();
})

app.get('/', (req, res) => {
  res.send(`
    <html>
      <body>
        <h1>Submit a URL</h1>
        <form action="/submit" method="post">
          <input type="text" name="url" placeholder="Enter URL">
          <button type="submit">Submit</button>
        </form>
      </body>
    </html>
  `);
});

app.post('/submit', (req, res) => {
  const { url } = req.body;

  if (!url) {
    return res.status(400).send('No URL provided');
  }

  let parsed = new URL(url);
  if (parsed.protocol !== 'http:' && parsed.protocol !== 'https:') {
    return res.status(400).send('Invalid URL');
  }

  // Don't even try to remove --headless, everything will break. If you want to try stuff, use --remote-debugging-port=9222 and disable all other remote debugging flags.
  const command = `bash -c "google-chrome-stable --disable-gpu --headless=new --no-sandbox --no-first-run ${flags} ${url}"`;

  res.send('URL submitted. Launched Chrome:<br><br>' + command);

  const chromeProcess = exec(command, (error, stdout, stderr) => {
    if (error) {
      console.error(`Error executing command: ${error.message}\nStdout: ${stdout}`);
    } else {
      console.error(`Stderr: ${stderr}\nStdout: ${stdout}`);
    }
  });

  setTimeout(() => {
    chromeProcess.kill();
    // seems brutal but chrome does Weird Things™ when launched with Every Possible Flag™
    exec('killall -9 chrome google-chrome-stable chromium', (error, stdout, stderr) => {});
    console.log('Chrome process aborted');
  }, 30_000);
});

app.get('/flag', (req, res) => {
  if (req.connection.remoteAddress === '::ffff:127.0.0.1' || req.connection.remoteAddress === "::1") return res.send(flag);
  res.send(`Nope! Your IP (${req.connection.remoteAddress}) is not localhost!`);
})

app.listen(port, () => {
  console.log(`Server running on port ${port}`);
});
