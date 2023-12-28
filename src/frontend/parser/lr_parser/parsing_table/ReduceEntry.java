package frontend.parser.lr_parser.parsing_table;

import frontend.parser.Rule;

public record ReduceEntry(Rule rule) implements TableEntry {
    public Action getAction() {
        return Action.REDUCE;
    }
}