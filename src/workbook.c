#include "ezxml.h"
#include <zip.h>
#include "zip_entry_read.h"
#include "../libxlsx.h"
#include "safe_malloc.h"

xlsxWorkBook *xlsx_open_file(const char *file)
{
	int err = 0;
	size_t size;
	char *buffer;
	struct zip_stat st;
	zip_file_t *f; 
	
	// try to open zip file
	zip_t *zip = zip_open(file, ZIP_RDONLY, &err);
	if (err){
		perror("extracting zip archive");
		return NULL;
	}
	
	// try to load workbook.xml
	if (zip_entry_read(zip, "xl/workbook.xml",
				&buffer, &size))
	{
		perror("file is not XLSX format");
		zip_close(zip);
		return NULL;
	}

	xlsxWorkBook *wb = 
		MALLOC(sizeof(xlsxWorkBook), 
				zip_close(zip);
				return NULL);

	// read workbook
	wb->workbook = ezxml_parse_str(buffer, size);
	free(buffer);
	if (!wb->workbook){
		perror("xl/workbook.xml is damaged");
		zip_close(zip);
		return NULL;
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

	// load styles
	wb->styles = NULL;
	if (!zip_entry_read(zip, "xl/styles.xml",
				&buffer, &size))
	{
		wb->styles = ezxml_parse_str(buffer, size);
		free(buffer);
	}

	// load sharedStrings
	wb->sharedStrings = NULL;
	if (!zip_entry_read(zip, "xl/sharedStrings.xml",
				&buffer, &size))
	{
		wb->sharedStrings = ezxml_parse_str(buffer, size);
		free(buffer);
	}

	wb->zip = zip;

	return wb;	
}

void xlsx_close_workbook(xlsxWorkBook* wb){
	zip_close(wb->zip);
	free(wb);
}
