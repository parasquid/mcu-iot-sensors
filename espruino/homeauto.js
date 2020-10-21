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

const changeAdvertising = (btn, device) => {
  console.log(`button ${btn.pin} pressed!`);
  pressCount++;

  const data = {};
  data[device] = [pressCount];
  NRF.setAdvertising(data, { interval: 200 });

  digitalPulse(LED, true, 50);
};

setWatch((btn) => changeAdvertising(btn, LIGHT), BTN1, { repeat: true, edge: 'rising' });
setWatch((btn) => changeAdvertising(btn, FAN), BTN2, { repeat: true, edge: 'rising' });
setWatch((btn) => changeAdvertising(btn, LIGHT_FAN_ON), BTN3, { repeat: true, edge: 'rising' });
setWatch((btn) => changeAdvertising(btn, LIGHT_FAN_OFF), BTN4, { repeat: true, edge: 'rising' });
