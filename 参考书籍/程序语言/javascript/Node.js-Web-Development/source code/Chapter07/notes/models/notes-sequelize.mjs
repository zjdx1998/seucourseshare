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

async function connectDB() { 

    if (typeof sequlz === 'undefined') {
        const yamltext = await fs.readFile(process.env.SEQUELIZE_CONNECT, 'utf8');
        const params = jsyaml.safeLoad(yamltext, 'utf8'); 
        sequlz = new Sequelize(params.dbname, 
                        params.username, params.password, 
                        params.params); 
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
    const SQNote = await connectDB();
    const note = new Note(key, title, body); 
    await SQNote.create({ 
        notekey: key, 
        title: title, 
        body: body 
    });
    return note;
}
 
export async function update(key, title, body) { 
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
}

export async function read(key) { 
    const SQNote = await connectDB();
    const note = await SQNote.find({ where: { notekey: key } }) 
    if (!note) { 
        throw new Error(`No note found for ${key}`); 
    } else { 
        return new Note(note.notekey, note.title, note.body); 
    } 
}

export async function destroy(key) { 
    const SQNote = await connectDB();
    const note = await SQNote.find({ where: { notekey: key } }) 
    return note.destroy(); 
}

export async function keylist() { 
    const SQNote = await connectDB();
    const notes = await SQNote.findAll({ attributes: [ 'notekey' ] });
    return notes.map(note => note.notekey); 
}

export async function count() { 
    const SQNote = await connectDB();
    const count = await SQNote.count()
    debug(`COUNT ${count}`); 
    return count; 
}

export async function close() {
    if (sequlz) sequlz.close();
    sequlz = undefined;
    SQNote = undefined;
}
