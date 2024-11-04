#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "Email.h"

using namespace std;

class QueueEmail
{
public:
    string sender;
    string receiver;
    string subject;
    string body;
    string date;
    string status;
    EmailPriority priority;
    QueueEmail *next;

    QueueEmail(string snd, string rcv, string sub, string bd, string dt = "", string st = "Pending", EmailPriority pri = UNASSIGNED)
        : sender(snd), receiver(rcv), subject(sub), body(bd), date(dt), status(st), priority(pri), next(nullptr)
    {
        // Create an Email object to use detectEmailPriority
        Email tempEmail(sender, receiver, subject, body, date, priority);
        tempEmail.detectEmailPriority();
        priority = tempEmail.priority;
    }
};

class EmailQueue
{
private:
    QueueEmail *front;
    QueueEmail *rear;
    string filename;

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
    EmailQueue(string csvFilename)
        : front(nullptr), rear(nullptr), filename(csvFilename)
    {
        ifstream fileCheck(filename);
        bool fileIsEmpty = fileCheck.peek() == ifstream::traits_type::eof();
        fileCheck.close();

        if (fileIsEmpty)
        {
            ofstream file(filename, ios::app);
            file << "Sender,Recipient,Subject,Date,Priority,Body,Status\n";
            file.close();
        }

        loadFromCSV();
    }

    void loadFromCSV()
    {
        ifstream inFile(filename);
        string line;
        getline(inFile, line); // Skip header

        while (getline(inFile, line))
        {
            string fields[7]; // Array to store fields
            int fieldCount = 0;
            splitString(line, fields, fieldCount, ',');

            if (fieldCount >= 6)
            {
                string from = fields[0];
                string to = fields[1];
                string subject = fields[2];
                string date = fields[3];
                string priority_str = fields[4];
                string body = fields[5];
                string status = (fieldCount >= 7) ? fields[6] : "Pending";

                // Convert priority string to enum
                EmailPriority priority = UNASSIGNED;
                if (priority_str == "SPAM")
                    priority = SPAM;
                else if (priority_str == "NORMAL")
                    priority = NORMAL;
                else if (priority_str == "IMPORTANT")
                    priority = IMPORTANT;

                // Only add if status is Pending
                if (status == "Pending")
                {
                    enqueue(from, to, subject, body, date, status, false);
                }
            }
        }
        inFile.close();
    }

    void saveToCSV(const QueueEmail *email)
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            string priority_str;
            switch (email->priority)
            {
            case SPAM:
                priority_str = "SPAM";
                break;
            case NORMAL:
                priority_str = "NORMAL";
                break;
            case IMPORTANT:
                priority_str = "IMPORTANT";
                break;
            default:
                priority_str = "UNASSIGNED";
                break;
            }

