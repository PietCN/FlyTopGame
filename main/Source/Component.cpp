#include <Component.hpp>

namespace GUI
{
	Component::Component()
		: mIsSelected(false)
		, mIsActived(false)
	{

	}
	Component::~Component()
	{

	}

	bool Component::isSelected() const
	{
		return mIsSelected;
	}
	void Component::select()
	{
		mIsSelected = true;
	}
	void Component::deselect()
	{
		mIsSelected = false;
	}
	bool Component::isActive() const
	{
		return mIsActived;
	}
	void Component::activate()
	{
		mIsActived = true;
	}
	void Component::deactivate()
	{
		mIsActived = false;
	}

}