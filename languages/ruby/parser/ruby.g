-----------------------------------------------------------------------------
-- Copyright (c) 2006 Alexander Dymo <adymo@kdevelop.org>
--
-- Permission is hereby granted, free of charge, to any person obtaining
-- a copy of this software and associated documentation files (the
-- "Software"), to deal in the Software without restriction, including
-- without limitation the rights to use, copy, modify, merge, publish,
-- distribute, sublicense, and/or sell copies of the Software, and to
-- permit persons to whom the Software is furnished to do so, subject to
-- the following conditions:
--
-- The above copyright notice and this permission notice shall be
-- included in all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
-- EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
-- MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
-- NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
-- LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
-- OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
-- WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
-- Grammar for Ruby 1.8.5
-- Modelled after:
-- YACC Ruby 1.8.4 grammar by Yukihiro Matsumoto (parse.y in Ruby sources);
-- BSD-licensed ANTLR Ruby grammar by Xue Yong Zhi (http://seclib.blogspot.com);
-- Ruby language reference from Programming Ruby SE book by Dave Thomas
-----------------------------------------------------------------------------

-- 1 first/follow conflict:
-- - FIRST/FOLLOW conflict in hashExpression: resolved manually using the
--   semantic predicate

-- Known problems:
-- - command rule should use methodInvocationArgumentWithoutParen subrule
--   but that leads to 16 FIRST/FOLLOW conflicts in expressions




------------------------------------------------------------
-- Parser class members
------------------------------------------------------------

%parserclass (public declaration)
[:
  /**
   * Transform the raw input into tokens.
   * When this method returns, the parser's token stream has been filled
   * and any parse_*() method can be called.
   */
  void tokenize( char *contents );

  enum problem_type {
      error,
      warning,
      info
  };

  void report_problem( parser::problem_type type, const char* message );
  void report_problem( parser::problem_type type, std::string message );
:]


%parserclass (private declaration)
[:
  struct parser_state {
      // ltCounter stores the amount of currently open type arguments rules,
      // all of which are beginning with a less than ("<") character.
      // This way, also RSHIFT (">>") can be used to close type arguments rules,
      // in addition to GREATER_THAN (">").
      int ltCounter;
  };
  parser_state _M_state;

  //state modifiers
  bool seen_star;
  bool seen_star_or_band;

:]




------------------------------------------------------------
-- List of defined tokens
------------------------------------------------------------

-- reserved words
%token END ("end"), ELSE ("else"), ELSIF ("elsif"), RESCUE ("rescue"),
    ENSURE ("ensure"), WHEN ("when"), IF_MODIFIER ("if"), WHILE_MODIFIER ("while"),
    UNLESS_MODIFIER ("unless"), UNTIL_MODIFIER ("until"), RESCUE_MODIFIER ("rescue"),
    UNDEF ("undef"), ALIAS ("alias"), BEGIN ("begin"), DO ("do"), RETURN ("return"),
    BREAK ("break"), NEXT ("next"), NIL ("nil"), TRUE ("true"), FALSE ("false"),
    FILE ("__FILE__"), LINE ("__LINE__"), SELF ("self"), SUPER ("super"),
    RETRY ("retry"), YIELD ("yield"), DEFINED ("defined?"), REDO ("redo"), IF ("if"),
    CASE ("case"), UNTIL ("until"), WHILE ("while"), FOR ("for"), MODULE ("module"),
    DEF ("def"), THEN ("then"), CLASS ("class"), UNLESS ("unless"), OR ("or"),
    AND ("and"), IN ("in"), KEYWORD_NOT ("not"), END_UPCASE ("END"), BEGIN_UPCASE ("BEGIN") ;;

-- punctuation
%token LCURLY ("{"), RCURLY ("}"), LPAREN ("("), RPAREN (")"), SEMI (";"), COMMA (","),
    LCURLY_BLOCK ("{"), RCURLY_BLOCK ("}"), LEADING_TWO_COLON ("::"),
    ASSOC ("=>"), TWO_COLON ("::"), COLON (":"), SINGLE_QUOTE ("'"), RBRACK ("]"),
    LBRACK ("["), LBRACK_ARRAY_ACCESS ("["), DOT ("."), QUESTION ("?"),
    COLON_WITH_NO_FOLLOWING_SPACE (":"), LCURLY_HASH ("{") ;;

-- operators
%token REST_ARG_PREFIX ("*"), ASSIGN ("="), ASSIGN_WITH_NO_LEADING_SPACE ("="),
    BOR ("|"), LOGICAL_OR ("||"), PLUS_ASSIGN ("+="), MINUS_ASSIGN ("-="),
    STAR_ASSIGN ("*="), DIV_ASSIGN ("/="), MOD_ASSIGN ("%="), POWER_ASSIGN ("**="),
    BAND_ASSIGN ("&="), BXOR_ASSIGN ("^="), BOR_ASSIGN ("|="),
    LEFT_SHIFT_ASSIGN ("<<="), RIGHT_SHIFT_ASSIGN (">>="),
    LOGICAL_AND_ASSIGN ("&&="), LOGICAL_OR_ASSIGN ("||="),
    INCLUSIVE_RANGE (".."), EXCLUSIVE_RANGE ("..."), LOGICAL_OR ("|"),
    LOGICAL_AND ("&"), COMPARE ("<=>"), EQUAL ("=="), CASE_EQUAL ("==="),
    MATCH ("=~"), NOT_EQUAL ("!="), NOT_MATCH ("!~"), LESS_THAN("<"),
    GREATER_THAN (">"), LESS_OR_EQUAL ("<="), GREATER_OR_EQUAL (">="),
    BXOR ("^"), BOR ("|"), BAND ("&"), LEFT_SHIFT ("<<"), RIGHT_SHIFT (">>"),
    PLUS ("+"), MINUS ("-"), STAR ("*"), DIV ("/"), MOD ("%"),
    UNARY_MINUS ("-"), UNARY_PLUS ("+"), NOT ("!"), BNOT ("~"), POWER ("**") ;;

-- special tokens
%token EOF ("end of file"), LINE_BREAK ("line break"), IDENTIFIER ("identifier"),
    CONSTANT ("constant"), FUNCTION ("function"), GLOBAL_VARIABLE ("global variable"),
    INSTANCE_VARIABLE ("instance variable"), CLASS_VARIABLE ("class variable"),
    UNARY_PLUS_MINUS_METHOD_NAME ("+/- method"),
    DOUBLE_QUOTED_STRING ("double quoted string"), SINGLE_QUOTED_STRING ("single quoted string"),
    STRING_BEFORE_EXPRESSION_SUBSTITUTION ("string before expression substitution"),
    STRING_BETWEEN_EXPRESSION_SUBSTITUTION ("string between expression substitution"),
    STRING_AFTER_EXPRESSION_SUBSTITUTION ("string after expression substitution"),
    REGEX_BEFORE_EXPRESSION_SUBSTITUTION ("regular expression before expression substitution"),
    REGEX ("regular expression"), COMMAND_OUTPUT ("command output"),
    COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION ("command output before expression substitution"),
    HERE_DOC_BEGIN ("here document beginning"), W_ARRAY ("%w{} array"),
    INTEGER ("integer number"), HEX ("hexadecimal number"), OCTAL ("octal number"),
    BINARY ("binary number"), FLOAT ("float number"), ASCII_VALUE ("ascii value"),
    DO_IN_CONDITION ("do in condition"), BLOCK_ARG_PREFIX ("block argument prefix") ;;

%token STUB_A, STUB_B, STUB_C ;;




------------------------------------------------------------
-- Start of the actual grammar
------------------------------------------------------------

    ( compoundStatement | 0 )
-> program ;;


    terminal ( statements | 0 )
    | statements
-> compoundStatement ;;


    statement (
        terminal
        [:
            if ((yytoken == Token_LCURLY) || (yytoken == Token_END) //block end
                || (yytoken == Token_RPAREN) || (yytoken == Token_ELSE)
                || (yytoken == Token_ELSIF) || (yytoken == Token_RESCUE)
                || (yytoken == Token_ENSURE) || (yytoken == Token_WHEN) )
                break;
        :]
        statement
    )*
-> statements ;;


    SEMI
    | LINE_BREAK
-> terminal ;;


    statementWithoutModifier (
        IF_MODIFIER expression
        | UNLESS_MODIFIER expression
        | WHILE_MODIFIER expression
        | UNTIL_MODIFIER expression
        | RESCUE_MODIFIER expression    -- FIXME: statement after rescue
    )*
-> statement ;;


    ALIAS aliasParameter (LINE_BREAK | 0) aliasParameter
    | UNDEF (undefParameter @ COMMA)
    | BEGIN_UPCASE LCURLY_BLOCK (compoundStatement | 0) RCURLY
    | END_UPCASE LCURLY_BLOCK (compoundStatement | 0) RCURLY
    | expression (parallelAssignmentLeftOver | 0)
    | REST_ARG_PREFIX mlhs_item ASSIGN mrhs
-> statementWithoutModifier ;;


    IDENTIFIER (ASSIGN_WITH_NO_LEADING_SPACE | 0)
    | CONSTANT (ASSIGN_WITH_NO_LEADING_SPACE | 0)
    | FUNCTION (ASSIGN_WITH_NO_LEADING_SPACE | 0)
    | symbol
    | keywordAsMethodName
    | operatorAsMethodname
-> undefParameter ;;


    undefParameter
    | GLOBAL_VARIABLE
-> aliasParameter ;;


    try/rollback (LPAREN dotAccess (COMMA dotAccess)+ RPAREN )
    catch (dotAccess)
-> mlhs_item ;;


    ( COMMA [: if ((yytoken == Token_ASSIGN) || (yytoken == Token_ASSIGN_WITH_NO_LEADING_SPACE)) break; :]
        (REST_ARG_PREFIX [: seen_star = true; :] | 0 [: seen_star = false; :])
        (mlhs_item | 0) [: if (seen_star) break; :] )+
    ( (ASSIGN mrhs) | 0 )
-> parallelAssignmentLeftOver ;;


    expression (COMMA [: if ((yytoken == Token_ASSIGN) || (yytoken == Token_RBRACK)) break; :]
        (REST_ARG_PREFIX [: seen_star = true; :] | 0 [: seen_star = false; :])
        expression [: if (seen_star) break; :] )*
    | REST_ARG_PREFIX expression
-> mrhs ;;


    try/rollback(
        LPAREN elementReference (
            COMMA [: if (yytoken == Token_RPAREN) break; :]
            (REST_ARG_PREFIX [: seen_star = true; :] | 0 [: seen_star = false; :]) elementReference
            [: if (seen_star) break; :] )*
            RPAREN
    ) catch (
        REST_ARG_PREFIX (elementReference | 0)
        | elementReference
    )
-> block_var ;;


    block_var (COMMA [: if ((yytoken == Token_BOR) || (yytoken == Token_IN)) break; :] block_var )*
-> block_vars ;;


    DO blockContent END
    | LCURLY_BLOCK blockContent RCURLY
-> codeBlock ;;


--FIXME: conflict
    ((( BOR (block_vars | 0) BOR ) | LOGICAL_OR) | 0) (compoundStatement | 0)
-> blockContent ;;


--FIXME: greedy issue
    COLON_WITH_NO_FOLLOWING_SPACE (
        IDENTIFIER (ASSIGN_WITH_NO_LEADING_SPACE | 0) --greedy
        | FUNCTION (ASSIGN_WITH_NO_LEADING_SPACE | 0) --greedy
        | CONSTANT (ASSIGN_WITH_NO_LEADING_SPACE | 0) --greedy
        | GLOBAL_VARIABLE
        | INSTANCE_VARIABLE
        | CLASS_VARIABLE
        | UNARY_PLUS_MINUS_METHOD_NAME
        | operatorAsMethodname
        | keyword
        | string
    )
-> symbol ;;


    andorExpression
-> expression ;;


--FIXME: greedy issue
    notExpression @ (AND | OR) --greedy
-> andorExpression ;;


    KEYWORD_NOT notExpression
    | ternaryIfThenElseExpression
-> notExpression ;;


--FIXME: greedy issue
    assignmentExpression ( (QUESTION ternaryIfThenElseExpression COLON ternaryIfThenElseExpression) | 0) --greedy
-> ternaryIfThenElseExpression ;;


--FIXME: greedy issue
    rangeExpression @ (
        ( ASSIGN
        | PLUS_ASSIGN
        | MINUS_ASSIGN
        | STAR_ASSIGN
        | DIV_ASSIGN
        | MOD_ASSIGN
        | POWER_ASSIGN
        | BAND_ASSIGN
        | BXOR_ASSIGN
        | BOR_ASSIGN
        | LEFT_SHIFT_ASSIGN
        | RIGHT_SHIFT_ASSIGN
        | LOGICAL_AND_ASSIGN
        | LOGICAL_OR_ASSIGN
        )
        (REST_ARG_PREFIX | 0)
    )
-> assignmentExpression ;;


--FIXME: greedy issue for all expression rules below

    logicalOrExpression @ (INCLUSIVE_RANGE | EXCLUSIVE_RANGE) --greedy
-> rangeExpression ;;


    logicalAndExpression @ LOGICAL_OR --greedy
-> logicalOrExpression ;;


    equalityExpression @ LOGICAL_AND --greedy
-> logicalAndExpression ;;


    relationalExpression @ (
        ( COMPARE
        | EQUAL
        | CASE_EQUAL
        | NOT_EQUAL
        | MATCH
        | NOT_MATCH
        )
    ) --greedy
-> equalityExpression ;;


    orExpression @ (
        ( LESS_THAN
        | GREATER_THAN
        | LESS_OR_EQUAL
        | GREATER_OR_EQUAL
        )
    ) --greedy
-> relationalExpression ;;


    andExpression @ (BXOR | BOR) --greedy
-> orExpression ;;


    shiftExpression @ BAND --greedy
-> andExpression ;;


    additiveExpression @ (LEFT_SHIFT | RIGHT_SHIFT)
-> shiftExpression ;;


    multiplicativeExpression @ (PLUS | MINUS)
-> additiveExpression ;;


    powerExpression @ (STAR | DIV | MOD)
-> multiplicativeExpression ;;


    unaryExpression @ POWER
-> powerExpression ;;

    (UNARY_PLUS
    |UNARY_MINUS
    |BNOT
    |NOT
    )*
    dotAccess
-> unaryExpression ;;


    elementReference @ DOT
-> dotAccess ;;


    command (arrayAccess)*
-> elementReference ;;


-- FIXME: 16 FIRST/FOLLOW conflicts because of this
    colonAccess (methodInvocationArgumentWithParen | 0) (codeBlock | 0)
    | (RETURN | BREAK | NEXT) (methodInvocationArgumentWithParen | 0)
-> command ;;


    methodCall @ TWO_COLON
-> colonAccess ;;


    primaryExpression (methodInvocationArgumentWithParen | 0)
-> methodCall ;;


    LPAREN (methodInvocationArgumentWithoutParen | 0) (LINE_BREAK | 0) RPAREN
-> methodInvocationArgumentWithParen ;;


-- see line break ignore rules in original grammar

    normalMethodInvocationArgument (COMMA
            [: if ((yytoken == Token_REST_ARG_PREFIX) || (yytoken == Token_BLOCK_ARG_PREFIX)) break; :]
            normalMethodInvocationArgument
            )*
        (restMethodInvocationArgument | blockMethodInvocationArgument | 0)
    | restMethodInvocationArgument
    | blockMethodInvocationArgument
-> methodInvocationArgumentWithoutParen ;;


    expression (ASSOC expression | 0) (LINE_BREAK | 0)
-> normalMethodInvocationArgument ;;


    REST_ARG_PREFIX expression (COMMA blockMethodInvocationArgument | 0)
-> restMethodInvocationArgument ;;


    BLOCK_ARG_PREFIX expression
-> blockMethodInvocationArgument ;;


    NIL
    | TRUE
    | FALSE
    | FILE
    | LINE
-> predefinedValue ;;


    FUNCTION
    | SELF
    | SUPER
    | IDENTIFIER
    | CONSTANT
    | INSTANCE_VARIABLE
    | GLOBAL_VARIABLE
    | CLASS_VARIABLE
    | predefinedValue
-> variable ;;


    DOUBLE_QUOTED_STRING
    | SINGLE_QUOTED_STRING
    | STRING_BEFORE_EXPRESSION_SUBSTITUTION
        (expressionSubstitution @ STRING_BETWEEN_EXPRESSION_SUBSTITUTION)
        STRING_AFTER_EXPRESSION_SUBSTITUTION
-> string ;;


    (LCURLY_BLOCK compoundStatement RCURLY
    |GLOBAL_VARIABLE
    |INSTANCE_VARIABLE
    |CLASS_VARIABLE
    )
-> expressionSubstitution ;;


    REGEX
    | REGEX_BEFORE_EXPRESSION_SUBSTITUTION
        (expressionSubstitution @ STRING_BETWEEN_EXPRESSION_SUBSTITUTION)
        STRING_AFTER_EXPRESSION_SUBSTITUTION
-> regex ;;


    COMMAND_OUTPUT
    | COMMAND_OUTPUT_BEFORE_EXPRESSION_SUBSTITUTION
        (expressionSubstitution @ STRING_BETWEEN_EXPRESSION_SUBSTITUTION)
        STRING_AFTER_EXPRESSION_SUBSTITUTION
-> commandOutput ;;


    regex
    | (string)+
    | HERE_DOC_BEGIN
    | commandOutput
    | symbol
    | W_ARRAY
-> literal ;;


    INTEGER
    | HEX
    | BINARY
    | OCTAL
    | FLOAT
    | ASCII_VALUE
-> numeric ;;


    LEADING_TWO_COLON FUNCTION
    | variable
    | numeric
    | literal
    | arrayExpression
    | hashExpression
    | LPAREN compoundStatement RPAREN
    | ifExpression
    | unlessExpression
    | whileExpression
    | untilExpression
    | caseExpression
    | forExpression
    | exceptionHandlingExpression
    | moduleDefinition
    | classDefinition
    | methodDefinition
    | RETRY
    | YIELD
    | DEFINED
    | REDO
-> primaryExpression ;;


    keyValuePair (COMMA [: if ((yytoken == Token_ASSIGN) || (yytoken == Token_RBRACK)) break; :]
        (REST_ARG_PREFIX [: seen_star = true; :] | 0 [: seen_star = false; :])
        keyValuePair [: if (seen_star) break; :] )*
    | REST_ARG_PREFIX expression (LINE_BREAK | 0)
-> arrayReferenceArgument ;;


    LBRACK_ARRAY_ACCESS (arrayReferenceArgument | 0) RBRACK
-> arrayAccess ;;


    LBRACK (arrayReferenceArgument | 0) RBRACK
-> arrayExpression ;;


    expression (ASSOC expression | 0) (LINE_BREAK | 0)
-> keyValuePair ;;


    LCURLY_HASH (
        ( keyValuePair
        (?[: Token_RCURLY != LA(2).kind :] COMMA keyValuePair)*
        (COMMA |0) ) | 0 )
        RCURLY
-> hashExpression ;;


    (compoundStatement | 0)
    (RESCUE exceptionList thenOrTerminalOrColon (compoundStatement | 0))*
    (ELSE (compoundStatement | 0) | 0)
    (ENSURE (compoundStatement| 0) | 0)
-> bodyStatement ;;


    BEGIN bodyStatement END
-> exceptionHandlingExpression ;;

    ( ((className|INSTANCE_VARIABLE) @ COMMA) | 0) ((ASSOC (IDENTIFIER|FUNCTION)) | 0)
-> exceptionList ;;


    IF expression thenOrTerminalOrColon (compoundStatement | 0)
    (ELSIF (expression | 0) thenOrTerminalOrColon (compoundStatement | 0))*
    (ELSE (compoundStatement|0) |0)
    END
-> ifExpression ;;


    UNLESS expression thenOrTerminalOrColon (compoundStatement | 0)
    ( (ELSE (compoundStatement|0)) | 0)
    END
-> unlessExpression ;;


    CASE (compoundStatement | 0)
        (WHEN mrhs thenOrTerminalOrColon (compoundStatement | 0))+
        ((ELSE (compoundStatement | 0)) | 0)
        END
-> caseExpression ;;


    FOR block_vars IN expression doOrTerminalOrColon
    (compoundStatement | 0)
    END
-> forExpression ;;


    WHILE expression doOrTerminalOrColon (compoundStatement | 0) END
-> whileExpression ;;


    UNTIL expression doOrTerminalOrColon (compoundStatement | 0) END
-> untilExpression ;;


    MODULE moduleName terminal bodyStatement END
-> moduleDefinition ;;


    CONSTANT (TWO_COLON FUNCTION)*
-> moduleName ;;


    CLASS ( className (LESS_THAN expression | 0)
        | LEFT_SHIFT expression )
        terminal bodyStatement END
-> classDefinition ;;


    (CONSTANT | FUNCTION | LEADING_TWO_COLON FUNCTION) (TWO_COLON FUNCTION)*
-> className ;;


    DEF methodName methodDefinitionArgument bodyStatement END
-> methodDefinition ;;


    (DOT | TWO_COLON)
    (IDENTIFIER	(ASSIGN_WITH_NO_LEADING_SPACE | 0)
    |FUNCTION	(ASSIGN_WITH_NO_LEADING_SPACE | 0)
    |CONSTANT	(ASSIGN_WITH_NO_LEADING_SPACE | 0)
    |operatorAsMethodname
    |keyword
    )
-> methodNameSupplement ;;


    operatorAsMethodname
    | keywordAsMethodName
    | (IDENTIFIER | FUNCTION | CONSTANT) (methodNameSupplement | ASSIGN_WITH_NO_LEADING_SPACE | 0)
    | LPAREN expression (LINE_BREAK | 0) RPAREN methodNameSupplement
    | (INSTANCE_VARIABLE | CLASS_VARIABLE | GLOBAL_VARIABLE) methodNameSupplement
    | (NIL | SELF | TRUE | FALSE | FILE | LINE) methodNameSupplement
    | UNARY_PLUS_MINUS_METHOD_NAME
-> methodName ;;


    LEFT_SHIFT
    |RIGHT_SHIFT
    |EQUAL
    |CASE_EQUAL
    |GREATER_THAN
    |GREATER_OR_EQUAL
    |LESS_THAN
    |LESS_OR_EQUAL
    |PLUS
    |MINUS
    |STAR
    |DIV
    |MOD
    |POWER
    |BAND
    |BOR
    |BXOR
    |(LBRACK|LBRACK_ARRAY_ACCESS) RBRACK (ASSIGN_WITH_NO_LEADING_SPACE | 0)
    |MATCH
    |COMPARE
    |BNOT
    |SINGLE_QUOTE
-> operatorAsMethodname ;;


    keywordAsMethodName
    |NIL
    |SELF
    |TRUE
    |FALSE
    |FILE
    |LINE
-> keyword ;;

    AND
    |DEF
    |END
    |IN
    |OR
    |UNLESS
    |BEGIN
    |DEFINED
    |ENSURE
    |MODULE
    |REDO
    |SUPER
    |UNTIL
    |BEGIN_UPCASE
    |BREAK
    |DO
    |NEXT
    |RESCUE
    |THEN
    |WHEN
    |END_UPCASE
    |CASE
    |ELSE
    |FOR
    |RETRY
    |WHILE
    |ALIAS
    |CLASS
    |ELSIF
    |IF
    |NOT
    |RETURN
    |UNDEF
    |YIELD
-> keywordAsMethodName ;;


    LPAREN (methodDefinitionArgumentWithoutParen | 0) RPAREN (terminal | 0)
    | (methodDefinitionArgumentWithoutParen | 0) terminal
-> methodDefinitionArgument ;;


    normalMethodDefinitionArgument
        (COMMA
            [: if (Token_REST_ARG_PREFIX == yytoken || Token_BLOCK_ARG_PREFIX == yytoken)
                {seen_star_or_band = true; break;} :]
        normalMethodDefinitionArgument)*
        (?[: seen_star_or_band :] restMethodDefinitionArgument
            |?[: seen_star_or_band :] blockMethodDefinitionArgument
            | 0)
    | restMethodDefinitionArgument
    | blockMethodDefinitionArgument
-> methodDefinitionArgumentWithoutParen ;;


    (IDENTIFIER | FUNCTION) ( (ASSIGN | ASSIGN_WITH_NO_LEADING_SPACE) expression | 0)
-> normalMethodDefinitionArgument ;;


    REST_ARG_PREFIX ((IDENTIFIER | FUNCTION) (COMMA blockMethodDefinitionArgument | 0) | 0)
-> restMethodDefinitionArgument ;;

    BLOCK_ARG_PREFIX (IDENTIFIER | FUNCTION)
-> blockMethodDefinitionArgument ;;


    THEN
    | terminal (THEN | 0)
    | COLON
-> thenOrTerminalOrColon ;;


    DO_IN_CONDITION
    | terminal
    | COLON
-> doOrTerminalOrColon ;;




-----------------------------------------------------------------
-- Code segments copied to the implementation (.cpp) file.
-- If existent, kdevelop-pg's current syntax requires this block
-- to occur at the end of the file.
-----------------------------------------------------------------

[:

#include "ruby_lexer.h"

namespace ruby {

void parser::tokenize( char *contents )
{
    Lexer lexer( this, contents );

    int kind = parser::Token_EOF;
    do
    {
        kind = lexer.yylex();
        std::cerr << lexer.YYText() << " of kind " << kind << std::endl; //" "; // debug output

        if ( !kind ) // when the lexer returns 0, the end of file is reached
            kind = parser::Token_EOF;

        parser::token_type &t = this->token_stream->next();
        t.kind = kind;
        t.begin = lexer.tokenBegin();
        t.end = lexer.tokenEnd();
        t.text = contents;
    }
    while (kind != parser::Token_EOF);

    this->yylex(); // produce the look ahead token
}

parser::parser_state *parser::copy_current_state()
{
    parser_state *state = new parser_state();
    state->ltCounter = _M_state.ltCounter;
    return state;
}

void parser::restore_state( parser::parser_state *state )
{
    _M_state.ltCounter = state->ltCounter;
}

} // end of namespace ruby

:]
