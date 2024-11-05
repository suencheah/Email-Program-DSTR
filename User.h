#ifndef USER_H // Include guard
#define USER_H

#include <string>
#include <iostream>
#include <fstream>
#include "HelperFunctions.h"
#include "EmailStack.h"
#include "Outbox.h"

using namespace std;

// Forward declaration of EmailQueue
class EmailQueue;

class User
{

public:
  string email;
  string dataFilePath;
  EmailStack importantEmails;
  EmailStack normalEmails;
  EmailStack spamEmails;
  EmailStack sentEmails;
  EmailQueue outbox;

public:
  // no more default Constructors
  // User() : email(""), dataFilePath("") {};

  User(string userEmail) : email(userEmail), dataFilePath("DataFiles/" + userEmail + ".csv")
  {
    receiveEmails();
    getUserEmails(dataFilePath);
  }

  static bool isRegistered(string const &email)
  {
    ifstream credentialsFile("DataFiles/userCredentials.txt");
    if (!credentialsFile.is_open())
    {
      cerr << "unable to load user credentials file." << endl;
      return false;
    }
    else
    {
      string line;
      while (getline(credentialsFile, line))
      {
        try
        {
          stringstream ss(line);
          string emailInFile;
          string password;
          getline(ss, emailInFile, ','),
              getline(ss, password);
          if (email == emailInFile)
          {
            return true;
          }
        }
        catch (const exception &e)
        {
          cerr << "Exception: " << e.what() << '\n';
        }
      }
    }
    return false;
  }

  static void registerNewUser(string email, string password)
  {
    bool userIsAlreadyRegistered = isRegistered(email);
    if (userIsAlreadyRegistered)
    {
      cerr << "Your email is already registered, please proceed to login." << endl;
      return;
    }
    // Add user credentials into text file
    ofstream credentialsFile("DataFiles/userCredentials.txt", ios::app);
    if (!credentialsFile.is_open())
    {
      std::cerr << "Unable to open user email file for appending." << std::endl;
      return;
    }
    credentialsFile << email << "," << password << "\n";

    // Create new csv file for this user
    ofstream userFile("DataFiles/" + email + ".csv");

    if (userFile.is_open())
    {
      // Write column headers
      userFile << "Sender,Recipient,Subject,Date,Priority,Body\n";
      userFile.close();
    }
    else
    {
      cerr << "Failed to create data file for user." << endl;
    }
  };

  bool static authenticateUser(const string inputEmail, const string inputPassword)
  {
    bool emailMatched = false;
    bool authenticated = false;
    ifstream userCredentials("DataFiles/userCredentials.txt");
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

          if (inputEmail == email)
          {
            emailMatched = true;
            if (inputPassword == password)
            {
              authenticated = true;
              cout << "Login successful." << endl;
              break;
            }
          }
        }
        catch (const exception &e)
        {
          cerr << "Exception: " << e.what() << '\n';
        }
      }
    }
    if (!emailMatched)
    {
      cerr << "Your email is not registered." << endl;
    }
    else
    {
      cout << "Wrong credentials entered." << endl;
    }
    return authenticated;
  }

  void appendOutgoingEmailToCSV(const Email &email)
  {
    ofstream file(dataFilePath, ios::app);
    if (file.is_open())
    {
      string tempString = email.FormatEmailToCsvLine();
      file << tempString;
      file.close();
    }
    else
    {
      cout << "Error: Unable to save to CSV file!" << endl;
    }
  }

  void updateEmailStatusInCSV(Email currEmail, const string &newStatus)
  {
    ifstream inFile(dataFilePath);
    ofstream tempFile("temp.csv");

    string line;
    getline(inFile, line);
    tempFile << "Sender,Recipient,Subject,Date,Priority,Body,Status\n";

    while (getline(inFile, line))
    {
      stringstream ss(line);
      string sender, recipient, subject, date, priorityString, body, status;

      getline(ss, sender, ',');
      getline(ss, recipient, ',');
      getline(ss, subject, ',');
      getline(ss, date, ',');
      getline(ss, priorityString, ',');
      getline(ss, body, ',');
      getline(ss, status);

      if (sender == currEmail.sender && recipient == currEmail.recipient && date == currEmail.date && subject == currEmail.subject)
      {
        currEmail.status = newStatus;
        string csvString = currEmail.FormatEmailToCsvLine();
        tempFile << csvString;
      }
      else
      {
        tempFile << line << "\n";
      }
      // }
    }

    inFile.close();
    tempFile.close();
    remove(dataFilePath.c_str());
    rename("temp.csv", dataFilePath.c_str());
  }
  void updateEmailPriorityInCSV(Email currEmail, const EmailPriority &newPriority)
  {
    ifstream inFile(dataFilePath);
    ofstream tempFile("temp.csv");

    string line;
    getline(inFile, line);
    tempFile << "Sender,Recipient,Subject,Date,Priority,Body,Status\n";

    while (getline(inFile, line))
    {
      stringstream ss(line);
      string sender, recipient, subject, date, priorityString, body, status;

      getline(ss, sender, ',');
      getline(ss, recipient, ',');
      getline(ss, subject, ',');
      getline(ss, date, ',');
      getline(ss, priorityString, ',');
      getline(ss, body, ',');
      getline(ss, status);

      if (sender == currEmail.sender && recipient == currEmail.recipient && date == currEmail.date && subject == currEmail.subject)
      {
        currEmail.priority = newPriority;
        string csvString = currEmail.FormatEmailToCsvLine();
        tempFile << csvString;
      }
      else
      {
        tempFile << line << "\n";
      }
      // }
    }

    inFile.close();
    tempFile.close();
    remove(dataFilePath.c_str());
    rename("temp.csv", dataFilePath.c_str());
  }

  void sendFirstEmail(Email toSend)
  {
    cout << "\nSending Email:\n";
    cout << "From: " << toSend.sender << "\n";
    cout << "To: " << toSend.recipient << "\n";
    cout << "Subject: " << toSend.subject << "\n";
    cout << "Body: " << toSend.body << "\n";
    // cout << "Priority: ";
    // switch (toSend.priority)
    // {
    // case SPAM:
    //     cout << "SPAM";
    //     break;
    // case NORMAL:
    //     cout << "NORMAL";
    //     break;
    // case IMPORTANT:
    //     cout << "IMPORTANT";
    //     break;
    // default:
    //     cout << "UNASSIGNED";
    //     break;
    // }
    cout << "\n\n";

    updateEmailStatusInCSV(toSend, "Sent");

    sentEmails.push(toSend);

    cout << "Email sent and status updated.\n";
  }

