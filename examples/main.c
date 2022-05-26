#include "../test.h"

test("Testing test") {
  int val = 1;
  before_each() { val = 0; }
  after_each() { val = 1; }
  /* before() { printf("Ciao mondo\n"); } */
  /* after() { printf("Hello world\n"); } */

  it("Runs") { expect(4 == 3); }
  it("Runs another time") { expect(val == 0); }
  describe("Hello") {
    it("Says Hello") { expect(strcmp("Hello", "Bye") != 0); }
  }
  describe("two") {
    skip("Runs fine") { expect(1 == 0); }
    it("Runs0") { expect(4 == 2); }
    it("Runs1") { expect(4 == 4); }
    it("Runs2") { expect(4 == 3); }
    it("Runs3") { expect(4 == 4); }
  }
}
