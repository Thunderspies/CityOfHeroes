#include "entScript.h"
#include "MessageHandler.h"
#include <assert.h>
#include <string.h>
#include "utils.h"
#include "SimpleParser.h"
#include "cmdserver.h"


static int MessageDeclHandler(ParseContext context, char* params);
static int ClassDeclHandler(ParseContext context, char* params);
static int ClassExtendsHandler(ParseContext context, char* params);
static int ClassStateDeclHandler(ParseContext context, char* params);
static int ClassHandler(ParseContext context, char* params);
static int ClassStateHandler(ParseContext context, char* params);
static int ClassStateOnHandler(ParseContext context, char* params);
static int ClassStateOnFlowchartHandler(ParseContext context, char* params);
static int ClassStateOnContentHandler(ParseContext context, char* params);
static int ClassStateOnExit(ParseContext context, char* params);
static int ClassEndHandler(ParseContext context, char* params);

/*************************************************************
* Entity AI script parse rules
*/
#pragma warning(push)
#pragma warning(disable:4047) // return type differs in lvl of indirection warning

PCommandWord MessageDeclContents[] = {
	{"Message", {{EXECUTE_HANDLER, MessageDeclHandler}}},
	{"EndMessageDeclaration", {{POP_COMMANDWORDS}}},
	{0}
};

PCommandWord ClassDeclContents[] = {
	{"Class", {{EXECUTE_HANDLER, ClassDeclHandler}}},
	{"EndClassDeclaration", {{POP_COMMANDWORDS}}},
	{0}
};

PCommandWord ClassStateDeclContents[] = {
	{"State", {{EXECUTE_HANDLER, ClassStateDeclHandler}}},
	{"EndStateDeclaration", {{POP_COMMANDWORDS}}},
	{0}
};		

PCommandWord ClassStateOnContents[] = {
	{"Flowchart", {EXECUTE_HANDLER, ClassStateOnFlowchartHandler}},
	{"EndOn", {{POP_COMMANDWORDS}, {EXECUTE_HANDLER, ClassStateOnExit}, {STORE_STRUCTURE}/* pop manually pushed message handler structure */}},
	{"*", {{EXECUTE_HANDLER, ClassStateOnContentHandler}}},
	{0}
};

PCommandWord ClassStateContents[] = {
	{"On",{{PUSH_COMMANDWORDS, ClassStateOnContents}, {EXECUTE_HANDLER, ClassStateOnHandler}}},
	{"EndState", {{POP_COMMANDWORDS}, {STORE_STRUCTURE} /* pop manually pushed class state structure*/}},
	{0}
};

PCommandWord ClassContents[] = {
	{"Extends", {{EXECUTE_HANDLER, ClassExtendsHandler}}},
	{"StateDeclaration", {{PUSH_COMMANDWORDS, ClassStateDeclContents}}},
	{"State", {{PUSH_COMMANDWORDS, ClassStateContents}, {EXECUTE_HANDLER, ClassStateHandler}}},
	{"EndClass", {{POP_COMMANDWORDS}, {EXECUTE_HANDLER, ClassEndHandler}, {STORE_STRUCTURE} /* pop manually pushed class def structure*/}},
};

PCommandWord MetaSpawnAreaContents[] = {
	{"MessageDeclaration", {{PUSH_COMMANDWORDS, &MessageDeclContents}}},
	{"ClassDeclaration", {{PUSH_COMMANDWORDS, &ClassDeclContents}}},
	{"Class", {{PUSH_COMMANDWORDS, &ClassContents}, {EXECUTE_HANDLER, ClassHandler}}},
	{"EndMetaSpawnArea", {{POP_COMMANDWORDS}}},
	{0}
};

PCommandWord MetaSpawnAreaStart[] = {
	{"MetaSpawnArea", {{PUSH_COMMANDWORDS, MetaSpawnAreaContents}}},
	{0}
};
#pragma warning(pop)
/*
 * Entity AI script parse rules
 *************************************************************/


/*************************************************************
 * Entity AI script parse handlers
 */
