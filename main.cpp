#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Email.h"
#include "EmailStack.h"
#include "User.h"
#include "EmailSearch.h"

using namespace std;
int promptIntInput(int start, int end);
bool mainMenu(User user);
void composeEmail(User user);
void userSearchEmails(User user);
void replyToEmail(User user, string targetRecipient);
void sendEmailToOutbox(User user, Email newEmail);
void showIndividualEmails(EmailStack &stack, User &user, string inboxName);

// int main()
// {
//   string test = "meisuen@apu.com,angelina@apu.com,\"test \"\"hi\"\" and , are u ok?\",05-11-2024 19:36,IMPORTANT,\"TESTTTTTT \"\"WOW\"\", \"\"COOL\"\", amazing\",Pending";
//   string sender, recipient, subject, date, body, priority, status;
//   Helper::parseCSVLine(test, sender, recipient, subject, date, priority, body, status);
//   cout << sender << endl;
//   cout << recipient << endl;
//   cout << subject << endl;
//   cout << date << endl;
//   cout << priority << endl;
//   cout << body << endl;
//   cout << status << endl;

//   return 0;
// }

int main()
{
  int loginOrRegister;
  while (loginOrRegister != 2)
  {
    string email, password;
    cout << "______Welcome to Our Email Management System______\n"
         << endl;
    cout << "Would you like to:" << endl;
    cout << "1. Register new user" << endl;
    cout << "2. Login to the system" << endl;
    loginOrRegister = promptIntInput(1, 2);

    if (loginOrRegister == 1)
    {
      cout << "\nEnter your email: ";
      getline(cin, email);
      cout << "Enter your password: ";
      getline(cin, password);
      cout << endl;
      if (User::registerNewUser(email, password))
      {
        cout << "Registration successful." << endl;
        string continueToLogin;
        cout << "Would you like to continue to login? (1 - yes, 2 - no) : ";
        getline(cin, continueToLogin);
        if (continueToLogin == "1")
        {
          loginOrRegister = 2;
          break;
        }
      }
      else
      {
        cout << "Registration failed, please try again." << endl;
      }
    }
  }

  cout << "Please login with your email and password: \n"
       << endl;

  bool loggedIn = false;
  while (!loggedIn)
  {
    // LOGIN
    string email, password;
    bool userAuthenticated = false;

    while (!userAuthenticated)
    {
      cout << "\nEnter your email: ";
      getline(cin, email);
      cout << "Enter your password: ";
      getline(cin, password);
      cout << endl;

      userAuthenticated = User::authenticateUser(email, password);
    }

    User loggedInUser = User(email);

    // MAIN MENU
    loggedIn = mainMenu(loggedInUser);
  }

  return 0;
}

