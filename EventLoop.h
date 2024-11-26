#pragma once
#include"Channel.h"
#include"Dispatcher.h"

struct EventLoop
{
	struct Dispatcher* dispatcher;
	void* DispatcherData;
};