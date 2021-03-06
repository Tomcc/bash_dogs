#include "stdafx.h"

#include "Console.h"
#include "Level.h"
#include <xlocale>
#include <locale>

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

bash_dogs::Console::Command::Command(const String& command, const String& help) :
command(command),
helpText(help)
{

}

void bash_dogs::Console::Command::addParameter(int i, const String& p) {
	DEBUG_ASSERT(i, "wut");

	parameters[i] = p;
}


int bash_dogs::Console::_keyForChar(char c) const {
	char caps = c - 32;
	for (int k = 0; k < ASCII.size(); ++k) {
		if (ASCII[k] == c || ASCII[k] == caps)
			return k;
	}
	return 0;
}

void bash_dogs::Console::_addCommand(const String& cmd, const std::vector<String>& parameters, const String& help) {

	auto& c = (command[_keyForChar(cmd[0])] = Command(cmd, help));

	for (auto& p : parameters)
		c.addParameter(_keyForChar(p[0]), p);
}

bash_dogs::Console::Console(Level& level, const Vector& pos) :
Object(&level, pos),
level(level) {

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
	ASCII[KC_MINUS] = '_';

	command.resize(256);

	//initialize commands

	_addCommand(
		"alert-system",
		{ "fire-splines", "security-splice" },
		"");

	_addCommand(
		"ban",
		{ "no-mercy", "delete-userdir", "bash" },
		"");

	_addCommand(
		"changedir",
		{},
		"Jumps to an open file or directory");

	_addCommand(
		"delete",
		{ "recursive", "ignore-fail-on-non-empty", "parents" },
		"Deletes an open file or directory");

	_addCommand(
		"entropy",
		{ "reticulate-splines", "instruct-object" },
		"Teleports the opponent at random");

	_addCommand(
		"filesystem-check",
		{ "fragcheck=YES", "discard", "l=DIRECTORY_LOCK" },
		"Restores a deleted file");

	_addCommand(
		"goto",
		{ "foo", "bar" },
		"");

	_addCommand(
		"hide",
		{ "foo", "bar" },
		"Make an open or locked file unknown");

	_addCommand(
		"install-get",
		{ "foo", "bar" },
		"");

	_addCommand(
		"jump-to",
		{ "skip-subdir", "no-symlinks", "zap-tree" },
		"Jumps to a discovered file");

	_addCommand(
		"key-generate",
		{ "foo", "bar" },
		"Unlocks a locked file");

	_addCommand(
		"list-files",
		{ "foo", "bar" },
		"If in a folder, discovers all files");

	_addCommand(
		"mount-fs",
		{ "make-runbindable", "bind=/dev/root/usr", "no-canonicalize" },
		"");

	_addCommand(
		"network-map",
		{ "exclude-external", "open|closed", "top-ports" },
		"Shows a part of the network");

	_addCommand(
		"open",
		{ "foo", "bar" }, "");
	_addCommand(
		"perl",
		{ "foo", "bar" }, "");

	_addCommand(
		"quit",
		{ "are-you-sure=Y/N" },
		"Quits the game");

	_addCommand(
		"rtfm",
		{ "dude", "gpresult", "endlocal" },
		"Prints out this list");

	_addCommand(
		"sort",
		{ "foo", "bar" }, "");
	_addCommand(
		"tar-lz",
		{ "foo", "bar" }, "");

	_addCommand(
		"unlink",
		{ "foo", "bar" },
		"Makes a known file unknown");

	_addCommand(
		"view",
		{ "foo", "bar" },
		"Makes an unknown file known");

	_addCommand(
		"wget",
		{ "foo", "bar" },
		"Starts a download to transfer the file to the attacker");

	_addCommand(
		"x-conf",
		{ "foo", "bar" }, "");
	_addCommand(
		"yzf",
		{ "foo", "bar" }, "");
	_addCommand(
		"zip",
		{ "foo", "bar" }, "");

	logo = {
		" _____ _____ _____ _____       ____  _____ _____ _____ ",
		"| __  |  _  |   __|  |  |     |    \|     |   __|   __|",
		"| __ -|     |__   |     |     |  |  |  |  |  |  |__   |",
		"|_____|__|__|_____|__|__|_____|____/|_____|_____|_____|",
		"                        |_____|                 ", };

	cursor = new TextArea(this, "debugFont", Vector::ZERO);
	cursor->addText("_");
	addChild(cursor, (int)Layers::LL_CONSOLE);

	if (Platform::getSingleton()->getUserConfiguration().getBool("skip_intro"))
		setState(CS_LOGIN);
	else
		setState(CS_INTRO1);
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
	getLastLine().setVisibleCharacters(getLastLine().getContent().size());

	if (isCurrentState(CS_NORMAL)) {
		commandText.resize(commandText.size() - 1);
	}
}

