#include "Chat.h"
#include "SecureDistributedChat.h"

using namespace std;
Chat::Chat(string s) : name(s){
}

string Chat::getName(){
  return name;
}

void Chat::addUser(const sdc::User &u){
  users[u.ID]=u;
}

void Chat::rmUser(const sdc::User &u){
  users.erase(u.ID);
}

sdc::User Chat::getUser(const string &s){
  return users[s];
}