#!/bin/bash

#set -x

# Make PEM (Privacy Enhanced Mail)
openssl genrsa 2048 > Private.pem
#ssh-keygen -f Private.pem -P "password"
openssl rsa -in Private.pem -pubout -out Public.pem

mv Private.pem key/
mv Public.pem key/



