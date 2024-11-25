#include "ChannelMap.h"
#include<stdlib.h>
#include<string.h>
struct ChannelMap* ChannelMapInit(int size)
{
	ChannelMap* map = (ChannelMap*)malloc(sizeof(struct ChannelMap));
	map->size = size;
	map->list = (struct Channel**)malloc(size * sizeof(struct ChannelMap*));
	return map;
}

void ChannelMapClear(ChannelMap* map)
{
	if (map != NULL) {
		for (int i = 0;i < map->size;i++) {
			if (map->list[i] != NULL) {
				free(map->list[i]);
			}
		}
		free(map->list);
		map->list = NULL;
	}
	map->size = 0;
}

bool makeMapRoom(ChannelMap* map, int newSize, int unitSize)
{
	if (map->size < newSize) {
		int curSize = map->size;
		while (curSize < newSize) {
			curSize *= 2;
		}
		struct Channel** curChannel = (Channel**)realloc(map->list, curSize * unitSize);
		if (curChannel == NULL) {
			return false;
		}
		map->list = curChannel;
		memset(&map->list[map->size],0,(curSize-map->size)*unitSize);
		map->size = curSize;
	}
	return true;
}
