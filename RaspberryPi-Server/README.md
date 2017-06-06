# RaspberryPi-Server


## Python 3.5.1 installieren
Zunächst benötigt der Raspberry-Pi mindestens Python Version 3.5.
Diese installiert man wie folgt:

### Updates
    $ sudo apt-get install build-essential \
                       libncursesw5-dev \
                       libreadline5-dev \
                       libssl-dev \
                       libgdbm-dev \
                       libc6-dev \
                       libsqlite3-dev tk-dev \
                       libbz2-dev

### Python 3.5.1 downloaden und installieren
    $ tar -xzvf https://www.python.org/ftp/python/3.5.1/Python-3.5.1.tgz
    $ cd Python-3.5.1
    $ ./configure && make && sudo make install

Achtung, dieses benötigt ungefähr 2h Zeit !!

### pip3 updaten
    $ cd~
    $ wget https://bootstrap.pypa.io/get-pip.py
    $ sudo python3 get-pip.py
    $ sudo pip3 install requests
    
### pyserial installieren
    $ python3 -m pip install pyserial

## aiocoap
    $ sudo pip3 install --upgrade "aiocoap[all]"

Funktionen von aiocoap: http://aiocoap.readthedocs.io/en/latest/guidedtour.html
