FROM opensuse/leap:15

RUN zypper -n install build \
                      gcc gcc-c++ \
                      make \
                      vim \
                      screen \
                      telnet \
                      git

WORKDIR /root/

RUN git clone https://github.com/heartfairway/NetworkProgramming.git
