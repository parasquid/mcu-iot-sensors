let pressCount = 0;

const batteryPercentage = () => (NRF.getBattery() - 2) * 100;
const LIGHT = 0xFFFF;
const FAN = 0xFFFE;
const LIGHT_FAN_ON = 0xFFFD;
const LIGHT_FAN_OFF = 0xFFFC;

E.on("init", () => {
  NRF.setAdvertising({
    0x180F: [batteryPercentage()]
  }, { interval: 1000 });
});

setInterval(() => {
  NRF.setAdvertising({
    0x180F: [batteryPercentage()]
  }, { interval: 5000 });
}, 30000);

const changeAdvertising = (btn) => {
  console.log(`button ${btn.pin} pressed!`);
  pressCount++;
  switch(btn.pin) {
    case BTN1.valueOf():
      NRF.setAdvertising({
        0xFFFF: [pressCount]
      }, { interval: 200 });
      break;
    case BTN2.valueOf():
      NRF.setAdvertising({
        0xFFFE: [pressCount]
      }, { interval: 200 });
      break;
    case BTN3.valueOf():
      NRF.setAdvertising({
        0xFFFD: [pressCount]
      }, { interval: 200 });
      break;
    case BTN4.valueOf():
      NRF.setAdvertising({
        0xFFFC: [pressCount]
      }, { interval: 200 });
      break;
    default:
      console.log(`no handler for button ${btn.pin}`);
  }

  digitalPulse(LED, true, 50);
};

setWatch((btn) => changeAdvertising(btn), BTN1, { repeat: true, edge: 'rising' });
setWatch((btn) => changeAdvertising(btn), BTN2, { repeat: true, edge: 'rising' });
setWatch((btn) => changeAdvertising(btn), BTN3, { repeat: true, edge: 'rising' });
setWatch((btn) => changeAdvertising(btn), BTN4, { repeat: true, edge: 'rising' });
