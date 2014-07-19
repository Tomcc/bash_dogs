
#ifndef Level_h__
#define Level_h__
        
#include "common_header.h"
#include "Server.h"

namespace bash_dogs {

	class BashDogsGame;
	class Console;
	class FileSystem;

    class Level :
	public Dojo::GameState,
	public Dojo::InputDevice::Listener,
	public Dojo::InputSystem::Listener,
	public Dojo::TouchArea::Listener
    {
    public:

        Level( BashDogsGame* game );

		virtual ~Level();

		bool connect(const String& address);

		virtual void onDeviceConnected(Dojo::InputDevice* j) override;
		void onDeviceDisconnected(Dojo::InputDevice* j) override;

		virtual void onButtonPressed(Dojo::InputDevice* j, int action) override;

		void onButtonReleased(Dojo::InputDevice* j, int action) override;

		Server* getServer() {
			return server.get();
		}

		Console& getConsole() const {
			return *console;
		}

    protected:

		Unique<Server> server;

		FileSystem* fileSystem;

		Console* console;
		float distortion = 2.f;
		
        virtual void onBegin();
        virtual void onEnd();
        
        //----- immediate substate events
        virtual void onStateBegin();
        
        virtual void onStateLoop( float dt );
        
        virtual void onStateEnd();

    private:
    };
}   
#endif // Level_h__