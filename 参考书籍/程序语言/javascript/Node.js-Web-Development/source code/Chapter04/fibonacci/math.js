
exports.fibonacci = function(n) {
    if (n === 0) return 0;
    else if (n === 1 || n === 2) return 1;
    else return exports.fibonacci(n-1) + exports.fibonacci(n-2);
};

exports.fibonacciLoop = function(n) {
    var fibos = [];
    fibos[0] = 0;
    fibos[1] = 1;
    fibos[2] = 1;
    for (var i = 3; i <= n; i++) {
        fibos[i] = fibos[i-2] + fibos[i-1];
    }
    return fibos[n];
};

exports.fibonacciAsync = function(n, done) {
    if (n === 0)
        done(undefined, 0);
    else if (n === 1 || n === 2)
        done(undefined, 1);
    else {
        setImmediate(() => {
            exports.fibonacciAsync(n-1, (err, val1) => {
                if (err) done(err);
                else setImmediate(() => {
                    exports.fibonacciAsync(n-2, (err, val2) => {
                        if (err) done(err);
                        else done(undefined, val1+val2);
                    });
                });
            });
        });
    }
};

exports.fibonacciPromise = function(n) {
    return new Promise((resolve, reject) => {
        exports.fibonacciAsync(n, (err, val) => {
            if (err) reject(err);
            else resolve(val);
        });
    });
};

const fibonacciAwait = exports.fibonacciAwait = async function(n) {
    if (n === 0) return 0;
    if (n === 1 || n === 2) return 1;
    if (n === 3) return 2;
    else {
        return await new Promise((resolve, reject) => {
            setImmediate(async () => {
                let n1 = await fibonacciAwait(n-1);
                let n2 = await fibonacciAwait(n-2);
                resolve(n1+n2);
            });
        })
    }
};