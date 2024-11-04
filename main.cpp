#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Email.h"
#include "EmailStack.h"
#include "User.h"
#include "EmailSearch.h"

using namespace std;

int main()
{
  // Testing
  User::registerNewUser("meisuen@apu.com", "meisuen");
  bool passedAuth = User::authenticateUser("meisuen@apu.com", "meisuen");

  User myUser = User("meisuen@apu.com");

  myUser.importantEmails.showEmails("important mails");
  // myUser.normalEmails.showEmails("normal mails");
  // myUser.spamEmails.showEmails("spam mails");
  // myUser.sentEmails.showEmails("sent mails");

    // Test search functionality
    string keyword;
    cout << "Enter keyword to search for: ";
    cin >> keyword;
    EmailSearch::searchEmails(myUser, keyword, "all");

  return 0;
}//