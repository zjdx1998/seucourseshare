import fs from 'fs-extra';
import path from 'path';
import util from 'util';
import Note from './Note';
import DBG from 'debug';
const debug = DBG('notes:notes-fs');
const error = DBG('notes:error-fs');

async function notesDir() { 
    const dir = process.env.NOTES_FS_DIR || "notes-fs-data"; 
    await fs.ensureDir(dir);
    return dir;
} 

function filePath(notesdir, key) { 
    return path.join(notesdir, `${key}.json`); 
} 

async function readJSON(notesdir, key) { 
    const readFrom = filePath(notesdir, key); 
    var data = await fs.readFile(readFrom, 'utf8');
    debug(`readJSON ${data}`); 
    return Note.fromJSON(data);
}

async function crupdate(key, title, body) { 
    var notesdir = await notesDir();
    if (key.indexOf('/') >= 0)  
        throw new Error(`key ${key} cannot contain '/'`); 
    var note = new Note(key, title, body); 
    const writeTo = filePath(notesdir, key); 
    const writeJSON = note.JSON; 
    debug(`WRITE ${writeTo} ${writeJSON}`);
    await fs.writeFile(writeTo, writeJSON, 'utf8');
    return note;
}

export function create(key, title, body) {
    return crupdate(key, title, body);
}

export function update(key, title, body) {
    return crupdate(key, title, body);
}

export async function read(key) { 
    var notesdir = await notesDir();
    var thenote = await readJSON(notesdir, key);
    debug(`READ ${notesdir}/${key} ${util.inspect(thenote)}`); 
    return thenote; 
}

export async function destroy(key) { 
    var notesdir = await notesDir();
    await fs.unlink(filePath(notesdir, key));
}

export async function keylist() { 
    var notesdir = await notesDir();
    var filez = await fs.readdir(notesdir);
    if (!filez || typeof filez === 'undefined') filez = []; 
    debug(`keylist dir ${notesdir} files=${util.inspect(filez)}`); 
    var thenotes = filez.map(async fname => { 
        var key = path.basename(fname, '.json'); 
        debug(`About to READ ${key}`); 
        var thenote = await readJSON(notesdir, key);
        return thenote.key; 
    }); 
    return Promise.all(thenotes); 
}

export async function count() { 
    var notesdir = await notesDir();
    var filez = await fs.readdir(notesdir); 
    return filez.length;
}

export async function close() {
}

