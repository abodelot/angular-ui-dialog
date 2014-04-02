#include "StateQueue.hpp"
#include "State.hpp"


StateQueue::StateQueue():
    head_(nullptr)
{
}


StateQueue::~StateQueue()
{
}


void StateQueue::prepend(State* state)
{
    state->next = head_;
    head_ = state;
}


void StateQueue::insert(State* state)
{
    State* temp = nullptr;
    State* n = head_;
    while (n != nullptr && n->closer_to_end_than(*state)) {
        temp = n;
        n = n->next;
    }

    state->next = n;
    if (temp != nullptr) {
        temp->next = state;
    }
    else {
        // Empty queue, state is the new head
        head_ = state;
    }
}


State* StateQueue::pop()
{
    State* n = head_;
    if (n != nullptr) {
        head_ = n->next;
    }
    return n;
}


bool StateQueue::is_empty() const
{
    return head_ == nullptr;
}
