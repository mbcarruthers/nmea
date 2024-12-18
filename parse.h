//
// Created by mbc on 12/17/24.
//

/*
 * @ parse.h - Page 156 from CPL, essentially. Some minor nuances changed
 */
#ifndef CPL_PARSE_H
#define CPL_PARSE_H

// Note: Good example of parsing pattern
// page 156 CPL
void parse(char * restrict fmt, ...) {
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;

    // starts the variable argument list moving
    // must be called before va_list(ap) is used
    va_start(ap, fmt);
    for(p = fmt; *p;p++) {
        if(*p != '%') {
            putchar(*p);
            continue;
        }
        switch(*++p) {
            case 'd':
                ival = va_arg(ap, int);
                printf("%d", ival);
                break;
            case 'f':
                dval = va_arg(ap, double);
                printf("%f", dval);
                break;
            case 's':
                for(sval = va_arg(ap, char*); *sval;sval++) {
            putchar(*sval);
        }
                break;
            default:
                putchar(*p);
                break;
        }
    }
    printf("\n");
    va_end(ap);
}

#endif //CPL_PARSE_H
