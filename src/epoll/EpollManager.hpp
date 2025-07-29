/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#ifndef EPOLLMANAGER_HPP
# define EPOLLMANAGER_HPP

#include <sys/epoll.h>
#include <unistd.h>
#include <bitset>
#include <climits>
#include <cstring>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "IEventHandler.hpp"
#include "macros.h"
#include "utils.h"


class IEventHandler;

class EpollManager
{
private:
    // Singleton
    static EpollManager *_epoll_manager;

    int	_epoll_fd;
    struct epoll_event _events[10];
    
    EpollManager(int max_events = 10);
	~EpollManager();
    
public:
    
    // Singleton
    static EpollManager *getEpollManager();
    
	std::vector<IEventHandler *>	handlers;
    /* 
	* Clear memory and destroy the epoll manager
	*/
	static void Destroy();

	/*
    * Add EventHandler to the set of event sources
    */
    void addEventHandler(IEventHandler *event_handler, uint32_t events_mas);

    /*
    * Remove EventHandler to the set of event sources
    */
    void removeEventHandler(IEventHandler *event_handler, bool delete_handler = true);

    /*
    * Modify the monitored events of event source
    */
    void modifyEventHandler(IEventHandler *event_handler, uint32_t events_mask);

    /*
    * Wait for process events; (timeout_ms = -1; infinity)
    */
    int waitEvents(int timeout_ms = -1);

    /*
    * Get file descriptor of event
    */
    IEventHandler * getEventHandler(size_t index);

    /*
    * Get events from a specific index
    */
    uint32_t getEventsType(size_t index) const;

    void    eraseHandler(IEventHandler* handler);
};

#endif // EPOLLMANAGER_HPP

/*
 * events_mask summary:
 *      EPOLLIN: FD ready to read
 *      EPOLLOUT: FD ready to write
 *      EPOLLERR: FD have an error
 *      EPOLLHUP: FD has been closed
 *      EPOLLRDHUP: The other end of the connection has closed the connection (sockets)
 *      EPOLLET: Edge Activation Mode (edge-triggered).
 *      EPOLLONESHOT: Event that is automatically deactivated after being handled.
 *      EPOLLPRI: FD has high priority data.
 */

