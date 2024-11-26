#include "Dispatcher.h"
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>

#define Max 1024
struct SelectData
{
    fd_set readSet;
    fd_set writeSet;
};

static void* selectInit();
static int selectAdd( struct EventLoop* evLoop,struct Channel* channel);
static int selectRemove( struct EventLoop* evLoop,struct Channel* channel);
static int selectModify( struct EventLoop* evLoop,struct Channel* channel);
static int selectDispatch(struct EventLoop* evLoop, int timeout); // ��λ: s
static int selectClear(struct EventLoop* evLoop);
static void setFdSet(struct Channel* channel, struct SelectData* data);
static void clearFdSet(struct Channel* channel, struct SelectData* data);

struct Dispatcher SelectDispatcher = {
    selectInit,
    selectAdd,
    selectRemove,
    selectModify,
    selectDispatch,
    selectClear
};

static void* selectInit()
{
    struct SelectData* data = (struct SelectData*)malloc(sizeof(struct SelectData));
    FD_ZERO(&data->readSet);
    FD_ZERO(&data->writeSet);

    return data;
}

static void setFdSet(struct Channel* channel,struct SelectData* data)
{
    if (channel->events & Read)
    {
        FD_SET(channel->fd, &data->readSet);
    }
    if (channel->events & Write)
    {
        FD_SET(channel->fd, &data->writeSet);
    }
}
static void clearFdSet(struct Channel* channel, struct SelectData* data)
{
    if (channel->events & Read)
    {
        FD_CLR(channel->fd, &data->readSet);
    }
    if (channel->events & Write)
    {
        FD_CLR(channel->fd, &data->writeSet);
    }
}

static int selectAdd( struct EventLoop* evLoop,struct Channel* channel)
{
    struct SelectData* data = (struct SelectData*)evLoop->DispatcherData;
    if (channel->fd >= Max)
    {
        return -1;
    }
    setFdSet(channel, data);
    return 0;
}

static int selectRemove( struct EventLoop* evLoop,struct Channel* channel)
{
    struct SelectData* data = (struct SelectData*)evLoop->DispatcherData;
    clearFdSet(channel, data);
    // ͨ�� channel �ͷŶ�Ӧ�� TcpConnection ��Դ
    channel->destroyCallback(channel->arg);

    return 0;
}

static int selectModify( struct EventLoop* evLoop,struct Channel* channel)
{
    struct SelectData* data = (struct SelectData*)evLoop->DispatcherData;
    setFdSet(channel, data);
    clearFdSet(channel, data);
    return 0;
}

static int selectDispatch(struct EventLoop* evLoop, int timeout)
{
    struct SelectData* data = (struct SelectData*)evLoop->DispatcherData;
    struct timeval val;
    val.tv_sec = timeout;
    val.tv_usec = 0;
    fd_set rdtmp = data->readSet;
    fd_set wrtmp = data->writeSet;
    int count = select(Max, &rdtmp, &wrtmp, NULL, &val);
    if (count == -1)
    {
        perror("select");
        exit(0);
    }
    for (int i = 0; i < Max; ++i)
    {
        if (FD_ISSET(i, &rdtmp))
        {
          
        }

        if (FD_ISSET(i, &wrtmp))
        {
            
        }
    }
    return 0;
}

static int selectClear(struct EventLoop* evLoop)
{
    struct SelectData* data = (struct SelectData*)evLoop->DispatcherData;
    free(data);
    return 0;
}