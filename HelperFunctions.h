#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

enum EmailPriority
{
  IMPORTANT,
  NORMAL,
  SPAM,
  UNASSIGNED
};

struct Helper
{
  static string getCurrentTimestamp()
  {
    // get current time
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);

    // Format the time as DD-MM-YYYY HH:MM
    stringstream ss;
    ss << put_time(localtime(&now_time), "%d-%m-%Y %H:%M");
    return ss.str();
  }

  static string output_csv(string s)
  {
    if (s.find('"') != string::npos)
    { // Escape double-quotes
      for (string::size_type n = 0; (n = s.find('"', n)) != string::npos; n += 2)
        s.replace(n, 1, "\"\"");
      return '"' + s + '"';
    }
    else if (s.find(',') != string::npos)
      return '"' + s + '"';
    else
      return s;
  }

  #include <iostream>
#include <sstream>
#include <string>

    static void parseCSVLine(const std::string &line, std::string &sender, std::string &recipient,
                             std::string &subject, std::string &date, std::string &priority,
                             std::string &body, std::string &status) {
        std::istringstream stream(line);
        std::string temp;
        bool inQuotes = false;
        int fieldCount = 0;

        while (std::getline(stream, temp, ',')) {
            // Handle quotes
            if (temp.front() == '"' && temp.back() == '"') {
                temp = temp.substr(1, temp.size() - 2); // Remove surrounding quotes
            } else if (temp.front() == '"') {
                inQuotes = true;
                temp = temp.substr(1); // Remove starting quote
            }

            if (inQuotes) {
                // Continue reading until we find the closing quote
                std::string next;
                while (std::getline(stream, next, ',')) {
                    temp += ',' + next; // Append to the current field
                    if (next.back() == '"') {
                        inQuotes = false;
                        temp = temp.substr(0, temp.size() - 1); // Remove closing quote
                        break;
                    }
                }
            }

            // Replace escaped quotes
            size_t pos = 0;
            while ((pos = temp.find("\"\"", pos)) != std::string::npos) {
                temp.replace(pos, 2, "\"");
                pos++; // Move past the replacement
            }

            // Assign fields based on count
            switch (fieldCount) {
                case 0: sender = temp; break;
                case 1: recipient = temp; break;
                case 2: subject = temp; break;
                case 3: date = temp; break;
                case 4: priority = temp; break;
                case 5: body = temp; break;
                case 6: status = temp; break;
            }
            fieldCount++;
        }
    }



  static EmailPriority stringToEmailPriority(string priorityString)
  {
    if (priorityString == "IMPORTANT")
    {
      return IMPORTANT;
    }
    else if (priorityString == "NORMAL")
    {
      return NORMAL;
    }
    else if (priorityString == "SPAM")
    {
      return SPAM;
    }
    else
    {
      return UNASSIGNED;
    }
  }

  static string EmailPriorityToString(EmailPriority priority)
  {
    switch (priority)
    {
    case IMPORTANT:
      return "IMPORTANT";
    case NORMAL:
      return "NORMAL";
    case SPAM:
      return "SPAM";
    case UNASSIGNED:
      return "UNASSIGNED";
    default:
      return "UNASSIGNED";
    }
  }
};