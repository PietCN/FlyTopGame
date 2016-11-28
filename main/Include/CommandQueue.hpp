#pragma once
#ifndef COMMANDQUEUE_GUARD
#define COMMANDQUEUE_GUARD

#include <queue>
#include <Command.hpp>

class CommandQueue
{
public:
	void								push(const Command& command);
	Command							pop();
	bool								isEmpty() const;

private:
	std::queue<Command>	mQueue;
};

#endif COMMANDQUEUE_GUARD