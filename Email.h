#include <string>
#include <fstream> // for file operations
#include <utility> // for std::pair
#include <sstream>
#include <algorithm> // for transform and sorting
#include <iomanip>   // for std::fixed and std::setprecision
#include <cmath>     // for round() function
#include <chrono>
#include "InboxStack.h"
// #pragma once 

using namespace std;

enum EmailPriority { IMPORTANT, NORMAL, SPAM};
struct Email{
  string sender;
  string receiver;
  string subject;
  string body;
  string timestamp;
  EmailPriority priority;

  void getEmails(InboxStack *ImportantEmails, InboxStack *NormalEmails, InboxStack *SpamEmails)

};