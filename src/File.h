#pragma once

#include "common_header.h"

namespace bash_dogs {
	class FileSystem;

	class File : public Renderable
	{
	public:
		enum State {
			FS_UNKNOWN,
			FS_LOCKED,
			FS_OPEN
		};

		enum Type {
			T_FILE,
			T_FOLDER
		};

		const Type type;

		File(FileSystem& parent, const Vector&pos, const String& name, Type type, State initialState);

		File(FileSystem& parent, const Vector& pos, Unique<Table> desc);

		virtual void onAction(float dt);

		Unique<Table> serialize() const;

	protected:
		TextArea* label;

		const String name;
		State state;

		static Mesh* _meshForState(ResourceGroup& rg, State state, Type type);
	private:
	};
}



