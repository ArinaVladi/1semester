#pragma once

#define P printf("%s: %d\n", __func__, __LINE__);

const int  CAPACITY   =     10;
const double POISON   = -2.038;
const int INT_POISON = -12345;

enum ELEM_TYPES {
    HEAD     = 1,
    ORDINARY = 2,
    TAIL     = 3
};

// #define Xprintf_img(format, ...)  fprintf(dump_file, format, __VA_ARGS__);
// #define Xprint_img(str)           fprintf(dump_file, "%s", str);

#define Xline_color(clr) Xprintf_img("node[color=\"%s\",fontsize=14];\n", #clr);

#define Xelem(ind, status)  \
        Xprintf_img("elem_%d[shape=record,label=\" <ind> %d\\n%s| {<prev> prev\\n%d | <next> next\\n%d} | value\\n%f\", style=\"filled\", fillcolor=\"%s\"];\n", \
                        ind,                                                                                                                                     \
                        ind,                                                                                                                                     \
                        (status == HEAD)? "head" :                                                                                                               \
                            ((status == TAIL)? "tail" :                                                                                                          \
                                ".") ,                                                                                                                            \
                        elements[ind].prev,                                                                                                                      \
                        elements[ind].next,                                                                                                                      \
                        elements[ind].data,                                                                                                                      \
                        ((status == HEAD) || (status == TAIL))? "goldenrod" :                                                                                    \
                                ((elements[ind].prev == -1)? "darkolivegreen1" :                                                                                 \
                                    "cornsilk"));
                          
#define Xaux_line_next(ind)   Xprintf_img("elem_%d:next -> elem_%d:ind;\n", ind, ind+1);
#define Xaux_line_prev(ind)   Xprintf_img("elem_%d:prev -> elem_%d:ind [color=red];\n", ind, ind-1);

#define Xline_next(ind)       Xprintf_img("elem_%d:next -> elem_%d:ind [constrain=true];\n", ind, elements[ind].next);
#define Xline_prev(ind)       Xprintf_img("elem_%d:prev -> elem_%d:ind [constrain=true, color=green];\n", ind, elements[ind].prev);

#define Xprintf_html(format, ...)  fprintf(html_file, format, __VA_ARGS__);
#define Xprint_html(...)           fprintf(html_file, "%s", __VA_ARGS__);