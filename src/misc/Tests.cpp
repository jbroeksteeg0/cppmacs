#include "Tests.h"
#include <chrono>
#include <iostream>

#include "../backend/Rope.h"

#define TEST_COND(x)                                                           \
  if (!(x)) {                                                                  \
    std::cout << "TEST FAILED ON LINE " << __LINE__ << ": " << (#x)            \
              << std::endl;                                                    \
    exit(1);                                                                   \
  }

void Tester::Tests::test_rope() {
  // Should be empty
}

void Tester::Tests::test_rope_randomised() {
  srand(time(0));

  Rope<int> root;
  std::vector<int> vec;

  const int num_iters = 10000;
  for (int i = 0; i < num_iters; i++) {
    int oper = rand() % 3;
    if (vec.empty())
      oper = 0;

    if (oper == 0) {    // insert
      int ind = rand() % (vec.size() + 1);

      root.insert(i, ind);
      vec.insert(vec.begin() + ind, i);
    } else if (oper == 1) {
      int ind = rand() % vec.size();

      root.erase(ind);
      vec.erase(vec.begin() + ind);
    } else if (oper == 2) {
      int ind = rand() % vec.size();
      TEST_COND(vec.at(ind) == root.at(ind));
    }
  }

  TEST_COND(vec.size() == root.size());

  std::string vec_string = "[";
  for (int i = 0; i < (int)vec.size(); i++) {
    if (i != 0)
      vec_string += ", ";
    vec_string += std::to_string(vec[i]);
  }
  vec_string += "]";
  std::cout << vec_string << std::endl << root.to_string() << std::endl;
  TEST_COND(vec_string == root.to_string());
}

void Tester::Benchmarks::benchmark_rope(int seed, int iters) {
  srand(seed);
  std::chrono::time_point<std::chrono::system_clock> before =
    std::chrono::system_clock::now();

  Rope<int> r;
  for (int i = 0; i < iters; i++) {
    int oper = rand() % 5;

    if (r.size() == 0)
      oper = 0;

    if (oper <= 2) {    // insert
      int ind = rand() % (r.size() + 1);
      int val = rand();
      r.insert(val, ind);
    } else if (oper == 3) {    // delete
      int ind = rand() % r.size();
      r.erase(ind);
    } else {    // index
      int ind = rand() % r.size();
      int value = r.at(ind);
      value++;
    }
  }
  std::chrono::time_point<std::chrono::system_clock> after =
    std::chrono::system_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                 after - before
  )
                 .count()
            << std::endl;
}
void Tester::Benchmarks::benchmark_vec(int seed, int iters) {
  srand(seed);
  std::chrono::time_point<std::chrono::system_clock> before =
    std::chrono::system_clock::now();

  std::vector<int> r;
  for (int i = 0; i < iters; i++) {
    int oper = rand() % 5;

    if (r.size() == 0)
      oper = 0;

    if (oper <= 2) {    // insert
      int ind = rand() % (r.size() + 1);
      int val = rand();
      r.insert(r.begin() + ind, val);
    } else if (oper == 3) {    // delete
      int ind = rand() % r.size();
      r.erase(r.begin() + ind);
    } else {    // index
      int ind = rand() % r.size();
      int value = r[ind];
      value++;
    }
  }

  std::chrono::time_point<std::chrono::system_clock> after =
    std::chrono::system_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                 after - before
  )
                 .count()
            << std::endl;
}

void Tester::Tests::test_rope_simple() {
  Rope<int> root;
  for (int i = 0; i < 20; i++) {
    root.insert(i,rand()%(i+1));
  }
}

void Tester::run_tests() {
  using namespace Tester::Tests;
  std::cout << "Testing: Simple Rope Test" << std::endl;
  test_rope_simple();

  std::cout << "Testing: Randomised Rope Test" << std::endl;
  test_rope_randomised();

  std::cout << "ALL TESTS PASSED" << std::endl;
}
