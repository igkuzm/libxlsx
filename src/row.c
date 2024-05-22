#include "row.h"
#include "array.h"
#include "ezxml.h"
#include "alloc.h"
#include <stdlib.h>
#include <string.h>
#include "cell.h"
#include "font.h"
#include "log.h"

void xlsx_parse_row(xlsxRow *r, ezxml_t row, 
		xlsxWorkBook *wb)
{
#ifdef DEBUG
	LOG("parse row"); 
#endif
	r->height = XLSX_DEF_ROW_HEIGHT;

	//get row ref
	const char * ref = ezxml_attr(row, "r");
	strncpy(r->ref, ref, sizeof(r->ref) - 1);

	//get row hidden
	const char * hidden = ezxml_attr(row, "hidden");
	if (hidden)
		r->hidden = atoi(hidden);

	//get row level
	const char * level = ezxml_attr(row, "level");
	if (level)
		r->level = atoi(level);			

	//get row level
	const char * collapsed = ezxml_attr(row, "collapsed");
	if (collapsed)
		r->collapsed = atoi(collapsed);			
	
	//get row spans
	const char * spans = ezxml_attr(row, "spans");
	if (spans){
	}

	const char * customHeight = ezxml_attr(row, "customHeight");
	if (customHeight && atoi(customHeight)){
		const char * ht = ezxml_attr(row, "ht");
		r->height = atof(ht);			
	}			

	//thick bottom
	const char * thickBot = ezxml_attr(row, "thickBot");
	if (thickBot)
		r->thickBot = atof(thickBot);			

	//thick top
	const char * thickTop = ezxml_attr(row, "thickTop");
	if (thickTop)
		r->thickTop = atof(thickTop);			

	//??
	const char * dyDescent = ezxml_attr(row, "x14ac:dyDescent");
	if (dyDescent){
	}

	//get cell
#ifdef DEBUG
	LOG("parse cells"); 
#endif
	ezxml_t cell = ezxml_child(row, "c");
	array_t *a = array_new(xlsxCell*,
		 	ERR("array_new"); return);
	
	for(; cell; cell = cell->next){
		// allocate cell
		xlsxCell *c = MALLOC(sizeof(xlsxCell), 
				ERR("malloc"); return);
		
		xlsx_parse_cell(c, cell, wb);
		
		array_append(a, xlsxCell*, c, 
				ERR("array_append"); return);
	}
#ifdef DEBUG
	LOG("parse cells - done"); 
#endif
	
	r->cells = a->data;
	r->ncells = a->len;
	free(a);

#ifdef DEBUG
	LOG("parse row - done"); 
#endif
}
