#include "ParsingTable.h"

const TableEntry &ParsingTable::getAction(int state, const symbol &symbol) {
    return *actionTable.at(state).at(symbol).get();
}

std::set<symbol> ParsingTable::acceptableSymbolsAtState(int state) {
    const std::map<symbol, std::unique_ptr<TableEntry>> &table = actionTable.at(state);
    std::set<symbol> res;
    for (const auto &[symbol, _]: table) {
        res.emplace(symbol);
    }
    return res;
}

TableEntry &ParsingTable::getGoto(int state, const symbol &symbol) {
    return *gotoTable.at(state).at(symbol).get();
}

void ParsingTable::setActionReduce(int state, symbol symbol, const parseRule &rule) {
    actionTable.at(state).emplace(symbol, std::make_unique<ReduceEntry>(rule));
}

void ParsingTable::setActionShift(int state, symbol symbol, int nextState) {
    actionTable.at(state).emplace(symbol, std::make_unique<ShiftEntry>(nextState));
}

void ParsingTable::setActionAccept(int state, symbol symbol) {
    actionTable.at(state).emplace(symbol, std::make_unique<AcceptEntry>());
}

void ParsingTable::setGoto(int state, symbol symbol, int n) {
    gotoTable.at(state).emplace(symbol, std::make_unique<GotoEntry>(n));
}