#pragma once
#ifndef RESOURCEHOLDER_GUARD
#define RESOURCEHOLDER_GUARD

#include <memory>
#include <map>
#include <cassert>
#include <string>

template <typename RESOURCE, typename Identifier>
class ResourceHolder
{
public:
	void						load(Identifier id, const std::string& filename);
	template <typename Parameter>
	void						load(Identifier id, const std::string& filename, Parameter secondParam);

	RESOURCE&				get(Identifier id);
	const RESOURCE&  get(Identifier id) const;

private:
	std::map<Identifier, std::unique_ptr<RESOURCE>>	mResourceMap;
};

#include "ResourceHolder.inl"

#endif RESOURCEHOLDER_GUARD