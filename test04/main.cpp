#include <libspu.hpp>
#include <iostream>

using namespace std;
using namespace SPU;

int main()
{
  Structure<string> str1(
    {
      { "one",    5 },
      { "two",    7 },
      { "three", 10 }
    }
  );

  value_t val = { 50, 123 };
  str1.insert(
    {
      {"one",     27 }, 
      { "two",    10 },
      { "three",  30 }
    },
    val
  );

  pair_t pair = str1.search(
    {
        { "one",    2342 }, 
        { "two",    10 },
        { "three",  30 }
    }
  );

  unsigned int val0 = pair.value.cont[0];
  unsigned int val1 = pair.value.cont[1];

  cout << "Found: " << val0 << " " << val1 << endl;
  cout << "Full pair is " << to_string(pair) << endl;

  return 0;
}