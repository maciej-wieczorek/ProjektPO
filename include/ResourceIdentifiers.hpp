#pragma once

enum class Fonts
{
	Main
};

enum class Textures
{
	Floor, Button
};

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Font, Fonts> FontHolder;
typedef ResourceHolder<sf::Texture, Textures> TextureHolder;