import fs from 'fs-extra';
import path from 'path';
import util from 'util';
import jsyaml from 'js-yaml';
import Note from './Note';
import Sequelize from 'sequelize';
import DBG from 'debug';
const debug = DBG('notes:notes-sequelize'); 
const error = DBG('notes:error-sequelize'); 

var SQNote; 
var sequlz;
var dbparams;

async function connectDB() {
    
    if (typeof dbparams === 'undefined') {
        const yamltext = await fs.readFile(process.env.SEQUELIZE_CONNECT, 'utf8');
        dbparams = jsyaml.safeLoad(yamltext, 'utf8'); 
    }

    if (typeof sequlz === 'undefined') {
        sequlz = new Sequelize(dbparams.dbname, 
                        dbparams.username, dbparams.password, 
                        dbparams.params); 
    }

    if (SQNote) return SQNote.sync(); 
    SQNote = sequlz.define('Note', { 
        notekey: { type: Sequelize.STRING, 
                    primaryKey: true, unique: true }, 
        title: Sequelize.STRING, 
        body: Sequelize.TEXT 
    }); 
    return SQNote.sync();
}

export async function create(key, title, body) {
    try {
        const SQNote = await connectDB();
        const note = new Note(key, title, body); 
        await SQNote.create({ 
            notekey: key, 
            title: title, 
            body: body 
        });
        return note;
    } catch (e) {
        error(`notes CREATE ERROR ${e.stack}`);
        throw e;
    }
}
 
export async function update(key, title, body) { 
    try {
        const SQNote = await connectDB();
        const note = await SQNote.find({ where: { notekey: key } }) 
        if (!note) {
            throw new Error(`No note found for ${key}`); 
        } else {
            await note.updateAttributes({
                title: title,
                body: body
            });
            return new Note(key, title, body);
        }
    } catch (e) {
        error(`notes UPDATE ERROR ${e.stack}`);
        throw e;
    }
}

export async function read(key) {
    try {
        const SQNote = await connectDB();
        const note = await SQNote.find({ where: { notekey: key } })
        if (!note) {
            throw new Error(`No note found for ${key}`);
        } else {
            return new Note(note.notekey, note.title, note.body); 
        }
    } catch (e) {
        error(`notes READ ERROR ${e.stack}`);
        throw e;
    }
}

export async function destroy(key) {
    try {
        const SQNote = await connectDB();
        const note = await SQNote.find({ where: { notekey: key } });
        note.destroy();
    } catch (e) {
        error(`notes DESTROY ERROR ${e.stack}`);
        throw e;
    }
}

export async function keylist() {
    try {
        const SQNote = await connectDB();
        const notes = await SQNote.findAll({ attributes: [ 'notekey' ] });
        return notes.map(note => note.notekey);
    } catch (e) {
        error(`notes KEYLIST ERROR ${e.stack}`);
        throw e;
    }
}

export async function count() {
    try {
        const SQNote = await connectDB();
        const count = await SQNote.count();
        debug(`COUNT ${count}`);
        return count;
    } catch (e) {
        error(`notes COUNT ERROR ${e.stack}`);
        throw e;
    }
}

export async function close() {
    if (sequlz) sequlz.close();
    sequlz = undefined;
    SQNote = undefined;
}
