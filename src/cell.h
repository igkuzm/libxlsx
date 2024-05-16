#ifndef XLSX_CELL_H
#define XLSX_CELL_H
#include "../libxlsx.h"
#include "ezxml.h"

void xlsx_parse_cell(xlsxCell *c, ezxml_t xml, 
		xlsxWorkBook *wb);

#endif /* ifndef XLSX_CELL_H */
