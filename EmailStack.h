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

  void showEmailsBy10(string inboxName)
  {
    EmailStack tempStack = *this;
    if (isEmpty()){
      cout << "You have no " << inboxName << "." << endl;
      return;
    }
    Node *curr = top;
    bool shown = false;
    int shownCount = 0;
    bool continueShowing = true;
    while (!tempStack.isEmpty() && continueShowing)
    {
      Email curr = tempStack.pop();
      cout << "From: " << curr.sender << endl;
      cout << "Date: " << curr.date << endl;
      cout << "Subject: " << curr.subject << endl;
      cout << "Body: " << curr.body << endl;
      cout << "------------------" << endl;

      shownCount++;

      if (shownCount == 10)
      {
        // Prompt user to continue or stop with validation
        char choice;
        do
        {
          cout << "Show more emails? (y/n): ";
          cin >> choice;

          // Validate input
          if (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N')
          {
            cout << "Invalid input. Please enter 'y' or 'n'.\n";
          }
        } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');

        continueShowing = (choice == 'y' || choice == 'Y');
        shownCount = 0; // Reset count for the next set of 10 emails
      }
    }
    if (tempStack.isEmpty()){
      cout << "End of "<< inboxName <<  " reached.\n";
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
};//
