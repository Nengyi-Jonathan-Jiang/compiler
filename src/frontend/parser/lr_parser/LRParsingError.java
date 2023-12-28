package frontend.parser.lr_parser;

import frontend.Symbol;

import java.util.Set;

public class LRParsingError extends RuntimeException {
    private final Set<Symbol> expected;

    public LRParsingError(String message, Set<Symbol> expected) {
        super(message);
        this.expected = expected;
    }

    public Set<Symbol> getExpected() {
        return expected;
    }
}
