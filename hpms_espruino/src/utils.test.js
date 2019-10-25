import { pack, unpack } from "./utils";

test("packing and unpacking", () => {
  const bytes = pack([65, 66, 67, 68]);
  expect(unpack(bytes)).toEqual([65, 66, 67, 68]);
});