const Vector interline(0, -0.4);

void bash_dogs::Console::scrollUp( bool force /*= false*/ ) {

	if (getLastLine().position.y < -9 || force) {
		//lines.front()->dispose = true;
		lines.erase(lines.begin());

		for (auto l : lines)
			l->position -= interline;
	}
}

bash_dogs::Console::Line bash_dogs::Console::newLine(const String& author) {

	const int usernameLength = username.size() + 1;
	String param, res = commandText;
	if (lines.size()>0) {
		int cmdlen = usernameLength + commandText.size() + 2;
		if (commandText.size() > 0 && getLastLine().getContent().size() > cmdlen)
			param = getLastLine().getContent().substr(cmdlen);

		if (lines.size() > 20)
			scrollUp();
	}

	commandText = String::EMPTY;
	currentCommand = Command();

	//create a new one
	TextArea* line = new TextArea(this, "debugFont", interline * lines.size(), false, getParent()->getSize());
	addChild(line, (int)Layers::LL_CONSOLE);
	line->addText( author + "> ");
	line->color = Color::GREEN;
	line->setVisibleCharacters(usernameLength);
	line->setVisible(false);
	line->setBlending(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);

	lines.push_back(line);
	++lastLineID;

	return { res, param, lastLineID - 1 };
}

bool bash_dogs::Console::_edit(String& field, int key, int maxChars, const String& defaultName) {
	auto c = ASCII[key];

	if (!defaultName.empty() && Platform::getSingleton()->getUserConfiguration().existsAs(defaultName, Table::FT_STRING)) {
		field = Platform::getSingleton()->getUserConfiguration().getString(defaultName);
		return true;
	}

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

	if (isCurrentState(CS_INTRO1)) {
		Platform::getSingleton()->getSoundManager()->playSound(getGameState()->getSound("dialup"), 0.1f);

		newLine();
		write("#!/bin/bash_dogs");
	}
	else if (isCurrentState(CS_INTRO2)) {
		newLine();
		write("PRESS ANY KEY TO CONTINUE (YEAH/N)");
		newLine();
	}
	else if (isCurrentState(CS_INTRO3)) {
		introCount = 0;
	}
	else if (isCurrentState(CS_INTRO4)) {
		introCount = 0;

		write("Welcome To H4XX0R TERMINAL");
		newLine();

		write("  (c)2054 T.C. - G.P.");
		newLine();

		write("TYPE SHIT TO HACKER");
		newLine();

		write("-------------------");
		newLine();
	}
	else if (isCurrentState(CS_LOGIN)) {

		newLine();
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

	}
	else if (isCurrentState(CS_FREEFORM_INPUT)) {
		commandField.clear();
		write(" --target=");
	}
}

