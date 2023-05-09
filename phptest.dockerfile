FROM php:7-apache

RUN docker-php-ext-configure sockets \
  && docker-php-ext-install sockets \
  && docker-php-ext-enable sockets

COPY *.php /var/www/html/

COPY iot_server.c /var/www/html/
RUN gcc iot_server.c -o iot_server
