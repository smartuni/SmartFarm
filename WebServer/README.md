# requirements
* NodeJS (implements npm)
* CoAP-CLI (https://github.com/mcollina/coap-cli)

# information

* command names are case sensitive (getUserCount =/= getusercount)

# installation
```
npm install
```

# start the server

```
node coan.js
```
* Server is in localhost
* Port is displayed on the console

# commands

Name | Description | Parameter | ReturnValues
--- | --- | --- | ---
getUserCount | User Count (number) | none | int
getUser | User Data | user_id | object
getFenceCount | Fence Count (number) | none | int
getFence | Fence Data | fence_id | object
getGateCount | Gate Count (number) | none | int
getGate | Gate Data | gate_id | object
toggleGate | Change state of Gate | gate_id | object

# example

```
echo '' | coap post coap://localhost/getUserCount
```

```
echo '' | coap post coap://localhost/getUser/2
```