#include "alignment.h"
#include <string.h>

void xlsx_parse_alignment(xlsxAlignment *a, ezxml_t alignment)
{
	const char * horizontal = ezxml_attr(alignment, "horizontal");
	if (horizontal)
		strncpy(a->horizontal, horizontal, sizeof(a->horizontal)-1);
		
	const char * vertical = ezxml_attr(alignment, "vertical");
	if (vertical)
		strncpy(a->vertical, vertical, sizeof(a->vertical)-1);
	
	const char * wrapText = ezxml_attr(alignment, "wrapText");
	if (wrapText && atoi(wrapText))
		a->wrap = true;
}
