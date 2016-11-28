/*********************************************************************************
* Author				Patrick Gehrt
* last Update		08.11.2016
* last Step			p.188 Chapter 8
* description
*		Button.cpp/hpp anpassen / Buttontexturen
*
* Version 0.1 -> 0.2
*   
*		
*
*********************************************************************************/
#include <SFML\Graphics.hpp>
#include <iostream>

#include <Application.hpp>

int main()
{
	try
	{
		Application app;

		app.run();
	}
	catch (std::exception e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		std::cin.get();
	}

	return EXIT_SUCCESS;
}