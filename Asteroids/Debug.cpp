#include "Debug.h"

#include <iostream>

Debug debug = Debug();

// Constructor.
DebugMessage::DebugMessage(std::string fileName, std::string functionName, std::string coderMessage)
{
	fileName = fileName;
	functionName = functionName;
	coderMessage = coderMessage;
}

// Constructor.
Debug::Debug()
{
	showMessagesOnConsole = false;
}

// Log a message and attempt to print it to console if allowed.
void Debug::Log(std::string className, std::string functionName, std::string coderMessage)
{
	DebugMessage newMessage(className, functionName, coderMessage);

	if (showMessagesOnConsole)
		std::cout << className + ":" + functionName + " | " + coderMessage + "\n";
}