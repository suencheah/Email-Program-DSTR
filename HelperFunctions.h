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
  UNASSIGNED,
  DELETED
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
                         std::string &body, std::string &status)
{
    std::string temp;
    bool inQuotes = false;
    int fieldCount = 0;
    
    for (size_t i = 0; i < line.size(); ++i) {
        char currentChar = line[i];
        
        // Check if we’re entering or exiting a quoted section
        if (currentChar == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                // Handle escaped quote ("" -> ")
                temp += '"';
                ++i;  // Skip the next quote
            } else {
                // Toggle the inQuotes flag
                inQuotes = !inQuotes;
            }
        } else if (currentChar == ',' && !inQuotes) {
            // If we hit a comma and we're not inside quotes, finalize the current field
            switch (fieldCount) {
                case 0: sender = temp; break;
                case 1: recipient = temp; break;
                case 2: subject = temp; break;
                case 3: date = temp; break;
                case 4: priority = temp; break;
                case 5: body = temp; break;
                case 6: status = temp; break;
            }
            temp.clear();  // Clear temp for the next field
            fieldCount++;  // Move to the next field
        } else {
            // Regular character, add to current field content
            temp += currentChar;
        }
    }

    // Assign the last field after the loop
    switch (fieldCount) {
        case 0: sender = temp; break;
        case 1: recipient = temp; break;
        case 2: subject = temp; break;
        case 3: date = temp; break;
        case 4: priority = temp; break;
        case 5: body = temp; break;
        case 6: status = temp; break;
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
    else if (priorityString == "DELETED")
    {
      return DELETED;
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
    case DELETED:
      return "DELETED";
    default:
      return "UNASSIGNED";
    }
  }
};