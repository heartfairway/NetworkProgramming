FROM opensuse/leap:15

RUN zypper -n install lighttpd

ADD index.html /srv/www/htdocs/

ENTRYPOINT ["lighttpd", "-D", "-f", "/etc/lighttpd/lighttpd.conf"]




