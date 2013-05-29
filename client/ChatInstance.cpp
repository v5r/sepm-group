#include "ChatInstance.h"

#include <vector>
#include <algorithm>

//Qt
#include <QString>
#include <QTime>

//homebrew
#include "Logging.h"

//helper
#include "sdcHelper.h"

	ChatInstance::ChatInstance(const sdc::StringSeq &users, const string &chatID, const sdc::ByteSeq &key,
		function<void(sdc::ByteSeq, string)> sendCallback, function<void(string)> leaveChatCallback): 
		users(users), chatID(chatID), key(key), sendCallback(sendCallback){
		
		//TODO Parameterzuweiseung nach oben verschieben
		this->leaveChatCallback = leaveChatCallback;

		//UI
		chatwin = std::make_shared<QDialog>();
    	cw_ui.setupUi(chatwin.get());

    	//set title
    	chatwin->setWindowTitle(QString::fromStdString(chatID));
    	chatwin->show();

    	//Connect Signals to Slots
		connect(cw_ui.btn_send, SIGNAL(clicked()), this, SLOT(sendMessage()));
		connect(cw_ui.btn_leave, SIGNAL(clicked()), this, SLOT(leaveChat()));
		connect(cw_ui.btn_invite, SIGNAL(clicked()), this, SLOT(invite()));
	}

	/**
	 * sendMessage
	 *
	 * Reads the Message from the Chat-Input and fowards it to the
	 * ChatManager and further consequence to a Server
	 *
	 */
	void ChatInstance::sendMessage(){
		
		//TODO Exceptions

		INFO("Send message from Chat-Window to Chat-Manager via Callback-Object");

		QString message = cw_ui.txt_message->toPlainText();

		cw_ui.txt_message->setText("");

		sendCallback(sdc::sdcHelper::bStringToByteSeq(message.toLocal8Bit().data(),
			message.length()), this->chatID);
	}

	/**
	 * addChatParticipant
	 *
	 * Add Chat-Participant to Chat
	 *
	 */
	void ChatInstance::addChatParticipant(const sdc::User &participant){

		INFO("Try to add Participant to Chat");

		if(findUser(QString::fromStdString(participant.ID)) != -1){
			INFO("User is already Participant");
		}else{
			INFO("User will be added to Participant List");

			//Add user at the end of Participant-List
			users.push_back(participant.ID);
		}
	}

	/**
	 * removeChatParticipant
	 *
	 * Remove Chat-Participant from Chat
	 *
	 */
	void ChatInstance::removeChatParticipant(const sdc::User &participant){
		
		//TODO Excpetion, if no user found

		INFO("Try to Remove User from Participant-List");

		boost::optional<int> index = findUser(QString::fromStdString(participant.ID));

		if(index){
			INFO("User will be removed from Participant List");
			users.erase(users.begin(),users.begin() + *index);			
		}else{
			INFO("User is not Participant!");
		}
	}

	/**
	 * appendMessageToChat
	 *
	 * append Message to Chat-Window
	 *
	 */
	void ChatInstance::appendMessageToChat(const sdc::ByteSeq &message, const sdc::User &participant){

		INFO("Try to append Message to Chat-Window!");

		QTime time = QTime::currentTime();
		QString timeString = time.toString();

		QString text = cw_ui.txt_message->toPlainText();

		//Convert Byte-Array to String
		QString msg = QString::fromStdString(sdc::sdcHelper::getBinaryString(message));

		QString username = QString::fromStdString(sdc::sdcHelper::getNameFromID(participant.ID));

		//Append Message to Chat-Window
		cw_ui.txt_chat->setText(cw_ui.txt_chat->text() + "[" + timeString + "] " +
			username + " " + msg + "\n");

		INFO("Message appended to Chat Window");
	}

	void ChatInstance::leaveChat(){
		
		//TODO implement
		chatwin->done(1);
		leaveChatCallback(this->chatID);
		//send Chat-Messages to Server for saving it??





	}

	void ChatInstance::invite(){
		INFO("USER will be added...in the future...maybe!");
	}

	/**
	 * findUser
	 *
	 * find user in the List of Chat-Participants
	 *
	 * @return -1 if no users is found, index of User in participant lists otherwise
	 */

	boost::optional<int> ChatInstance::findUser(const QString &userID){

		INFO("Try to find User ins Participant List and return Position");

		int index = find(users.begin(), users.end(), userID.toStdString()) - users.begin();

		if(index < (int)users.size()) {
			return { index };
		} else {
			return { };
		}
	}

	std::string ChatInstance::id(){
		return chatID;
	}

	ChatInstance::~ChatInstance(){
		//TODO implement
	}