            file << email->sender << ","
                 << email->receiver << ","
                 << email->subject << ","
                 << email->date << ","
                 << priority_str << ","
                 << email->body << ","
                 << email->status << "\n";
            file.close();
        }
        else
        {
            cout << "Error: Unable to save to CSV file!" << endl;
        }
    }

    void updateStatusInCSV(const string &sender, const string &receiver, const string &subject, const string &newStatus)
    {
        ifstream inFile(filename);
        ofstream tempFile("temp.csv");

        string line;
        getline(inFile, line);
        tempFile << "Sender,Recipient,Subject,Date,Priority,Body,Status\n";

        while (getline(inFile, line))
        {
            string fields[7];
            int fieldCount = 0;
            splitString(line, fields, fieldCount, ',');

            if (fieldCount >= 6)
            {
                string from = fields[0];
                string to = fields[1];
                string subj = fields[2];
                string date = fields[3];
                string priority = fields[4];
                string body = fields[5];
                string current_status = (fieldCount >= 7) ? fields[6] : "Pending";

                if (from == sender && to == receiver && subj == subject)
                {
                    tempFile << from << "," << to << "," << subj << ","
                             << date << "," << priority << "," << body << ","
                             << newStatus << "\n";
                }
                else
                {
                    if (fieldCount >= 7)
                    {
                        tempFile << line << "\n";
                    }
                    else
                    {
                        tempFile << line << "," << current_status << "\n";
                    }
                }
            }
        }

        inFile.close();
        tempFile.close();
        remove(filename.c_str());
        rename("temp.csv", filename.c_str());
    }

    void enqueue(const string &sender, const string &receiver, const string &subject,
                 const string &body, const string &date = "", const string &status = "Pending", bool save = true)
    {
        QueueEmail *newEmail = new QueueEmail(sender, receiver, subject, body, date, status);

        if (save)
        {
            saveToCSV(newEmail);
        }

        if (rear == nullptr)
        {
            front = rear = newEmail;
        }
        else
        {
            rear->next = newEmail;
            rear = newEmail;
        }

        cout << "Email added to queue with status: " << status;
        cout << " and priority: ";
        switch (newEmail->priority)
        {
        case SPAM:
            cout << "SPAM";
            break;
        case NORMAL:
            cout << "NORMAL";
            break;
        case IMPORTANT:
            cout << "IMPORTANT";
            break;
        default:
            cout << "UNASSIGNED";
            break;
        }
        cout << "\n";
    }

    void sendFirstEmail()
    {
        if (front == nullptr)
        {
            cout << "No emails to send.\n";
            return;
        }

        cout << "\nSending Email:\n";
        cout << "From: " << front->sender << "\n";
        cout << "To: " << front->receiver << "\n";
        cout << "Subject: " << front->subject << "\n";
        cout << "Body: " << front->body << "\n";
        cout << "Priority: ";
        switch (front->priority)
        {
        case SPAM:
            cout << "SPAM";
            break;
        case NORMAL:
            cout << "NORMAL";
            break;
        case IMPORTANT:
            cout << "IMPORTANT";
            break;
        default:
            cout << "UNASSIGNED";
            break;
        }
        cout << "\n\n";

        updateStatusInCSV(front->sender, front->receiver, front->subject, "Sent");

        QueueEmail *temp = front;
        front = front->next;
        if (front == nullptr)
            rear = nullptr;
        delete temp;

        cout << "Email sent and status updated.\n";
    }

    void displayOutbox() const
    {
        if (front == nullptr)
        {
            cout << "No emails in the Outbox.\n";
            return;
        }

        QueueEmail *current = front;
        cout << "\nOutbox Contents:\n";
        cout << "------------------------\n";
        while (current != nullptr)
        {
            if (current->status == "Pending")
            {
                cout << "From: " << current->sender << "\n";
                cout << "To: " << current->receiver << "\n";
                cout << "Subject: " << current->subject << "\n";
                cout << "Status: " << current->status << "\n";
                cout << "Priority: ";
                switch (current->priority)
                {
                case SPAM:
                    cout << "SPAM";
                    break;
                case NORMAL:
                    cout << "NORMAL";
                    break;
                case IMPORTANT:
                    cout << "IMPORTANT";
                    break;
                default:
                    cout << "UNASSIGNED";
                    break;
                }
                cout << "\n------------------------\n";
            }
            current = current->next;
        }
    }

    void displaySentEmails() const
    {
        ifstream inFile(filename);
        string line;
        getline(inFile, line); // Skip header

        cout << "\nSent Emails:\n";
        cout << "------------------------\n";
        bool found = false;

        while (getline(inFile, line))
        {
            string fields[7] = {"", "", "", "", "", "", ""}; // Initialize all elements
            int fieldCount = 0;
            splitString(line, fields, fieldCount, ',');

            // Only process if we have all fields and status is "Sent"
            if (fieldCount >= 7 && fields[6] == "Sent")
            {
                cout << "From: " << fields[0] << "\n";
                cout << "To: " << fields[1] << "\n";
                cout << "Subject: " << fields[2] << "\n";
                cout << "Date: " << fields[3] << "\n";
                cout << "Priority: " << fields[4] << "\n";
                cout << "Body: " << fields[5] << "\n";
                cout << "Status: " << fields[6] << "\n";
                cout << "------------------------\n";
                found = true;
            }
        }

        if (!found)
        {
            cout << "No sent emails available.\n";
        }

        inFile.close();
    }

    ~EmailQueue()
    {
        while (front != nullptr)
        {
            QueueEmail *temp = front;
            front = front->next;
            delete temp;
        }
    }
};

int main()
{
    string filename = "D:/Year 2/SEM 2/Data Structure/Assignment part 2/Email-Program-DSTR/DataFiles/meisuen@apu.com.csv";
    EmailQueue queue(filename);
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

            cout << "\nFrom: ";
            getline(cin, from);

            cout << "To: ";
            getline(cin, to);

            cout << "Subject: ";
            getline(cin, subject);

            cout << "Enter the body of the email: ";
            getline(cin, body);

            queue.enqueue(from, to, subject, body);
            break;
        }
        case '2':
            queue.sendFirstEmail();
            break;
        case '3':
            queue.displayOutbox();
            break;
        case '4':
            queue.displaySentEmails();
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