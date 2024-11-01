#include <string>
#include <iostream>
#include <fstream>
#include "EmailPriorityEnum.h"
#include "EmailStack.h"

using namespace std;

class User
{
  string email;
  string dataFilePath;
  EmailStack userEmails;

public:
  // Constructors
  User() : email(""), dataFilePath("") {};

  User(string userEmail)
  {
    email = userEmail;
    dataFilePath = userEmail + ".csv";
    userEmails = getUserEmails(dataFilePath);
  }

  static void registerNewUser(string email, string password)
  {
    // Add user credentials into text file
    ofstream credentialsFile("userCredentials.txt", ios::app);
    if (!credentialsFile.is_open())
    {
      std::cerr << "Unable to open user email file for appending." << std::endl;
      return;
    }
    credentialsFile << email << "," << password << "\n";

    // Create new csv file for this user
    ofstream userFile(email + ".csv");

    if (userFile.is_open())
    {
      // Write column headers
      userFile << "Sender,Recipient,Subject,Body,Date,Priority\n";
      userFile.close();
    }
    else
    {
      cerr << "Failed to create data file for user." << endl;
    }
  };

  bool static authenticateUser(const string inputEmail, const string inputPassword)
  {
    bool authenticated = false;
    ifstream userCredentials("userCredentials.txt");
    if (!userCredentials.is_open())
    {
      cerr << "unable to load user credentials file." << endl;
    }
    else
    {
      string line;
      while (getline(userCredentials, line))
      {
        try
        {
          stringstream ss(line);
          string email;
          string password;
          getline(ss, email, ','),
              getline(ss, password);

          if (inputEmail == email && inputPassword == password)
          {
            authenticated = true;
            break;
          }
        }
        catch (const exception &e)
        {
          cerr << "Exception: " << e.what() << '\n';
        }
      }
    }
    return authenticated;
  }

  static EmailStack getUserEmails(string userDataFilePath)
  {
    EmailStack userEmails;
    ifstream userEmailFile(userDataFilePath);

    // Get all Emails from CSV file
    if (userEmailFile.is_open())
    {
      string line;

      // Skip first line for column header
      getline(userEmailFile, line);

      while (getline(userEmailFile, line))
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
          std::getline(ss, currEmail.date, ',');
          std::getline(ss, priorityString);

          currEmail.priority = stringToEmailPriority(priorityString);
          if (currEmail.priority == UNASSIGNED)
          {
            currEmail.detectEmailPriority();
          }

          userEmails.push(currEmail);
        }
        catch (const exception &e)
        {
          cerr << "Exception: " << e.what() << '\n';
        }
      }
    }
    else
    {
      cerr << "Unable to open file" << endl;
    }
    userEmailFile.close();
    return userEmails;
  }

  void receiveEmails()
  {
    string filePath = "pendingEmails.csv";
    ifstream pendingEmails(filePath);
    string remainingEmails;

    // Open user's emails file for appending
    ofstream userFile(dataFilePath, ios::app);
    if (!userFile.is_open())
    {
      std::cerr << "Unable to open user email file for appending." << std::endl;
      return;
    }

    // Get pending emails
    if (pendingEmails.is_open())
    {
      string line;

      // Skip and store column header
      getline(pendingEmails, line);
      remainingEmails += line + "\n";

      while (getline(pendingEmails, line))
      {
        try
        {
          stringstream ss(line);
          Email currEmail;
          string priorityString;

          // Parse Email Details
          getline(ss, currEmail.sender, ',');
          getline(ss, currEmail.recipient, ',');
          getline(ss, currEmail.subject, ',');
          getline(ss, currEmail.body, ',');
          getline(ss, currEmail.date, ',');
          getline(ss, priorityString);

          // If user is recipient
          if (currEmail.recipient == email)
          {
            // Detect priority
            currEmail.priority = stringToEmailPriority(priorityString);

            if (currEmail.priority == UNASSIGNED)
            {
              currEmail.detectEmailPriority();
            }

            currEmail.detectEmailPriority();
            userEmails.push(currEmail);
            // Append received email into user's file
            userFile << currEmail.sender << ","
                     << currEmail.recipient << ","
                     << currEmail.subject << ","
                     << currEmail.body << ","
                     << currEmail.date << ","
                     << EmailPriorityToString(currEmail.priority) << "\n";
          }
          else
          {
            // If user is not recipient, retain that line
            remainingEmails += line + "\n";
          }
        }
        catch (const exception &e)
        {
          cerr << "Exception: " << e.what() << '\n';
        }
      }
      userFile.close();
      pendingEmails.close();
    }
    else
    {
      cerr << "Unable to open file" << endl;
      return;
    }

    // Rewrite the pending emails file with remaining emails
    ofstream outputFile("pendingEmails.csv", ios::trunc);
    if (outputFile.is_open())
    {
      outputFile << remainingEmails;
      outputFile.close();
    }
    else
    {
      cerr << "Unable to update pending emails file." << endl;
    }
  }

  void showInbox(const EmailPriority priorityChosen)
  {
    Node *curr = userEmails.top;
    int shownCount = 0;
    bool continueShowing = true; // User wants to continue showing next page?

    while (curr != nullptr && continueShowing)
    {
      // Check if the recipient matches the specified user email
      if (curr->data.recipient == email)
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
          cout << "From: " << curr->data.sender << "\n";
          cout << "Date: " << curr->data.date << "\n";
          cout << "Subject: " << curr->data.subject << "\n";
          cout << "Body: " << curr->data.body << "\n";
          shownCount++;
        }
      }

      // Check if 10 emails have been shown for pagination control
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

      curr = curr->next; // Move to the next email
    }

    // Check if end of list is reached
    if (curr == nullptr)
    {
      cout << "End of inbox reached.\n";
    }
  }

  void showSentEmails()
  {
    Node *curr = userEmails.top;
    int shownCount = 0;
    bool continueShowing = true; // User wants to continue showing next page?

    while (curr != nullptr && continueShowing)
    {
      // Check if the sender matches the specified user email
      if (curr->data.sender == email)
      {
        // Display sent email content
        cout << "To: " << curr->data.recipient << "\n";
        cout << "Date: " << curr->data.date << "\n";
        cout << "Subject: " << curr->data.subject << "\n";
        cout << "Body: " << curr->data.body << "\n";
        shownCount++;
      }

      // Check if 10 emails have been shown for pagination control
      if (shownCount == 10)
      {
        // Prompt user to continue or stop with validation
        char choice;
        do
        {
          cout << "Show more sent emails? (y/n): ";
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

      curr = curr->next; // Move to the next email
    }

    // Check if end of list is reached
    if (curr == nullptr)
    {
      cout << "End of sent emails reached.\n";
    }
  }
};