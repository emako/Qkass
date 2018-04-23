#ifndef ASS_TAG_H
#define ASS_TAG_H

#define QT_PASS do{ }while(false)

/*EOL str of ASS.*/
static const char ASS_TAG_HARD_EOL[]  = "\\N";
static const char ASS_TAG_SOFT_EOL[]  = "\\n";
static const char ASS_TAG_BLANK[]     = "\\h";
static const char ASS_TAG_BLANK2[]    = "　";
static const char ASS_TAG_SKIP_FLAG[] = "\\";
static const int ASS_TAG_SKIP_NUM     = 2;

/*KARAOKE tag of ASS.*/
static const char ASS_TAG_KARAOKE_K_LOWER[] = "\\k";
static const char ASS_TAG_KARAOKE_K_UPPER[] = "\\K";
static const char ASS_TAG_KARAOKE_KF[]      = "\\kf";
static const char ASS_TAG_KARAOKE_KO[]      = "\\ko";

/*Ext of ASS.*/
static const char ASS_EXT_TYPE_A1[] = "ass";
static const char ASS_EXT_TYPE_B1[] = "ssa";
static const char ASS_EXT_TYPE_C1[] = "txt";
static const char ASS_EXT_TYPE_A2[] = ".ass";
static const char ASS_EXT_TYPE_B2[] = ".ssa";
static const char ASS_EXT_TYPE_C2[] = ".txt";

/*AssTt tool.*/
static const char ASS_TAG_TT[]                    = "\\t";
static const char ASS_TAG_STR_TT[]                = "%1(%2,%3,%4)";

/*Other str of ASS.*/
static const char ASS_TAG_EVENT_FORMAT_DIALOGUE[] = "Dialogue";
static const char ASS_TAG_EVENT_FORMAT_COMMENT[]  = "Comment";
static const char ASS_TAG_FORMAT[]                = "Format";
static const char ASS_TAG_STYLE[]                 = "Style";
static const char ASS_TAG_TIMECODE_FORMAT[]       = "h:mm:ss.z";
static const char ASS_TAG_COMMA[]                 = ",";
static const char ASS_TAG_SAVE_NAME[]             = "_k";
static const char ASS_TAG_BRACE_REGEXP[]          = "(\\{)[^}]*(\\})";
static const char ASS_TAG_COMMENT_SCRIPT_REGEXP[] = "^(Comment.*)";
static const char ASS_TAG_FORMAT_REGEXP[]         = "^(Format.*)";
static const char ASS_TAG_STYLE_REGEXP[]          = "^(Style.*)";
static const char ASS_TAG_COMMENT_TEXT_REGEXP[]   = "^(;.*)";
static const char ASS_TAG_SECTION_REGEXP[]        = "^(\\[.*\\])";
static const char ASS_TAG_KEYWORD_QSTR[]          = "(%1)";

static const int QT_INDEX_NOT_FIND    = -1;
static const int QT_INDEX_0           = 0;
static const int QT_INDEX_1           = 1;
static const int QT_INDEX_2           = 2;
static const int QT_INDEX_3           = 3;
static const int QT_INDEX_4           = 4;
static const int QT_INDEX_5           = 5;
static const int QT_INDEX_6           = 6;
static const int QT_INDEX_7           = 7;
static const int QT_FONT_SIZE         = 10;

static const char QT_EMPTY[]          = "";
static const char QT_BLANK[]          = " ";
static const char QT_EXT_SPLITE[]     = ".";
static const char QT_NOR_EOL[]        = "\n";
static const char QT_OTR_EOL[]        = "\r\n";
static const char QT_META[]           = "\\b";
static const char QT_FILE_PROTOCOL[]  = "file:///";
static const char QT_STR_ARG_1[]      = "%1";
static const char QT_STR_ARG_2[]      = "%1%2";
static const char QT_STR_ARG_3[]      = "%1%2%3";
static const char QT_STR_TAG[]        = "{%1%2}";
static const char QT_STR_TAG_OTR[]    = "{%1}";
static const char QT_STR_TAG_TEXT[]   = "{%1%2}%3";

/*App info.*/
static const char APP_NAME[]            = "Qkass";
static const char  APP_VERSION[]        = "r2";

/*Tools info.*/
static const char  APP_TOOLS_ASSTT[]    = "AssTt";
static const char  APP_TOOLS_ASS2TEXT[] = "Ass2text";

enum class AssEvents{
    Layer = 0,
    Start,
    End,
    Style,
    Name,
    MarginL,
    MarginR,
    MarginV,
    Effect,
    Text,
    MaxEvent,
};

enum class CallK{
    Average,
    Fixed,
    Other,
    MaxCallK,
};

#endif // ASS_TAG_H
