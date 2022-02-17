#pragma once

// forward declarations
namespace sf
{
	class Font;
	class Texture;
}
template <typename Resource, typename Identifier>
class ResourceHolder;
//

enum class Fonts
{
	Main
};

enum class Textures
{
	Entities, Floor, Grass, Button, Bullet, Particle
};

typedef ResourceHolder<sf::Font, Fonts> FontHolder;
typedef ResourceHolder<sf::Texture, Textures> TextureHolder;