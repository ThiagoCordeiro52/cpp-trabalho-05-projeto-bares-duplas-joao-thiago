#include "../include/parser.h"
#include "../lib/stack.h"
#include <iterator>
#include <algorithm>
#include <stack>
#include <cstdlib>
#include <cstring>

/// Converts the input character c_ into its corresponding terminal symbol code.
Parser::terminal_symbol_t  Parser::lexer( char c_ ) const {
    switch( c_ ) {
        case '(':  return terminal_symbol_t::TS_OPEN_PARENTHESIS;
        case ')':  return terminal_symbol_t::TS_CLOSE_PARENTHESIS;
        case '+':  return terminal_symbol_t::TS_PLUS;
        case '-':  return terminal_symbol_t::TS_MINUS;
        case '*':  return terminal_symbol_t::TS_MULTI;
        case '/':  return terminal_symbol_t::TS_DIVISION;
        case '%':  return terminal_symbol_t::TS_REST;
        case '^':  return terminal_symbol_t::TS_EXPO;
        case ' ':  return terminal_symbol_t::TS_WS;
        case   9:  return terminal_symbol_t::TS_TAB;
        case '0':  return terminal_symbol_t::TS_ZERO;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':  return terminal_symbol_t::TS_NON_ZERO_DIGIT;
        case '\0': return terminal_symbol_t::TS_EOS; // end of string: the $ terminal symbol
    }
    return terminal_symbol_t::TS_INVALID;
}

/// Consumes a valid character from the input expression.
void Parser::next_symbol( void ) {
    // Advances iterator to the next valid symbol for processing
    std::advance( m_it_curr_symb, 1 ); // Mesmo que: m_it_curr_symb++;
}

/// Checks whether we reached the end of the input expression string.
bool Parser::end_input( void ) const {
    // "Fim de entrada" ocorre quando o iterador chega ao
    // fim da string que guarda a expressão.
    return m_it_curr_symb == m_expr.end();
}

// Returns the result of trying to match the current character with c_, **without** consuming the current character from the input expression.
bool Parser::peek( terminal_symbol_t c_ ) const {
    // Checks whether the input symbol is equal to the argument symbol.
    return ( not end_input() and
             lexer( *m_it_curr_symb ) == c_ );
}

/// Returns the result of trying to match and consume the current character with c_.
/*!
 * If the match is not successful, the current character (from the input) **is not consumed**.
 * @param c_ the current character from the input source expression we wish to parse.
 * @see peek().
 * @return true if we got a successful match; false otherwise.
 */
bool Parser::accept( terminal_symbol_t c_ ) {
    // If we have a match, we consume the character from the input source expression.
    // caractere da entrada.
    if ( not end_input() and lexer( *m_it_curr_symb ) == c_  ) {
        next_symbol();
        return true;
    }
    return false;
}

#ifdef EXPECT
// Skips all white spaces and tries to accept() the next valid character. @see accept().
bool Parser::expect( terminal_symbol_t c_ ) {
    // Skip all white spaces first.
    skip_ws();
    return accept( c_ );
}
#endif


/// Ignores any white space or tabs in the expression until reach a valid character or end of input.
void Parser::skip_ws( void ) {
    // Skip white spaces, while at the same time, check for end of string.
    while ( not end_input() and std::isspace( *m_it_curr_symb ) )
        next_symbol();
}



//=== Non Terminal Symbols (NTS) methods.

/// Validates (i.e. returns true or false) and consumes an **expression** from the input expression string.
/*! This method parses a valid expression from the input and, at the same time, it tokenizes its components.
 *
 * Production rule is:
 * ```
 *  <expr> := <term>,{ ("+"|"-"),<term> };
 * ```
 * An expression might be just a term or one or more terms with '+'/'-' between them.
 */