// Called for each message declaration in a MessageDecl bock
static int MessageDeclHandler(ParseContext context, char* params){
	static unsigned int newMessageID = 0;
	MessageType* type;
	
	// Check if the message name already exists
	// If so, output error message and stop further processing.
	if(hashFindValue(MessageTypeNameHash, params)){
		printf("Message %s has already been declared\n", params);
		return 0;
	}
	
	// Otherwise, create a new message type and fill in all the info
	type = mpAlloc(MessageTypeMemoryPool);
	type->name = (char*)strTableAddString(NameStringTable, params);
	type->ID = newMessageID;
	
	// Assign a new message ID to the next declared message
	newMessageID++;
	
	hashAddElement(MessageTypeNameHash, params, type);
	return 0;
}

static int ClassDeclHandler(ParseContext context, char* params){
	ClassDef* type;
	char* className;
	int result = 1;
	
	// Extract class name from passed parameter
	beginParse(params);
	result &= getString(&className);
	endParse();
	assert(result);
	
	// Check if the class name already exists
	// If so, output error message and stop further processing.
	if(hashFindValue(ClassDefNameHash, className)){
		printf("Class %s has already been declared\n", className);
		return 0;
	}
	
	// Otherwise, create a new message type and fill in all the info
	type = mpAlloc(ClassDefMemoryPool);
	type->name = (char*)strTableAddString(NameStringTable, className);

	hashAddElement(ClassDefNameHash, className, type);
	return 0;
}

static int ClassStateDeclHandler(ParseContext context, char* params){
	State* stateType;
	ClassDef* classType = parseGetCurrentStructure(context);
	HashTable stateMap = classType->stateMap;
	
	// Check if the state name already exists
	// If so, output error message and stop further processing.
	if(hashFindValue(stateMap, params)){
		printf("State %s has already been declared for Class %s\n", params, classType->name);
		return 0;
	}
	
	// Otherwise, create a new message type and fill in all the info
	stateType = mpAlloc(StateMemoryPool);
	stateType->name = (char*)strTableAddString(NameStringTable, params);
	
	// Add the state to the map
	hashAddElement(stateMap, stateType->name, stateType);
	return 0;
}

/* Function ClassHandler
 *	A class definition block has been found.  The parser has been configured to update
 *	the command words that it uses to parse the block.  This function now needs to place
 *	the correct empty class definition on top of the parser structure stack so the
 *	rest of the parser can simple access the stack top to get the structure.
 *
 *
 */
static int ClassHandler(ParseContext context, char* params){
	ClassDef* classDef;
	char* className;
	int result = 1;
	
	// Extract class name from passed parameter
	beginParse(params);
	result &= getString(&className);
	endParse();
	assert(result);

	classDef = hashFindValue(ClassDefNameHash, className);

	if(!classDef){
		printf("Definition for unknown Class named \"%s\" on line %i\n", params, parseGetLineno(context));  
		assert(0);  // What if the class def is simple created if not found?
		return 0;
	}

	if(classDef->parentClass || classDef->stateMap){
		printf("Class %s redefinition found on line\n", params, parseGetLineno(context));
		assert(0);
		return 0;
	}

	classDef->stateMap = createHashTable();
	initHashTable(classDef->stateMap, 10);

	parsePushStructure(context, classDef);
	return 0;
}

static int ClassExtendsHandler(ParseContext context, char* params){
	ClassDef* parentClass = hashFindValue(ClassDefNameHash, params);

	if(!parentClass){
		printf("Unknown parent Class name \"%s\" on line %i\n", params, parseGetLineno(context));
		return 0;
	}else{
		ClassDef* type = parseGetCurrentStructure(context);
		type->parentClass = parentClass;
		return 0;
	}
}

static int ClassStateHandler(ParseContext context, char* params){
	ClassDef* classType;
	State* stateType;

	classType = parseGetCurrentStructure(context);
	stateType = hashFindValue(classType->stateMap, params);

	if(!stateType){
		printf("Unknown State %s found in Class %s\n", params, classType->name);
		assert(0);
		return 0;
	}

	parsePushStructure(context, stateType);
	return 0;
}

static int ClassStateOnHandler(ParseContext context, char* messageName){
	State* stateType;
	MessageType* messageType;
	MessageHandler* messageHandler;

	stateType = parseGetCurrentStructure(context);
	messageType = hashFindValue(MessageTypeNameHash, messageName);

	if(!messageType){
		printf("Unknown message %s on line %i\n", messageName, parseGetLineno(context));
		assert(0);
		return 0;
	}
	
	messageHandler = mpAlloc(MessageHandlerMemoryPool);
	messageHandler->messageType = messageType;
	parsePushStructure(context, messageHandler);

	if(!stateType->msgHandlerMap){
		stateType->msgHandlerMap = createHashTable();
		initHashTable(stateType->msgHandlerMap, 10);
	}

	hashAddElement(stateType->msgHandlerMap, messageName, messageHandler);
	return 0;	
}

