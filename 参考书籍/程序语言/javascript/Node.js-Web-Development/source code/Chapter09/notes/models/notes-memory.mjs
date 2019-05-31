'use strict';

import util from 'util';
import Note from './Note';

var notes = [];

async function crupdate(key, title, body) {
    notes[key] = new Note(key, title, body);
    return notes[key];
}

export function create(key, title, body) {
    return crupdate(key, title, body);
}

export function update(key, title, body) {
    return crupdate(key, title, body);
}

export async function read(key) {
    if (notes[key]) return notes[key];
    else throw new Error(`Note ${key} does not exist`);
}

export async function destroy(key) {
    if (notes[key]) {
        delete notes[key];
    } else throw new Error(`Note ${key} does not exist`);
}

export async function keylist() {
    return Object.keys(notes);
}

export async function count() {
    return notes.length;
}

export async function close() {
}
