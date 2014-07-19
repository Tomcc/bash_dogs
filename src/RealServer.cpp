#include "stdafx.h"

#include "RealServer.h"

using namespace bash_dogs;

void RealServer::runCommand(const String& command, const ReplyCallback& callback) {
	callback("lol");
}
