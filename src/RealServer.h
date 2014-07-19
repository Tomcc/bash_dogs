#pragma once

#include "common_header.h"

#include "Server.h"
#include "FileSystem.h"

namespace Poco {
	namespace Net {
		class ServerSocket;
		class StreamSocket;
	}
}

namespace bash_dogs {
	class RealServer : public Server
	{
	public:

		RealServer(Level& level);

		~RealServer();

		virtual void runCommand(const String& command, const ReplyCallback& callback) override;

		virtual bool isLocalHost() const override {
			return true;
		}

	protected:
		Level& level;

		Unique<Poco::Net::ServerSocket> serverSocket;
		Unique<Poco::Net::StreamSocket> clientSocket;

		std::mutex clientSocketMutex;

		Unique<FileSystem> masterFileSystem;

		String _dispatchCommand(const String& command);

	};
}



