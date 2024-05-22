#include "ezxml.h"
#include <zip.h>
#include "zip_entry_read.h"
#include "../libxlsx.h"
#include "alloc.h"
#include "array.h"
#include "log.h"

xlsxWorkBook *xlsx_open_file(const char *file)
{
#ifdef DEBUG
	LOG("open file: %s", file);
#endif

	int err = 0;
	zip_file_t *f; 
	
	// try to open zip file
	zip_t *zip = zip_open(file, ZIP_RDONLY, &err);
	if (err){
		ERR("extracting zip archive");
		return NULL;
	}
	
	xlsxWorkBook *wb = 
		MALLOC(sizeof(xlsxWorkBook), 
				ERR("workbook malloc");
				zip_close(zip);
				return NULL);

	// try to load workbook.xml
	{
		size_t size;
		if (zip_entry_read(zip, "xl/workbook.xml",
					&wb->workbookb, &size))
		{
			ERR("file is not XLSX format");
			zip_close(zip);
			return NULL;
		}

#ifdef DEBUG
	LOG("parse workbook"); 
#endif
		// read workbook
		wb->workbook = ezxml_parse_str(wb->workbookb, size);
		if (!wb->workbook){
			ERR("xl/workbook.xml is damaged");
			zip_close(zip);
			return NULL;
		}
	}

	// get number of sheets
	wb->nsheets = 0;
	ezxml_t sheets = 
		ezxml_child(wb->workbook, "sheets");
	if (sheets){
		ezxml_t s;
		for (s = ezxml_child(sheets, "sheet"); 
				s; s = s->next)
			wb->nsheets++;
	}		

#ifdef DEBUG
	LOG("workbook has %d sheets", wb->nsheets);
#endif

	// load styles
	{
		size_t size;
		wb->styles = NULL;
		if (!zip_entry_read(zip, "xl/styles.xml",
					&wb->stylesb, &size))
			wb->styles = ezxml_parse_str(wb->stylesb, size);
	}
	 
	// load sharedStrings
	{
		size_t size;
		wb->sharedStrings = NULL;
		if (!zip_entry_read(zip, "xl/sharedStrings.xml",
					&wb->sharedStringsb, &size))
			wb->sharedStrings = ezxml_parse_str(wb->sharedStringsb, size);
	}

	wb->zip = zip;

	return wb;	
}

void xlsx_close_workbook(xlsxWorkBook* wb){
#ifdef DEBUG
	LOG("close workbook");
#endif
	if (!wb)
		return;
	if (wb->zip)
		zip_close(wb->zip);
	if (wb->workbook)
		ezxml_free(wb->workbook);
	if (wb->workbookb)
		free(wb->workbookb);
	if (wb->sharedStrings)
		ezxml_free(wb->sharedStrings);
	if (wb->sharedStringsb)
		free(wb->sharedStringsb);
	if (wb->styles)
		ezxml_free(wb->styles);
	if (wb->stylesb)
		free(wb->stylesb);
	free(wb);
}
