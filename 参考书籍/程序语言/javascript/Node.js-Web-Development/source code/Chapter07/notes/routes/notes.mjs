import util from 'util';
import express from 'express';
import * as notes from '../models/notes';

export const router = express.Router();

// Add Note. (create)
router.get('/add', (req, res, next) => {
    try {
        res.render('noteedit', {
            title: "Add a Note",
            docreate: true,
            notekey: "",
            note: undefined
        });
    } catch (e) { next(e); }
});

// Save Note (update)
router.post('/save', async (req, res, next) => {
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
    } catch (e) { next(e); }
});

// Read Note (read)
router.get('/view', async (req, res, next) => {
    try {
        var note = await notes.read(req.query.key);
        res.render('noteview', {
            title: note ? note.title : "",
            notekey: req.query.key,
            note: note
        });
    } catch (e) { next(e); }
});

// Edit note (update)
router.get('/edit', async (req, res, next) => {
    try {
        var note = await notes.read(req.query.key);
        res.render('noteedit', {
            title: note ? ("Edit " + note.title) : "Add a Note",
            docreate: false,
            notekey: req.query.key,
            note: note
        });
    } catch (e) { next(e); }
});

// Ask to Delete note (destroy)
router.get('/destroy', async (req, res, next) => {
    try {
        var note = await notes.read(req.query.key);
        res.render('notedestroy', {
            title: note ? `Delete ${note.title}` : "",
            notekey: req.query.key,
            note: note
        });
    } catch (e) { next(e); }
});

// Really destroy note (destroy)
router.post('/destroy/confirm', async (req, res, next) => {
    try {
        await notes.destroy(req.body.notekey);
        res.redirect('/');
    } catch (e) { next(e); }
});
