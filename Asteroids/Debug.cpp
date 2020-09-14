#include "Debug.h"

Debug debug = Debug();

DebugMessage::DebugMessage(std::string fileName, std::string functionName, std::string coderMessage)
{
	fileName = fileName;
	functionName = functionName;
	coderMessage = coderMessage;
}

Debug::Debug()
{
	showMessagesOnConsole = false;
}

void Debug::Log(std::string className, std::string functionName, std::string coderMessage)
{
	DebugMessage newMessage(className, functionName, coderMessage);

	if (showMessagesOnConsole)
		std::cout << className + ":" + functionName + " | " + coderMessage + "\n";
}