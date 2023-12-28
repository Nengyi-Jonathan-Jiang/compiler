package frontend.parser.lr_parser.parsing_table;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;

import frontend.parser.Rule;
import frontend.Symbol;

public class ParsingTable{
    
    private final List<Map<Symbol,TableEntry>> actionTable, gotoTable;
    private final int numStates;
    
    public ParsingTable(int numStates){
        this.numStates = numStates;
        actionTable = new ArrayList<>(numStates);
        gotoTable = new ArrayList<>(numStates);
        for(int i = 0; i < numStates; i++){
            actionTable.add(new TreeMap<>());
            gotoTable.add(new TreeMap<>());
        }
    }
    
    public TableEntry getAction(int state, Symbol symbol){
        return actionTable.get(state).get(symbol);
    }

    public Set<Symbol> acceptableSymbolsAtState(int state) {
        return actionTable.get(state).keySet();
    }
    
    public TableEntry getGoto(int state, Symbol symbol){
        return gotoTable.get(state).get(symbol);
    }
    
    public void setActionReduce(int state, Symbol symbol, Rule rule){
        actionTable.get(state).put(symbol, new ReduceEntry(rule));
    }
    public void setActionShift(int state, Symbol symbol, int nextState){
        actionTable.get(state).put(symbol, new ShiftEntry(nextState));
    }
    public void setActionAccept(int state, Symbol symbol){
        actionTable.get(state).put(symbol, new AcceptEntry());
    }
    public void setGoto(int state, Symbol symbol, int n){
        gotoTable.get(state).put(symbol, new GotoEntry(n));
    }

    public void saveToFile(String filename){
        try {
            //noinspection ResultOfMethodCallIgnored
            new File(filename).createNewFile();
        } catch (IOException e) {
            throw new RuntimeException("Unable to create file " + filename, e);
        }
        try(PrintWriter printWriter = new PrintWriter(filename)) {
            printWriter.write(dataAsString());
        }
        catch(Exception e){
            throw new RuntimeException("Error saving LR parse table to file", e);
        }
    }

    private String dataAsString() {
        StringBuilder sb = new StringBuilder();

        sb.append(numStates);

        for(int state = 0; state < numStates; state++){
            for(Map.Entry<Symbol, TableEntry> mapEntry : actionTable.get(state).entrySet()){
                Symbol symbol = mapEntry.getKey();
                TableEntry entry = mapEntry.getValue();
                sb.append("\na ");
                sb.append(symbol);
                switch (entry.getAction()) {
                    case SHIFT -> {
                        sb.append(" s ");
                        sb.append(((ShiftEntry) entry).nextState());
                    }
                    case ACCEPT -> sb.append(" a ");
                    case REDUCE -> {
                        sb.append(" r ");
                        Rule rule = ((ReduceEntry) entry).rule();

                        sb.append(rule.getRhsSize());
                        sb.append(" ");

                        if(rule.chained) sb.append("__c ");
                        if(!rule.unwrap) sb.append("__w ");

                        sb.append(rule.getLhs());
                        sb.append(" ");
                        sb.append(rule.getRhs().toString());
                    }
                }
            }
            for(Map.Entry<Symbol, TableEntry> mapEntry : gotoTable.get(state).entrySet()){
                Symbol symbol = mapEntry.getKey();
                GotoEntry entry = (GotoEntry) mapEntry.getValue();
                sb.append("\ng ");
                sb.append(symbol);
                sb.append(" ");
                sb.append(entry.nextState());
            }
            sb.append("\ns");
        }

        return sb.toString();
    }

    public static ParsingTable loadFromFile(Symbol.SymbolTable symbolTable, String filename){
        try(Scanner scan = new Scanner(ParsingTable.class.getResourceAsStream("/" + filename))){
            int size = scan.nextInt();
            ParsingTable table = new ParsingTable(size);
            for(int state = 0; state < size; state++){
                String entryType;
                while(!(entryType = scan.next()).equals("s")){
                    Symbol symbol = symbolTable.get(scan.next());
                    switch (entryType) {
                        case "a" -> {
                            switch (scan.next()) {
                                case "a" -> table.setActionAccept(state, symbol);
                                case "s" -> table.setActionShift(state, symbol, scan.nextInt());
                                case "r" -> {
                                    int rhsSize = scan.nextInt();
                                    boolean chained = false, unwrap = true;
                                    Symbol lhs;
                                    {
                                        String n = scan.next();
                                        while(true) {
                                            if(n.equals("__c")) chained = true;
                                            else if(n.equals("__w")) unwrap = false;
                                            else break;
                                            n = scan.next();
                                        }
                                        lhs = symbolTable.get(n);
                                    }


                                    Symbol[] rhs = new Symbol[rhsSize];
                                    for (int i = 0; i < rhsSize; i++) {
                                        var a = scan.next();
                                        try {
                                            rhs[i] = symbolTable.get(a);
                                        }
                                        catch (Error e){
                                            throw new Error("Could not get symbol " + a + " at state " + state);
                                        }
                                    }
                                    table.setActionReduce(state, symbol, new Rule(lhs, chained, unwrap, rhs));
                                }
                            }
                        }
                        case "g" -> table.setGoto(state, symbol, scan.nextInt());
                    }
                }
            }
            return table;
        }
        catch(Exception e){
            System.out.println("Error loading LR parse table from file");
            e.printStackTrace();
            return null;
        }
    }
}
