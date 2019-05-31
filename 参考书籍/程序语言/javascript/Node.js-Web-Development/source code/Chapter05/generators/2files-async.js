const fs    = require('fs-extra');
const util  = require('util');

async function twofiles() {
    var texts = [
        await fs.readFile('hello.txt', 'utf8'),
        await fs.readFile('goodbye.txt', 'utf8')
    ];
    console.log(util.inspect(texts));
}

twofiles();
