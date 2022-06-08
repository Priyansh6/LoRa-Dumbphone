#ifndef BRANCH_H
#define BRANCH_H

#include "../utilities.h"
#include "symboltable.h"

word assemble_B(token_t t, symbol_table_t *st, address inst_addr);

#endif
