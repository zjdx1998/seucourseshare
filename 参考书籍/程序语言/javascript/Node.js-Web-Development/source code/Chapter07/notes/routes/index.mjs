import util from 'util';
import express from 'express';
import * as notes from '../models/notes';

export const router = express.Router();

/* GET home page. */
router.get('/', async (req, res, next) => {
  let keylist = await notes.keylist();
  console.log(`keylist ${util.inspect(keylist)}`);
  let keyPromises = keylist.map(key => {
    return notes.read(key)
  });
  let notelist = await Promise.all(keyPromises);
  console.log(util.inspect(notelist));
  res.render('index', { title: 'Notes', notelist: notelist });
});

