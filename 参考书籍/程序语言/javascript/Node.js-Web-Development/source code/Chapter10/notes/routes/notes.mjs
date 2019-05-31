import util from 'util';
import express from 'express';
import * as notes from '../models/notes';
import { ensureAuthenticated } from './users'; 
import * as messages from '../models/messages-sequelize'; 

export const router = express.Router();

import DBG from 'debug';
const debug = DBG('notes:router-users'); 
const error = DBG('notes:error-notes'); 

// Add Note. (create)
router.get('/add', ensureAuthenticated, (req, res, next) => {
    try {
        res.render('noteedit', {
            title: "Add a Note",
            docreate: true,
            notekey: "",
            user: req.user ? req.user : undefined, 
            note: undefined
        });
    } catch (e) { 
        error(`/add ERROR ${e.stack}`);
        next(e); 
    }
});

// Save Note (update)
router.post('/save', ensureAuthenticated, async (req, res, next) => {
    try {
        var note;
        if (req.body.docreate === "create") {
            note = await notes.create(req.body.notekey,
                    req.body.title, req.body.body);
        } else {
            note = await notes.update(req.body.notekey,
                    req.body.title, req.body.body);
        }
        res.redirect('/notes/view?key='+ req.body.notekey);
    } catch (e) { 
        error(`/save ERROR ${e.stack}`);
        next(e); 
    }
});

// Read Note (read)
router.get('/view', async (req, res, next) => {
    try {
        var note = await notes.read(req.query.key);
        res.render('noteview', {
            title: note ? note.title : "",
            notekey: req.query.key,
            user: req.user ? req.user : undefined, 
            note: note
        });
    } catch (e) { 
        error(`/view ERROR ${e.stack}`);
        next(e); 
    }
});

// Edit note (update)
router.get('/edit', ensureAuthenticated, async (req, res, next) => {
    try {
        var note = await notes.read(req.query.key);
        res.render('noteedit', {
            title: note ? ("Edit " + note.title) : "Add a Note",
            docreate: false,
            notekey: req.query.key,
            user: req.user ? req.user : undefined, 
            note: note
        });
    } catch (e) { 
        error(`/edit ERROR ${e.stack}`);
        next(e); 
    }
});

// Ask to Delete note (destroy)
router.get('/destroy', ensureAuthenticated, async (req, res, next) => {
    try {
        var note = await notes.read(req.query.key);
        res.render('notedestroy', {
            title: note ? `Delete ${note.title}` : "",
            notekey: req.query.key,
            user: req.user ? req.user : undefined, 
            note: note
        });
    } catch (e) { 
        error(`/destroy ERROR ${e.stack}`);
        next(e); 
    }
});

// Really destroy note (destroy)
router.post('/destroy/confirm', ensureAuthenticated, async (req, res, next) => {
    try {
        await notes.destroy(req.body.notekey);
        res.redirect('/');
    } catch (e) { 
        error(`/destroy/confirm ERROR ${e.stack}`);
        next(e); 
    }
});

// Save incoming message to message pool, then broadcast it 
router.post('/make-comment', ensureAuthenticated, async (req, res, next) => { 
    try {
        await messages.postMessage(req.body.from, 
          req.body.namespace, req.body.message);
        res.status(200).json({ });
    } catch(err) {
        res.status(500).end(err.stack); 
    }
}); 
 
// Delete the indicated message 
router.post('/del-message', ensureAuthenticated, async (req, res, next) => { 
    try {
        await messages.destroyMessage(req.body.id, req.body.namespace);
        res.status(200).json({ });
    } catch(err) { 
        res.status(500).end(err.stack); 
    }
}); 

export function socketio(io) { 

    io.of('/view').on('connection', function(socket) {
        // 'cb' is a function sent from the browser, to which we
        // send the messages for the named note.
        debug(`/view connected on ${socket.id}`);
        socket.on('getnotemessages', (namespace, cb) => {
            debug('getnotemessages ' + namespace);
            messages.recentMessages(namespace)
            .then(cb)
            .catch(err => console.error(err.stack));
        });
    });

    messages.emitter.on('newmessage',  newmsg => {
        io.of('/view').emit('newmessage', newmsg);  
    });
    messages.emitter.on('destroymessage',  data => {
        io.of('/view').emit('destroymessage', data);  
    });
    
    /* var nspView = io.of('/view'); 
    var forNoteViewClients = function(cb) { 
        nspView.clients((err, clients) => { 
            clients.forEach(id => { 
                cb(nspView.connected[id]); 
            }); 
        }); 
    };  */
 
    notes.events.on('noteupdate',  newnote => { 
        /* forNoteViewClients(socket => { 
           socket.emit('noteupdate', newnote); 
        }); */
        io.of('/view').emit('noteupdate', newnote);  
    }); 
    notes.events.on('notedestroy', data => { 
        /* forNoteViewClients(socket => { 
           socket.emit('notedestroy', data);  
        }); */
        io.of('/view').emit('notedestroy', data);  
    }); 
}; 