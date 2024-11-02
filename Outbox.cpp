#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Email {
public:
    string sender;
    string receiver;
    string subject;
    string body;
    string status;
    Email* next;

    Email(string snd, string rcv, string sub, string bd, string st)
        : sender(snd), receiver(rcv), subject(sub), body(bd), status(st), next(nullptr) {}
};

class EmailQueue {
private:
    Email* front;
    Email* rear;
    string filename;

public:
    EmailQueue(string csvFilename) 
        : front(nullptr), rear(nullptr), filename(csvFilename) {
        // Check if the file is empty, and if so, add the header
        ifstream fileCheck(filename);
        bool fileIsEmpty = fileCheck.peek() == ifstream::traits_type::eof();
        fileCheck.close();

        if (fileIsEmpty) {
            ofstream file(filename, ios::app);
            file << "From,To,Subject,Body,Status\n";
            file.close();
        }

        loadFromCSV();
    }

    void loadFromCSV() {
        ifstream inFile(filename);
        string line;
        getline(inFile, line);  

        while (getline(inFile, line)) {
            stringstream ss(line);
            string from, to, subject, body, status;

            getline(ss, from, ',');
            getline(ss, to, ',');
            getline(ss, subject, ',');
            getline(ss, body, ',');
            getline(ss, status, ',');

            if (status == "Pending") {
                enqueue(from, to, subject, body, status, false);  
            }
        }
        inFile.close();
    }

    void saveToCSV(const Email* email) {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << email->sender << "," 
                 << email->receiver << "," 
                 << email->subject << "," 
                 << email->body << "," 
                 << email->status << "\n";
            file.close();
        } else {
            cout << "Error: Unable to save to CSV file!" << endl;
        }
    }

    void enqueue(const string& sender, const string& receiver, const string& subject, const string& body, const string& status = "Pending", bool save = true) {
        Email* newEmail = new Email(sender, receiver, subject, body, status);
        
        if (save) {
            saveToCSV(newEmail);
        }

        if (rear == nullptr) {
            front = rear = newEmail;
        } else {
            rear->next = newEmail;
            rear = newEmail;
        }

        cout << "Email added to queue with " << status << " status.\n";
    }

    void sendFirstEmail() {
        if (front == nullptr) {
            cout << "No emails to send.\n";
            return;
        }

        cout << "\nSending Email:\n";
        cout << "From: " << front->sender << "\n";
        cout << "To: " << front->receiver << "\n";
        cout << "Subject: " << front->subject << "\n";
        cout << "Body: " << front->body << "\n\n";

        updateStatusInCSV(front->sender, front->receiver, front->subject, "Sent");
        
        Email* temp = front;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;

        cout << "Email sent and status updated.\n";
    }

    void updateStatusInCSV(const string& sender, const string& receiver, const string& subject, const string& newStatus) {
        ifstream inFile(filename);
        ofstream tempFile("temp.csv");

        string line;
        getline(inFile, line);
        tempFile << line << "\n";  // Write header to temp file

        while (getline(inFile, line)) {
            stringstream ss(line);
            string from, to, subj, body, status;
            
            getline(ss, from, ',');
            getline(ss, to, ',');
            getline(ss, subj, ',');
            getline(ss, body, ',');
            getline(ss, status, ',');

            if (from == sender && to == receiver && subj == subject) {
                tempFile << from << "," << to << "," << subj << "," << body << "," << newStatus << "\n";
            } else {
                tempFile << line << "\n";
            }
        }

        inFile.close();
        tempFile.close();
        remove(filename.c_str());
        rename("temp.csv", filename.c_str());
    }

    void displayOutbox() const {
        if (front == nullptr) {
            cout << "No emails in the Outbox.\n";
            return;
        }

        Email* current = front;
        cout << "\nOutbox Contents:\n";
        cout << "------------------------\n";
        while (current != nullptr) {
            if (current->status == "Pending") {
                cout << "From: " << current->sender << "\n";
                cout << "To: " << current->receiver << "\n";
                cout << "Subject: " << current->subject << "\n";
                cout << "Status: " << current->status << "\n";
                cout << "------------------------\n";
            }
            current = current->next;
        }
    }

    void displaySentEmails() const {
        ifstream inFile(filename);
        string line;
        getline(inFile, line);  

        cout << "\nSent Emails:\n";
        cout << "------------------------\n";
        bool found = false;

        while (getline(inFile, line)) {
            stringstream ss(line);
            string from, to, subject, body, status;

            getline(ss, from, ',');
            getline(ss, to, ',');
            getline(ss, subject, ',');
            getline(ss, body, ',');
            getline(ss, status, ',');

            if (status == "Sent") {
                cout << "From: " << from << "\n";
                cout << "To: " << to << "\n";
                cout << "Subject: " << subject << "\n";
                cout << "Body: " << body << "\n";
                cout << "Status: " << status << "\n";
                cout << "------------------------\n";
                found = true;
            }
        }

        if (!found) {
            cout << "No sent emails available.\n";
        }

        inFile.close();
    }

    ~EmailQueue() {
        while (front != nullptr) {
            Email* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

int main() {
    string filename = "emails.csv";
    EmailQueue queue(filename);
    char choice;

    do {
        cout << "\nEmail System Menu:\n";
        cout << "1. Compose Email\n";
        cout << "2. Send First Email in Outbox\n";
        cout << "3. View Outbox\n";
        cout << "4. View Sent Emails\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case '1': {
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
