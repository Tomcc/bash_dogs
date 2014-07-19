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

void RealServer::_dispatchCommand(const String& command) {

}

void RealServer::runCommand(const String& command, const ReplyCallback& callback) {

	_dispatchCommand(command);

	callback("lol");
}