bool mainMenu(User user)
{
  cout << "\nEmail System Menu:\n";
  cout << "1. Compose and Send Email\n";
  cout << "2. View Inbox (list)\n";
  cout << "3. View Inbox (one-by-one)\n";
  cout << "4. View Sent Emails\n";
  cout << "5. View Recycle Bin\n";
  cout << "6. View Outbox\n";
  cout << "7. Send First Email in Outbox\n";
  cout << "8. Search Emails\n";
  cout << "9. Log Out\n";
  cout << "10. Exit\n"
       << endl;
  ;
  // cout << "Enter your choice: ";
  int choice = promptIntInput(1, 10);

  switch (choice)
  {
  // Compose and Send Email
  case 1:
    composeEmail(user);
    cout << "done2" << endl;
    mainMenu(user);
    break;
  //  View Inbox (list)
  case 2:
    cout << "Which type of mail would you like to view?\n";
    cout << "1.Important mail\n";
    cout << "2.Normal mail\n";
    cout << "3.Spam mail\n";
    switch (promptIntInput(1, 3))
    {
    case 1:
      user.importantEmails.showEmailsBy10("inbox");
      break;
    case 2:
      user.normalEmails.showEmailsBy10("inbox");
      break;
    case 3:
      user.spamEmails.showEmailsBy10("inbox");
    }
    break;
  // View Inbox (one-by-one)
  case 3:
    cout << "Which type of mail would you like to view?\n";
    cout << "1.Important mail\n";
    cout << "2.Normal mail\n";
    cout << "3.Spam mail\n";
    switch (promptIntInput(1, 3))
    {
    case 1:
      showIndividualEmails(user.importantEmails, user, "important mails");
      break;
    case 2:
      showIndividualEmails(user.normalEmails, user, "normal mails");
      break;
    case 3:
      showIndividualEmails(user.spamEmails, user, "spam mails");
      break;
    }
  // View Sent Emails
  case 4:
    user.sentEmails.showEmailsBy10("sent emails");
    mainMenu(user);
    break;
  // View Recycle Bin
  case 5:
    user.deletedEmails.showEmailsBy10("deleted emails");
    break;
  // View Outbox
  case 6:
    user.outbox.displayOutbox();
    break;
  // Send First Email in Outbox
  case 7:
    if (user.outbox.front == nullptr)
    {
      cout << "No emails to send.\n";
    }
    else
    {
      Email toSend = user.outbox.dequeue();
      user.sendFirstEmail(toSend);
    }
    break;
  // Search Emails
  case 8:
    userSearchEmails(user);
    break;
  // Log Out
  case 9:
    cout << "Logged out.\n";
    main();
    return false;
  case 10:
    cout << "Exiting program...\n";
    exit(0);
    break;
  default:
    cout << "Invalid choice!\n";
  }
  mainMenu(user);
}

void composeEmail(User user)
{
  string to, subject, body;

  cout << "To: ";
  getline(cin, to);

  cout << "Subject: ";
  getline(cin, subject);

  cout << "Enter the content of the email: ";
  getline(cin, body);

  string date = Helper::getCurrentTimestamp();
  Email newEmail = Email(user.email, to, subject, body, date);
  sendEmailToOutbox(user, newEmail);
  // cout << "Done" << endl;
  // mainMenu(user);
}

void userSearchEmails(User user)
{
  int searchCriteria;
  cout << "Please chose criteria for searching: \n";
  cout << "1. Sender email\n";
  cout << "2. Recipient email\n";
  cout << "3. Email Subject\n";
  cout << "4. Content\n";
  cout << "5. All\n";
  cout << "6. Back to main menu\n";
  searchCriteria = promptIntInput(1, 6);

  string keyword;
  cout << "Please enter keyword for searching: ";
  getline(cin, keyword);

  EmailStack searchResults;

  switch (searchCriteria)
  {
  case 1:
    searchResults = EmailSearch::searchEmails(user, keyword, "sender");
    break;
  case 2:
    searchResults = EmailSearch::searchEmails(user, keyword, "receiver");
    break;
  case 3:
    searchResults = EmailSearch::searchEmails(user, keyword, "subject");
    break;
  case 4:
    searchResults = EmailSearch::searchEmails(user, keyword, "body");
    break;
  case 5:
    searchResults = EmailSearch::searchEmails(user, keyword, "all");
    break;
  case 6:
    mainMenu(user);
    break;
    ;
  }
  showIndividualEmails(searchResults, user, "search results");
  mainMenu(user);
}

void replyToEmail(User user, string targetRecipient)
{
  string subject, body;

  cout << "Composing email to " << targetRecipient << " ...\n";

  cout << "Subject: ";
  getline(cin, subject);

  cout << "Enter the content of the email: ";
  getline(cin, body);

  string date = Helper::getCurrentTimestamp();
  Email newEmail = Email(user.email, targetRecipient, subject, body, date);
  sendEmailToOutbox(user, newEmail);
  // return;
}

void sendEmailToOutbox(User user, const Email newEmail)
{
  user.outbox.enqueue(newEmail);
  cout << "\nEmail added to outbox queue with status: " << newEmail.status;
  user.appendOutgoingEmailToCSV(newEmail);
  cout << "\nPlease manually send from outbox.\n\n";

  // cout << "done1.5" << endl;
  mainMenu(user);
}

