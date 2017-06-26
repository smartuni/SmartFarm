# requirements
* NodeJS (implements npm)
* CoAP-CLI (https://github.com/mcollina/coap-cli)
* Apache and MySQL

# information

* command names are case sensitive (getUserCount =/= getusercount)

# installation

```
npm install
```

* including database connection
 * copy /functions/database.js.sample and remove .sample
 * fill in data to connect to your database
 * import database tables: /install/Database_Import.sql

# start the server

```
node coan.js
```
* Server is in localhost
* Port is displayed on the console

# commands

Name | Description | Parameter | ReturnValues
--- | --- | --- | ---
getUserCount | User Count (number) | *none* | int
getUser | User Data | user_id (int) | object
getFenceCount | Fence Count (number) | *none* | int
getFence | Fence Data | fence_id (int) | object  
setFenceState | Set State of Fence | fence_id (int), state (0/1) | object
getLivestockCount | Livestock Count (number) | *none* | int
getLivestock | Livestock Data | RFID Tag (string) | object
setLivestockFence | Set Fence ID of Livestock | RFID Tag (string), fence_id (int) | object
getGateCount | Gate Count (number) | *none* | int
getGate | Gate Data | gate_id (int)| object
setGateState | Set State of Gate | gate_id (int), state (0/1) | object
toggleGate | Change state of Gate | gate_id (int) | object

# example

```
echo '' | coap post coap://localhost/getUserCount
```

```
echo '' | coap post coap://localhost/getUser/2
```