## generate certificates

    openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -keyout key.pem -out cert.pem

## run the server

    npm install -g http-server
    http-server -S