static int ClassStateOnFlowchartHandler(ParseContext context, char* params){
	MessageHandler* messageHandler;

	messageHandler = parseGetCurrentStructure(context);
	messageHandler->flowchart = (char*)strTableAddString(NameStringTable, params);
	return 0;
}



HashTable InstructionNameHash;
Instruction* LastInstruction;
Array ParseStack;

static int InstructionNeedsParam(ParseContext context, int ParseStackIdx){
	Instruction* inst;
	char* instName;

	inst = ParseStack.storage[ParseStackIdx];
	instName = inst->instDef->instructionName;

	if(':' == instName[strlen(instName) - 1])
		return 1;
	else
		return 0;
}

static int LastInstructionNeedsParam(){
	Instruction* inst;
	char* instName;

	// Try to get the last instruction in the parse stack
	inst = arrayGetBack(&ParseStack);

	// Is there an instruction at all?
	if(!inst)
		return 0;

	// Is the "instruction" a special token?
	if(*((int*)inst) == '\0[')
		return 0;

	// Does it already have a parameter attached?
	if(inst->param)
		return 0;

	instName = inst->instDef->instructionName;

	if(':' == instName[strlen(instName) - 1])
		return 1;
	else
		return 0;
}

static void reduceParseStack(){
	Instruction* nextInstruction;
	Instruction* prevInstruction;

	nextInstruction = arrayPopBack(&ParseStack);
	prevInstruction = arrayGetBack(&ParseStack);

	prevInstruction->next = nextInstruction;
}

static int ClassStateOnContentHandler(ParseContext context, char* params){
	char* token;
	char foundDelim;

	// Get the original line
	char* line = parseGetLine(context);
	

	if(hashGetSize(InstructionNameHash) == 0){
		InstructionDef* cursor = InstructionTable;

		// Insert all elements until the terminating entry with an invalid name is found.
		for(cursor = InstructionTable; cursor->instructionName; cursor++)
			hashAddElement(InstructionNameHash, cursor->instructionName, cursor);
	}

	if(ParseStack.maxSize == 0){
		initArray(&ParseStack, 256);
	}
	
	
	// Keep gettings tokens until the string is exhausted.
	while(token = strsep2(&line, " []", &foundDelim)){
		Instruction* inst;
		InstructionDef* instDef;
		
		token = removeLeadingWhiteSpaces(token);
		removeTrailingWhiteSpaces(&token);
		
		// Found beginning of block?
		if('[' == foundDelim){
			arrayPushBack(&ParseStack, "[");
			continue;
		}
		
		if('\0' == token[0])
			goto CheckEndOfBlock;

		// Found a parameter?
		if(LastInstructionNeedsParam()){
			Instruction* lastInst = arrayGetBack(&ParseStack);
			lastInst->param = strdup(token);
			goto CheckEndOfBlock;
		}
		
		// The token must be a instruction name
		// Locate the instruction definition
		instDef = hashFindValue(InstructionNameHash, token);
		
		// Invalid instruction name?
		if(!instDef){
			if('\0' == *token)
				goto CheckEndOfBlock;
			printf("Unknow script keyword \"%s\" found on line %i\n", token, parseGetLineno(context));
			assert(0);
			endParse();
			return 0;
		}
		
		// Create an instruction to hold information to be extracted.
		inst = mpAlloc(InstructionMemoryPool);
		assert(inst);
		
		// Fill in other fields of this instruction
		inst->instDef = instDef;
		LastInstruction = inst;
		arrayPushBack(&ParseStack, inst);
		
CheckEndOfBlock:
		// Was the end of a block found?
		if(']' == foundDelim){
			Instruction* childInstruction;
			Instruction* parentInstruction;
			
			// If so, connect/reduce all instruction until the last two items
			// on the parse stack is a '[' and an instruction
			while(*((int*)ParseStack.storage[ParseStack.size - 2]) != '\0[')
				reduceParseStack();
			
			// Grab the first instruction in the block
			childInstruction = arrayPopBack(&ParseStack);
			
			// Pop the special '[', block open token
			arrayPopBack(&ParseStack);
			
			// Store the first instruction in the block as the parent instruction's
			// parameter.  This is only logical if the parent instruction is some
			// sort of execution path control instruction.
			parentInstruction = arrayGetBack(&ParseStack);
			parentInstruction->param = (char*)childInstruction;
		}
	}
	
	return 0;
}

