import { pack, unpack } from "./utils";

test("packing and unpacking", () => {
  const string = pack([65, 66, 67, 68]); // "ABCD"
  expect(unpack(string)).toEqual([65, 66, 67, 68]);
});

test("unpacking and packing", () => {
  const byteArray = unpack("ABCD"); // [ 65, 66, 67, 68 ]
  expect(pack(byteArray)).toEqual("ABCD");
});
