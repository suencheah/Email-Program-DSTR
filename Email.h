#pragma once;
#include <string>
#include <sstream>
// #include <chrono>
#pragma once

using namespace std;

enum EmailPriority
{
  IMPORTANT,
  NORMAL,
  SPAM,
  UNASSIGNED
};
struct Email
{
  string sender;
  string recipient;
  string subject;
  string body;
  string timestamp;
  EmailPriority priority;

  // Default Constructor
  Email() : sender(""), recipient(""), subject(""), body(""), timestamp(""), priority(UNASSIGNED) {};

  // Constructor
  Email(string senderEmailAdd, string recipientEmailAdd, string subjectLine, string bodyContent, string emailTimestamp, EmailPriority emailPriority)
      : sender(senderEmailAdd), recipient(recipientEmailAdd), subject(subjectLine), body(bodyContent),
        timestamp(emailTimestamp), priority(emailPriority) {};

  void detectEmailPriority()
  {
    string tempSender = sender;
    string tempSubject = subject;
    string tempBody = body;
    transform(tempSender.begin(), tempSender.end(), tempSender.begin(), ::tolower);
    transform(tempSubject.begin(), tempSubject.end(), tempSubject.begin(), ::tolower);
    transform(tempBody.begin(), tempBody.end(), tempBody.begin(), ::tolower);
    int score = 0;

    // Important criteria
    if (tempSender.find("@apu.com") != string::npos)
    {
      score += 3;
      // cout << "found sender important" << endl;
    }
    if (tempSender.find("@work.com") != string::npos)
    {
      score += 3;
      // cout << "found sender important" << endl;
    }
    if (tempSubject.find("urgent") != string::npos)
    {
      score += 2;
      // cout << "found subject urgent" << endl;
    }
    if (tempBody.find("meeting") != string::npos)
    {
      score += 1;
      // cout << "found body meeting" << endl;
    }

    // Spam Criteria
    if (tempSubject.find("free") != string::npos)
    {
      score -= 2;
      // cout << "found subject Winner" << endl;
    }
    if (tempBody.find("free") != string::npos)
    {
      score -= 1;
      // cout << "found free" << endl;
    }
    if (tempBody.find("prize") != string::npos)
    {
      score -= 1;
      // cout << "found click" << endl;
    }
    if (tempBody.find("win") != string::npos)
    {
      score -= 1;
      // cout << "found win" << endl;
    }
    if (tempBody.find("cash") != string::npos)
    {
      score -= 1;
      // cout << "found cash" << endl;
    }
    if (tempBody.find("promotion") != string::npos)
    {
      score -= 1;
      // cout << "found cash" << endl;
    }

    if (score < 0)
    {
      priority = SPAM;
    }
    else if (score < 5)
    {
      priority = NORMAL;
    }
    else
    {
      priority = IMPORTANT;
    }
    // cout << to_string(score) << endl;
  }
};