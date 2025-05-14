#!/bin/bash
openssl genrsa 2048 > Private.Key
#ssh-keygen -f Private.Key -P "password"

openssl rsa -in Private.Key -pubout -out Public.Key

cp Private.Key key/
cp Public.Key key/

