#include "color.h"

void xlsx_parse_color(xlsxColor *c, ezxml_t xml)
{
	const char * rgb = ezxml_attr(xml, "rgb");
	if (rgb)
		sscanf(rgb, "%x", &c->rgb);

	const char * indexed = ezxml_attr(xml, "indexed");
	if (indexed)
		c->indexed = atoi(indexed);
	
	const char * theme = ezxml_attr(xml, "theme"); 
	if (theme)
		c->theme = atoi(theme);
}
