export const unpack = (string) => {
  let byteArray = [];

  for(let i = 0; i < string.length; i++) {
    byteArray.push(string.charCodeAt(i));
  }

  return byteArray;
};

export const pack = (byteArray) => {
  let string = "";

  for(let i = 0; i < byteArray.length; i++) {
    string += String.fromCharCode(byteArray[i]);
  }

  return string;
};
