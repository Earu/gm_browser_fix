#include <GarrysMod/Lua/Interface.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#if __unix__
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef __APPLE__
#define OPEN_COMMAND "open"
#elif defined(__linux__)
#define OPEN_COMMAND "xdg-open"
#endif

LUA_FUNCTION(open_url)
{
	const bool has_url = LUA->Top() > 0;
	if (!has_url) return 0;

	const char* input = LUA->CheckString(1);
	std::string url(input);

	// fix protocol if it isnt there
	if (!(url.find("http://") == 0 || url.find("https://") == 0))
		url = "https://" + url;

// windows
#ifdef _WIN32
	ShellExecute(0, 0, url.c_str(), 0, 0, SW_SHOW);
#endif

// linux & macos
#if defined(__APPLE__) || defined(__linux__)
	pid_t pid = fork();
	// Are we the child?
	if (pid == 0) {
		execlp(OPEN_COMMAND, OPEN_COMMAND, url.c_str(), NULL);
		exit(1);
	}
#endif

	return 0;
}

GMOD_MODULE_OPEN() 
{
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->GetField(-1, "gui");
	LUA->PushCFunction(open_url);
	LUA->SetField(-2, "OpenURL");
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}
