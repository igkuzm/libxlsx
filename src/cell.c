#include "cell.h"
#include "border.h"
#include "color.h"
#include "font.h"
#include "alignment.h"
#include <string.h>
#include "cell_value.h"

void xlsx_parse_cell(xlsxCell *c, ezxml_t cell, 
		xlsxWorkBook *wb)
{
	//cell ref 
	const char * ref = ezxml_attr(cell, "r");
	strncpy(c->ref, ref, sizeof(c->ref) - 1);
	
	//parse cell style
	const char * s = ezxml_attr(cell, "s");
	if (s && wb->styles) {
		ezxml_t xf = ezxml_get(wb->styles, "cellXfs", 0, "xf", atoi(s), "");
		if (xf){
			const char * applyNumberFormat = ezxml_attr(xf, "applyNumberFormat");
			if (applyNumberFormat && atoi(applyNumberFormat)){
				const char * numFmtId = ezxml_attr(xf, "numFmtId");
				if (numFmtId)
					//set fmt
					c->numFmtId = atoi(numFmtId);
			}
			const char * applyFill = ezxml_attr(xf, "applyFill");
			if (applyFill && atoi(applyFill)){
				const char * fillId = ezxml_attr(xf, "fillId");
				if (fillId){
					//set fill
					ezxml_t patternFill = ezxml_get(wb->styles, "fills", 0, "fill", atoi(fillId), "patternFill", 0, "");
					if (patternFill){
						ezxml_t fgColor = ezxml_child(patternFill, "fgColor");
						if (fgColor)
							xlsx_parse_color(&c->fg, fgColor);
						
						ezxml_t bgColor = ezxml_child(patternFill, "bgColor");
						if (bgColor)
							xlsx_parse_color(&c->fg, bgColor);

						const char * patternType = ezxml_attr(patternFill, "patternType");
						if (patternType)
							strncpy(c->pattern, patternType, sizeof(c->pattern)-1);
					}
				}						
			}						
			const char * xfId = ezxml_attr(xf, "xfId");
			if (xfId){
				//set xf
			}						
			const char * applyProtection = ezxml_attr(xf, "applyProtection");
			if (applyProtection){
				//cell protect
			}						
			const char * applyFont = ezxml_attr(xf, "applyFont");
			if (applyFont && atoi(applyFont)){
				//set applyFont
				const char * fontId = ezxml_attr(xf, "fontId");
				if (fontId && wb->styles){
					//set font
					ezxml_t font = ezxml_get(wb->styles, "fonts", 0, "font", atoi(fontId), "");
					if (font)
						xlsx_parse_font(&c->font, font);
				}						
			}						
			const char * applyBorder = ezxml_attr(xf, "applyBorder");
			if (applyBorder && atoi(applyBorder)){
				//set applyBorder
				const char * borderId = ezxml_attr(xf, "borderId");
				if (borderId && wb->styles){
					//set border
					ezxml_t border = ezxml_get(wb->styles, "borders", 0, "border", atoi(borderId), ""); 
					if (border){
						ezxml_t left = ezxml_child(border, "left");
						if (left)
							xlsx_parse_border(&c->left, left);
						
						ezxml_t right = ezxml_child(border, "right");
						if (right) 
							xlsx_parse_border(&c->right, right);
															
						ezxml_t top = ezxml_child(border, "top");
						if (top) 
							xlsx_parse_border(&c->top, top);
																								
						ezxml_t bottom = ezxml_child(border, "bottom");
						if (bottom) 
							xlsx_parse_border(&c->bottom, bottom);
																																
						ezxml_t diagonal = ezxml_child(border, "diagonal");
						if (diagonal) 
							xlsx_parse_border(&c->diagonal, diagonal);
															
					}
				}						
			}						
			const char * applyAlignment = ezxml_attr(xf, "applyAlignment");
			if (applyAlignment){
				//set applyAlignment
				ezxml_t alignment = ezxml_child(xf, "alignment");
				if (alignment)
					xlsx_parse_alignment(&c->alignment, alignment);
			}						
		}
	}

	// parse type and value
	xlsx_parse_cell_value(c, cell, wb);
}
