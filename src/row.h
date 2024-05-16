#ifndef XLSX_ROW_H
#define XLSX_ROW_H
#include "../libxlsx.h"
#include "ezxml.h"

void xlsx_parse_row(xlsxRow *r, ezxml_t xml, 
		xlsxWorkBook *wb);

#endif /* ifndef XLSX_ROW_H */
