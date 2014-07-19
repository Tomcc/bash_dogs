#pragma once

#include "common_header.h"

#include "Server.h"

namespace bash_dogs {
	class Client : public Server
	{
	public:

		virtual bool isLocalHost() const override {
			return false;
		}

	protected:
	private:
	};
}



