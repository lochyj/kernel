#pragma once

void __DEBUG_PANIC(const char* file, int line, const char* message);

void __DEBUG_ERROR(const char* file, int line);

#define PANIC(message) do {__DEBUG_PANIC(__FILE__, __LINE__, message);} while (0)

#define ASSERT(expr) do {if (!(expr)) __DEBUG_ERROR(__FILE__, __LINE__);} while (0)