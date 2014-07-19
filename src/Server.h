#pragma once

#include "common_header.h"

namespace bash_dogs {
	class Server
	{
	public:

		typedef std::function<void(const String& reply)> ReplyCallback;

		virtual void runCommand(const String& command, const ReplyCallback& callback) = 0;

	protected:
	private:
	};
}



