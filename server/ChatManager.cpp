#include "ChatManager.h"
#include "Chat.h"
#include <boost/format.hpp>

using namespace std;

ChatManager::ChatManager() : i(0) {
}

shared_ptr<Chat> ChatManager::newChat() {
  auto name = boost::format("Chat%1%") % (i++);
	shared_ptr<Chat> cp(new Chat(name.str()));
  chats[cp->getName()]=cp;
  return cp;
}

void ChatManager::rmChat(string){
	//TODO implement rmChat
}
