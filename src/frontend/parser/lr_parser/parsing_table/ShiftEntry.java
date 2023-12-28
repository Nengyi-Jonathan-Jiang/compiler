package frontend.parser.lr_parser.parsing_table;

public record ShiftEntry(int nextState) implements TableEntry {
    public Action getAction() {
        return Action.SHIFT;
    }
}
