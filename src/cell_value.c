#include "cell_value.h"
#include <string.h>
#include "ezxml.h"
#include "font.h"
#include "safe_malloc.h"
#include "str.h"

void xlsx_parse_cell_value(xlsxCell *c, ezxml_t cell, 
		xlsxWorkBook *wb)
{
	//cell type
	const char * type = ezxml_attr(cell, "t");
	if (type) {
		if (strcmp(type, "b") == 0){
			//boolean
			ezxml_t v = ezxml_child(cell, "v");
			if (v && v->txt)
				c->value = strdup(atoi(v->txt)?"true":"false");
		} 
		else 
		if (strcmp(type, "e") == 0){
			//error
			c->value = NULL;
		} 
		else 
		if (strcmp(type, "inlineStr") == 0){
			//inline str
			ezxml_t t = ezxml_get(cell, "is", 0, "t", 0, "");
			if (t && t->txt)
				c->value = strdup(t->txt);
		} 
		else 
		if (strcmp(type, "n") == 0){
			//number type
			ezxml_t v = ezxml_child(cell, "v");
			if (v && v->txt)
				c->value = strdup(v->txt);
			
			ezxml_t f = ezxml_child(cell, "f"); //formula
			if (f && f->txt)
				c->formula = strdup(f->txt);
		}
		else 
		if (strcmp(type, "s") == 0){
			//string is Shared String 
			ezxml_t v = ezxml_child(cell, "v");
			if (v && wb->sharedStrings){
				const char * value = v->txt;
				ezxml_t _r = ezxml_get(wb->sharedStrings, "si", atoi(value), "r", 0, "");
				if (_r) //shared ritch string
				{ 
					struct str str; // non-formating string to save value
					str_init(&str, 64);
					for (; _r; _r=_r->next)
					{
						// cycle trough ritch string pointer
						xlsxFormatedString *ptr = c->fsting;
						while(ptr)
							ptr = ptr->next;
						
						//allocate ritch string
						xlsxFormatedString *new = MALLOC(sizeof(xlsxFormatedString), return);
						if (ptr)
							ptr->next = new;
					
						ezxml_t t = ezxml_child(_r, "t");
						if (t){
							const char * space = ezxml_attr(t, "xml:space");
							if (space){	
								///* TODO:  xml:space="preserve" */
							}
							ezxml_t rPr = ezxml_child(_r, "rPr");
							if (rPr)
								xlsx_parse_font(&new->font, rPr);

							// add string value
							if (t->txt){
								// add to formated string
								new->string = strdup(t->txt);
								// add to non-formated string
								str_append(&str, t->txt, strlen(t->txt));
							}
						}
					}
					// save non-formated string to value
					c->value = str.str;
				} 
				//common shared string
				else {
					ezxml_t t = ezxml_get(wb->sharedStrings, "si", atoi(value), "t", 0, "");
					if (t && t->txt)
						c->value = strdup(t->txt);
				}						
			}
		}
		else 
		if (strcmp(type, "str") == 0){
			//formula string
			ezxml_t f = ezxml_child(cell, "f");
			if (f && f->txt)
				c->formula = strdup(f->txt);
		} 
	}
	else {
		// numbers
		ezxml_t v = ezxml_child(cell, "v");
		if (v && v->txt)
			c->value = strdup(v->txt);
		
		ezxml_t f = ezxml_child(cell, "f"); //formula
		if (f && f->txt)
			c->formula = strdup(f->txt);
	}
}
