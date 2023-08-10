#pragma once

#include <string>
#include <utility>
#include <vector>
#include "../../common/symbol.h"
#include "lexer.h"
#include "fsm/FSM.h"
#include "fsm/NFA.h"
#include "fsm/regexParser.h"
#include "fsm/NFABuilder.h"
#include "../../common/algorithms.h"

namespace lexer {
    class lexer;

    class lexerBuilder {
    public:
    };
}