import util from 'util';
import DBG from 'debug';
const debug = DBG('notes:notes-events'); 
const error = DBG('notes:error-events'); 

import EventEmitter from 'events'; 
class NotesEmitter extends EventEmitter {

    noteCreated(note) { 
        debug(`noteCreated ${util.inspect(note)}`);
        this.emit('notecreated', note); 
    }

    noteUpdate (note) { 
        debug(`noteUpdate ${util.inspect(note)}`);
        this.emit('noteupdate', note); 
    }

    noteDestroy (data) { 
        debug(`noteDestroy ${util.inspect(data)}`);
        this.emit('notedestroy', data); 
    }
} 

export default new NotesEmitter(); 
