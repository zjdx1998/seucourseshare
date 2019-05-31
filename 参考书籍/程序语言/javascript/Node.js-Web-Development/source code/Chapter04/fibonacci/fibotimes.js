
const math = require('./math');
const util = require('util');

// Uncomment this section for synchronous Fibonacci
/* 
for (var num = 1; num < 8000; num++) {
    let now = new Date().toISOString();
    console.log(`${now} Fibonacci for ${num} = ${math.fibonacci(num)}`);
}
*/

// Uncomment this section for an efficient synchronous Fibonacci
/* 
for (var num = 1; num < 8000; num++) {
    let now = new Date().toISOString();
    console.log(`${now} Fibonacci for ${num} = ${math.fibonacciLoop(num)}`);
}
*/


// Uncomment this section for an asynchronous Fibonacci
/*
(async () => {
    for (var num = 1; num < 8000; num++) {
        await new Promise((resolve, reject) => {
            math.fibonacciAsync(num, (err, fibo) => {
                if (err) reject(err);
                else {
                    let now = new Date().toISOString();
                    console.log(`${now} Fibonacci for ${num} = ${fibo}`);
                    resolve();
                }
            })
            
        })
    }
})().catch(err => { console.error(err); });
*/

(async () => {
    for (var num = 1; num < 8000; num++) {
        let fibo = await math.fibonacciAwait(num);
        let now = new Date().toISOString();
        console.log(`${now} Fibonacci for ${num} = ${fibo}`);
    }
})().catch(err => { console.error(err); });