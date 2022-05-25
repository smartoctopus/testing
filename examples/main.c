#include "test.h"

test("Testing test") {
  before_each() { printf("Hello\n"); }
  after_each() { printf("World\n"); }
  before() { printf("Ciao mondo\n"); }
  after() { printf("Hello world\n"); }
  describe("Hello") {
    it("Says Hello") { expect(strcmp("Hello", "Bye") == 0); }
  }
  skip("Runs fine") { expect(1 == 0); }
  it("Runs0") { expect(0 == 4); }
  it("Runs1") { expect(1 == 4); }
  it("Runs2") { expect(2 == 4); }
  it("Runs3") { expect(3 == 4); }
}
