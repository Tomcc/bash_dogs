#pragma once

#include "common_header.h"

namespace bash_dogs {

	class Level;

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
			String helpText;

			Command() {}

			Command(const String& command, const String& help);

			void addParameter(int i, const String& p);

			String getParam(int action);

			operator bool() const {
				return !command.empty();
			}
		};

		Console(Level& level, const Vector& pos);

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

		Level& level;

		int lastLineID = 0;
		TextArea* cursor;
		float blink = 0;
		float characterRefresh = 0;

		String username, address;

		Command currentCommand;
		String commandText;

		std::vector<TextArea*> lines;

		std::vector<Command> command;

		std::vector<char> ASCII;
		
		bool _edit(String& field, int key, int maxChars);

		virtual void onStateBegin();

		virtual void onStateLoop(float dt);

		virtual void onStateEnd();

		void _addCommand(const String& command, const std::vector<String>& parameters, const String& help);

		int _keyForChar(char c) const;

	private:
	};

}



