const http = require('http');
const url  = require('url');
const util = require('util');

const argUrl = process.argv[2];
const parsedUrl = url.parse(argUrl, true);

// The options object is passed to http.request
// telling it the URL to retrieve
const options = {
  host: parsedUrl.hostname,
  port: parsedUrl.port,
  path: parsedUrl.pathname,
  method: 'GET'
};

if (parsedUrl.search) options.path += "?"+parsedUrl.search;

const req = http.request(options);
// Invoked when the request is finished
req.on('response', res => {
  console.log('STATUS: ' + res.statusCode);
  console.log('HEADERS: ' + util.inspect(res.headers));
  res.setEncoding('utf8');
  res.on('data', chunk => { console.log('BODY: ' + chunk); });
  res.on('error', err => {  console.log('RESPONSE ERROR: ' + err); });
});
// Invoked on errors
req.on('error', err => { console.log('REQUEST ERROR: ' + err); });
req.end();
