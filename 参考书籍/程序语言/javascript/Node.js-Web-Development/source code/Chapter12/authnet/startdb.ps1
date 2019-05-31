
docker volume create db-userauth-volume

docker run --name db-userauth --env MYSQL_RANDOM_ROOT_PASSWORD=true --env MYSQL_USER=userauth --env MYSQL_PASSWORD=userauth --env MYSQL_DATABASE=userauth --volume $PSScriptRoot\my.cnf:/etc/my.cnf --volume db-userauth-volume:/var/lib/mysql --network authnet mysql/mysql-server:5.7

