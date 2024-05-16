#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/**
 * @brief Convert an Excel `A1` cell string into a `(row, col)` pair.
 *
 * Convert an Excel `A1` cell string into a `(row, col)` pair.
 *
 * This is a little syntactic shortcut to help with worksheet layout:
 *
 * @code
 *      worksheet_write_string(worksheet, CELL("A1"), "Foo", NULL);
 *
 *      //Same as:
 *      worksheet_write_string(worksheet, 0, 0,       "Foo", NULL);
 * @endcode
 *
 * @note
 *
 * This macro shouldn't be used in performance critical situations since it
 * expands to two function calls.
 */
#define CELL(cell) \
    xlsx_name_to_row(cell), xlsx_name_to_col(cell)

/**
 * @brief Convert an Excel `A:B` column range into a `(col1, col2)` pair.
 *
 * Convert an Excel `A:B` column range into a `(col1, col2)` pair.
 *
 * This is a little syntactic shortcut to help with worksheet layout:
 *
 * @code
 *     worksheet_set_column(worksheet, COLS("B:D"), 20, NULL, NULL);
 *
 *     // Same as:
 *     worksheet_set_column(worksheet, 1, 3,        20, NULL, NULL);
 * @endcode
 *
 */
#define COLS(cols) \
    xlsx_name_to_col(cols), xlsx_name_to_col_2(cols)

/**
 * @brief Convert an Excel `A1:B2` range into a `(first_row, first_col,
 *        last_row, last_col)` sequence.
 *
 * Convert an Excel `A1:B2` range into a `(first_row, first_col, last_row,
 * last_col)` sequence.
 *
 * This is a little syntactic shortcut to help with worksheet layout.
 *
 * @code
 *     worksheet_print_area(worksheet, 0, 0, 41, 10); // A1:K42.
 *
 *     // Same as:
 *     worksheet_print_area(worksheet, RANGE("A1:K42"));
 * @endcode
 */
#define RANGE(range) \
    xlsx_name_to_row(range), xlsx_name_to_col(range), \
    xlsx_name_to_row_2(range), xlsx_name_to_col_2(range)

/*
 * Convert Excel A-XFD style column name to zero based number.
 */
static void
xlsx_col_to_name(char *col_name, int col_num, char absolute)
{
    char pos = 0;
    size_t len;
    size_t i;

    /* Change from 0 index to 1 index. */
    col_num++;

    /* Convert the column number to a string in reverse order. */
    while (col_num) {

        /* Get the remainder in base 26. */
        int remainder = col_num % 26;

        if (remainder == 0)
            remainder = 26;

        /* Convert the remainder value to a character. */
        col_name[pos++] = 'A' + remainder - 1;
        col_name[pos] = '\0';

        /* Get the next order of magnitude. */
        col_num = (col_num - 1) / 26;
    }

    if (absolute) {
        col_name[pos] = '$';
        col_name[pos + 1] = '\0';
    }

    /* Reverse the column name string. */
    len = strlen(col_name);
    for (i = 0; i < (len / 2); i++) {
        char tmp = col_name[i];
        col_name[i] = col_name[len - i - 1];
        col_name[len - i - 1] = tmp;
    }
}

/*
 * Convert zero indexed row and column to an Excel style A1 cell reference.
 */
static void
xlsx_rowcol_to_cell(char *cell_name, int row, int col)
{
    size_t pos;

    /* Add the column to the cell. */
    xlsx_col_to_name(cell_name, col, 0);

    /* Get the end of the cell. */
    pos = strlen(cell_name);

    /* Add the row to the cell. */
    snprintf(&cell_name[pos], BUFSIZ, "%d", ++row);
}

/*
 * Convert zero indexed row and column to an Excel style $A$1 cell with
 * an absolute reference.
 */
static void
xlsx_rowcol_to_cell_abs(char *cell_name, int row, int col,
                       char abs_row, char abs_col)
{
    size_t pos;

    /* Add the column to the cell. */
    xlsx_col_to_name(cell_name, col, abs_col);

    /* Get the end of the cell. */
    pos = strlen(cell_name);

    if (abs_row)
        cell_name[pos++] = '$';

    /* Add the row to the cell. */
    snprintf(&cell_name[pos], BUFSIZ, "%d", ++row);
}

