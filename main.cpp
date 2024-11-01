#include <string>
#include <iostream>
#include <algorithm>
#include "Email.h"
#include "EmailStack.h"
#include "User.h"

using namespace std;

int main()
{
  // Testing
  // User::registerNewUser("meisuen@apu.com", "meisuen");
  bool passedAuth = User::authenticateUser("meisuen@apu.com", "meisuen");

  User myUser = User("meisuen@apu.com");
  myUser.receiveEmails();

  myUser.showInbox(NORMAL);
  cout << "!!!!!!!!!!!!!!!!!!!\n" << endl;
  myUser.showSentEmails();

  return 0;
}