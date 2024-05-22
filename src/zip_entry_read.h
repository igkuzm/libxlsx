#ifndef ZIP_ENTRY_READ_H
#define ZIP_ENTRY_READ_H

#include <zip.h>
#include <stdlib.h>
#include "log.h"
#include "alloc.h"

static int 
zip_entry_read(zip_t *zip, const char *name, 
		void **buffer, size_t *size)
{
#ifdef DEBUG
	LOG("open entry: %s", name);
#endif
	zip_file_t *f = zip_fopen(zip, name, ZIP_FL_UNCHANGED);
	if (!f)
		return -1;
	
	struct zip_stat st;
	zip_stat(zip, name, 0, &st);
	void *buf = MALLOC(st.size, 
			ERR("malloc"); 
			zip_fclose(f);
			return -1);
		
#ifdef DEBUG
	LOG("reading %d bites to buffer", st.size);
#endif
	zip_fread(f, buf, st.size);

	*buffer = buf;
	*size = st.size;

	zip_fclose(f);
	
	return 0;
}

#endif /* ifndef ZIP_ENTRY_READ_H */