/*
 * Convert zero indexed row and column pair to an Excel style A1:C5
 * range reference.
 */
static void
xlsx_rowcol_to_range(char *range,
                    int first_row, int first_col,
                    int last_row, int last_col)
{
    size_t pos;

    /* Add the first cell to the range. */
    xlsx_rowcol_to_cell(range, first_row, first_col);

    /* If the start and end cells are the same just return a single cell. */
    if (first_row == last_row && first_col == last_col)
        return;

    /* Get the end of the cell. */
    pos = strlen(range);

    /* Add the range separator. */
    range[pos++] = ':';

    /* Add the first cell to the range. */
    xlsx_rowcol_to_cell(&range[pos], last_row, last_col);
}

/*
 * Convert zero indexed row and column pairs to an Excel style $A$1:$C$5
 * range reference with absolute values.
 */
static void
xlsx_rowcol_to_range_abs(char *range,
                        int first_row, int first_col,
                        int last_row, int last_col)
{
    size_t pos;

    /* Add the first cell to the range. */
    xlsx_rowcol_to_cell_abs(range, first_row, first_col, 1, 1);

    /* If the start and end cells are the same just return a single cell. */
    if (first_row == last_row && first_col == last_col)
        return;

    /* Get the end of the cell. */
    pos = strlen(range);

    /* Add the range separator. */
    range[pos++] = ':';

    /* Add the first cell to the range. */
    xlsx_rowcol_to_cell_abs(&range[pos], last_row, last_col, 1, 1);
}

/*
 * Convert sheetname and zero indexed row and column pairs to an Excel style
 * Sheet1!$A$1:$C$5 formula reference with absolute values.
 */
static void
xlsx_rowcol_to_formula_abs(char *formula, const char *sheetname,
                          int first_row, int first_col,
                          int last_row, int last_col)
{
    size_t pos;
    char quoted_name[BUFSIZ];
		sprintf(quoted_name, "\'%s\'", sheetname);

    strncpy(formula, quoted_name, BUFSIZ - 1);

    /* Get the end of the sheetname. */
    pos = strlen(formula);

    /* Add the range separator. */
    formula[pos++] = '!';

    /* Add the first cell to the range. */
    xlsx_rowcol_to_cell_abs(&formula[pos], first_row, first_col, 1, 1);

    /* If the start and end cells are the same just return a single cell. */
    if (first_row == last_row && first_col == last_col)
        return;

    /* Get the end of the cell. */
    pos = strlen(formula);

    /* Add the range separator. */
    formula[pos++] = ':';

    /* Add the first cell to the range. */
    xlsx_rowcol_to_cell_abs(&formula[pos], last_row, last_col, 1, 1);
}

/*
 * Convert an Excel style A1 cell reference to a zero indexed row number.
 */
static int
xlsx_name_to_row(const char *row_str)
{
    int row_num = 0;
    const char *p = row_str;

    /* Skip the column letters and absolute symbol of the A1 cell. */
    while (p && !isdigit((unsigned char) *p))
        p++;

    /* Convert the row part of the A1 cell to a number. */
    if (p)
        row_num = atoi(p);

    if (row_num)
        return row_num - 1;
    else
        return 0;
}

/*
 * Convert an Excel style A1 cell reference to a zero indexed column number.
 */
static int
xlsx_name_to_col(const char *col_str)
{
    int col_num = 0;
    const char *p = col_str;

    /* Convert leading column letters of A1 cell. Ignore absolute $ marker. */
    while (p && (isupper((unsigned char) *p) || *p == '$')) {
        if (*p != '$')
            col_num = (col_num * 26) + (*p - 'A' + 1);
        p++;
    }

    return col_num - 1;
}

/*
 * Convert the second row of an Excel range ref to a zero indexed number.
 */
static int
xlsx_name_to_row_2(const char *row_str)
{
    const char *p = row_str;

    /* Find the : separator in the range. */
    while (p && *p != ':')
        p++;

    if (p)
        return xlsx_name_to_row(++p);
    else
        return -1;
}

/*
 * Convert the second column of an Excel range ref to a zero indexed number.
 */
