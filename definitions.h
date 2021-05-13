#ifndef ERRORS_H
#define ERRORS_H

//errors
const std::string INVALID_COMMAND = "Command not found.";
const std::string INVALID_ARGUMENT = "Invalid argument or command ";
const std::string EXISTING_TYPE = "This type already exists.";
const std::string EXISTING_KEY = "Redefinition of key name.";
const std::string WRONG_AMOUNT_OF_ARGS = "Wrong amount of arguments for command ";
const std::string NON_EXISTING_TYPE = "This type doesn't exist.";
const std::string NON_EXISTING_KEY = " key doesn't exist for this type.";


//commands
const std::string NEW_TYPE_COMMAND = "construct";
const std::string EXIT_COMMAND = "exit";
const std::string CLEAR_COMMAND = "clear";
const std::string NEW_TYPE_CONSTRUCT_COMMAND = "build";
const std::string GET_WHERE_COMMAND = "getWhere";
const std::string UPDATE_COMMAND = "modify";
const std::string DELETE_COMMAND = "delete";
const std::string REMOVE_COMMAND = "remove";

//other
const std::string SEPARATOR = "_______________\n";

#endif