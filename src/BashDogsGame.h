#ifndef FrontierWorlds_h__
#define FrontierWorlds_h__

#include "common_header.h"

#include "Level.h"

namespace bash_dogs
{
	class BashDogsGame : 
		public Dojo::Game,
		public Dojo::ResourceGroup
	{
	public:

		BashDogsGame();

		void reset();


	protected:

		virtual void onBegin();
	};
}

#endif // Gibberish_h__