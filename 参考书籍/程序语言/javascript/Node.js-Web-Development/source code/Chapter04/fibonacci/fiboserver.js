const math  = require('./math');
const express = require('express');
const logger = require('morgan');
const app = express();
app.use(logger('dev'));
app.get('/fibonacci/:n', (req, res, next) => {
    math.fibonacciAsync(Math.floor(req.params.n), (err, val) => {
        if (err) next('FIBO SERVER ERROR ' + err);
        else {
            res.send({
                n: req.params.n,
                result: val
            });
        }
    });
});
app.listen(process.env.SERVERPORT);