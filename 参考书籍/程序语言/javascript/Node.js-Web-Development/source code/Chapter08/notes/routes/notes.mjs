import util from 'util';
import express from 'express';
import * as notes from '../models/notes';

export const router = express.Router();

import { ensureAuthenticated } from './users'; 

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
