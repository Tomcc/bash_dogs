#pragma once

#include "common_header.h"

namespace bash_dogs {

	class Console : 
		public Object,
		public InputDevice::Listener
	{
	public:

		template < typename T >
		static const T& oneOf(const std::vector<T>& strings) {
			return strings[Math::rangeRandom(0, (float)strings.size())];
		}

		struct Command {
			String command;
			std::unordered_map<int, String> parameters;

			Command() {}

			Command(const String& command, const std::unordered_map<int, String>& parameters) :
				command(command),
				parameters(parameters) {
			}

			String getParam(int action);

			operator bool() const {
				return !command.empty();
			}
		};

		Console(Object& parent, const Vector& pos, InputDevice& keyboard);

		virtual ~Console();

		void write(const String& s);

		String newLine();

		TextArea& getLastLine() {
			return *lines.back();
		}

		virtual void onAction(float dt) override;

		virtual void onButtonPressed(Dojo::InputDevice* j, int action) override;

	protected:
		InputDevice& keyboard;

		TextArea* cursor;
		float blink = 0;

		Command currentCommand;

		std::vector<TextArea*> lines;

		std::vector<Command> command;
		
		const String& _getCommandForKey(int key) const;

	private:
	};

}



