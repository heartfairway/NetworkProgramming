FROM opensuse/leap:15.3

RUN zypper -n install build \
                      gcc \
                      vim \
                      screen \
                      telnet \
                      git

COPY *.c /root/

WORKDIR /root/
