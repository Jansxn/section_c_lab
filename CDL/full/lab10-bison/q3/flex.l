%{
    #include "q4.tab.h"   
%}

%%
[0-9] {return num; }
\n {return NL ;}
[a-zA-Z][a-zA-Z0-9_]* {return ID; }
. {return yytext[0]; }
%%
int yywrap() {}