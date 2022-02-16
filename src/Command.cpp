#include "Command.hpp"

#include <functional>

Command::Command() :
	action{},
	category{static_cast<unsigned int>(Category::None)}
{

}