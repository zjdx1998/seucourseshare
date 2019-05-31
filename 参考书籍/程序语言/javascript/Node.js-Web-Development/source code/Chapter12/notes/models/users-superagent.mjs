import request from 'superagent';
import util from 'util';
import url from 'url'; 
const URL = url.URL;
import DBG from 'debug';
const debug = DBG('notes:users-superagent'); 
const error = DBG('notes:error-superagent'); 
 
function reqURL(path) {
    const requrl = new URL(process.env.USER_SERVICE_URL);
    requrl.pathname = path;
    return requrl.toString();
}

export async function create(username, password, 
            provider, familyName, givenName, middleName, 
            emails, photos) {
    try {
        debug(`create ${username} ${password}`);
        var res = await request
            .post(reqURL('/create-user'))
            .send({ 
                username, password, provider, 
                familyName, givenName, middleName, emails, photos 
            })
            .set('Content-Type', 'application/json')
            .set('Acccept', 'application/json')
            .auth('them', 'D4ED43C0-8BD6-4FE2-B358-7C0E230D11EF');
        return res.body;
    } catch (e) {
        error(`users CREATE ERROR ${e.stack}`);
        throw e;
    }
}
 
export async function update(username, password, 
            provider, familyName, givenName, middleName, 
            emails, photos) { 
    try {
        debug(`update ${username} ${password}`);
        var res = await request
            .post(reqURL(`/update-user/${username}`))
            .send({ 
                username, password, provider, 
                familyName, givenName, middleName, emails, photos 
            })
            .set('Content-Type', 'application/json')
            .set('Acccept', 'application/json')
            .auth('them', 'D4ED43C0-8BD6-4FE2-B358-7C0E230D11EF');
        return res.body;
    } catch (e) {
        error(`users UPDATE ERROR ${e.stack}`);
        throw e;
    }
}

export async function find(username) {
    try {
        debug(`find ${username}`);
        var res = await request
            .get(reqURL(`/find/${username}`))
            .set('Content-Type', 'application/json')
            .set('Acccept', 'application/json')
            .auth('them', 'D4ED43C0-8BD6-4FE2-B358-7C0E230D11EF');
        debug(`find ${username} RESULT ${util.inspect(res.body)}`);
        return res.body;
    } catch (e) {
        error(`users FIND ERROR ${e.stack}`);
        throw e;
    }
}


export async function userPasswordCheck(username, password) { 
    try {
        debug(`userPasswordCheck(${username}, ${password})`);
        var res = await request
            .post(reqURL(`/passwordCheck`))
            .send({ username, password })
            .set('Content-Type', 'application/json')
            .set('Acccept', 'application/json')
            .auth('them', 'D4ED43C0-8BD6-4FE2-B358-7C0E230D11EF');
        return res.body;
    } catch (e) {
        error(`users userPasswordCheck ERROR ${e.stack}`);
        throw e;
    }
}

export async function findOrCreate(profile) { 
    try {
        debug(`findOrCreate ${profile.id} ${profile.password}`); 
        var res = await request
            .post(reqURL('/find-or-create'))
            .send({ 
                username: profile.id, 
                password: profile.password, 
                provider: profile.provider, 
                familyName: profile.familyName, 
                givenName: profile.givenName, 
                middleName: profile.middleName, 
                emails: profile.emails, photos: profile.photos 
            })
            .set('Content-Type', 'application/json')
            .set('Acccept', 'application/json')
            .auth('them', 'D4ED43C0-8BD6-4FE2-B358-7C0E230D11EF');
        return res.body;
    } catch (e) {
        error(`users findOrCreate ERROR ${e.stack}`);
        throw e;
    }
}

export async function listUsers() { 
    try {
        debug(`listUsers`); 
        var res = await request
            .get(reqURL('/list'))
            .set('Content-Type', 'application/json')
            .set('Acccept', 'application/json')
            .auth('them', 'D4ED43C0-8BD6-4FE2-B358-7C0E230D11EF'); 
        return res.body;
    } catch (e) {
        error(`users listUsers ERROR ${e.stack}`);
        throw e;
    }
}