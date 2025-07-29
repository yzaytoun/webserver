/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#include "EpollManager.hpp"

EpollManager* EpollManager::_epoll_manager = NULL;

EpollManager* EpollManager::getEpollManager()
{
  if (_epoll_manager == NULL) {
	_epoll_manager = new EpollManager();
  }
  return (_epoll_manager);
}

EpollManager::EpollManager(int max_events)
{
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1) {
		throw std::runtime_error("Error create new epoll");
	}
	(void)max_events;
	//_events.resize(max_events);
}

EpollManager::~EpollManager() {
	if (_epoll_fd != -1) {
		close(_epoll_fd);
	}
	for (size_t i = 0; i < handlers.size(); i++)
	{
		delete handlers[i];
		handlers[i] = NULL;
	}
	
	handlers.clear();
}

void	EpollManager::Destroy()
{
	delete _epoll_manager;
	_epoll_manager = NULL;
}

void EpollManager::addEventHandler(IEventHandler *event_handler, uint32_t events_mask) {
	struct epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = events_mask;
	ev.data.ptr = event_handler;
	handlers.push_back(event_handler);
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, event_handler->fileDescriptor, &ev) == -1) {
		throw std::runtime_error("Error adding EventHandler descriptor to epoll");
	}
}

void EpollManager::removeEventHandler(IEventHandler *event_handler, bool delete_handler) {
	
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, event_handler->fileDescriptor, NULL) == -1) {
		throw std::runtime_error("Error removing EventHandler from epoll");
	}
	if (delete_handler) {
		eraseHandler(event_handler);
		delete event_handler;
		event_handler = NULL;
	}
}

void EpollManager::modifyEventHandler(IEventHandler *event_handler, uint32_t events_mask) {
	struct epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = events_mask;
	ev.data.ptr = event_handler;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, event_handler->fileDescriptor, &ev) == -1) {
		throw std::runtime_error("Error modifying EventHandler in epoll");
	}
}

int EpollManager::waitEvents(int timeout_ms) {
	int _num_events = epoll_wait(_epoll_fd, _events, EPOLL_MAX_EVENTS, timeout_ms);
	if (_num_events == -1) {
		throw std::runtime_error("epoll_wait failed");
	}
	return _num_events;
}

IEventHandler *EpollManager::getEventHandler(size_t index) {
	if (index >= EPOLL_MAX_EVENTS) {
		throw std::out_of_range("Index out of range");
	}
	return static_cast<IEventHandler *>(_events[index].data.ptr);
}

uint32_t EpollManager::getEventsType(size_t index) const {
	if (index >= EPOLL_MAX_EVENTS) {
		throw std::out_of_range("Index out of range");
	}
	return _events[index].events;
}

void    EpollManager::eraseHandler(IEventHandler* handler)
{
	for (std::size_t i = 0; i < handlers.size(); i++)
	{
		if (handlers[i] == handler)
		{
			handlers.erase(handlers.begin() + i);
			i = 0;
		}
	}
}
