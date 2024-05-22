#include "cell_value.h"
#include <string.h>
#include "ezxml.h"
#include "font.h"
#include "alloc.h"
#include "str.h"
#include "log.h"
#include "array.h"

void xlsx_parse_cell_value(xlsxCell *c, ezxml_t cell, 
		xlsxWorkBook *wb)
{
#ifdef DEBUG
	LOG("parse value for cell: %s", ezxml_toxml(cell));
#endif
	// number
	ezxml_t v = ezxml_child(cell, "v");
	if (v && v->txt){
#ifdef DEBUG
	LOG("cell has number value");
#endif
		c->value = strdup(v->txt);
	}
	
	// formula
	ezxml_t f = ezxml_child(cell, "f"); //formula
	if (f && f->txt){
#ifdef DEBUG
	LOG("cell has formula value");
#endif
		c->formula = strdup(f->txt);
	}

	//cell type
	const char * type = ezxml_attr(cell, "t");
	if (!type)
		return;
	
#ifdef DEBUG
	LOG("cell has type: %s", type);
#endif

	if (strcmp(type, "str") == 0){
		//formula string
#ifdef DEBUG
LOG("formula string"); 
#endif
		ezxml_t f = ezxml_child(cell, "f");
		if (f && f->txt)
			c->formula = strdup(f->txt);
	} 
	
	else if (strcmp(type, "b") == 0){
		//boolean
		ezxml_t v = ezxml_child(cell, "v");
		if (v && v->txt)
			c->value = strdup(atoi(v->txt)?"true":"false");
	} 
	
	else if (strcmp(type, "e") == 0){
		//error
		c->value = NULL;
	} 

	else if (strcmp(type, "inlineStr") == 0){
		//inline str
		ezxml_t t = ezxml_get(cell, "is", 0, "t", 0, "");
		if (t && t->txt)
			c->value = strdup(t->txt);
	} 
	
	else if (strcmp(type, "n") == 0){
		//number type
		ezxml_t v = ezxml_child(cell, "v");
		if (v && v->txt)
			c->value = strdup(v->txt);
		
		ezxml_t f = ezxml_child(cell, "f"); //formula
		if (f && f->txt)
			c->formula = strdup(f->txt);
		}
	else if (strcmp(type, "s") == 0){
		//string is Shared String 
		ezxml_t v = ezxml_child(cell, "v");
		if (v && wb->sharedStrings){
			const char * value = v->txt;
			if (value){
				ezxml_t _r = ezxml_get(wb->sharedStrings, "si", atoi(value), "r", 0, "");
				if (_r) //shared rich string
				{ 
#ifdef DEBUG
LOG("riched sharedStrings"); 
#endif
					// non-formating string to save value
					struct str str; 
					str_init(&str, 64);
					// formated string to save as array
					array_t *a = array_new(xlsxFormatedString, 
							ERR("%s","array_new"); return);
					for (; _r; _r=_r->next)
					{
						xlsxFormatedString fsting;
						memset(&fsting, 0, sizeof(xlsxFormatedString));

						ezxml_t t = ezxml_child(_r, "t");
						if (t){
							const char * space = ezxml_attr(t, "xml:space");
							if (space){	
								///* TODO:  xml:space="preserve" */
							}
							ezxml_t rPr = ezxml_child(_r, "rPr");
							if (rPr)
								xlsx_parse_font(&fsting.font, rPr);

							// add string value
							if (t->txt){
								// add to formated string
								fsting.string = strdup(t->txt);
								// add to non-formated string
								str_append(&str, t->txt, strlen(t->txt));
							}

							array_append(a, xlsxFormatedString, fsting, 
									ERR("array_append"); return);
						}
					}
					// save non-formated string to value
					c->value = str.str;

					// save formated string
					c->fsting = a->data;
					c->nfstring = a->len;
					free(a);
				} 
				//common shared string
				else {
#ifdef DEBUG
LOG("common sharedStrings"); 
#endif
					ezxml_t t = ezxml_get(wb->sharedStrings, "si", atoi(value), "t", 0, "");
					if (t && t->txt)
						c->value = strdup(t->txt);
				}						
			}
		}
	}
}
