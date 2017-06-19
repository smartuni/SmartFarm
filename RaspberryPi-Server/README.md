# RaspberryPi-Server


## install Python 3.5.1 
First, the Raspberry-Pi requires at least Python version 3.5.
These are installed as follows:

### Updates
    $ sudo apt-get install build-essential \
                       libncursesw5-dev \
                       libreadline5-dev \
                       libssl-dev \
                       libgdbm-dev \
                       libc6-dev \
                       libsqlite3-dev tk-dev \
                       libbz2-dev

### Python 3.5.1 download and install
    $ tar -xzvf https://www.python.org/ftp/python/3.5.1/Python-3.5.1.tgz
    $ cd Python-3.5.1
    $ ./configure && make && sudo make install

Attention, this takes about 2h !!

### update pip3
    $ cd~
    $ wget https://bootstrap.pypa.io/get-pip.py
    $ sudo python3 get-pip.py
    $ sudo pip3 install requests
    
### install pyserial
    $ python3 -m pip install pyserial

## aiocoap
    $ sudo pip3 install --upgrade "aiocoap[all]"

Funktionen von aiocoap: http://aiocoap.readthedocs.io/en/latest/guidedtour.html

## LoWPAN
install LowPAN for Raspbian:
https://github.com/RIOT-Makers/wpan-raspbian/wiki

    
    

