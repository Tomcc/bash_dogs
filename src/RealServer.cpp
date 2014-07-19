#include "stdafx.h"

#include "RealServer.h"

#include <Poco/Net/ServerSocket.h>

using namespace bash_dogs;
using namespace Poco;
using namespace Poco::Net;

RealServer::RealServer() {
	serverSocket = make_unique<ServerSocket>(PORT);

	new std::thread([this](){
		auto ss = serverSocket->acceptConnection();
		serverSocket = nullptr;

		LockGuard lock();
		clientSocket = make_unique<StreamSocket>(ss);
	});
}

RealServer::~RealServer() {

}

Dojo::String bash_dogs::RealServer::_dispatchCommand(const String& line) {

	String command = line.substr(0, line.find_first_of(' '));

	if (command == String("network-map") ) {
		return "LOL";
	}
	return "Error";
}

void RealServer::runCommand(const String& command, const ReplyCallback& callback) {

	callback(_dispatchCommand(command));
}