bool Parser::expression( void ) {
    if ( not term() ) return false;
    // Process terms
    while( m_result.type == ResultType::OK ) {
        skip_ws();
        if ( accept( Parser::terminal_symbol_t::TS_MINUS ) ) {
            // Stores the "-" token in the list.
            m_tk_list.emplace_back( Token{ "-", Token::token_t::OPERATOR } );
        }
        else if ( accept( Parser::terminal_symbol_t::TS_PLUS ) ) {
            // Stores the "+" token in the list.
            m_tk_list.emplace_back( Token{ "+", Token::token_t::OPERATOR } );
        }
        else if ( accept( Parser::terminal_symbol_t::TS_MULTI ) ) {
            // Stores the "*" token in the list.
            m_tk_list.emplace_back( Token{ "*", Token::token_t::OPERATOR } );
        }
        else if ( accept( Parser::terminal_symbol_t::TS_DIVISION ) ) {
            // Stores the "/" token in the list.
            m_tk_list.emplace_back( Token{ "/", Token::token_t::OPERATOR } );
        }
        else if ( accept( Parser::terminal_symbol_t::TS_REST ) ) {
            // Stores the "%" token in the list.
            m_tk_list.emplace_back( Token{ "%", Token::token_t::OPERATOR } );
        }
        else if ( accept( Parser::terminal_symbol_t::TS_EXPO ) ) {
            // Stores the "^" token in the list.
            m_tk_list.emplace_back( Token{ "^", Token::token_t::OPERATOR } );
        }
        else break;

        // After a operator we expect a valid term, otherwise we have a missing term.
        if ( not term() and m_result.type == ResultType::ILL_FORMED_INTEGER ) {
            // Make the error more specific.
            m_result.type = ResultType::MISSING_TERM;
        }
    }
    // Return true if everything ran smoothly.
    return m_result.type == ResultType::OK;
}

/// Validates (i.e. returns true or false) and consumes a **term** from the input expression string.
/*! This method parses and tokenizes a valid term from the input.
 *
 * Production rule is:
 * ```
 *  <term> := <integer>;
 * ```
 * A term is made of a single integer.
 *
 * @return true if a term has been successfuly parsed from the input; false otherwise.
 */
bool Parser::term( void ) {
    // Guarda o início do termo no input, para possíveis mensagens de erro.
    begin_token();
    // Vamos tokenizar o inteiro, se ele for bem formado.
    if ( integer() ) {
        // Copiar a substring correspondente para uma variável string.
        std::string token  = complete_token();
        // Tentar realizar a conversão de string para inteiro (usar stoll()).
        input_int_type token_value;
        token_value = stoll( token );

        // Recebemos um inteiro válido, resta saber se está dentro da faixa.
        if ( token_value < std::numeric_limits< required_int_type >::min() or
             token_value > std::numeric_limits< required_int_type >::max() ) {
            // Fora da faixa, reportar erro.
            m_result = ResultType{ ResultType::INTEGER_OUT_OF_RANGE, token_location() }; 
                               // std::distance( m_expr.begin(), begin_token ) );
        }
        else {
            // Coloca o novo token na nossa lista de tokens.
            m_tk_list.emplace_back( Token{ token, Token::token_t::OPERAND } );
        }
    }
    else if ( *m_it_curr_symb == ')' or *m_it_curr_symb == ')' ) {
        // Copiar a substring correspondente para uma variável string.
        // std::string token  = complete_token();
        // // Tentar realizar a conversão de string para inteiro (usar stoll()).
        // input_int_type token_value;
        // token_value = stoll( token );
        if (*m_it_curr_symb == '(') {
            m_tk_list.emplace_back( Token{ "(", Token::token_t::OPEN_PARENTHESIS } );
        } else if (*m_it_curr_symb == ')') {
            m_tk_list.emplace_back( Token{  ")", Token::token_t::CLOSE_PARENTHESIS } );
        }
        
    }
    else {
        // Create the corresponding error.
        m_result =  ResultType{ ResultType::ILL_FORMED_INTEGER, std::distance( m_expr.begin(), m_it_curr_symb ) } ;
    }
    return m_result.type == ResultType::OK;
}

/// Validates (i.e. returns true or false) and consumes an **integer** from the input expression string.
/*! This method parses a valid integer from the input and, at the same time, add the integer to the token list.
 *
 * Production rule is:
 * ```
 * <integer> := 0 | ["-"],<natural_number>;
 * ```
 * A integer might be a zero or a natural number, which, in turn, might begin with an unary minus.
 *
 * @return true if an integer has been successfuly parsed from the input; false otherwise.
 */
bool Parser::integer( void ) {
    // Se aceitarmos um zero, então o inteiro acabou aqui.
    if ( accept( terminal_symbol_t::TS_ZERO ) )
        return true; // OK
    // Vamos tentar aceitar o '-'.
    accept( terminal_symbol_t::TS_MINUS );
    // Retonar o resultado da tentativa de validar um numero natural.
    return  natural_number();
}

