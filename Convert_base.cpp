// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <string>

using std::cout;
using std::default_random_engine;
using std::endl;
using std::random_device;
using std::string;
using std::uniform_int_distribution;

// @include

/* Problem:
 The given number is in base1. Convert it to base 2
Idea: Convert from base1 to decimal
	  Convert from decimal to base2
 */
//okay, here's the real logic
//given numberAsString, base1,base2
string convert_base(const string &s, int b1, int b2) {
  bool neg = s.front() == '-';  //first find if the number is negative or positive
  
  int x = 0; //init with 0
  
  //if negative, ignore the first digit and start from 1
  //if positive, start from the first
  
  for (int i = (neg == true ? 1 : 0); i < s.size(); ++i) {
    x *= b1; //Eg "354" to base 10 number 
	//Multiply 0 by 10   (in real case, instead of 10, it is b1)
	   // Multiply 3 by 10 to give 30
	       //Multiply 35 by 10 to give 350
    x += isdigit(s[i]) ? s[i] - '0' : s[i] - 'A' + 10; //numerical value of the character
	//Now we are 3
	    //Now we are 35 
		   // Now we are 354
  }

  string ans;
  while (x) {
    int r = x % b2;
    ans.push_back(r >= 10 ? 'A' + r - 10: '0' + r);
    x /= b2;
  }

  if (ans.empty()) {
    ans.push_back('0');
  }
  if (neg) {
    ans.push_back('-');
  }
  reverse(ans.begin(), ans.end());
  return ans;
}
// @exclude

string rand_int_string(int len) {
  default_random_engine gen((random_device())());
  string ret;
  if (len == 0) {
    return {"0"};
  }
  uniform_int_distribution<int> pos_or_neg(0, 1);
  if (pos_or_neg(gen)) {
    ret.push_back('-');
  }
  uniform_int_distribution<int> num_dis('1', '9');
  ret.push_back(num_dis(gen));
  while (--len) {
    uniform_int_distribution<int> dis('0', '9');
    ret.push_back(dis(gen));
  }
  return ret;
}

int main(int argc, char *argv[]) {
  default_random_engine gen((random_device())());
  if (argc == 4) {
    string input(argv[1]);
    cout << convert_base(input, atoi(argv[2]), atoi(argv[3])) << endl;
    assert(input == convert_base(convert_base(input, atoi(argv[2]),
                                 atoi(argv[3])), atoi(argv[3]), atoi(argv[2])));
  } else {
    for (int times = 0; times < 100000; ++times) {
      uniform_int_distribution<int> len_dis(1, 9);
      string input = rand_int_string(len_dis(gen));
      uniform_int_distribution<int> base_dis(2, 16);
      int base = base_dis(gen);
      cout << "input is " << input << ", base1 = 10, base2 = "
           << base << ", ans = " << convert_base(input, 10, base) << endl;
      assert(input == convert_base(convert_base(input, 10, base), base, 10));
    }
  }
  return 0;
}
