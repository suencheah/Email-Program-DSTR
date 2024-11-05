#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Email.h"
#include "EmailStack.h"
#include "User.h"
#include "EmailSearch.h"

using namespace std;

// int main()
// {
//   // Testing
//   User::registerNewUser("meisuen@apu.com", "meisuen");
//   bool passedAuth = User::authenticateUser("meisuen@apu.com", "meisuen");

//   User myUser = User("meisuen@apu.com");
//   myUser.importantEmails.showEmailsBy10("important mails");
//   // myUser.normalEmails.showEmailsBy10("normal mails");
//   // myUser.spamEmails.showEmailsBy10("spam mails");
//   // myUser.sentEmails.showEmailsBy10("sent mails");

//     // Test search functionality
//     string keyword;
//     cout << "Enter keyword to search for: ";
//     cin >> keyword;
//     EmailSearch::searchEmails(myUser, keyword, "all");

//   return 0;
// }//

int main()
{
  // string testParser = "meisuen@apu.com,jamie@apu.com,Test double qquotes in body,05-11-2024 19:31,IMPORTANT,\"Hi i am testing \"\"doubleQQuotes\"\" in body and comma, is this working?\",Pending";
  // string sender, recipient, subject, date, body, priority, status;
  // Helper::parseCSVLine(testParser, sender, recipient, subject, date, priority, body, status);
  // cout << sender << endl;
  // cout << recipient << endl;
  // cout << subject << endl;
  // cout << date << endl;
  // cout << priority << endl;
  // cout << body << endl;
  // cout << status << endl;

  // return 0;
  

  User::registerNewUser("meisuen@apu.com", "meisuen");
  bool passedAuth = User::authenticateUser("meisuen@apu.com", "meisuen");
  User loggedInUser = User("meisuen@apu.com");

  string filename = loggedInUser.dataFilePath;
  char choice;

  do
  {
    cout << "\nEmail System Menu:\n";
    cout << "1. Compose Email\n";
    cout << "2. Send First Email in Outbox\n";
    cout << "3. View Outbox\n";
    cout << "4. View Sent Emails\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case '1':
    {
      string from, to, subject, body;

      // cout << "\nFrom: ";
      // getline(cin, from);

      cout << "To: ";
      getline(cin, to);

      cout << "Subject: ";
      getline(cin, subject);

      cout << "Enter the content of the email: ";
      getline(cin, body);

      string date = Helper::getCurrentTimestamp();

      // Queue new email to outbox and save to csv with "Pending" status
      Email newEmail = Email(loggedInUser.email, to, subject, body, date);
      loggedInUser.outbox.enqueue(newEmail);
      loggedInUser.appendOutgoingEmailToCSV(newEmail);
      break;
    }
    case '2':
      if (loggedInUser.outbox.front == nullptr){
        cout << "No emails to send.\n";
      }
      else
      {
        Email toSend = loggedInUser.outbox.dequeue();
        loggedInUser.sendFirstEmail(toSend);
      }
      break;
    case '3':
      loggedInUser.outbox.displayOutbox();
      break;
    case '4':
      cout << endl;
      loggedInUser.sentEmails.showEmailsBy10("sent emails");
      break;
    case '5':
      cout << "Exiting program...\n";
      break;
    default:
      cout << "Invalid choice!\n";
    }
  } while (choice != '5');

  return 0;
}