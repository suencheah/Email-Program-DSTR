#pragma once
#include <string>

using namespace std;

enum EmailPriority
{
  IMPORTANT,
  NORMAL,
  SPAM,
  UNASSIGNED
};


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