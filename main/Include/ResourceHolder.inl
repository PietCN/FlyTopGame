/*********************************************************************************
* Versucht eine Resource zu einer spezifischen ID zu laden
*
*********************************************************************************/
template <typename RESOURCE, typename Identifier>
void ResourceHolder<RESOURCE, Identifier>::load(Identifier id, const std::string& filename)
{
	std::unique_ptr<RESOURCE> resource(new RESOURCE);

	if (!resource->loadFromFile(filename))
		throw (std::runtime_error("Resource::Load - Failed to load " + filename));

	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	//exception duble id
	assert(inserted.second);
}

/*********************************************************************************
* Versucht eine Resource zu einer spezifischen ID zu laden
* loadFromFile wird mit zwei Parametern bedient
*********************************************************************************/
template <typename RESOURCE, typename Identifier>
template <typename Parameter>
void ResourceHolder<RESOURCE, Identifier>::load(Identifier id, const std::string& filename, Parameter secondParam)
{
	std::unique_ptr<RESOURCE> resource(new RESOURCE);

	if (!resource->loadFromFile(filename, secondParam))
		throw (std::runtime_error("Resource::Load - Failed to load " + filename));

	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	//exception duble id
	assert(inserted.second)
}

/*********************************************************************************
* gibt eine RESOURCE Referenz zur gesuchten ID zurück
*
*********************************************************************************/
template <typename RESOURCE, typename Identifier>
RESOURCE& ResourceHolder<RESOURCE, Identifier>::get(Identifier id)
{
	auto found = mResourceMap.find(id);
	//id not found
	assert(found != mResourceMap.end());

	return *found->second;
}

/*********************************************************************************
* gibt eine const RESOURCE Referenz zur gesuchten ID zurück
*
*********************************************************************************/
template <typename RESOURCE, typename Identifier>
const RESOURCE& ResourceHolder<RESOURCE, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	//id not found
	assert(found != mResourceMap.end());

	return *found->second;
}