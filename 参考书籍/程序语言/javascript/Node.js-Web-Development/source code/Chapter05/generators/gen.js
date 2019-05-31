
function* gen() {
    yield 1;
    yield 2;
    yield 3;
    yield 4;
    yield 5;
}

// for (let g of gen()) {
//    console.log(g);
// }

var geniter = gen();
console.log(geniter.next());
console.log(geniter.next());
console.log(geniter.next());
console.log(geniter.next());
console.log(geniter.next());
console.log(geniter.next());
console.log(geniter.next());
