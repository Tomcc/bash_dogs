
#ifndef Level_h__
#define Level_h__
        
#include "common_header.h"

namespace bash_dogs {

	class BashDogsGame;
	class Console;

    class Level :
	public Dojo::GameState,
	public Dojo::InputDevice::Listener,
	public Dojo::InputSystem::Listener,
	public Dojo::TouchArea::Listener
    {
    public:

        Level( BashDogsGame* game );

		virtual ~Level();

		virtual void onDeviceConnected(Dojo::InputDevice* j) override;
		void onDeviceDisconnected(Dojo::InputDevice* j) override;

		void onButtonReleased(Dojo::InputDevice* j, int action) override;

    protected:

		Console* console;
		
        virtual void onBegin();
        virtual void onEnd();
        
        //----- immediate substate events
        virtual void onStateBegin();
        
        virtual void onStateLoop( float dt );
        
        virtual void onStateEnd();

		void _makeWorld();
    private:
    };
}   
#endif // Level_h__