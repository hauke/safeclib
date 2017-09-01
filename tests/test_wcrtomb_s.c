/*------------------------------------------------------------------
 * test_wcrtomb_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static char      dest[LEN];
static wchar_t   src;

#ifdef HAVE_WCHAR_H
#include <stdlib.h>
#include <locale.h>
#include <langinfo.h>

#define CLRPS \
  memset (&ps, '\0', sizeof (mbstate_t))

int test_wcrtomb_s (void)
{
    errno_t rc;
    size_t ind;
    const char* chs;
    mbstate_t ps;
    int errs = 0;

/*--------------------------------------------------*/

    src = L'a';
    rc = wcrtomb_s(NULL, NULL, LEN, src, &ps);
    ERR(ESNULLP);
    CLRPS;

    rc = wcrtomb_s(&ind, NULL, LEN, src, NULL);
    ERR(ESNULLP);

    rc = wcrtomb_s(&ind, dest, LEN, L'\0', &ps);
    ERR(EOK);
    INDCMP(!= 1);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, 0, src, &ps);
    ERR(ESZEROL);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, RSIZE_MAX_STR+1, src, &ps);
    ERR(ESLEMAX);
    CLRPS;

/*--------------------------------------------------*/

    rc = wcrtomb_s(&ind, dest, LEN, L'\a', &ps);
    ERR(EOK);
    INDCMP(!= 1);
    CLRPS;

    setlocale(LC_CTYPE, "C");
    chs = nl_langinfo(CODESET);
    /* not a big problem if this fails */
    if ( !strcmp(chs, "C") ||
         !strcmp(chs, "ASCII") ||
         !strcmp(chs, "ANSI_X3.4-1968") ||
         !strcmp(chs, "US-ASCII") )
        ; /* all fine */
    else /* dont inspect the values */
        printf(__FILE__ ": cannot set C locale for test"
                   " (codeset=%s)\n", chs);

    /* no-breaking space illegal in ASCII, but legal in C */
    rc = wcrtomb_s(&ind, dest, LEN, L'\xa0', &ps);
    if (rc == 0) { /* legal */
      ERR(EOK);
      INDCMP(!= 1);
      if ((unsigned char)dest[0] != 0xa0) {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[0]);
        errs++;
      }
      if (dest[1] != L'\0') {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[1]);
        errs++;
      }
    } else {
      ERR(EILSEQ); /* or illegal */
      INDCMP(!= -1);
      if (dest[0] != '\0') {
        printf("%s %u  Error  ind=%d rc=%d %d\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[0]);
        errs++;
      }
    }
    CLRPS;

    rc = wcrtomb_s(&ind, dest, LEN, L'\x78', &ps);
    ERR(EOK);
    INDCMP(!= 1);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, LEN, L'\xdf81', &ps);
    if (rc == 0) { /* well, musl on ASCII allows this */
      INDCMP(!= 1);
    } else {
      ERR(EILSEQ);
      INDCMP(!= -1);
    }
    CLRPS;

    setlocale(LC_CTYPE, "en_US.UTF-8") ||
	setlocale(LC_CTYPE, "en_GB.UTF-8") ||
	setlocale(LC_CTYPE, "en.UTF-8") ||
	setlocale(LC_CTYPE, "POSIX.UTF-8") ||
	setlocale(LC_CTYPE, "C.UTF-8") ||
	setlocale(LC_CTYPE, "UTF-8") ||
	setlocale(LC_CTYPE, "");
    chs = nl_langinfo(CODESET);
    if (!strstr(chs, "UTF-8")) {
        printf(__FILE__ ": cannot set UTF-8 locale for test"
               " (codeset=%s)\n", chs);
        return 0;
    }

    /* overlarge utf-8 sequence */
    rc = wcrtomb_s(&ind, dest, 2, L'\x2219', &ps);
    ERR(ESNOSPC);
    CLRPS;
    rc = wcrtomb_s(&ind, dest, 3, L'\x2219', &ps);
    ERR(ESNOSPC);
    CLRPS;

    rc = wcrtomb_s(&ind, dest, 4, L'\x2219', &ps);
    ERR(EOK);
    INDCMP(!= 3);
    if (dest[0] != '\xe2') {
        printf("%s %u  Error  ind=%d rc=%d %x\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[0]);
        errs++;
    }
    if (dest[1] != '\x88') {
        printf("%s %u  Error  ind=%d rc=%d %x\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[1]);
        errs++;
    }
    if (dest[2] != '\x99') {
        printf("%s %u  Error  ind=%d rc=%d %x\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[2]);
        errs++;
    }
    if (dest[3] != '\0') {
        printf("%s %u  Error  ind=%d rc=%d %x\n",
               __FUNCTION__, __LINE__, (int)ind, rc, dest[3]);
        errs++;
    }
    CLRPS;

    /* illegal utf-8 sequence */
    rc = wcrtomb_s(&ind, dest, LEN, L'\xdf79', &ps);
    ERR(EILSEQ);
    INDCMP(!= -1);

/*--------------------------------------------------*/
    
    return (errs);
}

#endif

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
#ifdef HAVE_WCHAR_H
    return (test_wcrtomb_s());
#else
    return 0;
#endif    
}
#endif
