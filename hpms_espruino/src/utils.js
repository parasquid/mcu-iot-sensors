export const unpack = (string) => {
  let bytes = [];
  for(let i = 0; i < string.length; i++) {
    let char = string.charCodeAt(i);
    bytes.push(char);
  }
  return bytes;
};

export const pack = (byteArray) => {
  let string = "";
  for(let i = 0; i < byteArray.length; i++) {
    string += String.fromCharCode(byteArray[i]);
  }
  return string;
};
