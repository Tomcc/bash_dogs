#include "stdafx.h"

#include "Console.h"

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


bash_dogs::Console::Console(Object& parent, const Vector& pos, InputDevice& keyboard) :
Object(&parent, pos),
keyboard(keyboard) {

	newLine();

	write("Welcome To HAXEOR TERMINAL 2055");
	newLine();

	write("TYPE SHIT TO HACKER");
	newLine();

	cursor = new TextArea(this, "debugFont", Vector::ZERO);
	cursor->addText("_");
	addChild(cursor, (int)Layers::LL_CONSOLE);

	keyboard.addListener(this);

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
	command[KC_Q] = Command("quantize", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_R] = Command("rtfm", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_S] = Command("sort", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_T] = Command("tar-lz", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_U] = Command("unlink", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_V] = Command("view", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_W] = Command("wget", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_X] = Command("x-conf", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_Y] = Command("yzf", { { KC_F, "foo" }, { KC_B, "bar" } });
	command[KC_Z] = Command("zip", { { KC_F, "foo" }, { KC_B, "bar" } });
}

void bash_dogs::Console::write(const String& s) {
	getLastLine().addText(s);
}

String bash_dogs::Console::newLine() {
	const Vector interline(0, -0.4);

	String res;
	if (lines.size() > 20) {
		res = getLastLine().getContent();

		if (getLastLine().position.y < -9) {
 			lines.front()->dispose = true;
 			lines.erase(lines.begin());

			for (auto l : lines)
				l->position -= interline;
		}
	}

	currentCommand = Command();

	//create a new one
	TextArea* line = new TextArea(this, "debugFont", interline * lines.size(), false, getParent()->getSize());
	addChild(line, (int)Layers::LL_CONSOLE);
	line->addText("> ");
	line->color = Color::GREEN;

	lines.push_back(line);

	return res;
}

void bash_dogs::Console::onAction(float dt) {
	Object::onAction(dt);

	blink += dt;
	if (blink > 0.5f) {
		cursor->setVisible(!cursor->isVisible());
		blink = 0;
	}

	cursor->position.x = getLastLine().position.x + getLastLine().getSize().x * getLastLine().scale.x;
	cursor->position.y = getLastLine().position.y;
}

const Dojo::String& bash_dogs::Console::_getCommandForKey(int key) const {

	if (command.empty()) {
		switch (key)
		{
		
		default:
			return String::EMPTY;
		}
	}
	else {
		return String::EMPTY;
	}

}

void bash_dogs::Console::onButtonPressed(Dojo::InputDevice* j, int action) {

	if (action == KC_RETURN)
		newLine();
	else if( !currentCommand ) {

		if (action == KC_TAB) {
			while (!currentCommand)
				currentCommand = oneOf(command);
		}
		else
			currentCommand = command[action];
		
		if (currentCommand)
			write( currentCommand.command + " " );
	}
	else {
		String param = currentCommand.getParam(action);
		if (!param.empty())
			write("--" + param + " ");
	}
}

bash_dogs::Console::~Console() {
	keyboard.removeListener(this);
}
