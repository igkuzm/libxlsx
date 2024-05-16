# C library to read XLSX tables

```c
xlsxWorkBook *wb = xlsx_open_file("my_file.xlsx");
if (wb){
	xlsxWorkSheet *ws = xlsx_get_worksheet(wb, 1);
	if (ws){
		for (int i=0; i < ws->nrows; ++i){
			xlsxRow *row = ws->rows[i];
			for (int k=0; k < row->ncells; ++k){
				xlsxCell *cell = row->cells[k];
				printf("%s\t", cell->value);
			}
			printf("\n");
		}
		xlsx_close_worksheet(ws);
	}

	xlsx_close_workbook(wb);
}
```
