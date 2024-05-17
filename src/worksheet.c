#include "../libxlsx.h"
#include "ezxml.h"
#include "safe_malloc.h"
#include "zip_entry_read.h"
#include <string.h>
#include <stdbool.h>
#include "helpers.h"
#include "row.h"

static int 
parse_worksheet(xlsxWorkBook *wb, xlsxWorkSheet *ws, int num)
{
	int i;

	char sheetxml[32];
	sprintf(sheetxml, "xl/worksheets/sheet%d.xml", num);

	void *buffer;
	size_t size;
	if (zip_entry_read(wb->zip, sheetxml,
				&buffer, &size))
			return -1;
	
	ezxml_t xml = ezxml_parse_str(buffer, size);
	free(buffer);
	if (!xml)
		return -1;

	ws->xml = xml;
	
	/*! TODO: add sheet properties
	 *  \todo add sheet properties
	 */

	ezxml_t data = ezxml_child(xml, "sheetData");
	if (!data)
		return -1;

	//pageMargins
	ezxml_t pageMargins = ezxml_child(xml, "pageMargins");
	if (pageMargins){
		const char * left = ezxml_attr(pageMargins, "left");			
		const char * right = ezxml_attr(pageMargins, "right");			
		const char * top = ezxml_attr(pageMargins, "top");			
		const char * bottom = ezxml_attr(pageMargins, "bottom");			
		const char * header = ezxml_attr(pageMargins, "header");			
		const char * footer = ezxml_attr(pageMargins, "footer");			
		if (left)
			ws->margin_left = atof(left);
		if (right)
			ws->margin_right = atof(right);
		if (top)
			ws->margin_top = atof(top);
		if (bottom)
			ws->margin_bottom = atof(bottom);
		if (header)
			ws->margin_header = atof(header);
		if (footer)
			ws->margin_footer = atof(footer);
	}
			
	//pageSetup
	ezxml_t pageSetup = ezxml_child(xml, "pageSetup");
	if (pageSetup){
		const char * paperSize = ezxml_attr(pageSetup, "paperSize");			
		if (paperSize)
			ws->paperSize = atoi(paperSize);
		
		const char * scale = ezxml_attr(pageSetup, "scale");			
		if (scale)
			ws->printScale = atoi(scale);

		const char * fitToWidth = ezxml_attr(pageSetup, "fitToWidth");			
		if (fitToWidth)
			ws->fitToWidth = atoi(fitToWidth);
		
		const char * fitToHeight = ezxml_attr(pageSetup, "fitToHeight");			
		if (fitToHeight)
			ws->fitToHeight = atoi(fitToHeight);
		
		const char * horizontalDpi = ezxml_attr(pageSetup, "horizontalDpi");			
		if (horizontalDpi)
			ws->horizontal_dpi = atoi(horizontalDpi);

		const char * verticalDpi = ezxml_attr(pageSetup, "verticalDpi");			
		if (verticalDpi)
			ws->vertical_dpi = atoi(verticalDpi);
		
		const char * pageOrder = ezxml_attr(pageSetup, "pageOrder");			
		if (pageOrder)
			if (strcmp(pageOrder, "overThenDown") == 0)
				ws->pageOrder = 1;
		
		const char * orientation = ezxml_attr(pageSetup, "orientation");			
		if (orientation)
			strncpy(ws->orientation, orientation, sizeof(ws->orientation)-1);
		
		const char * blackAndWhite = ezxml_attr(pageSetup, "blackAndWhite");			
		if (blackAndWhite && atoi(blackAndWhite))
			ws->blackAndWhite = 1;
		
		const char * draft = ezxml_attr(pageSetup, "draft");			
		if (draft)
			ws->draft = atoi(draft);
	}
	
	//parse cols 
	ezxml_t cols = ezxml_child(xml, "cols");
	ezxml_t col = ezxml_child(cols, "col");
	ws->cols = MALLOC(1, return -1);
	for(;col; col = col->next){
		// allocate column
		xlsxCol *column = MALLOC(sizeof(xlsxCol), return -1);
		
		// realloc columns
		ws->cols = 
			REALLOC(column, (1+i)*sizeof(xlsxCol*), return  -1;);
		ws->cols[ws->ncols++] = column;	

		column->width = XLSX_DEF_COL_WIDTH;
		
		//get col hidden
		const char * hidden = ezxml_attr(col, "hidden");
		if (hidden)
			column->hidden = atoi(hidden);
		
		//get col level
		const char * level = ezxml_attr(col, "level");
		if (level)
			column->level = atoi(level);			
		
		//get col collapsed
		const char * collapsed = ezxml_attr(col, "collapsed");
		if (collapsed)
			column->collapsed = atoi(collapsed);			
					
		const char * min = ezxml_attr(col, "min");
		const char * max = ezxml_attr(col, "max");
		const char * bestFit = ezxml_attr(col, "bestFit");

		const char * customWidth = ezxml_attr(col, "customWidth");
		if (customWidth && atoi(customWidth)){
			const char * width = ezxml_attr(col, "width");
			if (width)
				column->width = atof(width);
		}
	}

	//merge cells
	ezxml_t mergeCells = ezxml_child(xml, "mergeCells");
	ezxml_t mergeCell =  ezxml_child(mergeCells, "mergeCell");
	ws->mergedCells = MALLOC(1, return -1);
	for (; mergeCell; mergeCell = mergeCell->next) {
		// allocate merged cell
		xlsxMergedCell *mc = MALLOC(sizeof(xlsxMergedCell), return -1);
		
		// realloc columns
		ws->mergedCells = 
			REALLOC(ws->mergedCells, (1+i)*sizeof(xlsxMergedCell*), return  -1;);
		ws->mergedCells[ws->nmergedCells++] = mc;	

		const char * ref = ezxml_attr(mergeCell, "ref");
		if (ref){
			_xlsx_merge_range_cells(mc, RANGE(ref));
		}
	}

	//parse row
	int addrow = 0, rowindex = 0;
	ezxml_t prev_row = NULL;
	ezxml_t row = ezxml_child(data, "row");
	ws->rows = MALLOC(1, return -1);
	for(;row; prev_row = row, row = row->next, rowindex++)
	{
		// allocate row
		xlsxRow *r = MALLOC(sizeof(xlsxRow), return -1);
	
		// realloc rows
		ws->rows = 
			REALLOC(ws->rows, (1+i)*sizeof(xlsxRow*), return  -1;);
		ws->rows[ws->nrows++] = r;	
		
		xlsx_parse_row(r, row, wb->styles);
	}	
		
	return 0;
}

xlsxWorkSheet * 
xlsx_get_worksheet(xlsxWorkBook* wb, int num)
{
	if (num < 0 || num > wb->nsheets)
		return NULL;

	xlsxWorkSheet *ws = 
		MALLOC(sizeof(xlsxWorkSheet), return NULL);
	memset(ws, 0, sizeof(xlsxWorkSheet));

	// parse worksheet
	parse_worksheet(wb, ws, num);
	
	return ws;
}


void xlsx_close_worksheet(xlsxWorkSheet* ws){
	if (!ws)
		return;
	int i, k;
	for (i = 0; i < ws->nrows; ++i) {
		xlsxRow *row;
		for (k = 0; k < row->ncells; ++k) {
			xlsxCell *cell;
			free(cell->value);
			free(cell);
		}
		free(row->cells);
		free(row);
	}
	free(ws->rows);
	if (ws->xml)
		ezxml_free(ws->xml);
	free(ws);
}
