FROM opensuse/leap:15.3

RUN zypper -n install build \
                      gcc \
                      make \
                      vim \
                      screen \
                      telnet \
                      git

WORKDIR /root/

RUN git clone https://github.com/heartfairway/NetworkProgramming.git
