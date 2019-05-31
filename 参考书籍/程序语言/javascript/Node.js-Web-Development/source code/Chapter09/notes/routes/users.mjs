import path  from 'path';
import util from 'util';
import express from 'express'; 
import passport from 'passport'; 
import passportTwitter from 'passport-twitter';
const TwitterStrategy = passportTwitter.Strategy; 
import passportLocal from 'passport-local';
const LocalStrategy = passportLocal.Strategy; 
import * as usersModel from '../models/users-superagent';
import { sessionCookieName } from '../app';

export const router = express.Router();

import DBG from 'debug';
const debug = DBG('notes:router-users'); 
const error = DBG('notes:error-users'); 

export function initPassport(app) { 
  app.use(passport.initialize()); 
  app.use(passport.session()); 
}
 
export function ensureAuthenticated(req, res, next) { 
  try {
    // req.user is set by Passport in the deserialize function 
    if (req.user) next(); 
    else res.redirect('/users/login'); 
  } catch (e) {
    error(`ensureAuthenticated ERROR ${e.stack}`);
    next(e); 
  }
}

router.get('/login', function(req, res, next) { 
  try {
    res.render('login', { 
      title: "Login to Notes", 
      user: req.user, 
    }); 
  } catch (e) {
    error(`/login ERROR ${e.stack}`);
    next(e); 
  }
}); 
 
router.post('/login', 
  passport.authenticate('local', { 
    successRedirect: '/',     // SUCCESS: Go to home page 
    failureRedirect: 'login', // FAIL: Go to /user/login 
  }) 
);

router.get('/logout', function(req, res, next) { 
  try {
    debug(`/logout`);
    req.session.destroy();
    req.logout(); 
    res.clearCookie(sessionCookieName);
    res.redirect('/'); 
  } catch (e) {
    error(`/logout ERROR ${e.stack}`);
    next(e); 
  }
}); 

passport.use(new LocalStrategy( 
  async (username, password, done) => { 
    try {
      debug(`userPasswordCheck(${username}, ${password})`);
      var check = await usersModel.userPasswordCheck(username, password);
      if (check.check) { 
        debug(`userPasswordCheck shows good user ${util.inspect(check)}`);
        done(null, { id: check.username, 
                      username: check.username }); 
      } else { 
        debug(`userPasswordCheck shows BAD user ${util.inspect(check)}`);
        done(null, false, check.message); 
      } 
    } catch (e) { 
      error(`userPasswordCheck shows ERROR ${e.stack}`);
      done(e); 
    }
  } 
)); 

passport.use(new TwitterStrategy({ 
  consumerKey: "V5oBDLJOGsC7QztZlRqAk8sI4", 
  consumerSecret: "0Vc1HHUEY3a4YYYfNJDFHPOiljruKMTiecHYyAufxN2Mc0Gxbm", 
  callbackURL: "http://localhost:3000/users/auth/twitter/callback" 
}, 
async function(token, tokenSecret, profile, done) { 
  try {
    done(null, await usersModel.findOrCreate({ 
      id: profile.username, username: profile.username, password: "", provider: profile.provider, familyName: profile.displayName, givenName: "", middleName: "", photos: profile.photos, emails: profile.emails 
    }));
  } catch(err) { done(err); }
}));

router.get('/auth/twitter', passport.authenticate('twitter'));

router.get('/auth/twitter/callback', 
  passport.authenticate('twitter', { successRedirect: '/', 
                       failureRedirect: '/users/login' })); 

passport.serializeUser(function(user, done) { 
  try {
    done(null, user.username); 
  } catch (e) { 
    error(`serializeUser ERROR ${e.stack}`);
    done(e); 
  }
}); 
 
passport.deserializeUser(async (username, done) => { 
  try {
    var user = await usersModel.find(username);
    done(null, user);
  } catch(e) {
    error(`deserializeUser ERROR ${e.stack}`);
    done(e);
  }
}); 
