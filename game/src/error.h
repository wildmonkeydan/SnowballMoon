#pragma once

typedef enum ErrorType {
	ERROR_MISSING,
	ERROR_LOADING,
	ERROR_GAMELOGIC
}ErrorType;

void ErrorTrap(ErrorType type, const char* message);