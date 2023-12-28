package frontend.parser.lr_parser.parsing_table;

public record GotoEntry(int nextState) implements TableEntry {
    public Action getAction() {
        return Action.GOTO;
    }
}