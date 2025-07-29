/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#ifndef IEVENTHANDLER_HPP
# define IEVENTHANDLER_HPP

#include "EpollManager.hpp"
#include "Timer.hpp"

class EpollManager;

class IEventHandler {
protected:
    EpollManager    *_epollmng;
    Timer           _timer;
    int64_t        _timeout;

public:
	/* Attributes */
	int             fileDescriptor;
    bool            RemoveHandler;
    bool            has_timeout;
    bool            deleted;

	/* Methods */
    virtual ~IEventHandler();
    /*
    * Handle output event
    */
    virtual void handleWrite();

    /*
    * Handle input event
    */
    virtual void handleRead();

    /*
    * Handle error event
    */
    virtual void handleError();

    /*
    * Handle disconnection event
    */
    virtual void handleHangUp();

    /*
    * Handle other event
    */
    virtual void handleGeneric();

    /*
    * Check for timeout
    */
    virtual bool hasTimeOut();
};

#endif // IEVENTHANDLER_HPP