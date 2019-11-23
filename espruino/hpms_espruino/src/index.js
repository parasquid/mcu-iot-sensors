/* eslint-disable no-console */

E.setClock(80);

import {pack, unpack} from "./utils.js";

// eslint-disable-next-line no-undef
let s = new Serial();

s.setup(9600, {tx:0, rx:2});

let command = pack([0x68, 0x01, 0x20, 0x77]);
s.print(command);

// let buffer = "";
s.on("data", (data) => {
  console.log(unpack(data));
});

console.log("hello");
console.log(unpack(command));
console.log(command);
