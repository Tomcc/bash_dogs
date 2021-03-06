
#ifndef common_header_h__
#define common_header_h__

#include <dojo.h>

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <mutex>

#include <Poco/Net/StreamSocket.h>

#define PORT 12300

using namespace Dojo;

#define DLLX __declspec(dllimport)

//some STL glue
template< typename T >
using Unique = std::unique_ptr < T > ;

template<typename T>
using Shared = std::shared_ptr < T > ;

template<typename T>
using Weak = std::weak_ptr < T > ;

typedef std::lock_guard<std::mutex> LockGuard;

//C++14 where not available
#ifndef WIN32
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&& ...args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#else
using std::make_unique;
#endif
using std::make_shared;

extern float rythmDuration;

template<typename T, typename M>
std::unique_ptr<T>&& unique_cast(std::unique_ptr<M>& in) {
	return std::unique_ptr<T>((T*)in.release());
}

namespace bash_dogs {

	enum Keys {
		KEY_JUMP = 999
	};


	enum Layers
	{
		LL_BACKGROUND,

		LL_GRAPH_WIREFRAME, 
		LL_GRAPH,


		LL_CONSOLE,

		LL_LIGHT,

		LL_WIREFRAME,

		LL_FADER,

		LL_DEBUG,

		LL_MAIN_WINDOW
	};
}


typedef uint_fast8_t TerraiMaterialID;
typedef int_fast8_t Layer;
typedef int_fast16_t Height;

#define MAX_TERRAIN_MATERIAL_ID UINT_FAST8_MAX
#define MAX_HEIGHT INT_FAST16_MAX

using namespace Dojo;

#endif // common_header_h__