/**
 * File              : log.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 19.03.2023
 * Last Modified Date: 22.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * log.h
 * Copyright (c) 2023 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Debugging in Android/iOS and other platforms
 */

#ifndef k_lib_log_h
#define k_lib_log_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#define STR(...) ({char s[BUFSIZ]; snprintf(s, BUFSIZ-1, __VA_ARGS__); s[BUFSIZ-1]=0; s;})

#ifdef __ANDROID__
	#include <android/log.h>
	#define LOG(...) __android_log_print(ANDROID_LOG_INFO,  STR("%s: _%s: ", __FILE__, __func__), __VA_ARGS__)
	#define ERR(...) __android_log_print(ANDROID_LOG_ERROR, STR("%s: _%s: %d: ", __FILE__, __func__, __LINE__), __VA_ARGS__) 	
#elif defined __APPLE__
	#include <CoreFoundation/CoreFoundation.h>
	void NSLog(CFStringRef format, ...);
	#define LOG(fmt, ...) NSLog(CFSTR(fmt), ##__VA_ARGS__)
	#define ERR(fmt, ...) LOG("E/_%s: %d: %s", __func__, __LINE__, STR(fmt, __VA_ARGS__)) 
#else
	#define LOG(...) \
	({ \
		char s[BUFSIZ]; snprintf(s, BUFSIZ-1, __VA_ARGS__); s[BUFSIZ-1]=0; \
		fprintf(stderr, "%s: _%s: %s\n",   __FILE__, __func__, s); \
	 })

	#define ERR(...) \
	({\
		char s[BUFSIZ]; snprintf(s, BUFSIZ-1, __VA_ARGS__); s[BUFSIZ-1]=0; \
		char e[BUFSIZ]; snprintf(e, BUFSIZ-1, "E/%s: _%s: %d: %s", __FILE__, __func__, __LINE__, s); e[BUFSIZ-1]=0; \
	  perror(e); \
	})
	
#endif



#ifdef __cplusplus
}
#endif

#endif /* ifndef k_lib_log_h */
