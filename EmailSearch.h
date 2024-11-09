#pragma once
#include <string>
#include "EmailStack.h"
// #include "User.h"

using namespace std;

class EmailSearch
{
public:
  // Main search function to find emails based on keyword and type
  static EmailStack searchEmails(User &user, const string &keyword, const string &searchType = "all")
  {
    EmailStack matchedEmails;
    searchInStack(user.importantEmails, keyword, searchType, matchedEmails);
    searchInStack(user.normalEmails, keyword, searchType, matchedEmails);
    searchInStack(user.spamEmails, keyword, searchType, matchedEmails);
    searchInStack(user.sentEmails, keyword, searchType, matchedEmails);
    searchInStack(user.deletedEmails, keyword, searchType, matchedEmails);

    return matchedEmails;
    // if (matchedEmails.isEmpty())
    // {
    //   cout << "No emails found with the specified search criteria.\n";
    // }
    // else
    // {
    //   cout << "Displaying matched emails for keyword \"" << keyword << "\" in " << searchType << ":\n";
    //   matchedEmails.showEmailsBy10("matched emails");
    // }
  }

private:
  // Helper function to search in a specific stack and add results to matchedEmails
  static void searchInStack(EmailStack &stack, const string &keyword, const string &searchType, EmailStack &matchedEmails)
  {
    EmailStack tempStack;  // Temporary stack to preserve the original order after searching

    while (!stack.isEmpty())
    {
      Email email = stack.pop();
      bool matchFound = false;

      // Check fields based on the search type
      if ((searchType == "all" || searchType == "sender") && email.sender.find(keyword) != string::npos)
      {
        matchFound = true;
      }
      if ((searchType == "all" || searchType == "receiver") && email.recipient.find(keyword) != string::npos)
      {
        matchFound = true;
      }
      if ((searchType == "all" || searchType == "subject") && email.subject.find(keyword) != string::npos)
      {
        matchFound = true;
      }
      if ((searchType == "all" || searchType == "body") && email.body.find(keyword) != string::npos)
      {
        matchFound = true;
      }

      // Push matching emails to matchedEmails stack
      if (matchFound)
      {
        matchedEmails.push(email);
      }

      // Store the email in tempStack to restore original stack order later
      tempStack.push(email);
    }

    // Restore emails to the original stack
    while (!tempStack.isEmpty())
    {
      stack.push(tempStack.pop());
    }
  }
};//
