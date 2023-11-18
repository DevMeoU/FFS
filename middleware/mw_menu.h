/*** HARDWARE ABSTRACTION LAYER ***/
#ifndef _MW_MENU_H_ /* MIDDLEWARE MENU */
#define _MW_MENU_H_ /* MIDDLEWARE MENU */

#include "../utilities/linked_list/linked_list.h"

#define NEWLINE printf("\n");
#define TAB printf("  ");
#define POINT printf("..");
#define ENDLINE printf("-");
#define EXIT printf("0. Exit");
#define CLS system("cls");
#define BACK printf("0. Back");
#define PAUSE system("pause");

#define __ENDLINE(N)                \
    {                               \
        for (int i = 0; i < N; i++) \
        {                           \
            ENDLINE                 \
        }                           \
    }

#define __NEWLINE(N)                \
    {                               \
        for (int i = 0; i < N; i++) \
        {                           \
            NEWLINE                 \
        }                           \
    }

#define __TAB(N)                    \
    {                               \
        for (int i = 0; i < N; i++) \
        {                           \
            TAB                     \
        }                           \
    }
    
#define __POINT(N)                  \
    {                               \
        for (int i = 0; i < N; i++) \
        {                           \
            POINT                   \
        }                           \
    }

#define __LINE {\
    __TAB(4) printf("|");\
    __ENDLINE(4) printf("|");\
    __ENDLINE(20) printf("|");\
    __ENDLINE(20) printf("|");\
    __ENDLINE(20) printf("|");\
    __ENDLINE(20) printf("|");\
    __ENDLINE(24) printf("|");\
    __ENDLINE(24) printf("|");\
    NEWLINE\
}

#define TITLE_MENU {\
    __NEWLINE(2)\
    __LINE\
    __TAB(4)\
    printf("|.No.|"); __TAB(4)\
    printf("NAME"); __TAB(4) printf("|"); __TAB(4)\
    printf("ATTR"); __TAB(4) printf("|"); __TAB(4)\
    printf("TYPE"); __TAB(4) printf("|"); __TAB(4)\
    printf("SIZE"); __TAB(4) printf("|"); __TAB(4)\
    printf("DATE-CRE"); __TAB(4) printf("|"); __TAB(4)\
    printf("TIME-CRE"); __TAB(4) printf("|"); __TAB(2) NEWLINE\
    __LINE\
}

/** Function Prototype **/
/* This Funtion is initilize menu */
int32_t APPMIDW_MenuInit(const char * const filePath);

static inline int32_t APPMIDW_Assert(int32_t condition, const uint8_t * err_indicate, const uint8_t * info_indicate) {
    if (condition != 0) {
        if(err_indicate != NULL)
        {
            printf("%s\n", err_indicate);
        }
        exit(1);
    }
    if(info_indicate != NULL)
    {
        printf("%s\n", info_indicate);
    }
    return 0;
}

#endif /* _MW_MENU_H_ */
