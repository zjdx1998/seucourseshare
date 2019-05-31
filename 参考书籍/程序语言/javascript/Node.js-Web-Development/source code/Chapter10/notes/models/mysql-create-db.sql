CREATE DATABASE notes;
    CREATE USER 'notes'@'localhost' IDENTIFIED BY 'notes';
    GRANT ALL PRIVILEGES ON notes.* TO 'notes'@'localhost'
          WITH GRANT OPTION;