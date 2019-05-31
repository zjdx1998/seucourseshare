docker volume create notes-data-volume

docker run --name db-notes --env MYSQL_RANDOM_ROOT_PASSWORD=true --env MYSQL_USER=notes --env MYSQL_PASSWORD=notes12345 --env MYSQL_DATABASE=notes --volume $PSScriptRoot\my.cnf:/etc/my.cnf --volume notes-data-volume:/var/lib/mysql --network frontnet mysql/mysql-server:5.7