static int
xlsx_name_to_col_2(const char *col_str)
{
    const char *p = col_str;

    /* Find the : separator in the range. */
    while (p && *p != ':')
        p++;

    if (p)
        return xlsx_name_to_col(++p);
    else
        return -1;
}

/*
 * Convert a xlsx_datetime struct to an Excel serial date, with a 1900
 * or 1904 epoch.
 */
static double
xlsx_datetime_to_excel_date_epoch(struct tm *datetime, int date_1904)
{
    int year = datetime->tm_year;
    int month = datetime->tm_mon;
    int day = datetime->tm_mday;
    int hour = datetime->tm_hour;
    int min = datetime->tm_min;
    double sec = datetime->tm_sec;
    double seconds;
    int epoch = date_1904 ? 1904 : 1900;
    int offset = date_1904 ? 4 : 0;
    int norm = 300;
    int range;
    /* Set month days and check for leap year. */
    int mdays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int leap = 0;
    int days = 0;
    int i;

    /* For times without dates set the default date for the epoch. */
    if (!year) {
        if (!date_1904) {
            year = 1899;
            month = 12;
            day = 31;
        }
        else {
            year = 1904;
            month = 1;
            day = 1;
        }
    }

    /* Convert the Excel seconds to a fraction of the seconds in 24 hours. */
    seconds = (hour * 60 * 60 + min * 60 + sec) / (24 * 60 * 60.0);

    /* Special cases for Excel dates in the 1900 epoch. */
    if (!date_1904) {
        /* Excel 1900 epoch. */
        if (year == 1899 && month == 12 && day == 31)
            return seconds;

        /* Excel 1900 epoch. */
        if (year == 1900 && month == 1 && day == 0)
            return seconds;

        /* Excel false leapday */
        if (year == 1900 && month == 2 && day == 29)
            return 60 + seconds;
    }

    /* We calculate the date by calculating the number of days since the */
    /* epoch and adjust for the number of leap days. We calculate the */
    /* number of leap days by normalizing the year in relation to the */
    /* epoch. Thus the year 2000 becomes 100 for 4-year and 100-year */
    /* leapdays and 400 for 400-year leapdays. */
    range = year - epoch;

    if (year % 4 == 0 && (year % 100 > 0 || year % 400 == 0)) {
        leap = 1;
        mdays[2] = 29;
    }

    /*
     * Calculate the serial date by accumulating the number of days
     * since the epoch.
     */

    /* Add days for previous months. */
    for (i = 0; i < month; i++) {
        days += mdays[i];
    }
    /* Add days for current month. */
    days += day;
    /* Add days for all previous years.  */
    days += range * 365;
    /* Add 4 year leapdays. */
    days += range / 4;
    /* Remove 100 year leapdays. */
    days -= (range + offset) / 100;
    /* Add 400 year leapdays. */
    days += (range + offset + norm) / 400;
    /* Remove leap days already counted. */
    days -= leap;

    /* Adjust for Excel erroneously treating 1900 as a leap year. */
    if (!date_1904 && days > 59)
        days++;

    return days + seconds;
}

/*
 * Convert a xlsx_datetime struct to an Excel serial date, for the 1900 epoch.
 */
static double
xlsx_datetime_to_excel_datetime(struct tm *datetime)
{
    return xlsx_datetime_to_excel_date_epoch(datetime, 0);
}

/*
 * Convert a unix datetime (1970/01/01 epoch) to an Excel serial date, with a
 * 1900 or 1904 epoch.
 */
static double
xlsx_unixtime_to_excel_date_epoch(int64_t unixtime, int date_1904)
{
    double excel_datetime = 0.0;
    double epoch = date_1904 ? 24107.0 : 25568.0;

    excel_datetime = epoch + (unixtime / (24 * 60 * 60.0));

    if (!date_1904 && excel_datetime >= 60.0)
        excel_datetime = excel_datetime + 1.0;

    return excel_datetime;
}

/*
 * Convert a unix datetime (1970/01/01 epoch) to an Excel serial date, with a
 * 1900 epoch.
 */
static double
xlsx_unixtime_to_excel_date(int64_t unixtime)
{
    return xlsx_unixtime_to_excel_date_epoch(unixtime, 0);
}

#endif /* ifndef HELPERS_H */
