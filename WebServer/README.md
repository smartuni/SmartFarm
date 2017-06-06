# Installation
```
npm install
```

# start the server
```
node coan.js
```
* Der Server startet im localhost, der Port wird auf der Konsole ausgegeben

# Server ansprechen
* Es muss der CoAP-CLI installiert werden (https://github.com/mcollina/coap-cli)
```
echo '' | coap post coap://localhost/getuser/1
```