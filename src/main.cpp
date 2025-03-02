#include <cstdlib>

#include "scenes/ebo_rectangle.hpp"
#include "scenes/hello_triangle.hpp"

using namespace scenes;

int main() {
  ebo_rectangle::main();
  hello_triangle::main();

  return EXIT_SUCCESS;
}