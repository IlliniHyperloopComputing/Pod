#include "StateMachine.h"
 
StateMachine::StateMachine(unsigned char maxStates) :
    currentState(0),
    _maxStates(maxStates),
    _eventGenerated(false),
    _pEventData(NULL)
{
}    
// returns the current state of the StateMachine
unsigned char StateMachine::getCurrentState() {
    std::lock_guard<std::mutex> guard(_mutex);
	return currentState;
}
// generates an external event. called once per external event 
// to start the state machine executing
void StateMachine::ExternalEvent(unsigned char newState, 
                                 EventData* pData)
{
    // if we are supposed to ignore this event
    if (newState == EVENT_IGNORED) {
        // just delete the event data, if any
        if (pData)  
            delete pData;
    }
    else {
        
        // generate the event and execute the state engine
        InternalEvent(newState, pData); 
        StateEngine();     

    }
}

// generates an internal event. called from within a state 
// function to transition to a new state
void StateMachine::InternalEvent(unsigned char newState, 
                                 EventData* pData)
{
    std::lock_guard<std::mutex> guard(_mutex); // Mutex lock for thread safety
	if (pData == NULL)
		pData = new EventData();

    _pEventData = pData;
    _eventGenerated = true;
    currentState = newState;
}

// the state engine executes the state machine states
void StateMachine::StateEngine(void)
{
    EventData* pDataTemp = NULL;
 
    // while events are being generated keep executing states
    while (_eventGenerated) {         
        pDataTemp = _pEventData;  // copy of event data pointer
        _pEventData = NULL;       // event data used up, reset ptr
        _eventGenerated = false;  // event used up, reset flag
 
        // assert(currentState < _maxStates);
        // We should avoid assert because it will casue a crash
        if (currentState < _maxStates) {
            // GO TO ERROR STATE MUST BE IMPLEMENTED
        }

		// get state map
        const StateStruct* pStateMap = GetStateMap();

        // execute the state passing in event data, if any
        (this->*pStateMap[currentState].pStateFunc)(pDataTemp);
 
        // if event data was used, then delete it
        if (pDataTemp) {
            delete pDataTemp;
            pDataTemp = NULL;
        }
    }
}
