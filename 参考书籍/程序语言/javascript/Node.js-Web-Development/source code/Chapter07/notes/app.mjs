import fs from 'fs-extra';
import url from 'url';
import express from 'express';
import hbs from 'hbs';
import path from 'path';
import util from 'util';
import favicon from 'serve-favicon';
import logger from 'morgan';
import cookieParser from 'cookie-parser';
import bodyParser from 'body-parser';
import DBG from 'debug';
const debug = DBG('notes:debug'); 
const error = DBG('notes:error'); 

import { router as index } from './routes/index';
// const users = require('./routes/users');
import { router as notes } from './routes/notes'; 

// Workaround for lack of __dirname in ES6 modules
import dirname from './dirname.js';
const {__dirname} = dirname;

const app = express();

import rfs from 'rotating-file-stream';
var logStream;
// Log to a file if requested
if (process.env.REQUEST_LOG_FILE) {
  (async () => {
    let logDirectory = path.dirname(process.env.REQUEST_LOG_FILE); 
    await fs.ensureDir(logDirectory);
    logStream = rfs(process.env.REQUEST_LOG_FILE, {
        size:     '10M', // rotate every 10 MegaBytes written
        interval: '1d',  // rotate daily
        compress: 'gzip' // compress rotated files
    });
  })().catch(err => { console.error(err); });
}

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'hbs');
hbs.registerPartials(path.join(__dirname, 'partials'));

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger(process.env.REQUEST_LOG_FORMAT || 'dev', { 
  stream: logStream ? logStream : process.stdout 
}));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
// app.use('/assets/vendor/bootstrap', express.static( 
//   path.join(__dirname, 'node_modules', 'bootstrap', 'dist'))); 
// app.use('/assets/vendor/bootstrap', express.static( 
//   path.join(__dirname, 'theme', 'bootstrap-4.0.0-beta.3', 'dist'))); 
app.use('/assets/vendor/bootstrap/js', express.static( 
  path.join(__dirname, 'node_modules', 'bootstrap', 'dist', 'js'))); 
app.use('/assets/vendor/bootstrap/css', express.static( 
  path.join(__dirname, 'minty'))); 
app.use('/assets/vendor/jquery', express.static( 
  path.join(__dirname, 'node_modules', 'jquery'))); 
app.use('/assets/vendor/popper.js', express.static( 
  path.join(__dirname, 'node_modules', 'popper.js', 'dist')));  
app.use('/assets/vendor/feather-icons', express.static( 
  path.join(__dirname, 'node_modules', 'feather-icons', 'dist'))); 

app.use('/', index);
// app.use('/users', users); 
app.use('/notes', notes);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

process.on('uncaughtException', function(err) { 
  error("I've crashed!!! - "+ (err.stack || err)); 
}); 

process.on('unhandledRejection', (reason, p) => {
  error(`Unhandled Rejection at: ${util.inspect(p)} reason: ${reason}`);
});

if (app.get('env') === 'development') { 
  app.use(function(err, req, res, next) { 
    // util.log(err.message); 
    res.status(err.status || 500); 
    error((err.status || 500) +' '+ error.message); 
    res.render('error', { 
      message: err.message, 
      error: err 
    }); 
  }); 
} 

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

export default app;
