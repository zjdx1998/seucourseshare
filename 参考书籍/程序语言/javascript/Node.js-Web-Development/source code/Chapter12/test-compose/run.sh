#!/bin/sh

set -x

# docker-compose stop

# docker-compose build
# docker-compose up --force-recreate -d

docker ps
docker network ls

sleep 20

docker exec -it --workdir  /notesapp/test -e DEBUG= notes-test npm install

docker exec -it --workdir  /notesapp/test -e DEBUG= notes-test npm run test-notes-memory
docker exec -it --workdir  /notesapp/test -e DEBUG= notes-test npm run test-notes-fs
docker exec -it --workdir  /notesapp/test -e DEBUG= notes-test npm run test-notes-levelup
docker exec -it --workdir  /notesapp/test -e DEBUG= notes-test npm run test-notes-sqlite3
docker exec -it --workdir  /notesapp/test -e DEBUG= notes-test npm run test-notes-sequelize-sqlite
docker exec -it --workdir  /notesapp/test -e DEBUG= notes-test npm run test-notes-sequelize-mysql
docker exec -it --workdir  /notesapp/test -e DEBUG= notes-test npm run test-notes-mongodb

docker exec -it userauth-test PORT=3333 node ./users-add
NOTES_HOME_URL=http://localhost:3000 ./node_modules/.bin/mocha --no-timeouts uitest.js 


# docker-compose stop