static int ClassStateOnExit(ParseContext context, char* params){
	MessageHandler* handler = parseGetCurrentStructure(context);
	LastInstruction = NULL;
	
	// Continuously connect all instructions in the stack together until only
	// one is left.
	while(ParseStack.size > 1)
		reduceParseStack();
	
	// Store the first instruction with the message handler.
	handler->inst = arrayPopBack(&ParseStack);
	return 0;
}

static int ClassEndHandler(ParseContext context, char* params){
	ClassDef* classDef = parseGetCurrentStructure(context);
	
	classDef->defaultHandlerState = hashFindValue(classDef->stateMap, "DefaultHandler");
	
	return 0;
}

/*
 * Entity AI script parse handlers
 *************************************************************/

void initAIScriptParser(){
	if(!ClassDefMemoryPool){
		ClassDefMemoryPool = createMemoryPool();
		initMemoryPool(ClassDefMemoryPool, sizeof(ClassDef), MAX_CLASS_DEFS);
	}else
		mpFreeAll(ClassDefMemoryPool);
	
	if(!MessageTypeMemoryPool){
		MessageTypeMemoryPool = createMemoryPool();
		initMemoryPool(MessageTypeMemoryPool, sizeof(MessageType), MAX_MESSAGE_TYPES);
	}else
		mpFreeAll(MessageTypeMemoryPool);
	
	if(!StateMemoryPool){
		StateMemoryPool = createMemoryPool();
		initMemoryPool(StateMemoryPool, sizeof(State), MAX_STATES);
	}else
		mpFreeAll(StateMemoryPool);
	
	if(!MessageHandlerMemoryPool){
		MessageHandlerMemoryPool = createMemoryPool();
		initMemoryPool(MessageHandlerMemoryPool, sizeof(MessageHandler), MAX_MESSAGE_HANDLERS);
	}else
		mpFreeAll(MessageHandlerMemoryPool);
	
	if(!MessageReceiverMemoryPool){
		MessageReceiverMemoryPool = createMemoryPool();
		initMemoryPool(MessageReceiverMemoryPool, sizeof(MessageReceiver), MAX_MESSAGE_RECEIVERS);
	}else
		mpFreeAll(MessageReceiverMemoryPool);
	
	if(!InstructionMemoryPool){
		InstructionMemoryPool = createMemoryPool();
		initMemoryPool(InstructionMemoryPool, sizeof(Instruction), MAX_INSTRUCTIONS);
	}else
		mpFreeAll(InstructionMemoryPool);

	if(!ClassDefNameHash){
		ClassDefNameHash = createHashTable();
		hashSetMode(ClassDefNameHash, CaseInsensitive);
		initHashTable(ClassDefNameHash, 100);
	}else
		clearHashTable(ClassDefNameHash);
	
	if(!MessageTypeNameHash){
		MessageTypeNameHash = createHashTable();
		initHashTable(MessageTypeNameHash, 100);
	}else
		clearHashTable(MessageTypeNameHash);
	
	if(!InstructionNameHash){
		InstructionNameHash = createHashTable();
		initHashTable(InstructionNameHash, 128);
	}else
		clearHashTable(InstructionNameHash);
	
	if(!NameStringTable){
		NameStringTable = createStringTable();
		initStringTable(NameStringTable, 1024);
	}else
		strTableClear(NameStringTable);
	
	mtGridFreeAll();	
}

void AIScriptLoad(int reload){
	FILE* file;
	char *aiScript = "AIScript/Globals.txt";
	static unsigned char loaded = 0;
	
	if(server_state.aiScript)
		aiScript = server_state.aiScript;

	if(!loaded || reload){
		// Make sure all parser related variables are initialized.
		initAIScriptParser();
		
		//	Check if the global spawn area file is present
		file = fileOpen(aiScript, "r");
		if(NULL == file){
			printf("Could not find Global SpawnArea file...\n");
			printf("SpawnArea file expected: %s\n", aiScript);
		}else{
			fclose(file);
			
			// Process the global file using SpawnArea command words.

			parseCommandFile(aiScript, MetaSpawnAreaStart);
		}

		loaded = 1;
	}
}

