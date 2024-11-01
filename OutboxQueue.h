#pragma once
#include <iostream>
#include "Email.h"
#include "EmailStack.h"

using namespace std;

class Node
{
public:
  Email data;
  Node *next;
};

/* OOP: Inheritance - child class of the parent class */

class OutboxQueue
{
  Node *front;
  Node *rear;

public:
  OutboxQueue()
  {
    front = nullptr;
    rear = nullptr;
  }

  bool isEmpty()
  {
    return (front == nullptr) && (rear == nullptr);
  }

  void enqueue(Email newEmail)
  {
    if (isEmpty())
    {
      rear = new Node();
      rear->data = newEmail;
      rear->next = nullptr;
      front = rear;
    }
    else
    {
      Node *newNode = new Node();
      rear->next = newNode;
      newNode->data = newEmail;
      newNode->next = nullptr;
      rear = newNode;
    }
  }

  Email dequeue()
  {
    Email elem;
    Node *temp = front;
    if (front == nullptr) {
      cout << "Queue is underflow!" << endl;
    } else {
      elem = front->data;
      if (temp->next != nullptr) {
        temp = temp->next;
        free(front);
        front = temp;
      } else {
        free(front);
        front = rear = nullptr;
      }
    }
    return elem;
  }

  bool sendEmail(Email emailToBeSent, EmailStack allEmails){
    // push new email into allEmails Stack.
    
    // write new email into csv file
        // when writing into the csv use function : output_csv(ostream &out, string s)- located in "HelperFunctions.h", 
        // to escape the commas and double quotes

    // After sent successfully, dequeue email from outbox

  }
};
