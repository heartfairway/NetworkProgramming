FROM opensuse/leap:15

WORKDIR /root

RUN zypper -n install lighttpd libfcgi0 \
              vim gcc FastCGI-devel \
              php7-cli php7-json php7-curl php7-fastcgi
              

ADD index.html /srv/www/htdocs/
ADD index.php /srv/www/htdocs/
ADD cgi_hello.sh /srv/www/htdocs/
ADD cgi.conf /etc/lighttpd/conf.d/
ADD fastcgi.conf /etc/lighttpd/conf.d/
ADD modules.conf /etc/lighttpd/

ADD fcgi_test_simple.c .
ADD fcgi_test_manual.c .

RUN gcc fcgi_test_simple.c -o fcgi_test_simple.fcgi -l fcgi
RUN gcc fcgi_test_manual.c -o fcgi_test_manual.fcgi -l fcgi
RUN chown lighttpd.lighttpd fcgi_test_simple.fcgi
RUN chown lighttpd.lighttpd fcgi_test_manual.fcgi
RUN mv fcgi_test_simple.fcgi /srv/www/cgi-bin/
RUN mv fcgi_test_manual.fcgi /srv/www/cgi-bin/

ENTRYPOINT ["lighttpd", "-D", "-f", "/etc/lighttpd/lighttpd.conf"]




