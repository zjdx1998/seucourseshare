const puppeteer = require('puppeteer');
const assert  = require('chai').assert;
const util    = require('util');
const { URL } = require('url');
// ??? const {startServer} = require('polyserve');

function waitFor(timeToWait) {
    return new Promise(resolve => {
      setTimeout(() => {
        resolve(true);
      }, timeToWait);
    });
};

function printBrowserTargetEvent(e) {
    console.log(`${e.type()} ${e.url()}`);
}

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

    async function waitForBtnGoHome() {
        return page.waitForSelector('#btnGoHome');
    }

    before(async function() {
        browser = await puppeteer.launch({
            args: ['--no-sandbox', '--disable-setuid-sandbox', '--disable-dev-shm-usage'],
            slomo: 500
        });
        browser.on('targetchanged', printBrowserTargetEvent);
        browser.on('targetcreated', printBrowserTargetEvent);
        browser.on('targetdestroyed', printBrowserTargetEvent);
        page = await browser.newPage();
        page.on('console', msg => {
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
        page.on('framenavigated', async (frame) => frameEvent('framenavigated', await frame));
        await page.goto(process.env.NOTES_HOME_URL);
    });

    after(async function() {
        await page.close();
        await browser.close();
    });

    describe('Login', function() {
        before(async function() {
            await page.screenshot({
                type: 'png',
                path: `./screen/login-01-start.png`
            });
        });
        it('should click on login button', async function() {
            const btnLogin = await page.waitForSelector('#btnloginlocal');
            await btnLogin.click();
            console.log(`login after btnLogin`);
            await page.screenshot({
                type: 'png',
                path: `./screen/login-02-after-btnLogin.png`
            });
        });
        it('should fill in login form', async function() {
            await waitFor(1000);
            const loginForm = await page.waitForSelector('#notesLoginPage #notesLoginForm');
            await page.type('#notesLoginForm #username', "me");
            await page.type('#notesLoginForm #password', "w0rd");
            console.log(`login entered user/password`);
            await page.screenshot({
                type: 'png',
                path: `./screen/login-03-before-login-button.png`
            });
            await page.click('#formLoginBtn');
            await waitFor(3000);
            console.log(`login clicked login button`);
            await page.screenshot({
                type: 'png',
                path: `./screen/login-04-after-login-button.png`
            });
        });
        it('should return to home page', async function() {
            console.log(`should return to home page`);
            const home = await page.waitForSelector('#notesHomePage');
            console.log(`login home exists`);
            await page.screenshot({
                type: 'png',
                path: `./screen/login-05-after-login-home-page.png`
            });
            await waitFor(3000);
            console.log(`should have correct home page`);
            const btnLogout = await page.waitForSelector('#btnLogout');
            console.log(`login btnLogout`);
            const btnAddNote = await page.$('#btnAddNote');
            console.log(`login btnAddNote`);
            await page.screenshot({
                type: 'png',
                path: `./screen/login-06-have-correct-home-page.png`
            });
            console.log(`login all finished`);
        });
        after(async function() {
            await page.screenshot({
                type: 'png',
                path: `./screen/login-07-finished.png`
            });
        });
    });
    describe('Delete extant notes', function() {
        before(async function() {
            await page.screenshot({
                type: 'png',
                path: `./screen/delete-extant-01-start.png`
            });
        });
        it('should be on home page', async function() {
            console.log(`delete extant notes start`);
            page.click('#btnGoHome');
            const home = await page.waitForSelector('#notesHomePage');
            console.log(`delete extant notes home exists`);
            await page.screenshot({
                type: 'png',
                path: `./screen/delete-extant-02-home-page.png`
            });
        });
        it('should loop delete existing notes', async function() {
            let count = 0;
            while (true) {
                console.log(`delete extant start of loop`);
                let href;
                try {
                    href = await page.$eval('#notesHomePage .btn-group-vertical .btn:first-child', el => el.href);
                } catch(e) {
                    // console.log(`delete extant caught error checking for button (it's okay) ${e.stack}`);
                    href = undefined;
                }
                if (!href) {
                    console.log(`delete extant NO BUTTONS FOUND`);
                    break;
                }
                console.log(`delete extant found button url ${href}`);
                let hrefUrl = new URL(href);
                if (hrefUrl.pathname !== '/notes/view') break;
                let query = hrefUrl.searchParams;
                let key = query.get('key');
                console.log(`delete extant got href= ${href} /notes/view key=${key}`);
                await waitFor(3000);
                await page.screenshot({
                    type: 'png',
                    path: `./screen/delete-extant-03-homepage-${count}.png`
                });
                await page.click(`#${key}`);
                await waitFor(3000);
                const noteView = await page.waitForSelector('#noteView');
                await page.screenshot({
                    type: 'png',
                    path: `./screen/delete-extant-04-noteview-${count}.png`
                });
                await waitFor(3000);
                await page.screenshot({
                    type: 'png',
                    path: `./screen/delete-extant-05-waiting-btn-go-home.png`
                });
                const btnDestroyNote = await page.$('#btnDestroyNote');
                console.log(`delete extant notes btnDestroyNote`);
                await btnDestroyNote.click();
                await waitFor(3000);
                const formDestroyNote = await page.waitForSelector('#formDestroyNote');
                console.log(`delete extant notes formDestroyNote`);
                await page.screenshot({
                    type: 'png',
                    path: `./screen/delete-extant-06-confirmdestroy-${count}.png`
                });
                await waitFor(3000);
                await page.screenshot({
                    type: 'png',
                    path: `./screen/delete-extant-07-waiting-btn-go-home-${count}.png`
                });
                const btnConfirmDeleteNote = await page.$('#btnConfirmDeleteNote');
                console.log(`delete extant notes btnConfirmDeleteNote`);
                await btnConfirmDeleteNote.click();
                const home2 = await page.waitForSelector('#notesHomePage');
            }
            await page.screenshot({
                type: 'png',
                path: `./screen/delete-extant-08-end.png`
            });

        });
        after(async function() {
            await page.screenshot({
                type: 'png',
                path: `./screen/delete-extant-09-finished.png`
            });
        });
    });

    /* */
    describe('Add Note', function() {
        before(async function() {
            await page.screenshot({
                type: 'png',
                path: `./screen/addnote-01-start.png`
            });
        });
        it('should see Add Note button', async function() {
            console.log(`addnote start`);
            const btnAddNote = await page.waitForSelector('#btnAddNote');
            console.log(`addnote start btnAddNote`);
            console.log(`addnote btnAddNote`);
            await btnAddNote.click();
            await page.screenshot({
                type: 'png',
                path: `./screen/addnote-02-after-click-add-note.png`
            });
        });
        it('should fill in Add Note form', async function() {
            const formAddEditNote = await page.waitForSelector('#formAddEditNote');
            await page.type('#notekey', 'key42');
            await page.type('#title', 'Hello, world!');
            await page.type('#body', 'Lorem ipsum dolor');
            console.log(`addnote before btnSave`);
            await page.screenshot({
                type: 'png',
                path: `./screen/addnote-03-added-data.png`
            });
            await page.click('#btnSave');
        });
        it('should view note', async function() {
            console.log('addnote view note');
            let noteView;
            try {
                noteView = await page.waitForSelector('#noteView');
            } catch(err) {
                console.log(`addnote waiting for home page caught error ${err.stack}`);
                throw err;
            }
            console.log('addnote view note #2');
            await page.screenshot({
                type: 'png',
                path: `./screen/addnote-04-saved.png`
            });
            console.log('addnote view note #3');
            console.log('addnote view note #4');
            const shownKey = await page.$eval('#showKey', el => el.innerText);
            console.log(`addnote showKey ${shownKey} === key42?`);
            assert.exists(shownKey);
            assert.isString(shownKey);
            assert.include(shownKey, 'key42');
            // const notetitle = await page.waitForSelector('#notetitle');
            const shownTitle = await page.$eval('#notetitle', el => el.innerText);
            console.log(`addnote notetitle ${shownTitle}`);
            assert.exists(shownTitle);
            assert.isString(shownTitle);
            assert.include(shownTitle, 'Hello, world!');
            // const notebody = await page.waitForSelector('#notebody');
            const shownBody = await page.$eval('#notebody', el => el.innerText);
            console.log(`addnote notebody ${shownBody}`);
            assert.exists(shownBody);
            assert.isString(shownBody);
            assert.include(shownBody, 'Lorem ipsum dolor');
            await waitForBtnGoHome();
            await page.click('#btnGoHome');
            console.log(`addnote btnGoHome after click`);
            await page.screenshot({
                type: 'png',
                path: `./screen/addnote-05-after-btnGoHome.png`
            });
        });
        it('should go to home page', async function() {
            console.log(`addnote go home`);
            let home = await page.$('#notesHomePage');
            if (!home) {
                console.log(`addnote clicking on go home`);
                await waitForBtnGoHome();
                await page.click('#btnGoHome');
                home = await page.waitForSelector('#notesHomePage');
            }
            const titles = await page.$('#notetitles');
            console.log(`addnote titles`);
            assert.exists(titles);
            await waitFor(3000);
            const btnLogout = await page.$('#btnLogout');
            console.log(`addnote btnLogout`);
            assert.exists(btnLogout);
            const btnAddNote2 = await page.$('#btnAddNote');
            console.log(`addnote btnAddNote2`);
            assert.exists(btnAddNote2);
            console.log(`addnote FINISH`);
        });
        after(async function() {
            await page.screenshot({
                type: 'png',
                path: `./screen/addnote-09-finished.png`
            });
        });
    });

    describe('Delete Note', function() {
        before(async function() {
            await page.screenshot({
                type: 'png',
                path: `./screen/deletenote-01-start.png`
            });
        });
        it('should be on home page', async function() {
            console.log(`deletenote start`);
            let home = await page.$('#notesHomePage');
            if (!home) {
                await waitForBtnGoHome();
                await page.click('#btnGoHome');
                home = await page.waitForSelector('#notesHomePage');
            }
            assert.exists(home);
            console.log(`deletenote home`);
            const btnLogout = await page.$('#btnLogout');
            console.log(`addnote btnLogout`);
            assert.exists(btnLogout);
        });
        it('should go to note key42', async function() {
            const key42 = await page.$('#key42');
            assert.exists(key42);
            await key42.click();
            console.log(`deletenote key42`);
            const noteView = await page.waitForSelector('#noteView');
            console.log(`deletenote noteView #1`);
            const shownKey = await page.$eval('#showKey', el => el.innerText);
            console.log(`deletenote showKey ${shownKey} === key42`);
            assert.exists(shownKey);
            assert.include(shownKey.toString(), 'key42');
            await page.screenshot({
                type: 'png',
                path: `./screen/deletenote-02-shown-note.png`
            });
        });
        it('should delete note', async function() {
            console.log(`deletenote start`);
            const btnDestroyNote = await page.$('#btnDestroyNote');
            assert.exists(btnDestroyNote);
            await btnDestroyNote.click();
            console.log(`deletenote btnDestroyNote`);
            const formDestroyNote = await page.waitForSelector('#formDestroyNote');
            await page.screenshot({
                type: 'png',
                path: `./screen/deletenote-03-form-destroy-note.png`
            });
            console.log(`deletenote formDestroyNote`);
            await page.screenshot({
                type: 'png',
                path: `./screen/deletenote-04-confirm-delete-note.png`
            });
            await waitForBtnGoHome();
            const btnConfirmDeleteNote = await page.$('#btnConfirmDeleteNote');
            assert.exists(btnConfirmDeleteNote);
            await btnConfirmDeleteNote.click();
            console.log(`deletenote btnConfirmDeleteNote`);
            await page.screenshot({
                type: 'png',
                path: `./screen/deletenote-05-deleted-note.png`
            });
        });
        it('should return to home page', async function() {
            let home2;
            try {
                home2 = await page.waitForSelector('#notesHomePage');
            } catch (errr) {
                console.log(`deletenote after delete waiting for home page error ${errr.stack}`);
                throw errr;
            }
            await page.screenshot({
                type: 'png',
                path: `./screen/deletenote-06-home-page.png`
            });
            console.log(`deletenote home2 #1`);
            await waitFor(3000);
            const btnLogout = await page.$('#btnLogout');
            console.log(`deletenote btnLogout`);
            assert.exists(btnLogout);
            const key42_2 = await page.$('#key42');
            assert.notExists(key42_2);
            console.log(`deletenote key42_2`);
            await page.screenshot({
                type: 'png',
                path: `./screen/deletenote-07-home-page.png`
            });
        });
        after(async function() {
            await page.screenshot({
                type: 'png',
                path: `./screen/deletenote-09-finished.png`
            });
        });
    }); /* */

    describe('Log Out', function() {

        it('should logout', async function() {
            this.timeout(10000);
            await page.screenshot({
                type: 'png',
                path: `./screen/logout-01-start.png`
            });
            try {
            console.log(`logout start`);
            /* const home = await page.waitForSelector('#notesHomePage');
            assert.exists(home);
            console.log(`logout home`);
            const btnAddNote = await page.waitForSelector('#btnAddNote');
            assert.exists(btnAddNote);
            console.log(`logout btnAddNote`); */
            const btnLogout = await page.waitForSelector('#btnLogout');
            await btnLogout.click();
            console.log(`logout btnLogout`);
            await page.screenshot({
                type: 'png',
                path: `./screen/logout-02-after-logout.png`
            });
            const home2 = await page.waitForSelector('#notesHomePage');
            console.log(`logout home2`);
            await page.screenshot({
                type: 'png',
                path: `./screen/logout-03-home2.png`
            });
            await waitForBtnGoHome();
            const btnLogin = await page.$('#btnloginlocal');
            assert.exists(btnLogin);
            console.log(`logout btnLogin`);
            await page.screenshot({
                type: 'png',
                path: `./screen/logout-04-btnLogin.png`
            });
            /* const btnAddNote2 = await page.waitForSelector('#btnAddNote');
            assert.notExists(btnAddNote2);
            console.log(`logout btnAddNote2`); */
            } catch (e) {
                console.log(`logout caught error ${e.stack}`);
                await page.screenshot({
                    type: 'png',
                    path: `./screen/logout-05-caught-error.png`
                });
                throw e;
            }
            await page.screenshot({
                type: 'png',
                path: `./screen/logout-06-end.png`
            });
        });
    });
});