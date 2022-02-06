# Contex Free Grammar for Syntax Analyzer

    Program → program id; ConstBlock VarBlock MainCode
    MainCode → begin StatementList end.
    ConstBlock → const ConstList  | e
    ConstList → id = Value; ConstList
    ConstList → e
    Value → v_int | v_real | v_string
    VarBlock → var VarList  | e
    VarList → VarDecl : Type ; VarList
    VarList → e
    VarDecl → id | id, VarDecl
    Type → real | integer | string
    StatementList → Statement StatementList | Statement
    Statement → ForStatement | IfStatement | Assign | WriteLn | Write | break; | continue;
    ForStatement → for id := Value To Expr do begin StatementList end;
    IfStatement → if (Expr) then begin StatementList end; else begin StatementList end;
    Assign → id := Expr;
    Expr → not Expr Expr'
    Expr → Expr2 Expr'
    Expr'→ BooleanOp Expr2 Expr' | e
    Expr2 → Expr3 Expr2'
    Expr2'→ RelOp Expr3 Expr2' | e
    Expr3 → Term Expr3'
    Expr3'→ + Term Expr3'
    Expr3'→ - Term Expr3'
    Expr3'→ e
    Term → Factor Term'
    Term' → * Factor Term'
    Term' → / Factor Term'
    Term' → div Factor Term'
    Term' → mod Factor Term'
    Term' → e
    Factor → id
    Factor → Value
    Factor → (Expr)
    RelOp → = |<>| <| <=| >= | >
    BooleanOp → and | or
    WriteLn → writeln ( Value );//<-
    WriteLn → writeln ( Value, Value);
    WriteLn → writeLn ( Value, Value, Value);
    Write → write (Expr);//<-
    To → to | downto

# Every Follow from every non-terminal

    ConstBlock → var
    ConstList → var, id (SINCE CONSTLIST IS RECURSIVE)
    Value → ;, To, Follow(Factor), ), , , 
    VarBlock → begin
    VarList → id (since varlist is recursive),  begin
    VarDecl → :, id
    Type → ;
    StatementList → First(Statement)
    Statement → ;, First(Statement)
    ForStatement → First(Statement)
    IfStatement → First(Statement)
    Assign → ;, First(Statement)
    Expr → ;, First(Expr'),),
    Expr' → Follow(Expr')
    Expr2 → First(Expr'), First(Expr3), RelOp
    Expr2' →  Follow(Expr2), RelOp
    Expr3 → First(Expr2')
    Expr3' → +,-, First(Expr2')
    Term → First(Expr3')
    Term' → First(Expr3'), *,/,div/mod
    Factor → First(Term')
    RelOp → First(Expr3)
    BooleanOp → First(Expr2)
    WriteLn → First(Statement)
    Write → First(Statement)
    To → First(Expr)