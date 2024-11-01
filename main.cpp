#include <string>
#include <iostream>
#include <algorithm>
#include "Email.h"
#include "EmailStack.h"

using namespace std;


int main (){
  // Load emails
    EmailStack emailStack = EmailStack::getAllEmails();

    // Show inbox for a specific user (replace with actual email)
    std::string userEmail = "brittney@apu.com";
    //emailStack.showInbox(IMPORTANT, userEmail);

    // Show sent emails for a specific user
    emailStack.showSentEmails(userEmail);

    return 0;
}