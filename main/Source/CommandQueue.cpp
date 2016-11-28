#include <CommandQueue.hpp>

/*********************************************************************************
* F�gt ein Kommando der Kommandoschlange hinzu
*
*********************************************************************************/
void CommandQueue::push(const Command& command)
{
	mQueue.push(command);
}

/*********************************************************************************
* Holt das erste Kommando aus der Kommandoschlange heraus
*
*********************************************************************************/
Command CommandQueue::pop()
{
	Command command = mQueue.front();
	mQueue.pop();

	return command;
}

/*********************************************************************************
* Pr�ft ob die Kommandoschlange leer ist
* true = leer
* false = nicht leer
*
*********************************************************************************/
bool CommandQueue::isEmpty() const
{
	return mQueue.empty();
}