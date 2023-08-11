#include "FSM.h"

const std::set<char> lexer::impl_::FSM::all_chars = make_set<char>(std::string{
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
});

const std::set<char> lexer::impl_::FSM::word_chars = make_set<char>(std::string{
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz0123456789"
});

const std::set<char> lexer::impl_::FSM::letters = make_set<char>(std::string{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"});

const std::set<char> lexer::impl_::FSM::digits = make_set<char>(std::string{"0123456789"});