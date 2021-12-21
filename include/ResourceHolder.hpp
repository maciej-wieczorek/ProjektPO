#pragma once

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <class Resource, class Identifier>
class ResourceHolder
{
public:
	void load(Identifier id, const std::string& filename);

	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;

private:
	std::map<Identifier, std::unique_ptr<Resource> > mResources;
	void insertResource(Identifier id, std::unique_ptr<Resource> resource);
};

#include "ResourceHolder.inl"
