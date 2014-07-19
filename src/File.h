#pragma once

#include "common_header.h"

namespace bash_dogs {
	class FileSystem;

	class File : public Renderable
	{
	public:
		enum State {
			LL_UNKNOWN,
			LL_LOCKED,
			LL_FOLDER,
			LL_FILE,
		};

		File(FileSystem& parent, const Vector&pos, const String& name, State initialState);

		virtual void onAction(float dt);

	protected:
		TextArea* label;

		State state;

		static Mesh* _meshForState(ResourceGroup& rg, State state);
	private:
	};
}



