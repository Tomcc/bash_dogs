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

		virtual void onLoop( float dt )
		{

		}
		virtual void onEnd()
		{

		}

		//----- immediate substate events
		virtual void onStateBegin()
		{

		}

		virtual void onStateLoop( float dt )
		{

		}

		virtual void onStateEnd()
		{

		}

	protected:

		Unique<Server> server;
	private:
	};
}

#endif // Gibberish_h__