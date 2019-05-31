const express = require('express');
const router = express.Router();

const math = require('../math');

router.get('/', function(req, res, next) {
  if (req.query.fibonum) {
    // Calculate directly in this server
    res.render('fibonacci', {
      title: "Calculate Fibonacci numbers",
      fibonum: req.query.fibonum,
      fiboval: math.fibonacci(req.query.fibonum)
    }); 
    // Calculate using async-aware function, in this server
    /* math.fibonacciAsync(req.query.fibonum, (err, fiboval) => {
      res.render('fibonacci', {
        title: "Calculate Fibonacci numbers",
        fibonum: req.query.fibonum,
        fiboval: fiboval
      });
    }); */
    // Pass request off to back end server
    /* var httpreq = require('http').request({
      host: "localhost",
      port: process.env.SERVERPORT,
      path: "/fibonacci/"+Math.floor(req.query.fibonum),
      method: 'GET'
    },
    httpresp => {
      httpresp.on('data', chunk => {
        var data = JSON.parse(chunk);
        res.render('fibonacci', {
          title: "Calculate Fibonacci numbers",
          fibonum: req.query.fibonum,
          fiboval: data.result
        });
      });
      httpresp.on('error', err => { next(err); });
    });
    httpreq.on('error', err => { next(err); });
    httpreq.end(); */
  } else {
    res.render('fibonacci', {
      title: "Calculate Fibonacci numbers",
      fiboval: undefined
    });
  }
});

module.exports = router;
