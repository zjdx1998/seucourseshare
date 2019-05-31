const puppeteer = require('puppeteer');
const assert = require('chai').assert;
const util = require('util');
const { URL } = require('url');

function frameEvent(evtname, frame) {
    console.log(`${evtname} ${frame.url()} ${frame.title()}`);
}

function ignoreURL(url) {
    if (url.match(/\/assets\//) === null 
     && url.match(/\/socket.io\//) === null
     && url.match(/fonts.gstatic.com/) === null
     && url.match(/fonts.googleapis.com/) === null) {
        return false;
    } else {
        return true;
    }
}

describe('Notes', function() {

    this.timeout(10000);
    let browser;
    let page;

    before(async function() {
        browser = await puppeteer.launch({ slomo: 500 });
        page = await browser.newPage();
        /* page.on('console', msg => {
            console.log(`${msg.type()} ${msg.text()} ${msg.args().join(' ')}`);
        });
        page.on('error', err => {
            console.error(`page ERROR ${err.stack}`);
        });
        page.on('pageerror', err => {
            console.error(`page PAGEERROR ${err.stack}`);
        });
        page.on('request', req => {
            if (ignoreURL(req.url())) return;
            console.log(`page request ${req.method()} ${req.url()}`);
        });
        page.on('response', async (res) => {
            if (ignoreURL(res.url())) return;
            let text;
            try {
                text = ""; // await res.text();
            } catch(e) { text = ""; }
            console.log(`page response ${res.status()} ${res.url()}
            ${text}`);
        });
        page.on('frameattached', async (frame) => frameEvent('frameattached', await frame));
        page.on('framedetached', async (frame) => frameEvent('framedetached', await frame));
        page.on('framenavigated', async (frame) => frameEvent('framenavigated', await frame)); */
        await page.goto(process.env.NOTES_HOME_URL);
    });

    after(async function() {
        await page.close();
        await browser.close();
    });
    describe('Login', function() {
        // before(async function() { ... });
        it('should click on login button', async function() {
            const btnLogin = await page.waitForSelector('#btnloginlocal');
            await btnLogin.click();
        });
        it('should fill in login form', async function() {
            const loginForm = await page.waitForSelector('#notesLoginPage #notesLoginForm');
            await page.type('#notesLoginForm #username', "me");
            await page.type('#notesLoginForm #password', "w0rd");
            await page.click('#formLoginBtn');
        });
        it('should return to home page', async function() {
            const home = await page.waitForSelector('#notesHomePage');
            const btnLogout = await page.waitForSelector('#btnLogout');
            const btnAddNote = await page.$('#btnAddNote');
            assert.exists(btnAddNote);
        });
        // after(async function() { ... });
    });

    describe('Add Note', function() {
        // before(async function() { ... });
        it('should see Add Note button', async function() {
            const btnAddNote = await page.waitForSelector('#btnAddNote');
            await btnAddNote.click();
        });
        it('should fill in Add Note form', async function() {
            const formAddEditNote = await page.waitForSelector('#formAddEditNote');
            await page.type('#notekey', 'key42');
            await page.type('#title', 'Hello, world!');
            await page.type('#body', 'Lorem ipsum dolor');
            await page.click('#btnSave');
        });
        it('should view note', async function() {
            await page.waitForSelector('#noteView');
            const shownKey = await page.$eval('#showKey', el => el.innerText);
            assert.exists(shownKey);
            assert.isString(shownKey);
            assert.include(shownKey, 'key42');
            const shownTitle = await page.$eval('#notetitle', el => el.innerText);
            assert.exists(shownTitle);
            assert.isString(shownTitle);
            assert.include(shownTitle, 'Hello, world!');
            const shownBody = await page.$eval('#notebody', el => el.innerText);
            assert.exists(shownBody);
            assert.isString(shownBody);
            assert.include(shownBody, 'Lorem ipsum dolor');
        });
        it('should go to home page', async function() {
            await page.waitForSelector('#btnGoHome');
            await page.goto(process.env.NOTES_HOME_URL);
            // await page.click('#btnGoHome');
            await page.waitForSelector('#notesHomePage');
            const titles = await page.$('#notetitles');
            assert.exists(titles);
            const key42 = await page.$('#key42');
            assert.exists(key42);
            const btnLogout = await page.$('#btnLogout');
            assert.exists(btnLogout);
            const btnAddNote = await page.$('#btnAddNote');
            assert.exists(btnAddNote);
        });
        // after(async function() { ... });
    });

    describe('Delete Note', function() {
        // before(async function() { ... });
        it('should be on home page', async function() {
            await page.waitForSelector('#notesHomePage');
            const btnLogout = await page.$('#btnLogout');
            assert.exists(btnLogout);
        });
        it('should go to note key42', async function() {
            const key42 = await page.$('#key42');
            assert.exists(key42);
            await key42.click();
            const noteView = await page.waitForSelector('#noteView');
            const shownKey = await page.$eval('#showKey', el => el.innerText);
            assert.exists(shownKey);
            assert.include(shownKey.toString(), 'key42');
        });
        it('should delete note', async function() {
            const btnDestroyNote = await page.$('#btnDestroyNote');
            assert.exists(btnDestroyNote);
            await btnDestroyNote.click();
            const formDestroyNote = await page.waitForSelector('#formDestroyNote');
            const btnConfirmDeleteNote = await page.$('#btnConfirmDeleteNote');
            assert.exists(btnConfirmDeleteNote);
            await btnConfirmDeleteNote.click();
        });
        it('should return to home page', async function() {
            await page.waitForSelector('#notesHomePage');
            const btnLogout = await page.$('#btnLogout');
            assert.exists(btnLogout);
            const key42_2 = await page.$('#key42');
            assert.notExists(key42_2);
        });
        // after(async function() { ... });
    }); 
    describe('Log Out', function() {
        it('should logout', async function() {
            const btnLogout = await page.waitForSelector('#btnLogout');
            await btnLogout.click();
            await page.waitForSelector('#notesHomePage');
            const btnLogin = await page.$('#btnloginlocal');
            assert.exists(btnLogin);
        });
    });
});