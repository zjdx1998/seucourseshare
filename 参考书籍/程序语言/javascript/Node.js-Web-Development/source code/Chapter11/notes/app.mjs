#!/usr/bin/env node 

import fs from 'fs-extra';
import url from 'url';
import express from 'express';
import http from 'http';
import hbs from 'hbs';
import path from 'path';
import util from 'util';
import favicon from 'serve-favicon';
import logger from 'morgan';
import cookieParser from 'cookie-parser';
import bodyParser from 'body-parser';
import DBG from 'debug';
const debug = DBG('notes:debug-INDEX'); 
const error = DBG('notes:error-INDEX'); 

import { socketio as indexSocketio, router as index } from './routes/index';
import { router as users, initPassport } from './routes/users';
import { socketio as notesSocketio, router as notes } from './routes/notes'; 

import passportSocketIo from 'passport.socketio'; 
import session from 'express-session';
import sessionFileStore from 'session-file-store';
const FileStore = sessionFileStore(session); 

export const sessionCookieName = 'notescookie.sid';
const sessionSecret = 'keyboard mouse'; 
const sessionStore  = new FileStore({ 
  path: process.env.NOTES_SESSIONS_DIR ?             
        process.env.NOTES_SESSIONS_DIR : "sessions" 
}); 

// Workaround for lack of __dirname in ES6 modules
import dirname from './dirname.js';
const {__dirname} = dirname;

const app = express();

export default app;

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());

/**
 * Create HTTP server.
 */

const server = http.createServer(app);
import socketio from 'socket.io';
const io = socketio(server); 
 
io.use(passportSocketIo.authorize({ 
  cookieParser: cookieParser,  
  key:          sessionCookie, 
  secret:       sessionSecret, 
  store:        sessionStore,
  name:         sessionCookieName
})); 

/**
 * Get port from environment and store in Express.
 */

var port = normalizePort(process.env.PORT || '3000');
app.set('port', port);

/**
 * Listen on provided port, on all network interfaces.
 */

server.listen(port);
server.on('error', onError);
server.on('listening', onListening);

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

app.use(session({ 
  store: sessionStore, 
  secret: sessionSecret,
  resave: true, 
  saveUninitialized: true,
  name: sessionCookieName
})); 
initPassport(app);

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'hbs');
hbs.registerPartials(path.join(__dirname, 'partials'));

app.disable('etags');

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger(process.env.REQUEST_LOG_FORMAT || 'dev', { 
  stream: logStream ? logStream : process.stdout 
}));
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
app.use('/users', users); 
app.use('/notes', notes);

io.on('connection', function(socket){
  debug('a user connected');
  socket.on('disconnect', function(){
    console.log('user disconnected');
  });
});

indexSocketio(io); 
notesSocketio(io); 

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  error(`Not Found will return 404 status requrl=${req.path} query=${util.inspect(req.query)}`)
  var err = new Error(`Not Found`);
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
    error((err.status || 500) +' DEV ERROR '+ err.stack);
    // error(util.inspect(err)); 
    res.render('error', { 
      user: req.user ? req.user : undefined, 
      message: err.message, 
      error: err 
    }); 
  }); 
} 

// error handler
app.use(function(err, req, res, next) {
  error(`APP ERROR HANDLER ${err.stack}`);
  // set locals, only providing error in development
  res.locals.user = req.user ? req.user : undefined;
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

function normalizePort(val) {
  var port = parseInt(val, 10);

  if (isNaN(port)) {
    // named pipe
    return val;
  }

  if (port >= 0) {
    // port number
    return port;
  }

  return false;
}

/**
 * Event listener for HTTP server "error" event.
 */

function onError(error) {
  if (error.syscall !== 'listen') {
    throw error;
  }

  var bind = typeof port === 'string'
    ? 'Pipe ' + port
    : 'Port ' + port;

  // handle specific listen errors with friendly messages
  switch (error.code) {
    case 'EACCES':
      console.error(bind + ' requires elevated privileges');
      process.exit(1);
      break;
    case 'EADDRINUSE':
      console.error(bind + ' is already in use');
      process.exit(1);
      break;
    default:
      throw error;
  }
}

/**
 * Event listener for HTTP server "listening" event.
 */

function onListening() {
  var addr = server.address();
  var bind = typeof addr === 'string'
    ? 'pipe ' + addr
    : 'port ' + addr.port;
  debug('Listening on ' + bind);
}

export function enableSocketio() {
  var ret = true;
  const env = process.env.NOTES_DISABLE_SOCKETIO;
  if (!env || env !== 'true') {
    ret = true;
  }
  return ret;
}