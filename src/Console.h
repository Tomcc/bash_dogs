#pragma once

#include "common_header.h"

namespace bash_dogs {

	class Console : 
		public Object,
		public StateInterface
	{
	public:

		enum State {
			CS_LOGIN,
			CS_CHOOSE,
			CS_NORMAL
		};

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

		Console(Object& parent, const Vector& pos);

		virtual ~Console();

		void write(const String& s);
		void backspace();

		typedef struct Line {
			String cmd; int id;
		};

		Line newLine(const String& author = String::EMPTY);

		TextArea& getLastLine() {
			return *lines.back();
		}

		virtual void onAction(float dt) override;

		Unique<Line> onKeyPressed(int key);

	protected:
		int lastLineID = 0;
		TextArea* cursor;
		float blink = 0;
		float characterRefresh = 0;

		String username, address;

		Command currentCommand;

		std::vector<TextArea*> lines;

		std::vector<Command> command;

		std::vector<char> ASCII;
		
		const String& _getCommandForKey(int key) const;

		bool _edit(String& field, int key, int maxChars);

		virtual void onStateBegin();

		virtual void onStateLoop(float dt);
	private:
	};

}



