#pragma once
#include"Channel.h"
#include<stdbool.h>
struct ChannelMap {
	int size;
	struct Channel** list;
};
struct ChannelMap* ChannelMapInit(int size);
void ChannelMapClear(struct ChannelMap* map);
bool makeMapRoom(struct ChannelMap* map, int newSize, int unitSize);