bool manageEmail(Email &email, User &user)
{
  while (true)
  {
    int choice;
    cout << "\n------------------" << endl;
    cout << "From:      " << email.sender << endl;
    cout << "Date:      " << email.date << endl;
    cout << "Subject:   " << email.subject << endl;
    cout << "Body: " << endl
         << email.body << endl;
    cout << "------------------" << endl;

    cout << "Would you like to : " << endl;
    cout << "1. View next email" << endl;
    cout << "2. Reply to this email" << endl;
    cout << "3. Delete this email" << endl;
    cout << "4. Mark this email as Important" << endl;
    cout << "5. Mark this email as Normal" << endl;
    cout << "6. Mark this email as Spam" << endl;
    cout << "7. Stop viewing" << endl;
    choice = promptIntInput(1, 7);

    switch (choice)
    {
    case 1:
      return true;
    case 2:
      replyToEmail(user, email.sender);
      break;
    case 3:
      if (email.priority == EmailPriority::DELETED)
      {
        cout << "Email is already deleted." << endl;
        break;
      }
      email.priority = EmailPriority::DELETED;
      user.updateEmailPriorityInCSV(email, EmailPriority::DELETED);
      // Update the inboxes
      user.getUserEmails();
      cout << "Email moved to deleted bin successfully!" << endl;
      break;
    case 4:
      if (email.priority == EmailPriority::IMPORTANT)
      {
        cout << "Email is already marked as important." << endl;
        break;
      }
      email.priority = EmailPriority::IMPORTANT;
      user.updateEmailPriorityInCSV(email, EmailPriority::IMPORTANT);
      // Update the inboxes
      user.getUserEmails();
      cout << "Email marked as important, you can view it in the important inbox!" << endl;
      break;
    case 5:
      if (email.priority == EmailPriority::NORMAL)
      {
        cout << "Email is already marked as normal." << endl;
        break;
      }
      email.priority = EmailPriority::NORMAL;
      user.updateEmailPriorityInCSV(email, EmailPriority::NORMAL);
      // Update the inboxes
      user.getUserEmails();
      cout << "Email marked as normal, you can view it in the normal inbox!" << endl;
      break;
    case 6:
      if (email.priority == EmailPriority::SPAM)
      {
        cout << "Email is already marked as spam." << endl;
        break;
      }
      email.priority = EmailPriority::SPAM;
      user.updateEmailPriorityInCSV(email, EmailPriority::SPAM);
      // Update the inboxes
      user.getUserEmails();
      cout << "Email marked as spam, you can view it in the spam inbox!" << endl;
      break;
    case 7:
      return false;
      break;
    default:
      break;
    }
  }
}

void showIndividualEmails(EmailStack &stack, User &user, string inboxName)
{
  bool viewNext = true;
  if (stack.isEmpty())
  {
    cout << "You have no " << inboxName << "." << endl;
    return;
  }
  Node *currEmail = stack.top;
  while (viewNext && currEmail != nullptr)
  {
    viewNext = manageEmail(currEmail->data, user);
    currEmail = currEmail->next;
  }
  if (viewNext && currEmail == nullptr)
  {
    cout << "You have reached the end of " << inboxName << "." << endl;
  }
}

int promptIntInput(int start, int end)
{
  bool validInput = false;
  string input;

  while (!validInput)
  {
    std::cout << "--Enter a choice between " << start << " and " << end << ": ";
    string input;
    getline(cin, input);

    try
    {
      int choice = stoi(input);
      if (choice < start || choice > end)
      {
        std::cout << "Invalid input. ";
      }
      else
      {
        validInput = true;
        cout << endl;
        return choice;
      }
    }
    catch (const invalid_argument &e)
    {
      std::cout << "Invalid input. ";
    }
    catch (const out_of_range &e)
    {
      std::cout << "Invalid input. ";
    }
  }
  return -1;
}