# Air Quality Monitor

* Flash Espruino on the ESP8266 https://www.espruino.com/ESP8266_Flashing

* Install the dependencies via `yarn` or `npm install`

* Build the app: `yarn build`

* Flash the app: `./watch.sh`

* `watch.sh` will watch changes to `dist/main.js` so during development, if
  you'd like to update and reflash the app on the MCU, just do another
  `yarn build` and it will automatically upload the changes.
