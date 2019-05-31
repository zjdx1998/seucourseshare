docker run --name db-userauth \
    --env MYSQL_RANDOM_ROOT_PASSWORD=true \
    --env MYSQL_USER=userauth \
    --env MYSQL_PASSWORD=userauth \
    --env MYSQL_DATABASE=userauth \
    --volume `pwd`/my.cnf:/etc/my.cnf \
    --volume `pwd`/../userauth-data:/var/lib/mysql \
    --network authnet \
    mysql/mysql-server:5.7


#    --env MYSQL_ROOT_HOST=172.0.0.0/255.0.0.0 
