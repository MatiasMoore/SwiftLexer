/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SWIFT_TAB_H_INCLUDED
# define YY_YY_SWIFT_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    LITERAL_FLOAT = 258,           /* LITERAL_FLOAT  */
    LITERAL_INT = 259,             /* LITERAL_INT  */
    LITERAL_STRING = 260,          /* LITERAL_STRING  */
    ID = 261,                      /* ID  */
    HI = 262,                      /* HI  */
    BYE = 263,                     /* BYE  */
    ASSOCIATED_TYPE = 264,         /* ASSOCIATED_TYPE  */
    CLASS = 265,                   /* CLASS  */
    DEINIT = 266,                  /* DEINIT  */
    ENUM = 267,                    /* ENUM  */
    EXTENSION = 268,               /* EXTENSION  */
    FILEPRIVATE = 269,             /* FILEPRIVATE  */
    IMPORT = 270,                  /* IMPORT  */
    INIT = 271,                    /* INIT  */
    INOUT = 272,                   /* INOUT  */
    INTERNAL = 273,                /* INTERNAL  */
    LET = 274,                     /* LET  */
    OPEN = 275,                    /* OPEN  */
    OPERATOR = 276,                /* OPERATOR  */
    PRIVATE = 277,                 /* PRIVATE  */
    PRECEDENCEGROUP = 278,         /* PRECEDENCEGROUP  */
    PROTOCOL = 279,                /* PROTOCOL  */
    PUBLIC = 280,                  /* PUBLIC  */
    RETHROWS = 281,                /* RETHROWS  */
    STATIC = 282,                  /* STATIC  */
    STRUCT = 283,                  /* STRUCT  */
    SUBSCRIPT = 284,               /* SUBSCRIPT  */
    TYPEALIAS = 285,               /* TYPEALIAS  */
    FUNC = 286,                    /* FUNC  */
    VAR = 287,                     /* VAR  */
    BREAK = 288,                   /* BREAK  */
    CASE = 289,                    /* CASE  */
    CATCH = 290,                   /* CATCH  */
    CONTINUE = 291,                /* CONTINUE  */
    DEFAULT = 292,                 /* DEFAULT  */
    DEFER = 293,                   /* DEFER  */
    DO = 294,                      /* DO  */
    ELSE = 295,                    /* ELSE  */
    FALLTHROUGH = 296,             /* FALLTHROUGH  */
    FOR = 297,                     /* FOR  */
    GUARD = 298,                   /* GUARD  */
    IF = 299,                      /* IF  */
    IN = 300,                      /* IN  */
    REPEAT = 301,                  /* REPEAT  */
    RETURN = 302,                  /* RETURN  */
    THROW = 303,                   /* THROW  */
    SWITCH = 304,                  /* SWITCH  */
    WHERE = 305,                   /* WHERE  */
    WHILE = 306,                   /* WHILE  */
    ANY = 307,                     /* ANY  */
    AS = 308,                      /* AS  */
    FALSE = 309,                   /* FALSE  */
    IS = 310,                      /* IS  */
    NIL = 311,                     /* NIL  */
    SELF = 312,                    /* SELF  */
    SELF_TYPE = 313,               /* SELF_TYPE  */
    SUPER = 314,                   /* SUPER  */
    THROWS = 315,                  /* THROWS  */
    TRUE = 316,                    /* TRUE  */
    TRY = 317,                     /* TRY  */
    ASSOCIATIVITY = 318,           /* ASSOCIATIVITY  */
    CONVENIENCE = 319,             /* CONVENIENCE  */
    DIDSET = 320,                  /* DIDSET  */
    DYNAMIC = 321,                 /* DYNAMIC  */
    FINAL = 322,                   /* FINAL  */
    GET = 323,                     /* GET  */
    INDIRECT = 324,                /* INDIRECT  */
    INFIX = 325,                   /* INFIX  */
    LAZY = 326,                    /* LAZY  */
    LEFT = 327,                    /* LEFT  */
    MUTATING = 328,                /* MUTATING  */
    NONE = 329,                    /* NONE  */
    NONMUTATING = 330,             /* NONMUTATING  */
    OPTIONAL = 331,                /* OPTIONAL  */
    OVERRIDE = 332,                /* OVERRIDE  */
    POSTFIX = 333,                 /* POSTFIX  */
    PRECEDENCE = 334,              /* PRECEDENCE  */
    PREFIX = 335,                  /* PREFIX  */
    PROTOCOL_UPPERCASE = 336,      /* PROTOCOL_UPPERCASE  */
    REQUIRED = 337,                /* REQUIRED  */
    RIGHT = 338,                   /* RIGHT  */
    SET = 339,                     /* SET  */
    SOME = 340,                    /* SOME  */
    TYPE = 341,                    /* TYPE  */
    UNOWNED = 342,                 /* UNOWNED  */
    WEAK = 343,                    /* WEAK  */
    WILLSET = 344,                 /* WILLSET  */
    HASH_AVAILABLE = 345,          /* HASH_AVAILABLE  */
    HASH_COLORLITERAL = 346,       /* HASH_COLORLITERAL  */
    HASH_COLUMN = 347,             /* HASH_COLUMN  */
    HASH_DSOHANDLE = 348,          /* HASH_DSOHANDLE  */
    HASH_ELSEIF = 349,             /* HASH_ELSEIF  */
    HASH_ELSE = 350,               /* HASH_ELSE  */
    HASH_ENDIF = 351,              /* HASH_ENDIF  */
    HASH_ERROR = 352,              /* HASH_ERROR  */
    HASH_FILEID = 353,             /* HASH_FILEID  */
    HASH_FILELITERAL = 354,        /* HASH_FILELITERAL  */
    HASH_FILEPATH = 355,           /* HASH_FILEPATH  */
    HASH_FILE = 356,               /* HASH_FILE  */
    HASH_FUNCTION = 357,           /* HASH_FUNCTION  */
    HASH_IF = 358,                 /* HASH_IF  */
    HASH_IMAGELITERAL = 359,       /* HASH_IMAGELITERAL  */
    HASH_KEYPATH = 360,            /* HASH_KEYPATH  */
    HASH_LINE = 361,               /* HASH_LINE  */
    HASH_SELECTOR = 362,           /* HASH_SELECTOR  */
    HASH_SOURCELOCATION = 363,     /* HASH_SOURCELOCATION  */
    HASH_WARNING = 364,            /* HASH_WARNING  */
    OP_PLUS_ASSIGN = 365,          /* OP_PLUS_ASSIGN  */
    OP_MINUS_ASSIGN = 366,         /* OP_MINUS_ASSIGN  */
    OP_DIV_ASSIGN = 367,           /* OP_DIV_ASSIGN  */
    OP_MUL_ASSIGN = 368,           /* OP_MUL_ASSIGN  */
    OP_MOD_ASSIGN = 369,           /* OP_MOD_ASSIGN  */
    OP_LSHIFT = 370,               /* OP_LSHIFT  */
    OP_RSHIFT = 371,               /* OP_RSHIFT  */
    OP_EQ = 372,                   /* OP_EQ  */
    OP_LTE = 373,                  /* OP_LTE  */
    OP_GTE = 374,                  /* OP_GTE  */
    OP_NEQ = 375,                  /* OP_NEQ  */
    OP_LOG_AND = 376,              /* OP_LOG_AND  */
    OP_LOG_OR = 377,               /* OP_LOG_OR  */
    OP_NIL_COALESCE = 378,         /* OP_NIL_COALESCE  */
    OP_INC = 379,                  /* OP_INC  */
    OP_DEC = 380,                  /* OP_DEC  */
    OP_CLOSED_RANGE = 381,         /* OP_CLOSED_RANGE  */
    OP_HALF_OPEN_RANGE = 382,      /* OP_HALF_OPEN_RANGE  */
    OP_FUNC_RETURN = 383,          /* OP_FUNC_RETURN  */
    TYPE_BOOL = 384,               /* TYPE_BOOL  */
    TYPE_STRING = 385,             /* TYPE_STRING  */
    TYPE_CHARACTER = 386,          /* TYPE_CHARACTER  */
    TYPE_INT8 = 387,               /* TYPE_INT8  */
    TYPE_INT16 = 388,              /* TYPE_INT16  */
    TYPE_INT32 = 389,              /* TYPE_INT32  */
    TYPE_INT64 = 390,              /* TYPE_INT64  */
    TYPE_INT = 391,                /* TYPE_INT  */
    TYPE_UINT8 = 392,              /* TYPE_UINT8  */
    TYPE_UINT16 = 393,             /* TYPE_UINT16  */
    TYPE_UINT32 = 394,             /* TYPE_UINT32  */
    TYPE_UINT64 = 395,             /* TYPE_UINT64  */
    TYPE_UINT = 396,               /* TYPE_UINT  */
    TYPE_FLOAT = 397,              /* TYPE_FLOAT  */
    TYPE_FLOAT80 = 398,            /* TYPE_FLOAT80  */
    TYPE_DOUBLE = 399,             /* TYPE_DOUBLE  */
    NEWLINE = 400                  /* NEWLINE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 11 "swift.y"

	int ival;
	float fval;
	double dval;
    char* strval;

#line 216 "swift.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SWIFT_TAB_H_INCLUDED  */
