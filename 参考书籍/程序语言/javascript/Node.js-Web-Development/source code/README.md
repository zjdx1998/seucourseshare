# Node.js Web Development - Fourth Edition
This is the code repository for [Node.js Web Development - Fourth Edition](https://www.packtpub.com/web-development/nodejs-web-development-fourth-edition?utm_source=github&utm_medium=repository&utm_campaign=9781788626859), published by [Packt](https://www.packtpub.com/?utm_source=github). It contains all the supporting project files necessary to work through the book from start to finish.
## About the Book
Node.js is a server-side JavaScript platform using an event-driven, non-blocking I/O model, allowing users to build fast and scalable transaction-intensive applications running in real time. It plays a significant role in the software development world and liberates JavaScript from the web browser. With Node.js, we can reuse our JavaScript skills for general software development on a large range of systems.

It runs atop the ultra-fast JavaScript engine at the heart of Google's Chrome browser, V8, and adds a fast and robust library of asynchronous network I/O modules. 

The primary focus of Node.js is developing high performance, highly scalable web applications, and it also sees a widespread use in other areas. Electron, the Node.js-based wrapper around the Chrome engine, is the basis for popular desktop applications, such as Atom and Visual Studio Code editors, GitKraken, Postman, Etcher, and the desktop Slack client. Node.js is popular for developing Internet of Things devices and sees a tremendous adoption in microservice development and for building tools for frontend web developers and more. Node.js, as a lightweight high-performance platform, fits microservice development like a glove.
## Instructions and Navigation
All of the code is organized into folders. Each folder starts with a number followed by the application name. For example, Chapter02.



The code will look like the following:
```
var http = require('http'); 
http.createServer(function (req, res) { 
  res.writeHead(200, {'Content-Type': 'text/plain'}); 
  res.end('Hello World\n'); 
}).listen(8124, "127.0.0.1"); 
console.log('Server running at http://127.0.0.1:8124/'); 
```

We assume that you have some knowledge of JavaScript and possibly have experience with server-side code development, and that you are looking for a different way of developing server-side code.



The basic requirement is to install Node.js and have a programmer-oriented text editor. The editor need not be anything fancy, vi/vim will even do in a pinch. We will show you how to install everything that's needed. It's all open source software that can be easily downloaded from websites. 

The most important tool is the one between your ears.

Some chapters require database engines, such as MySQL and MongoDB.

Although Node.js is a cross-platform software development platform, some third-party modules are written in C/C++ and must be compiled during installation. To do so, native-code development tools such as C/C++ compilers are required, and Python is required to run the tool-chain. The details are covered in Chapter 2, Setting up Node.js. Microsoft is involved with the Node.js project and to ensure developer productivity with Node.js on Windows.





## Related Products
* [Node.js Web Development - Fourth Edition](https://amzn.to/2YP9npd)

* [RESTful Web API Design with Node.js 10 - Third Edition](https://amzn.to/30Cb7Uh)

* [Hands-on Application Development with Node.js [Video]](https://www.packtpub.com/web-development/hands-application-development-nodejs-video?utm_source=github&utm_medium=repository&utm_campaign=9781789135244)


