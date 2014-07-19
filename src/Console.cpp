#include "stdafx.h"

#include "Console.h"
#include "Level.h"

using namespace Dojo;
using namespace bash_dogs;

String bash_dogs::Console::Command::getParam(int key) {
	auto elem = parameters.find(key);
	if (elem != parameters.end()) {
		auto res = elem->second;
		parameters.erase(elem);
		return res;
	}
	else
		return String::EMPTY;
}


bash_dogs::Console::Console(Level& level, const Vector& pos) :
Object(&level, pos),
level(level) {

	newLine();
	newLine();

	write("Welcome To HAXXOR TERMINAL");
	newLine();

	write("  (c)2054 T.C. - G.P.");
	newLine();

	write("TYPE SHIT TO HACKER");
	newLine();

	write("-------------------");
	newLine();

	cursor = new TextArea(this, "debugFont", Vector::ZERO);
	cursor->addText("_");
	addChild(cursor, (int)Layers::LL_CONSOLE);

	command.resize(256);

	//initialize commands
	command[KC_A] = Command("alert-system", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_B] = Command("ban", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_C] = Command("changedir", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_D] = Command("delete", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_E] = Command("entropy", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_F] = Command("filesystem", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_G] = Command("goto", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_H] = Command("hide", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_I] = Command("install-get", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_J] = Command("jump-to", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_K] = Command("key-generate", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_L] = Command("list-files", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_M] = Command("mount-fs", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_N] = Command("network-map", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_O] = Command("open", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_P] = Command("perl", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_Q] = Command("quit", {});
	command[KC_R] = Command("rtfm", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_S] = Command("sort", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_T] = Command("tar-lz", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_U] = Command("unlink", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_V] = Command("view", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_W] = Command("wget", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_X] = Command("x-conf", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_Y] = Command("yzf", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_Z] = Command("zip", { { KC_F, "foo" }, { KC_B, "bar" } });

	ASCII.resize(256);
	ASCII[KC_A] = 'A';
	ASCII[KC_B] = 'B';
	ASCII[KC_C] = 'C';
	ASCII[KC_D] = 'D';
	ASCII[KC_E] = 'E';
	ASCII[KC_F] = 'F';
	ASCII[KC_G] = 'G';
	ASCII[KC_H] = 'H';
	ASCII[KC_I] = 'I';
	ASCII[KC_J] = 'J';
	ASCII[KC_K] = 'K';
	ASCII[KC_L] = 'L';
	ASCII[KC_M] = 'M';
	ASCII[KC_N] = 'N';
	ASCII[KC_O] = 'O';
	ASCII[KC_P] = 'P';
	ASCII[KC_Q] = 'Q';
	ASCII[KC_R] = 'R';
	ASCII[KC_S] = 'S';
	ASCII[KC_T] = 'T';
	ASCII[KC_U] = 'U';
	ASCII[KC_V] = 'V';
	ASCII[KC_W] = 'W';
	ASCII[KC_X] = 'X';
	ASCII[KC_Y] = 'Y';
	ASCII[KC_Z] = 'Z';
	ASCII[KC_0] = '0';
	ASCII[KC_1] = '1';
	ASCII[KC_2] = '2';
	ASCII[KC_3] = '3';
	ASCII[KC_4] = '4';
	ASCII[KC_5] = '5';
	ASCII[KC_6] = '6';
	ASCII[KC_7] = '7';
	ASCII[KC_8] = '8';
	ASCII[KC_9] = '9';
	ASCII[KC_PERIOD] = '.';

	setState(CS_LOGIN);
}

bash_dogs::Console::~Console() {

}

void bash_dogs::Console::write(const String& s) {
	getLastLine().addText(s);

	if (isCurrentState(CS_NORMAL)) {
		commandText += s;
	}
}

void bash_dogs::Console::backspace() {
	auto old = getLastLine().getContent();
	getLastLine().clearText();
	getLastLine().addText(old.substr(0, old.size() - 1));

	if (isCurrentState(CS_NORMAL)) {
		commandText.resize(commandText.size() - 1);
	}
}

