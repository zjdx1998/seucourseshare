import restify from 'restify-clients'; 
import DBG from 'debug';
const debug = DBG('notes:users-rest-client'); 
const error = DBG('notes:error'); 
 
async function connectREST() { 
    var client = await (restify.createJsonClient({ 
        url: process.env.USER_SERVICE_URL, 
        version: '*' 
    })); 
    client.basicAuth('them', 'D4ED43C0-8BD6-4FE2-B358-7C0E230D11EF'); 
    return client; 
}

export async function create(username, password, provider, familyName, givenName, middleName, emails, photos) { 
    var client = await connectREST();
    return await new Promise((resolve, reject) => {
        client.post('/create-user', { 
            username, password, provider, 
            familyName, givenName, middleName, emails, photos 
        }, 
        (err, req, res, obj) => { 
            if (err) return reject(err); 
            resolve(obj); 
        }); 
    })
}
 
export async function update(username, password, provider, familyName, givenName, middleName, emails, photos) { 
    var client = await connectREST();
    return new Promise((resolve, reject) => { 
        client.post(`/update-user/${username}`, { 
            password, provider, 
            familyName, givenName, middleName, emails, photos 
        }, 
        (err, req, res, obj) => { 
            if (err) return reject(err); 
            resolve(obj); 
        }); 
    }); 
}

export async function find(username) { 
    var client = await connectREST();
    return new Promise((resolve, reject) => { 
        client.get(`/find/${username}`, 
        (err, req, res, obj) => { 
            if (err) return reject(err); 
            resolve(obj); 
        }); 
    }); 
}


export async function userPasswordCheck(username, password) { 
    var client = await connectREST();
    return new Promise((resolve, reject) => { 
        client.post('/passwordCheck', { 
            username, password 
        }, 
        (err, req, res, obj) => { 
            if (err) return reject(err); 
            resolve(obj); 
        }); 
    }); 
}

export async function findOrCreate(profile) {  
    var client = await connectREST();
    return new Promise((resolve, reject) => { 
        client.post('/find-or-create', { 
            username: profile.id, 
            password: profile.password, 
            provider: profile.provider, 
            familyName: profile.familyName, 
            givenName: profile.givenName, 
            middleName: profile.middleName, 
            emails: profile.emails, photos: profile.photos 
        }, 
        (err, req, res, obj) => { 
            if (err) return reject(err); 
            resolve(obj); 
        }); 
    }); 
}

export async function listUsers() {  
    var client = await connectREST();
    return new Promise((resolve, reject) => { 
        client.get('/list', (err, req, res, obj) => { 
            if (err) return reject(err); 
            resolve(obj); 
        }); 
    }); 
}