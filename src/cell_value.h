#ifndef XLSX_CELL_VALUE_H
#define XLSX_CELL_VALUE_H
#include "../libxlsx.h"
#include "ezxml.h"

void xlsx_parse_cell_value(xlsxCell *c, ezxml_t xml, 
		xlsxWorkBook *wb);

#endif /* ifndef XLSX_CELL_VALUE_H */
