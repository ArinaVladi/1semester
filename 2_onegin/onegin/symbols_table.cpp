#include "symbols_table.h"

static const int symbols_table[256] = {
        IsStrangeDigit                                 , //   0  " "
        IsStrangeDigit                                 , //   1  ""
        IsStrangeDigit                                 , //   2  ""
        IsStrangeDigit                                 , //   3  ""
        IsStrangeDigit                                 , //   4  ""
        IsStrangeDigit                                 , //   5  ""
        IsStrangeDigit                                 , //   6  ""
        IsStrangeDigit                                 , //   7  ""
        IsStrangeDigit                                 , //   8  ""
        IsNotLineBegin|IsPunct|IsStrangeDigit|IsSpace  , //   9 "\t"
        IsNotLineBegin|IsPunct|IsStrangeDigit|IsSpace  , //  10 "\n"
        IsNotLineBegin|IsPunct|IsStrangeDigit|IsSpace  , //  11 "\v"
        IsStrangeDigit                                 , //  12  ""
        IsNotLineBegin|IsPunct|IsStrangeDigit|IsSpace  , //  13 "\r"
        IsStrangeDigit                                 , //  14  ""
        IsStrangeDigit                                 , //  15  ""
        IsStrangeDigit                                 , //  16  ""
        IsStrangeDigit                                 , //  17  ""
        IsStrangeDigit                                 , //  18  ""
        IsStrangeDigit                                 , //  19  ""
        IsStrangeDigit                                 , //  20  ""
        IsStrangeDigit                                 , //  21  ""
        IsStrangeDigit                                 , //  22  ""
        IsStrangeDigit                                 , //  23  ""
        IsStrangeDigit                                 , //  24  ""
        IsStrangeDigit                                 , //  25  ""
        IsStrangeDigit                                 , //  26  ""
        IsStrangeDigit                                 , //  27  ""
        IsStrangeDigit                                 , //  28  ""
        IsStrangeDigit                                 , //  29  ""
        IsStrangeDigit                                 , //  30  ""
        IsStrangeDigit                                 , //  31  ""
        IsNotLineBegin|IsPunct|IsSpace                 , //  32  " "
        IsPunct                                        , //  33  "!"
        IsPunct                                        , //  34  """
        IsPunct                                        , //  35  "#"
        IsPunct                                        , //  36  "$"
        IsPunct                                        , //  37  "%"
        IsPunct                                        , //  38  "&"
        IsPunct                                        , //  39  "'"
        IsPunct                                        , //  40  " ("
        IsPunct                                        , //  41  ")"
        IsNotLineBegin|IsPunct                         , //  42  "*"
        IsPunct                                        , //  43  "+"
        IsPunct                                        , //  44  ","
        IsPunct                                        , //  45  "-"
        IsNotLineBegin|IsPunct                         , //  46  "."
        IsPunct                                        , //  47  "/"
        IsDigit                                        , //  48  "0"
        IsDigit                                        , //  49  "1"
        IsDigit                                        , //  50  "2"
        IsDigit                                        , //  51  "3"
        IsDigit                                        , //  52  "4"
        IsDigit                                        , //  53  "5"
        IsDigit                                        , //  54  "6"
        IsDigit                                        , //  55  "7"
        IsDigit                                        , //  56  "8"
        IsDigit                                        , //  57  "9"
        IsPunct                                        , //  58  ":"
        IsPunct                                        , //  59  ";"
        IsPunct                                        , //  60  "<"
        IsPunct                                        , //  61  "="
        IsPunct                                        , //  62  ">"
        IsPunct                                        , //  63  "?"
        IsPunct                                        , //  64  "@"
        IsAlpha|IsUpper                                , //  65  "A"
        IsAlpha|IsUpper                                , //  66  "B"
        IsAlpha|IsRomanDigit|IsNotLineBegin|IsUpper    , //  67  "C"
        IsAlpha|IsRomanDigit|IsNotLineBegin|IsUpper    , //  68  "D"
        IsAlpha|IsUpper                                , //  69  "E"
        IsAlpha|IsUpper                                , //  70  "F"
        IsAlpha|IsUpper                                , //  71  "G"
        IsAlpha|IsUpper                                , //  72  "H"
        IsAlpha|IsRomanDigit|IsNotLineBegin|IsUpper    , //  73  "I"
        IsAlpha|IsUpper                                , //  74  "J"
        IsAlpha|IsUpper                                , //  75  "K"
        IsAlpha|IsRomanDigit|IsNotLineBegin|IsUpper    , //  76  "L"
        IsAlpha|IsRomanDigit|IsNotLineBegin|IsUpper    , //  77  "M"
        IsAlpha|IsUpper                                , //  78  "N"
        IsAlpha|IsUpper                                , //  79  "O"
        IsAlpha|IsUpper                                , //  80  "P"
        IsAlpha|IsUpper                                , //  81  "Q"
        IsAlpha|IsUpper                                , //  82  "R"
        IsAlpha|IsUpper                                , //  83  "S"
        IsAlpha|IsUpper                                , //  84  "T"
        IsAlpha|IsUpper                                , //  85  "U"
        IsAlpha|IsRomanDigit|IsNotLineBegin|IsUpper    , //  86  "V"
        IsAlpha|IsUpper                                , //  87  "W"
        IsAlpha|IsRomanDigit|IsNotLineBegin|IsUpper    , //  88  "X"
        IsAlpha|IsUpper                                , //  89  "Y"
        IsAlpha|IsUpper                                , //  90  "Z"
        IsPunct                                        , //  91  "["
        IsPunct                                        , //  92  "\"
        IsPunct                                        , //  93  "]"
        IsPunct                                        , //  94  "^"
        IsPunct                                        , //  95  "_"
        IsPunct                                        , //  96  "`"
        IsAlpha|IsLower                                , //  97  "a"
        IsAlpha|IsLower                                , //  98  "b"
        IsAlpha|IsLower                                , //  99  "c"
        IsAlpha|IsLower                                , // 100  "d"
        IsAlpha|IsLower                                , // 101  "e"
        IsAlpha|IsLower                                , // 102  "f"
        IsAlpha|IsLower                                , // 103  "g"
        IsAlpha|IsLower                                , // 104  "h"
        IsAlpha|IsLower                                , // 105  "i"
        IsAlpha|IsLower                                , // 106  "j"
        IsAlpha|IsLower                                , // 107  "k"
        IsAlpha|IsLower                                , // 108  "l"
        IsAlpha|IsLower                                , // 109  "m"
        IsAlpha|IsLower                                , // 110  "n"
        IsAlpha|IsLower                                , // 111  "o"
        IsAlpha|IsLower                                , // 112  "p"
        IsAlpha|IsLower                                , // 113  "q"
        IsAlpha|IsLower                                , // 114  "r"
        IsAlpha|IsLower                                , // 115  "s"
        IsAlpha|IsLower                                , // 116  "t"
        IsAlpha|IsLower                                , // 117  "u"
        IsAlpha|IsLower                                , // 118  "v"
        IsAlpha|IsLower                                , // 119  "w"
        IsAlpha|IsLower                                , // 120  "x"
        IsAlpha|IsLower                                , // 121  "y"
        IsAlpha|IsLower                                , // 122  "z"
        IsPunct                                        , // 123  "{"
        IsPunct                                        , // 124  "|"
        IsPunct                                        , // 125  "}"
        IsPunct                                        , // 126  "~"
        IsStrangeDigit                                 , // 127  ""
        IsPunct                                        , // 128  "Ђ"
        IsPunct                                        , // 129  "Ѓ"
        IsPunct                                        , // 130  "‚"
        IsPunct                                        , // 131  "ѓ"
        IsPunct                                        , // 132  "„"
        IsNotLineBegin|IsPunct                         , // 133  "…"
        IsPunct                                        , // 134  "†"
        IsPunct                                        , // 135  "‡"
        IsPunct                                        , // 136  "€"
        IsPunct                                        , // 137  "‰"
        IsPunct                                        , // 138  "Љ"
        IsPunct                                        , // 139  "‹"
        IsPunct                                        , // 140  "Њ"
        IsPunct                                        , // 141  "Ќ"
        IsPunct                                        , // 142  "Ћ"
        IsPunct                                        , // 143  "Џ"
        IsPunct                                        , // 144  "ђ"
        IsPunct                                        , // 145  "‘"
        IsPunct                                        , // 146  "’"
        IsPunct                                        , // 147  "“"
        IsPunct                                        , // 148  "”"
        IsPunct                                        , // 149  "•"
        IsPunct                                        , // 150  "–"
        IsPunct                                        , // 151  "—"
        IsPunct                                        , // 152  ""
        IsPunct                                        , // 153  "™"
        IsPunct                                        , // 154  "љ"
        IsPunct                                        , // 155  "›"
        IsPunct                                        , // 156  "њ"
        IsPunct                                        , // 157  "ќ"
        IsPunct                                        , // 158  "ћ"
        IsPunct                                        , // 159  "џ"
        IsPunct                                        , // 160  " "
        IsPunct                                        , // 161  "Ў"
        IsPunct                                        , // 162  "ў"
        IsPunct                                        , // 163  "Ј"
        IsPunct                                        , // 164  "¤"
        IsPunct                                        , // 165  "Ґ"
        IsPunct                                        , // 166  "¦"
        IsPunct                                        , // 167  "§"
        IsAlpha|IsUpper                                , // 168  "Ё"
        IsPunct                                        , // 169  "©"
        IsPunct                                        , // 170  "Є"
        IsPunct                                        , // 171  "«"
        IsPunct                                        , // 172  "¬"
        IsPunct                                        , // 173  "­"
        IsPunct                                        , // 174  "®"
        IsPunct                                        , // 175  "Ї"
        IsPunct                                        , // 176  "°"
        IsPunct                                        , // 177  "±"
        IsPunct                                        , // 178  "І"
        IsPunct                                        , // 179  "і"
        IsPunct                                        , // 180  "ґ"
        IsPunct                                        , // 181  "µ"
        IsPunct                                        , // 182  "¶"
        IsPunct                                        , // 183  "·"
        IsAlpha|IsLower                                , // 184  "ё"
        IsPunct                                        , // 185  "№"
        IsPunct                                        , // 186  "є"
        IsPunct                                        , // 187  "»"
        IsPunct                                        , // 188  "ј"
        IsPunct                                        , // 189  "Ѕ"
        IsPunct                                        , // 190  "ѕ"
        IsPunct                                        , // 191  "ї"
        IsAlpha|IsUpper                                , // 192  "А"
        IsAlpha|IsUpper                                , // 193  "Б"
        IsAlpha|IsUpper                                , // 194  "В"
        IsAlpha|IsUpper                                , // 195  "Г"
        IsAlpha|IsUpper                                , // 196  "Д"
        IsAlpha|IsUpper                                , // 197  "Е"
        IsAlpha|IsUpper                                , // 198  "Ж"
        IsAlpha|IsUpper                                , // 199  "З"
        IsAlpha|IsUpper                                , // 200  "И"
        IsAlpha|IsUpper                                , // 201  "Й"
        IsAlpha|IsUpper                                , // 202  "К"
        IsAlpha|IsUpper                                , // 203  "Л"
        IsAlpha|IsUpper                                , // 204  "М"
        IsAlpha|IsUpper                                , // 205  "Н"
        IsAlpha|IsUpper                                , // 206  "О"
        IsAlpha|IsUpper                                , // 207  "П"
        IsAlpha|IsUpper                                , // 208  "Р"
        IsAlpha|IsUpper                                , // 209  "С"
        IsAlpha|IsUpper                                , // 210  "Т"
        IsAlpha|IsUpper                                , // 211  "У"
        IsAlpha|IsUpper                                , // 212  "Ф"
        IsAlpha|IsUpper                                , // 213  "Х"
        IsAlpha|IsUpper                                , // 214  "Ц"
        IsAlpha|IsUpper                                , // 215  "Ч"
        IsAlpha|IsUpper                                , // 216  "Ш"
        IsAlpha|IsUpper                                , // 217  "Щ"
        IsAlpha|IsUpper                                , // 218  "Ъ"
        IsAlpha|IsUpper                                , // 219  "Ы"
        IsAlpha|IsUpper                                , // 220  "Ь"
        IsAlpha|IsUpper                                , // 221  "Э"
        IsAlpha|IsUpper                                , // 222  "Ю"
        IsAlpha|IsUpper                                , // 223  "Я"
        IsAlpha|IsLower                                , // 224  "а"
        IsAlpha|IsLower                                , // 225  "б"
        IsAlpha|IsLower                                , // 226  "в"
        IsAlpha|IsLower                                , // 227  "г"
        IsAlpha|IsLower                                , // 228  "д"
        IsAlpha|IsLower                                , // 229  "е"
        IsAlpha|IsLower                                , // 230  "ж"
        IsAlpha|IsLower                                , // 231  "з"
        IsAlpha|IsLower                                , // 232  "и"
        IsAlpha|IsLower                                , // 233  "й"
        IsAlpha|IsLower                                , // 234  "к"
        IsAlpha|IsLower                                , // 235  "л"
        IsAlpha|IsLower                                , // 236  "м"
        IsAlpha|IsLower                                , // 237  "н"
        IsAlpha|IsLower                                , // 238  "о"
        IsAlpha|IsLower                                , // 239  "п"
        IsAlpha|IsLower                                , // 240  "р"
        IsAlpha|IsLower                                , // 241  "с"
        IsAlpha|IsLower                                , // 242  "т"
        IsAlpha|IsLower                                , // 243  "у"
        IsAlpha|IsLower                                , // 244  "ф"
        IsAlpha|IsLower                                , // 245  "х"
        IsAlpha|IsLower                                , // 246  "ц"
        IsAlpha|IsLower                                , // 247  "ч"
        IsAlpha|IsLower                                , // 248  "ш"
        IsAlpha|IsLower                                , // 249  "щ"
        IsAlpha|IsLower                                , // 250  "ъ"
        IsAlpha|IsLower                                , // 251  "ы"
        IsAlpha|IsLower                                , // 252  "ь"
        IsAlpha|IsLower                                , // 253  "э"
        IsAlpha|IsLower                                , // 254  "ю"
        IsAlpha|IsLower                                  // 255  "я"
};

int compare  (unsigned char c, int mask) {
    return  ( ( (symbols_table[ (unsigned int)c] & mask) == mask) ? 1 : 0);
}