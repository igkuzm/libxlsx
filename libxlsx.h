#ifdef __cplusplus
extern "C" {
#endif
#ifndef LIB_XLSX_H
#define LIB_XLSX_H

#include <stdio.h>
#include <stdbool.h>

typedef struct xlsx_workbook xlsxWorkBook;
/* read xlsx file and get workbook */
xlsxWorkBook *xlsx_open_file(const char *file);

/* close file and free memory */
void xlsx_close_workbook(xlsxWorkBook* wb);

typedef struct xlsx_worksheet xlsxWorkSheet;
/* get worksheet */
xlsxWorkSheet * xlsx_get_worksheet(xlsxWorkBook* wb, 
		int num);
/* free worksheet memory */
void xlsx_close_worksheet(xlsxWorkSheet* ws);

typedef struct xlsx_row xlsxRow;
/* get row with number */
xlsxRow *xlsx_row(xlsxWorkSheet* ws, int cellRow);

typedef struct xlsx_cell xlsxCell;
/* get cell with row and column number */
xlsxCell	*xlsx_cell(xlsxWorkSheet* ws, int cellRow, int cellCol);

typedef struct xlsx_color {
	int rgb;
	int indexed;
	int theme;
} xlsxColor;

typedef struct xlsx_alignment {
	char horizontal[32];
	char vertical[32];
	bool wrap;
} xlsxAlignment;

typedef struct xlsx_font {
	char name[32];
	char scheme[32];
	xlsxColor color;
	int family;
	int charset;
	double size;
	bool bold;
	bool italic;
	bool outline;
	bool strike;
	bool shadow;
} xlsxFont;

typedef struct xlsx_border {
  char style[32];	
	xlsxColor color;
} xlsxBorder;

typedef struct xlsx_formated_string {
	xlsxFont font;
	char *string;
	struct xlsx_formated_string *next;
} xlsxFormatedString;

struct xlsx_cell {
	char ref[32];
	char *value;
	char *formula;
	xlsxFormatedString *fsting;
	int numFmtId;
	xlsxColor fg;
	xlsxColor bg;
	char pattern[32];
	xlsxFont font;	
	xlsxBorder left;
	xlsxBorder right;
	xlsxBorder top;
	xlsxBorder bottom;
	xlsxBorder diagonal;
	xlsxAlignment alignment;
};

#define XLSX_DEF_ROW_HEIGHT 15
struct xlsx_row {
	xlsxCell **cells;
	int ncells;
	char ref[32];
	double height;
	bool hidden;
	int level;
	bool collapsed;
	double thickBot;
	double thickTop;
};

#define XLSX_DEF_COL_WIDTH 8.43
typedef struct xlsx_col {
	double width;
	bool hidden;
	int level;
	bool collapsed;
} xlsxCol;

typedef struct xlsx_merged_cell {
	int first_row;
	int first_col;
	int last_row;
	int last_col;
} xlsxMergedCell;

static void 
_xlsx_merge_range_cells(xlsxMergedCell *mc, int fr, int fc, int lr, int lc){
	mc->first_row = fr;
	mc->first_col = fc;
	mc->last_row  = lr;
	mc->last_col  = lc;
}

struct xlsx_worksheet {
	void *xml;
	xlsxRow **rows;
	int nrows;
	xlsxCol **cols;
	int ncols;
	xlsxMergedCell **mergedCells;
	int nmergedCells;
	float margin_top;
	float margin_bottom;
	float margin_right;
	float margin_left;
	float margin_header;
	float margin_footer;
	int paperSize;
	int printScale;
	int fitToWidth;
	int fitToHeight;
	int horizontal_dpi;
	int vertical_dpi;
	bool pageOrder;
	char orientation[32];
	bool blackAndWhite;
	int draft;
};

struct xlsx_workbook {
	int nsheets;
	void *zip;
	void *workbook;
	void *sharedStrings;
	void *styles;
};

#endif // ifndef LIB_XLSX_H 
#ifdef __cplusplus
}  // end of the 'extern "C"' block
#endif
