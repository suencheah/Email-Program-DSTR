#pragma once
#include <iostream>
#include <string>

using namespace std;

enum EmailPriority
{
  IMPORTANT,
  NORMAL,
  SPAM,
  UNASSIGNED
};

  void output_csv(ostream &out, string s)
  {
    if (s.find('"') != string::npos)
    { // Escape double-quotes
      for (string::size_type n = 0; (n = s.find('"', n)) != string::npos; n += 2)
        s.replace(n, 1, "\"\"");
      out << '"' << s << '"';
    }
    else if (s.find(',') != string::npos)
      out << '"' << s << '"';
    else
      out << s;
  }

  static EmailPriority stringToEmailPriority(string priorityString)
  {
    if (priorityString == "IMPORTANT") {
      return IMPORTANT;
    } else if (priorityString == "NORMAL") {
      return NORMAL;
    } else if (priorityString == "SPAM") {
      return SPAM;
    } else {
      return UNASSIGNED;
    }
  }

  static string EmailPriorityToString(EmailPriority priority){
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