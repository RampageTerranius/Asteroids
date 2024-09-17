#pragma once
#include <string>
#include <vector>

// Debug classes used for showing messages in console.
// A single debug message.
class DebugMessage
{
private:
	std::string className;
	std::string functionName;
	std::string coderMessage;
	std::string programMessage;

public:
	DebugMessage(std::string fileName, std::string functionName, std::string coderMessage);
	std::string ClassName() { return className; }
	std::string FunctionName() { return functionName; }
	std::string CoderMessage() { return coderMessage; }
	std::string ProgramMessage() { return programMessage; }
};

// A class with a list storing and handling all debug messages.
class Debug
{
public:
	Debug();
	bool showMessagesOnConsole;
	void Log(std::string className, std::string functionName, std::string coderMessage);

private:
	std::vector<DebugMessage> msg;
};

extern Debug debug;