private:
  void getUserEmails(string userDataFilePath)
  {
    // EmailStack userEmails;
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
          // stringstream ss(line);
          // string sender, recipient, subject, date, body, priorityString, status;
          Email currEmail;
          string priorityString;
          Helper::parseCSVLine(line, currEmail.sender, currEmail.recipient, currEmail.subject, currEmail.date, priorityString, currEmail.body, currEmail.status);

          // getline(ss, currEmail.sender, ',');
          // getline(ss, currEmail.recipient, ',');
          // getline(ss, currEmail.subject, ',');
          // getline(ss, currEmail.date, ',');
          // getline(ss, priorityString, ',');
          // getline(ss, currEmail.body, ',');
          // getline(ss, currEmail.status);
          // cout<< currEmail.body << endl;

          currEmail.priority = Helper::stringToEmailPriority(priorityString);
          if (currEmail.recipient == email)
          {
            if (currEmail.priority == UNASSIGNED)
            {
              currEmail.detectEmailPriority();
            }
            switch (currEmail.priority)
            {
            case IMPORTANT:
              importantEmails.push(currEmail);
              break;
            case NORMAL:
              normalEmails.push(currEmail);
              break;
            case SPAM:
              spamEmails.push(currEmail);
              break;

            default:
              break;
            }
          }
          else if (currEmail.sender == email)
          {
            if (currEmail.status == "Sent")
            {
              cout << "found sent email" << endl;
              sentEmails.push(currEmail);
            }
            else if (currEmail.status == "Pending")
            {
              outbox.enqueue(currEmail);
            }
          }
        }
        catch (const exception &e)
        {
          cerr << "Exception: " << e.what() << '\n';
        }
      }
    }
    else
    {
      cerr << "Unable to open file 2" << endl;
    }
    userEmailFile.close();
    // return userEmails;
  }

  void receiveEmails()
  {
    string filePath = "DataFiles/pendingEmails.csv";
    fstream pendingEmails(filePath);
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
        // cout << "Line- " << line << endl;
        try
        {
          stringstream ss(line);
          Email currEmail;
          string priorityString;

          // Parse Email Details
          getline(ss, currEmail.sender, ',');
          getline(ss, currEmail.recipient, ',');
          getline(ss, currEmail.subject, ',');
          getline(ss, currEmail.date, ',');
          getline(ss, priorityString, ',');
          getline(ss, currEmail.body, ',');
          getline(ss, currEmail.status);
          // cout << "HERE1---" << currEmail.body << endl;

          // If user is recipient
          if (currEmail.recipient == email)
          {
            // Detect priority
            currEmail.priority = Helper::stringToEmailPriority(priorityString);

            if (currEmail.priority == UNASSIGNED)
            {
              currEmail.detectEmailPriority();
            }

            // Append received email into user's file
            string tempString = currEmail.FormatEmailToCsvLine();
            userFile << tempString;
            // userFile << currEmail.sender;
            // userFile << ",";
            // userFile << currEmail.recipient;
            // userFile << ",";
            // userFile << currEmail.subject;
            // userFile << ",";
            // userFile << currEmail.date;
            // userFile << ",";
            // userFile << Helper::EmailPriorityToString(currEmail.priority);
            // userFile << ",";
            // userFile << currEmail.body;
            // userFile << ",";
            // userFile << currEmail.status;
            // userFile << "\n";
          }
          else
          {
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
    ofstream outputFile("DataFiles/pendingEmails.csv", ios::trunc);
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

}; //

#endif // USER_H