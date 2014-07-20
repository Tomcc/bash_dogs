#include "stdafx.h"

#include "Client.h"

using namespace bash_dogs;
using namespace Poco;
using namespace Poco::Net;


bash_dogs::Client::Client(const String& address) {
	auto str = (address + ":" + String(PORT)).ASCII();

	try {
		SocketAddress addr(str);
		socket = make_unique<StreamSocket>(addr);
	}
	catch (...) {
		//meh, exceptions suck
	}
}

void bash_dogs::Client::runCommand(const String& command, const String& param, const ReplyCallback& callback) {



}
