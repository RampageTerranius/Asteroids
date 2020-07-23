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

void Debug::Log(std::string fileName, std::string functionName, std::string coderMessage)
{
	DebugMessage newMessage(fileName, functionName, coderMessage);

	if (showMessagesOnConsole)
		std::cout << fileName + ":" + functionName + " | " + coderMessage + "\n";
}