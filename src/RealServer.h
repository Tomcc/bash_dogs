#pragma once

#include "common_header.h"

#include "Server.h"

namespace bash_dogs {
	class RealServer : public Server
	{
	public:
		virtual void runCommand(const String& command, const ReplyCallback& callback) override;

		virtual bool isLocalHost() const override {
			return true;
		}

	protected:
	private:



	};
}



