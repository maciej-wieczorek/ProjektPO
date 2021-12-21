#pragma once
template<class Resource, class Identifier>
inline void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	std::unique_ptr<Resource> resource(new Resource);
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	// move ownership of resource to map
	insertResource(id, std::move(resource));
}

template<class Resource, class Identifier>
inline Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = mResources.find(id);
	// handle debug level exception
	assert(found != mResources.end());

	return *found->second;
}

template<class Resource, class Identifier>
inline const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResources.find(id);
	// handle debug level exception
	assert(found != mResources.end());

	return *found->second;
}

template<class Resource, class Identifier>
inline void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
	// insert resource into map
	auto checkInsert = mResources.insert(std::make_pair(id, std::move(resource)));
	assert(checkInsert.second);
}
