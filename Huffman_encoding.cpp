// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <iostream>
#include <queue>
#include <memory>
#include <random>
#include <string>
#include <vector>

using std::cout;
using std::default_random_engine;
using std::endl;
using std::priority_queue;
using std::random_device;
using std::shared_ptr;
using std::string;
using std::uniform_int_distribution;
using std::vector;

double EnglishFreq[] = {8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015,
                        6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749,
                        7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758,
                        0.978, 2.360, 0.150, 1.974, 0.074};

// @include
struct Symbol {
  char c;
  double prob;
  string code;
};

struct BinaryTree {
  double prob;
  Symbol *s;
  shared_ptr<BinaryTree> left, right;
};

struct Compare {
  bool operator()(const shared_ptr<BinaryTree> &lhs,
                  const shared_ptr<BinaryTree> &rhs) const {
    return lhs->prob > rhs->prob;
  }
};

// Traverse tree and assign code
void assign_huffman_code(const shared_ptr<BinaryTree> &r, const string &s) {
  if (r) {
    // This node (i.e.,leaf) contains symbol
    if (r->s) {
      r->s->code = s;
    } else {  // non-leaf node
      assign_huffman_code(r->left, s + '0');
      assign_huffman_code(r->right, s + '1');
    }
  }
}

void Huffman_encoding(vector<Symbol> *symbols) {
  // Initially assign each symbol into min->heap
  priority_queue<shared_ptr<BinaryTree>, vector<shared_ptr<BinaryTree>>, 
                 Compare> min_heap;
  for (auto &s : *symbols) {
    min_heap.emplace(new BinaryTree{s.prob, &s, nullptr, nullptr});
  }

  // Keep combining two nodes until there is one node left
  while (min_heap.size() > 1) {
    shared_ptr<BinaryTree> l = min_heap.top();
    min_heap.pop();
    shared_ptr<BinaryTree> r = min_heap.top();
    min_heap.pop();
    min_heap.emplace(new BinaryTree{l->prob + r->prob, nullptr, l, r});
  }

  // Traverse the binary tree and assign code
  assign_huffman_code(min_heap.top(), string());
}
// @exclude

int main(int argc, char *argv[]) {
  int n;
  default_random_engine gen((random_device())());
  if (argc == 2) {
    if (0 != strcmp(argv[1], "huffman"))  {
      n = atoi(argv[1]);
    } else {
      n = 26;
    }
  } else {
    uniform_int_distribution<int> dis(1, 255);
    n = dis(gen);
  }
  vector<Symbol> symbols;
  int sum = 0;
  if (argc == 1 || (0 != strcmp(argv[1], "huffman"))) {
    for (int i = 0; i < n; ++i) {
      Symbol t;
      t.c = i;
      uniform_int_distribution<int> dis(0, 100000);
      t.prob = dis(gen);
      sum += t.prob;
      symbols.emplace_back(t);
    }
    for (int i = 0; i < n; ++i) {
      symbols[i].prob /= sum;
    }
  } else {
    for (int i = 0; i < n; ++i) {
      Symbol t;
      t.c = 'a' + i;
      t.prob = EnglishFreq[i];
      symbols.emplace_back(t);
    }
  }
  Huffman_encoding(&symbols);
  double avg = 0.0;
  for (int i = 0; i < symbols.size(); ++i) {
    cout << symbols[i].c << ' ' << symbols[i].prob << ' '
         << symbols[i].code << endl;
    avg += symbols[i].prob / 100 * symbols[i].code.size();
  }
  cout << "average huffman code length = " << avg << endl;
  return 0;
}
