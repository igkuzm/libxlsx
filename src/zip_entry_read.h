#ifndef ZIP_ENTRY_READ_H
#define ZIP_ENTRY_READ_H

#include <zip.h>
#include <stdlib.h>

static int 
zip_entry_read(zip_t *zip, const char *name, 
		void **buffer, size_t *size)
{
	zip_file_t *f = zip_fopen(zip, name, ZIP_FL_UNCHANGED);
	if (!f)
		return -1;
	
	struct zip_stat st;
	zip_stat(zip, name, 0, &st);
	void *buf = malloc(st.size);
	if (!buf){
		perror("malloc");
		zip_fclose(f);
		return -1;
	}

	zip_fread(f, buf, st.size);
	zip_fclose(f);

	if (*buffer)
		*buffer = buf;
	else
		free(buf);

	if (*size)
		*size = st.size;

	return 0;
}

#endif /* ifndef ZIP_ENTRY_READ_H */
