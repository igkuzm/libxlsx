#include "font.h"
#include "color.h"
#include <string.h>
#include "log.h"

void xlsx_parse_font(xlsxFont *f, ezxml_t font)
{
#ifdef DEBUG
	LOG(ezxml_toxml(font));
#endif
	ezxml_t name = ezxml_child(font, "name");
	if (name){
		const char * val = ezxml_attr(name, "val");
		if(val)
			strncpy(f->name, val, sizeof(f->name) - 1);
	}
	ezxml_t scheme = ezxml_child(font, "scheme");
	if (scheme){
		const char * val = ezxml_attr(scheme, "val");
		if(val)
			strncpy(f->scheme, val, sizeof(f->scheme) - 1);
	}
	ezxml_t rFont = ezxml_child(font, "rFont");
	// name for formated string font
	if (rFont){
		const char * val = ezxml_attr(rFont, "val");
		if(val)
			strncpy(f->name, val, sizeof(f->name) - 1);
	}
	ezxml_t color = ezxml_child(font, "color");
	if (color)
		xlsx_parse_color(&f->color, color);

	ezxml_t family = ezxml_child(font, "family");
	if (family){
		const char * val = ezxml_attr(family, "val");
		if(val)
			f->family = atoi(val);
	}									
	ezxml_t charset = ezxml_child(font, "charset");
	if (charset){
		const char * val = ezxml_attr(charset, "val");
		if(val)
			f->charset = atoi(val);
	}									
	ezxml_t sz = ezxml_child(font, "sz"); //size
	if (sz){
		const char * val = ezxml_attr(sz, "val");
		if(val)
			f->size = atof(val);
	}									
	ezxml_t b = ezxml_child(font, "b"); //bold
	if (b)
		f->bold = true;
	
	ezxml_t i = ezxml_child(font, "i"); //italic
	if (i)
		f->italic = true;
	
	ezxml_t outline = ezxml_child(font, "outline"); //outline
	if (outline)
		f->outline = true;
	
	ezxml_t strike = ezxml_child(font, "strike"); //strike
	if (strike)
		f->strike = true;
	
	ezxml_t shadow = ezxml_child(font, "shadow"); //shadow
	if (shadow)
		f->shadow = true;
}
