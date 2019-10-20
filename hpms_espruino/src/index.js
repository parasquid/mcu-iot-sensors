/* eslint-disable no-console */

const unpack = (str) => {
  var bytes = [];
  for(var i = 0; i < str.length; i++) {
    var char = str.charCodeAt(i);
    bytes.push(char);
  }
  return bytes;
};

const pack = (byteArray) => {
  var str = "";
  for(var i = 0; i < byteArray.length; i++) {
    str += String.fromCharCode(byteArray[i]);
  }
  return str;
};

// eslint-disable-next-line no-undef
var s = new Serial();

s.setup(9600, {tx:16, rx:2});

let command = pack([0x68, 0x01, 0x20, 0x77]);
s.print(command);

// let buffer = "";
s.on("data", (data) => {
  console.log(unpack(data));
});

console.log("hello");
console.log(unpack(command));
console.log(command);
