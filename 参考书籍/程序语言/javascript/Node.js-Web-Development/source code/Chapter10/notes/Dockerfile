FROM node:9.5 
 
ENV DEBUG="notes:*,messages:*" 
ENV SEQUELIZE_CONNECT="models/sequelize-docker-mysql.yaml" 
ENV NOTES_MODEL="sequelize" 
ENV USER_SERVICE_URL="http://userauth:3333" 
ENV PORT="3000" 
ENV NOTES_SESSIONS_DIR="/sessions"
# Use this line when the Twitter Callback URL
# has to be other than localhost:3000
# ENV TWITTER_CALLBACK_HOST=45.55.37.74:3000 

RUN mkdir -p /notesapp /notesapp/minty /notesapp/partials /notesapp/public /notesapp/routes /notesapp/theme /notesapp/views
COPY minty/ /notesapp/minty/
COPY models/*.mjs models/sequelize-docker-mysql.yaml /notesapp/models/
COPY partials/ /notesapp/partials/
COPY public/ /notesapp/public/
COPY routes/ /notesapp/routes/
COPY theme/ /notesapp/theme/
COPY views/ /notesapp/views/
COPY app.mjs package.json /notesapp/

WORKDIR /notesapp
RUN apt-get update -y \
    && apt-get -y install curl python build-essential git ca-certificates sqlite3 \
    && npm install --unsafe-perm

# Uncomment to build the theme directory
# WORKDIR /notesapp/theme
# RUN npm run download && npm run build && npm run clean

WORKDIR /notesapp

VOLUME /sessions 
EXPOSE 3000 
CMD node --experimental-modules ./app
