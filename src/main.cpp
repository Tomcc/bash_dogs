#include "stdafx.h"

#include "BashDogsGame.h"

using namespace Dojo;
using namespace bash_dogs;

int main( int argc, char** argv )
{
	Table config;

	Platform* platform = Platform::create( config );

	platform->run( new BashDogsGame() );

	return 0;
}

#ifdef WIN32

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
{
	main( 1, &strCmdLine );
}

#endif