bash_dogs::Console::Line bash_dogs::Console::newLine(const String& author) {
	const Vector interline(0, -0.4);

	String res = commandText;
	if (lines.size() > 0) {

		if (lines.size() > 20) {

			if (getLastLine().position.y < -9) {
				lines.front()->dispose = true;
				lines.erase(lines.begin());

				for (auto l : lines)
					l->position -= interline;
			}
		}
	}

	commandText = String::EMPTY;
	currentCommand = Command();

	//create a new one
	TextArea* line = new TextArea(this, "debugFont", interline * lines.size(), false, getParent()->getSize());
	addChild(line, (int)Layers::LL_CONSOLE);
	line->addText( author + "> ");
	line->color = Color::GREEN;
	line->setVisibleCharacters(username.size() + 1);
	line->setVisible(false);

	lines.push_back(line);
	++lastLineID;

	return { res, lastLineID - 1 };
}

bool bash_dogs::Console::_edit(String& field, int key, int maxChars) {
	auto c = ASCII[key];

	if (key == KC_RETURN)
		return field.size() > 0;
	else if (key == KC_BACK) {
		if (field.size() > 0) {
			field.resize(field.size() - 1);
			backspace();
		}
	}
	else if (c && field.size() < maxChars) {
		field += c;
		write(c);
	}
	return false;
}

void bash_dogs::Console::onStateBegin() {

	if (isCurrentState(CS_LOGIN)) {
		write("1. We would like you to LOGIN:");
		newLine();
		write("username: ");
	}
	if (isCurrentState(CS_CHOOSE)) {
		
		newLine();
		write("2. Please select an ATTACK or DEFENSE target");
		newLine(username);
		write("address: ");
	}
	else if (isCurrentState(CS_NORMAL)) {
		newLine();
		write("3. WELCOME, '" + username + "'");
		newLine();
		if (level.getServer()->isLocalHost())
			write("   PREPARE TO DEFEND THE MAINFRAME");
		else
			write("   ATTACKING `" + address + "'");

		newLine(username);
	}
}

void bash_dogs::Console::onStateLoop(float dt) {
	
}

Unique<bash_dogs::Console::Line> bash_dogs::Console::onKeyPressed(int key) {

	if (isCurrentState(CS_LOGIN)) {
		if (_edit(username, key, 9))
			setState(CS_CHOOSE);
	}
	else if (isCurrentState(CS_CHOOSE)) {
		if (_edit(address, key, 16)) {

			if (level.connect(address))
				setState(CS_NORMAL);
			else {
				address = String::EMPTY;

				newLine();
				write("ADDRESS NOT RECOGNIZED");
				newLine(username);
				write("address: ");
			}
		}
	}
	else if (isCurrentState(CS_NORMAL)) {
		if (key == KC_RETURN)
			return make_unique<Line>(newLine(username));
		else if (!currentCommand) {

			if (key == KC_TAB) {
				while (!currentCommand)
					currentCommand = oneOf(command);
			}
			else
				currentCommand = command[key];

			if (currentCommand)
				write(currentCommand.command);
		}
		else {
			String param = currentCommand.getParam(key);
			if (!param.empty())
				write(" --" + param);
		}
	}

	return nullptr;
}

void bash_dogs::Console::onStateEnd() {
	
}

void bash_dogs::Console::onAction(float dt) {
	Object::onAction(dt);

	loop(dt);

	blink += dt;
	if (blink > 0.5f) {
		cursor->setVisible(!cursor->isVisible());
		blink = 0;
	}

	characterRefresh += dt;
	if (characterRefresh > 0.015f) {
		for (auto& l : lines) {
			l->setVisibleCharacters(l->getVisibleCharacters() + 1);
		}
		characterRefresh = 0;
	}

	//make sure that invisible lines are invisible (duh)
	for (auto& l : lines)
		l->setVisible(l->canBeRendered());

	cursor->position.x = getLastLine().position.x + getLastLine().getSize().x * getLastLine().scale.x;
	cursor->position.y = getLastLine().position.y;
}