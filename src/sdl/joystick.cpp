#include "sdl/error.h"
#include "sdl/joystick.h"

#include <SDL2/SDL_joystick.h>

namespace SDL {

Joystick::Joystick() :
	m_ptr{nullptr, SDL_JoystickClose}
{}

Joystick::Joystick(int device_index) :
	Joystick{}
{
	m_ptr.reset(SDL_JoystickOpen(device_index));

	if (!m_ptr)
		throw Error{"SDL_JoystickOpen"};
}

SDL_Joystick* Joystick::Get() const
{
    return m_ptr.get();
}

Sint16 Joystick::GetAxis(int axis) const
{
	return SDL_JoystickGetAxis(Get(), axis);
}

Uint8 Joystick::GetButton(int button) const
{
	return SDL_JoystickGetButton(Get(), button);
}

Uint8 Joystick::GetHat(int hat) const
{
	return SDL_JoystickGetHat(Get(), hat);
}

SDL_JoystickID Joystick::GetId() const
{
	return SDL_JoystickInstanceID(Get());
}

std::string Joystick::GetName() const
{
	return SDL_JoystickName(Get());
}

int Joystick::GetNumAxes() const
{
	return SDL_JoystickNumAxes(Get());
}

int Joystick::GetNumButtons() const
{
	return SDL_JoystickNumButtons(Get());
}

int Joystick::GetNumHats() const
{
	return SDL_JoystickNumHats(Get());
}

int Joystick::GetCount()
{
	return SDL_NumJoysticks();
}

std::string Joystick::GetNameForIndex(int device_index)
{
	auto name = SDL_JoystickNameForIndex(device_index);
	return name ? name : std::string{};
}

void Joystick::Update()
{
	SDL_JoystickUpdate();
}

}
