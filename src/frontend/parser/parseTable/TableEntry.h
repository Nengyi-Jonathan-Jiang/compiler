#pragma once

#include <utility>

#include "frontend/parser/ParseRule.h"

struct TableEntry {
    enum class Action: uint8_t { SHIFT, REDUCE, ACCEPT, GOTO };
    virtual Action getAction()=0;

    virtual ~TableEntry()=0;
};

struct ReduceEntry : public TableEntry {
    parseRule rule;
    ReduceEntry(parseRule rule) : rule(std::move(rule)) {}
    Action getAction() final { return TableEntry::Action::REDUCE; }
};

struct ShiftEntry : public TableEntry {
    int nextState;
    ShiftEntry(int nextState) : nextState(nextState) {}
    Action getAction() final { return TableEntry::Action::SHIFT; }
};

struct AcceptEntry : public TableEntry {
    Action getAction() final { return TableEntry::Action::ACCEPT; }
};

struct GotoEntry : public TableEntry {
    int n;
    GotoEntry(int n) : n(n) {}
    Action getAction() final { return TableEntry::Action::GOTO; }
};