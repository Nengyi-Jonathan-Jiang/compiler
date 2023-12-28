#pragma once

#include <map>
#include <iostream>
#include "ItemSet.h"
#include "ParsingTable.h"
#include "frontend/parser/grammar/Grammar.h"
#include "../../../common/Symbol.h"

class LRParseTableBuilderBase {
protected:

    std::map<ItemSet, int> configuratingSets;

    std::shared_ptr<ParsingTable> table;
    std::shared_ptr<Grammar> grammar;

    std::map<int, std::map<symbol, int>> successors;

    LRParseTableBuilderBase(const Grammar &grammar) : grammar(std::make_shared<Grammar>(grammar)) {
        generateConfiguratingSets();
        generateParsingTable();
    }
    
    virtual ~LRParseTableBuilderBase() = 0;

private:
    /** Generates the parsing table */
    void generateParsingTable() {
        std::cout << "Generating parsing table entries...";

        table = std::make_shared<ParsingTable>(configuratingSets.size());

        int i = 0;
        for (const auto &[itemSet, state]: configuratingSets) {
            // Generate Action table
            generateActionSetEntries(state, itemSet);
            // Generate Goto table
            generateGotoSetEntries(state, itemSet);
            // Debug
            std::cout << "Generated parsing table entries for " << (++i) << " states (currently on state " << state
                      << ")";
        }
    }

    Item getStartItem() {
        return {grammar->getData().start, 0, grammar->follow(special_symbols::START)};
    }

    /** Compute all configurating sets */
    void generateConfiguratingSets() {
        std::cout << "Generating configurating sets...";

        ItemSet initialState = closure(getStartItem());
        configuratingSets.put(initialState, 0);
        successors.put(0, new TreeMap<>());

        Set <ItemSet> edge = new TreeSet<>(Collections.singletonList(initialState));

        boolean updated = true;
        while (updated) {
            updated = false;

            Set <ItemSet> newEdge = new TreeSet<>();

            for (ItemSet configuratingSet: edge) {
                int state1 = configuratingSets.get(configuratingSet);
                for (Symbol symbol: grammar.getAllSymbols()) {
                    ItemSet successor = successor(configuratingSet, symbol);
                    if (successor.isEmpty()) continue;

                    if (addConfiguratingState(state1, symbol, successor)) {
                        updated = true;
                        newEdge.add(successor);
                    }
                }
            }

            edge = newEdge;
        }
    }

    /** Tries to add a configurating set to the family of configurating sets and returns true if something was updated */
    bool addConfiguratingState(int state, Symbol symbol, ItemSet successor) {
        if (!configuratingSets.containsKey(successor)) {
            int newState = configuratingSets.size();
            successors.put(newState, new TreeMap<>());
            configuratingSets.put(successor, newState);
            successors.get(state).put(symbol, newState);
            System.out.println(
                    "Found " + configuratingSets.size() + "th configurating set (" + successor.size() + " items)");
            return true;
        } else {
            successors.get(state).put(symbol, configuratingSets.get(successor));
            return false;
        }
    }

    /** Generates the action table entries for a state */
    void generateActionSetEntries(int state, ItemSet itemSet) {
        for (Item item: itemSet) {
            if (item.isFinished() && item.getRule().equals(grammar.getStartRule())) {
                table.setActionAccept(state, grammar.symbolTable.__END__);
            } else if (item.isFinished()) {
                generateReductions(state, item);
            } else {
                generateShifts(state, item);
            }
        }
    }

    /** Generate reduction entries given a state and an item */
    void generateReductions(int state, Item item) {
        Rule reduce = item.getRule();
        for (Symbol symbol: item.getLookahead()) {
            table.setActionReduce(state, symbol, reduce);
        }
    }

    /** Generate shift entries given a state and an item */
    void generateShifts(int state, Item item) {
        Integer nextState = successors.get(state).get(item.next());
        if (nextState != null) {
            table.setActionShift(state, item.next(), nextState);
        }
    }

    /** Generates the goto table entries for a state */
    void generateGotoSetEntries(int state, ItemSet itemSet) {
        for (Symbol symbol: grammar.getNonTerminals()) {
            Integer nextState = successors.get(state).get(symbol);
            if (nextState != null) {
                table.setGoto(state, symbol, nextState);
            }
        }
    }

    /** Computes the closure of an item */
    virtual ItemSet closure(Item item) = 0;

    /** Computes the closure of an itemset */
    ItemSet closure(ItemSet itemSet) {
        ItemSet res = itemSet.copy();
        for (Item item: itemSet) res.addAll(closure(item));
        return res;
    }

    /** Computes the successor set of an itemset */
    ItemSet successor(ItemSet itemSet, Symbol symbol) {
        ItemSet res = new ItemSet();
        for (Item item: itemSet)
            if (!item.isFinished() && item.next().equals(symbol))
                res.add(item.shift());
        return closure(res);
    }

public:
    /** Get the parsing table */
    const ParsingTable &getTable() {
        return *table;
    }
};