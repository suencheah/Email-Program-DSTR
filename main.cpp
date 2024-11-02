#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Email.h"
#include "EmailStack.h"
#include "User.h"

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

  return 0;
}