#pragma once;
#include <chrono>
#include <string>
#include <iostream>
#include <optional>
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

  Email* const peek()
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
      delete (toDelete);
      return elem;
    }
  }

  static EmailStack getAllEmails()
  {
    cout << "Loading all emails...\n";
    EmailStack allEmails;
    string filePath = "dummyData.csv";
    ifstream inputFile(filePath);

    // get all Emails from csv file
    if (inputFile.is_open())
    {
      string line;

      // Skip first line for column header
      getline(inputFile, line);

      while (getline(inputFile, line))
      {
        try
        {
          stringstream ss(line);
          Email currEmail;
          string priorityString;
          std::getline(ss, currEmail.sender, ',');
          std::getline(ss, currEmail.recipient, ',');
          std::getline(ss, currEmail.subject, ',');
          std::getline(ss, currEmail.body, ',');
          std::getline(ss, currEmail.timestamp, ',');
          std::getline(ss, priorityString);

          if (priorityString == "IMPORTANT")
          {
            currEmail.priority == IMPORTANT;
          }
          else if (priorityString == "NORMAL")
          {
            currEmail.priority == NORMAL;
          }
          else if (priorityString == "SPAM")
          {
            currEmail.priority == SPAM;
          }
          else if (priorityString == "UNASSIGNED")
          {
            currEmail.priority == UNASSIGNED;
          }
          else
          {
            currEmail.priority == UNASSIGNED;
          }

          if (currEmail.priority == UNASSIGNED)
          {
            currEmail.detectEmailPriority();
          }

          allEmails.push(currEmail);
        }
        catch (const exception &e)
        {
          cerr << "exception:" << e.what() << '\n';
        }
      }
    }
    else
    {
      cerr << "Unable to open file" << endl;
    }
    inputFile.close();
    return allEmails;
  }

  void showInbox(EmailPriority priorityChosen, const string &userEmail)
  {
    Node *curr = top;
    int shownCount = 0;
    bool continueShowing = true; // user wants to continue showing next page?

    while (curr != nullptr && continueShowing)
    {
      // Check if the recipient matches the specified user email
      if (curr->data.recipient == userEmail)
      {
        // Determine if the current email should be shown based on priority
        bool shouldShow = false;
        if (priorityChosen == IMPORTANT && curr->data.priority == IMPORTANT)
        {
          shouldShow = true;
        }
        else if (priorityChosen == NORMAL && (curr->data.priority == IMPORTANT || curr->data.priority == NORMAL))
        {
          shouldShow = true;
        }
        else if (priorityChosen == SPAM && curr->data.priority == SPAM)
        {
          shouldShow = true;
        }

        // Display email if it matches the chosen priority
        if (shouldShow)
        {
          // Display email content (adjust this to your display logic)
          cout << "From: " << curr->data.sender << "\n";
          cout << "Date: " << curr->data.timestamp << "\n";
          cout << "Subject: " << curr->data.subject << "\n";
          cout << "Body: " << curr->data.body << "\n";
          shownCount++;
        }
      }

      // Check if 10 emails have been shown for pagination control
      if (shownCount == 10)
      {
        // Prompt user to continue or stop (replace this with appropriate input handling)
        cout << "Show more emails? (y/n): "; // TODO validate user's input to only accept y/Y or n/N
        char choice;
        cin >> choice;
        continueShowing = (choice == 'y' || choice == 'Y');
        shownCount = 0; // Reset count for the next set of 10 emails
      }

      curr = curr->next; // Move to the next email
    }
    // Check if end of list is reached
    if (curr == nullptr)
    {
      cout << "End of inbox reached.\n";
    }
  }

  void const showSentEmails(){
    // TODO
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
