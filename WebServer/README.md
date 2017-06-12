# requirements
* NodeJS (implements npm)
* CoAP-CLI (https://github.com/mcollina/coap-cli)

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
getusers | User Count (number) | none | int
getuser | User Data | user_id | object
getfences | Fence Count (number) | none | int
getfence | Fence Data | fence_id | object
getgates | Gate Count (number) | none | int
getgate | Gate Data | gate_id | object
togglegate | Change state of Gate | gate_id | object

# example

```
echo '' | coap post coap://localhost/getuser/1
```