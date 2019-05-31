docker run --name db-notes \
    --env MYSQL_RANDOM_ROOT_PASSWORD=true \
    --env MYSQL_USER=notes \
    --env MYSQL_PASSWORD=notes12345 \
    --env MYSQL_DATABASE=notes \
    --volume `pwd`/my.cnf:/etc/my.cnf \
    --volume `pwd`/../notes-data:/var/lib/mysql \
    --network frontnet \
    mysql/mysql-server:5.7


#    --env MYSQL_ROOT_HOST=172.0.0.0/255.0.0.0 
