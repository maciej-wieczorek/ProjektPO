#include "Pickup.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "Category.hpp"

namespace
{
    const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures) :
    Entity{ 1 },
    mType{ type },
    mSprite{ textures.get(Table[static_cast<unsigned int>(type)].texture),
             Table[static_cast<unsigned int>(type)].textureRect }
{
    centerOrigin(mSprite);
}

unsigned int Pickup::getCategory() const
{
    return static_cast<unsigned int>(Category::Pickup);
}

sf::FloatRect Pickup::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Character& player) const
{
    Table[static_cast<unsigned int>(mType)].action(player);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
