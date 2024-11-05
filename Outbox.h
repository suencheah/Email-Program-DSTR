#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "Email.h"

using namespace std;

class QueueEmailNode
{
public:
    Email data;
    QueueEmailNode *next;

    // Constructor for QueueEmailNode
    QueueEmailNode(Email newEmail) : data(newEmail), next(nullptr) {}

    // QueueEmail(string snd, string rcv, string sub, string bd, string dt = "", string st = "Pending", EmailPriority pri = UNASSIGNED)
    //     : Email(snd, rcv, sub, bd, dt, st, pri), next(nullptr) {}
};

class EmailQueue
{
public:
    QueueEmailNode *front;
    QueueEmailNode *rear;
    // string filename;

    void splitString(const string &str, string fields[], int &count, char delimiter) const
    {
        stringstream ss(str);
        string temp;
        count = 0;

        // Reset all fields to empty strings first
        for (int i = 0; i < 7; i++)
        {
            fields[i] = "";
        }

        // Split the string
        while (getline(ss, temp, delimiter) && count < 7)
        {
            fields[count] = temp;
            count++;
        }
    }

public:
    EmailQueue() : front(nullptr), rear(nullptr)
    {
        // ifstream fileCheck(filename);
        // bool fileIsEmpty = fileCheck.peek() == ifstream::traits_type::eof();
        // fileCheck.close();

        // if (fileIsEmpty)
        // {
        //     ofstream file(filename, ios::app);
        //     file << "Sender,Recipient,Subject,Date,Priority,Body,Status\n";
        //     file.close();
        // }

        // loadFromCSV();
    }

    // void loadFromCSV()
    // {
    //     ifstream inFile(filename);
    //     string line;
    //     getline(inFile, line); // Skip header

    //     while (getline(inFile, line))
    //     {
    //         string fields[7]; // Array to store fields
    //         int fieldCount = 0;
    //         splitString(line, fields, fieldCount, ',');

    //         if (fieldCount >= 6)
    //         {
    //             string from = fields[0];
    //             string to = fields[1];
    //             string subject = fields[2];
    //             string date = fields[3];
    //             string priority_str = fields[4];
    //             string body = fields[5];
    //             string status = (fieldCount >= 7) ? fields[6] : "Pending";

    //             // Convert priority string to enum
    //             EmailPriority priority = Helper::stringToEmailPriority(priority_str);
    //             // Only add if status is Pending
    //             if (status == "Pending")
    //             {
    //                 enqueue(from, to, subject, body, date, status, false);
    //             }
    //         }
    //     }
    //     inFile.close();
    // }

    void enqueue(Email newEmail)
    {
        QueueEmailNode *newEmailNode = new QueueEmailNode(newEmail);
        if (rear == nullptr)
        {
            front = rear = newEmailNode;
        }
        else
        {
            rear->next = newEmailNode;
            rear = newEmailNode;
        }

        cout << "Email added to queue with status: " << newEmailNode->data.status;
        cout << " and priority: ";
        cout << Helper::EmailPriorityToString(newEmailNode->data.priority);
        cout << "\n";
    }

    Email dequeue()
    {
        QueueEmailNode *tempNode = front;
        Email emailData = front->data;
        front = front->next;
        if (front == nullptr)
            rear = nullptr;
        delete tempNode;
        return emailData;
    }

    void displayOutbox() const
    {
        if (front == nullptr)
        {
            cout << "No emails in the Outbox.\n";
            return;
        }

        QueueEmailNode *current = front;
        cout << "\nOutbox Contents:\n";
        cout << "------------------------\n";
        while (current != nullptr)
        {
            if (current->data.status == "Pending")
            {
                cout << "From: " << current->data.sender << "\n";
                cout << "To: " << current->data.recipient << "\n";
                cout << "Subject: " << current->data.subject << "\n";
                cout << "Status: " << current->data.status << "\n";
                // cout << "Priority: ";
                // switch (current->data.priority)
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
                cout << "\n------------------------\n";
            }
            current = current->next;
        }
    }

    // void displaySentEmails() const
    // {
    //     ifstream inFile(filename);
    //     string line;
    //     getline(inFile, line); // Skip header

    //     cout << "\nSent Emails:\n";
    //     cout << "------------------------\n";
    //     bool found = false;

    //     while (getline(inFile, line))
    //     {
    //         string fields[7] = {"", "", "", "", "", "", ""}; // Initialize all elements
    //         int fieldCount = 0;
    //         splitString(line, fields, fieldCount, ',');

    //         // Only process if we have all fields and status is "Sent"
    //         if (fieldCount >= 7 && fields[6] == "Sent")
    //         {
    //             cout << "From: " << fields[0] << "\n";
    //             cout << "To: " << fields[1] << "\n";
    //             cout << "Subject: " << fields[2] << "\n";
    //             cout << "Date: " << fields[3] << "\n";
    //             cout << "Priority: " << fields[4] << "\n";
    //             cout << "Body: " << fields[5] << "\n";
    //             cout << "Status: " << fields[6] << "\n";
    //             cout << "------------------------\n";
    //             found = true;
    //         }
    //     }

    //     if (!found)
    //     {
    //         cout << "No sent emails available.\n";
    //     }

    //     inFile.close();
    // }

    ~EmailQueue()
    {
        while (front != nullptr)
        {
            QueueEmailNode *temp = front;
            front = front->next;
            delete temp;
        }
    }
};