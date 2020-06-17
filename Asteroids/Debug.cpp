#include "Debug.h"

Debug debug = Debug();

DebugMessage::DebugMessage(std::string fileName, std::string functionName, std::string coderMessage)
{
	this->fileName = fileName;
	this->functionName = functionName;
	this->coderMessage = coderMessage;
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