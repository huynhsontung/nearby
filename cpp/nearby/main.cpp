#include "interop.h"

int main(int argc, char const *argv[])
{
  auto core = Core_Create();
  Core_Delete(core);
  return 0;
}
