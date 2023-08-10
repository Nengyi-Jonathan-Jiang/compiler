#include "FSM.h"

const std::set<char> FSM::all_chars = make_set<char>(std::string{
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
});

const std::set<char> FSM::word_chars = make_set<char>(std::string{
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz0123456789"
});

const std::set<char> FSM::letters = make_set<char>(std::string{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"});

const std::set<char> FSM::digits = make_set<char>(std::string{"0123456789"});