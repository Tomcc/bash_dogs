#include "stdafx.h"

#include "RealServer.h"
#include "Console.h"
#include "Level.h"
#include "File.h"

#include <Poco/Net/ServerSocket.h>
#include "FileSystem.h"

using namespace bash_dogs;
using namespace Poco;
using namespace Poco::Net;

RealServer::RealServer(Level& level) :
level(level) {
	serverSocket = make_unique<ServerSocket>(PORT);

	new std::thread([this](){
		auto ss = serverSocket->acceptConnection();
		serverSocket = nullptr;

		LockGuard lock();
		clientSocket = make_unique<StreamSocket>(ss);
	});


	masterFileSystem = make_unique<FileSystem>(level, Vector::ZERO, false);

	//make the filesystem structure


}

RealServer::~RealServer() {

}

Dojo::String bash_dogs::RealServer::_dispatchCommand(const String& line, const String& param) {

	String command = line.substr(0, line.find_first_of(' '));

	if (command == String("rtfm")) {

		//pick some random commands
		return level.getConsole().getHelp(15);
	}
	else if (command == String("changedir")) {
		//HACK
		String name = param.substr(9);
		auto& fs = *level.getFileSystem();

		auto file = fs.getFile(name);
		if (!file)
			return "echo={\"ERROR: FILE NOT FOUND\"}";
		else if (file->isDeleted())
			return "echo={\"ERROR: FILE IS DELETED\"}";
		else if (file->isLocked())
			return "echo={\"ERROR: FILE IS LOCKED\"}";
		else if (!fs.canMoveBetween(*file, fs.getSelected() ))
			return "echo={\"ERROR: FILE IS NOT REACHABLE FROM HERE\"}";
		else
			return "setdir=\"" + name + "\" echo={\"Directory changed\"}";
	}
	else
		return String::EMPTY;
}

void RealServer::runCommand(const String& command, const String& param, const ReplyCallback& callback) {

	callback(_dispatchCommand(command, param));
}
