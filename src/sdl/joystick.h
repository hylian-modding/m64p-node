#pragma once

#include <memory>
#include <string>

#include <SDL2/SDL_stdinc.h>

struct _SDL_Joystick;
using SDL_Joystick = struct _SDL_Joystick;
using SDL_JoystickID = Sint32;

namespace SDL {

class Joystick {
public:
	Joystick();
	explicit Joystick(int device_index);

	SDL_Joystick* Get() const;
	Sint16 GetAxis(int axis) const;
	Uint8 GetButton(int button) const;
	Uint8 GetHat(int hat) const;
	SDL_JoystickID GetId() const;
	std::string GetName() const;
	int GetNumAxes() const;
	int GetNumButtons() const;
	int GetNumHats() const;

	static int GetCount();
	static std::string GetNameForIndex(int device_index);
	static void Update();

private:
	std::unique_ptr<SDL_Joystick, void(*)(SDL_Joystick*)> m_ptr;
};

}
