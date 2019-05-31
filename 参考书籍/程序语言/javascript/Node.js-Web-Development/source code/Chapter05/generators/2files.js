
const fs    = require('fs-extra');
const co    = require('co');
const util  = require('util');

co(function* () {
    var texts = [
        yield fs.readFile('hello.txt', 'utf8'),
        yield fs.readFile('goodbye.txt', 'utf8')
    ];
    console.log(util.inspect(texts));
});

