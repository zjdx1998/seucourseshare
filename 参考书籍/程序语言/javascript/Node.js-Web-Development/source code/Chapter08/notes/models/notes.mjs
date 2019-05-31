
var TheModule;

async function model() {
    if (TheModule) return TheModule;
    TheModule = await import(`../models/notes-${process.env.NOTES_MODEL}`);
    return TheModule;
}

export async function create(key, title, body) { return (await model()).create(key, title, body); }
export async function update(key, title, body) { return (await model()).update(key, title, body); }
export async function read(key) { return (await model()).read(key); }
export async function destroy(key) { return (await model()).destroy(key); }
export async function keylist() { return (await model()).keylist(); }
export async function count() { return (await model()).count(); }
export async function close() { return (await model()).close(); }
