#pragma once

#include "common_header.h"

#include "Server.h"

namespace bash_dogs {
	class Client : public Server
	{
	public:

		Client(const String& address);

		virtual bool isLocalHost() const override {
			return false;
		}

		virtual void runCommand(const String& command, const String& param, const ReplyCallback& callback) override;

		bool isConnected() const {
			return socket != nullptr;
		}

	protected:
		Unique<Poco::Net::StreamSocket> socket;
	private:
	};
}



