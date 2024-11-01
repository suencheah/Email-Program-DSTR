#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Email.h"

using namespace std;

class Node
{
public:
  Email data;
  Node *next;
};

class EmailStack
{
public:
  Node *top = nullptr;

  bool isEmpty() { return (top == nullptr); }

  Email *const peek()
  {
    if (isEmpty())
    {
      return nullptr;
    }
    return &top->data;
  }

  // Push new email into stack
  void push(Email value)
  {
    Node *newNode = new Node();
    newNode->data = value;
    newNode->next = top;
    top = newNode;
  }

  // Return and deletes top email from stack
  Email pop()
  {
    if (isEmpty())
    {
      return {};
    }
    else
    {
      Email elem = top->data;
      Node *toDelete = top;
      top = top->next;
      delete toDelete;
      return elem;
    }
  }

  // Destructor
  ~EmailStack()
  {
    while (!isEmpty())
    {
      pop(); // Pop all elements to free memory
    }
  }
};
