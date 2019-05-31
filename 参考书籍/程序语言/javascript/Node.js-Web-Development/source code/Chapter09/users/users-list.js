'use strict';

const util = require('util');
const restify = require('restify-clients');

var client = restify.createJsonClient({
  url: 'http://localhost:'+process.env.PORT,
  version: '*'
});

client.basicAuth('them', 'D4ED43C0-8BD6-4FE2-B358-7C0E230D11EF');

client.get('/list', 
(err, req, res, obj) => {
    if (err) console.error(err.stack);
    else console.log('List '+ util.inspect(obj));
});