void bash_dogs::Console::onStateLoop(float dt) {
	if (isCurrentState(CS_INTRO1)) {

		logoTimer += dt;
		if (logoTimer > 0.15f) {

			if (introCount < 10) {
				newLine();
				++introCount;
			}
			else if (!logo.empty()) {
				write(logo.front());
				logo.erase(logo.begin());

				newLine();

				logoTimer = 0;
			}
			else {
				setState(CS_INTRO2);
			}
		}
	}
	else if (isCurrentState(CS_INTRO3)) {
		logoTimer += dt;

		if (logoTimer > 0.05f) {
			++introCount;

			newLine();

			logoTimer = 0;
			if (introCount > 17)
				setState(CS_INTRO4);
		}
	}
	else if (isCurrentState(CS_INTRO4)) {
		logoTimer += dt;

		if (logoTimer > 0.05f) {
			++introCount;

			scrollUp(true);

			logoTimer = 0;
			if (introCount > 17)
				setState(CS_LOGIN);
		}
	}
}

Unique<bash_dogs::Console::Line> bash_dogs::Console::onKeyPressed(int key) {

	if (isCurrentState(CS_INTRO2)) {
		setState(CS_INTRO3);
	}
	else if (isCurrentState(CS_LOGIN)) {
		if (_edit(username, key, 5, "default_username"))
			setState(CS_CHOOSE);
	}
	else if (isCurrentState(CS_CHOOSE)) {
		if (_edit(address, key, 16, "default_address")) {

			if (level.connect(address)) {

				newLine();
				write("3. WELCOME, '" + username + "'");
				newLine();
				if (level.getServer()->isLocalHost())
					write("   PREPARE TO DEFEND THE MAINFRAME");
				else
					write("   ATTACKING `" + address + "'");

				newLine(username);

				setState(CS_NORMAL);
			}
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
		if (key == KC_RETURN) {
			if (currentCommand.hasFreeFormInput())
				setState(CS_FREEFORM_INPUT);
			else
				return make_unique<Line>(newLine(username));
		}
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
	else if (isCurrentState(CS_FREEFORM_INPUT)) {
		if (_edit(commandField, key, 10, "") && commandField.size() > 0) {
			setState(CS_NORMAL);
			return make_unique<Line>(newLine(username));
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
	if (blink > rythmDuration) {
		cursor->setVisible(!cursor->isVisible());
		blink = 0;
	}

	characterRefresh += dt;
	if (characterRefresh > 0.015f) {
		bool atLeastOneNewChar = false;
		for (auto& l : lines) {
			int totalChars = l->getContent().size();

			if (totalChars >= l->getVisibleCharacters()) {
				l->setVisibleCharacters(l->getVisibleCharacters() + 1);
				atLeastOneNewChar = true;
			}
		}

		static int idx = 0;
		++idx;
		if (atLeastOneNewChar && idx % 1 == 0)
			Platform::getSingleton()->getSoundManager()->playSound(getGameState()->getSound("coin"), 0.1f);

		characterRefresh = 0;
	}

	//make sure that invisible lines are invisible (duh)
	for (auto& l : lines)
		l->setVisible(l->canBeRendered());

	if (lines.size() > 0) {
		cursor->position.x = getLastLine().position.x + getLastLine().getSize().x * getLastLine().scale.x;
		cursor->position.y = getLastLine().position.y;
	}
}

Dojo::String bash_dogs::Console::getHelp( int n ) const {
	auto cmds = command;

	{
		for (int i = 0; i < cmds.size(); ++i) {
			auto& c = cmds[i];

			if (!c || c.helpText.empty() ) {
				cmds.erase(cmds.begin() + i);
				--i;
			}
		}
	}

	//now remove stuff until n remain
	while (cmds.size() > n)
		cmds.erase(cmds.begin() + Math::rangeRandom(0, cmds.size()));

	//finally print out help
	std::locale loc;
	String helpText = "echo={";
	for (auto& cmd : cmds)
		helpText += "\"" + cmd.command.toUpper() + ": " + cmd.helpText + "\"";

	helpText += '}';
	return helpText;
}
