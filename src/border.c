#include "border.h"
#include "color.h"
#include <string.h>
#include "log.h"

void xlsx_parse_border(xlsxBorder *b, ezxml_t xml)
{
#ifdef DEBUG
	LOG("%s", ezxml_toxml(xml));
#endif
	const char * style = ezxml_attr(xml, "style");
	if (style)
		strncpy(b->style, style, sizeof(b->style)-1);
	
	ezxml_t color = ezxml_child(xml, "color");
	if (color)
		xlsx_parse_color(&b->color, color);
}
