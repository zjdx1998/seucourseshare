import util from 'util';
import express from 'express';
import * as notes from '../models/notes';

export const router = express.Router();

import DBG from 'debug';
const debug = DBG('notes:debug-index'); 
const error = DBG('notes:error-index'); 

/* GET home page. */
router.get('/', async (req, res, next) => {
  try {
    let keylist = await notes.keylist();
    debug(`keylist ${util.inspect(keylist)}`);
    let keyPromises = keylist.map(key => {
      return notes.read(key)
    });
    let notelist = await Promise.all(keyPromises);
    debug(util.inspect(notelist));
    res.render('index', { 
      title: 'Notes', 
      notelist: notelist,
      user: req.user ? req.user : undefined
    });
  } catch (e) {
    error(`INDEX FAIL ${e}`);
    next(e);
  }
});

