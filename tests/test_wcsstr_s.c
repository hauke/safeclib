/*------------------------------------------------------------------
 * test_wcsstr_s
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )
#define SHORT_LEN  ( 5 )

static wchar_t   wstr1[LEN];
static wchar_t   wstr2[LEN];

int test_wcsstr_s (void)
{
    errno_t rc;
    wchar_t *sub;
    wchar_t *std_sub;

    rsize_t len1;
    rsize_t len2;
    int errs = 0;

/*--------------------------------------------------*/

    rc = wcsstr_s(NULL, LEN, wstr2, LEN, &sub);
    ERR(ESNULLP)
    WSUBNULL();

/*--------------------------------------------------*/

    rc = wcsstr_s(wstr1, LEN, NULL, LEN, &sub);
    ERR(ESNULLP)
    WSUBNULL();

/*--------------------------------------------------*/

    rc = wcsstr_s(wstr1, LEN, wstr2, LEN, NULL);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = wcsstr_s(wstr1, 0, wstr2, LEN, &sub);
    ERR(ESZEROL)
    WSUBNULL();

/*--------------------------------------------------*/

    rc = wcsstr_s(wstr1, RSIZE_MAX_STR+1, wstr2, LEN, &sub);
    ERR(ESLEMAX)
    WSUBNULL();

/*--------------------------------------------------*/

    rc = wcsstr_s(wstr1, LEN, wstr2, 0, &sub);
    ERR(ESZEROL)
    WSUBNULL();

/*--------------------------------------------------*/

    rc = wcsstr_s(wstr1, LEN, wstr2, RSIZE_MAX_STR+1, &sub);
    ERR(ESLEMAX)
    WSUBNULL();

/*--------------------------------------------------*/

    *wstr1 = '\0';
    *wstr2 = '\0';

    rc = wcsstr_s(wstr1, LEN, wstr2, LEN, &sub);
    ERR(EOK)
    if (sub != wstr1) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    *wstr2 = L'\0';

    rc = wcsstr_s(wstr1, LEN, wstr2, LEN, &sub);
    ERR(EOK)
    if (sub != wstr1) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"kee");

    /* substring at beginning */
    rc = wcsstr_s(wstr1, LEN, wstr2, LEN, &sub);
    ERR(EOK)
    if (sub != &wstr1[0]) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"eep it");

    /* substring in the middle - left */
    rc = wcsstr_s(wstr1, LEN, wstr2, LEN, &sub);
    ERR(EOK)
    if (sub != &wstr1[1]) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"ethe");

    /* substring in the middle - right */
    rc = wcsstr_s(wstr1, LEN, wstr2, LEN, &sub);
    ERR(EOK)
    if (sub != &wstr1[15]) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/
               // 012345678901234567890
    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"he");

    len1 = wcslen(wstr1);
    len2 = wcslen(wstr2);

    rc = wcsstr_s(wstr1, len1, wstr2, len2, &sub);
    ERR(EOK)
    if (sub != &wstr1[17]) {
        debug_printf("%s %u  Error rc=%d  sub=\"%ls\" \n",
                     __FUNCTION__, __LINE__, rc, sub);
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"er");

    len1 = wcslen(wstr1);
    len2 = wcslen(wstr2);

    rc = wcsstr_s(wstr1, len1, wstr2, len2, &sub);
    ERR(EOK)
    if (sub != &wstr1[18]) {
        debug_printf("%s %u  Error rc=%d  sub=\"%ls\" \n",
                     __FUNCTION__, __LINE__, rc, sub);
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"it all");

    rc = wcsstr_s(wstr1, 3, wstr2, LEN, &sub);
    ERR(ESNOTFND)
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"it all");

    rc = wcsstr_s(wstr1, 333, wstr2, LEN, &sub);
    ERR(EOK)
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"it all");

    rc = wcsstr_s(wstr1, LEN, wstr2, 5, &sub);
    ERR(EOK)
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"1234");

    len1 = wcslen(wstr1);

    rc = wcsstr_s(wstr1, len1, wstr2, LEN, &sub);
    ERR(ESNOTFND)
    if (sub != NULL) {
        debug_printf("%s %u  Error rc=%d \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"it all");

    rc = wcsstr_s(wstr1, LEN, wstr2, 2, &sub);
    ERR(EOK)
/*--------------------------------------------------*/

    wcscpy(wstr1, L"keep it all together");
    wcscpy(wstr2, L"eep");

    rc = wcsstr_s(wstr1, LEN, wstr2, LEN, &sub);
    ERR(EOK)
    if (sub != &wstr1[1]) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
    /* compare to legacy */
    std_sub = wcsstr(wstr1, wstr2);
    if (sub != std_sub) {
        debug_printf("%s %u  Error rc=%u \n",
                     __FUNCTION__, __LINE__, rc);
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wcsstr_s());
}
#endif
