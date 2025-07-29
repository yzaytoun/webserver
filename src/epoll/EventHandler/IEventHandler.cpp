/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#include "IEventHandler.hpp"

IEventHandler::~IEventHandler() {
    // Empty method
};

void IEventHandler::handleWrite() {
    // Empty method
};

void IEventHandler::handleRead() {
    // Empty method
};

void IEventHandler::handleError() {
    // Empty method
};

void IEventHandler::handleHangUp() {
    // Empty method
};

void IEventHandler::handleGeneric() {
    // Empty method
};

bool IEventHandler::hasTimeOut() {
    // Empty method
    return (false);
};
