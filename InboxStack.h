#include <fstream> // for file operations
#include <utility> // for std::pair
#include <sstream>
#include <algorithm> // for transform and sorting
#include <iomanip>   // for std::fixed and std::setprecision
#include <cmath>     // for round() function
#include <chrono>
#include <string>
#include "Email.h"

using namespace std;

class Node
{
public:
  Email data;
  Node *next;
};

class InboxStack
{
public:
  Node *top = nullptr;

public:
  bool isEmpty() { return (top == nullptr) ? true : false; }

  Email peek(){
    return top->data;
  }

  void push(Email value){
    Node *newNode = new Node();
    newNode->data = value;
    newNode->next = top;

    top = newNode;
  }

  Email pop() {
    Email elem;
    if (isEmpty()){
      cout << "Stack is empty!" << endl;
    } else {
      elem = top->data;
      top = top->next;
    }
    return elem;
  }

};
