#include <string>
#include <iostream>
#include <algorithm>
#include "Email.h"
#include "EmailStack.h"
#include "User.h"

using namespace std;

int main()
{
  // User::registerNewUser("meisuen@apu.com", "meisuen");
  bool passedAuth = User::authenticateUser("meisuen@apu.com", "meisuen");

  if (passedAuth) {
    User myUser = User("meisuen@apu.com");
    myUser.receiveEmails();
  }

  return 0;
}