#ifndef _BARESMANAGER_H_
#define _BARESMANAGER_H_

#include "parser.h"

class BaresManager {
    public:
        void print_error_msg( const Parser::ResultType & result, const std::string &str );
        void read_and_compute(std::string expr);
        int prec(std::string c);
        void infixToPostfix(void);
        void calculate(void);
    private:
        Parser::ResultType m_result;
        std::vector<Token> tokens;
        Parser::required_int_type final_result;
};

#endif