/// Validates (i.e. returns true or false) and consumes a **natural number** from the input string.
/*! This method parses a valid natural number from the input.
 *
 * Production rule is:
 * ```
 * <natural_number> := <digit_excl_zero>,{<digit>};
 * ```
 *
 * @return true if a natural number has been successfuly parsed from the input; false otherwise.
 */
bool Parser::natural_number( void ) {
    // Tem que vir um número que não seja zero! (de acordo com a definição).
    if ( not digit_excl_zero() )
        return false; // FAILED HERE.
    // Cosumir os demais dígitos, se existirem...
    while( digit() ) /* empty */ ;
    //
    return true; // OK
}

/// Validates (i.e. returns true or false) and consumes a **non-zero digit** from the input expression string.
/*! This method parses a single valid non-zero digit from the input.
 *
 * Production rule is:
 * ```
 * <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 * ```
 *
 * @return true if a non-zero digit has been successfuly parsed from the input; false otherwise.
 */
bool Parser::digit_excl_zero( void ) {
    return accept( terminal_symbol_t::TS_NON_ZERO_DIGIT );
}

/// Validates (i.e. returns true or false) and consumes a **digit** from the input expression string.
/*! This method parses a single valid digit from the input.
 *
 * Production rule is:
 * ```
 * <digit> := "0"| <digit_excl_zero>;
 * ```
 *
 * @return true if a digit has been successfuly parsed from the input; false otherwise.
 */
bool Parser::digit( void ) {
    return ( accept( terminal_symbol_t::TS_ZERO ) or digit_excl_zero() ) ? true : false;
}

/*!
 * This is the parser's  **entry point** method that should be called in the client code.
 * This method tries to (recursivelly) validate an expression.
 * During this process, we also store the tokens into a container.
 *
 m e_ The string with the expression to parse.
 * \return The parsing result.
 *
 * @see ResultType
 */
Parser::ResultType  Parser::parse_and_tokenize( std::string e_ ) {
    m_expr = e_; //  Stores the input expression in the private class member.
    m_it_curr_symb = m_expr.begin(); // Defines the first char to be processed (consumed).
    m_begin_token = m_it_curr_symb;
    m_result = ResultType{ ResultType::OK }; // Ok, by default,

    // We alway clean up the token from (possible) previous processing.
    m_tk_list.clear();

    // Let us ignore any leading white spaces.
    skip_ws();
    if ( end_input() ) { // Premature end?
        // Input has finished before we even started to parse...
        m_result =  ResultType{ ResultType::UNEXPECTED_END_OF_EXPRESSION,
                                std::distance( m_expr.begin(), m_it_curr_symb ) };
    }
    else {
        // Trying to validate an expression.
        if ( expression() ) {
            // At this point there should not be any non-whitespace character in the input expression.
            skip_ws(); // Anyway, let us clear any remaining 'whitespaces'.
            if ( not end_input() ) {// Is there still any residual symbol left in the string?
                m_result =  ResultType{ ResultType::EXTRANEOUS_SYMBOL,
                                        std::distance( m_expr.begin(), m_it_curr_symb ) };
            }
        }
    }
    return m_result;
}

void Parser::begin_token(void) {
    skip_ws();
    // Marke the begining of the token, so we can copy it later over to the vector of tokens.
    m_begin_token = m_it_curr_symb;
}

std::string Parser::complete_token(void) {
    std::string token_str;
    std::copy( m_begin_token, m_it_curr_symb, std::back_inserter( token_str ) );
    return token_str;
}

Parser::ResultType::size_type Parser::token_location(void) {
    return std::distance( m_expr.begin(), m_begin_token );
}

/*!
 * Return the list of tokens, which is the by-product created during the syntax analysis.
 * This method should be called in the cliente code **after** tha parser has
 * returned successfuly.
 */
std::vector< Token >
Parser::get_tokens( void ) const {
    return m_tk_list;
}

// Function to return precedence of operators
int Parser::prec(std::string c) {
    if(c == "^")
        return 3;
    else if(c == "/" || c == "*" || c == "%")
        return 2;
    else if(c == "+" || c == "-")
        return 1;
    else
        return -1;
}

