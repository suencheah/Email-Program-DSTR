#include <string>
#include <iostream>
#include <fstream>
#include "HelperFunctions.h"
#include "EmailStack.h"

using namespace std;

class User
{
  string dataFilePath;

public:
  string email;
  EmailStack importantEmails;
  EmailStack normalEmails;
  EmailStack spamEmails;
  EmailStack sentEmails;

public:
  // Constructors
  User() : email(""), dataFilePath("") {};

  User(string userEmail)
  {
    email = userEmail;
    dataFilePath = "DataFiles/" + userEmail + ".csv";
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
    if (!emailMatched) {
      cerr << "Your email is not registered." << endl;
    }else{
      cout << "Wrong credentials entered." << endl;
    }
    return authenticated;
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
          stringstream ss(line);
          Email currEmail;
          string priorityString;

          getline(ss, currEmail.sender, ',');
          getline(ss, currEmail.recipient, ',');
          getline(ss, currEmail.subject, ',');
          getline(ss, currEmail.date, ',');
          getline(ss, priorityString, ',');
          getline(ss, currEmail.body);

          currEmail.priority = stringToEmailPriority(priorityString);
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
            sentEmails.push(currEmail);
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
      // char bom[3];
      // pendingEmails.read(bom, 3);
      // if (!(bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF'))
      // {
      //   // No BOM, reset the position to the beginning
      //   pendingEmails.seekg(0);
      // }
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
          getline(ss, currEmail.body);
          // cout << "HERE1---" << currEmail.body << endl;

          // If user is recipient
          if (currEmail.recipient == email)
          {
            // Detect priority
            currEmail.priority = stringToEmailPriority(priorityString);

            if (currEmail.priority == UNASSIGNED)
            {
              currEmail.detectEmailPriority();
            }

            // Append received email into user's file
            userFile << currEmail.sender;
            userFile << ",";
            userFile << currEmail.recipient;
            userFile << ",";
            userFile << currEmail.subject;
            userFile << ",";
            userFile << currEmail.date;
            userFile << ",";
            userFile << EmailPriorityToString(currEmail.priority);
            userFile << ",";
            userFile << currEmail.body;
            userFile << "\n";
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
};