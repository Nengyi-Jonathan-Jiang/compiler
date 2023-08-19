#pragma once

#include <vector>
#include <map>
#include <set>
#include "common/symbol.h"
#include "TableEntry.h"
#include "frontend/parser/parseRule.h"

class ParsingTable {

    std::vector<std::map<symbol, std::unique_ptr<TableEntry>>> actionTable, gotoTable;
    int numStates;

public:
    ParsingTable(int numStates) : numStates(numStates),
                                  actionTable(numStates),
                                  gotoTable(numStates) {}

    const TableEntry &getAction(int state, const symbol &symbol);

    std::set<symbol> acceptableSymbolsAtState(int state);

    TableEntry &getGoto(int state, const symbol &symbol);

    void setActionReduce(int state, symbol symbol, const parseRule& rule);

    void setActionShift(int state, symbol symbol, int nextState);

    void setActionAccept(int state, symbol symbol);

    void setGoto(int state, symbol symbol, int n);
};