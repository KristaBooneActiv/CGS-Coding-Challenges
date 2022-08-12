#pragma once
#include <iostream>
#include <sstream>
#include <mutex>

static std::mutex COUT_LOCK;
static void WriteOutputChunk(const std::string& aOutput)
{
	std::lock_guard lock(COUT_LOCK);
	std::cout << aOutput;
}