// The main function to convert infix expression
// to postfix expression
void Parser::infixToPostfix(void) {
    std::stack<Token> st; // For stack operations, we are using C++ built in stack
    // std::string result;
    std::vector<Token> pf_tk_list;

    for (size_t i{0}; i < m_tk_list.size(); i++) {
        Token c = m_tk_list[i];

        // If the scanned character is
        // an operand, add it to output string.
        if (c.type == Token::token_t::OPERAND)
            pf_tk_list.push_back(c);
            // result += c.value;

        // If the scanned character is an
        // ‘(‘, push it to the stack.
        else if (c.value == "(")
            st.push(c);

        // If the scanned character is an ‘)’,
        // pop and to output string from the stack
        // until an ‘(‘ is encountered.
        else if (c.value == ")") {
            while (st.top().value != "(")
            {
                pf_tk_list.push_back(st.top());
                // result += st.top().value;
                st.pop();
            }
            st.pop();
        }

        //If an operator is scanned
        else {
            while (not st.empty() and prec(m_tk_list[i].value) <= prec(st.top().value)) {
                pf_tk_list.push_back(st.top());
                // result += st.top().value;
                st.pop();
            }
            st.push(c);
        }
    }

    // Pop all the remaining elements from the stack
    while (not st.empty()) {
        pf_tk_list.push_back(st.top());
        // result += st.top().value;
        st.pop();
    }

    m_tk_list = pf_tk_list;

    // std::cout << result << std::endl;
}

// Function that calculates the postfix expression
void Parser::calculate(void) {
    std::stack<input_int_type> st; // The stack to store the operands.
    input_int_type result; // The result of expression;

    // Travels the tokens to calculate the expression.
    for (size_t i{0}; i < m_tk_list.size(); i++) {
        Token c = m_tk_list[i];
        // transform std::string to char[]
        // to use atoll and switch
        size_t sz = c.value.length();
        // declaring character array
        char c_value[sz + 1];
        // copying the contents of the
        // string to char array
        std::strcpy(c_value, c.value.c_str());

        // If it is an operand, transform in int and push on the stack.
        if (c.type == Token::token_t::OPERAND) {
            // transform in int and push on the stack.
            input_int_type value = std::atoll(c_value);
            st.push(value);
        }
        else if(c.type == Token::token_t::CLOSE_PARENTHESIS) {
            st.pop();
            while(c.type == Token::token_t::OPEN_PARENTHESIS) {
                input_int_type second_operand = st.top();
                st.pop();
                input_int_type first_operand = st.top();
                st.pop();
                // Decide the operation that will be made.
                switch (c_value[0]) {
                    case '+':  result = first_operand + second_operand; break;
                    case '-':  result = first_operand - second_operand; break;
                    case '*':  result = first_operand * second_operand; break;
                    case '/':  result = first_operand / second_operand; break;
                    case '%':  result = first_operand % second_operand; break;
                    case '^':
                        // Calculate the exception of x^0 = 1
                        if (second_operand == 0) {
                            result = 1;
                        }
                        else {
                            input_int_type expo = first_operand;
                            while (second_operand != 1) {
                                expo *= first_operand;
                                second_operand--;
                            }
                            result = expo;
                        }
                }
                // Insert the result on the top of stack.
                st.push(result);
                i++;
                Token c = m_tk_list[i];
                // transform std::string to char[]
                // to use atoll and switch
                size_t sz = c.value.length();
                // declaring character array
                char c_value[sz + 1];
                // copying the contents of the
                // string to char array
                std::strcpy(c_value, c.value.c_str());
            }
            st.pop();
        }
        // If it is an operator, pop twice on stack and calculate the expression.
        else {
            // Take from stack the two values that will be calculated.
            input_int_type second_operand = st.top();
            st.pop();
            input_int_type first_operand = st.top();
            st.pop();
            // Decide the operation that will be made.
            switch (c_value[0]) {
                case '+':  result = first_operand + second_operand; break;
                case '-':  result = first_operand - second_operand; break;
                case '*':  result = first_operand * second_operand; break;
                case '/':  result = first_operand / second_operand; break;
                case '%':  result = first_operand % second_operand; break;
                case '^':
                    // Calculate the exception of x^0 = 1
                    if (second_operand == 0) {
                        result = 1;
                    }
                    else {
                        input_int_type expo = first_operand;
                        while (second_operand != 1) {
                            expo *= first_operand;
                            second_operand--;
                        }
                        result = expo;
                    }
            }
            // Insert the result on the top of stack.
            st.push(result);
        }
    }
    // Case of one operand is passed.
    if (st.size() == 1) {
        result = st.top();
        st.pop();
    }

    std::cout << "Result: " << result << std::endl;
}

//==========================[ End of parse.cpp ]==========================//
