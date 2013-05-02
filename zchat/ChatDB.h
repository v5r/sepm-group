#ifndef CHAT_DB_H
#define CHAT_DB_H

#include "SecureDistributedChat.h"
#include <set>
#include <string>
#include <boost/thread.hpp>
#include "InterServerImpl.h"
#include "IceClient.h"

using namespace std;

class ChatDB {
public:
  static ChatDB* i();
  set<string> usersForChat(const string &chat);
  sdc::User userForString(const string &user);

  sdc::InterServerI* serverForUser(const string &user);
  sdc::InterServerI* serverForChat(const string &chat);

  string decryptMsgForChat(const string &chat, const sdc::ByteSeq &msg);
  sdc::ByteSeq encryptMsgForChat(const string &chat, const string &msg);

  void setKeyForChat(const string &chat, const sdc::ByteSeq &key);
  sdc::ByteSeq getKeyForChat(const string &chat);

  void addUserToChat(const string &chat, const string &user);
  void removeUserFromChat(const string &chat, const string &user);

  sdc::ByteSeq publicKey;
  sdc::ByteSeq privateKey;
private:
  ChatDB();
  static ChatDB* instance;
  static boost::mutex mutex;

  sdc::InterServerI* serverForString(const string &server);

  InterServerImpl localServer;

  map<string, set<string>> chatUsers; /** Map[Chat, List[User]] */
  map<string, sdc::InterServerI*> userServers; /** Map[User, Server] */
  map<string, sdc::InterServerI*> chatServers; /** Map[Chat, Server] */
  map<string, sdc::User> users; /** Map[UserStr, sdc::User] */
  map<string, sdc::ByteSeq> chatKeys; /** Map[Chat, AES-Key] */
  map<string, IceClient*> servers;
};

#endif