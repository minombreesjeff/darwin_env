/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END_OF_INPUT = 258,
     CLOSE_SYM = 259,
     HANDLER_SYM = 260,
     LAST_SYM = 261,
     NEXT_SYM = 262,
     PREV_SYM = 263,
     DIV_SYM = 264,
     EQ = 265,
     EQUAL_SYM = 266,
     SOUNDS_SYM = 267,
     GE = 268,
     GT_SYM = 269,
     LE = 270,
     LT = 271,
     NE = 272,
     IS = 273,
     MOD_SYM = 274,
     SHIFT_LEFT = 275,
     SHIFT_RIGHT = 276,
     SET_VAR = 277,
     ABORT_SYM = 278,
     ADD = 279,
     AFTER_SYM = 280,
     ALTER = 281,
     ANALYZE_SYM = 282,
     ANY_SYM = 283,
     AVG_SYM = 284,
     BEGIN_SYM = 285,
     BINLOG_SYM = 286,
     CHANGE = 287,
     CLIENT_SYM = 288,
     COMMENT_SYM = 289,
     COMMIT_SYM = 290,
     CONSISTENT_SYM = 291,
     COUNT_SYM = 292,
     CREATE = 293,
     CROSS = 294,
     CUBE_SYM = 295,
     DELETE_SYM = 296,
     DUAL_SYM = 297,
     DO_SYM = 298,
     DROP = 299,
     EVENTS_SYM = 300,
     EXECUTE_SYM = 301,
     EXPANSION_SYM = 302,
     FLUSH_SYM = 303,
     HELP_SYM = 304,
     INSERT = 305,
     RELAY_THREAD = 306,
     KILL_SYM = 307,
     LOAD = 308,
     LOCKS_SYM = 309,
     LOCK_SYM = 310,
     MASTER_SYM = 311,
     MAX_SYM = 312,
     MIN_SYM = 313,
     NONE_SYM = 314,
     OPTIMIZE = 315,
     PURGE = 316,
     REPAIR = 317,
     REPLICATION = 318,
     RESET_SYM = 319,
     ROLLBACK_SYM = 320,
     ROLLUP_SYM = 321,
     SAVEPOINT_SYM = 322,
     SELECT_SYM = 323,
     SHOW = 324,
     SLAVE = 325,
     SNAPSHOT_SYM = 326,
     SQL_THREAD = 327,
     START_SYM = 328,
     STD_SYM = 329,
     VARIANCE_SYM = 330,
     STOP_SYM = 331,
     SUM_SYM = 332,
     ADDDATE_SYM = 333,
     SUPER_SYM = 334,
     TRUNCATE_SYM = 335,
     UNLOCK_SYM = 336,
     UNTIL_SYM = 337,
     UPDATE_SYM = 338,
     ACTION = 339,
     AGGREGATE_SYM = 340,
     ALL = 341,
     AND_SYM = 342,
     AS = 343,
     ASC = 344,
     AUTO_INC = 345,
     AVG_ROW_LENGTH = 346,
     BACKUP_SYM = 347,
     BERKELEY_DB_SYM = 348,
     BINARY = 349,
     BIT_SYM = 350,
     BOOL_SYM = 351,
     BOOLEAN_SYM = 352,
     BOTH = 353,
     BTREE_SYM = 354,
     BY = 355,
     BYTE_SYM = 356,
     CACHE_SYM = 357,
     CASCADE = 358,
     CAST_SYM = 359,
     CHARSET = 360,
     CHECKSUM_SYM = 361,
     CHECK_SYM = 362,
     COMMITTED_SYM = 363,
     COLLATE_SYM = 364,
     COLLATION_SYM = 365,
     COLUMNS = 366,
     COLUMN_SYM = 367,
     CONCURRENT = 368,
     CONSTRAINT = 369,
     CONVERT_SYM = 370,
     CURRENT_USER = 371,
     DATABASES = 372,
     DATA_SYM = 373,
     DEFAULT = 374,
     DELAYED_SYM = 375,
     DELAY_KEY_WRITE_SYM = 376,
     DESC = 377,
     DESCRIBE = 378,
     DES_KEY_FILE = 379,
     DISABLE_SYM = 380,
     DISCARD = 381,
     DISTINCT = 382,
     DUPLICATE_SYM = 383,
     DYNAMIC_SYM = 384,
     ENABLE_SYM = 385,
     ENCLOSED = 386,
     ESCAPED = 387,
     DIRECTORY_SYM = 388,
     ESCAPE_SYM = 389,
     EXISTS = 390,
     EXTENDED_SYM = 391,
     FALSE_SYM = 392,
     FILE_SYM = 393,
     FIRST_SYM = 394,
     FIXED_SYM = 395,
     FLOAT_NUM = 396,
     FORCE_SYM = 397,
     FOREIGN = 398,
     FROM = 399,
     FULL = 400,
     FULLTEXT_SYM = 401,
     GLOBAL_SYM = 402,
     GRANT = 403,
     GRANTS = 404,
     GREATEST_SYM = 405,
     GROUP = 406,
     HAVING = 407,
     HASH_SYM = 408,
     HEX_NUM = 409,
     HIGH_PRIORITY = 410,
     HOSTS_SYM = 411,
     IDENT = 412,
     IDENT_QUOTED = 413,
     IGNORE_SYM = 414,
     IMPORT = 415,
     INDEX_SYM = 416,
     INDEXES = 417,
     INFILE = 418,
     INNER_SYM = 419,
     INNOBASE_SYM = 420,
     INTO = 421,
     IN_SYM = 422,
     ISOLATION = 423,
     JOIN_SYM = 424,
     KEYS = 425,
     KEY_SYM = 426,
     LEADING = 427,
     LEAST_SYM = 428,
     LEAVES = 429,
     LEVEL_SYM = 430,
     LEX_HOSTNAME = 431,
     LIKE = 432,
     LINES = 433,
     LOCAL_SYM = 434,
     LOG_SYM = 435,
     LOGS_SYM = 436,
     LONG_NUM = 437,
     LONG_SYM = 438,
     LOW_PRIORITY = 439,
     MASTER_HOST_SYM = 440,
     MASTER_USER_SYM = 441,
     MASTER_LOG_FILE_SYM = 442,
     MASTER_LOG_POS_SYM = 443,
     MASTER_PASSWORD_SYM = 444,
     MASTER_PORT_SYM = 445,
     MASTER_CONNECT_RETRY_SYM = 446,
     MASTER_SERVER_ID_SYM = 447,
     MASTER_SSL_SYM = 448,
     MASTER_SSL_CA_SYM = 449,
     MASTER_SSL_CAPATH_SYM = 450,
     MASTER_SSL_CERT_SYM = 451,
     MASTER_SSL_CIPHER_SYM = 452,
     MASTER_SSL_KEY_SYM = 453,
     RELAY_LOG_FILE_SYM = 454,
     RELAY_LOG_POS_SYM = 455,
     MATCH = 456,
     MAX_ROWS = 457,
     MAX_CONNECTIONS_PER_HOUR = 458,
     MAX_QUERIES_PER_HOUR = 459,
     MAX_UPDATES_PER_HOUR = 460,
     MEDIUM_SYM = 461,
     MIN_ROWS = 462,
     NAMES_SYM = 463,
     NATIONAL_SYM = 464,
     NATURAL = 465,
     NDBCLUSTER_SYM = 466,
     NEW_SYM = 467,
     NCHAR_SYM = 468,
     NCHAR_STRING = 469,
     NVARCHAR_SYM = 470,
     NOT = 471,
     NO_SYM = 472,
     NULL_SYM = 473,
     NUM = 474,
     OFFSET_SYM = 475,
     ON = 476,
     ONE_SHOT_SYM = 477,
     OPEN_SYM = 478,
     OPTION = 479,
     OPTIONALLY = 480,
     OR_SYM = 481,
     OR_OR_CONCAT = 482,
     ORDER_SYM = 483,
     OUTER = 484,
     OUTFILE = 485,
     DUMPFILE = 486,
     PACK_KEYS_SYM = 487,
     PARTIAL = 488,
     PRIMARY_SYM = 489,
     PRIVILEGES = 490,
     PROCESS = 491,
     PROCESSLIST_SYM = 492,
     QUERY_SYM = 493,
     RAID_0_SYM = 494,
     RAID_STRIPED_SYM = 495,
     RAID_TYPE = 496,
     RAID_CHUNKS = 497,
     RAID_CHUNKSIZE = 498,
     READ_SYM = 499,
     REAL_NUM = 500,
     REFERENCES = 501,
     REGEXP = 502,
     RELOAD = 503,
     RENAME = 504,
     REPEATABLE_SYM = 505,
     REQUIRE_SYM = 506,
     RESOURCES = 507,
     RESTORE_SYM = 508,
     RESTRICT = 509,
     REVOKE = 510,
     ROWS_SYM = 511,
     ROW_FORMAT_SYM = 512,
     ROW_SYM = 513,
     RTREE_SYM = 514,
     SET = 515,
     SEPARATOR_SYM = 516,
     SERIAL_SYM = 517,
     SERIALIZABLE_SYM = 518,
     SESSION_SYM = 519,
     SIMPLE_SYM = 520,
     SHUTDOWN = 521,
     SPATIAL_SYM = 522,
     SSL_SYM = 523,
     STARTING = 524,
     STATUS_SYM = 525,
     STORAGE_SYM = 526,
     STRAIGHT_JOIN = 527,
     SUBJECT_SYM = 528,
     TABLES = 529,
     TABLE_SYM = 530,
     TABLESPACE = 531,
     TEMPORARY = 532,
     TERMINATED = 533,
     TEXT_STRING = 534,
     TO_SYM = 535,
     TRAILING = 536,
     TRANSACTION_SYM = 537,
     TRUE_SYM = 538,
     TYPE_SYM = 539,
     TYPES_SYM = 540,
     FUNC_ARG0 = 541,
     FUNC_ARG1 = 542,
     FUNC_ARG2 = 543,
     FUNC_ARG3 = 544,
     UDF_RETURNS_SYM = 545,
     UDF_SONAME_SYM = 546,
     UDF_SYM = 547,
     UNCOMMITTED_SYM = 548,
     UNDERSCORE_CHARSET = 549,
     UNICODE_SYM = 550,
     UNION_SYM = 551,
     UNIQUE_SYM = 552,
     USAGE = 553,
     USE_FRM = 554,
     USE_SYM = 555,
     USING = 556,
     VALUE_SYM = 557,
     VALUES = 558,
     VARIABLES = 559,
     WHERE = 560,
     WITH = 561,
     WRITE_SYM = 562,
     NO_WRITE_TO_BINLOG = 563,
     X509_SYM = 564,
     XOR = 565,
     COMPRESSED_SYM = 566,
     ERRORS = 567,
     WARNINGS = 568,
     ASCII_SYM = 569,
     BIGINT = 570,
     BLOB_SYM = 571,
     CHAR_SYM = 572,
     CHANGED = 573,
     COALESCE = 574,
     DATETIME = 575,
     DATE_SYM = 576,
     DECIMAL_SYM = 577,
     DOUBLE_SYM = 578,
     ENUM = 579,
     FAST_SYM = 580,
     FLOAT_SYM = 581,
     GEOMETRY_SYM = 582,
     INT_SYM = 583,
     LIMIT = 584,
     LONGBLOB = 585,
     LONGTEXT = 586,
     MEDIUMBLOB = 587,
     MEDIUMINT = 588,
     MEDIUMTEXT = 589,
     NUMERIC_SYM = 590,
     PRECISION = 591,
     PREPARE_SYM = 592,
     DEALLOCATE_SYM = 593,
     QUICK = 594,
     REAL = 595,
     SIGNED_SYM = 596,
     SMALLINT = 597,
     STRING_SYM = 598,
     TEXT_SYM = 599,
     TIMESTAMP = 600,
     TIME_SYM = 601,
     TINYBLOB = 602,
     TINYINT = 603,
     TINYTEXT = 604,
     ULONGLONG_NUM = 605,
     UNSIGNED = 606,
     VARBINARY = 607,
     VARCHAR = 608,
     VARYING = 609,
     ZEROFILL = 610,
     AGAINST = 611,
     ATAN = 612,
     BETWEEN_SYM = 613,
     BIT_AND = 614,
     BIT_OR = 615,
     BIT_XOR = 616,
     CASE_SYM = 617,
     CONCAT = 618,
     CONCAT_WS = 619,
     CONVERT_TZ_SYM = 620,
     CURDATE = 621,
     CURTIME = 622,
     DATABASE = 623,
     DATE_ADD_INTERVAL = 624,
     DATE_SUB_INTERVAL = 625,
     DAY_HOUR_SYM = 626,
     DAY_MICROSECOND_SYM = 627,
     DAY_MINUTE_SYM = 628,
     DAY_SECOND_SYM = 629,
     DAY_SYM = 630,
     DECODE_SYM = 631,
     DES_ENCRYPT_SYM = 632,
     DES_DECRYPT_SYM = 633,
     ELSE = 634,
     ELT_FUNC = 635,
     ENCODE_SYM = 636,
     ENGINE_SYM = 637,
     ENGINES_SYM = 638,
     ENCRYPT = 639,
     EXPORT_SET = 640,
     EXTRACT_SYM = 641,
     FIELD_FUNC = 642,
     FORMAT_SYM = 643,
     FOR_SYM = 644,
     FROM_UNIXTIME = 645,
     GEOMCOLLFROMTEXT = 646,
     GEOMFROMTEXT = 647,
     GEOMFROMWKB = 648,
     GEOMETRYCOLLECTION = 649,
     GROUP_CONCAT_SYM = 650,
     GROUP_UNIQUE_USERS = 651,
     GET_FORMAT = 652,
     HOUR_MICROSECOND_SYM = 653,
     HOUR_MINUTE_SYM = 654,
     HOUR_SECOND_SYM = 655,
     HOUR_SYM = 656,
     IDENTIFIED_SYM = 657,
     IF = 658,
     INSERT_METHOD = 659,
     INTERVAL_SYM = 660,
     LAST_INSERT_ID = 661,
     LEFT = 662,
     LINEFROMTEXT = 663,
     LINESTRING = 664,
     LOCATE = 665,
     MAKE_SET_SYM = 666,
     MASTER_POS_WAIT = 667,
     MICROSECOND_SYM = 668,
     MINUTE_MICROSECOND_SYM = 669,
     MINUTE_SECOND_SYM = 670,
     MINUTE_SYM = 671,
     MODE_SYM = 672,
     MODIFY_SYM = 673,
     MONTH_SYM = 674,
     MLINEFROMTEXT = 675,
     MPOINTFROMTEXT = 676,
     MPOLYFROMTEXT = 677,
     MULTILINESTRING = 678,
     MULTIPOINT = 679,
     MULTIPOLYGON = 680,
     NOW_SYM = 681,
     OLD_PASSWORD = 682,
     PASSWORD = 683,
     POINTFROMTEXT = 684,
     POINT_SYM = 685,
     POLYFROMTEXT = 686,
     POLYGON = 687,
     POSITION_SYM = 688,
     PROCEDURE = 689,
     RAND = 690,
     REPLACE = 691,
     RIGHT = 692,
     ROUND = 693,
     SECOND_SYM = 694,
     SECOND_MICROSECOND_SYM = 695,
     SHARE_SYM = 696,
     SUBDATE_SYM = 697,
     SUBSTRING = 698,
     SUBSTRING_INDEX = 699,
     TRIM = 700,
     UDA_CHAR_SUM = 701,
     UDA_FLOAT_SUM = 702,
     UDA_INT_SUM = 703,
     UDF_CHAR_FUNC = 704,
     UDF_FLOAT_FUNC = 705,
     UDF_INT_FUNC = 706,
     UNIQUE_USERS = 707,
     UNIX_TIMESTAMP = 708,
     USER = 709,
     UTC_DATE_SYM = 710,
     UTC_TIME_SYM = 711,
     UTC_TIMESTAMP_SYM = 712,
     WEEK_SYM = 713,
     WHEN_SYM = 714,
     WORK_SYM = 715,
     YEAR_MONTH_SYM = 716,
     YEAR_SYM = 717,
     YEARWEEK = 718,
     BENCHMARK_SYM = 719,
     END = 720,
     THEN_SYM = 721,
     SQL_BIG_RESULT = 722,
     SQL_CACHE_SYM = 723,
     SQL_CALC_FOUND_ROWS = 724,
     SQL_NO_CACHE_SYM = 725,
     SQL_SMALL_RESULT = 726,
     SQL_BUFFER_RESULT = 727,
     ISSUER_SYM = 728,
     CIPHER_SYM = 729,
     BEFORE_SYM = 730,
     NEG = 731
   };
#endif
#define END_OF_INPUT 258
#define CLOSE_SYM 259
#define HANDLER_SYM 260
#define LAST_SYM 261
#define NEXT_SYM 262
#define PREV_SYM 263
#define DIV_SYM 264
#define EQ 265
#define EQUAL_SYM 266
#define SOUNDS_SYM 267
#define GE 268
#define GT_SYM 269
#define LE 270
#define LT 271
#define NE 272
#define IS 273
#define MOD_SYM 274
#define SHIFT_LEFT 275
#define SHIFT_RIGHT 276
#define SET_VAR 277
#define ABORT_SYM 278
#define ADD 279
#define AFTER_SYM 280
#define ALTER 281
#define ANALYZE_SYM 282
#define ANY_SYM 283
#define AVG_SYM 284
#define BEGIN_SYM 285
#define BINLOG_SYM 286
#define CHANGE 287
#define CLIENT_SYM 288
#define COMMENT_SYM 289
#define COMMIT_SYM 290
#define CONSISTENT_SYM 291
#define COUNT_SYM 292
#define CREATE 293
#define CROSS 294
#define CUBE_SYM 295
#define DELETE_SYM 296
#define DUAL_SYM 297
#define DO_SYM 298
#define DROP 299
#define EVENTS_SYM 300
#define EXECUTE_SYM 301
#define EXPANSION_SYM 302
#define FLUSH_SYM 303
#define HELP_SYM 304
#define INSERT 305
#define RELAY_THREAD 306
#define KILL_SYM 307
#define LOAD 308
#define LOCKS_SYM 309
#define LOCK_SYM 310
#define MASTER_SYM 311
#define MAX_SYM 312
#define MIN_SYM 313
#define NONE_SYM 314
#define OPTIMIZE 315
#define PURGE 316
#define REPAIR 317
#define REPLICATION 318
#define RESET_SYM 319
#define ROLLBACK_SYM 320
#define ROLLUP_SYM 321
#define SAVEPOINT_SYM 322
#define SELECT_SYM 323
#define SHOW 324
#define SLAVE 325
#define SNAPSHOT_SYM 326
#define SQL_THREAD 327
#define START_SYM 328
#define STD_SYM 329
#define VARIANCE_SYM 330
#define STOP_SYM 331
#define SUM_SYM 332
#define ADDDATE_SYM 333
#define SUPER_SYM 334
#define TRUNCATE_SYM 335
#define UNLOCK_SYM 336
#define UNTIL_SYM 337
#define UPDATE_SYM 338
#define ACTION 339
#define AGGREGATE_SYM 340
#define ALL 341
#define AND_SYM 342
#define AS 343
#define ASC 344
#define AUTO_INC 345
#define AVG_ROW_LENGTH 346
#define BACKUP_SYM 347
#define BERKELEY_DB_SYM 348
#define BINARY 349
#define BIT_SYM 350
#define BOOL_SYM 351
#define BOOLEAN_SYM 352
#define BOTH 353
#define BTREE_SYM 354
#define BY 355
#define BYTE_SYM 356
#define CACHE_SYM 357
#define CASCADE 358
#define CAST_SYM 359
#define CHARSET 360
#define CHECKSUM_SYM 361
#define CHECK_SYM 362
#define COMMITTED_SYM 363
#define COLLATE_SYM 364
#define COLLATION_SYM 365
#define COLUMNS 366
#define COLUMN_SYM 367
#define CONCURRENT 368
#define CONSTRAINT 369
#define CONVERT_SYM 370
#define CURRENT_USER 371
#define DATABASES 372
#define DATA_SYM 373
#define DEFAULT 374
#define DELAYED_SYM 375
#define DELAY_KEY_WRITE_SYM 376
#define DESC 377
#define DESCRIBE 378
#define DES_KEY_FILE 379
#define DISABLE_SYM 380
#define DISCARD 381
#define DISTINCT 382
#define DUPLICATE_SYM 383
#define DYNAMIC_SYM 384
#define ENABLE_SYM 385
#define ENCLOSED 386
#define ESCAPED 387
#define DIRECTORY_SYM 388
#define ESCAPE_SYM 389
#define EXISTS 390
#define EXTENDED_SYM 391
#define FALSE_SYM 392
#define FILE_SYM 393
#define FIRST_SYM 394
#define FIXED_SYM 395
#define FLOAT_NUM 396
#define FORCE_SYM 397
#define FOREIGN 398
#define FROM 399
#define FULL 400
#define FULLTEXT_SYM 401
#define GLOBAL_SYM 402
#define GRANT 403
#define GRANTS 404
#define GREATEST_SYM 405
#define GROUP 406
#define HAVING 407
#define HASH_SYM 408
#define HEX_NUM 409
#define HIGH_PRIORITY 410
#define HOSTS_SYM 411
#define IDENT 412
#define IDENT_QUOTED 413
#define IGNORE_SYM 414
#define IMPORT 415
#define INDEX_SYM 416
#define INDEXES 417
#define INFILE 418
#define INNER_SYM 419
#define INNOBASE_SYM 420
#define INTO 421
#define IN_SYM 422
#define ISOLATION 423
#define JOIN_SYM 424
#define KEYS 425
#define KEY_SYM 426
#define LEADING 427
#define LEAST_SYM 428
#define LEAVES 429
#define LEVEL_SYM 430
#define LEX_HOSTNAME 431
#define LIKE 432
#define LINES 433
#define LOCAL_SYM 434
#define LOG_SYM 435
#define LOGS_SYM 436
#define LONG_NUM 437
#define LONG_SYM 438
#define LOW_PRIORITY 439
#define MASTER_HOST_SYM 440
#define MASTER_USER_SYM 441
#define MASTER_LOG_FILE_SYM 442
#define MASTER_LOG_POS_SYM 443
#define MASTER_PASSWORD_SYM 444
#define MASTER_PORT_SYM 445
#define MASTER_CONNECT_RETRY_SYM 446
#define MASTER_SERVER_ID_SYM 447
#define MASTER_SSL_SYM 448
#define MASTER_SSL_CA_SYM 449
#define MASTER_SSL_CAPATH_SYM 450
#define MASTER_SSL_CERT_SYM 451
#define MASTER_SSL_CIPHER_SYM 452
#define MASTER_SSL_KEY_SYM 453
#define RELAY_LOG_FILE_SYM 454
#define RELAY_LOG_POS_SYM 455
#define MATCH 456
#define MAX_ROWS 457
#define MAX_CONNECTIONS_PER_HOUR 458
#define MAX_QUERIES_PER_HOUR 459
#define MAX_UPDATES_PER_HOUR 460
#define MEDIUM_SYM 461
#define MIN_ROWS 462
#define NAMES_SYM 463
#define NATIONAL_SYM 464
#define NATURAL 465
#define NDBCLUSTER_SYM 466
#define NEW_SYM 467
#define NCHAR_SYM 468
#define NCHAR_STRING 469
#define NVARCHAR_SYM 470
#define NOT 471
#define NO_SYM 472
#define NULL_SYM 473
#define NUM 474
#define OFFSET_SYM 475
#define ON 476
#define ONE_SHOT_SYM 477
#define OPEN_SYM 478
#define OPTION 479
#define OPTIONALLY 480
#define OR_SYM 481
#define OR_OR_CONCAT 482
#define ORDER_SYM 483
#define OUTER 484
#define OUTFILE 485
#define DUMPFILE 486
#define PACK_KEYS_SYM 487
#define PARTIAL 488
#define PRIMARY_SYM 489
#define PRIVILEGES 490
#define PROCESS 491
#define PROCESSLIST_SYM 492
#define QUERY_SYM 493
#define RAID_0_SYM 494
#define RAID_STRIPED_SYM 495
#define RAID_TYPE 496
#define RAID_CHUNKS 497
#define RAID_CHUNKSIZE 498
#define READ_SYM 499
#define REAL_NUM 500
#define REFERENCES 501
#define REGEXP 502
#define RELOAD 503
#define RENAME 504
#define REPEATABLE_SYM 505
#define REQUIRE_SYM 506
#define RESOURCES 507
#define RESTORE_SYM 508
#define RESTRICT 509
#define REVOKE 510
#define ROWS_SYM 511
#define ROW_FORMAT_SYM 512
#define ROW_SYM 513
#define RTREE_SYM 514
#define SET 515
#define SEPARATOR_SYM 516
#define SERIAL_SYM 517
#define SERIALIZABLE_SYM 518
#define SESSION_SYM 519
#define SIMPLE_SYM 520
#define SHUTDOWN 521
#define SPATIAL_SYM 522
#define SSL_SYM 523
#define STARTING 524
#define STATUS_SYM 525
#define STORAGE_SYM 526
#define STRAIGHT_JOIN 527
#define SUBJECT_SYM 528
#define TABLES 529
#define TABLE_SYM 530
#define TABLESPACE 531
#define TEMPORARY 532
#define TERMINATED 533
#define TEXT_STRING 534
#define TO_SYM 535
#define TRAILING 536
#define TRANSACTION_SYM 537
#define TRUE_SYM 538
#define TYPE_SYM 539
#define TYPES_SYM 540
#define FUNC_ARG0 541
#define FUNC_ARG1 542
#define FUNC_ARG2 543
#define FUNC_ARG3 544
#define UDF_RETURNS_SYM 545
#define UDF_SONAME_SYM 546
#define UDF_SYM 547
#define UNCOMMITTED_SYM 548
#define UNDERSCORE_CHARSET 549
#define UNICODE_SYM 550
#define UNION_SYM 551
#define UNIQUE_SYM 552
#define USAGE 553
#define USE_FRM 554
#define USE_SYM 555
#define USING 556
#define VALUE_SYM 557
#define VALUES 558
#define VARIABLES 559
#define WHERE 560
#define WITH 561
#define WRITE_SYM 562
#define NO_WRITE_TO_BINLOG 563
#define X509_SYM 564
#define XOR 565
#define COMPRESSED_SYM 566
#define ERRORS 567
#define WARNINGS 568
#define ASCII_SYM 569
#define BIGINT 570
#define BLOB_SYM 571
#define CHAR_SYM 572
#define CHANGED 573
#define COALESCE 574
#define DATETIME 575
#define DATE_SYM 576
#define DECIMAL_SYM 577
#define DOUBLE_SYM 578
#define ENUM 579
#define FAST_SYM 580
#define FLOAT_SYM 581
#define GEOMETRY_SYM 582
#define INT_SYM 583
#define LIMIT 584
#define LONGBLOB 585
#define LONGTEXT 586
#define MEDIUMBLOB 587
#define MEDIUMINT 588
#define MEDIUMTEXT 589
#define NUMERIC_SYM 590
#define PRECISION 591
#define PREPARE_SYM 592
#define DEALLOCATE_SYM 593
#define QUICK 594
#define REAL 595
#define SIGNED_SYM 596
#define SMALLINT 597
#define STRING_SYM 598
#define TEXT_SYM 599
#define TIMESTAMP 600
#define TIME_SYM 601
#define TINYBLOB 602
#define TINYINT 603
#define TINYTEXT 604
#define ULONGLONG_NUM 605
#define UNSIGNED 606
#define VARBINARY 607
#define VARCHAR 608
#define VARYING 609
#define ZEROFILL 610
#define AGAINST 611
#define ATAN 612
#define BETWEEN_SYM 613
#define BIT_AND 614
#define BIT_OR 615
#define BIT_XOR 616
#define CASE_SYM 617
#define CONCAT 618
#define CONCAT_WS 619
#define CONVERT_TZ_SYM 620
#define CURDATE 621
#define CURTIME 622
#define DATABASE 623
#define DATE_ADD_INTERVAL 624
#define DATE_SUB_INTERVAL 625
#define DAY_HOUR_SYM 626
#define DAY_MICROSECOND_SYM 627
#define DAY_MINUTE_SYM 628
#define DAY_SECOND_SYM 629
#define DAY_SYM 630
#define DECODE_SYM 631
#define DES_ENCRYPT_SYM 632
#define DES_DECRYPT_SYM 633
#define ELSE 634
#define ELT_FUNC 635
#define ENCODE_SYM 636
#define ENGINE_SYM 637
#define ENGINES_SYM 638
#define ENCRYPT 639
#define EXPORT_SET 640
#define EXTRACT_SYM 641
#define FIELD_FUNC 642
#define FORMAT_SYM 643
#define FOR_SYM 644
#define FROM_UNIXTIME 645
#define GEOMCOLLFROMTEXT 646
#define GEOMFROMTEXT 647
#define GEOMFROMWKB 648
#define GEOMETRYCOLLECTION 649
#define GROUP_CONCAT_SYM 650
#define GROUP_UNIQUE_USERS 651
#define GET_FORMAT 652
#define HOUR_MICROSECOND_SYM 653
#define HOUR_MINUTE_SYM 654
#define HOUR_SECOND_SYM 655
#define HOUR_SYM 656
#define IDENTIFIED_SYM 657
#define IF 658
#define INSERT_METHOD 659
#define INTERVAL_SYM 660
#define LAST_INSERT_ID 661
#define LEFT 662
#define LINEFROMTEXT 663
#define LINESTRING 664
#define LOCATE 665
#define MAKE_SET_SYM 666
#define MASTER_POS_WAIT 667
#define MICROSECOND_SYM 668
#define MINUTE_MICROSECOND_SYM 669
#define MINUTE_SECOND_SYM 670
#define MINUTE_SYM 671
#define MODE_SYM 672
#define MODIFY_SYM 673
#define MONTH_SYM 674
#define MLINEFROMTEXT 675
#define MPOINTFROMTEXT 676
#define MPOLYFROMTEXT 677
#define MULTILINESTRING 678
#define MULTIPOINT 679
#define MULTIPOLYGON 680
#define NOW_SYM 681
#define OLD_PASSWORD 682
#define PASSWORD 683
#define POINTFROMTEXT 684
#define POINT_SYM 685
#define POLYFROMTEXT 686
#define POLYGON 687
#define POSITION_SYM 688
#define PROCEDURE 689
#define RAND 690
#define REPLACE 691
#define RIGHT 692
#define ROUND 693
#define SECOND_SYM 694
#define SECOND_MICROSECOND_SYM 695
#define SHARE_SYM 696
#define SUBDATE_SYM 697
#define SUBSTRING 698
#define SUBSTRING_INDEX 699
#define TRIM 700
#define UDA_CHAR_SUM 701
#define UDA_FLOAT_SUM 702
#define UDA_INT_SUM 703
#define UDF_CHAR_FUNC 704
#define UDF_FLOAT_FUNC 705
#define UDF_INT_FUNC 706
#define UNIQUE_USERS 707
#define UNIX_TIMESTAMP 708
#define USER 709
#define UTC_DATE_SYM 710
#define UTC_TIME_SYM 711
#define UTC_TIMESTAMP_SYM 712
#define WEEK_SYM 713
#define WHEN_SYM 714
#define WORK_SYM 715
#define YEAR_MONTH_SYM 716
#define YEAR_SYM 717
#define YEARWEEK 718
#define BENCHMARK_SYM 719
#define END 720
#define THEN_SYM 721
#define SQL_BIG_RESULT 722
#define SQL_CACHE_SYM 723
#define SQL_CALC_FOUND_ROWS 724
#define SQL_NO_CACHE_SYM 725
#define SQL_SMALL_RESULT 726
#define SQL_BUFFER_RESULT 727
#define ISSUER_SYM 728
#define CIPHER_SYM 729
#define BEFORE_SYM 730
#define NEG 731




/* Copy the first part of user declarations.  */
#line 19 "sql_yacc.yy"

/* thd is passed as an arg to yyparse(), and subsequently to yylex().
** The type will be void*, so it must be  cast to (THD*) when used.
** Use the YYTHD macro for this.
*/
#define YYPARSE_PARAM yythd
#define YYLEX_PARAM yythd
#define YYTHD ((THD *)yythd)

#define MYSQL_YACC
#define YYINITDEPTH 100
#define YYMAXDEPTH 3200				/* Because of 64K stack */
#define Lex (YYTHD->lex)
#define Select Lex->current_select
#include "mysql_priv.h"
#include "slave.h"
#include "lex_symbol.h"
#include "item_create.h"
#include <myisam.h>
#include <myisammrg.h>

int yylex(void *yylval, void *yythd);

#define yyoverflow(A,B,C,D,E,F) {ulong val= *(F); if(my_yyoverflow((B), (D), &val)) { yyerror((char*) (A)); return 2; } else { *(F)= (YYSIZE_T)val; }}

#define WARN_DEPRECATED(A,B) \
  push_warning_printf(((THD *)yythd), MYSQL_ERROR::WARN_LEVEL_WARN, \
		      ER_WARN_DEPRECATED_SYNTAX, \
		      ER(ER_WARN_DEPRECATED_SYNTAX), (A), (B)); 

inline Item *or_or_concat(THD *thd, Item* A, Item* B)
{
  return (thd->variables.sql_mode & MODE_PIPES_AS_CONCAT ?
          (Item*) new Item_func_concat(A,B) : (Item*) new Item_cond_or(A,B));
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 56 "sql_yacc.yy"
typedef union YYSTYPE {
  int  num;
  ulong ulong_num;
  ulonglong ulonglong_number;
  LEX_STRING lex_str;
  LEX_STRING *lex_str_ptr;
  LEX_SYMBOL symbol;
  Table_ident *table;
  char *simple_string;
  Item *item;
  Item_num *item_num;
  List<Item> *item_list;
  List<String> *string_list;
  String *string;
  key_part_spec *key_part;
  TABLE_LIST *table_list;
  udf_func *udf;
  LEX_USER *lex_user;
  struct sys_var_with_base variable;
  Key::Keytype key_type;
  enum ha_key_alg key_alg;
  enum db_type db_type;
  enum row_type row_type;
  enum ha_rkey_function ha_rkey_mode;
  enum enum_tx_isolation tx_isolation;
  enum Cast_target cast_type;
  enum Item_udftype udf_type;
  CHARSET_INFO *charset;
  thr_lock_type lock_type;
  interval_type interval;
  timestamp_type date_time_type;
  st_select_lex *select_lex;
  chooser_compare_func_creator boolfunc2creator;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 1101 "sql_yacc.cc"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 91 "sql_yacc.yy"

bool my_yyoverflow(short **a, YYSTYPE **b, ulong *yystacksize);


/* Line 214 of yacc.c.  */
#line 1116 "sql_yacc.cc"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  548
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   39120

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  495
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  492
/* YYNRULES -- Number of rules. */
#define YYNRULES  1672
/* YYNRULES -- Number of states. */
#define YYNSTATES  3104

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   731

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   489,     2,     2,     2,   482,   477,     2,
     486,   487,   480,   479,   488,   478,   493,   481,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   494,   492,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   483,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   490,   476,   491,   484,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   485
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    46,    48,    50,    52,    54,    56,    58,
      60,    62,    64,    66,    68,    70,    72,    74,    76,    78,
      80,    82,    84,    86,    88,    90,    92,    94,    96,    98,
     102,   104,   106,   111,   113,   116,   117,   122,   123,   126,
     130,   132,   135,   138,   139,   145,   147,   151,   155,   159,
     163,   167,   171,   175,   179,   183,   187,   191,   195,   197,
     201,   205,   209,   213,   214,   222,   223,   235,   236,   243,
     244,   254,   257,   260,   263,   268,   273,   274,   279,   280,
     281,   287,   288,   296,   297,   298,   305,   306,   308,   309,
     311,   313,   316,   318,   320,   321,   323,   325,   328,   330,
     331,   335,   336,   338,   340,   343,   345,   348,   352,   356,
     360,   364,   368,   372,   376,   380,   384,   388,   392,   396,
     400,   404,   408,   412,   416,   422,   424,   426,   430,   435,
     440,   445,   450,   452,   454,   456,   458,   460,   462,   464,
     466,   468,   470,   472,   474,   477,   478,   480,   482,   484,
     486,   488,   492,   494,   496,   499,   502,   509,   517,   526,
     529,   532,   533,   535,   538,   539,   541,   544,   545,   550,
     554,   558,   562,   565,   567,   569,   575,   578,   583,   585,
     590,   592,   598,   603,   608,   612,   614,   616,   619,   621,
     623,   626,   628,   630,   632,   635,   639,   642,   646,   649,
     652,   656,   660,   664,   665,   672,   673,   680,   683,   685,
     687,   689,   691,   693,   695,   697,   699,   701,   703,   705,
     708,   711,   713,   716,   718,   721,   725,   728,   730,   732,
     734,   736,   738,   740,   742,   745,   746,   750,   752,   758,
     759,   761,   764,   766,   768,   770,   772,   773,   777,   778,
     780,   781,   783,   786,   788,   790,   793,   796,   801,   803,
     807,   810,   812,   815,   818,   820,   823,   826,   828,   831,
     833,   835,   837,   839,   841,   843,   845,   847,   849,   851,
     852,   855,   857,   859,   860,   862,   863,   865,   867,   869,
     872,   873,   875,   876,   881,   883,   888,   892,   894,   895,
     897,   900,   902,   906,   910,   913,   916,   919,   921,   923,
     926,   929,   932,   934,   937,   940,   943,   946,   948,   950,
     951,   953,   955,   957,   959,   960,   962,   964,   966,   967,
     970,   973,   975,   977,   979,   984,   987,   989,   994,   995,
     997,   998,  1001,  1003,  1007,  1008,  1015,  1016,  1022,  1023,
    1025,  1026,  1029,  1032,  1034,  1038,  1041,  1045,  1048,  1053,
    1054,  1061,  1062,  1063,  1072,  1077,  1082,  1086,  1090,  1093,
    1096,  1103,  1109,  1113,  1119,  1121,  1123,  1124,  1126,  1127,
    1129,  1130,  1132,  1134,  1135,  1138,  1140,  1141,  1143,  1145,
    1147,  1148,  1154,  1158,  1159,  1165,  1169,  1173,  1174,  1178,
    1179,  1182,  1184,  1188,  1189,  1191,  1193,  1194,  1197,  1199,
    1203,  1204,  1211,  1212,  1219,  1220,  1226,  1227,  1229,  1231,
    1232,  1239,  1240,  1242,  1244,  1247,  1249,  1251,  1253,  1254,
    1261,  1262,  1268,  1269,  1271,  1273,  1276,  1278,  1280,  1282,
    1284,  1286,  1287,  1294,  1295,  1297,  1299,  1300,  1305,  1307,
    1311,  1315,  1321,  1323,  1327,  1330,  1332,  1334,  1335,  1342,
    1344,  1348,  1352,  1353,  1356,  1357,  1362,  1363,  1366,  1368,
    1371,  1376,  1379,  1383,  1384,  1388,  1389,  1390,  1397,  1400,
    1402,  1404,  1407,  1410,  1419,  1423,  1424,  1426,  1429,  1431,
    1433,  1435,  1437,  1439,  1441,  1443,  1445,  1447,  1449,  1451,
    1452,  1455,  1460,  1464,  1466,  1468,  1473,  1474,  1475,  1477,
    1479,  1480,  1483,  1486,  1488,  1490,  1491,  1494,  1496,  1498,
    1500,  1502,  1504,  1506,  1508,  1510,  1512,  1514,  1520,  1527,
    1531,  1536,  1542,  1549,  1553,  1557,  1561,  1565,  1570,  1575,
    1581,  1585,  1590,  1594,  1599,  1603,  1607,  1612,  1616,  1620,
    1624,  1628,  1632,  1636,  1640,  1644,  1648,  1652,  1656,  1660,
    1665,  1670,  1676,  1683,  1687,  1691,  1695,  1699,  1704,  1709,
    1715,  1719,  1724,  1728,  1733,  1737,  1741,  1746,  1750,  1754,
    1758,  1762,  1766,  1770,  1774,  1778,  1782,  1786,  1790,  1794,
    1799,  1804,  1806,  1812,  1819,  1823,  1828,  1834,  1841,  1845,
    1849,  1853,  1858,  1863,  1869,  1873,  1878,  1882,  1887,  1891,
    1895,  1900,  1904,  1908,  1912,  1916,  1920,  1924,  1928,  1932,
    1936,  1940,  1944,  1948,  1953,  1958,  1960,  1963,  1965,  1969,
    1971,  1973,  1978,  1981,  1987,  1989,  1992,  1995,  1998,  2001,
    2004,  2008,  2014,  2021,  2024,  2026,  2031,  2039,  2044,  2047,
    2054,  2061,  2068,  2075,  2080,  2085,  2089,  2094,  2101,  2110,
    2117,  2126,  2131,  2138,  2143,  2148,  2153,  2158,  2163,  2170,
    2179,  2182,  2185,  2190,  2193,  2201,  2209,  2213,  2218,  2223,
    2230,  2237,  2242,  2249,  2256,  2263,  2268,  2275,  2280,  2287,
    2296,  2307,  2320,  2327,  2332,  2339,  2346,  2348,  2355,  2360,
    2369,  2380,  2385,  2387,  2391,  2396,  2403,  2410,  2419,  2426,
    2433,  2438,  2445,  2452,  2461,  2466,  2471,  2478,  2483,  2486,
    2491,  2496,  2501,  2508,  2513,  2517,  2526,  2533,  2538,  2545,
    2552,  2561,  2566,  2575,  2582,  2591,  2598,  2607,  2612,  2617,
    2624,  2629,  2637,  2645,  2653,  2660,  2667,  2674,  2681,  2688,
    2693,  2698,  2703,  2708,  2713,  2718,  2729,  2733,  2738,  2742,
    2745,  2748,  2751,  2756,  2763,  2768,  2773,  2780,  2787,  2794,
    2799,  2806,  2811,  2818,  2823,  2828,  2833,  2838,  2845,  2850,
    2857,  2862,  2869,  2874,  2879,  2886,  2891,  2898,  2903,  2910,
    2915,  2920,  2927,  2932,  2939,  2940,  2944,  2948,  2949,  2951,
    2956,  2961,  2966,  2971,  2977,  2982,  2983,  2984,  2992,  3003,
    3008,  3013,  3018,  3023,  3028,  3029,  3038,  3039,  3041,  3042,
    3045,  3046,  3048,  3049,  3053,  3056,  3060,  3063,  3065,  3068,
    3070,  3073,  3075,  3077,  3079,  3080,  3083,  3085,  3089,  3091,
    3095,  3096,  3099,  3101,  3105,  3106,  3108,  3109,  3112,  3113,
    3116,  3120,  3126,  3128,  3132,  3136,  3140,  3146,  3147,  3156,
    3164,  3165,  3176,  3183,  3191,  3192,  3203,  3210,  3215,  3217,
    3220,  3223,  3224,  3229,  3240,  3246,  3250,  3253,  3257,  3258,
    3259,  3265,  3266,  3268,  3269,  3272,  3275,  3278,  3279,  3285,
    3286,  3288,  3292,  3294,  3296,  3298,  3302,  3304,  3306,  3308,
    3310,  3312,  3314,  3316,  3318,  3320,  3322,  3324,  3326,  3328,
    3330,  3332,  3334,  3336,  3338,  3340,  3342,  3344,  3346,  3347,
    3349,  3351,  3352,  3355,  3356,  3358,  3359,  3360,  3364,  3365,
    3366,  3370,  3373,  3374,  3375,  3380,  3385,  3388,  3389,  3392,
    3395,  3396,  3398,  3399,  3404,  3409,  3412,  3413,  3415,  3417,
    3418,  3420,  3421,  3423,  3426,  3428,  3432,  3436,  3437,  3440,
    3442,  3444,  3446,  3448,  3450,  3452,  3454,  3456,  3458,  3460,
    3461,  3462,  3469,  3470,  3472,  3476,  3478,  3481,  3482,  3485,
    3489,  3491,  3494,  3495,  3502,  3506,  3509,  3510,  3514,  3521,
    3522,  3529,  3534,  3538,  3539,  3544,  3546,  3550,  3552,  3553,
    3556,  3557,  3559,  3560,  3561,  3570,  3571,  3572,  3579,  3580,
    3582,  3584,  3586,  3588,  3590,  3593,  3595,  3597,  3599,  3603,
    3608,  3609,  3613,  3614,  3618,  3621,  3625,  3627,  3630,  3633,
    3634,  3638,  3639,  3645,  3649,  3651,  3655,  3657,  3661,  3663,
    3665,  3666,  3668,  3669,  3674,  3675,  3677,  3681,  3683,  3685,
    3687,  3688,  3689,  3696,  3697,  3698,  3710,  3714,  3716,  3720,
    3724,  3726,  3730,  3731,  3733,  3734,  3739,  3740,  3747,  3748,
    3754,  3755,  3762,  3764,  3768,  3772,  3778,  3779,  3782,  3783,
    3786,  3788,  3790,  3792,  3796,  3797,  3799,  3800,  3804,  3807,
    3811,  3816,  3821,  3822,  3827,  3834,  3851,  3854,  3857,  3858,
    3865,  3870,  3873,  3876,  3879,  3881,  3887,  3893,  3896,  3899,
    3902,  3905,  3908,  3912,  3915,  3918,  3921,  3923,  3925,  3929,
    3934,  3938,  3941,  3944,  3946,  3948,  3950,  3952,  3953,  3955,
    3956,  3959,  3960,  3963,  3964,  3966,  3968,  3970,  3971,  3974,
    3975,  3978,  3979,  3984,  3985,  3990,  3992,  3994,  3995,  3997,
    3998,  4000,  4002,  4003,  4008,  4012,  4014,  4015,  4019,  4024,
    4027,  4029,  4031,  4033,  4035,  4037,  4039,  4041,  4043,  4044,
    4046,  4047,  4051,  4055,  4057,  4059,  4061,  4064,  4065,  4069,
    4073,  4076,  4079,  4082,  4085,  4086,  4102,  4108,  4113,  4114,
    4116,  4117,  4119,  4121,  4122,  4124,  4126,  4127,  4130,  4133,
    4135,  4139,  4144,  4148,  4152,  4153,  4156,  4159,  4161,  4165,
    4169,  4170,  4174,  4176,  4178,  4181,  4184,  4186,  4188,  4190,
    4192,  4195,  4198,  4200,  4202,  4204,  4206,  4208,  4210,  4213,
    4216,  4219,  4222,  4224,  4226,  4228,  4230,  4232,  4234,  4236,
    4240,  4246,  4248,  4250,  4254,  4259,  4265,  4267,  4273,  4277,
    4280,  4282,  4286,  4289,  4291,  4293,  4295,  4297,  4299,  4301,
    4303,  4305,  4307,  4309,  4311,  4315,  4318,  4320,  4322,  4324,
    4326,  4328,  4330,  4332,  4334,  4336,  4338,  4340,  4342,  4344,
    4346,  4348,  4350,  4352,  4354,  4356,  4358,  4360,  4362,  4364,
    4366,  4368,  4370,  4372,  4374,  4376,  4378,  4380,  4382,  4384,
    4386,  4388,  4390,  4392,  4394,  4396,  4398,  4400,  4402,  4404,
    4406,  4408,  4410,  4412,  4414,  4416,  4418,  4420,  4422,  4424,
    4426,  4428,  4430,  4432,  4434,  4436,  4438,  4440,  4442,  4444,
    4446,  4448,  4450,  4452,  4454,  4456,  4458,  4460,  4462,  4464,
    4466,  4468,  4470,  4472,  4474,  4476,  4478,  4480,  4482,  4484,
    4486,  4488,  4490,  4492,  4494,  4496,  4498,  4500,  4502,  4504,
    4506,  4508,  4510,  4512,  4514,  4516,  4518,  4520,  4522,  4524,
    4526,  4528,  4530,  4532,  4534,  4536,  4538,  4540,  4542,  4544,
    4546,  4548,  4550,  4552,  4554,  4556,  4558,  4560,  4562,  4564,
    4566,  4568,  4570,  4572,  4574,  4576,  4578,  4580,  4582,  4584,
    4586,  4588,  4590,  4592,  4594,  4596,  4598,  4600,  4602,  4604,
    4606,  4608,  4610,  4612,  4614,  4616,  4618,  4620,  4622,  4624,
    4626,  4628,  4630,  4632,  4634,  4636,  4638,  4640,  4642,  4644,
    4646,  4648,  4650,  4652,  4654,  4656,  4658,  4660,  4662,  4664,
    4666,  4668,  4670,  4672,  4674,  4676,  4678,  4680,  4682,  4684,
    4686,  4688,  4690,  4692,  4694,  4696,  4698,  4700,  4702,  4704,
    4706,  4708,  4710,  4712,  4714,  4716,  4718,  4720,  4722,  4724,
    4726,  4728,  4730,  4732,  4734,  4736,  4737,  4742,  4743,  4745,
    4747,  4751,  4754,  4757,  4759,  4761,  4763,  4765,  4766,  4768,
    4769,  4771,  4773,  4775,  4776,  4779,  4782,  4785,  4789,  4794,
    4799,  4806,  4809,  4813,  4817,  4823,  4825,  4829,  4833,  4836,
    4839,  4842,  4844,  4846,  4851,  4856,  4858,  4860,  4862,  4864,
    4866,  4867,  4872,  4874,  4876,  4878,  4882,  4886,  4888,  4890,
    4893,  4896,  4899,  4904,  4908,  4909,  4917,  4919,  4922,  4924,
    4926,  4928,  4930,  4932,  4934,  4935,  4941,  4943,  4945,  4947,
    4949,  4951,  4952,  4956,  4962,  4970,  4971,  4981,  4983,  4986,
    4987,  4989,  4991,  4995,  4996,  5000,  5001,  5005,  5006,  5010,
    5011,  5015,  5017,  5019,  5021,  5023,  5025,  5027,  5029,  5031,
    5033,  5035,  5037,  5040,  5043,  5045,  5049,  5052,  5055,  5058,
    5059,  5061,  5065,  5067,  5070,  5073,  5076,  5078,  5082,  5086,
    5088,  5090,  5094,  5099,  5105,  5107,  5108,  5112,  5116,  5118,
    5120,  5121,  5124,  5127,  5130,  5133,  5134,  5137,  5140,  5142,
    5145,  5148,  5151,  5154,  5155,  5159,  5160,  5162,  5164,  5166,
    5171,  5174,  5175,  5177,  5178,  5183,  5185,  5187,  5188,  5189,
    5192,  5195,  5197,  5198,  5200,  5202,  5206,  5208,  5212,  5214,
    5218,  5220,  5223
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     496,     0,    -1,     3,    -1,   497,     3,    -1,   611,    -1,
     649,    -1,   639,    -1,   965,    -1,   508,    -1,   651,    -1,
     641,    -1,   968,    -1,   513,    -1,   498,    -1,   829,    -1,
     856,    -1,   783,    -1,   785,    -1,   502,    -1,   862,    -1,
     942,    -1,   932,    -1,   507,    -1,   792,    -1,   876,    -1,
     878,    -1,   925,    -1,   656,    -1,   663,    -1,   667,    -1,
     500,    -1,   872,    -1,   659,    -1,   644,    -1,   795,    -1,
     868,    -1,   637,    -1,   939,    -1,   969,    -1,   970,    -1,
     675,    -1,   910,    -1,   842,    -1,   626,    -1,   629,    -1,
     840,    -1,   931,    -1,   821,    -1,   877,    -1,   499,   337,
     906,    -1,   338,    -1,    44,    -1,   337,   906,   144,   501,
      -1,   904,    -1,   492,   907,    -1,    -1,    46,   906,   503,
     504,    -1,    -1,   301,   505,    -1,   505,   488,   506,    -1,
     506,    -1,   492,   907,    -1,    49,   907,    -1,    -1,    32,
      56,   280,   509,   510,    -1,   511,    -1,   510,   488,   511,
      -1,   185,    10,   904,    -1,   186,    10,   904,    -1,   189,
      10,   904,    -1,   190,    10,   770,    -1,   191,    10,   770,
      -1,   193,    10,   770,    -1,   194,    10,   904,    -1,   195,
      10,   904,    -1,   196,    10,   904,    -1,   197,    10,   904,
      -1,   198,    10,   904,    -1,   512,    -1,   187,    10,   904,
      -1,   188,    10,   771,    -1,   199,    10,   904,    -1,   200,
      10,   770,    -1,    -1,    38,   531,   275,   534,   901,   514,
     518,    -1,    -1,    38,   603,   161,   906,   604,   221,   901,
     515,   486,   606,   487,    -1,    -1,    38,   368,   534,   906,
     516,   528,    -1,    -1,    38,   546,   292,   903,   517,   290,
     547,   291,   904,    -1,   486,   519,    -1,   535,   521,    -1,
     177,   901,    -1,   486,   177,   901,   487,    -1,   548,   487,
     535,   521,    -1,    -1,   524,   487,   520,   974,    -1,    -1,
      -1,   882,   527,   524,   522,   971,    -1,    -1,   882,   527,
     486,   524,   487,   523,   974,    -1,    -1,    -1,    68,   525,
     685,   689,   526,   545,    -1,    -1,    88,    -1,    -1,   529,
      -1,   530,    -1,   529,   530,    -1,   540,    -1,   539,    -1,
      -1,   532,    -1,   533,    -1,   533,   532,    -1,   277,    -1,
      -1,   403,   216,   135,    -1,    -1,   537,    -1,   538,    -1,
     538,   536,    -1,   538,    -1,   538,   537,    -1,   538,   488,
     537,    -1,   382,   813,   541,    -1,   284,   813,   541,    -1,
     202,   813,   771,    -1,   207,   813,   771,    -1,    91,   813,
     770,    -1,   428,   813,   904,    -1,    34,   813,   904,    -1,
      90,   813,   771,    -1,   232,   813,   770,    -1,   232,   813,
     119,    -1,   106,   813,   770,    -1,   121,   813,   770,    -1,
     257,   813,   542,    -1,   241,   813,   543,    -1,   242,   813,
     770,    -1,   243,   813,   770,    -1,   296,   813,   486,   788,
     487,    -1,   539,    -1,   540,    -1,   404,   813,   544,    -1,
     118,   133,   813,   904,    -1,   161,   133,   813,   904,    -1,
     587,   579,   813,   581,    -1,   587,   109,   813,   586,    -1,
     907,    -1,   119,    -1,   140,    -1,   129,    -1,   311,    -1,
     240,    -1,   239,    -1,   770,    -1,   217,    -1,   139,    -1,
       6,    -1,   766,    -1,   684,   688,    -1,    -1,    85,    -1,
     343,    -1,   340,    -1,   328,    -1,   549,    -1,   548,   488,
     549,    -1,   550,    -1,   551,    -1,   556,   552,    -1,   556,
     590,    -1,   598,   608,   604,   486,   606,   487,    -1,   554,
     599,   608,   604,   486,   606,   487,    -1,   554,   143,   171,
     608,   486,   606,   487,   590,    -1,   555,   552,    -1,   554,
     553,    -1,    -1,   553,    -1,   107,   696,    -1,    -1,   555,
      -1,   114,   608,    -1,    -1,   900,   557,   558,   575,    -1,
     566,   573,   570,    -1,   567,   574,   570,    -1,   326,   568,
     570,    -1,    95,   573,    -1,    96,    -1,    97,    -1,   562,
     486,   219,   487,   588,    -1,   562,   588,    -1,   563,   486,
     219,   487,    -1,   563,    -1,    94,   486,   219,   487,    -1,
      94,    -1,   564,   486,   219,   487,   588,    -1,   565,   486,
     219,   487,    -1,   352,   486,   219,   487,    -1,   462,   573,
     570,    -1,   321,    -1,   346,    -1,   345,   573,    -1,   320,
      -1,   347,    -1,   316,   573,    -1,   561,    -1,   332,    -1,
     330,    -1,   183,   352,    -1,   183,   564,   588,    -1,   349,
     588,    -1,   344,   573,   588,    -1,   334,   588,    -1,   331,
     588,    -1,   322,   568,   570,    -1,   335,   568,   570,    -1,
     140,   568,   570,    -1,    -1,   324,   559,   486,   610,   487,
     588,    -1,    -1,   260,   560,   486,   610,   487,   588,    -1,
     183,   588,    -1,   262,    -1,   327,    -1,   394,    -1,   430,
      -1,   424,    -1,   409,    -1,   423,    -1,   432,    -1,   425,
      -1,   317,    -1,   213,    -1,   209,   317,    -1,   562,   354,
      -1,   353,    -1,   209,   353,    -1,   215,    -1,   213,   353,
      -1,   209,   317,   354,    -1,   213,   354,    -1,   328,    -1,
     348,    -1,   342,    -1,   333,    -1,   315,    -1,   340,    -1,
     323,    -1,   323,   336,    -1,    -1,   486,   219,   487,    -1,
     569,    -1,   486,   219,   488,   219,   487,    -1,    -1,   571,
      -1,   571,   572,    -1,   572,    -1,   341,    -1,   351,    -1,
     355,    -1,    -1,   486,   219,   487,    -1,    -1,   569,    -1,
      -1,   576,    -1,   576,   577,    -1,   577,    -1,   218,    -1,
     216,   218,    -1,   119,   578,    -1,   221,    83,   426,   695,
      -1,    90,    -1,   262,   119,   302,    -1,   589,   171,    -1,
     297,    -1,   297,   171,    -1,    34,   904,    -1,    94,    -1,
     109,   584,    -1,   426,   695,    -1,   893,    -1,   317,   260,
      -1,   105,    -1,   907,    -1,    94,    -1,   580,    -1,   119,
      -1,   907,    -1,    94,    -1,   582,    -1,   119,    -1,   907,
      -1,    -1,   109,   586,    -1,   584,    -1,   119,    -1,    -1,
     119,    -1,    -1,   314,    -1,   101,    -1,   295,    -1,   579,
     580,    -1,    -1,   234,    -1,    -1,   246,   901,   591,   592,
      -1,   594,    -1,   486,   593,   487,   594,    -1,   593,   488,
     906,    -1,   906,    -1,    -1,   595,    -1,   595,   596,    -1,
     596,    -1,   221,    41,   597,    -1,   221,    83,   597,    -1,
     201,   145,    -1,   201,   233,    -1,   201,   265,    -1,   254,
      -1,   103,    -1,   260,   218,    -1,   217,    84,    -1,   260,
     119,    -1,   600,    -1,   146,   601,    -1,   267,   601,    -1,
     234,   171,    -1,   297,   601,    -1,   171,    -1,   161,    -1,
      -1,   600,    -1,   170,    -1,   161,    -1,   162,    -1,    -1,
     297,    -1,   146,    -1,   267,    -1,    -1,   301,   605,    -1,
     284,   605,    -1,    99,    -1,   259,    -1,   153,    -1,   606,
     488,   607,   764,    -1,   607,   764,    -1,   906,    -1,   906,
     486,   219,   487,    -1,    -1,   900,    -1,    -1,   493,   906,
      -1,   891,    -1,   610,   488,   891,    -1,    -1,    26,   622,
     275,   901,   612,   615,    -1,    -1,    26,   368,   614,   613,
     528,    -1,    -1,   906,    -1,    -1,   126,   276,    -1,   160,
     276,    -1,   617,    -1,   615,   488,   617,    -1,    24,   621,
      -1,   616,   550,   624,    -1,    24,   551,    -1,   616,   486,
     548,   487,    -1,    -1,    32,   621,   900,   618,   556,   624,
      -1,    -1,    -1,   418,   621,   900,   619,   558,   575,   620,
     624,    -1,    44,   621,   900,   623,    -1,    44,   143,   171,
     608,    -1,    44,   234,   171,    -1,    44,   600,   900,    -1,
     125,   170,    -1,   130,   170,    -1,    26,   621,   900,   260,
     119,   893,    -1,    26,   621,   900,    44,   119,    -1,   249,
     625,   901,    -1,   115,   280,   579,   581,   585,    -1,   536,
      -1,   761,    -1,    -1,   112,    -1,    -1,   159,    -1,    -1,
     254,    -1,   103,    -1,    -1,    25,   906,    -1,   139,    -1,
      -1,   280,    -1,    10,    -1,    88,    -1,    -1,    73,    70,
     631,   627,   635,    -1,    76,    70,   631,    -1,    -1,    70,
      73,   631,   628,   635,    -1,    70,    76,   631,    -1,    73,
     282,   630,    -1,    -1,   306,    36,    71,    -1,    -1,   632,
     633,    -1,   634,    -1,   633,   488,   634,    -1,    -1,    72,
      -1,    51,    -1,    -1,    82,   636,    -1,   512,    -1,   636,
     488,   512,    -1,    -1,   253,   927,   638,   788,   144,   904,
      -1,    -1,    92,   927,   640,   788,   280,   904,    -1,    -1,
     106,   927,   642,   788,   643,    -1,    -1,   339,    -1,   136,
      -1,    -1,    62,   658,   927,   645,   788,   646,    -1,    -1,
     647,    -1,   648,    -1,   648,   647,    -1,   339,    -1,   136,
      -1,   299,    -1,    -1,    27,   658,   927,   650,   788,   653,
      -1,    -1,   107,   927,   652,   788,   653,    -1,    -1,   654,
      -1,   655,    -1,   655,   654,    -1,   339,    -1,   325,    -1,
     206,    -1,   136,    -1,   318,    -1,    -1,    60,   658,   927,
     657,   788,   653,    -1,    -1,   308,    -1,   179,    -1,    -1,
     249,   927,   660,   661,    -1,   662,    -1,   661,   488,   662,
      -1,   901,   280,   901,    -1,   102,   161,   664,   167,   666,
      -1,   665,    -1,   664,   488,   665,    -1,   901,   671,    -1,
     906,    -1,   119,    -1,    -1,    53,   161,   166,   102,   668,
     669,    -1,   670,    -1,   669,   488,   670,    -1,   901,   671,
     674,    -1,    -1,   672,   673,    -1,    -1,   601,   486,   745,
     487,    -1,    -1,   159,   174,    -1,   676,    -1,    68,   678,
      -1,   486,   677,   487,   974,    -1,    68,   680,    -1,   486,
     677,   487,    -1,    -1,   680,   679,   971,    -1,    -1,    -1,
     681,   685,   689,   682,   683,   688,    -1,   760,   766,    -1,
     781,    -1,   684,    -1,   781,   684,    -1,   684,   781,    -1,
     144,   729,   752,   757,   754,   760,   766,   772,    -1,   144,
      42,   766,    -1,    -1,   686,    -1,   686,   687,    -1,   687,
      -1,   272,    -1,   155,    -1,   127,    -1,   471,    -1,   467,
      -1,   472,    -1,   469,    -1,   470,    -1,   468,    -1,    86,
      -1,    -1,   389,    83,    -1,    55,   167,   441,   417,    -1,
     689,   488,   690,    -1,   690,    -1,   480,    -1,   691,   693,
     692,   694,    -1,    -1,    -1,   897,    -1,   696,    -1,    -1,
      88,   906,    -1,    88,   904,    -1,   906,    -1,   904,    -1,
      -1,   486,   487,    -1,   699,    -1,   703,    -1,    10,    -1,
      13,    -1,    14,    -1,    15,    -1,    16,    -1,    17,    -1,
      86,    -1,    28,    -1,   696,   167,   486,   717,   487,    -1,
     696,   216,   167,   486,   717,   487,    -1,   696,   167,   983,
      -1,   696,   216,   167,   983,    -1,   696,   358,   701,    87,
     696,    -1,   696,   216,   358,   701,    87,   696,    -1,   696,
     227,   696,    -1,   696,   226,   696,    -1,   696,   310,   696,
      -1,   696,    87,   696,    -1,   696,    12,   177,   696,    -1,
     696,   177,   703,   756,    -1,   696,   216,   177,   703,   756,
      -1,   696,   247,   696,    -1,   696,   216,   247,   696,    -1,
     696,    18,   218,    -1,   696,    18,   216,   218,    -1,   696,
      11,   696,    -1,   696,   697,   696,    -1,   696,   697,   698,
     983,    -1,   696,    20,   696,    -1,   696,    21,   696,    -1,
     696,   479,   696,    -1,   696,   478,   696,    -1,   696,   480,
     696,    -1,   696,   481,   696,    -1,   696,     9,   696,    -1,
     696,    19,   696,    -1,   696,   476,   696,    -1,   696,   483,
     696,    -1,   696,   477,   696,    -1,   696,   482,   696,    -1,
     696,   479,   702,   747,    -1,   696,   478,   702,   747,    -1,
     700,   358,   701,    87,   696,    -1,   700,   216,   358,   701,
      87,   696,    -1,   700,   227,   696,    -1,   700,   226,   696,
      -1,   700,   310,   696,    -1,   700,    87,   696,    -1,   700,
      12,   177,   696,    -1,   700,   177,   703,   756,    -1,   700,
     216,   177,   703,   756,    -1,   700,   247,   696,    -1,   700,
     216,   247,   696,    -1,   700,    18,   218,    -1,   700,    18,
     216,   218,    -1,   700,    11,   696,    -1,   700,   697,   696,
      -1,   700,   697,   698,   983,    -1,   700,    20,   696,    -1,
     700,    21,   696,    -1,   700,   479,   696,    -1,   700,   478,
     696,    -1,   700,   480,   696,    -1,   700,   481,   696,    -1,
     700,     9,   696,    -1,   700,   476,   696,    -1,   700,   483,
     696,    -1,   700,   477,   696,    -1,   700,   482,   696,    -1,
     700,    19,   696,    -1,   700,   479,   702,   747,    -1,   700,
     478,   702,   747,    -1,   703,    -1,   701,   167,   486,   717,
     487,    -1,   701,   216,   167,   486,   717,   487,    -1,   701,
     167,   983,    -1,   701,   216,   167,   983,    -1,   701,   358,
     701,    87,   696,    -1,   701,   216,   358,   701,    87,   696,
      -1,   701,   227,   696,    -1,   701,   226,   696,    -1,   701,
     310,   696,    -1,   701,    12,   177,   696,    -1,   701,   177,
     703,   756,    -1,   701,   216,   177,   703,   756,    -1,   701,
     247,   696,    -1,   701,   216,   247,   696,    -1,   701,    18,
     218,    -1,   701,    18,   216,   218,    -1,   701,    11,   696,
      -1,   701,   697,   696,    -1,   701,   697,   698,   983,    -1,
     701,    20,   696,    -1,   701,    21,   696,    -1,   701,   479,
     696,    -1,   701,   478,   696,    -1,   701,   480,   696,    -1,
     701,   481,   696,    -1,   701,     9,   696,    -1,   701,   476,
     696,    -1,   701,   483,   696,    -1,   701,   477,   696,    -1,
     701,   482,   696,    -1,   701,    19,   696,    -1,   701,   479,
     702,   747,    -1,   701,   478,   702,   747,    -1,   703,    -1,
     405,   696,    -1,   899,    -1,   703,   109,   907,    -1,   894,
      -1,   892,    -1,   492,   907,    22,   696,    -1,   492,   907,
      -1,   492,   492,   918,   907,   609,    -1,   707,    -1,   479,
     696,    -1,   478,   696,    -1,   484,   696,    -1,   216,   696,
      -1,   489,   696,    -1,   486,   696,   487,    -1,   486,   696,
     488,   717,   487,    -1,   258,   486,   696,   488,   717,   487,
      -1,   135,   981,    -1,   979,    -1,   490,   906,   696,   491,
      -1,   201,   720,   356,   486,   696,   705,   487,    -1,   314,
     486,   696,   487,    -1,    94,   696,    -1,   104,   486,   696,
      88,   716,   487,    -1,   362,   724,   459,   726,   725,   465,
      -1,   115,   486,   696,   488,   716,   487,    -1,   115,   486,
     696,   301,   580,   487,    -1,   119,   486,   899,   487,    -1,
     303,   486,   899,   487,    -1,   286,   486,   487,    -1,   287,
     486,   696,   487,    -1,   288,   486,   696,   488,   696,   487,
      -1,   289,   486,   696,   488,   696,   488,   696,   487,    -1,
      78,   486,   696,   488,   696,   487,    -1,    78,   486,   696,
     488,   405,   696,   747,   487,    -1,   357,   486,   696,   487,
      -1,   357,   486,   696,   488,   696,   487,    -1,   317,   486,
     717,   487,    -1,   105,   486,   696,   487,    -1,   319,   486,
     717,   487,    -1,   110,   486,   696,   487,    -1,   363,   486,
     717,   487,    -1,   364,   486,   696,   488,   717,   487,    -1,
     365,   486,   696,   488,   696,   488,   696,   487,    -1,   366,
     695,    -1,   367,   695,    -1,   367,   486,   696,   487,    -1,
     116,   695,    -1,   369,   486,   696,   488,   702,   747,   487,
      -1,   370,   486,   696,   488,   702,   747,   487,    -1,   368,
     486,   487,    -1,   321,   486,   696,   487,    -1,   375,   486,
     696,   487,    -1,   380,   486,   696,   488,   717,   487,    -1,
     411,   486,   696,   488,   717,   487,    -1,   384,   486,   696,
     487,    -1,   384,   486,   696,   488,   696,   487,    -1,   376,
     486,   696,   488,   905,   487,    -1,   381,   486,   696,   488,
     905,   487,    -1,   378,   486,   696,   487,    -1,   378,   486,
     696,   488,   696,   487,    -1,   377,   486,   696,   487,    -1,
     377,   486,   696,   488,   696,   487,    -1,   385,   486,   696,
     488,   696,   488,   696,   487,    -1,   385,   486,   696,   488,
     696,   488,   696,   488,   696,   487,    -1,   385,   486,   696,
     488,   696,   488,   696,   488,   696,   488,   696,   487,    -1,
     388,   486,   696,   488,   219,   487,    -1,   390,   486,   696,
     487,    -1,   390,   486,   696,   488,   696,   487,    -1,   387,
     486,   696,   488,   717,   487,    -1,   704,    -1,   397,   486,
     748,   488,   696,   487,    -1,   401,   486,   696,   487,    -1,
     403,   486,   696,   488,   696,   488,   696,   487,    -1,    50,
     486,   696,   488,   696,   488,   696,   488,   696,   487,    -1,
     702,   747,   479,   696,    -1,   702,    -1,   406,   486,   487,
      -1,   406,   486,   696,   487,    -1,   407,   486,   696,   488,
     696,   487,    -1,   410,   486,   696,   488,   696,   487,    -1,
     410,   486,   696,   488,   696,   488,   696,   487,    -1,   150,
     486,   696,   488,   717,   487,    -1,   173,   486,   696,   488,
     717,   487,    -1,   180,   486,   696,   487,    -1,   180,   486,
     696,   488,   696,   487,    -1,   412,   486,   696,   488,   696,
     487,    -1,   412,   486,   696,   488,   696,   488,   696,   487,
      -1,   413,   486,   696,   487,    -1,   416,   486,   696,   487,
      -1,    19,   486,   696,   488,   696,   487,    -1,   419,   486,
     696,   487,    -1,   426,   695,    -1,   426,   486,   696,   487,
      -1,   428,   486,   696,   487,    -1,   427,   486,   696,   487,
      -1,   433,   486,   700,   167,   696,   487,    -1,   435,   486,
     696,   487,    -1,   435,   486,   487,    -1,   436,   486,   696,
     488,   696,   488,   696,   487,    -1,   437,   486,   696,   488,
     696,   487,    -1,   438,   486,   696,   487,    -1,   438,   486,
     696,   488,   696,   487,    -1,   442,   486,   696,   488,   696,
     487,    -1,   442,   486,   696,   488,   405,   696,   747,   487,
      -1,   439,   486,   696,   487,    -1,   443,   486,   696,   488,
     696,   488,   696,   487,    -1,   443,   486,   696,   488,   696,
     487,    -1,   443,   486,   696,   144,   696,   389,   696,   487,
      -1,   443,   486,   696,   144,   696,   487,    -1,   444,   486,
     696,   488,   696,   488,   696,   487,    -1,   346,   486,   696,
     487,    -1,   345,   486,   696,   487,    -1,   345,   486,   696,
     488,   696,   487,    -1,   445,   486,   696,   487,    -1,   445,
     486,   172,   696,   144,   696,   487,    -1,   445,   486,   281,
     696,   144,   696,   487,    -1,   445,   486,    98,   696,   144,
     696,   487,    -1,   445,   486,   172,   144,   696,   487,    -1,
     445,   486,   281,   144,   696,   487,    -1,   445,   486,    98,
     144,   696,   487,    -1,   445,   486,   696,   144,   696,   487,
      -1,    80,   486,   696,   488,   696,   487,    -1,   446,   486,
     706,   487,    -1,   447,   486,   706,   487,    -1,   448,   486,
     706,   487,    -1,   449,   486,   706,   487,    -1,   450,   486,
     706,   487,    -1,   451,   486,   706,   487,    -1,   452,   486,
     890,   488,   219,   488,   219,   488,   717,   487,    -1,   453,
     486,   487,    -1,   453,   486,   696,   487,    -1,   454,   486,
     487,    -1,   455,   695,    -1,   456,   695,    -1,   457,   695,
      -1,   458,   486,   696,   487,    -1,   458,   486,   696,   488,
     696,   487,    -1,   462,   486,   696,   487,    -1,   463,   486,
     696,   487,    -1,   463,   486,   696,   488,   696,   487,    -1,
     464,   486,   770,   488,   696,   487,    -1,   386,   486,   747,
     144,   696,   487,    -1,   392,   486,   696,   487,    -1,   392,
     486,   696,   488,   696,   487,    -1,   393,   486,   696,   487,
      -1,   393,   486,   696,   488,   696,   487,    -1,   394,   486,
     717,   487,    -1,   409,   486,   717,   487,    -1,   423,   486,
     717,   487,    -1,   420,   486,   696,   487,    -1,   420,   486,
     696,   488,   696,   487,    -1,   421,   486,   696,   487,    -1,
     421,   486,   696,   488,   696,   487,    -1,   422,   486,   696,
     487,    -1,   422,   486,   696,   488,   696,   487,    -1,   424,
     486,   717,   487,    -1,   425,   486,   717,   487,    -1,   430,
     486,   696,   488,   696,   487,    -1,   429,   486,   696,   487,
      -1,   429,   486,   696,   488,   696,   487,    -1,   431,   486,
     696,   487,    -1,   431,   486,   696,   488,   696,   487,    -1,
     432,   486,   717,   487,    -1,   391,   486,   696,   487,    -1,
     391,   486,   696,   488,   696,   487,    -1,   408,   486,   696,
     487,    -1,   408,   486,   696,   488,   696,   487,    -1,    -1,
     306,   238,    47,    -1,   167,    97,   417,    -1,    -1,   717,
      -1,    29,   486,   714,   487,    -1,   359,   486,   714,   487,
      -1,   360,   486,   714,   487,    -1,   361,   486,   714,   487,
      -1,    37,   486,   751,   480,   487,    -1,    37,   486,   714,
     487,    -1,    -1,    -1,    37,   486,   127,   708,   717,   709,
     487,    -1,   396,   486,   890,   488,   219,   488,   219,   488,
     714,   487,    -1,    58,   486,   714,   487,    -1,    57,   486,
     714,   487,    -1,    74,   486,   714,   487,    -1,    75,   486,
     714,   487,    -1,    77,   486,   714,   487,    -1,    -1,   395,
     486,   711,   710,   717,   713,   712,   487,    -1,    -1,   127,
      -1,    -1,   261,   891,    -1,    -1,   761,    -1,    -1,   751,
     715,   696,    -1,    94,   573,    -1,   317,   573,   588,    -1,
     213,   573,    -1,   341,    -1,   341,   328,    -1,   351,    -1,
     351,   328,    -1,   321,    -1,   346,    -1,   320,    -1,    -1,
     718,   719,    -1,   696,    -1,   719,   488,   696,    -1,   721,
      -1,   486,   721,   487,    -1,    -1,   722,   723,    -1,   899,
      -1,   723,   488,   899,    -1,    -1,   696,    -1,    -1,   379,
     696,    -1,    -1,   727,   728,    -1,   696,   466,   696,    -1,
     728,   459,   696,   466,   696,    -1,   734,    -1,   729,   488,
     729,    -1,   729,   733,   729,    -1,   729,   272,   729,    -1,
     729,   733,   729,   221,   696,    -1,    -1,   729,   733,   729,
     301,   730,   486,   746,   487,    -1,   729,   407,   740,   169,
     729,   221,   696,    -1,    -1,   729,   407,   740,   169,   729,
     731,   301,   486,   746,   487,    -1,   729,   210,   407,   740,
     169,   729,    -1,   729,   437,   740,   169,   729,   221,   696,
      -1,    -1,   729,   437,   740,   169,   729,   732,   301,   486,
     746,   487,    -1,   729,   210,   437,   740,   169,   729,    -1,
     729,   210,   169,   729,    -1,   169,    -1,   164,   169,    -1,
      39,   169,    -1,    -1,   735,   901,   750,   741,    -1,   490,
     906,   734,   407,   229,   169,   734,   221,   696,   491,    -1,
     486,   736,   974,   487,   750,    -1,   486,   729,   487,    -1,
      68,   737,    -1,   486,   736,   487,    -1,    -1,    -1,   738,
     685,   689,   739,   545,    -1,    -1,   229,    -1,    -1,   300,
     742,    -1,   142,   742,    -1,   159,   742,    -1,    -1,   600,
     743,   486,   744,   487,    -1,    -1,   745,    -1,   745,   488,
     906,    -1,   906,    -1,   234,    -1,   906,    -1,   746,   488,
     906,    -1,   371,    -1,   372,    -1,   373,    -1,   374,    -1,
     375,    -1,   398,    -1,   399,    -1,   400,    -1,   401,    -1,
     413,    -1,   414,    -1,   415,    -1,   416,    -1,   419,    -1,
     440,    -1,   439,    -1,   461,    -1,   462,    -1,   321,    -1,
     346,    -1,   320,    -1,   345,    -1,    -1,    88,    -1,    10,
      -1,    -1,   749,   906,    -1,    -1,    86,    -1,    -1,    -1,
     305,   753,   696,    -1,    -1,    -1,   152,   755,   696,    -1,
     134,   703,    -1,    -1,    -1,   151,   100,   758,   759,    -1,
     758,   488,   898,   764,    -1,   898,   764,    -1,    -1,   306,
      40,    -1,   306,    66,    -1,    -1,   761,    -1,    -1,   228,
     100,   762,   763,    -1,   763,   488,   898,   764,    -1,   898,
     764,    -1,    -1,    89,    -1,   122,    -1,    -1,   767,    -1,
      -1,   767,    -1,   329,   768,    -1,   770,    -1,   770,   488,
     770,    -1,   770,   220,   770,    -1,    -1,   329,   771,    -1,
     219,    -1,   182,    -1,   350,    -1,   245,    -1,   141,    -1,
     219,    -1,   350,    -1,   182,    -1,   245,    -1,   141,    -1,
      -1,    -1,   434,   906,   773,   486,   774,   487,    -1,    -1,
     775,    -1,   775,   488,   776,    -1,   776,    -1,   691,   696,
      -1,    -1,   778,   779,    -1,   779,   488,   780,    -1,   780,
      -1,   492,   907,    -1,    -1,   166,   230,   904,   782,   883,
     886,    -1,   166,   231,   904,    -1,   166,   777,    -1,    -1,
      43,   784,   717,    -1,    44,   791,   927,   790,   788,   623,
      -1,    -1,    44,   161,   906,   221,   901,   786,    -1,    44,
     368,   790,   906,    -1,    44,   292,   903,    -1,    -1,    44,
     454,   787,   956,    -1,   789,    -1,   788,   488,   789,    -1,
     901,    -1,    -1,   403,   135,    -1,    -1,   277,    -1,    -1,
      -1,    50,   793,   798,   622,   800,   794,   802,   819,    -1,
      -1,    -1,   436,   796,   799,   800,   797,   802,    -1,    -1,
     184,    -1,   120,    -1,   155,    -1,   828,    -1,   120,    -1,
     166,   801,    -1,   801,    -1,   789,    -1,   806,    -1,   486,
     487,   806,    -1,   486,   805,   487,   806,    -1,    -1,   260,
     803,   810,    -1,    -1,   486,   805,   487,    -1,   486,   487,
      -1,   805,   488,   896,    -1,   896,    -1,   303,   809,    -1,
     302,   809,    -1,    -1,   524,   807,   971,    -1,    -1,   486,
     524,   487,   808,   974,    -1,   809,   488,   814,    -1,   814,
      -1,   810,   488,   811,    -1,   811,    -1,   899,   812,   818,
      -1,    10,    -1,    22,    -1,    -1,   812,    -1,    -1,   486,
     815,   816,   487,    -1,    -1,   817,    -1,   817,   488,   818,
      -1,   818,    -1,   696,    -1,   119,    -1,    -1,    -1,   221,
     128,   820,   171,    83,   826,    -1,    -1,    -1,    83,   822,
     828,   622,   729,   260,   824,   823,   752,   760,   769,    -1,
     824,   488,   825,    -1,   825,    -1,   899,   812,   818,    -1,
     826,   488,   827,    -1,   827,    -1,   899,   812,   818,    -1,
      -1,   184,    -1,    -1,    41,   830,   838,   831,    -1,    -1,
     144,   901,   832,   752,   760,   769,    -1,    -1,   835,   833,
     144,   729,   752,    -1,    -1,   144,   835,   834,   301,   729,
     752,    -1,   836,    -1,   835,   488,   836,    -1,   906,   837,
     750,    -1,   906,   493,   906,   837,   750,    -1,    -1,   493,
     480,    -1,    -1,   839,   838,    -1,   339,    -1,   184,    -1,
     159,    -1,    80,   841,   789,    -1,    -1,   275,    -1,    -1,
      69,   843,   844,    -1,   117,   851,    -1,   274,   850,   851,
      -1,   275,   270,   850,   851,    -1,   223,   274,   850,   851,
      -1,    -1,   382,   541,   845,   847,    -1,   852,   111,   853,
     901,   850,   851,    -1,   212,    56,   389,    70,   306,   187,
      10,   904,    87,   188,    10,   771,    87,   192,    10,   770,
      -1,   848,   181,    -1,    70,   156,    -1,    -1,    31,    45,
     854,   855,   846,   765,    -1,   602,   853,   901,   850,    -1,
     112,   285,    -1,   275,   285,    -1,   849,   383,    -1,   235,
      -1,    37,   486,   480,   487,   313,    -1,    37,   486,   480,
     487,   312,    -1,   313,   765,    -1,   312,   765,    -1,   270,
     851,    -1,   165,   270,    -1,   852,   237,    -1,   917,   304,
     851,    -1,   579,   851,    -1,   110,   851,    -1,    93,   181,
      -1,   181,    -1,   149,    -1,   149,   389,   908,    -1,    38,
     368,   534,   906,    -1,    38,   275,   901,    -1,    56,   270,
      -1,    70,   270,    -1,   270,    -1,   181,    -1,    56,    -1,
      94,    -1,    -1,   271,    -1,    -1,   853,   906,    -1,    -1,
     177,   904,    -1,    -1,   145,    -1,   144,    -1,   167,    -1,
      -1,   167,   904,    -1,    -1,   144,   771,    -1,    -1,   859,
     901,   857,   861,    -1,    -1,   859,   860,   858,   675,    -1,
     122,    -1,   123,    -1,    -1,   136,    -1,    -1,   891,    -1,
     906,    -1,    -1,    48,   658,   863,   864,    -1,   864,   488,
     865,    -1,   865,    -1,    -1,   927,   866,   867,    -1,   274,
     306,   244,    55,    -1,   238,   102,    -1,   156,    -1,   235,
      -1,   181,    -1,   270,    -1,    70,    -1,    56,    -1,   124,
      -1,   252,    -1,    -1,   788,    -1,    -1,    64,   869,   870,
      -1,   870,   488,   871,    -1,   871,    -1,    70,    -1,    56,
      -1,   238,   102,    -1,    -1,    61,   873,   874,    -1,   848,
     181,   875,    -1,   280,   904,    -1,   475,   696,    -1,    52,
     696,    -1,   300,   906,    -1,    -1,    53,   118,   881,   880,
     163,   904,   879,   882,   166,   275,   901,   883,   886,   889,
     804,    -1,    53,   275,   901,   144,    56,    -1,    53,   118,
     144,    56,    -1,    -1,   179,    -1,    -1,   113,    -1,   184,
      -1,    -1,   436,    -1,   159,    -1,    -1,   111,   884,    -1,
     884,   885,    -1,   885,    -1,   278,   100,   891,    -1,   225,
     131,   100,   891,    -1,   131,   100,   891,    -1,   132,   100,
     891,    -1,    -1,   178,   887,    -1,   887,   888,    -1,   888,
      -1,   278,   100,   891,    -1,   269,   100,   891,    -1,    -1,
     159,   219,   178,    -1,   905,    -1,   214,    -1,   294,   279,
      -1,   890,   905,    -1,   905,    -1,   154,    -1,   494,    -1,
     894,    -1,   479,   895,    -1,   478,   895,    -1,   890,    -1,
     895,    -1,   218,    -1,   137,    -1,   283,    -1,   154,    -1,
     294,   154,    -1,   321,   890,    -1,   346,   890,    -1,   345,
     890,    -1,   219,    -1,   182,    -1,   350,    -1,   245,    -1,
     141,    -1,   899,    -1,   897,    -1,   906,   493,   480,    -1,
     906,   493,   906,   493,   480,    -1,   696,    -1,   906,    -1,
     906,   493,   906,    -1,   493,   906,   493,   906,    -1,   906,
     493,   906,   493,   906,    -1,   906,    -1,   906,   493,   906,
     493,   906,    -1,   906,   493,   906,    -1,   493,   906,    -1,
     906,    -1,   906,   493,   906,    -1,   493,   906,    -1,   906,
      -1,   157,    -1,   158,    -1,   279,    -1,   279,    -1,   903,
      -1,   909,    -1,   906,    -1,   904,    -1,   176,    -1,   907,
      -1,   907,   492,   907,    -1,   116,   695,    -1,    84,    -1,
      78,    -1,    25,    -1,   356,    -1,    85,    -1,    28,    -1,
     314,    -1,    90,    -1,    91,    -1,    29,    -1,    92,    -1,
      30,    -1,    93,    -1,    31,    -1,    95,    -1,    96,    -1,
      97,    -1,   101,    -1,    99,    -1,   102,    -1,   318,    -1,
     105,    -1,   106,    -1,   474,    -1,    33,    -1,     4,    -1,
     110,    -1,    34,    -1,   108,    -1,    35,    -1,   311,    -1,
     113,    -1,    36,    -1,    40,    -1,   118,    -1,   320,    -1,
     321,    -1,   375,    -1,   338,    -1,   121,    -1,   124,    -1,
     133,    -1,   126,    -1,    43,    -1,   231,    -1,   128,    -1,
     129,    -1,   465,    -1,   324,    -1,   382,    -1,   383,    -1,
     312,    -1,   134,    -1,    45,    -1,    46,    -1,    47,    -1,
     136,    -1,   325,    -1,   125,    -1,   130,    -1,   145,    -1,
     138,    -1,   139,    -1,   140,    -1,    48,    -1,   327,    -1,
     394,    -1,   397,    -1,   149,    -1,   147,    -1,     5,    -1,
     153,    -1,    49,    -1,   156,    -1,   401,    -1,   402,    -1,
     160,    -1,   162,    -1,   168,    -1,   473,    -1,   165,    -1,
     404,    -1,    51,    -1,     6,    -1,   174,    -1,   175,    -1,
     409,    -1,   179,    -1,    54,    -1,   181,    -1,   202,    -1,
      56,    -1,   185,    -1,   190,    -1,   187,    -1,   188,    -1,
     186,    -1,   189,    -1,   192,    -1,   191,    -1,   193,    -1,
     194,    -1,   195,    -1,   196,    -1,   197,    -1,   198,    -1,
     203,    -1,   204,    -1,   205,    -1,   206,    -1,   413,    -1,
     416,    -1,   207,    -1,   418,    -1,   417,    -1,   419,    -1,
     423,    -1,   424,    -1,   425,    -1,   208,    -1,   209,    -1,
     213,    -1,   211,    -1,     7,    -1,   212,    -1,   217,    -1,
      59,    -1,   215,    -1,   220,    -1,   427,    -1,   222,    -1,
     223,    -1,   232,    -1,   233,    -1,   428,    -1,   430,    -1,
     432,    -1,   337,    -1,     8,    -1,   236,    -1,   237,    -1,
     238,    -1,   339,    -1,   239,    -1,   242,    -1,   243,    -1,
     240,    -1,   241,    -1,   199,    -1,   200,    -1,   248,    -1,
      62,    -1,   250,    -1,    63,    -1,    64,    -1,   252,    -1,
     253,    -1,    65,    -1,    66,    -1,   256,    -1,   257,    -1,
     258,    -1,   259,    -1,    67,    -1,   439,    -1,   262,    -1,
     263,    -1,   264,    -1,   341,    -1,   265,    -1,   441,    -1,
     266,    -1,    70,    -1,    71,    -1,    12,    -1,   468,    -1,
     472,    -1,   470,    -1,    72,    -1,    73,    -1,   270,    -1,
      76,    -1,   271,    -1,   343,    -1,   442,    -1,   273,    -1,
      79,    -1,   276,    -1,   277,    -1,   344,    -1,   282,    -1,
      80,    -1,   345,    -1,   346,    -1,   284,    -1,   285,    -1,
     290,    -1,   292,    -1,   293,    -1,   295,    -1,    82,    -1,
     454,    -1,   299,    -1,   304,    -1,   302,    -1,   313,    -1,
     460,    -1,   309,    -1,   462,    -1,    -1,   260,   912,   911,
     913,    -1,    -1,   224,    -1,   914,    -1,   913,   488,   914,
      -1,   915,   919,    -1,   916,   920,    -1,   916,    -1,   147,
      -1,   179,    -1,   264,    -1,    -1,   222,    -1,    -1,   147,
      -1,   179,    -1,   264,    -1,    -1,   147,   493,    -1,   179,
     493,    -1,   264,   493,    -1,   921,   812,   924,    -1,   282,
     168,   175,   922,    -1,   492,   907,   812,   696,    -1,   492,
     492,   918,   921,   812,   924,    -1,   579,   583,    -1,   208,
     581,   585,    -1,   428,   812,   923,    -1,   428,   389,   908,
     812,   923,    -1,   906,    -1,   906,   493,   906,    -1,   119,
     493,   906,    -1,   244,   293,    -1,   244,   108,    -1,   250,
     244,    -1,   263,    -1,   279,    -1,   428,   486,   279,   487,
      -1,   427,   486,   279,   487,    -1,   696,    -1,   119,    -1,
     221,    -1,    86,    -1,    94,    -1,    -1,    55,   927,   926,
     928,    -1,   275,    -1,   274,    -1,   929,    -1,   928,   488,
     929,    -1,   901,   750,   930,    -1,   244,    -1,   307,    -1,
     184,   307,    -1,   244,   179,    -1,    81,   927,    -1,     5,
     901,   223,   750,    -1,     5,   902,     4,    -1,    -1,     5,
     902,   244,   933,   934,   752,   766,    -1,   935,    -1,   906,
     936,    -1,   139,    -1,     7,    -1,   139,    -1,     7,    -1,
       8,    -1,     6,    -1,    -1,   938,   937,   486,   817,   487,
      -1,    10,    -1,    13,    -1,    15,    -1,    14,    -1,    16,
      -1,    -1,   255,   940,   941,    -1,   944,   221,   955,   144,
     956,    -1,    86,   945,   488,   148,   224,   144,   956,    -1,
      -1,   148,   943,   944,   221,   955,   280,   956,   961,   962,
      -1,   946,    -1,    86,   945,    -1,    -1,   235,    -1,   947,
      -1,   946,   488,   947,    -1,    -1,    68,   948,   958,    -1,
      -1,    50,   949,   958,    -1,    -1,    83,   950,   958,    -1,
      -1,   246,   951,   958,    -1,    41,    -1,   298,    -1,   161,
      -1,    26,    -1,    38,    -1,    44,    -1,    46,    -1,   248,
      -1,   266,    -1,   236,    -1,   138,    -1,   148,   224,    -1,
      69,   117,    -1,    79,    -1,    38,   277,   274,    -1,    55,
     274,    -1,    63,    70,    -1,    63,    33,    -1,    -1,    87,
      -1,   954,   952,   953,    -1,   954,    -1,   273,   279,    -1,
     473,   279,    -1,   474,   279,    -1,   480,    -1,   906,   493,
     480,    -1,   480,   493,   480,    -1,   901,    -1,   957,    -1,
     956,   488,   957,    -1,   908,   402,   100,   279,    -1,   908,
     402,   100,   428,   279,    -1,   908,    -1,    -1,   486,   959,
     487,    -1,   959,   488,   960,    -1,   960,    -1,   906,    -1,
      -1,   251,   953,    -1,   251,   268,    -1,   251,   309,    -1,
     251,    59,    -1,    -1,   306,   963,    -1,   963,   964,    -1,
     964,    -1,   148,   224,    -1,   204,   770,    -1,   205,   770,
      -1,   203,   770,    -1,    -1,    30,   966,   967,    -1,    -1,
     460,    -1,    35,    -1,    65,    -1,    65,   280,    67,   906,
      -1,    67,   906,    -1,    -1,   972,    -1,    -1,   296,   978,
     973,   676,    -1,   972,    -1,   975,    -1,    -1,    -1,   976,
     977,    -1,   761,   765,    -1,   767,    -1,    -1,   127,    -1,
      86,    -1,   985,   980,   986,    -1,   678,    -1,   985,   982,
     986,    -1,   678,    -1,   985,   984,   986,    -1,   678,    -1,
     486,    68,    -1,   487,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   744,   744,   758,   761,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,   779,   780,   781,   782,   783,   784,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,   809,
     823,   824,   829,   843,   850,   860,   859,   875,   877,   881,
     882,   885,   897,   908,   907,   918,   919,   922,   927,   932,
     937,   942,   946,   951,   955,   959,   963,   967,   972,   976,
     980,   996,  1000,  1012,  1011,  1035,  1034,  1054,  1053,  1066,
    1065,  1081,  1082,  1083,  1089,  1098,  1099,  1099,  1103,  1105,
    1104,  1107,  1106,  1112,  1124,  1111,  1132,  1133,  1136,  1137,
    1140,  1141,  1144,  1145,  1148,  1149,  1152,  1153,  1156,  1159,
    1160,  1162,  1164,  1167,  1168,  1171,  1172,  1173,  1176,  1177,
    1178,  1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,
    1188,  1189,  1190,  1191,  1192,  1205,  1206,  1207,  1208,  1210,
    1213,  1230,  1246,  1256,  1257,  1258,  1259,  1262,  1263,  1264,
    1267,  1268,  1269,  1272,  1273,  1276,  1277,  1280,  1281,  1282,
    1285,  1286,  1290,  1291,  1295,  1296,  1303,  1309,  1317,  1331,
    1335,  1341,  1343,  1347,  1351,  1352,  1356,  1361,  1360,  1382,
    1383,  1384,  1385,  1387,  1389,  1391,  1393,  1395,  1398,  1401,
    1404,  1407,  1409,  1412,  1415,  1416,  1417,  1418,  1431,  1432,
    1434,  1436,  1448,  1450,  1452,  1454,  1455,  1456,  1457,  1458,
    1459,  1461,  1463,  1465,  1465,  1467,  1467,  1469,  1470,  1479,
    1480,  1481,  1482,  1483,  1484,  1485,  1486,  1490,  1494,  1495,
    1499,  1500,  1504,  1505,  1506,  1507,  1508,  1512,  1513,  1514,
    1515,  1516,  1519,  1521,  1522,  1526,  1527,  1528,  1531,  1538,
    1539,  1542,  1543,  1546,  1547,  1548,  1551,  1552,  1555,  1556,
    1559,  1560,  1563,  1564,  1567,  1568,  1569,  1570,  1572,  1573,
    1579,  1585,  1591,  1597,  1598,  1599,  1615,  1616,  1620,  1621,
    1625,  1633,  1637,  1638,  1642,  1651,  1655,  1656,  1659,  1669,
    1670,  1674,  1675,  1678,  1679,  1682,  1683,  1684,  1685,  1693,
    1695,  1697,  1702,  1701,  1713,  1714,  1717,  1718,  1722,  1723,
    1726,  1727,  1730,  1731,  1732,  1733,  1734,  1737,  1738,  1739,
    1740,  1741,  1744,  1745,  1746,  1758,  1759,  1762,  1763,  1766,
    1767,  1771,  1772,  1773,  1776,  1777,  1778,  1779,  1792,  1793,
    1794,  1797,  1798,  1802,  1805,  1806,  1809,  1810,  1825,  1826,
    1829,  1830,  1833,  1834,  1842,  1841,  1867,  1866,  1880,  1881,
    1884,  1885,  1886,  1887,  1888,  1891,  1899,  1900,  1906,  1908,
    1907,  1916,  1926,  1915,  1938,  1946,  1947,  1955,  1963,  1964,
    1965,  1972,  1980,  1993,  2014,  2020,  2028,  2029,  2032,  2033,
    2037,  2038,  2039,  2042,  2043,  2044,  2047,  2048,  2049,  2050,
    2058,  2057,  2068,  2076,  2075,  2085,  2095,  2103,  2104,  2111,
    2111,  2117,  2118,  2122,  2123,  2124,  2128,  2129,  2145,  2146,
    2151,  2150,  2161,  2160,  2171,  2170,  2180,  2181,  2182,  2187,
    2186,  2198,  2199,  2202,  2203,  2206,  2207,  2208,  2212,  2211,
    2224,  2223,  2234,  2235,  2238,  2239,  2242,  2243,  2244,  2245,
    2246,  2250,  2249,  2261,  2262,  2263,  2268,  2267,  2276,  2277,
    2280,  2292,  2301,  2302,  2305,  2318,  2319,  2324,  2323,  2333,
    2334,  2337,  2350,  2350,  2360,  2361,  2370,  2371,  2380,  2390,
    2392,  2395,  2415,  2419,  2418,  2438,  2446,  2438,  2452,  2453,
    2454,  2455,  2456,  2459,  2461,  2468,  2470,  2481,  2482,  2485,
    2486,  2492,  2493,  2494,  2495,  2501,  2507,  2508,  2512,  2515,
    2517,  2523,  2533,  2534,  2535,  2545,  2556,  2559,  2562,  2563,
    2566,  2567,  2568,  2569,  2570,  2574,  2575,  2579,  2580,  2583,
    2584,  2585,  2586,  2587,  2588,  2591,  2592,  2597,  2599,  2601,
    2603,  2607,  2609,  2611,  2612,  2613,  2614,  2615,  2620,  2622,
    2624,  2625,  2627,  2628,  2629,  2630,  2631,  2635,  2636,  2637,
    2638,  2639,  2640,  2641,  2642,  2643,  2644,  2645,  2646,  2647,
    2649,  2655,  2657,  2659,  2660,  2661,  2662,  2663,  2668,  2670,
    2672,  2673,  2675,  2676,  2677,  2678,  2679,  2683,  2684,  2685,
    2686,  2687,  2688,  2689,  2690,  2691,  2692,  2693,  2694,  2695,
    2697,  2699,  2703,  2705,  2707,  2709,  2713,  2715,  2717,  2718,
    2719,  2720,  2725,  2727,  2729,  2730,  2732,  2733,  2734,  2735,
    2736,  2740,  2741,  2742,  2743,  2744,  2745,  2746,  2747,  2748,
    2749,  2750,  2751,  2752,  2754,  2756,  2759,  2763,  2764,  2771,
    2772,  2773,  2778,  2783,  2794,  2795,  2796,  2797,  2798,  2802,
    2806,  2807,  2812,  2817,  2818,  2819,  2820,  2824,  2825,  2829,
    2835,  2837,  2843,  2845,  2847,  2849,  2860,  2871,  2882,  2893,
    2895,  2897,  2899,  2901,  2903,  2905,  2907,  2909,  2911,  2913,
    2918,  2920,  2922,  2927,  2929,  2931,  2933,  2938,  2940,  2942,
    2944,  2946,  2951,  2952,  2954,  2956,  2958,  2960,  2962,  2964,
    2966,  2968,  2970,  2972,  2974,  2978,  2980,  2990,  2992,  2994,
    2996,  2998,  3001,  3010,  3015,  3020,  3022,  3024,  3026,  3028,
    3030,  3032,  3034,  3039,  3044,  3046,  3048,  3050,  3052,  3054,
    3056,  3062,  3064,  3066,  3068,  3070,  3072,  3074,  3076,  3077,
    3079,  3081,  3083,  3085,  3087,  3089,  3091,  3093,  3095,  3097,
    3099,  3101,  3103,  3105,  3107,  3109,  3111,  3113,  3115,  3117,
    3124,  3131,  3138,  3145,  3152,  3159,  3163,  3168,  3170,  3172,
    3174,  3176,  3178,  3183,  3185,  3187,  3189,  3191,  3196,  3200,
    3202,  3204,  3206,  3208,  3212,  3215,  3218,  3220,  3222,  3224,
    3226,  3228,  3230,  3233,  3236,  3238,  3240,  3242,  3244,  3246,
    3249,  3251,  3253,  3255,  3260,  3261,  3262,  3266,  3267,  3270,
    3272,  3274,  3276,  3278,  3280,  3283,  3285,  3282,  3288,  3290,
    3292,  3294,  3296,  3298,  3301,  3300,  3312,  3313,  3316,  3317,
    3322,  3325,  3337,  3336,  3352,  3353,  3354,  3355,  3356,  3357,
    3358,  3359,  3360,  3361,  3365,  3365,  3370,  3371,  3374,  3375,
    3378,  3378,  3383,  3384,  3387,  3388,  3391,  3392,  3395,  3395,
    3400,  3406,  3414,  3415,  3416,  3417,  3419,  3423,  3421,  3431,
    3434,  3433,  3441,  3447,  3450,  3449,  3457,  3463,  3467,  3468,
    3469,  3473,  3473,  3489,  3491,  3503,  3506,  3507,  3523,  3541,
    3523,  3548,  3549,  3552,  3553,  3559,  3566,  3574,  3574,  3580,
    3581,  3585,  3589,  3593,  3599,  3608,  3616,  3617,  3618,  3619,
    3620,  3621,  3622,  3623,  3624,  3625,  3626,  3627,  3628,  3629,
    3630,  3631,  3632,  3633,  3637,  3638,  3639,  3640,  3643,  3645,
    3646,  3649,  3650,  3653,  3655,  3659,  3661,  3660,  3674,  3677,
    3676,  3691,  3693,  3703,  3705,  3708,  3710,  3714,  3715,  3728,
    3745,  3747,  3751,  3750,  3765,  3767,  3771,  3772,  3773,  3778,
    3784,  3788,  3789,  3793,  3797,  3804,  3811,  3823,  3827,  3835,
    3836,  3837,  3838,  3839,  3843,  3844,  3845,  3846,  3847,  3850,
    3853,  3852,  3873,  3874,  3877,  3878,  3881,  3893,  3893,  3903,
    3904,  3907,  3917,  3916,  3925,  3937,  3948,  3947,  3964,  3971,
    3971,  3982,  3989,  3996,  3995,  4007,  4008,  4011,  4019,  4020,
    4024,  4025,  4033,  4043,  4032,  4053,  4061,  4052,  4070,  4071,
    4072,  4073,  4077,  4078,  4081,  4082,  4085,  4094,  4095,  4096,
    4098,  4097,  4107,  4108,  4109,  4112,  4113,  4116,  4117,  4118,
    4118,  4119,  4119,  4123,  4124,  4127,  4129,  4132,  4140,  4141,
    4145,  4146,  4151,  4150,  4163,  4164,  4167,  4172,  4180,  4181,
    4184,  4186,  4186,  4194,  4203,  4193,  4224,  4225,  4228,  4235,
    4236,  4239,  4248,  4249,  4255,  4254,  4268,  4267,  4276,  4275,
    4279,  4278,  4285,  4286,  4289,  4295,  4306,  4307,  4311,  4312,
    4315,  4316,  4317,  4320,  4329,  4331,  4336,  4335,  4346,  4348,
    4354,  4361,  4368,  4367,  4370,  4378,  4388,  4392,  4397,  4396,
    4401,  4409,  4414,  4420,  4425,  4430,  4432,  4434,  4436,  4438,
    4440,  4442,  4444,  4450,  4452,  4454,  4456,  4458,  4481,  4488,
    4494,  4500,  4504,  4510,  4521,  4534,  4535,  4537,  4539,  4542,
    4543,  4545,  4547,  4552,  4553,  4556,  4557,  4560,  4561,  4564,
    4565,  4571,  4570,  4581,  4580,  4590,  4591,  4594,  4595,  4599,
    4600,  4601,  4609,  4608,  4619,  4620,  4623,  4623,  4624,  4625,
    4626,  4627,  4628,  4629,  4630,  4631,  4632,  4633,  4636,  4637,
    4641,  4640,  4649,  4650,  4653,  4654,  4655,  4659,  4658,  4667,
    4671,  4676,  4697,  4711,  4721,  4720,  4739,  4747,  4753,  4754,
    4757,  4758,  4759,  4763,  4764,  4765,  4767,  4769,  4772,  4773,
    4776,  4781,  4788,  4793,  4799,  4801,  4804,  4805,  4808,  4813,
    4819,  4821,  4830,  4835,  4837,  4839,  4844,  4846,  4860,  4883,
    4884,  4885,  4894,  4895,  4896,  4898,  4899,  4900,  4901,  4915,
    4916,  4917,  4920,  4921,  4922,  4923,  4924,  4932,  4933,  4936,
    4941,  4951,  4954,  4962,  4978,  4994,  5017,  5018,  5033,  5043,
    5046,  5047,  5048,  5052,  5056,  5057,  5082,  5094,  5107,  5108,
    5117,  5118,  5119,  5122,  5131,  5138,  5160,  5161,  5162,  5163,
    5164,  5165,  5166,  5167,  5168,  5169,  5170,  5171,  5172,  5173,
    5174,  5175,  5176,  5177,  5178,  5179,  5180,  5181,  5182,  5183,
    5184,  5185,  5186,  5187,  5188,  5189,  5190,  5191,  5192,  5193,
    5194,  5195,  5196,  5197,  5198,  5199,  5200,  5201,  5202,  5203,
    5204,  5205,  5206,  5207,  5208,  5209,  5210,  5211,  5212,  5213,
    5214,  5215,  5216,  5217,  5218,  5219,  5220,  5221,  5222,  5223,
    5224,  5225,  5226,  5227,  5228,  5229,  5230,  5231,  5232,  5233,
    5234,  5235,  5236,  5237,  5238,  5239,  5240,  5241,  5242,  5243,
    5244,  5245,  5246,  5247,  5248,  5249,  5250,  5251,  5252,  5253,
    5254,  5255,  5256,  5257,  5258,  5259,  5260,  5261,  5262,  5263,
    5264,  5265,  5266,  5267,  5268,  5269,  5270,  5271,  5272,  5273,
    5274,  5275,  5276,  5277,  5278,  5279,  5280,  5281,  5282,  5283,
    5284,  5285,  5286,  5287,  5288,  5289,  5290,  5291,  5292,  5293,
    5294,  5295,  5296,  5297,  5298,  5299,  5300,  5301,  5302,  5303,
    5304,  5305,  5306,  5307,  5308,  5309,  5310,  5311,  5312,  5313,
    5314,  5315,  5316,  5317,  5318,  5319,  5320,  5321,  5322,  5323,
    5324,  5325,  5326,  5327,  5328,  5329,  5330,  5331,  5332,  5333,
    5334,  5335,  5336,  5337,  5338,  5339,  5340,  5341,  5342,  5343,
    5344,  5345,  5346,  5347,  5348,  5349,  5350,  5351,  5352,  5353,
    5354,  5355,  5356,  5357,  5358,  5359,  5360,  5361,  5362,  5363,
    5364,  5365,  5366,  5367,  5368,  5375,  5374,  5388,  5389,  5392,
    5393,  5396,  5397,  5401,  5402,  5403,  5404,  5408,  5409,  5413,
    5414,  5415,  5416,  5420,  5421,  5422,  5423,  5427,  5433,  5447,
    5451,  5457,  5464,  5477,  5487,  5494,  5509,  5524,  5538,  5539,
    5540,  5541,  5545,  5546,  5553,  5562,  5563,  5564,  5565,  5566,
    5574,  5573,  5582,  5583,  5586,  5587,  5590,  5598,  5599,  5600,
    5601,  5605,  5614,  5621,  5629,  5628,  5642,  5643,  5647,  5648,
    5652,  5653,  5654,  5655,  5657,  5656,  5667,  5668,  5669,  5670,
    5671,  5678,  5677,  5694,  5697,  5705,  5704,  5722,  5723,  5726,
    5728,  5732,  5733,  5736,  5736,  5737,  5737,  5738,  5738,  5739,
    5739,  5740,  5741,  5742,  5743,  5744,  5745,  5746,  5747,  5748,
    5749,  5750,  5751,  5752,  5753,  5754,  5755,  5756,  5757,  5762,
    5763,  5767,  5768,  5772,  5782,  5792,  5805,  5817,  5829,  5841,
    5853,  5854,  5863,  5888,  5890,  5897,  5901,  5904,  5905,  5908,
    5929,  5930,  5934,  5938,  5942,  5949,  5950,  5953,  5954,  5958,
    5959,  5964,  5969,  5977,  5977,  5981,  5982,  5986,  5989,  5993,
    5999,  6011,  6012,  6017,  6016,  6042,  6043,  6047,  6049,  6049,
    6073,  6074,  6078,  6079,  6080,  6084,  6091,  6098,  6105,  6112,
    6119,  6125,  6140
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "END_OF_INPUT", "CLOSE_SYM",
  "HANDLER_SYM", "LAST_SYM", "NEXT_SYM", "PREV_SYM", "DIV_SYM", "EQ",
  "EQUAL_SYM", "SOUNDS_SYM", "GE", "GT_SYM", "LE", "LT", "NE", "IS",
  "MOD_SYM", "SHIFT_LEFT", "SHIFT_RIGHT", "SET_VAR", "ABORT_SYM", "ADD",
  "AFTER_SYM", "ALTER", "ANALYZE_SYM", "ANY_SYM", "AVG_SYM", "BEGIN_SYM",
  "BINLOG_SYM", "CHANGE", "CLIENT_SYM", "COMMENT_SYM", "COMMIT_SYM",
  "CONSISTENT_SYM", "COUNT_SYM", "CREATE", "CROSS", "CUBE_SYM",
  "DELETE_SYM", "DUAL_SYM", "DO_SYM", "DROP", "EVENTS_SYM", "EXECUTE_SYM",
  "EXPANSION_SYM", "FLUSH_SYM", "HELP_SYM", "INSERT", "RELAY_THREAD",
  "KILL_SYM", "LOAD", "LOCKS_SYM", "LOCK_SYM", "MASTER_SYM", "MAX_SYM",
  "MIN_SYM", "NONE_SYM", "OPTIMIZE", "PURGE", "REPAIR", "REPLICATION",
  "RESET_SYM", "ROLLBACK_SYM", "ROLLUP_SYM", "SAVEPOINT_SYM", "SELECT_SYM",
  "SHOW", "SLAVE", "SNAPSHOT_SYM", "SQL_THREAD", "START_SYM", "STD_SYM",
  "VARIANCE_SYM", "STOP_SYM", "SUM_SYM", "ADDDATE_SYM", "SUPER_SYM",
  "TRUNCATE_SYM", "UNLOCK_SYM", "UNTIL_SYM", "UPDATE_SYM", "ACTION",
  "AGGREGATE_SYM", "ALL", "AND_SYM", "AS", "ASC", "AUTO_INC",
  "AVG_ROW_LENGTH", "BACKUP_SYM", "BERKELEY_DB_SYM", "BINARY", "BIT_SYM",
  "BOOL_SYM", "BOOLEAN_SYM", "BOTH", "BTREE_SYM", "BY", "BYTE_SYM",
  "CACHE_SYM", "CASCADE", "CAST_SYM", "CHARSET", "CHECKSUM_SYM",
  "CHECK_SYM", "COMMITTED_SYM", "COLLATE_SYM", "COLLATION_SYM", "COLUMNS",
  "COLUMN_SYM", "CONCURRENT", "CONSTRAINT", "CONVERT_SYM", "CURRENT_USER",
  "DATABASES", "DATA_SYM", "DEFAULT", "DELAYED_SYM", "DELAY_KEY_WRITE_SYM",
  "DESC", "DESCRIBE", "DES_KEY_FILE", "DISABLE_SYM", "DISCARD", "DISTINCT",
  "DUPLICATE_SYM", "DYNAMIC_SYM", "ENABLE_SYM", "ENCLOSED", "ESCAPED",
  "DIRECTORY_SYM", "ESCAPE_SYM", "EXISTS", "EXTENDED_SYM", "FALSE_SYM",
  "FILE_SYM", "FIRST_SYM", "FIXED_SYM", "FLOAT_NUM", "FORCE_SYM",
  "FOREIGN", "FROM", "FULL", "FULLTEXT_SYM", "GLOBAL_SYM", "GRANT",
  "GRANTS", "GREATEST_SYM", "GROUP", "HAVING", "HASH_SYM", "HEX_NUM",
  "HIGH_PRIORITY", "HOSTS_SYM", "IDENT", "IDENT_QUOTED", "IGNORE_SYM",
  "IMPORT", "INDEX_SYM", "INDEXES", "INFILE", "INNER_SYM", "INNOBASE_SYM",
  "INTO", "IN_SYM", "ISOLATION", "JOIN_SYM", "KEYS", "KEY_SYM", "LEADING",
  "LEAST_SYM", "LEAVES", "LEVEL_SYM", "LEX_HOSTNAME", "LIKE", "LINES",
  "LOCAL_SYM", "LOG_SYM", "LOGS_SYM", "LONG_NUM", "LONG_SYM",
  "LOW_PRIORITY", "MASTER_HOST_SYM", "MASTER_USER_SYM",
  "MASTER_LOG_FILE_SYM", "MASTER_LOG_POS_SYM", "MASTER_PASSWORD_SYM",
  "MASTER_PORT_SYM", "MASTER_CONNECT_RETRY_SYM", "MASTER_SERVER_ID_SYM",
  "MASTER_SSL_SYM", "MASTER_SSL_CA_SYM", "MASTER_SSL_CAPATH_SYM",
  "MASTER_SSL_CERT_SYM", "MASTER_SSL_CIPHER_SYM", "MASTER_SSL_KEY_SYM",
  "RELAY_LOG_FILE_SYM", "RELAY_LOG_POS_SYM", "MATCH", "MAX_ROWS",
  "MAX_CONNECTIONS_PER_HOUR", "MAX_QUERIES_PER_HOUR",
  "MAX_UPDATES_PER_HOUR", "MEDIUM_SYM", "MIN_ROWS", "NAMES_SYM",
  "NATIONAL_SYM", "NATURAL", "NDBCLUSTER_SYM", "NEW_SYM", "NCHAR_SYM",
  "NCHAR_STRING", "NVARCHAR_SYM", "NOT", "NO_SYM", "NULL_SYM", "NUM",
  "OFFSET_SYM", "ON", "ONE_SHOT_SYM", "OPEN_SYM", "OPTION", "OPTIONALLY",
  "OR_SYM", "OR_OR_CONCAT", "ORDER_SYM", "OUTER", "OUTFILE", "DUMPFILE",
  "PACK_KEYS_SYM", "PARTIAL", "PRIMARY_SYM", "PRIVILEGES", "PROCESS",
  "PROCESSLIST_SYM", "QUERY_SYM", "RAID_0_SYM", "RAID_STRIPED_SYM",
  "RAID_TYPE", "RAID_CHUNKS", "RAID_CHUNKSIZE", "READ_SYM", "REAL_NUM",
  "REFERENCES", "REGEXP", "RELOAD", "RENAME", "REPEATABLE_SYM",
  "REQUIRE_SYM", "RESOURCES", "RESTORE_SYM", "RESTRICT", "REVOKE",
  "ROWS_SYM", "ROW_FORMAT_SYM", "ROW_SYM", "RTREE_SYM", "SET",
  "SEPARATOR_SYM", "SERIAL_SYM", "SERIALIZABLE_SYM", "SESSION_SYM",
  "SIMPLE_SYM", "SHUTDOWN", "SPATIAL_SYM", "SSL_SYM", "STARTING",
  "STATUS_SYM", "STORAGE_SYM", "STRAIGHT_JOIN", "SUBJECT_SYM", "TABLES",
  "TABLE_SYM", "TABLESPACE", "TEMPORARY", "TERMINATED", "TEXT_STRING",
  "TO_SYM", "TRAILING", "TRANSACTION_SYM", "TRUE_SYM", "TYPE_SYM",
  "TYPES_SYM", "FUNC_ARG0", "FUNC_ARG1", "FUNC_ARG2", "FUNC_ARG3",
  "UDF_RETURNS_SYM", "UDF_SONAME_SYM", "UDF_SYM", "UNCOMMITTED_SYM",
  "UNDERSCORE_CHARSET", "UNICODE_SYM", "UNION_SYM", "UNIQUE_SYM", "USAGE",
  "USE_FRM", "USE_SYM", "USING", "VALUE_SYM", "VALUES", "VARIABLES",
  "WHERE", "WITH", "WRITE_SYM", "NO_WRITE_TO_BINLOG", "X509_SYM", "XOR",
  "COMPRESSED_SYM", "ERRORS", "WARNINGS", "ASCII_SYM", "BIGINT",
  "BLOB_SYM", "CHAR_SYM", "CHANGED", "COALESCE", "DATETIME", "DATE_SYM",
  "DECIMAL_SYM", "DOUBLE_SYM", "ENUM", "FAST_SYM", "FLOAT_SYM",
  "GEOMETRY_SYM", "INT_SYM", "LIMIT", "LONGBLOB", "LONGTEXT", "MEDIUMBLOB",
  "MEDIUMINT", "MEDIUMTEXT", "NUMERIC_SYM", "PRECISION", "PREPARE_SYM",
  "DEALLOCATE_SYM", "QUICK", "REAL", "SIGNED_SYM", "SMALLINT",
  "STRING_SYM", "TEXT_SYM", "TIMESTAMP", "TIME_SYM", "TINYBLOB", "TINYINT",
  "TINYTEXT", "ULONGLONG_NUM", "UNSIGNED", "VARBINARY", "VARCHAR",
  "VARYING", "ZEROFILL", "AGAINST", "ATAN", "BETWEEN_SYM", "BIT_AND",
  "BIT_OR", "BIT_XOR", "CASE_SYM", "CONCAT", "CONCAT_WS", "CONVERT_TZ_SYM",
  "CURDATE", "CURTIME", "DATABASE", "DATE_ADD_INTERVAL",
  "DATE_SUB_INTERVAL", "DAY_HOUR_SYM", "DAY_MICROSECOND_SYM",
  "DAY_MINUTE_SYM", "DAY_SECOND_SYM", "DAY_SYM", "DECODE_SYM",
  "DES_ENCRYPT_SYM", "DES_DECRYPT_SYM", "ELSE", "ELT_FUNC", "ENCODE_SYM",
  "ENGINE_SYM", "ENGINES_SYM", "ENCRYPT", "EXPORT_SET", "EXTRACT_SYM",
  "FIELD_FUNC", "FORMAT_SYM", "FOR_SYM", "FROM_UNIXTIME",
  "GEOMCOLLFROMTEXT", "GEOMFROMTEXT", "GEOMFROMWKB", "GEOMETRYCOLLECTION",
  "GROUP_CONCAT_SYM", "GROUP_UNIQUE_USERS", "GET_FORMAT",
  "HOUR_MICROSECOND_SYM", "HOUR_MINUTE_SYM", "HOUR_SECOND_SYM", "HOUR_SYM",
  "IDENTIFIED_SYM", "IF", "INSERT_METHOD", "INTERVAL_SYM",
  "LAST_INSERT_ID", "LEFT", "LINEFROMTEXT", "LINESTRING", "LOCATE",
  "MAKE_SET_SYM", "MASTER_POS_WAIT", "MICROSECOND_SYM",
  "MINUTE_MICROSECOND_SYM", "MINUTE_SECOND_SYM", "MINUTE_SYM", "MODE_SYM",
  "MODIFY_SYM", "MONTH_SYM", "MLINEFROMTEXT", "MPOINTFROMTEXT",
  "MPOLYFROMTEXT", "MULTILINESTRING", "MULTIPOINT", "MULTIPOLYGON",
  "NOW_SYM", "OLD_PASSWORD", "PASSWORD", "POINTFROMTEXT", "POINT_SYM",
  "POLYFROMTEXT", "POLYGON", "POSITION_SYM", "PROCEDURE", "RAND",
  "REPLACE", "RIGHT", "ROUND", "SECOND_SYM", "SECOND_MICROSECOND_SYM",
  "SHARE_SYM", "SUBDATE_SYM", "SUBSTRING", "SUBSTRING_INDEX", "TRIM",
  "UDA_CHAR_SUM", "UDA_FLOAT_SUM", "UDA_INT_SUM", "UDF_CHAR_FUNC",
  "UDF_FLOAT_FUNC", "UDF_INT_FUNC", "UNIQUE_USERS", "UNIX_TIMESTAMP",
  "USER", "UTC_DATE_SYM", "UTC_TIME_SYM", "UTC_TIMESTAMP_SYM", "WEEK_SYM",
  "WHEN_SYM", "WORK_SYM", "YEAR_MONTH_SYM", "YEAR_SYM", "YEARWEEK",
  "BENCHMARK_SYM", "END", "THEN_SYM", "SQL_BIG_RESULT", "SQL_CACHE_SYM",
  "SQL_CALC_FOUND_ROWS", "SQL_NO_CACHE_SYM", "SQL_SMALL_RESULT",
  "SQL_BUFFER_RESULT", "ISSUER_SYM", "CIPHER_SYM", "BEFORE_SYM", "'|'",
  "'&'", "'-'", "'+'", "'*'", "'/'", "'%'", "'^'", "'~'", "NEG", "'('",
  "')'", "','", "'!'", "'{'", "'}'", "'@'", "'.'", "'?'", "$accept",
  "query", "verb_clause", "deallocate", "deallocate_or_drop", "prepare",
  "prepare_src", "execute", "@1", "execute_using", "execute_var_list",
  "execute_var_ident", "help", "change", "@2", "master_defs", "master_def",
  "master_file_def", "create", "@3", "@4", "@5", "@6", "create2",
  "create2a", "@7", "create3", "@8", "@9", "create_select", "@10", "@11",
  "opt_as", "opt_create_database_options", "create_database_options",
  "create_database_option", "opt_table_options", "table_options",
  "table_option", "opt_if_not_exists", "opt_create_table_options",
  "create_table_options_space_separated", "create_table_options",
  "create_table_option", "default_charset", "default_collation",
  "storage_engines", "row_types", "raid_types", "merge_insert_types",
  "opt_select_from", "udf_func_type", "udf_type", "field_list",
  "field_list_item", "column_def", "key_def", "opt_check_constraint",
  "check_constraint", "opt_constraint", "constraint", "field_spec", "@12",
  "type", "@13", "@14", "spatial_type", "char", "nchar", "varchar",
  "nvarchar", "int_type", "real_type", "float_options", "precision",
  "field_options", "field_opt_list", "field_option", "opt_len",
  "opt_precision", "opt_attribute", "opt_attribute_list", "attribute",
  "now_or_signed_literal", "charset", "charset_name",
  "charset_name_or_default", "old_or_new_charset_name",
  "old_or_new_charset_name_or_default", "collation_name", "opt_collate",
  "collation_name_or_default", "opt_default", "opt_binary", "opt_primary",
  "references", "@15", "opt_ref_list", "ref_list", "opt_on_delete",
  "opt_on_delete_list", "opt_on_delete_item", "delete_option", "key_type",
  "constraint_key_type", "key_or_index", "opt_key_or_index",
  "keys_or_index", "opt_unique_or_fulltext", "key_alg",
  "opt_btree_or_rtree", "key_list", "key_part", "opt_ident",
  "opt_component", "string_list", "alter", "@16", "@17", "ident_or_empty",
  "alter_list", "add_column", "alter_list_item", "@18", "@19", "@20",
  "opt_column", "opt_ignore", "opt_restrict", "opt_place", "opt_to",
  "slave", "@21", "@22", "start", "start_transaction_opts",
  "slave_thread_opts", "@23", "slave_thread_opt_list", "slave_thread_opt",
  "slave_until", "slave_until_opts", "restore", "@24", "backup", "@25",
  "checksum", "@26", "opt_checksum_type", "repair", "@27",
  "opt_mi_repair_type", "mi_repair_types", "mi_repair_type", "analyze",
  "@28", "check", "@29", "opt_mi_check_type", "mi_check_types",
  "mi_check_type", "optimize", "@30", "opt_no_write_to_binlog", "rename",
  "@31", "table_to_table_list", "table_to_table", "keycache",
  "keycache_list", "assign_to_keycache", "key_cache_name", "preload",
  "@32", "preload_list", "preload_keys", "cache_keys_spec", "@33",
  "cache_key_list_or_empty", "opt_ignore_leaves", "select", "select_init",
  "select_paren", "select_init2", "@34", "select_part2", "@35", "@36",
  "select_into", "select_from", "select_options", "select_option_list",
  "select_option", "select_lock_type", "select_item_list", "select_item",
  "remember_name", "remember_end", "select_item2", "select_alias",
  "optional_braces", "expr", "comp_op", "all_or_any", "expr_expr",
  "no_in_expr", "no_and_expr", "interval_expr", "simple_expr",
  "geometry_function", "fulltext_options", "udf_expr_list", "sum_expr",
  "@37", "@38", "@39", "opt_distinct", "opt_gconcat_separator",
  "opt_gorder_clause", "in_sum_expr", "@40", "cast_type", "expr_list",
  "@41", "expr_list2", "ident_list_arg", "ident_list", "@42",
  "ident_list2", "opt_expr", "opt_else", "when_list", "@43", "when_list2",
  "join_table_list", "@44", "@45", "@46", "normal_join", "join_table",
  "@47", "select_derived", "select_derived2", "@48", "@49", "opt_outer",
  "opt_key_definition", "key_usage_list", "@50", "key_list_or_empty",
  "key_usage_list2", "using_list", "interval", "date_time_type",
  "table_alias", "opt_table_alias", "opt_all", "where_clause", "@51",
  "having_clause", "@52", "opt_escape", "group_clause", "group_list",
  "olap_opt", "opt_order_clause", "order_clause", "@53", "order_list",
  "order_dir", "opt_limit_clause_init", "opt_limit_clause", "limit_clause",
  "limit_options", "delete_limit_clause", "ULONG_NUM", "ulonglong_num",
  "procedure_clause", "@54", "procedure_list", "procedure_list2",
  "procedure_item", "select_var_list_init", "@55", "select_var_list",
  "select_var_ident", "into", "@56", "do", "@57", "drop", "@58", "@59",
  "table_list", "table_name", "if_exists", "opt_temporary", "insert",
  "@60", "@61", "replace", "@62", "@63", "insert_lock_option",
  "replace_lock_option", "insert2", "insert_table", "insert_field_spec",
  "@64", "opt_field_spec", "fields", "insert_values", "@65", "@66",
  "values_list", "ident_eq_list", "ident_eq_value", "equal", "opt_equal",
  "no_braces", "@67", "opt_values", "values", "expr_or_default",
  "opt_insert_update", "@68", "update", "@69", "@70", "update_list",
  "update_elem", "insert_update_list", "insert_update_elem",
  "opt_low_priority", "delete", "@71", "single_multi", "@72", "@73", "@74",
  "table_wild_list", "table_wild_one", "opt_wild", "opt_delete_options",
  "opt_delete_option", "truncate", "opt_table_sym", "show", "@75",
  "show_param", "@76", "@77", "show_engine_param", "master_or_binary",
  "opt_storage", "opt_db", "wild", "opt_full", "from_or_in", "binlog_in",
  "binlog_from", "describe", "@78", "@79", "describe_command",
  "opt_extended_describe", "opt_describe_column", "flush", "@80",
  "flush_options", "flush_option", "@81", "opt_table_list", "reset", "@82",
  "reset_options", "reset_option", "purge", "@83", "purge_options",
  "purge_option", "kill", "use", "load", "@84", "opt_local",
  "load_data_lock", "opt_duplicate", "opt_field_term", "field_term_list",
  "field_term", "opt_line_term", "line_term_list", "line_term",
  "opt_ignore_lines", "text_literal", "text_string", "param_marker",
  "signed_literal", "literal", "NUM_literal", "insert_ident", "table_wild",
  "order_ident", "simple_ident", "field_ident", "table_ident",
  "table_ident_nodb", "IDENT_sys", "TEXT_STRING_sys",
  "TEXT_STRING_literal", "ident", "ident_or_text", "user", "keyword",
  "set", "@85", "opt_option", "option_value_list", "option_value_ext",
  "option_type_ext", "option_type", "opt_var_type", "opt_var_ident_type",
  "sys_option_value", "option_value", "internal_variable_name",
  "isolation_types", "text_or_password", "set_expr_or_default", "lock",
  "@86", "table_or_tables", "table_lock_list", "table_lock", "lock_option",
  "unlock", "handler", "@87", "handler_read_or_scan",
  "handler_scan_function", "handler_rkey_function", "@88",
  "handler_rkey_mode", "revoke", "@89", "revoke_command", "grant", "@90",
  "grant_privileges", "opt_privileges", "grant_privilege_list",
  "grant_privilege", "@91", "@92", "@93", "@94", "opt_and", "require_list",
  "require_list_element", "opt_table", "user_list", "grant_user",
  "opt_column_list", "column_list", "column_list_id", "require_clause",
  "grant_options", "grant_option_list", "grant_option", "begin", "@95",
  "opt_work", "commit", "rollback", "savepoint", "union_clause",
  "union_list", "@96", "union_opt", "optional_order_or_limit", "@97",
  "order_or_limit", "union_option", "singlerow_subselect",
  "singlerow_subselect_init", "exists_subselect", "exists_subselect_init",
  "in_subselect", "in_subselect_init", "subselect_start", "subselect_end", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,   689,   690,   691,   692,   693,   694,
     695,   696,   697,   698,   699,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,   710,   711,   712,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,   723,   724,
     725,   726,   727,   728,   729,   730,   124,    38,    45,    43,
      42,    47,    37,    94,   126,   731,    40,    41,    44,    33,
     123,   125,    64,    46,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   495,   496,   496,   497,   497,   497,   497,   497,   497,
     497,   497,   497,   497,   497,   497,   497,   497,   497,   497,
     497,   497,   497,   497,   497,   497,   497,   497,   497,   497,
     497,   497,   497,   497,   497,   497,   497,   497,   497,   497,
     497,   497,   497,   497,   497,   497,   497,   497,   497,   498,
     499,   499,   500,   501,   501,   503,   502,   504,   504,   505,
     505,   506,   507,   509,   508,   510,   510,   511,   511,   511,
     511,   511,   511,   511,   511,   511,   511,   511,   511,   512,
     512,   512,   512,   514,   513,   515,   513,   516,   513,   517,
     513,   518,   518,   518,   518,   519,   520,   519,   521,   522,
     521,   523,   521,   525,   526,   524,   527,   527,   528,   528,
     529,   529,   530,   530,   531,   531,   532,   532,   533,   534,
     534,   535,   535,   536,   536,   537,   537,   537,   538,   538,
     538,   538,   538,   538,   538,   538,   538,   538,   538,   538,
     538,   538,   538,   538,   538,   538,   538,   538,   538,   538,
     539,   540,   541,   542,   542,   542,   542,   543,   543,   543,
     544,   544,   544,   545,   545,   546,   546,   547,   547,   547,
     548,   548,   549,   549,   550,   550,   551,   551,   551,   551,
     551,   552,   552,   553,   554,   554,   555,   557,   556,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   559,   558,   560,   558,   558,   558,   561,
     561,   561,   561,   561,   561,   561,   561,   562,   563,   563,
     564,   564,   565,   565,   565,   565,   565,   566,   566,   566,
     566,   566,   567,   567,   567,   568,   568,   568,   569,   570,
     570,   571,   571,   572,   572,   572,   573,   573,   574,   574,
     575,   575,   576,   576,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   578,   578,   579,   579,
     580,   580,   581,   581,   582,   582,   583,   583,   584,   585,
     585,   586,   586,   587,   587,   588,   588,   588,   588,   588,
     589,   589,   591,   590,   592,   592,   593,   593,   594,   594,
     595,   595,   596,   596,   596,   596,   596,   597,   597,   597,
     597,   597,   598,   598,   598,   599,   599,   600,   600,   601,
     601,   602,   602,   602,   603,   603,   603,   603,   604,   604,
     604,   605,   605,   605,   606,   606,   607,   607,   608,   608,
     609,   609,   610,   610,   612,   611,   613,   611,   614,   614,
     615,   615,   615,   615,   615,   616,   617,   617,   617,   618,
     617,   619,   620,   617,   617,   617,   617,   617,   617,   617,
     617,   617,   617,   617,   617,   617,   621,   621,   622,   622,
     623,   623,   623,   624,   624,   624,   625,   625,   625,   625,
     627,   626,   626,   628,   626,   626,   629,   630,   630,   632,
     631,   633,   633,   634,   634,   634,   635,   635,   636,   636,
     638,   637,   640,   639,   642,   641,   643,   643,   643,   645,
     644,   646,   646,   647,   647,   648,   648,   648,   650,   649,
     652,   651,   653,   653,   654,   654,   655,   655,   655,   655,
     655,   657,   656,   658,   658,   658,   660,   659,   661,   661,
     662,   663,   664,   664,   665,   666,   666,   668,   667,   669,
     669,   670,   672,   671,   673,   673,   674,   674,   675,   676,
     676,   677,   677,   679,   678,   681,   682,   680,   683,   683,
     683,   683,   683,   684,   684,   685,   685,   686,   686,   687,
     687,   687,   687,   687,   687,   687,   687,   687,   687,   688,
     688,   688,   689,   689,   689,   690,   691,   692,   693,   693,
     694,   694,   694,   694,   694,   695,   695,   696,   696,   697,
     697,   697,   697,   697,   697,   698,   698,   699,   699,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     699,   700,   700,   700,   700,   700,   700,   700,   700,   700,
     700,   700,   700,   700,   700,   700,   700,   700,   700,   700,
     700,   700,   700,   700,   700,   700,   700,   700,   700,   700,
     700,   700,   701,   701,   701,   701,   701,   701,   701,   701,
     701,   701,   701,   701,   701,   701,   701,   701,   701,   701,
     701,   701,   701,   701,   701,   701,   701,   701,   701,   701,
     701,   701,   701,   701,   701,   701,   702,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     703,   703,   703,   703,   703,   703,   703,   703,   703,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   704,   705,   705,   705,   706,   706,   707,
     707,   707,   707,   707,   707,   708,   709,   707,   707,   707,
     707,   707,   707,   707,   710,   707,   711,   711,   712,   712,
     713,   713,   715,   714,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   718,   717,   719,   719,   720,   720,
     722,   721,   723,   723,   724,   724,   725,   725,   727,   726,
     728,   728,   729,   729,   729,   729,   729,   730,   729,   729,
     731,   729,   729,   729,   732,   729,   729,   729,   733,   733,
     733,   735,   734,   734,   734,   734,   736,   736,   738,   739,
     737,   740,   740,   741,   741,   741,   741,   743,   742,   744,
     744,   745,   745,   745,   746,   746,   747,   747,   747,   747,
     747,   747,   747,   747,   747,   747,   747,   747,   747,   747,
     747,   747,   747,   747,   748,   748,   748,   748,   749,   749,
     749,   750,   750,   751,   751,   752,   753,   752,   754,   755,
     754,   756,   756,   757,   757,   758,   758,   759,   759,   759,
     760,   760,   762,   761,   763,   763,   764,   764,   764,   765,
     765,   766,   766,   767,   768,   768,   768,   769,   769,   770,
     770,   770,   770,   770,   771,   771,   771,   771,   771,   772,
     773,   772,   774,   774,   775,   775,   776,   778,   777,   779,
     779,   780,   782,   781,   781,   781,   784,   783,   785,   786,
     785,   785,   785,   787,   785,   788,   788,   789,   790,   790,
     791,   791,   793,   794,   792,   796,   797,   795,   798,   798,
     798,   798,   799,   799,   800,   800,   801,   802,   802,   802,
     803,   802,   804,   804,   804,   805,   805,   806,   806,   807,
     806,   808,   806,   809,   809,   810,   810,   811,   812,   812,
     813,   813,   815,   814,   816,   816,   817,   817,   818,   818,
     819,   820,   819,   822,   823,   821,   824,   824,   825,   826,
     826,   827,   828,   828,   830,   829,   832,   831,   833,   831,
     834,   831,   835,   835,   836,   836,   837,   837,   838,   838,
     839,   839,   839,   840,   841,   841,   843,   842,   844,   844,
     844,   844,   845,   844,   844,   844,   844,   844,   846,   844,
     844,   844,   844,   844,   844,   844,   844,   844,   844,   844,
     844,   844,   844,   844,   844,   844,   844,   844,   844,   844,
     844,   844,   844,   847,   847,   848,   848,   849,   849,   850,
     850,   851,   851,   852,   852,   853,   853,   854,   854,   855,
     855,   857,   856,   858,   856,   859,   859,   860,   860,   861,
     861,   861,   863,   862,   864,   864,   866,   865,   865,   865,
     865,   865,   865,   865,   865,   865,   865,   865,   867,   867,
     869,   868,   870,   870,   871,   871,   871,   873,   872,   874,
     875,   875,   876,   877,   879,   878,   878,   878,   880,   880,
     881,   881,   881,   882,   882,   882,   883,   883,   884,   884,
     885,   885,   885,   885,   886,   886,   887,   887,   888,   888,
     889,   889,   890,   890,   890,   890,   891,   891,   892,   893,
     893,   893,   894,   894,   894,   894,   894,   894,   894,   894,
     894,   894,   895,   895,   895,   895,   895,   896,   896,   897,
     897,   898,   899,   899,   899,   899,   900,   900,   900,   900,
     901,   901,   901,   902,   903,   903,   904,   905,   906,   906,
     907,   907,   907,   908,   908,   908,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   909,   909,   909,   909,   909,
     909,   909,   909,   909,   909,   911,   910,   912,   912,   913,
     913,   914,   914,   915,   915,   915,   915,   916,   916,   917,
     917,   917,   917,   918,   918,   918,   918,   919,   919,   920,
     920,   920,   920,   920,   920,   921,   921,   921,   922,   922,
     922,   922,   923,   923,   923,   924,   924,   924,   924,   924,
     926,   925,   927,   927,   928,   928,   929,   930,   930,   930,
     930,   931,   932,   932,   933,   932,   934,   934,   935,   935,
     936,   936,   936,   936,   937,   936,   938,   938,   938,   938,
     938,   940,   939,   941,   941,   943,   942,   944,   944,   945,
     945,   946,   946,   948,   947,   949,   947,   950,   947,   951,
     947,   947,   947,   947,   947,   947,   947,   947,   947,   947,
     947,   947,   947,   947,   947,   947,   947,   947,   947,   952,
     952,   953,   953,   954,   954,   954,   955,   955,   955,   955,
     956,   956,   957,   957,   957,   958,   958,   959,   959,   960,
     961,   961,   961,   961,   961,   962,   962,   963,   963,   964,
     964,   964,   964,   966,   965,   967,   967,   968,   969,   969,
     970,   971,   971,   973,   972,   974,   974,   975,   976,   975,
     977,   977,   978,   978,   978,   979,   980,   981,   982,   983,
     984,   985,   986
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     4,     1,     2,     0,     4,     0,     2,     3,
       1,     2,     2,     0,     5,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     0,     7,     0,    11,     0,     6,     0,
       9,     2,     2,     2,     4,     4,     0,     4,     0,     0,
       5,     0,     7,     0,     0,     6,     0,     1,     0,     1,
       1,     2,     1,     1,     0,     1,     1,     2,     1,     0,
       3,     0,     1,     1,     2,     1,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     5,     1,     1,     3,     4,     4,
       4,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     1,     1,     1,     1,
       1,     3,     1,     1,     2,     2,     6,     7,     8,     2,
       2,     0,     1,     2,     0,     1,     2,     0,     4,     3,
       3,     3,     2,     1,     1,     5,     2,     4,     1,     4,
       1,     5,     4,     4,     3,     1,     1,     2,     1,     1,
       2,     1,     1,     1,     2,     3,     2,     3,     2,     2,
       3,     3,     3,     0,     6,     0,     6,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     2,     1,     2,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     3,     1,     5,     0,
       1,     2,     1,     1,     1,     1,     0,     3,     0,     1,
       0,     1,     2,     1,     1,     2,     2,     4,     1,     3,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       2,     1,     1,     0,     1,     0,     1,     1,     1,     2,
       0,     1,     0,     4,     1,     4,     3,     1,     0,     1,
       2,     1,     3,     3,     2,     2,     2,     1,     1,     2,
       2,     2,     1,     2,     2,     2,     2,     1,     1,     0,
       1,     1,     1,     1,     0,     1,     1,     1,     0,     2,
       2,     1,     1,     1,     4,     2,     1,     4,     0,     1,
       0,     2,     1,     3,     0,     6,     0,     5,     0,     1,
       0,     2,     2,     1,     3,     2,     3,     2,     4,     0,
       6,     0,     0,     8,     4,     4,     3,     3,     2,     2,
       6,     5,     3,     5,     1,     1,     0,     1,     0,     1,
       0,     1,     1,     0,     2,     1,     0,     1,     1,     1,
       0,     5,     3,     0,     5,     3,     3,     0,     3,     0,
       2,     1,     3,     0,     1,     1,     0,     2,     1,     3,
       0,     6,     0,     6,     0,     5,     0,     1,     1,     0,
       6,     0,     1,     1,     2,     1,     1,     1,     0,     6,
       0,     5,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     0,     6,     0,     1,     1,     0,     4,     1,     3,
       3,     5,     1,     3,     2,     1,     1,     0,     6,     1,
       3,     3,     0,     2,     0,     4,     0,     2,     1,     2,
       4,     2,     3,     0,     3,     0,     0,     6,     2,     1,
       1,     2,     2,     8,     3,     0,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       2,     4,     3,     1,     1,     4,     0,     0,     1,     1,
       0,     2,     2,     1,     1,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     5,     6,     3,
       4,     5,     6,     3,     3,     3,     3,     4,     4,     5,
       3,     4,     3,     4,     3,     3,     4,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     5,     6,     3,     3,     3,     3,     4,     4,     5,
       3,     4,     3,     4,     3,     3,     4,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     1,     5,     6,     3,     4,     5,     6,     3,     3,
       3,     4,     4,     5,     3,     4,     3,     4,     3,     3,
       4,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     1,     2,     1,     3,     1,
       1,     4,     2,     5,     1,     2,     2,     2,     2,     2,
       3,     5,     6,     2,     1,     4,     7,     4,     2,     6,
       6,     6,     6,     4,     4,     3,     4,     6,     8,     6,
       8,     4,     6,     4,     4,     4,     4,     4,     6,     8,
       2,     2,     4,     2,     7,     7,     3,     4,     4,     6,
       6,     4,     6,     6,     6,     4,     6,     4,     6,     8,
      10,    12,     6,     4,     6,     6,     1,     6,     4,     8,
      10,     4,     1,     3,     4,     6,     6,     8,     6,     6,
       4,     6,     6,     8,     4,     4,     6,     4,     2,     4,
       4,     4,     6,     4,     3,     8,     6,     4,     6,     6,
       8,     4,     8,     6,     8,     6,     8,     4,     4,     6,
       4,     7,     7,     7,     6,     6,     6,     6,     6,     4,
       4,     4,     4,     4,     4,    10,     3,     4,     3,     2,
       2,     2,     4,     6,     4,     4,     6,     6,     6,     4,
       6,     4,     6,     4,     4,     4,     4,     6,     4,     6,
       4,     6,     4,     4,     6,     4,     6,     4,     6,     4,
       4,     6,     4,     6,     0,     3,     3,     0,     1,     4,
       4,     4,     4,     5,     4,     0,     0,     7,    10,     4,
       4,     4,     4,     4,     0,     8,     0,     1,     0,     2,
       0,     1,     0,     3,     2,     3,     2,     1,     2,     1,
       2,     1,     1,     1,     0,     2,     1,     3,     1,     3,
       0,     2,     1,     3,     0,     1,     0,     2,     0,     2,
       3,     5,     1,     3,     3,     3,     5,     0,     8,     7,
       0,    10,     6,     7,     0,    10,     6,     4,     1,     2,
       2,     0,     4,    10,     5,     3,     2,     3,     0,     0,
       5,     0,     1,     0,     2,     2,     2,     0,     5,     0,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     0,     2,     0,     1,     0,     0,     3,     0,     0,
       3,     2,     0,     0,     4,     4,     2,     0,     2,     2,
       0,     1,     0,     4,     4,     2,     0,     1,     1,     0,
       1,     0,     1,     2,     1,     3,     3,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     6,     0,     1,     3,     1,     2,     0,     2,     3,
       1,     2,     0,     6,     3,     2,     0,     3,     6,     0,
       6,     4,     3,     0,     4,     1,     3,     1,     0,     2,
       0,     1,     0,     0,     8,     0,     0,     6,     0,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     3,     4,
       0,     3,     0,     3,     2,     3,     1,     2,     2,     0,
       3,     0,     5,     3,     1,     3,     1,     3,     1,     1,
       0,     1,     0,     4,     0,     1,     3,     1,     1,     1,
       0,     0,     6,     0,     0,    11,     3,     1,     3,     3,
       1,     3,     0,     1,     0,     4,     0,     6,     0,     5,
       0,     6,     1,     3,     3,     5,     0,     2,     0,     2,
       1,     1,     1,     3,     0,     1,     0,     3,     2,     3,
       4,     4,     0,     4,     6,    16,     2,     2,     0,     6,
       4,     2,     2,     2,     1,     5,     5,     2,     2,     2,
       2,     2,     3,     2,     2,     2,     1,     1,     3,     4,
       3,     2,     2,     1,     1,     1,     1,     0,     1,     0,
       2,     0,     2,     0,     1,     1,     1,     0,     2,     0,
       2,     0,     4,     0,     4,     1,     1,     0,     1,     0,
       1,     1,     0,     4,     3,     1,     0,     3,     4,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       0,     3,     3,     1,     1,     1,     2,     0,     3,     3,
       2,     2,     2,     2,     0,    15,     5,     4,     0,     1,
       0,     1,     1,     0,     1,     1,     0,     2,     2,     1,
       3,     4,     3,     3,     0,     2,     2,     1,     3,     3,
       0,     3,     1,     1,     2,     2,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     3,
       5,     1,     1,     3,     4,     5,     1,     5,     3,     2,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     4,     0,     1,     1,
       3,     2,     2,     1,     1,     1,     1,     0,     1,     0,
       1,     1,     1,     0,     2,     2,     2,     3,     4,     4,
       6,     2,     3,     3,     5,     1,     3,     3,     2,     2,
       2,     1,     1,     4,     4,     1,     1,     1,     1,     1,
       0,     4,     1,     1,     1,     3,     3,     1,     1,     2,
       2,     2,     4,     3,     0,     7,     1,     2,     1,     1,
       1,     1,     1,     1,     0,     5,     1,     1,     1,     1,
       1,     0,     3,     5,     7,     0,     9,     1,     2,     0,
       1,     1,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     3,     2,     2,     2,     0,
       1,     3,     1,     2,     2,     2,     1,     3,     3,     1,
       1,     3,     4,     5,     1,     0,     3,     3,     1,     1,
       0,     2,     2,     2,     2,     0,     2,     2,     1,     2,
       2,     2,     2,     0,     3,     0,     1,     1,     1,     4,
       2,     0,     1,     0,     4,     1,     1,     0,     0,     2,
       2,     1,     0,     1,     1,     3,     1,     3,     1,     3,
       1,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     2,     0,   398,   463,  1643,     0,  1647,   114,  1084,
    1006,  1020,     0,   463,     0,  1022,     0,     0,     0,   463,
    1197,   463,  1190,  1648,     0,   495,  1106,     0,     0,     0,
    1104,     0,  1073,     0,     0,     0,     0,  1165,  1166,  1575,
       0,     0,  1571,  1497,     0,     0,    50,  1025,     0,     0,
       0,    13,     0,    30,    18,    22,     8,    12,     4,    43,
      44,    36,     6,    10,    33,     5,     9,    27,    32,    28,
      29,    40,   488,    16,    17,    23,    34,    47,    14,    45,
      42,    15,  1167,    19,    35,    31,    24,    48,    25,    41,
      26,    46,    21,    37,    20,     7,    11,    38,    39,  1311,
    1356,  1369,  1409,  1424,  1460,  1288,  1291,  1295,  1297,  1299,
    1310,  1313,  1315,  1318,  1319,  1329,  1339,  1340,  1341,  1350,
    1358,  1368,  1374,  1377,  1412,  1437,  1439,  1440,  1443,  1444,
    1449,  1458,  1459,  1464,  1465,  1467,  1287,  1472,  1477,  1486,
    1286,  1290,  1293,  1294,  1296,  1298,  1300,  1301,  1302,  1304,
    1303,  1305,  1307,  1308,  1314,  1312,  1317,  1320,  1325,  1326,
    1344,  1328,  1331,  1332,  1345,  1327,  1338,  1342,  1347,  1348,
    1349,  1346,  1355,  1354,  1357,  1359,  1274,  1275,  1362,  1363,
    1366,  1364,  1370,  1371,  1373,  1375,  1378,  1382,  1380,  1381,
    1383,  1379,  1385,  1384,  1386,  1387,  1388,  1389,  1390,  1391,
    1434,  1435,  1376,  1392,  1393,  1394,  1395,  1398,  1405,  1406,
    1408,  1410,  1407,  1413,  1411,  1414,  1416,  1417,  1330,  1418,
    1419,  1425,  1426,  1427,  1429,  1432,  1433,  1430,  1431,  1436,
    1438,  1441,  1442,  1445,  1446,  1447,  1448,  1451,  1452,  1453,
    1455,  1457,  1466,  1468,  1471,  1473,  1474,  1476,  1480,  1481,
    1482,  1483,  1484,  1485,  1488,  1490,  1489,  1493,  1316,  1337,
    1491,  1292,  1306,  1321,  1322,  1334,  1343,  1351,  1423,  1324,
    1428,  1454,  1469,  1475,  1478,  1479,  1289,  1323,  1335,  1336,
    1352,  1353,  1360,  1361,  1367,  1372,  1396,  1397,  1400,  1399,
    1401,  1402,  1403,  1404,  1415,  1420,  1421,  1422,  1450,  1456,
    1470,  1487,  1492,  1494,  1333,  1461,  1463,  1462,  1365,  1309,
       0,     0,     0,  1278,  1273,  1279,   399,   368,     0,   465,
     464,     0,  1645,     0,   166,   346,   347,   118,   345,   119,
       0,   115,   116,     0,     0,  1098,   854,     0,  1021,     0,
    1018,  1013,     0,    55,  1172,  1282,  1276,  1281,  1280,    62,
    1028,     0,  1295,     0,     0,     0,     0,     0,     0,     0,
    1287,  1477,     0,     0,  1307,  1312,     0,   535,     0,     0,
    1245,  1256,     0,  1247,     0,     0,  1253,   860,  1233,     0,
    1244,  1252,  1255,  1447,  1277,  1246,     0,     0,     0,     0,
       0,     0,  1292,     0,     0,  1322,  1478,  1479,  1254,     0,
       0,     0,     0,   864,     0,     0,     0,   535,   535,     0,
       0,     0,  1323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1352,     0,     0,
    1353,  1360,     0,     0,     0,     0,     0,  1372,     0,     0,
       0,  1396,  1397,  1401,     0,     0,     0,  1402,  1403,  1404,
     535,  1415,  1420,     0,  1421,     0,  1422,     0,     0,     0,
       0,     0,  1450,  1470,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1487,   535,   535,   535,     0,
    1494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1238,  1202,   537,   722,   538,   716,   654,  1242,   650,
     649,  1243,   647,  1232,  1262,   664,   495,  1210,     0,     0,
    1543,  1542,  1540,     0,     0,     0,     0,     0,  1650,   489,
     493,   505,  1153,   419,   419,   419,   417,   419,  1105,     0,
    1551,  1082,   432,     0,   434,   450,     0,   466,   430,     0,
    1498,  1495,  1203,     0,  1082,   495,     0,     0,     1,     3,
       0,  1342,  1163,  1161,  1270,  1272,   938,  1553,  1554,     0,
     366,   369,     0,   448,  1646,  1644,    63,     0,     0,   119,
     117,     0,     0,  1102,  1101,  1100,     0,  1098,  1007,     0,
       0,  1012,     0,     0,     0,  1018,    57,     0,  1030,  1031,
    1029,   398,     0,   943,   943,     0,   943,   943,   943,   943,
     943,     0,     0,   668,     0,     0,     0,     0,     0,   693,
       0,     0,   663,   495,     0,     0,     0,   860,     0,   858,
       0,   658,     0,     0,     0,     0,     0,  1248,  1234,     0,
       0,   854,   854,     0,     0,  1249,     0,  1251,     0,  1250,
       0,   943,   943,   943,   865,     0,   854,     0,     0,   690,
       0,   691,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     854,   836,     0,     0,     0,     0,   646,     0,     0,     0,
     854,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     854,   854,   854,     0,   738,     0,     0,     0,     0,     0,
     854,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   854,   854,   854,   854,   854,   854,     0,     0,     0,
     779,   780,   781,     0,     0,     0,     0,   656,   655,   657,
    1671,     0,   659,     0,  1513,   652,     0,     0,   539,     0,
       0,   540,   541,   542,   543,   544,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,   926,   927,
     928,   929,   931,   930,   932,   933,     0,     0,  1235,     0,
    1666,     0,  1211,     0,  1212,  1208,     0,     0,     0,   461,
    1145,  1146,     0,  1198,   439,  1195,  1194,     0,  1191,  1193,
       0,  1651,   518,   511,   510,   509,   513,   517,   515,   516,
     512,   514,   526,   506,   508,     0,     0,     0,  1145,     0,
       0,   289,  1151,     0,  1151,  1154,  1510,  1137,   342,   343,
       0,   341,  1511,  1136,     0,     0,  1124,  1512,  1151,  1148,
    1149,     0,   969,   969,     0,     0,  1151,     0,  1107,     0,
       0,     0,     0,   413,   423,   415,   410,     0,   416,   412,
    1103,  1017,  1083,   398,     0,     0,   472,   482,     0,     0,
    1594,  1595,  1591,  1596,  1597,  1585,     0,     0,  1583,     0,
    1604,  1587,  1579,  1601,     0,  1593,  1600,  1589,  1598,  1599,
    1592,     0,  1577,  1581,     0,     0,  1579,  1572,     0,  1507,
       0,  1033,     0,  1032,   491,     0,  1657,    49,     0,  1169,
     940,   939,     0,  1552,     0,  1271,   303,   364,     0,     0,
       0,    87,     0,    89,   348,     0,  1085,  1088,  1092,  1096,
    1099,   856,   855,     0,  1019,  1011,   535,  1283,  1624,  1014,
    1620,     0,     0,    56,  1185,  1184,  1186,  1180,  1182,  1181,
       0,  1187,  1183,  1543,  1173,  1175,  1176,     0,     0,   944,
       0,   842,   825,     0,   842,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   536,     0,  1668,
       0,     0,     0,     0,     0,     0,   861,   862,     0,   675,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   868,     0,     0,     0,     0,   696,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   837,   834,
       0,   936,   934,   937,   935,     0,     0,     0,   723,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   611,   744,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   818,     0,
       0,     0,     0,     0,     0,   776,     0,   778,     0,     0,
       0,   983,   980,   979,   982,   981,     0,   660,   854,     0,
       0,     0,     0,     0,     0,     0,   573,   564,     0,     0,
     562,   574,   567,   568,   556,   854,   549,   495,   952,     0,
       0,     0,     0,   554,   553,   560,   555,     0,   645,   575,
     577,   570,   722,   569,   722,   571,   572,   578,   576,  1291,
     545,   565,     0,     0,   648,  1263,  1672,  1665,  1207,  1209,
       0,   477,     0,   938,  1541,  1544,     0,     0,     0,  1196,
       0,  1649,  1662,   494,  1652,   524,   496,   523,     0,   507,
    1157,     0,     0,   119,  1141,  1117,  1142,  1135,     0,  1134,
    1121,  1108,     0,  1130,     0,  1149,  1129,  1155,  1156,  1151,
       0,  1149,  1122,     0,  1128,   970,  1127,   288,  1112,   152,
    1133,     0,  1116,  1123,     0,  1131,  1151,   426,   425,   424,
     420,   421,   426,     0,   891,     0,  1015,     0,     0,   474,
     484,   436,   452,     0,  1625,  1606,  1608,  1607,  1625,  1603,
    1625,  1580,  1578,  1602,  1625,     0,     0,   467,   468,     0,
       0,  1578,     0,  1504,  1505,  1508,  1506,  1496,  1499,     0,
    1503,     0,    52,    53,     0,  1036,  1026,  1035,   492,  1655,
     490,  1656,     0,  1164,  1237,  1162,  1170,  1236,  1171,   942,
    1409,  1348,     0,   945,  1556,   304,   367,   303,   110,   113,
     112,     0,   303,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      65,    78,   120,   303,    83,     0,     0,     0,     0,  1090,
    1086,  1096,     0,     0,     0,   938,     0,  1009,  1285,     0,
       0,     0,   400,     0,    58,    60,  1179,     0,     0,  1188,
    1023,     0,   819,     0,   854,   824,     0,     0,   830,   829,
     831,   832,   833,     0,     0,     0,   684,   686,     0,     0,
     673,  1667,   854,   854,   730,     0,   859,     0,     0,   854,
     676,     0,     0,   674,   667,   683,   685,   697,   758,     0,
     757,   681,     0,   820,   821,   822,   866,     0,   687,   854,
       0,   692,     0,     0,   698,     0,   707,     0,   705,     0,
     854,     0,   701,     0,     0,     0,   854,     0,   713,     0,
     810,     0,   789,     0,   791,     0,   793,   854,     0,     0,
     718,     0,   724,     0,   812,     0,   794,     0,   854,     0,
     734,   735,   737,   796,     0,   798,     0,   800,     0,   795,
     802,   803,   739,   741,   740,   805,     0,     0,   807,     0,
     809,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   743,     0,     0,   747,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   760,   769,   770,   771,   772,   773,   774,
       0,   777,   782,     0,   784,   785,     0,     0,     0,   665,
    1514,  1515,  1516,   360,   651,  1264,   557,   563,     0,  1670,
       0,     0,   558,   854,   550,   952,   561,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   580,   579,   566,   721,     0,     0,     0,
    1206,     0,     0,   452,     0,     0,  1199,   441,  1192,  1664,
    1663,  1653,   526,   960,   527,   529,   528,  1262,     0,  1159,
       0,  1140,     0,  1152,  1138,     0,  1151,  1109,  1150,  1151,
     973,   974,     0,  1149,     0,  1132,     0,   414,   423,   411,
     418,   891,     0,     0,   872,     0,     0,     0,   476,   471,
     475,   473,   338,   337,   340,     0,   483,   438,   437,   435,
     459,   458,   460,   457,   456,   451,   453,   454,  1605,     0,
    1586,  1584,  1588,  1590,  1616,  1619,  1270,     0,  1582,     0,
       0,     0,     0,     0,  1507,     0,  1476,  1525,  1501,     0,
       0,     0,     0,     0,  1502,    54,  1034,     0,     0,   969,
    1661,  1659,  1563,  1561,  1562,  1566,  1567,  1569,  1568,  1570,
    1560,  1557,  1564,   946,   971,   111,  1060,  1060,   396,   396,
     396,  1060,   396,  1060,  1060,  1060,     0,     0,  1060,     0,
       0,     0,     0,     0,  1060,  1060,  1060,  1060,  1060,  1060,
     406,  1060,  1060,  1060,  1060,  1060,   396,  1060,   394,   303,
     145,   146,   365,     0,   373,   395,   449,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,   121,     0,   351,   353,   352,   350,
     349,     0,     0,   945,     0,  1093,   891,  1097,  1096,  1094,
     857,  1010,  1284,     0,  1621,   402,   401,  1008,    61,     0,
       0,  1174,  1189,  1177,     0,     0,   843,   826,   823,     0,
       0,     0,     0,   266,   266,   266,   853,   851,   847,   852,
     849,     0,   291,     0,   290,     0,     0,     0,     0,   814,
     863,     0,     0,     0,     0,     0,     0,     0,     0,   869,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   840,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   603,   594,     0,     0,   592,   608,   597,
     598,   586,     0,   952,     0,     0,     0,   584,   583,   590,
     585,     0,   604,   606,   600,   722,   599,   722,   601,   602,
     607,   605,   595,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   661,     0,   653,   547,  1669,   951,     0,
     559,     0,   637,   628,     0,     0,   626,   642,   631,   632,
     551,   854,   614,   952,     0,     0,     0,     0,   619,   618,
     624,   620,     0,   638,   640,   634,   722,   633,   722,   635,
     636,   641,   639,   629,     0,  1265,  1204,   478,   479,   482,
       0,  1547,  1548,  1546,  1545,   462,  1200,  1201,   446,   447,
     445,   440,   442,   443,     0,   522,   891,   997,   519,   500,
     971,   961,   499,   530,     0,  1158,     0,  1118,     0,  1139,
       0,  1111,  1110,     0,     0,  1144,  1143,  1113,  1120,  1149,
     428,   427,   422,   898,   891,     0,  1658,   891,     0,     0,
     888,     0,     0,   891,   901,   901,   891,   891,   938,   433,
    1016,     0,   455,  1629,     0,  1628,     0,     0,     0,   469,
     470,   431,     0,     0,  1500,     0,     0,     0,  1058,  1059,
       0,   293,   292,   299,     0,     0,  1513,     0,   295,   297,
     296,  1521,   294,   103,  1040,     0,     0,     0,  1049,  1027,
    1037,   962,  1660,     0,     0,  1555,   972,  1061,     0,     0,
     397,   358,   339,   339,   377,     0,   181,   358,   332,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1060,     0,   388,   371,   389,   372,  1060,     0,     0,
       0,     0,     0,     0,   408,   409,   407,     0,     0,     0,
       0,     0,     0,     0,     0,   124,   303,   184,     0,   403,
     181,   187,  1266,    67,    68,    79,   988,   986,   984,   987,
     985,    80,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    81,    82,    66,     0,   184,    84,  1213,   122,   125,
     169,   168,   167,     0,    85,   891,   960,  1096,   945,     0,
     938,  1622,     0,    59,  1178,  1070,   736,     0,     0,   646,
     679,   768,     0,   844,   846,   305,   848,   850,   669,   672,
     671,   728,   729,   731,     0,     0,     0,   662,   677,     0,
     759,   682,   867,   670,     0,     0,   688,     0,     0,     0,
     703,   708,   706,   699,   704,   702,     0,   788,   715,   712,
     714,   811,   790,   792,   838,   841,     0,   717,     0,   725,
     813,   726,     0,   700,   732,     0,   797,   799,   801,   806,
     804,   808,   587,   593,   742,   588,   952,   591,     0,     0,
     610,   609,   596,     0,   746,   748,   646,   749,     0,   755,
     753,     0,     0,   766,     0,   764,     0,   765,     0,   767,
       0,   783,   786,   787,   361,   548,   552,   621,   627,     0,
     622,   854,   615,   952,   625,     0,     0,   644,   643,   630,
    1213,     0,   486,  1549,  1550,   444,  1654,   971,   945,     0,
       0,  1005,     0,     0,     0,   497,   502,   498,   501,     0,
     525,   534,   533,  1259,  1263,  1160,   969,  1126,  1125,     0,
     976,   975,  1151,     0,   896,   505,  1658,   895,     0,     0,
     890,   889,   891,   901,   901,  1074,  1077,     0,   875,   902,
       0,     0,   873,   874,   903,   913,     0,   912,  1626,     0,
    1618,  1617,  1630,     0,  1573,  1527,     0,  1526,  1538,  1539,
    1536,  1537,  1535,  1517,     0,  1522,     0,  1532,     0,     0,
    1523,     0,     0,   505,     0,  1062,  1048,  1054,  1047,     0,
       0,     0,  1046,  1258,  1257,  1651,     0,     0,   947,   302,
     301,   151,   298,   150,   186,   359,   333,   334,     0,     0,
       0,   339,   180,   358,   179,   182,   348,     0,   379,   134,
     358,   386,   387,   400,   135,   132,   138,     0,     0,   139,
       0,   130,   131,   137,   136,   158,   157,   141,   159,   142,
     143,   392,   153,   155,   154,   156,   140,   129,     0,   128,
     162,   161,   160,   147,   381,   133,   374,     0,   170,   172,
     173,  1269,     0,   405,   376,     0,   174,   175,     0,     0,
      93,     0,    91,     0,     0,  1215,  1214,    92,   106,   303,
     126,     0,     0,   945,   977,  1089,  1095,  1623,     0,  1024,
     827,     0,     0,     0,   307,   308,   306,     0,   845,     0,
       0,   666,     0,   870,     0,     0,   694,   695,     0,     0,
       0,     0,     0,     0,     0,   589,     0,   581,     0,     0,
       0,     0,     0,   763,   761,   762,     0,   612,     0,   623,
       0,   616,     0,   480,     0,   481,   504,   953,  1002,  1004,
       0,   998,  1000,     0,   520,   532,   531,     0,  1119,     0,
    1114,   429,   526,   897,   938,     0,   887,     0,     0,     0,
     945,     0,   891,   891,     0,   877,     0,     0,     0,   892,
     485,     0,  1627,     0,  1635,     0,     0,     0,  1531,  1518,
     300,     0,     0,     0,     0,  1519,   526,  1041,  1056,     0,
    1064,     0,     0,  1038,  1051,     0,     0,  1050,  1261,   963,
     966,  1069,  1068,     0,  1067,   183,   358,   335,   336,   348,
       0,     0,     0,     0,   385,   384,   299,   148,   149,     0,
       0,   378,   184,   404,   312,   200,   266,   193,   194,   255,
     305,     0,   238,   243,   225,   228,   251,   266,   237,   208,
     205,   255,   253,   223,   255,   229,   247,   213,   305,   212,
     250,   305,   255,   252,   249,   266,   266,   206,   209,   248,
     305,     0,   241,   230,   233,   234,   232,   236,   231,   235,
     266,   270,   211,   305,   198,     0,     0,   266,   268,  1268,
       0,    96,   121,   107,     0,   127,    90,     0,  1091,     0,
    1087,  1071,     0,   680,   267,   309,   816,   815,   678,     0,
     689,   709,     0,   839,   835,   943,   719,   727,   733,   582,
     745,   750,   754,   752,   756,   854,   613,   617,     0,   487,
       0,   948,  1216,  1001,     0,     0,  1260,     0,   899,   894,
       0,   891,   891,  1076,   960,  1078,   880,   884,   876,     0,
     907,   905,   906,   904,   911,  1634,  1632,     0,  1633,     0,
       0,  1631,  1609,     0,  1576,  1574,  1529,  1528,  1530,  1524,
       0,     0,     0,   104,     0,     0,     0,  1065,  1053,  1657,
    1039,  1045,     0,   967,   968,   965,  1565,     0,     0,     0,
       0,   391,     0,   403,   393,   144,   270,   171,   318,     0,
     192,     0,   259,   257,   237,   214,     0,   305,   227,   239,
     242,   244,   246,     0,   210,   259,   254,     0,   259,   219,
     218,   259,   305,   207,   216,     0,   259,     0,   278,   284,
       0,     0,     0,   274,     0,   311,     0,   281,   188,   271,
     273,     0,   240,     0,   196,     0,     0,     0,   259,     0,
     269,   259,     0,    94,  1658,  1213,     0,    99,     0,   966,
     356,   978,     0,     0,   871,     0,     0,     0,     0,     0,
     949,   960,     0,  1224,   999,   521,     0,   971,   891,   882,
     886,   977,     0,     0,     0,     0,     0,     0,  1613,  1614,
    1615,  1610,     0,     0,     0,     0,     0,  1636,  1638,  1534,
    1533,  1520,   971,  1055,  1057,  1063,  1052,   966,  1066,     0,
       0,     0,     0,     0,     0,     0,     0,   390,  1239,   380,
     382,     0,     0,     0,   313,   314,   319,   321,     0,     0,
     263,   264,   265,   222,   260,   262,   215,   245,     0,   220,
       0,   191,   221,   217,     0,   204,   283,   285,   535,   276,
     287,   275,     0,     0,   282,   272,   280,     0,     0,     0,
       0,   189,     0,   190,  1267,    97,    95,     0,  1651,    86,
       0,   355,     0,     0,   720,   710,     0,   828,   775,  1216,
     957,   966,     0,   971,     0,     0,     0,     0,  1217,  1219,
       0,  1003,     0,   900,   519,   163,     0,  1075,   879,     0,
     883,     0,     0,   914,   909,  1611,  1639,  1642,  1640,  1641,
    1637,   105,   964,     0,     0,   176,  1241,  1240,   403,   324,
     325,   326,     0,     0,     0,   317,   320,   199,   256,     0,
     261,     0,   362,     0,   203,   286,   535,   279,   305,   197,
     305,   202,   101,   100,   966,     0,     0,     0,  1224,     0,
       0,   954,   956,   950,   989,     0,     0,     0,     0,  1218,
       0,     0,  1225,  1227,     0,   164,     0,     0,     0,   878,
       0,     0,   910,     0,   177,   383,   328,     0,   327,     0,
     322,   323,   318,     0,     0,   305,     0,   305,   277,   195,
     201,  1658,   354,   357,  1072,  1080,     0,   711,  1230,   958,
     959,   966,     0,   503,  1222,  1223,     0,  1220,     0,     0,
    1226,     0,     0,     0,     0,   915,   908,   178,   330,   331,
     329,   315,   316,   258,   226,   363,   224,   102,     0,     0,
       0,  1042,   955,   990,  1221,  1229,  1228,     0,   893,   881,
     885,  1079,  1081,     0,     0,  1205,     0,     0,  1231,  1044,
       0,   526,     0,  1043,     0,     0,   993,   995,     0,   996,
     991,   526,  1115,   994
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    49,    50,    51,    52,    53,  1262,    54,   586,   953,
    1334,  1335,    55,    56,   929,  1309,  1310,  1311,    57,  1734,
    2432,  1313,  1315,  2116,  2422,  2794,  2427,  2908,  3031,  2028,
    2333,  2842,  2634,  1286,  1287,  1288,   330,   331,   332,   568,
    2117,  1708,  2118,  2119,  1710,  1711,  1208,  2396,  2387,  2403,
    2933,   333,  2123,  2407,  2408,  2409,  2410,  2364,  2365,  2045,
    2046,  2090,  2418,  2621,  2757,  2753,  2622,  2623,  2624,  2625,
    2626,  2627,  2628,  2742,  2743,  2873,  2874,  2875,  2143,  2791,
    2778,  2779,  2780,  2889,  2447,  2012,  2013,  2020,  2021,  2350,
    2325,  2351,  1291,  2448,  2781,  2417,  2738,  2864,  2964,  2865,
    2866,  2867,  3020,  2047,  2363,  1614,  1615,   857,   334,  1318,
    1739,  2798,  2799,  2354,  1885,  2971,    58,  1292,   926,   560,
    1712,  1713,  1714,  2563,  2570,  2958,  2049,   318,  1757,  2414,
    2077,    59,  1222,  1217,    60,   868,   863,   864,  1220,  1221,
    1597,  1971,    61,   905,    62,   874,    63,   878,  1619,    64,
    1168,  1941,  1942,  1943,    65,   928,    66,   879,  1625,  1626,
    1627,    67,  1166,   321,    68,   904,  1247,  1248,    69,   875,
     876,  1609,    70,  1559,  1927,  1928,  1229,  1230,  1616,  2485,
      71,    72,   547,   519,   811,   520,   521,  1573,  1948,  2934,
     822,   823,   824,  2265,  1176,  1177,  1178,  1953,  1574,  2270,
     609,  2552,   767,  1152,   493,  1072,  1137,   494,   495,   496,
    2156,  1087,   497,  1344,  2137,  1417,  1039,  2460,  2184,   970,
    1343,  1781,  1088,   579,   942,   618,   619,   620,   996,   645,
    1797,  1386,  1387,  1799,  1975,  2689,  2823,  2825,  1987,  1604,
    1605,  1976,  2284,  2285,  2817,  2300,  2519,  2691,  2827,  3011,
    2306,  2942,   786,  1045,   922,   923,   971,  1674,  2034,  2811,
    2922,  1522,  2671,  2920,  2991,  1950,  1951,  2346,  2549,  2725,
    1204,  2935,  2036,  1590,  2640,  1106,  2101,  3043,  3086,  3095,
    3096,  3097,  2261,  2262,  2491,  2492,  1952,  2672,    73,   336,
      74,  1751,   584,  1225,  1226,   583,   342,    75,   350,  1764,
      76,   544,  1657,   591,   912,  1266,  1267,  2029,  2334,  3085,
    2341,  2030,  2345,  2719,  2336,  2537,  2538,  2037,  2038,  2337,
    2540,  2716,  2553,  2554,  2439,  2802,    77,   531,  2510,  2295,
    2296,  3034,  3035,   873,    78,   335,   936,  1743,  1323,  1742,
     937,   938,  1325,   576,   577,    79,   529,    80,   522,   858,
    1592,  2276,  1967,   802,   860,  1199,  1189,   861,  1200,  1579,
    1957,    81,   919,   918,    82,   552,  1275,    83,   587,   964,
     965,  1339,  1763,    84,   516,   808,   809,    85,   514,   803,
    1566,    86,    87,    88,  2250,  1160,   795,  2428,  2813,  2928,
    2929,  2931,  3002,  3003,  3071,   498,  2972,   499,  2857,   500,
     501,  2342,  2343,  2550,   502,  2091,   871,   312,   313,   347,
     503,   504,   947,   948,   315,    89,   909,   541,  1257,  1258,
    1259,  1260,   862,  1113,  1648,  1654,  1649,  2529,  2330,  2323,
      90,   798,   966,  1164,  1165,  1933,    91,    92,   924,  1283,
    1284,  1671,  2033,  1672,    93,   539,   907,    94,   536,   901,
    1242,   902,   903,  1238,  1234,  1240,  1244,  2832,  2701,  2702,
    1637,   949,   950,  1630,  1994,  1995,  2524,  2704,  2837,  2838,
      95,   322,   565,    96,    97,    98,  1173,  1269,  1944,  2288,
    1271,  1272,  1661,  1571,   505,   791,   612,   990,  1126,  1520,
     506,  1157
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -2622
static const int yypact[] =
{
    2156, -2622, 15038,    26,    37, -2622,   237, -2622,   523, -2622,
   -2622,   -14, 37776,    37, 33717, -2622, 10288,   594,   493,    37,
   -2622,    37, -2622,    21, 37776, -2622, -2622,   638,   128,   312,
     157,   493, -2622,   493,   350,   493,   493, -2622, -2622, -2622,
     493,   493, -2622,   315, 37776, 37776, -2622, -2622,    22,   565,
     591, -2622,   283, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, 15489, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   37776,   423,   148, -2622,   -48, -2622, -2622, 37776,   486, -2622,
   -2622,   493,   313,   517, -2622, -2622, -2622, -2622, -2622,   415,
     527, -2622,   533,   566,   721,   404, -2622, 37776, -2622,   774,
     471, -2622,   493, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
     827,   422,   429,   458,   482,   494,   497,   500,   526,   529,
     544,   568, 10288,   573,   577,   589,   599,   602,   615,   622,
   -2622, -2622,   626, -2622,   647,   649, -2622,   651, -2622, 10288,
   -2622, -2622, -2622,   664, -2622, -2622,   672,   686,   689,   691,
     282,   693,   708,   715,   718,   -40,   -13,    -7, -2622,   725,
     738,   741,   750, 10288,   756,   766,   768,   602,   772,   780,
     784,   801,   809,   811,   815,   816,   818,   822,   823,   828,
     829,   830,   831,   832,   838,   840,   842,   844,   849,   852,
     857,   859,   860, 10288,   861,   863,   865,   866,   868,   869,
     871,   872,   874,   876,   877,   878,   879,   885,   886,   887,
     888,   892,   894,   895,   896,   898,   901,   902,   903,   904,
     905,   906,   907,   909,   915,   916,   917,   920,   921,   926,
     927,   928,   930,   931,   932,   935,   602,   602,   602,   936,
     937,   939,   941, 10288, 10288, 10288,  5869, 10288, 37776, 16842,
   37776, -2622, 30604, -2622,  1397,   826, -2622, -2622,   607, -2622,
   -2622, -2622, -2622, -2622,   510, -2622, -2622,   762,   958, 15038,
   -2622, -2622, -2622,   493,   554,   493,   132,  1059, -2622, -2622,
   -2622,   263, 38737, -2622, -2622, -2622,   702, -2622, -2622, 15038,
   -2622,   997, -2622, 15038, -2622, -2622,  2799, -2622, -2622,  3616,
   -2622, -2622, -2622,  1063,   156, -2622,    22,   759, -2622, -2622,
   37776,    29, -2622, -2622,   722, -2622,   760, -2622, -2622, 37776,
   -2622, -2622, 15038, -2622, -2622, -2622, -2622,  1002, 37776,   415,
   -2622,   774, 37776, -2622, -2622, -2622, 35070,   404, -2622, 10288,
    1039, -2622,  1128, 37776, 32364,   471,   973,   922, -2622, -2622,
   -2622,  1119, 10288,  1195,   175, 10288,  1195,  1195,  1195,  1195,
    1195, 10288, 10288,   201, 10288, 10288, 10288, 10288,   942, -2622,
   15940,  1293, -2622, -2622, 10288, 10288, 10288, -2622,  1040, -2622,
   15940,   201, 10288,   943, 10288, 10288, 10288, -2622, -2622, 15940,
   10288, -2622, -2622,  1129, 10288,   607, 10288,   607, 10288,   607,
   10288,  1195,  1195,  1195, 30604,   960, -2622, 10288, 10288, -2622,
    6360, -2622,   944, 10288, 10288, 10288, 10288, 10288, 10288, 10288,
   10288, 10288, 10288,  1397, 10288, 10288, 10288, 10288, 10288, 10288,
   -2622,  1305,    48,   728, 10288, 10288, 30604,  6851, 10288, 10288,
   -2622, 10288, 10288, 10288, 10288, 10288, 10288, 10288, 10288, 10288,
   -2622, -2622, -2622,  6360, -2622, 10288, 10288, 10288, 10288, 10288,
   -2622, 10288,  7342, 10288, 10288, 10288, 10288, 10288, 10288, 10288,
    4887,   946,   946,   946,   946,   946,   946,    48,  7833,   947,
   -2622, -2622, -2622, 10288, 10288, 10288,   454,   201,   201,   201,
   -2622,  1954,   201, 10288,   408,  1413,   945, 10288, -2622, 10288,
    1259, -2622, -2622, -2622, -2622, -2622,   695, 10288, 10288, 10288,
   10288,   951, 10288,   503, 10288, 10288, 10288, 10288, 10288, 10288,
   10288, 10288, 10288, 10288, 10288, 10288, 10288,  8324, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622,   962, 33717, -2622, 37776,
   -2622,   955, -2622,  1387, -2622,  1265,  1343,  1302, 15038, -2622,
   -2622, -2622,  1267, -2622, -2622, -2622, -2622,  1347,   963, -2622,
   37776,  1154, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622,   972,   263, -2622,  1408,   968,   -63,  1186,   416,
    1276, -2622,  1281,  1175,  1281, -2622, -2622,  1072, -2622, -2622,
    1192, -2622, -2622, -2622,  1407,  1190, -2622, -2622,  1281, -2622,
     470,   431,  1137,  1137,  1207, 33717,  1281,   470, -2622,  1287,
    1086,   107,  1166, -2622,   539, -2622, -2622,  1435, -2622, -2622,
   -2622, -2622, -2622,  1119, 15038,   -17, -2622, -2622, 15038, 15038,
   -2622,  1196, -2622, -2622, -2622, -2622,  1198,   252, -2622,  1357,
   -2622, -2622,  1242, -2622,  1254, -2622, -2622, -2622, -2622, -2622,
   -2622,  1261,   992, -2622, 15038, 15038,  1242, -2622,  1262,    45,
    -123, -2622, 13174, -2622, -2622,  1001,   407, -2622,    32, 34168,
   -2622, -2622, 37776, -2622, 38227, -2622,   224, -2622, 15038,  1870,
    1349, -2622, 15038, -2622,   475, 15038, -2622,  1005, -2622,   996,
   -2622, 30604,  1006, 15038, -2622, -2622,   602,   998,  1093,  1008,
   -2622, 15038,  1007, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
    1395, -2622, -2622,  1194,  1010, -2622, -2622, 13174, 18953, -2622,
    1014, -2622, -2622,  1015,  1023, 18994,  1017,  1018,  1019,  1021,
    1025, 19234, 19350, 30111, 22325, 22348,  2732, -2622,  1026, -2622,
     955, 19476, 19569,  2766,  1027,  1024,  1028, -2622, 19592, -2622,
   22412, 19620, 19839,  1030, 22594,  1031,  1036, 22833,  3147, 22852,
    3500,  1037,  1042,  1043, -2622,  1044, 19900, 19927, 22866, -2622,
   19956, 20146, 22945, 20175,  3513,  3526, 20263, 20280,  3545, 20502,
    1371, 20548, 20588,  3558, 17641, 17777, 17815,  1046, -2622, -2622,
     -64, -2622, -2622, -2622, -2622,  1038, 22959, 20684, -2622, 22978,
   20770, 17828,  1047, 20828, 20866, 20954, 23217, 23347, 23361, 17862,
   17880, 17914,  1048,  1050,  1053, 23391, 23491, 23535, 18187, 21136,
   18231,  1054, 30650,   826, -2622, 23635, 21193, 21272, 18266, 23669,
   21310, 18366, 21375,  8815,  9306,  9797, 22267,  1055, -2622,  1058,
    1060,  1062,  1064,  1067,   -60, -2622, 23776, -2622, 18313, 23893,
   18411, -2622, -2622, -2622, -2622, -2622,  1068, -2622, -2622,   640,
    1034,  1066,  1069, 33717, 10288, 37776,    63,   213, 10288,  1301,
   -2622,    63,   241,   241, 19519,  1293, -2622, -2622,   180,  1078,
   10288, 10288, 10288, 27605, 27605,   213, 27605, 30696,   826,   151,
     226,    97,  1397,    97,  1397,    63,    63,    63,   201,  1079,
   -2622,   213,   622, 10288, -2622,  1073, -2622, -2622, -2622, -2622,
    1383, -2622,  1496,   167,  1080, -2622, 15038,   -23, 15038, -2622,
     132, -2622,   513, -2622, -2622, -2622,  1083, -2622, 10288, -2622,
    1405,  1094, 15038,   415, -2622, -2622, -2622, -2622,  1294, -2622,
   -2622, -2622, 32364, -2622,  1187,   470, -2622, -2622, -2622,  1281,
   37776,   470, -2622,   454, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, 15038, -2622, -2622,   470, -2622,  1281,  1497, -2622, -2622,
    1090, -2622,  1497,  1510,  -216,   -93, -2622, 35521, 15038, -2622,
     -16,    40,    50,  1310,  1100, -2622, -2622, -2622,  1100, -2622,
    1100, -2622, -2622, -2622,  1100, 13625,  4028,  1099, -2622,  1308,
     -34,  1101, 13625, -2622, -2622, -2622, -2622,  1102, -2622, 35972,
       9, 33717, -2622, -2622, 15038, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622,   -19, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
     120,   133,  1326,  1286, -2622, -2622, -2622,   261, -2622, -2622,
   -2622,    86,  1279,    50,  1583,  1584,  1585,  1586,  1587,  1588,
    1589,  1591,  1592,  1594,  1596,  1597,  1598,  1599,  1601,  1124,
   -2622, -2622, -2622,   224, -2622,  1323,   105,   105,  1393,  1005,
   -2622,    76, 37776,  1471, 28216,    85, 10288, -2622, -2622, 33717,
    1516, 32364,     5, 33717,  1130, -2622, -2622,  1375,   922, 15038,
   -2622, 10288, -2622, 10288, -2622, -2622,  1133, 10288, -2622, -2622,
   -2622, -2622, -2622, 10779, 10288,   655, -2622, -2622, 32815,   655,
   -2622, -2622, -2622, -2622, -2622, 10288, -2622, 10288, 15940, -2622,
   -2622, 10288, 10288, -2622, -2622, -2622, -2622, -2622, -2622, 10288,
   -2622, -2622, 10288, -2622, -2622, -2622,  1243, 10288, -2622, -2622,
   10288, -2622,  1216,  1216, -2622,   607, -2622, 10288, -2622, 10288,
   -2622,   607, -2622, 10288, 10288, 10288, -2622,  1404, -2622, 10288,
   -2622, 10288, -2622, 10288, -2622, 10288, -2622, -2622,  1406, 10288,
   -2622, 10288, -2622, 10288, -2622, 10288, -2622, 10288, -2622, 10288,
   -2622, -2622, -2622, -2622, 10288, -2622, 10288, -2622, 10288, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, 10288, 10288, -2622, 10288,
   -2622, 10288, 10288,  1447,   746, 10288, 10288, 10288, 10288, 10288,
   10288,   436, 10288, 10288, 10288, 10288, 10288, 10288, 10288, 10288,
   10288, 10288, 10288, 10288, 10288,  8324, -2622, 10288, 10288, -2622,
   10288, -2622, 11270, 10288, 10288, 10288, 10288, 30153, 10288, 30196,
   10288, 30218, 10288, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
    1410, -2622, -2622, 10288, -2622, -2622, 10288, 10288,  1139, -2622,
   -2622, -2622, -2622,  1138, 30604, -2622,   213, -2622,  1143, -2622,
     955, 10288, -2622,  1293, -2622,   180,   213, 30784, 10288, 10288,
    1455,   875, 10288, 10288, 10288, 10288,  1147, 10288,   713, 10288,
   10288, 10288, 10288, 10288, 10288, 10288, 10288, 10288, 10288, 10288,
   10288, 10288,  8324, -2622, -2622, -2622,    97, 37776,  1294, 15038,
   -2622,    46, 15038,    50,  1294, 10288, -2622,   -11, -2622, -2622,
   -2622, -2622, -2622,   449, -2622, 30604, -2622,  1141,  1294,  1491,
    1149, -2622, 37776, -2622, -2622,  1571,  1281, -2622, -2622,  1281,
   -2622,   -69,    24,   470, 15038, -2622,   910, -2622,   539, -2622,
   -2622,    19, 37776,   238, -2622, 15038,  1294, 15038, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622,  1157, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622,   400, -2622, 37776,
   -2622, -2622, -2622, -2622,  1156, -2622,  1159,  1367, -2622, 15038,
   15038,  1294,  1505,  1512,    45,  1164,  1492,  1172, -2622,   652,
   31462,   124, 17293, 31913, -2622, -2622, -2622,    36,  1559,  1137,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622,  1137, -2622,   652,   652,  1022,  1554,
    1554,   652,   645,   652,   652,   652,  1389,  1538,   652,  1502,
    1399,  1506,  1401,  1546,   652,   652,   652,   652,   652,   652,
     187,   652,   652,   652,   652,   652,  1554,   652, -2622,   130,
   -2622, -2622,  1197, 14096, -2622, -2622, -2622,  1294,  1294,  1294,
     795,  1294,   454,   454,   454,  1294,  1294,  1294,  1294,  1294,
    1294,   454,  1870, -2622,  1549,   522, -2622, -2622, -2622, -2622,
   -2622, 15038,  1379,  1286, 28216, -2622,  -216, -2622,  1191, -2622,
   30604, -2622, -2622,   240, -2622, -2622, -2622, -2622, -2622,  1007,
    1632, -2622,  1201, -2622,    36, 23910, 30604, -2622, -2622, 21549,
   10288, 24019, 24260,  1204,  1204,  1204, -2622, -2622,  1364, -2622,
    1365,  1209, -2622,  1211, -2622,  1212,  1213,  1214, 24277, 30252,
   -2622,  1217, 24292, 21634, 24307, 24394, 10288,  1229, 30297,  1246,
    1219, 21725,  1397,  1397,  1221, 24642, 24664,  1222,  1225, 24678,
   21817, 24922,  1226,  1227, 24950, 25014, 25029, 25048,  1493,  1231,
   25066, 21902, 25336, 25401, 18590,  1233, 18635, 25431, 25445, 25463,
   25575, 25816, 25833,    63,   213, 10288,  1504, -2622,    63,   241,
     241, 19519, 25848,   180, 10288, 10288, 10288, 27605, 27605,   213,
   27605, 30826,   151,   226,    97,  1397,    97,  1397,    63,    63,
      63,   201,   213,   622, 21915, 25943, 25960, 10288, 26184, 22312,
   18809, 21994, 26201, 10288, 26218, 10288, 26328, 10288, 26442,  1236,
   26552, 26569, 26586, -2622, 37776, -2622, -2622, -2622,   826,  1238,
   -2622, 10288,    63,   213, 10288,  1509, -2622,    63,   241,   241,
   19519,  1293, -2622,   180,  1248, 10288, 10288, 10288, 27605, 27605,
     213, 27605, 30890,   151,   226,    97,  1397,    97,  1397,    63,
      63,    63,   201,   213,   622, -2622, -2622,  1240, -2622, -2622,
    1422,  1557, -2622, -2622, -2622, -2622, -2622, 30604, -2622, -2622,
   -2622, -2622, -2622,   299,    32, -2622,    51,   743,    69,  1572,
    1137, -2622,  1593, 34619, 28687, -2622,   795, -2622,   731, -2622,
    1434, -2622, -2622,   454,   454, -2622, -2622, -2622, -2622,   470,
   -2622,  1253, -2622, -2622,    19,   335,   -28,  -216,  1575,  1576,
   -2622,   117, 15940,  -216,  1517,  1517,  -216,  -216,  1522, -2622,
   -2622, 36423, -2622, -2622,   580, -2622,  1268, 29158, 32364, -2622,
   -2622, -2622,  1523, 32364, -2622, 37776,  1574, 37776, -2622, -2622,
    5378, -2622, -2622,  1643, 32364,   -80,   408,   652, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622,  1271,  1271, 12703, -2622, -2622,
   -2622, -2622, -2622,  1272, 10288, -2622, -2622, -2622, 33266, 31462,
   -2622, 16391,   712,   712, -2622,   782,   802, 16391, -2622, -2622,
   16391, 16391,  1294,  1582,  1590, 16391, 16391,   795,   454,   454,
      91,   652,   454, -2622, -2622, -2622, -2622,   652,   795,   795,
     788,   574,   454,   454, -2622, -2622, -2622, 15038,   776, 33717,
    1277, 33717,   524, 16391,  1294, -2622,  2897, 12252, 37776,   558,
     104, -2622,  1269, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, 15038,  4304, -2622,    93, -2622,   971,
   -2622, -2622, -2622,  1463, -2622,  -216,  1493,    83,   732,  1285,
      85, -2622,  1487, -2622, -2622,  1553, -2622,  1288, 10288, 29977,
   -2622, -2622,  1558, -2622, -2622,   254, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622,     6,  1540,  1289, -2622, -2622, 10288,
   -2622, -2622, 30604, -2622, 10288, 10288, -2622, 10288,  1292,  1296,
   -2622, -2622, -2622, -2622, -2622, -2622, 10288, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622,  1519, -2622,  1566, -2622, 10288, -2622,
   -2622, -2622, 10288, -2622, -2622, 10288, -2622, -2622, -2622, -2622,
   -2622, -2622,   213, -2622, -2622, -2622,   180,   213, 30971, 10288,
   -2622, -2622, -2622, 10288, -2622, -2622, 29977, -2622, 10288, -2622,
   -2622, 10288, 10288, -2622, 26616, -2622, 26810, -2622, 26937, -2622,
    1568, -2622, -2622, -2622, -2622, -2622, 19519,   213, -2622,  1306,
   -2622,  1293, -2622,   180,   213, 31003, 10288, -2622, -2622, -2622,
     -20, 15038,  1629, -2622, -2622, -2622, -2622,  1137,   732,  1294,
    1294, -2622,  1297,  1634,  1716, -2622, -2622, -2622, -2622, 36874,
   -2622, -2622, -2622, -2622,  1309, -2622,  1137, -2622, -2622,  1616,
   -2622, -2622,  1281,   910, -2622,   263,     7, -2622,  1317,  1398,
   -2622, -2622,  -216,  1517,  1517,  1319, -2622,   652,   824, -2622,
    1639,  1640,   824,    59,   460, -2622,   619, -2622, -2622, 37776,
   -2622, -2622,   -82,  1671,  1008, -2622,   634, -2622, -2622, 10288,
     615, -2622, 30604, -2622, 33266, -2622,   652, -2622,  1335,  1339,
   -2622, 37325, 10288,   263, 15940, -2622,  1338, -2622,  1338,    31,
    1341,   660, -2622, -2622, -2622,  1154, 10288, 11761, 30604, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, 10288,  1658,
    1659,   712, -2622, 16391, -2622, -2622,   475,   139, -2622, -2622,
   16391, -2622, -2622,    87, -2622, -2622, -2622, 31462,  1294, -2622,
    1294, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, 15038, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622,   665, -2622, -2622,
   -2622, -2622, 37776, -2622, -2622, 15038, -2622, -2622, 38464, 37776,
   -2622, 15038, -2622,  1344,   675, -2622, -2622, -2622,  1744,  1612,
   -2622,  1294,  1348,   732,  1511, -2622, -2622, -2622,  1710, -2622,
   -2622, 22233,  1352,  1354, -2622, -2622, -2622, 32815, -2622,  1425,
    1796, -2622, 26951, 30604, 30476, 26984, -2622, -2622, 18860,   399,
    1359,  1360, 26998, 27176, 27223, -2622, 10288, 19519, 27320,  1362,
   27350, 27364, 27540, -2622, -2622, -2622,  1368, -2622,  1363, -2622,
   10288, 19519,  1681, -2622,  1678, -2622, -2622,  1706, -2622, -2622,
   33717,  1372, -2622,  1420, -2622, -2622, -2622, 29629, -2622,  1853,
   -2622, -2622,   972, -2622,   155,  1635,   824,  1696,  1698, 15940,
    1286, 11761,  -216,  -216, 10288, -2622,   712,   712,   712, -2622,
   -2622, 37776, -2622,    52,  1562, 32364,    98,  1626, -2622, -2622,
   -2622,   -80,  1600,  1602,   652, 30604,   972,  1384, -2622,   652,
   11761,  1271,  1803, -2622, -2622,    31, 15940, -2622, 30604,  1385,
     673,   615, 30604,   683, -2622, 30604, 16391, -2622, -2622,   475,
    1390,  1756,  1758, 16391, -2622, -2622,  1643, -2622, -2622,   698,
   38464, -2622, 12252, -2622, -2622,  1392,  1204, -2622, -2622,  1394,
     704,   247,   834, -2622, -2622, -2622, -2622,  1204, -2622, -2622,
   -2622,  1394,  1547, -2622,  1394, -2622, -2622, -2622,   254, -2622,
   -2622,   254,  1394, -2622, -2622,  1204,  1204, -2622, -2622, -2622,
     254,  1402, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
    1204,   933, -2622,   136,  1403,  1409,  1411,  1204,  1414,  1391,
    1412, -2622,  2859, -2622,    34, -2622, -2622, 37776, -2622,   795,
   -2622, -2622, 10288, -2622, -2622, -2622, -2622, -2622, -2622, 10288,
   -2622, -2622, 10288, -2622, -2622,  1195, -2622, -2622, -2622, 19519,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, 19519,  1610, -2622,
    1790,  1741,  1783, -2622,  1297,  1484, -2622,  1294,  1083, -2622,
    1734,  -216,  -216, -2622,  1493, -2622,   582,   792, 30604,  1426,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622,  1642, -2622,  1648,
    1651, -2622,   127,   821, -2622,  1008, -2622, -2622, -2622, -2622,
    1427,  1445,  5378,  1083, 15940, 11761,  1446,  1448, -2622,   407,
   -2622, -2622, 10288, -2622, -2622, -2622, -2622, 11761,  1449,  1451,
   37776, -2622,  1541,   558, -2622, -2622,   933, -2622,   -21,  1694,
   -2622,  1715,   630, -2622,  1207, -2622,  1595,   254, -2622,  1604,
   -2622, -2622, -2622,  1452, -2622,   630, -2622,  1453,   630, -2622,
   -2622,   630,   254, -2622, -2622,  1721,   630,  1294, -2622, -2622,
   33717,  1065,  1723, -2622,  1859, -2622,  1824,  1773, -2622,   933,
   -2622,  1774, -2622,  1727, -2622,  1729,  1731,  1735,   630,  1736,
   -2622,   630, 37776, -2622,   125,    93,  1803, -2622,   729,   673,
    1474, -2622,  1805, 27718, 30604, 18912,  1494,  1495, 15038, 10288,
   -2622,  1493,   806,  1800, -2622, -2622,  1874,    49,  -216,   824,
     824,  1511, 10288,  1679, 10288,  1682, 37776,  1498, -2622, -2622,
   -2622, -2622,   279,  1761,   454,   454,   454,   821, -2622, -2622,
   -2622, -2622,    49, -2622, -2622, -2622, -2622,   673, -2622, 37776,
   37776,   735,    48,    48,    48,   850,   850, -2622, -2622, -2622,
   -2622,   414,   199, 37776, -2622, -2622,   516, -2622,  1499,   745,
   -2622, -2622, -2622, -2622,   630, -2622, -2622, -2622,   399, -2622,
     399, -2622, -2622, -2622,  1500, -2622, -2622, -2622,   602, -2622,
   -2622, -2622,  1564,  1686, -2622, -2622, -2622,  1513,  1515,  1520,
    1521, -2622,  1503, -2622, -2622, -2622, -2622,  1524,  1154, -2622,
   37776, -2622,  1778,  1909, -2622, -2622, 10288, -2622, -2622,  1783,
     176,   673, 10288,  1137,  1893,  1898,  1868,  1904,   806, -2622,
     476, -2622,  1818, -2622,    69, -2622,  1792, -2622, 30604,  1528,
   30604,  1529,   751, -2622, 36423, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622,   753,   757, -2622, -2622, -2622,   558, -2622,
   -2622, -2622,   595,   595,   761, -2622, -2622, -2622, -2622,  1798,
   -2622,   777, -2622,   785, -2622, -2622,   602, -2622,   254, -2622,
     254, -2622, -2622, -2622,   673,  1531, 15940, 27732,  1800,   233,
   10288, -2622, -2622, 30604,  1608,   399,   399,  1921,   399, -2622,
    1922,  1923,   476, -2622,  2014, -2622, 10288, 37776, 37776, -2622,
   37776,  1539,  1544,  1781, -2622, -2622, -2622,  1944, -2622,   112,
   -2622, -2622,   516, 37776,  1542,   254,   399,   254, -2622, -2622,
   -2622,   125, -2622, -2622,  1545, -2622,   652, -2622,  1875, -2622,
   -2622,   673, 37776, -2622, -2622, -2622,   399, -2622,   399,   399,
   -2622,   795,  2493,   789,   798, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, 15940, 11761,
    1819,  1560, -2622, -2622, -2622, -2622, -2622,  1956, -2622, -2622,
   -2622, -2622, -2622,  1866, 14567, -2622,  1561,  1858, -2622, -2622,
     804,  1565,  2041, -2622, 10288,  1606,  1603, -2622,   454, 30604,
   -2622, -2622, -2622, -2622
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622,   303, -2622, -2622, -2622, -2622,   340, -1545, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622,  -721, -2622, -2622, -1967,
   -2622, -2622, -2622,   763, -2622,   791, -2622,  1748, -2622,  -507,
    -550,   374, -1990, -1240,  -811,  -807,  -979, -2622, -2622, -2622,
    -758, -2622, -2622,   -27,  -485,   381,   418,     8,    54, -2622,
   -2622,  -466, -2622,  -469, -2622, -2622, -2622,  -477, -2622,  -476,
   -2622, -2622, -2622, -1544,  -523, -1771, -2622,  -768, -1706, -2622,
    -629, -2622,  -671, -2622,  -502, -1332, -1922, -2622, -2622,  -661,
    -456,  -213, -2622, -1601, -2622,  -899, -2622, -2622, -2622,  -905,
   -2622,  -748,  -847, -2622, -2622, -1624, -1861, -2622, -2622, -2182,
     805, -2121,  -791, -1970, -2622,  -757, -2622, -2622, -2622, -2622,
   -2622, -2622,    38, -2622, -2622, -2622,  -641,  -450,  -248, -2554,
   -2622, -2622, -2622, -2622, -2622, -2622,   603, -2622, -2622,   528,
     908, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622,   184, -2622, -2622, -2622, -2622, -2622, -1085,   501,
   -2622, -2622, -2622,  1032, -2622, -2622, -2622,   495, -2622, -2622,
     911, -2622, -2622, -2622, -2622,  -119,   204, -2622, -2622, -2622,
    1218,   191,  1605,  -445, -2622,  1607, -2622, -2622, -2622, -1438,
   -2041, -2622,  1315,  -793, -1836,   572, -2290, -2622, -2622, -2622,
    -404,  1941, -1024,  -908, -2622, -2622, -1068,  -705,  -609, -2622,
   -2622,   428, -2622, -2622, -2622, -2622, -2622, -2622, -2622,  -560,
   -2622,   786,  -334, -2622, -2622, -2622,  1530, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -1206, -2622, -2622, -2622, -2622, -1933,
   -2622,   172, -2622, -2622, -2622, -1743, -2622, -1436, -2622, -2622,
    -796, -1714,  -647, -2622, -2622, -1136,  1555, -1667, -2622, -2622,
   -2622, -1459, -2622, -2622, -2622, -2067, -1222, -2622, -2622, -2621,
    -821, -1633,  -829, -2622,  -668, -1119, -1937, -2622, -2622, -2622,
   -2622,  -951, -2622, -2622, -2622,  -520,   206, -2622, -2622, -2622,
   -2622, -2622, -2622,  -850,  -482,  1573, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622,  1189,   893,   396, -2622, -2622,
    -922, -2136, -2622, -2622,   137, -2622,  -549, -1616,   221,  -377,
   -2622, -2622,  -374, -2444, -2622, -2622, -2622, -2622, -2622, -2622,
    -342, -2622,  -900,  1625, -2622, -2622, -2622, -2622, -2622, -2622,
    1237,   851, -1611,  1613, -2622, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622,  1649, -2622, -1110,  -803, -2622,  -750, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,
     836, -2622, -2622, -2622, -2622, -2622,  1009, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622,   -75,  -743, -2622,
    -751,  -810, -2622,  -818, -2622,  -388,  -906, -2622,  -586, -2148,
   -1556,  -359,  1011, -2457,  -599, -1929,    23, -2622,  -186,  -893,
    -497,    -2,    -8, -1143, -2622, -2622, -2622, -2622, -2622,   548,
   -2622, -2622, -2622,   177, -2622, -2622,  -136, -2622,  -335,  -514,
   -2622, -2622,   126, -2622,   641, -2622, -2622, -2622, -2622, -2622,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622,  1668,
    1304, -2622,   966, -2622, -2622, -2622, -2622, -2622,  -619, -2622,
     967, -1909,   883,  -281, -2622,   -94, -2622, -2622, -2622,  -615,
   -2622, -2622, -2622, -2622, -2622, -2622, -2282,  -806, -2622,  -902,
   -2622, -2622, -2622, -2622, -2622, -2622, -2622, -2622, -1064, -2622,
    -354,  -919
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1659
static const short yytable[] =
{
     314,   788,   578,   649,   651,  1174,   349,   635,   637,   639,
     343,   988,   348,  1276,  1270,   613,  1030,  1263,  1603,  2275,
     856,   997,   518,  1205,  1205,   311,  1783,  1561,  1231,  1232,
    1003,  1191,  1206,  2010,   973,  2015,   976,   977,   978,   979,
     980,  2035,   542,   543,  2289,  1196,   694,   870,  1475,  1584,
    1659,  1970,  1709,  1210,  2048,  1250,  1142,  1144,  2055,  2434,
    2340,   790,   932,  2547,  1527,  1524,  1890,  2685,  2144,  2145,
    1715,  1361,   720,   721,   722,   740,  2126,  2366,  1293, -1270,
     554,  1011,  1012,  1013,  1591,  1586, -1271,  1973,  1555,  2312,
     545,  1589,  1073,  2257,  2314,   920,   -98, -1168,  1978,  2023,
      25,  1332,  2023,  2449,  2023,   553,   737,  1211,  1755,   740,
    1641,  2695,  2355,  1552,   831,  1289,   747,  2353,  2355,  1290,
    2374,  2367,  2368, -1559,  2263,  1938,  2372,  2373, -1657,  2430,
   -1612,  2381,  2382,  -123,  2008,  1949, -1558,  2130,   788,  2425,
     788,   967,   788,  1128,   512,  1612,  2009,   337,  2423,  1138,
    1227,  1963,   557,   581,  2404,  1613,   346,   530,  -941,   532,
     737,   534,   535,   740,  1681,   920,   537,   538,   989,  2523,
     747,   748,   749,   921,   378, -1270,  1617,   920,  2911,  2859,
    2861,  2356,  2357,  2561,  2560,   316,  1620,  1606,   805,  1749,
    1755,   831,  1253,  1946,  -941,  1676,   831,  2074,   525,  2327,
    2862,   378,   806,  2543,  1736,  1965,  2706,   378,  1716,  1658,
    -941,  2358,  1182,   740,  2831,   384,   319,  1650,  1214,   384,
    1683,  1684,   737,  1979,  1254,   740,  2952,  -108,  1980,  -941,
    1930,  3059,   747,   748,   749,   737,  1685,  2444,   740,   384,
    2962,   831,  2301,   921,  2502,   747,   748,   749,  1687,  1285,
     737,  1688,  2425,   740,   633,   921,  1621,  1564,  1737,  1756,
     747,   969,   378,   338,  -109,  2847,   384,  1255,  1172,  1981,
    1601,  2844,   384,  3039,  1602,  2075,   911,  1978,   339,   753,
    2514,   633,  2963,  2848,  1040,  1236,  2292,   633,  1939,   787,
    1931,  1693,  2536,   323,  1966,  1583,  2024,  1005,  1006,  3040,
    2992,   517,   972,  1172, -1270,  1183,  -941,  -941,   555,  1256,
    1203, -1271,  1015,   753,  1521,   561,  1563,  2267,  1567,  -941,
    2696,  -941,  1237,   -51,  -941,  2697,   854,   384,  1940,  1094,
    3060,  1983,  1694,  2025,  2026,   580,  1037,  1695,  2025,  2026,
     872,  1756,   633,  1285,  1215,   320,  1052,  2328,  2329,   812,
    2415,  -941,  2921,  1932,   340,  2444,  1062,  1063,  1064,   831,
    2515,  2698,  1696,  3032,  1738,  -941,  1071,   753,  1622,  1261,
     807,  1697,  1698,  1699,  1978,  1623,  -941,  2729,  1203,  1618,
    1285, -1270,   527,  -941,  2205,   933,  -941,  1701, -1271,  1624,
     813,  2707,   558,  2559,   317,  1607,  1587,  1127,  1851,  2562,
    2564,  2332,  1979,   854,  3015, -1270,  1331,  1980,   854,  2720,
     526,  -941, -1271,  1595,  1702,  -941,  2426,   753,   814,  1964,
    3072,  1172,  1277,  1224,  1418, -1559,  1703,  -941,  1500,   753,
    1265,  2445,   528, -1612,  2355,  1938,   627,  1651, -1558,  2635,
     341,  2355,   753,  1660,  2240,   559,   634,   563,  1981, -1559,
    2446,  -941,  1565,   854,  1607,  2566,  -941,   753,  2264, -1657,
    -941,  2435, -1558,  2048,  1594,  2863,  1984,  2076,   585,  1709,
    -339,  1228,  1902,   636,  -941,  1912,  1289,  1607,  1935,   638,
    1290,   735,  2989,  1968,  -941,  1265,   733,   348,   736,  1762,
    2782,  2048,  1125,  1607,  2503,  1553,  1985,  1554,  1982,  1979,
    2558,  1652,  1289,  1552,  1980,  1974,  1290,   554,   546,  1602,
    1983,   533,  1704,  2014,  2268, -1168,  2130,  2542,    48,  2131,
    2796,  1525,  2027,  1138,  2293,  2699,  2700,   554,  1607,  2426,
    2400,   554,   797,  3041,  1705,   815,  1620,  1601,  1607,   540,
    2128,  1602,  1328,   788,  -941,  1981,   766,  1986,   917,  2445,
    2507,  2508,  2697,  1274,  2294,  1110,   877,   925,  1707,  2959,
     554,   628,  -941,   573,  2749,   548,   931,  1863,  2446,  1744,
     934,   854,  1185,  -941,   939,  2340,  2129,   763,   764,   765,
     766,   945,   348,  2412,  2858,   927,  2728,  1111,   574,  -941,
    1218,  2487,  -941,  1946,   549,  1101,  2185,   788,  1939,  1569,
    2750,  1887,  2516,  2103,  2104,  2105,  1621,  1983,   324,  2851,
     800,  1219,  2112,  1844,  1197,  1947,  2705,  2821,  -123,  2517,
     550,  1978,  2783,  2858,  2486,  3082,  2983,  2355,   760,   761,
     762,   763,   764,   765,   766, -1658,  1102,  1198,  1940,   799,
    1570,   804,  -941,  -941,  1924,  1984,   556,  2960,   801,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,  2008,  2401,  2990,  1926,  2678,  2797,  2132,   325,
    1129,  1936,  1112,  1103,  2009,  1985,  1582,  1658,   384,  2961,
    1130,  2511,  1519,  1845,  -344,  1955,  1186,  1802,  1803,   759,
     760,   761,   762,   763,   764,   765,   766,  2413,  3016,  1104,
    2713,  1201,  2801,  1172,   761,   762,   763,   764,   765,   766,
    2531,   523,   507,  1989,   524,  1101,  1202,  2861,  1622,   761,
     762,   763,   764,   765,   766,  1623,  1986,   750,  2953,  2954,
     816,   817,   818,   819,   820,   821, -1658,  2862,  2501,  1624,
    2258,  2402,  1984,   575,  2923,  3000,  1979,  2465,  2001,  1773,
    1131,  1980,  2699,  2700,  3001,   508,  1102,  2040,  1653,  1316,
    2518,   562,  2723,  -941,  1855,  1857,  2638,   510,   511,  1790,
     920,  1978,  1985,   564,  1508,  1127,  1317,  2298,  2208,  1154,
    2302,  2303,  1265,  1961,  2479,   348,  1962,  1155,  2053,  1677,
     326,  1518,  1981,  1103,  1846,  2724,   554,   566,  1127,  2212,
     327,  3094,   569,  2822,  1105,  2444,  1612,   751,  1171,   831,
     327,  3094,  3017,  2385,  2386,  -165,  1613,   752,   567,  1104,
     328,  1163,  2287,  1986,  2093,  2094,  2095,  1552,  2102,  2907,
    1205,  1978,  2106,  2107,  2108,  2109,  2110,  2111,  2032,  2245,
    2242,  1916,  1918,  2684,  2280,  2281,  1709,  1209,   921,  3018,
    2120,  1843,  2304,   348,  1983,  3019,   753,  1138,   571,  2282,
    2249,  1132,  2121,  1978,  1715,  2122,   754,   755,  1774,   509,
    2740,  2326,   554,  1612,   582,   792,   554,   554,  2526,  2054,
    1904,  2754,   572,  1613,  2527,  2936,   384,   756,  1552,  2358,
    1905,   329,  2690,  2690,  2690,  2392,  1979,  2528,  1804,  2762,
    2763,  1980,   554,   554,  1808,  2393,   793,  2383,   592,  2358,
     554,  1119,  1888,  1120,  2766,   593,  2394,  1278,  2712,  2433,
    1279,  2788,  1282,  2715,  1105,  2359,   554,  1249,  1903,  1101,
     554,   176,   177,  1321,  1138,   787,  2096,  2924,  2925,  2375,
    2376,   554,  1981,  2379,   594,  -185,   794,   588,  2048,   554,
     757,  2384,  2388,  2389,  2390,  1314,  1979,  1631,  1320,  1632,
    1906,  1980,  1836,  1633,  1837,   554,  1327,  2767,   595,  2833,
    1102,  2870,  1775,  2259,  2260,  1776,  1777,  2097,   954,  2748,
     596,  2871,   589,   597,  2879,  2872,   598,  2881,  1979,  1984,
    2882,   371,   955,  1980,  2436,  2885,  1778,  2759,   758,  2445,
    2760,  1779,  1981,   789,  1983,  1681,  1780,  1103,   867,  2764,
    1767,   590,   599,  2824,  2098,   600,  2360,  2901,  2446,  1985,
    2903,  2744,  2784,  2768,  2834,  2835,  2836,  2769,  1786,  1787,
     601,  2926,   376,  1104,  1981,  1791,  -185,  1673,  2050,  2051,
    2099,  2056,  2770,  2277,  2278,   344,   956,  2755,  1041,  1042,
    2758,   513,  2771,   515,   602,  1800,  2745,  2612,  2761,   604,
    2271,  1683,  1684,   605,  1983,  2083,  1807,  2308,  2309,   381,
    1986,  1907,  1812,  1043,  1044,   606,  -303,  1685,   957,  2361,
    -303,  2692,  2693,  1818,  2927,   607,  2506,  2395,   608,  1687,
    1285,  1895,  1688,  1896,  1825,   382,  1983,  1296,  1297,  -185,
    2397,   610,  2399,   958,  -310,  1513,  2520,  2521,   611,  1307,
    1308,   348,   614,  1515,  3077,  2645,   759,   760,   761,   762,
     763,   764,   765,   766,   796,  1990,   810,   865,   866,  -184,
     869,  1509,  1693,   615,  2040,   616,  2041,   617,  1105,  1984,
    1089,  1090,  1091,  1092,  1093,  2100,  2876,  2545,  2546,  2772,
     622,  2773,  2571,  2572,  2774,  2168,  2169,   959,   623,  2369,
     960,  2883,  2632,  2572,   554,  -184,   554,  2775,  2042,  1985,
    2726,  2727,   624,  1694,   961,   625,  1577,   626,  1695,   629,
     554,   872,  1127,  1612,  1552,  2735,  1607,  2751,  2752,  1889,
     348,  2405,   962,  1613,   630,  2776,   963,   511,  1588,  1984,
     398,   631,   370,  1696,   632,  1581,   371,   910,  2210,   554,
    2211,   640,  1697,  1698,  1699,   559,  2909,  2910,   930,   373,
    1986,  1552,  2955,  2910,   641,  1610,   554,   642,  1701,  1985,
    2777,  1984,  2968,  2969,  1593,  2206,   643,  1138,  3009,  3010,
    3013,  2910,   646,  1636,  3014,  2910,   916,   376,  3022,  3023,
    1636,   877,   647,  1655,   648,  1702,  -184,  1647,   650,   348,
     943,  1985,   554,   944,  3025,  3026,   652,  1703,  1635,  2247,
     653,  2248,  3027,  3026,   952,  1635,  3079,  3010,   316,   378,
    1986,   969,  -370,   380,   381,  3080,  3010,   654,  -303,  2043,
    2994,  3093,  2546,  3053,  3054,   655,  2243,   656,  1138,  2956,
    2957,   657,   658,  1678,   659,  1679,  2686,  2687,   660,   661,
     382,  1680,  1986,  1681,   662,   663,   664,   665,   666,  -184,
     939,  1752,  1748,  1682,   667,  1758,   668,   348,   669,   348,
     670,   348,  1662,  1663,  1664,   671,  1665,   554,   672,  1666,
    1667,  1668,  1669,   673,   384,   674,   675,   677,   385,   678,
    1784,   679,   680,  1704,   681,   682,   348,   683,   684,   390,
     685,   730,   686,   687,   688,   689,  2488,  2489,  2679,  1683,
    1684,   690,   691,   692,   693,  1705,  2495,  3029,   695,  3030,
     696,   697,   698,  2297,   699,  1685,  2852,   700,   701,   702,
     703,   704,   705,   706,  1686,   707,   995,  1687,  1285,  1707,
    1688,   708,   709,   710,  1689,  1690,   711,   712,   628,  1691,
    2853,  2854,   713,   714,   715,   398,   716,   717,   718,  1014,
    3069,   719,   723,   724,  3064,   725,  3066,   726,  2344,   987,
     999,  1019,  1038,  -817,  1097,  1114,  1118,  1125,  1115,  1692,
    1693,  1153,  1156,  1158,  1159,  1161,  1162,  1205,  1167,  1169,
    1172,  1170,  1175,  1180,  1181,  2498,  1184,  1187,  1188,  2429,
    1190,  1192,  1193,  1194,  1195,  1670,  1203,  1207,  1212,  1213,
    1216,  1223,  1235,  1233,  1239,  2819,  2820,  1241,  1243,  2500,
    1246,  1694,  1245,  1252,  1312,  2567,  1695,  2568,  1268,  1324,
    1329,  2888,  2442,  1322,  1326,  1330,  1331,  1336,  1338,  1333,
    1337,  1342,  1345,  1346,  1348,  1349,  1350,  1658,  1351,  1127,
    1367,  1696,  1352,  1360,  1366,  1405,  1368,  1373,  1375,  1517,
    1697,  1698,  1699,  1376,  1383,  -941,  1419,  1510,  1700,  1384,
    1385,  1388,   920,  1416,  1426,  1439,  1701,  1440,  2636,  1174,
    1441,  1450,  1494,  2855,  2856,  1495,  1558,  1496,  2569,  1497,
    1127,  1498,  1560,  2653,  1499,  1925,  1507,   554,  2377,  1511,
     554,  -941,  1512,  1702,  1523,  -546,  1557,  2239,  1562,  2469,
    1127,  1572,  1578,   346,  1580,  1703,  1585,  -941,  1598,  1596,
    1959,  1600,  1929,  1681,  1628,  1163,  1629,  1639,  1640,  1642,
    1644,  1673,   554,  1717,  1718,  1719,  1720,  1721,  1722,  1723,
    1977,  1724,  1725,   554,  1726,   554,  1727,  1728,  1729,  1730,
     921,  1731,  1732,  1735,  1741,  1746,  1753,  1969,  1759,  1760,
    1768,   433,  1796,  1813,  1835,  1819,  1883,  1993,  1988,  1879,
    1886,  1884,  1894,  1901,  1954,  1956,  1958,   554,   554,  1683,
    1684,  1960,  1784,  1991,  2017,  2022,  1681,  1998,   348,  1996,
     348,   348,  1997,  2002,  -303,  1685,  2003,  2005,  -303,  2031,
    2006,  1704,  1249,  2000,  -941,  2007,  2040,  1687,  1285,  2060,
    1688,  2061,  2063,  -941,  -941,  2064,  2065,  2066,   370,  2067,
    2125,  -941,   371,  1705,  2129,  2086,  -941,  2134,  -941,  1607,
    2142,  -941,  2146,  2147,  2163,   373,  2148,  1706,  2149,  2150,
    2151,  2152,  1683,  1684,  2157,  2165,  2166,  1707,  2170,  2173,
    1693,  2092,  2174,  2178,  2179,  2947,  2948,  2949,  1685,  2186,
    2193,  1658,  2203,   376,  2230,  2235,  2114,  2238,  2251,  2253,
    1687,  1285,  -941,  1688,  2241,  2539,  2254,  1946,  1947,   554,
    2279,  2283,  2127,  -941,  2290,  2291,  2299,  2313,  2310,  2316,
    -941,  1694,  2324,  2370,  2431,   378,  1695,  2335,  2347,   380,
     381,  2371,  2419,  2398,  2124,  1747,  2437,  -370,   768,   769,
     770,   771,   772,  1693,  2438,  2440,  2451,  2443,  2450,  2456,
    2459,  1696,  -941,  2457,  2816,  2461,   382,  2476,  2484,  2490,
    1697,  1698,  1699,  2477,  -941,   773,   774,   775,   776,  2494,
    1127,  2493,  2497,  2499,  2504,  2505,  1701,  2509,  2512,  2513,
     777,   778,   779,   780,  1694,  2525,   781,  2846,  -941,  1695,
     384,  2532,  -941,  -941,   385,  2533,  2541,  -941,  2544,  2556,
    2557,  2631,  2633,  1702,  2637,   390,   782,   783,  2641,  2643,
    2639,  2644,  2646,  2647,  1696,  1703,  2654,  2668,  2655,  2661,
    2666,  -941,  2669,  1697,  1698,  1699,  2665,  2670,   784,   785,
    2674,  2675,  2852,  2677,  2680,  2681,  -303,  2682,  2703,  1701,
    2708,  2023,  2714,  2722,  2886,  2731,  2730,  2732,  2739,  2710,
    2741,  2711,  2234,  2756,  2792,  2808,  2853,  2854,  2765,  2785,
    2809,   398,  2905,  2810,  2812,  2786,  1702,  2787,  2039,  2793,
    2789,  2815,  2052,  2818,  2057,  2058,  2059,  2478,  1703,  2062,
    2297,  -941,  2826,  2868,  2839,  2068,  2069,  2070,  2071,  2072,
    2073,  2828,  2078,  2079,  2080,  2081,  2082,  2829,  2084,  -941,
    2830,  1704,  2840,  2845,  2869,  2849,  2727,  2850,  2878,  2880,
    2884,  2891,  2892,  2893,  2894,  2896,  2897,  2344,  2898,  2782,
    2899,  2272,  2274,  1705,  2900,  2902,  -941,   492,  2877,  -941,
    2912,  2932,  1277,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,  2913,  1707,  2930,  3102,
    2939,  2917,  2918,  2941,  2944,  2946,  2967,  2974,  2977,  2307,
    2976,  2969,  2986,  2995,  1704,   925,   348,  2985,  2996,  2997,
    2978,   348,  2979,  2315,  2998,  2317,  3004,  2980,  2981,  -941,
    -941,  2982,   348,  3006,  3007,  3008,  1705,  3024,  3033,  2855,
    2856,  3046,  3048,  3049,  3051,  1577,  3056,  2415,  3058,  3063,
    2352,  1784,  2521,  3068,  3070,  2115,   348,   348,  3083,  2092,
    1707,   750,  3042,  3087,  3088,  2092,  3084,  3091,  2092,  2092,
    3092,  3098,  -992,  2092,  2092,  1294,  1295,  1296,  1297,  1298,
    1299,  1300,  2133,  1301,  1302,  1303,  1304,  1305,  1306,  1307,
    1308,  1209,  2113,  1209,  2906,   554,  1733,   348,  1675,   348,
     570,  2092,  2795,  2085,  2951,  2092,  2411,  2737,  2424,  3044,
    3045,  3101,  3047,  3100,  2089,  2806,  2044,  2733,  2416,  2362,
    2391,  2736,  1174,  2746,  2747,  2790,  2970,  2860,  2895,  2887,
    2734,  2530,   554,  2092,  3057,  2539,  3021,  3061,  2966,  2984,
    3065,   751,  1740,  2973,  2406,  2565,  1972,  2255,  1992,  3067,
    1599,   752,  2483,  2252,  1999,  2256,  1273,  2420,  1179,  1611,
    3074,  3005,  3075,  3076,  1945,  1785,  2286,   994,  3012,   974,
    3103,   915,   914,  2937,  2814,  2266,  1340,  1656,   951,     1,
    2135,     2,  3090,  2338,  2718,  2843,  2717,  2683,  3081,   913,
     753,   859,  1319,  1745,  1761,  2482,  2988,  2999,  3038,  1568,
     754,   755,     3,     4,  3050,  2890,     5,  2721,     6,  1576,
     940,     7,  2004,  2331,     8,  2534,  2709,     9,  2841,    10,
      11,   756,    12,  1934,    13,    14,    15,   908,    16,    17,
    1251,    18,  1638,  2945,  1754,  2522,    19,    20,    21,  1643,
      22,    23,  2950,    24,    25,    26,    27,     0,     0,    28,
       0,     0,    29,     0,     0,     0,    30,    31,     0,    32,
       0,     0,     0,     0,     0,     0,     0,     0,    33,   554,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    35,    36,   757,     0,     0,  2496,     0,     0,
       0,     0,     0,     0,  1929,     0,     0,     0,    37,    38,
       0,     0,  2378,     0,     0,     0,     0,     0,  2380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   603,    39,     0,     0,  1993,     0,     0,
       0,     0,   758,     0,     0,     0,  2352,     0,     0,     0,
     621,     0,   348,     0,     0,     0,     0,     0,     0,  1647,
       0,  2807,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   644,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  2092,     0,     0,     0,     0,     0,     0,  2092,  1784,
       0,     0,     0,     0,   676,   348,     0,     0,     0,     0,
       0,  1277,     0,  1277,     0,     0,     0,  3036,     0,     0,
       0,     0,     0,     0,     0,     0,   554,     0,     0,     0,
       0,     0,     0,     0,     0,    40,     0,     0,     0,    41,
    2573,    42,     0,   554,     0,     0,    43,  2629,     0,   554,
       0,     0,     0,     0,   727,   728,   729,   731,   732,     0,
     759,   760,   761,   762,   763,   764,   765,   766,  2574,  1784,
       0,  1107,  1108,     0,  2630,   348,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,     0,     0,     0,
       0,     0,     0,     0,   635,   637,   639,     0,     0,  3036,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  2673,     0,  2975,  2344,     0,     0,   348,     0,
       0,     0,     0,    45,    46,  1925,     0,     0,  1277,  1277,
       0,  1277,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,     0,     0,     0,  2694,
     941,     0,     0,   348,     0,     0,     0,     0,     0,  1277,
       0,     0,     0,   968,     0,     0,   975,     0,     0,     0,
       0,     0,   981,   982,  1577,   983,   984,   985,   986,  1277,
       0,  1277,  1277,     0,  2092,   991,   992,   993,     0,     0,
       0,  2092,     0,   998,     0,  1000,  1001,  1002,     0,     0,
    2092,  1004,  3028,     0,     0,  1007,     0,  1008,     0,  1009,
     750,  1010,     0,     0,     0,     0,     0,     0,  1016,  1017,
       0,  1018,    47,     0,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,     0,  1031,  1032,  1033,  1034,  1035,
    1036,     0,     0,     0,     0,  1046,  1047,     0,  1049,  1050,
    1051,     0,  1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,
    1061,     0,     0,     0,  1065,  2800,  1066,  1067,  1068,  1069,
    1070,     0,    48,  1075,  1076,  1077,  1078,  1079,  1080,  1081,
    1082,  1086,     0,     0,     0,     0,     0,     0,     0,  1096,
     751,     0,     0,     0,  1098,  1099,  1100,     0,     0,     0,
     752,     0,     0,     0,  1109,     0,     0,     0,  1116,     0,
    1117,     0,     0,     0,     0,     0,     0,     0,  1121,  1122,
    1123,  1124,     0,     0,     0,  1133,  1134,  1135,  1136,     0,
    1139,  1140,  1141,  1143,  1145,  1146,  1147,  1148,  1151,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
     755,     0,     0,     0,     0,     0,     0,     0,  2800,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     756,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,     0,     0,     0,     0,
       0,     0,  2352,     0,     0,     0,     0,     0,   348,     0,
       0,     0,     0,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
    2904,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   757,     0,     0,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,     0,     0,  2943,   880,     0,     0,     0,     0,
       0,  2919,     0,     0,     0,     0,     0,   881,     0,     0,
     882,     0,     0,   883,     0,   884,     0,  2800,  2800,   885,
       0,   758,     0,   750,   886,     0,     0,     0,     0,     0,
       0,  2965,   887,     0,     0,     0,     0,   888,   889,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   890,     0,
       0,     0,   891,     0,     0,   892,     0,     0,     0,     0,
       0,     0,     0,  1681,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,  2800,   752,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1678,     0,  1679,     0,     0,     0,     0,     0,  1680,
       0,  1681,     0,   751,     0,     0,     0,   893,     0,     0,
       0,  1682,  2307,   752,     0,     0,     0,   894,   753,  1683,
    1684,     0,     0,     0,     0,     0,     0,     0,   754,   755,
     895,     0,     0,     0,  -303,  1685,     0,     0,  -303,   759,
     760,   761,   762,   763,   764,   765,   766,  1687,  1285,   756,
    1688,     0,   753,     0,  3078,     0,     0,  1683,  1684,     0,
       0,     0,   754,   755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1685,     0,  2943,  2943,     0,  3055,     0,
       0,     0,  1686,   756,     0,  1687,  1285,     0,  1688,     0,
    1693,  3062,  1689,     0,  1487,  1489,  1491,  1691,     0,     0,
       0,     0,     0,  1358,     0,   896,     0,     0,     0,     0,
    3073,     0,   757,     0,     0,   897,     0,   898,     0,     0,
       0,     0,     0,     0,     0,  1514,     0,     0,  1693,  1516,
       0,  1694,     0,     0,     0,   899,  1695,     0,     0,     0,
       0,     0,  1526,     0,     0,     0,   757,     0,     0,     0,
       0,     0,  1577,     0,     0,     0,     0,     0,     0,     0,
     758,  1696,     0,     0,  1556,     0,     0,   900,     0,  1694,
    1697,  1698,  1699,     0,  1695,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1701,     0,     0,  1575,
       0,     0,     0,     0,   758,  1658,     0,     0,     0,  1696,
       0,     0,     0,     0,     0,     0,     0,     0,  1697,  1698,
    1699,     0,     0,  1702,     0,     0,  1700,     0,     0,     0,
       0,     0,     0,     0,  1701,  1703,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
       0,     0,     0,     0,     0,     0,  -303,     0,     0,     0,
       0,  1702,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1703,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,     0,     0,     0,     0,
    1359,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   750,     0,     0,     0,     0,     0,
       0,  1704,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,     0,  1364,  1365,     0,     0,     0,     0,     0,
       0,     0,     0,  1705,     0,     0,     0,  1750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1704,
       0,     0,  1765,     0,  1766,     0,     0,  1707,  1769,     0,
       0,     0,     0,     0,  1771,  1772,     0,     0,     0,     0,
       0,  1705,     0,     0,     0,     0,  1788,     0,  1789,     0,
       0,     0,  1792,  1793,   751,  1706,     0,     0,     0,     0,
    1794,     0,     0,  1795,   752,  1707,     0,     0,  1798,     0,
       0,  1801,     0,     0,     0,     0,     0,     0,  1805,     0,
    1806,     0,     0,     0,  1809,  1810,  1811,     0,     0,     0,
    1814,     0,  1815,     0,  1816,     0,  1817,     0,     0,     0,
    1820,     0,  1821,   753,  1822,     0,  1823,     0,  1824,     0,
    1826,     0,     0,   754,   755,  1827,     0,  1828,     0,  1829,
       0,     0,     0,     0,     0,     0,     0,  1830,  1831,     0,
    1832,     0,  1833,  1834,   756,     0,  1838,  1839,  1840,  1841,
    1842,     0,     0,  1847,  1848,  1849,  1850,     0,  1852,  1853,
    1854,  1856,  1858,  1859,  1860,  1861,  1862,     0,  1864,  1865,
       0,  1866,     0,  1868,  1869,  1870,  1871,  1872,     0,  1874,
       0,  1876,     0,  1878,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1880,     0,     0,  1881,  1882,     0,
       0,     0,     0,     0,     0,     0,     0,   757,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1892,
    1893,     0,     0,  1897,  1898,  1899,  1900,     0,     0,     0,
    1908,  1909,  1910,  1911,     0,  1913,  1914,  1915,  1917,  1919,
    1920,  1921,  1922,  1923,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   758,  1937,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
       0,     0,     0,     0,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     750,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   759,   760,   761,   762,   763,   764,   765,
     766,     0,   750,     0,  1378,  1379,     0,     0,     0,     0,
       0,     0,   880,     0,     0,   750,     0,     0,     0,     0,
       0,     0,     0,     0,   881,     0,     0,   882,     0,     0,
     883,     0,   884,     0,     0,     0,   885,   751,     0,     0,
       0,   886,     0,     0,     0,     0,     0,   752,     0,   887,
     751,     0,     0,     0,   888,   889,     0,     0,     0,     0,
     752,     0,     0,   751,     0,   890,     0,     0,     0,   891,
       0,     0,   906,   752,     0,     0,     0,     0,     0,     0,
       0,  2139,   751,     0,     0,     0,   753,     0,     0,     0,
       0,     0,   752,     0,     0,   751,   754,   755,     0,   753,
       0,     0,     0,     0,     0,   752,     0,  2162,     0,   754,
     755,     0,   753,     0,     0,     0,     0,   756,     0,     0,
       0,     0,   754,   755,   893,     0,     0,     0,     0,     0,
     756,   753,     0,     0,   894,     0,     0,     0,     0,     0,
       0,   754,   755,   756,   753,     0,  2202,   895,     0,     0,
       0,     0,     0,     0,   754,   755,  2207,     0,     0,     0,
       0,     0,   756,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   756,     0,     0,  2216,     0,
     757,     0,     0,     0,  2224,     0,  2226,     0,  2228,     0,
       0,     0,     0,   757,     0,     0,     0,     0,     0,     0,
       0,     0,  2236,     0,     0,  2237,   757,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2244,     0,     0,
       0,     0,   896,     0,     0,   757,     0,     0,   758,     0,
       0,     0,   897,     0,   898,     0,     0,     0,   757,     0,
       0,   758,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   899,     0,   758,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   758,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   900,     0,   758,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  2322,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  2348,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,     0,  1381,  1382,   759,
     760,   761,   762,   763,   764,   765,   766,     0,     0,     0,
    1396,  1397,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,     0,  1398,  1399,     0,     0,     0,     0,     0,
       0,   759,   760,   761,   762,   763,   764,   765,   766,     0,
       0,     0,  1402,  1403,   759,   760,   761,   762,   763,   764,
     765,   766,     0,     0,     0,  1408,  1409,     0,     0,     0,
       0,     0,     0,     0,   880,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   881,     0,     0,   882,
       0,     0,   883,     0,   884,     0,     0,     0,   885,  2441,
       0,     0,     0,   886,     0,     0,     0,     0,     0,     0,
       0,   887,     0,     0,     0,     0,   888,   889,     0,     0,
    2452,     0,     0,     0,     0,  2453,  2454,   890,  2455,     0,
       0,   891,     0,     0,     0,     0,     0,  2458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  2462,
       0,     0,     0,  2463,     0,     0,  2464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    2467,     0,     0,     0,  2468,     0,     0,     0,     0,  2470,
       0,     0,  2471,  2472,     0,     0,   893,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   894,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2481,     0,   895,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     603,     0,     0,     0,   896,     0,     0,     0,     0,     0,
       0,     0,     0,  2535,   897,     0,   898,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2548,     0,     0,
       0,     0,     0,     0,   899,     0,     0,     0,     0,  2555,
       0,     0,     0,     0,     0,     0,     0,     0,    99,   100,
     101,   102,   103,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   900,     0,     0,   105,
       0,     0,   106,   107,   108,   109,     0,   110,   111,   112,
     113,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,   120,     0,   121,     0,     0,   122,     0,
     123,     0,     0,   124,     0,     0,   125,   126,   127,   128,
     129,   130,  2023,     0,   131,   132,   133,   134,     0,     0,
     135,     0,   136,   137,   138,     0,   139,     0,   140,   141,
       0,     0,     0,     0,   142,   143,   144,   145,     0,   146,
     147,   148,     0,   149,     0,   150,   151,  2659,     0,   152,
     153,     0,   154,     0,   155,     0,     0,   156,  2041,     0,
       0,  2667,   157,     0,     0,   158,     0,     0,   159,   160,
     161,     0,   162,   163,   164,     0,     0,   165,   166,     0,
     167,     0,   168,   169,   170,     0,     0,     0,     0,   171,
    2042,   172,     0,   173,     0,  2688,     0,   174,     0,     0,
     175,   176,   177,     0,   178,  1612,   179,     0,     0,   180,
       0,     0,   181,     0,     0,  1613,     0,     0,   182,   183,
       0,  2421,     0,   184,     0,   185,     0,     0,     0,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,     0,   210,   211,   212,     0,   213,
       0,   214,     0,     0,   215,     0,   216,   217,     0,     0,
       0,     0,     0,     0,     0,   218,   219,   220,     0,     0,
     221,   222,   223,   224,   225,   226,   227,   228,     0,     0,
       0,     0,   229,     0,   230,     0,   231,   232,     0,     0,
     233,   234,   235,   236,     0,     0,   237,   238,   239,   240,
     241,  2043,     0,     0,   242,   243,     0,   244,     0,     0,
     245,   246,     0,  2803,     0,     0,   247,     0,   248,   249,
    2804,     0,     0,  2805,   250,     0,   251,   252,     0,   253,
       0,     0,     0,   254,     0,     0,   255,     0,   256,     0,
       0,     0,     0,   257,     0,   258,   259,   260,   261,     0,
       0,     0,   262,     0,   263,   264,     0,     0,   265,   266,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,   269,   270,     0,   271,     0,   272,   273,   274,
     275,     0,     0,  2322,     0,     0,     0,     0,     0,     0,
     276,     0,     0,  2548,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,   278,   279,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   280,     0,
       0,   281,     0,     0,     0,   282,   283,     0,   284,     0,
       0,     0,     0,   285,     0,     0,     0,   286,     0,     0,
     287,   288,   289,   290,     0,     0,     0,   291,   292,   293,
       0,   294,   295,     0,   296,     0,   297,     0,     0,     0,
       0,     0,     0,   298,     0,   299,   300,     0,     0,     0,
    2548,     0,     0,     0,     0,     0,     0,     0,   301,     0,
       0,     0,     0,  2938,   302,  2940,   303,     0,     0,   304,
       0,     0,   305,     0,   306,     0,   307,   308,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2088,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2987,     0,     0,
       0,     0,     0,  2993,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,   100,   101,   102,   103,     0,     0,     0,   104,
       0,     0,     0,     0,     0,     0,   351,     0,     0,     0,
       0,     0,   105,     0,     0,   106,   352,   108,   109,     0,
     110,   111,   112,   113,   353,     0,     0,   114,     0,     0,
     115,  2548,   116,   117,   118,   119,   120,   354,   121,     0,
       0,   122,     0,   123,   355,   356,   124,  3052,     0,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,   357,   358,   135,   359,   360,   137,   361,     0,   139,
       0,   140,   141,     0,     0,     0,     0,   142,   143,   144,
     145,   362,   146,   147,   148,  1083,   149,     0,   150,   151,
       0,   363,   364,   153,     0,   154,     0,   365,     0,     0,
     156,     0,   366,   367,     0,   157,   368,     0,   158,     0,
       0,   159,   160,   161,     0,   162,   163,   164,     0,     0,
     165,   166,   369,   167,   370,   168,   169,   170,   371,     0,
       0,     0,   171,     0,   172,  3099,   173,   372,     0,     0,
     174,   373,     0,   175,   176,   177,     0,   178,     0,   179,
       0,     0,   180,     0,     0,   181,     0,     0,     0,  1084,
     374,   182,   183,     0,     0,     0,   184,   375,   185,   376,
       0,     0,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   377,   202,
     203,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,   378,   213,   379,   214,   380,   381,   215,     0,   216,
     217,     0,     0,     0,     0,     0,     0,     0,   218,   219,
     220,     0,     0,   221,   222,   223,   224,   225,   226,   227,
     228,     0,   382,     0,     0,   229,     0,   230,     0,   231,
     232,     0,     0,   233,   234,   383,   236,     0,     0,   237,
     238,   239,   240,   241,     0,     0,     0,   242,   243,     0,
     244,     0,     0,   245,   246,     0,   384,     0,  1085,   247,
     385,   248,   249,   386,   387,   388,   389,   250,     0,   251,
     252,   390,   253,     0,     0,     0,   254,     0,     0,   255,
     391,   256,     0,     0,     0,     0,   257,     0,   258,   259,
     260,   392,     0,     0,   393,   262,   394,   263,   395,     0,
       0,   265,   266,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,   269,   270,     0,   271,     0,
     272,   273,   396,   397,     0,     0,     0,   398,     0,     0,
       0,     0,     0,   276,   399,     0,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,     0,     0,
       0,     0,   412,   413,   414,   415,     0,   416,   417,   278,
     279,   418,   419,   420,   421,   422,     0,   423,   424,   425,
     426,   427,   428,   429,   430,     0,     0,     0,   431,   283,
     432,   284,   433,   434,   435,   436,   437,   438,   439,   440,
     441,     0,     0,   442,   288,   289,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,     0,   458,   459,   460,   461,   462,     0,   299,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,     0,   302,     0,   480,
     481,   482,   304,     0,     0,   305,     0,   306,     0,   307,
     308,   309,     0,     0,     0,   483,   484,     0,     0,     0,
       0,   485,     0,   486,     0,     0,   487,   488,     0,   489,
     490,   491,    99,   100,   101,   102,   103,     0,     0,     0,
     104,     0,     0,     0,     0,     0,     0,   351,     0,     0,
       0,     0,     0,   105,     0,     0,   106,   352,   108,   109,
       0,   110,   111,   112,   113,   353,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,   120,   354,   121,
       0,     0,   122,     0,   123,   355,   356,   124,     0,     0,
     125,   126,   127,   128,   129,   130,     0,     0,   131,   132,
     133,   134,   357,   358,   135,   359,   360,   137,   361,     0,
     139,     0,   140,   141,  2318,     0,     0,     0,   142,   143,
     144,   145,  2319,   146,   147,   148,     0,   149,     0,   150,
     151,     0,   363,   364,   153,     0,   154,     0,   365,     0,
       0,   156,     0,   366,   367,     0,   157,  2320,     0,   158,
       0,     0,   159,   160,   161,     0,   162,   163,   164,     0,
       0,   165,   166,   369,   167,   370,   168,   169,   170,   371,
       0,     0,     0,   171,     0,   172,     0,   173,   372,     0,
       0,   174,   373,     0,   175,   176,   177,     0,   178,     0,
     179,     0,     0,   180,     0,     0,   181,     0,     0,     0,
       0,   374,   182,   183,     0,     0,     0,   184,   375,   185,
     376,     0,     0,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   377,
     202,   203,   204,   205,   206,   207,   208,   209,     0,   210,
     211,   212,   378,   213,   379,   214,   380,   381,   215,  2321,
     216,   217,     0,     0,     0,     0,     0,     0,     0,   218,
     219,   220,     0,     0,   221,   222,   223,   224,   225,   226,
     227,   228,     0,   382,     0,     0,   229,     0,   230,     0,
     231,   232,     0,     0,   233,   234,   383,   236,     0,     0,
     237,   238,   239,   240,   241,     0,     0,     0,   242,   243,
       0,   244,     0,     0,   245,   246,     0,   384,     0,     0,
     247,   385,   248,   249,   386,   387,   388,   389,   250,     0,
     251,   252,   390,   253,     0,     0,     0,   254,     0,     0,
     255,   391,   256,     0,     0,     0,     0,   257,     0,   258,
     259,   260,   392,     0,     0,   393,   262,   394,   263,   395,
       0,     0,   265,   266,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,   269,   270,     0,   271,
       0,   272,   273,   396,   397,     0,     0,     0,   398,     0,
       0,     0,     0,     0,   276,   399,     0,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,     0,
       0,     0,     0,   412,   413,   414,   415,     0,   416,   417,
     278,   279,   418,   419,   420,   421,   422,     0,   423,   424,
     425,   426,   427,   428,   429,   430,     0,     0,     0,   431,
     283,   432,   284,   433,   434,   435,   436,   437,   438,   439,
     440,   441,     0,     0,   442,   288,   289,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,     0,   458,   459,   460,   461,   462,     0,   299,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,     0,   302,     0,
     480,   481,   482,   304,     0,     0,   305,     0,   306,     0,
     307,   308,   309,     0,     0,     0,   483,   484,     0,     0,
       0,     0,   485,     0,   486,     0,     0,   487,   488,     0,
     489,   490,   491,    99,   100,   101,   102,   103,     0,     0,
       0,   104,     0,     0,     0,     0,     0,     0,   351,     0,
       0,     0,     0,     0,   105,     0,     0,   106,   352,   108,
     109,     0,   110,   111,   112,   113,   353,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,   120,   354,
     121,     0,     0,   122,     0,   123,   355,   356,   124,     0,
       0,   125,   126,   127,   128,   129,   130,   730,     0,   131,
     132,   133,   134,   357,   358,   135,   359,   360,   137,   361,
       0,   139,     0,   140,   141,     0,     0,     0,     0,   142,
     143,   144,   145,   362,   146,   147,   148,     0,   149,     0,
     150,   151,     0,   363,   364,   153,     0,   154,     0,   365,
       0,     0,   156,     0,   366,   367,     0,   157,   368,     0,
     158,     0,     0,   159,   160,   161,     0,   162,   163,   164,
       0,     0,   165,   166,   369,   167,   370,   168,   169,   170,
     371,     0,     0,     0,   171,     0,   172,     0,   173,   372,
       0,     0,   174,   373,     0,   175,   176,   177,     0,   178,
       0,   179,     0,     0,   180,     0,     0,   181,     0,     0,
       0,     0,   374,   182,   183,     0,     0,     0,   184,   375,
     185,   376,     0,     0,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     377,   202,   203,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   378,   213,   379,   214,   380,   381,   215,
       0,   216,   217,     0,     0,     0,     0,     0,     0,     0,
     218,   219,   220,     0,     0,   221,   222,   223,   224,   225,
     226,   227,   228,     0,   382,     0,     0,   229,     0,   230,
       0,   231,   232,     0,     0,   233,   234,   383,   236,     0,
       0,   237,   238,   239,   240,   241,     0,     0,     0,   242,
     243,     0,   244,     0,     0,   245,   246,     0,   384,     0,
       0,   247,   385,   248,   249,   386,   387,   388,   389,   250,
       0,   251,   252,   390,   253,     0,     0,     0,   254,     0,
       0,   255,   391,   256,     0,     0,     0,     0,   257,     0,
     258,   259,   260,   392,     0,     0,   393,   262,   394,   263,
     395,     0,     0,   265,   266,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,   269,   270,     0,
     271,     0,   272,   273,   396,   397,     0,     0,     0,   398,
       0,     0,     0,     0,     0,   276,   399,     0,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
       0,     0,     0,     0,   412,   413,   414,   415,     0,   416,
     417,   278,   279,   418,   419,   420,   421,   422,     0,   423,
     424,   425,   426,   427,   428,   429,   430,     0,     0,     0,
     431,   283,   432,   284,   433,   434,   435,   436,   437,   438,
     439,   440,   441,     0,     0,   442,   288,   289,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,     0,   458,   459,   460,   461,   462,     0,
     299,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,     0,   302,
       0,   480,   481,   482,   304,     0,     0,   305,     0,   306,
       0,   307,   308,   309,     0,     0,     0,   483,   484,     0,
       0,     0,     0,   485,     0,   486,     0,     0,   487,   488,
       0,   489,   490,   491,    99,   100,   101,   102,   103,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,   351,
       0,     0,     0,     0,     0,   105,     0,     0,   106,   352,
     108,   109,     0,   110,   111,   112,   113,   353,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,   120,
     354,   121,     0,     0,   122,     0,   123,   355,   356,   124,
       0,     0,   125,   126,   127,   128,   129,   130,     0,     0,
     131,   132,   133,   134,   357,   358,   135,   359,   360,   137,
     361,     0,   139,     0,   140,   141,     0,     0,     0,     0,
     142,   143,   144,   145,   362,   146,   147,   148,     0,   149,
       0,   150,   151,     0,   363,   364,   153,     0,   154,     0,
     365,     0,     0,   156,     0,   366,   367,     0,   157,   368,
       0,   158,     0,     0,   159,   160,   161,     0,   162,   163,
     164,     0,     0,   165,   166,   369,   167,   370,   168,   169,
     170,   371,     0,     0,     0,   171,     0,   172,     0,   173,
     372,     0,     0,   174,   373,     0,   175,   176,   177,     0,
     178,     0,   179,     0,     0,   180,     0,     0,   181,     0,
       0,     0,     0,   374,   182,   183,     0,     0,     0,   184,
     375,   185,   376,     0,     0,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   377,   202,   203,   204,   205,   206,   207,   208,   209,
       0,   210,   211,   212,   378,   213,   379,   214,   380,   381,
     215,     0,   216,   217,     0,     0,     0,     0,     0,     0,
       0,   218,   219,   220,     0,     0,   221,   222,   223,   224,
     225,   226,   227,   228,     0,   382,     0,     0,   229,     0,
     230,     0,   231,   232,     0,     0,   233,   234,   383,   236,
       0,     0,   237,   238,   239,   240,   241,     0,     0,     0,
     242,   243,     0,   244,     0,     0,   245,   246,     0,   384,
       0,     0,   247,   385,   248,   249,   386,   387,   388,   389,
     250,     0,   251,   252,   390,   253,     0,     0,     0,   254,
       0,     0,   255,   391,   256,     0,     0,     0,     0,   257,
       0,   258,   259,   260,   392,     0,     0,   393,   262,   394,
     263,   395,     0,     0,   265,   266,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,   269,   270,
       0,   271,     0,   272,   273,   396,   397,     0,     0,     0,
     398,     0,     0,     0,     0,     0,   276,   399,     0,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,     0,     0,     0,     0,   412,   413,   414,   415,     0,
     416,   417,   278,   279,   418,   419,   420,   421,   422,     0,
     423,   424,   425,   426,   427,   428,   429,   430,     0,     0,
       0,   431,   283,   432,   284,   433,   434,   435,   436,   437,
     438,   439,   440,   441,     0,     0,   442,   288,   289,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,     0,   458,   459,   460,   461,   462,
       0,   299,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,     0,
     302,     0,   480,   481,   482,   304,     0,     0,   305,     0,
     306,     0,   307,   308,   309,     0,     0,     0,   483,   484,
       0,     0,     0,     0,   485,     0,   486,   987,     0,   487,
     488,     0,   489,   490,   491,    99,   100,   101,   102,   103,
       0,     0,     0,   104,     0,     0,     0,     0,     0,     0,
     351,     0,     0,     0,     0,     0,   105,     0,     0,   106,
     352,   108,   109,     0,   110,   111,   112,   113,   353,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
     120,   354,   121,     0,     0,   122,     0,   123,   355,   356,
     124,     0,     0,   125,   126,   127,   128,   129,   130,     0,
       0,   131,   132,   133,   134,   357,   358,   135,   359,   360,
     137,   361,     0,   139,     0,   140,   141,     0,     0,     0,
       0,   142,   143,   144,   145,   362,   146,   147,   148,     0,
     149,     0,   150,   151,     0,   363,   364,   153,     0,   154,
       0,   365,     0,     0,   156,     0,   366,   367,     0,   157,
     368,     0,   158,     0,     0,   159,   160,   161,     0,   162,
     163,   164,     0,     0,   165,   166,   369,   167,   370,   168,
     169,   170,   371,     0,     0,     0,   171,     0,   172,     0,
     173,   372,     0,     0,   174,   373,     0,   175,   176,   177,
       0,   178,     0,   179,     0,     0,   180,     0,     0,   181,
       0,     0,     0,     0,   374,   182,   183,     0,     0,     0,
     184,   375,   185,   376,     0,     0,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   377,   202,   203,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   378,   213,   379,   214,   380,
     381,   215,     0,   216,   217,     0,     0,     0,     0,     0,
       0,     0,   218,   219,   220,     0,     0,   221,   222,   223,
     224,   225,   226,   227,   228,     0,   382,     0,     0,   229,
       0,   230,     0,   231,   232,     0,     0,   233,   234,   383,
     236,     0,     0,   237,   238,   239,   240,   241,     0,     0,
       0,   242,   243,     0,   244,     0,     0,   245,   246,     0,
     384,     0,     0,   247,   385,   248,   249,   386,   387,   388,
     389,   250,     0,   251,   252,   390,   253,     0,     0,     0,
     254,     0,     0,   255,   391,   256,     0,     0,     0,     0,
     257,     0,   258,   259,   260,   392,     0,     0,   393,   262,
     394,   263,   395,     0,     0,   265,   266,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,   269,
     270,     0,   271,     0,   272,   273,   396,   397,     0,     0,
       0,   398,     0,     0,     0,     0,     0,   276,   399,     0,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,     0,     0,     0,     0,   412,   413,   414,   415,
       0,   416,   417,   278,   279,   418,   419,   420,   421,   422,
       0,   423,   424,   425,   426,   427,   428,   429,   430,     0,
       0,     0,   431,   283,   432,   284,   433,   434,   435,   436,
     437,   438,   439,   440,   441,     0,     0,   442,   288,   289,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,     0,   458,   459,   460,   461,
     462,     0,   299,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
       0,   302,     0,   480,   481,   482,   304,     0,     0,   305,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   483,
     484,     0,     0,     0,     0,   485,     0,   486,  1048,     0,
     487,   488,     0,   489,   490,   491,    99,   100,   101,   102,
     103,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,   351,     0,     0,     0,     0,     0,   105,     0,     0,
     106,   352,   108,   109,     0,   110,   111,   112,   113,   353,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,   120,   354,   121,     0,     0,   122,     0,   123,   355,
     356,   124,     0,     0,   125,   126,   127,   128,   129,   130,
       0,     0,   131,   132,   133,   134,   357,   358,   135,   359,
     360,   137,   361,     0,   139,     0,   140,   141,     0,     0,
       0,     0,   142,   143,   144,   145,   362,   146,   147,   148,
       0,   149,     0,   150,   151,     0,   363,   364,   153,     0,
     154,     0,   365,     0,     0,   156,     0,   366,   367,     0,
     157,   368,     0,   158,     0,     0,   159,   160,   161,     0,
     162,   163,   164,     0,     0,   165,   166,   369,   167,   370,
     168,   169,   170,   371,     0,     0,     0,   171,     0,   172,
       0,   173,   372,     0,     0,   174,   373,     0,   175,   176,
     177,     0,   178,     0,   179,     0,     0,   180,     0,     0,
     181,     0,     0,     0,     0,   374,   182,   183,     0,     0,
       0,   184,   375,   185,   376,     0,     0,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   377,   202,   203,   204,   205,   206,   207,
     208,   209,     0,   210,   211,   212,   378,   213,   379,   214,
     380,   381,   215,     0,   216,   217,     0,     0,     0,     0,
       0,     0,     0,   218,   219,   220,     0,     0,   221,   222,
     223,   224,   225,   226,   227,   228,     0,   382,     0,     0,
     229,     0,   230,     0,   231,   232,     0,     0,   233,   234,
     383,   236,     0,     0,   237,   238,   239,   240,   241,     0,
       0,     0,   242,   243,     0,   244,     0,     0,   245,   246,
       0,   384,     0,     0,   247,   385,   248,   249,   386,   387,
     388,   389,   250,     0,   251,   252,   390,   253,     0,     0,
       0,   254,     0,     0,   255,   391,   256,     0,     0,     0,
       0,   257,     0,   258,   259,   260,   392,     0,     0,   393,
     262,   394,   263,   395,     0,     0,   265,   266,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
     269,   270,     0,   271,     0,   272,   273,   396,   397,     0,
       0,     0,   398,     0,     0,     0,     0,     0,   276,   399,
       0,   400,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,     0,     0,     0,     0,   412,   413,   414,
     415,     0,   416,   417,   278,   279,   418,   419,   420,   421,
     422,     0,   423,   424,   425,   426,   427,   428,   429,   430,
       0,     0,     0,   431,   283,   432,   284,   433,   434,   435,
     436,   437,   438,   439,   440,   441,     0,     0,   442,   288,
     289,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,     0,   458,   459,   460,
     461,   462,     0,   299,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,     0,   302,     0,   480,   481,   482,   304,     0,     0,
     305,     0,   306,     0,   307,   308,   309,     0,     0,     0,
     483,   484,     0,     0,     0,     0,   485,     0,   486,  1074,
       0,   487,   488,     0,   489,   490,   491,    99,   100,   101,
     102,   103,     0,     0,     0,   104,     0,     0,     0,     0,
       0,     0,   351,     0,     0,     0,     0,     0,   105,     0,
       0,   106,   352,   108,   109,     0,   110,   111,   112,   113,
     353,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,   120,   354,   121,     0,     0,   122,     0,   123,
     355,   356,   124,     0,     0,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,   357,   358,   135,
     359,   360,   137,   361,     0,   139,     0,   140,   141,     0,
       0,     0,     0,   142,   143,   144,   145,   362,   146,   147,
     148,     0,   149,     0,   150,   151,     0,   363,   364,   153,
       0,   154,     0,   365,     0,     0,   156,     0,   366,   367,
       0,   157,   368,     0,   158,     0,     0,   159,   160,   161,
       0,   162,   163,   164,     0,     0,   165,   166,   369,   167,
     370,   168,   169,   170,   371,     0,     0,     0,   171,     0,
     172,     0,   173,   372,     0,     0,   174,   373,     0,   175,
     176,   177,     0,   178,     0,   179,     0,     0,   180,     0,
       0,   181,     0,     0,     0,     0,   374,   182,   183,     0,
       0,     0,   184,   375,   185,   376,     0,     0,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   377,   202,   203,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,   378,   213,   379,
     214,   380,   381,   215,     0,   216,   217,     0,     0,     0,
       0,     0,     0,     0,   218,   219,   220,     0,     0,   221,
     222,   223,   224,   225,   226,   227,   228,     0,   382,     0,
       0,   229,     0,   230,     0,   231,   232,     0,     0,   233,
     234,   383,   236,     0,     0,   237,   238,   239,   240,   241,
       0,     0,     0,   242,   243,     0,   244,     0,     0,   245,
     246,     0,   384,     0,     0,   247,   385,   248,   249,   386,
     387,   388,   389,   250,     0,   251,   252,   390,   253,     0,
       0,     0,   254,     0,     0,   255,   391,   256,     0,     0,
       0,     0,   257,     0,   258,   259,   260,   392,     0,     0,
     393,   262,   394,   263,   395,     0,     0,   265,   266,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,   269,   270,     0,   271,     0,   272,   273,   396,   397,
       0,     0,     0,   398,     0,     0,     0,     0,     0,   276,
     399,     0,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,     0,     0,     0,     0,   412,   413,
     414,   415,     0,   416,   417,   278,   279,   418,   419,   420,
     421,   422,     0,   423,   424,   425,   426,   427,   428,   429,
     430,     0,     0,     0,   431,   283,   432,   284,   433,   434,
     435,   436,   437,   438,   439,   440,   441,     0,     0,   442,
     288,   289,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,     0,   458,   459,
     460,   461,   462,     0,   299,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,     0,   302,     0,   480,   481,   482,   304,     0,
       0,   305,     0,   306,     0,   307,   308,   309,     0,     0,
       0,   483,   484,     0,     0,     0,     0,   485,     0,   486,
    1095,     0,   487,   488,     0,   489,   490,   491,    99,   100,
     101,   102,   103,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,   351,     0,     0,     0,     0,     0,   105,
       0,     0,  1149,   352,   108,   109,     0,   110,   111,   112,
     113,   353,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,   120,   354,   121,     0,     0,   122,     0,
     123,   355,   356,   124,     0,     0,   125,   126,   127,   128,
     129,   130,     0,     0,   131,   132,   133,   134,   357,   358,
     135,   359,   360,   137,   361,     0,   139,     0,   140,   141,
    1150,     0,     0,     0,   142,   143,   144,   145,   362,   146,
     147,   148,     0,   149,     0,   150,   151,     0,   363,   364,
     153,     0,   154,     0,   365,     0,     0,   156,     0,   366,
     367,     0,   157,   368,     0,   158,     0,     0,   159,   160,
     161,     0,   162,   163,   164,     0,     0,   165,   166,   369,
     167,   370,   168,   169,   170,   371,     0,     0,     0,   171,
       0,   172,     0,   173,   372,     0,     0,   174,   373,     0,
     175,   176,   177,     0,   178,     0,   179,     0,     0,   180,
       0,     0,   181,     0,     0,     0,     0,   374,   182,   183,
       0,     0,     0,   184,   375,   185,   376,     0,     0,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   377,   202,   203,   204,   205,
     206,   207,   208,   209,     0,   210,   211,   212,   378,   213,
     379,   214,   380,   381,   215,     0,   216,   217,     0,     0,
       0,     0,     0,     0,     0,   218,   219,   220,     0,     0,
     221,   222,   223,   224,   225,   226,   227,   228,     0,   382,
       0,     0,   229,     0,   230,     0,   231,   232,     0,     0,
     233,   234,   383,   236,     0,     0,   237,   238,   239,   240,
     241,     0,     0,     0,   242,   243,     0,   244,     0,     0,
     245,   246,     0,   384,     0,     0,   247,   385,   248,   249,
     386,   387,   388,   389,   250,     0,   251,   252,   390,   253,
       0,     0,     0,   254,     0,     0,   255,   391,   256,     0,
       0,     0,     0,   257,     0,   258,   259,   260,   392,     0,
       0,   393,   262,   394,   263,   395,     0,     0,   265,   266,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,   269,   270,     0,   271,     0,   272,   273,   396,
     397,     0,     0,     0,   398,     0,     0,     0,     0,     0,
     276,   399,     0,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,     0,     0,     0,     0,   412,
     413,   414,   415,     0,   416,   417,   278,   279,   418,   419,
     420,   421,   422,     0,   423,   424,   425,   426,   427,   428,
     429,   430,     0,     0,     0,   431,   283,   432,   284,   433,
     434,   435,   436,   437,   438,   439,   440,   441,     0,     0,
     442,   288,   289,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,     0,   458,
     459,   460,   461,   462,     0,   299,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,     0,   302,     0,   480,   481,   482,   304,
       0,     0,   305,     0,   306,     0,   307,   308,   309,     0,
       0,     0,   483,   484,     0,     0,     0,     0,   485,     0,
     486,     0,     0,   487,   488,     0,   489,   490,   491,    99,
     100,   101,   102,   103,     0,     0,     0,   104,     0,     0,
       0,     0,     0,     0,   351,     0,     0,     0,     0,     0,
     105,     0,     0,   106,   352,   108,   109,     0,   110,   111,
     112,   113,   353,     0,     0,   114,     0,     0,   115,     0,
     116,   117,   118,   119,   120,   354,   121,     0,     0,   122,
       0,   123,   355,   356,   124,     0,     0,   125,   126,   127,
     128,   129,   130,     0,     0,   131,   132,   133,   134,   357,
     358,   135,   359,   360,   137,   361,     0,   139,     0,   140,
     141,     0,     0,     0,     0,   142,   143,   144,   145,   362,
     146,   147,   148,     0,   149,     0,   150,   151,     0,   363,
     364,   153,     0,   154,     0,   365,     0,     0,   156,     0,
     366,   367,     0,   157,   368,     0,   158,     0,     0,   159,
     160,   161,     0,   162,   163,   164,     0,     0,   165,   166,
     369,   167,   370,   168,   169,   170,   371,     0,     0,  1486,
     171,     0,   172,     0,   173,   372,     0,     0,   174,   373,
       0,   175,   176,   177,     0,   178,     0,   179,     0,     0,
     180,     0,     0,   181,     0,     0,     0,     0,   374,   182,
     183,     0,     0,     0,   184,   375,   185,   376,     0,     0,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   377,   202,   203,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   378,
     213,   379,   214,   380,   381,   215,     0,   216,   217,     0,
       0,     0,     0,     0,     0,     0,   218,   219,   220,     0,
       0,   221,   222,   223,   224,   225,   226,   227,   228,     0,
     382,     0,     0,   229,     0,   230,     0,   231,   232,     0,
       0,   233,   234,   383,   236,     0,     0,   237,   238,   239,
     240,   241,     0,     0,     0,   242,   243,     0,   244,     0,
       0,   245,   246,     0,   384,     0,     0,   247,   385,   248,
     249,   386,   387,   388,   389,   250,     0,   251,   252,   390,
     253,     0,     0,     0,   254,     0,     0,   255,   391,   256,
       0,     0,     0,     0,   257,     0,   258,   259,   260,   392,
       0,     0,   393,   262,   394,   263,   395,     0,     0,   265,
     266,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,   269,   270,     0,   271,     0,   272,   273,
     396,   397,     0,     0,     0,   398,     0,     0,     0,     0,
       0,   276,   399,     0,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,     0,     0,     0,     0,
     412,   413,   414,   415,     0,   416,   417,   278,   279,   418,
     419,   420,   421,   422,     0,   423,   424,   425,   426,   427,
     428,   429,   430,     0,     0,     0,   431,   283,   432,   284,
     433,   434,   435,   436,   437,   438,   439,   440,   441,     0,
       0,   442,   288,   289,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,     0,
     458,   459,   460,   461,   462,     0,   299,   463,   464,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,     0,   302,     0,   480,   481,   482,
     304,     0,     0,   305,     0,   306,     0,   307,   308,   309,
       0,     0,     0,   483,   484,     0,     0,     0,     0,   485,
       0,   486,     0,     0,   487,   488,     0,   489,   490,   491,
      99,   100,   101,   102,   103,     0,     0,     0,   104,     0,
       0,     0,     0,     0,     0,   351,     0,     0,     0,     0,
       0,   105,     0,     0,   106,   352,   108,   109,     0,   110,
     111,   112,   113,   353,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,   120,   354,   121,     0,     0,
     122,     0,   123,   355,   356,   124,     0,     0,   125,   126,
     127,   128,   129,   130,     0,     0,   131,   132,   133,   134,
     357,   358,   135,   359,   360,   137,   361,     0,   139,     0,
     140,   141,     0,     0,     0,     0,   142,   143,   144,   145,
     362,   146,   147,   148,     0,   149,     0,   150,   151,     0,
     363,   364,   153,     0,   154,     0,   365,     0,     0,   156,
       0,   366,   367,     0,   157,   368,     0,   158,     0,     0,
     159,   160,   161,     0,   162,   163,   164,     0,     0,   165,
     166,   369,   167,   370,   168,   169,   170,   371,     0,     0,
    1488,   171,     0,   172,     0,   173,   372,     0,     0,   174,
     373,     0,   175,   176,   177,     0,   178,     0,   179,     0,
       0,   180,     0,     0,   181,     0,     0,     0,     0,   374,
     182,   183,     0,     0,     0,   184,   375,   185,   376,     0,
       0,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   377,   202,   203,
     204,   205,   206,   207,   208,   209,     0,   210,   211,   212,
     378,   213,   379,   214,   380,   381,   215,     0,   216,   217,
       0,     0,     0,     0,     0,     0,     0,   218,   219,   220,
       0,     0,   221,   222,   223,   224,   225,   226,   227,   228,
       0,   382,     0,     0,   229,     0,   230,     0,   231,   232,
       0,     0,   233,   234,   383,   236,     0,     0,   237,   238,
     239,   240,   241,     0,     0,     0,   242,   243,     0,   244,
       0,     0,   245,   246,     0,   384,     0,     0,   247,   385,
     248,   249,   386,   387,   388,   389,   250,     0,   251,   252,
     390,   253,     0,     0,     0,   254,     0,     0,   255,   391,
     256,     0,     0,     0,     0,   257,     0,   258,   259,   260,
     392,     0,     0,   393,   262,   394,   263,   395,     0,     0,
     265,   266,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,   269,   270,     0,   271,     0,   272,
     273,   396,   397,     0,     0,     0,   398,     0,     0,     0,
       0,     0,   276,   399,     0,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,     0,     0,     0,
       0,   412,   413,   414,   415,     0,   416,   417,   278,   279,
     418,   419,   420,   421,   422,     0,   423,   424,   425,   426,
     427,   428,   429,   430,     0,     0,     0,   431,   283,   432,
     284,   433,   434,   435,   436,   437,   438,   439,   440,   441,
       0,     0,   442,   288,   289,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
       0,   458,   459,   460,   461,   462,     0,   299,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,     0,   302,     0,   480,   481,
     482,   304,     0,     0,   305,     0,   306,     0,   307,   308,
     309,     0,     0,     0,   483,   484,     0,     0,     0,     0,
     485,     0,   486,     0,     0,   487,   488,     0,   489,   490,
     491,    99,   100,   101,   102,   103,     0,     0,     0,   104,
       0,     0,     0,     0,     0,     0,   351,     0,     0,     0,
       0,     0,   105,     0,     0,   106,   352,   108,   109,     0,
     110,   111,   112,   113,   353,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,   120,   354,   121,     0,
       0,   122,     0,   123,   355,   356,   124,     0,     0,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,   357,   358,   135,   359,   360,   137,   361,     0,   139,
       0,   140,   141,     0,     0,     0,     0,   142,   143,   144,
     145,   362,   146,   147,   148,     0,   149,     0,   150,   151,
       0,   363,   364,   153,     0,   154,     0,   365,     0,     0,
     156,     0,   366,   367,     0,   157,   368,     0,   158,     0,
       0,   159,   160,   161,     0,   162,   163,   164,     0,     0,
     165,   166,   369,   167,   370,   168,   169,   170,   371,     0,
       0,  1490,   171,     0,   172,     0,   173,   372,     0,     0,
     174,   373,     0,   175,   176,   177,     0,   178,     0,   179,
       0,     0,   180,     0,     0,   181,     0,     0,     0,     0,
     374,   182,   183,     0,     0,     0,   184,   375,   185,   376,
       0,     0,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   377,   202,
     203,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,   378,   213,   379,   214,   380,   381,   215,     0,   216,
     217,     0,     0,     0,     0,     0,     0,     0,   218,   219,
     220,     0,     0,   221,   222,   223,   224,   225,   226,   227,
     228,     0,   382,     0,     0,   229,     0,   230,     0,   231,
     232,     0,     0,   233,   234,   383,   236,     0,     0,   237,
     238,   239,   240,   241,     0,     0,     0,   242,   243,     0,
     244,     0,     0,   245,   246,     0,   384,     0,     0,   247,
     385,   248,   249,   386,   387,   388,   389,   250,     0,   251,
     252,   390,   253,     0,     0,     0,   254,     0,     0,   255,
     391,   256,     0,     0,     0,     0,   257,     0,   258,   259,
     260,   392,     0,     0,   393,   262,   394,   263,   395,     0,
       0,   265,   266,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,   269,   270,     0,   271,     0,
     272,   273,   396,   397,     0,     0,     0,   398,     0,     0,
       0,     0,     0,   276,   399,     0,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,     0,     0,
       0,     0,   412,   413,   414,   415,     0,   416,   417,   278,
     279,   418,   419,   420,   421,   422,     0,   423,   424,   425,
     426,   427,   428,   429,   430,     0,     0,     0,   431,   283,
     432,   284,   433,   434,   435,   436,   437,   438,   439,   440,
     441,     0,     0,   442,   288,   289,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,     0,   458,   459,   460,   461,   462,     0,   299,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,     0,   302,     0,   480,
     481,   482,   304,     0,     0,   305,     0,   306,     0,   307,
     308,   309,     0,     0,     0,   483,   484,     0,     0,     0,
       0,   485,     0,   486,     0,     0,   487,   488,     0,   489,
     490,   491,    99,   100,   101,   102,   103,     0,     0,     0,
     104,     0,     0,     0,     0,     0,     0,   351,     0,     0,
       0,     0,     0,   105,     0,     0,   106,   352,   108,   109,
       0,   110,   111,   112,   113,   353,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,   120,   354,   121,
       0,     0,   122,     0,   123,   355,   356,   124,     0,     0,
     125,   126,   127,   128,   129,   130,     0,     0,   131,   132,
     133,   134,   357,   358,   135,   359,   360,   137,   361,     0,
     139,     0,   140,   141,     0,     0,     0,     0,   142,   143,
     144,   145,   362,   146,   147,   148,     0,   149,     0,   150,
     151,     0,   363,   364,   153,     0,   154,     0,   365,     0,
       0,   156,     0,   366,   367,     0,   157,   368,     0,   158,
       0,     0,   159,   160,   161,     0,   162,   163,   164,     0,
       0,   165,   166,   369,   167,   370,   168,   169,   170,   371,
       0,     0,     0,   171,     0,   172,     0,   173,   372,     0,
       0,   174,   373,     0,   175,   176,   177,     0,   178,     0,
     179,     0,     0,   180,     0,     0,   181,     0,     0,     0,
       0,   374,   182,   183,     0,     0,     0,   184,   375,   185,
     376,     0,     0,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   377,
     202,   203,   204,   205,   206,   207,   208,   209,     0,   210,
     211,   212,   378,   213,   379,   214,   380,   381,   215,     0,
     216,   217,     0,     0,     0,     0,     0,     0,     0,   218,
     219,   220,     0,     0,   221,   222,   223,   224,   225,   226,
     227,   228,     0,   382,     0,     0,   229,     0,   230,     0,
     231,   232,     0,     0,   233,   234,   383,   236,     0,     0,
     237,   238,   239,   240,   241,     0,     0,     0,   242,   243,
       0,   244,     0,     0,   245,   246,     0,   384,     0,     0,
     247,   385,   248,   249,   386,   387,   388,   389,   250,     0,
     251,   252,   390,   253,     0,     0,     0,   254,     0,     0,
     255,   391,   256,     0,     0,     0,     0,   257,     0,   258,
     259,   260,   392,     0,     0,   393,   262,   394,   263,   395,
       0,     0,   265,   266,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,   269,   270,     0,   271,
       0,   272,   273,   396,   397,     0,     0,     0,   398,     0,
       0,     0,     0,     0,   276,   399,     0,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,     0,
       0,     0,     0,   412,   413,   414,   415,     0,   416,   417,
     278,   279,   418,   419,   420,   421,   422,     0,   423,   424,
     425,   426,   427,   428,   429,   430,     0,     0,     0,   431,
     283,   432,   284,   433,   434,   435,   436,   437,   438,   439,
     440,   441,     0,     0,   442,   288,   289,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,     0,   458,   459,   460,   461,   462,     0,   299,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,     0,   302,     0,
     480,   481,   482,   304,     0,     0,   305,     0,   306,     0,
     307,   308,   309,     0,     0,     0,   483,   484,     0,     0,
       0,     0,   485,     0,   486,     0,     0,   487,   488,     0,
     489,   490,   491,    99,   100,   101,   102,   103,     0,     0,
       0,   104,     0,     0,     0,     0,     0,     0,   351,     0,
       0,     0,     0,     0,   105,     0,     0,   106,   352,   108,
     109,     0,   110,   111,   112,   113,   353,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,   120,   354,
     121,     0,     0,   122,     0,   123,   355,   356,   124,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,   131,
     132,   133,   134,   357,   358,   135,   359,   360,   137,   361,
       0,   139,     0,   140,   141,     0,     0,     0,     0,   142,
     143,   144,   145,   362,   146,   147,   148,     0,   149,     0,
     150,   151,     0,   363,   364,   153,     0,   154,     0,   365,
       0,     0,   156,     0,   366,   367,     0,   157,   368,     0,
     158,     0,     0,   159,   160,   161,     0,   162,   163,   164,
       0,     0,   165,   166,   369,   167,   370,   168,   169,   170,
     371,     0,     0,     0,   171,     0,   172,     0,   173,   372,
       0,     0,   174,   373,     0,   175,   176,   177,     0,   178,
       0,   179,     0,     0,   180,     0,     0,   181,     0,     0,
       0,     0,   374,   182,   183,     0,     0,     0,   184,   375,
     185,   376,     0,     0,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     377,   202,   203,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   378,   213,   379,   214,   380,   381,   215,
       0,   216,   217,     0,     0,     0,     0,     0,     0,     0,
     218,   219,   220,     0,     0,   221,   222,   223,   224,   225,
     226,   227,   228,     0,   382,     0,     0,   229,     0,   230,
       0,   231,   232,     0,     0,   233,   234,   383,   236,     0,
       0,   237,   238,   239,   240,   241,     0,     0,     0,   242,
     243,     0,   244,     0,     0,   245,   246,     0,   384,     0,
       0,   247,   385,   248,   249,   386,   387,   388,   389,   250,
       0,   251,   252,   390,   253,     0,     0,     0,   254,     0,
       0,   255,   391,   256,     0,     0,     0,     0,   257,     0,
     258,   259,   260,   392,     0,     0,   393,   262,   394,   263,
     395,     0,     0,   265,   266,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,   269,   270,     0,
     271,     0,   272,   273,   396,   397,     0,     0,     0,   398,
       0,     0,     0,     0,     0,   276,   399,     0,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
       0,     0,     0,     0,   412,   413,   414,   415,     0,   416,
     417,   278,   279,   418,   419,   420,   421,   422,     0,   423,
     424,   425,   426,   427,   428,   429,   430,     0,     0,     0,
     431,   283,   432,   284,  1770,   434,   435,   436,   437,   438,
     439,   440,   441,     0,     0,   442,   288,   289,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,     0,   458,   459,   460,   461,   462,     0,
     299,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,     0,   302,
       0,   480,   481,   482,   304,     0,     0,   305,     0,   306,
       0,   307,   308,   309,     0,     0,     0,   483,   484,     0,
       0,     0,     0,   485,     0,   486,     0,     0,   487,   488,
       0,   489,   490,   491,    99,   100,   101,   102,   103,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,   351,
       0,     0,     0,     0,     0,   105,     0,     0,   106,   352,
     108,   109,     0,   110,   111,   112,   113,   353,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,   120,
     354,   121,     0,     0,   122,     0,   123,   355,   356,   124,
       0,     0,   125,   126,   127,   128,   129,   130,     0,     0,
     131,   132,   133,   134,   357,   358,   135,   359,   360,   137,
     361,     0,   139,     0,   140,   141,     0,     0,     0,     0,
     142,   143,   144,   145,   362,   146,   147,   148,     0,   149,
       0,   150,   151,     0,   363,   364,   153,     0,   154,     0,
     365,     0,     0,   156,     0,   366,   367,     0,   157,   368,
       0,   158,     0,     0,   159,   160,   161,     0,   162,   163,
     164,     0,     0,   165,   166,   369,   167,   370,   168,   169,
     170,   371,     0,     0,     0,   171,     0,   172,     0,   173,
     372,     0,     0,   174,   373,     0,   175,   176,   177,     0,
     178,     0,   179,     0,     0,   180,     0,     0,   181,     0,
       0,     0,     0,   374,   182,   183,     0,     0,     0,   184,
     375,   185,   376,     0,     0,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   377,   202,   203,   204,   205,   206,   207,   208,   209,
       0,   210,   211,   212,   378,   213,   379,   214,   380,   381,
     215,     0,   216,   217,     0,     0,     0,     0,     0,     0,
       0,   218,   219,   220,     0,     0,   221,   222,   223,   224,
     225,   226,   227,   228,     0,   382,     0,     0,   229,     0,
     230,     0,   231,   232,     0,     0,   233,   234,   383,   236,
       0,     0,   237,   238,   239,   240,   241,     0,     0,     0,
     242,   243,     0,   244,     0,     0,   245,   246,     0,   384,
       0,     0,   247,   385,   248,   249,   386,   387,   388,   389,
     250,     0,   251,   252,   390,   253,     0,     0,     0,   254,
       0,     0,   255,   391,   256,     0,     0,     0,     0,   257,
       0,   258,   259,   260,   392,     0,     0,   393,   262,   394,
     263,   395,     0,     0,   265,   266,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,   269,   270,
       0,   271,     0,   272,   273,   396,   397,     0,     0,     0,
     398,     0,     0,     0,     0,     0,   276,   399,     0,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,     0,     0,     0,     0,   412,   413,   414,   415,     0,
     416,   417,   278,   279,   418,   419,   420,   421,   422,     0,
     423,   424,   425,   426,   427,   428,   429,   430,     0,     0,
       0,   431,   283,   432,   284,  1867,   434,   435,   436,   437,
     438,   439,   440,   441,     0,     0,   442,   288,   289,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,     0,   458,   459,   460,   461,   462,
       0,   299,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,     0,
     302,     0,   480,   481,   482,   304,     0,     0,   305,     0,
     306,     0,   307,   308,   309,     0,     0,     0,   483,   484,
       0,     0,     0,     0,   485,     0,   486,     0,     0,   487,
     488,     0,   489,   490,   491,    99,   100,   101,   102,   103,
       0,     0,     0,   104,     0,     0,     0,     0,     0,     0,
     351,     0,     0,     0,     0,     0,   105,     0,     0,   106,
     352,   108,   109,     0,   110,   111,   112,   113,   353,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
     120,   354,   121,     0,     0,   122,     0,   123,   355,   356,
     124,     0,     0,   125,   126,   127,   128,   129,   130,     0,
       0,   131,   132,   133,   134,   357,   358,   135,   359,   360,
     137,   361,     0,   139,     0,   140,   141,     0,     0,     0,
       0,   142,   143,   144,   145,   362,   146,   147,   148,     0,
     149,     0,   150,   151,     0,   363,   364,   153,     0,   154,
       0,   365,     0,     0,   156,     0,   366,   367,     0,   157,
    2551,     0,   158,     0,     0,   159,   160,   161,     0,   162,
     163,   164,     0,     0,   165,   166,   369,   167,   370,   168,
     169,   170,   371,     0,     0,     0,   171,     0,   172,     0,
     173,   372,     0,     0,   174,   373,     0,   175,   176,   177,
       0,   178,     0,   179,     0,     0,   180,     0,     0,   181,
       0,     0,     0,     0,   374,   182,   183,     0,     0,     0,
     184,   375,   185,   376,     0,     0,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   377,   202,   203,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   378,   213,   379,   214,   380,
     381,   215,     0,   216,   217,     0,     0,     0,     0,     0,
       0,     0,   218,   219,   220,     0,     0,   221,   222,   223,
     224,   225,   226,   227,   228,     0,   382,     0,     0,   229,
       0,   230,     0,   231,   232,     0,     0,   233,   234,   383,
     236,     0,     0,   237,   238,   239,   240,   241,     0,     0,
       0,   242,   243,     0,   244,     0,     0,   245,   246,     0,
     384,     0,     0,   247,   385,   248,   249,   386,   387,   388,
     389,   250,     0,   251,   252,   390,   253,     0,     0,     0,
     254,     0,     0,   255,   391,   256,     0,     0,     0,     0,
     257,     0,   258,   259,   260,   392,     0,     0,   393,   262,
     394,   263,   395,     0,     0,   265,   266,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,   269,
     270,     0,   271,     0,   272,   273,   396,   397,     0,     0,
       0,   398,     0,     0,     0,     0,     0,   276,   399,     0,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,     0,     0,     0,     0,   412,   413,   414,   415,
       0,   416,   417,   278,   279,   418,   419,   420,   421,   422,
       0,   423,   424,   425,   426,   427,   428,   429,   430,     0,
       0,     0,   431,   283,   432,   284,   433,   434,   435,   436,
     437,   438,   439,   440,   441,     0,     0,   442,   288,   289,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,     0,   458,   459,   460,   461,
     462,     0,   299,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
       0,   302,     0,   480,   481,   482,   304,     0,     0,   305,
       0,   306,     0,   307,   308,   309,     0,     0,     0,   483,
     484,     0,     0,     0,     0,   485,     0,   486,     0,     0,
     487,   488,     0,   489,   490,   491,    99,   100,   101,   102,
     103,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,     0,
     106,   107,   108,   109,     0,   110,   111,   112,   113,     0,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,   120,     0,   121,     0,     0,   122,     0,   123,     0,
       0,   124,     0,     0,   125,   126,   127,   128,   129,   130,
       0,     0,   131,   132,   133,   134,     0,     0,   135,     0,
     136,   137,   138,     0,   139,     0,   140,   141,     0,     0,
       0,     0,   142,   143,   144,   145,     0,   146,   147,   148,
       0,   149,     0,   150,   151,     0,     0,   152,   153,     0,
     154,     0,   155,     0,     0,   156,  2041,     0,     0,     0,
     157,     0,     0,   158,     0,     0,   159,   160,   161,     0,
     162,   163,   164,     0,     0,   165,   166,     0,   167,     0,
     168,   169,   170,     0,     0,     0,     0,   171,  2042,   172,
       0,   173,     0,     0,     0,   174,     0,     0,   175,   176,
     177,     0,   178,  1612,   179,     0,     0,   180,     0,     0,
     181,     0,     0,  1613,     0,     0,   182,   183,     0,     0,
       0,   184,     0,   185,     0,     0,     0,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,     0,   210,   211,   212,     0,   213,     0,   214,
       0,     0,   215,     0,   216,   217,     0,     0,     0,     0,
       0,     0,     0,   218,   219,   220,     0,     0,   221,   222,
     223,   224,   225,   226,   227,   228,     0,     0,     0,     0,
     229,     0,   230,     0,   231,   232,     0,     0,   233,   234,
     235,   236,     0,     0,   237,   238,   239,   240,   241,  2043,
       0,     0,   242,   243,     0,   244,     0,     0,   245,   246,
       0,     0,     0,     0,   247,     0,   248,   249,     0,     0,
       0,     0,   250,     0,   251,   252,     0,   253,     0,     0,
       0,   254,     0,     0,   255,     0,   256,     0,     0,     0,
       0,   257,     0,   258,   259,   260,   261,     0,     0,     0,
     262,     0,   263,   264,     0,     0,   265,   266,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
     269,   270,     0,   271,     0,   272,   273,   274,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,   278,   279,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   280,     0,     0,   281,
       0,     0,     0,   282,   283,     0,   284,     0,     0,     0,
       0,   285,     0,     0,     0,   286,     0,     0,   287,   288,
     289,   290,     0,     0,     0,   291,   292,   293,     0,   294,
     295,     0,   296,     0,   297,     0,     0,     0,     0,     0,
       0,   298,     0,   299,   300,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,    99,   100,   101,
     102,   103,   302,     0,   303,   104,     0,   304,     0,     0,
     305,     0,   306,     0,   307,   308,   309,     0,   105,     0,
       0,   106,   107,   108,   109,     0,   110,   111,   112,   113,
       0,     0,     0,   114,     0,  2088,   115,     0,   116,   117,
     118,   119,   120,     0,   121,     0,     0,   122,     0,   123,
       0,     0,   124,     0,     0,   125,   126,   127,   128,   129,
     130,  2023,     0,   131,   132,   133,   134,     0,     0,   135,
       0,   136,   137,   138,     0,   139,     0,   140,   141,     0,
       0,     0,     0,   142,   143,   144,   145,     0,   146,   147,
     148,     0,   149,     0,   150,   151,     0,     0,   152,   153,
       0,   154,     0,   155,     0,     0,   156,     0,     0,     0,
       0,   157,     0,     0,   158,     0,     0,   159,   160,   161,
       0,   162,   163,   164,     0,     0,   165,   166,     0,   167,
       0,   168,   169,   170,     0,     0,     0,     0,   171,     0,
     172,     0,   173,     0,     0,     0,   174,     0,     0,   175,
     176,   177,     0,   178,     0,   179,     0,     0,   180,     0,
       0,   181,     0,     0,     0,     0,     0,   182,   183,     0,
       0,     0,   184,     0,   185,     0,     0,     0,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,     0,   213,     0,
     214,     0,     0,   215,     0,   216,   217,     0,     0,     0,
       0,     0,     0,     0,   218,   219,   220,     0,     0,   221,
     222,   223,   224,   225,   226,   227,   228,     0,     0,     0,
       0,   229,     0,   230,     0,   231,   232,     0,     0,   233,
     234,   235,   236,     0,     0,   237,   238,   239,   240,   241,
       0,     0,     0,   242,   243,     0,   244,     0,     0,   245,
     246,     0,     0,     0,     0,   247,     0,   248,   249,     0,
       0,     0,     0,   250,     0,   251,   252,     0,   253,     0,
       0,     0,   254,     0,     0,   255,     0,   256,     0,     0,
       0,     0,   257,     0,   258,   259,   260,   261,     0,     0,
       0,   262,     0,   263,   264,     0,     0,   265,   266,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,   269,   270,     0,   271,     0,   272,   273,   274,   275,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,   278,   279,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   280,     0,     0,
     281,     0,     0,     0,   282,   283,     0,   284,     0,     0,
       0,     0,   285,     0,     0,     0,   286,     0,     0,   287,
     288,   289,   290,     0,     0,     0,   291,   292,   293,     0,
     294,   295,     0,   296,     0,   297,     0,     0,     0,     0,
       0,     0,   298,     0,   299,   300,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,     0,
       0,     0,     0,   302,     0,   303,     0,     0,   304,     0,
       0,   305,     0,   306,     0,   307,   308,   309,    99,   100,
     101,   102,   103,     0,     0,     0,   104,     0,     0,     0,
    2339,     0,     0,     0,     0,     0,   490,     0,     0,   105,
       0,     0,   106,   107,   108,   109,     0,   110,   111,   112,
     113,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,   120,     0,   121,     0,     0,   122,     0,
     123,     0,     0,   124,     0,     0,   125,   126,   127,   128,
     129,   130,     0,     0,   131,   132,   133,   134,     0,     0,
     135,     0,   136,   137,   138,     0,   139,     0,   140,   141,
       0,     0,     0,     0,   142,   143,   144,   145,     0,   146,
     147,   148,     0,   149,     0,   150,   151,     0,     0,   152,
     153,     0,   154,     0,   155,     0,     0,   156,     0,     0,
       0,     0,   157,     0,     0,   158,     0,     0,   159,   160,
     161,     0,   162,   163,   164,     0,     0,   165,   166,     0,
     167,     0,   168,   169,   170,     0,     0,     0,     0,   171,
       0,   172,     0,   173,     0,     0,     0,   174,     0,     0,
     175,   176,   177,     0,   178,     0,   179,     0,     0,   180,
    1264,     0,   181,     0,     0,     0,     0,     0,   182,   183,
       0,     0,     0,   184,     0,   185,     0,     0,     0,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,     0,   210,   211,   212,     0,   213,
       0,   214,     0,     0,   215,     0,   216,   217,     0,     0,
       0,     0,     0,     0,     0,   218,   219,   220,     0,     0,
     221,   222,   223,   224,   225,   226,   227,   228,     0,     0,
       0,     0,   229,     0,   230,     0,   231,   232,     0,     0,
     233,   234,   235,   236,     0,     0,   237,   238,   239,   240,
     241,     0,     0,     0,   242,   243,     0,   244,     0,     0,
     245,   246,     0,     0,     0,     0,   247,     0,   248,   249,
       0,     0,     0,     0,   250,     0,   251,   252,     0,   253,
       0,     0,     0,   254,     0,     0,   255,     0,   256,     0,
       0,     0,     0,   257,     0,   258,   259,   260,   261,     0,
       0,     0,   262,     0,   263,   264,     0,     0,   265,   266,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,   269,   270,     0,   271,     0,   272,   273,   274,
     275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,   278,   279,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   280,     0,
       0,   281,     0,     0,     0,   282,   283,     0,   284,     0,
       0,     0,     0,   285,     0,     0,     0,   286,     0,     0,
     287,   288,   289,   290,     0,     0,     0,   291,   292,   293,
       0,   294,   295,     0,   296,     0,   297,     0,     0,     0,
       0,     0,     0,   298,     0,   299,   300,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   301,    99,
     100,   101,   102,   103,   302,     0,   303,   104,     0,   304,
       0,     0,   305,     0,   306,     0,   307,   308,   309,     0,
     105,     0,     0,   106,   107,   108,   109,     0,   110,   111,
     112,   113,     0,     0,     0,   114,     0,   310,   115,     0,
     116,   117,   118,   119,   120,     0,   121,     0,     0,   122,
       0,   123,     0,     0,   124,     0,     0,   125,   126,   127,
     128,   129,   130,     0,     0,   131,   132,   133,   134,     0,
       0,   135,     0,   136,   137,   138,     0,   139,     0,   140,
     141,     0,     0,     0,     0,   142,   143,   144,   145,     0,
     146,   147,   148,     0,   149,     0,   150,   151,     0,     0,
     152,   153,     0,   154,     0,   155,     0,     0,   156,     0,
       0,     0,     0,   157,     0,     0,   158,     0,     0,   159,
     160,   161,     0,   162,   163,   164,     0,     0,   165,   166,
       0,   167,     0,   168,   169,   170,     0,     0,     0,     0,
     171,     0,   172,     0,   173,     0,     0,     0,   174,     0,
       0,   175,   176,   177,     0,   178,     0,   179,     0,     0,
     180,     0,     0,   181,     0,     0,     0,     0,     0,   182,
     183,     0,     0,     0,   184,     0,   185,     0,     0,     0,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,     0,   202,   203,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,     0,
     213,     0,   214,     0,     0,   215,     0,   216,   217,     0,
       0,     0,     0,     0,     0,     0,   218,   219,   220,     0,
       0,   221,   222,   223,   224,   225,   226,   227,   228,     0,
       0,     0,     0,   229,     0,   230,     0,   231,   232,     0,
       0,   233,   234,   235,   236,     0,     0,   237,   238,   239,
     240,   241,     0,     0,     0,   242,   243,     0,   244,     0,
       0,   245,   246,     0,     0,     0,     0,   247,     0,   248,
     249,     0,     0,     0,     0,   250,     0,   251,   252,     0,
     253,     0,     0,     0,   254,     0,     0,   255,     0,   256,
       0,     0,     0,     0,   257,     0,   258,   259,   260,   261,
       0,     0,     0,   262,     0,   263,   264,     0,     0,   265,
     266,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,   269,   270,     0,   271,     0,   272,   273,
     274,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,   278,   279,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   280,
       0,     0,   281,     0,     0,     0,   282,   283,     0,   284,
       0,     0,     0,     0,   285,     0,     0,     0,   286,     0,
       0,   287,   288,   289,   290,     0,     0,     0,   291,   292,
     293,     0,   294,   295,     0,   296,     0,   297,     0,     0,
       0,     0,     0,     0,   298,     0,   299,   300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   301,
       0,     0,     0,     0,     0,   302,     0,   303,     0,     0,
     304,     0,     0,   305,     0,   306,     0,   307,   308,   309,
      99,   100,   101,   102,   103,  1634,     0,     0,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   310,     0,
       0,   105,     0,     0,   106,   107,   108,   109,     0,   110,
     111,   112,   113,     0,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,   120,     0,   121,     0,     0,
     122,     0,   123,     0,     0,   124,     0,     0,   125,   126,
     127,   128,   129,   130,     0,     0,   131,   132,   133,   134,
       0,     0,   135,     0,   136,   137,   138,     0,   139,     0,
     140,   141,     0,     0,     0,     0,   142,   143,   144,   145,
       0,   146,   147,   148,     0,   149,     0,   150,   151,     0,
       0,   152,   153,     0,   154,     0,   155,     0,     0,   156,
       0,     0,     0,     0,   157,     0,     0,   158,     0,     0,
     159,   160,   161,     0,   162,   163,   164,     0,     0,   165,
     166,     0,   167,     0,   168,   169,   170,     0,     0,     0,
       0,   171,     0,   172,     0,   173,     0,     0,     0,   174,
       0,     0,   175,   176,   177,     0,   178,     0,   179,     0,
       0,   180,     0,     0,   181,     0,     0,     0,     0,     0,
     182,   183,     0,     0,     0,   184,     0,   185,     0,     0,
       0,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,     0,   210,   211,   212,
       0,   213,     0,   214,     0,     0,   215,     0,   216,   217,
       0,     0,     0,     0,     0,     0,     0,   218,   219,   220,
       0,     0,   221,   222,   223,   224,   225,   226,   227,   228,
       0,     0,     0,     0,   229,     0,   230,     0,   231,   232,
       0,     0,   233,   234,   235,   236,     0,     0,   237,   238,
     239,   240,   241,     0,     0,     0,   242,   243,     0,   244,
       0,     0,   245,   246,     0,     0,     0,     0,   247,     0,
     248,   249,     0,     0,     0,     0,   250,     0,   251,   252,
       0,   253,     0,     0,     0,   254,     0,     0,   255,     0,
     256,     0,     0,     0,     0,   257,     0,   258,   259,   260,
     261,     0,     0,     0,   262,     0,   263,   264,     0,     0,
     265,   266,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,   269,   270,     0,   271,     0,   272,
     273,   274,   275,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,   278,   279,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     280,     0,     0,   281,     0,     0,     0,   282,   283,     0,
     284,     0,     0,     0,     0,   285,     0,     0,     0,   286,
       0,     0,   287,   288,   289,   290,     0,     0,     0,   291,
     292,   293,     0,   294,   295,     0,   296,     0,   297,     0,
       0,     0,     0,     0,     0,   298,     0,   299,   300,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,     0,     0,     0,     0,     0,   302,     0,   303,     0,
       0,   304,     0,     0,   305,     0,   306,     0,   307,   308,
     309,    99,   100,   101,   102,   103,     0,     0,     0,   104,
       0,     0,  2087,     0,     0,     0,     0,     0,     0,  2088,
       0,     0,   105,     0,     0,   106,   107,   108,   109,     0,
     110,   111,   112,   113,     0,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,   120,     0,   121,     0,
       0,   122,     0,   123,     0,     0,   124,     0,     0,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,     0,     0,   135,     0,   136,   137,   138,     0,   139,
       0,   140,   141,     0,     0,     0,     0,   142,   143,   144,
     145,     0,   146,   147,   148,     0,   149,     0,   150,   151,
       0,     0,   152,   153,     0,   154,     0,   155,     0,     0,
     156,     0,     0,     0,     0,   157,     0,     0,   158,     0,
       0,   159,   160,   161,     0,   162,   163,   164,     0,     0,
     165,   166,     0,   167,     0,   168,   169,   170,     0,     0,
       0,     0,   171,     0,   172,     0,   173,     0,     0,     0,
     174,     0,     0,   175,   176,   177,     0,   178,     0,   179,
       0,     0,   180,     0,     0,   181,     0,     0,     0,     0,
       0,   182,   183,     0,     0,     0,   184,     0,   185,     0,
       0,     0,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,     0,   213,     0,   214,     0,     0,   215,     0,   216,
     217,     0,     0,     0,     0,     0,     0,     0,   218,   219,
     220,     0,     0,   221,   222,   223,   224,   225,   226,   227,
     228,     0,     0,     0,     0,   229,     0,   230,     0,   231,
     232,     0,     0,   233,   234,   235,   236,     0,     0,   237,
     238,   239,   240,   241,     0,     0,     0,   242,   243,     0,
     244,     0,     0,   245,   246,     0,     0,     0,     0,   247,
       0,   248,   249,     0,     0,     0,     0,   250,     0,   251,
     252,     0,   253,     0,     0,     0,   254,     0,     0,   255,
       0,   256,     0,     0,     0,     0,   257,     0,   258,   259,
     260,   261,     0,     0,     0,   262,     0,   263,   264,     0,
       0,   265,   266,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,   269,   270,     0,   271,     0,
     272,   273,   274,   275,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,   278,
     279,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   280,     0,     0,   281,     0,     0,     0,   282,   283,
       0,   284,     0,     0,     0,     0,   285,     0,     0,     0,
     286,     0,     0,   287,   288,   289,   290,     0,     0,     0,
     291,   292,   293,     0,   294,   295,     0,   296,     0,   297,
       0,     0,     0,     0,     0,     0,   298,     0,   299,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,   302,     0,   303,
       0,     0,   304,     0,     0,   305,     0,   306,     0,   307,
     308,   309,    99,   100,   101,   102,   103,     0,     0,     0,
     104,     0,     0,     0,  3089,     0,     0,     0,     0,     0,
     490,     0,     0,   105,     0,     0,   106,   107,   108,   109,
       0,   110,   111,   112,   113,     0,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,   120,     0,   121,
       0,     0,   122,     0,   123,     0,     0,   124,     0,     0,
     125,   126,   127,   128,   129,   130,     0,     0,   131,   132,
     133,   134,     0,     0,   135,     0,   136,   137,   138,     0,
     139,     0,   140,   141,     0,     0,     0,     0,   142,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,     0,     0,   152,   153,     0,   154,     0,   155,     0,
       0,   156,     0,     0,     0,     0,   157,     0,     0,   158,
       0,     0,   159,   160,   161,     0,   162,   163,   164,     0,
       0,   165,   166,     0,   167,     0,   168,   169,   170,     0,
       0,     0,     0,   171,     0,   172,     0,   173,     0,     0,
       0,   174,     0,     0,   175,   176,   177,     0,   178,     0,
     179,     0,     0,   180,     0,     0,   181,     0,     0,     0,
       0,     0,   182,   183,     0,     0,     0,   184,     0,   185,
       0,     0,     0,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,     0,   210,
     211,   212,     0,   213,     0,   214,     0,     0,   215,     0,
     216,   217,     0,     0,     0,     0,     0,     0,     0,   218,
     219,   220,     0,     0,   221,   222,   223,   224,   225,   226,
     227,   228,     0,     0,     0,     0,   229,     0,   230,     0,
     231,   232,     0,     0,   233,   234,   235,   236,     0,     0,
     237,   238,   239,   240,   241,     0,     0,     0,   242,   243,
       0,   244,     0,     0,   245,   246,     0,     0,     0,     0,
     247,     0,   248,   249,     0,     0,     0,     0,   250,     0,
     251,   252,     0,   253,     0,     0,     0,   254,     0,     0,
     255,     0,   256,     0,     0,     0,     0,   257,     0,   258,
     259,   260,   261,     0,     0,     0,   262,     0,   263,   264,
       0,     0,   265,   266,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,   269,   270,     0,   271,
       0,   272,   273,   274,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,     0,     0,     0,     0,     0,     0,
     278,   279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   280,     0,     0,   281,     0,     0,     0,   282,
     283,     0,   284,     0,     0,     0,     0,   285,     0,     0,
       0,   286,     0,     0,   287,   288,   289,   290,     0,     0,
       0,   291,   292,   293,     0,   294,   295,     0,   296,     0,
     297,     0,     0,     0,     0,     0,     0,   298,     0,   299,
     300,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   301,    99,   100,   101,   102,   103,   302,     0,
     303,   104,     0,   304,     0,     0,   305,     0,   306,     0,
     307,   308,   309,     0,   105,     0,     0,   106,   107,   108,
     109,     0,   110,   111,   112,   113,     0,     0,     0,   114,
       0,   310,   115,     0,   116,   117,   118,   119,   120,     0,
     121,     0,     0,   122,     0,   123,     0,     0,   124,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,   131,
     132,   133,   134,     0,     0,   135,     0,   136,   137,   138,
       0,   139,     0,   140,   141,     0,     0,     0,     0,   142,
     143,   144,   145,     0,   146,   147,   148,     0,   149,     0,
     150,   151,     0,     0,   152,   153,     0,   154,     0,   155,
       0,     0,   156,     0,     0,     0,     0,   157,     0,     0,
     158,     0,     0,   159,   160,   161,     0,   162,   163,   164,
       0,     0,   165,   166,     0,   551,     0,   168,   169,   170,
       0,     0,     0,     0,   171,     0,   172,     0,   173,     0,
       0,     0,   174,     0,     0,   175,   176,   177,     0,   178,
       0,   179,     0,     0,   180,     0,     0,   181,     0,     0,
       0,     0,     0,   182,   183,     0,     0,     0,   184,     0,
     185,     0,     0,     0,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,     0,   213,     0,   214,     0,     0,   215,
       0,   216,   217,     0,     0,     0,     0,     0,     0,     0,
     218,   219,   220,     0,     0,   221,   222,   223,   224,   225,
     226,   227,   228,     0,     0,     0,     0,   229,     0,   230,
       0,   231,   232,     0,     0,   233,   234,   235,   236,     0,
       0,   237,   238,   239,   240,   241,     0,     0,     0,   242,
     243,     0,   244,     0,     0,   245,   246,     0,     0,     0,
       0,   247,     0,   248,   249,     0,     0,     0,     0,   250,
       0,   251,   252,     0,   253,     0,     0,     0,   254,     0,
       0,   255,     0,   256,     0,     0,     0,     0,   257,     0,
     258,   259,   260,   261,     0,     0,     0,   262,     0,   263,
     264,     0,     0,   265,   266,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,   269,   270,     0,
     271,     0,   272,   273,   274,   275,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,     0,
       0,   278,   279,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   280,     0,     0,   281,     0,     0,     0,
     282,   283,     0,   284,     0,     0,     0,     0,   285,     0,
       0,     0,   286,     0,     0,   287,   288,   289,   290,     0,
       0,     0,   291,   292,   293,     0,   294,   295,     0,   296,
       0,   297,     0,     0,     0,     0,     0,     0,   298,     0,
     299,   300,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,    99,   100,   101,   102,   103,   302,
       0,   303,   104,     0,   304,     0,     0,   305,     0,   306,
       0,   307,   308,   309,     0,   105,     0,     0,   106,   107,
     108,   109,     0,   110,   111,   112,   113,     0,     0,     0,
     114,     0,   310,   115,     0,   116,   117,   118,   119,   120,
       0,   121,     0,     0,   122,     0,   123,     0,     0,   124,
       0,     0,   125,   126,   127,   128,   129,   130,     0,     0,
     131,   132,   133,   134,     0,     0,   135,     0,   136,   137,
     138,     0,   139,     0,   140,   141,     0,     0,     0,     0,
     142,   143,   144,   145,     0,   146,   147,   148,     0,   149,
       0,   150,   151,     0,     0,   152,   153,     0,   154,     0,
     155,     0,     0,   156,     0,     0,     0,     0,   157,     0,
       0,   158,     0,     0,   159,   160,   161,     0,   162,   163,
     164,     0,     0,   165,   166,     0,   167,     0,   168,   169,
     170,     0,     0,     0,     0,   171,     0,   172,     0,   173,
       0,     0,     0,   174,     0,     0,   175,   176,   177,     0,
     178,     0,   179,     0,     0,   180,     0,     0,   181,     0,
       0,     0,     0,     0,   182,   183,     0,     0,     0,   184,
       0,   185,     0,     0,     0,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
       0,   210,   211,   212,     0,   213,     0,   214,     0,     0,
     215,     0,   216,   217,     0,     0,     0,     0,     0,     0,
       0,   218,   219,   220,     0,     0,   221,   222,   223,   224,
     225,   226,   227,   228,     0,     0,     0,     0,   229,     0,
     230,     0,   231,   232,     0,     0,   233,   234,   235,   236,
       0,     0,   237,   238,   239,   240,   241,     0,     0,     0,
     242,   243,     0,   244,     0,     0,   245,   246,     0,     0,
       0,     0,   247,     0,   248,   249,     0,     0,     0,     0,
     250,     0,   251,   252,     0,   253,     0,     0,     0,   254,
       0,     0,   255,     0,   256,     0,     0,     0,     0,   257,
       0,   258,   259,   260,   261,     0,     0,     0,   262,     0,
     263,   264,     0,     0,   265,   266,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,   269,   270,
       0,   271,     0,   272,   273,   274,   275,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,     0,     0,     0,
       0,     0,   278,   279,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   280,     0,     0,   281,     0,     0,
       0,   282,   283,     0,   284,     0,     0,     0,     0,   285,
       0,     0,     0,   286,     0,     0,   287,   288,   289,   290,
       0,     0,     0,   291,   292,   293,     0,   294,   295,     0,
     296,     0,   297,     0,     0,     0,     0,     0,     0,   298,
       0,   299,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,    99,   100,   101,   102,   103,
     302,     0,   303,   104,     0,   304,     0,     0,   305,     0,
     306,     0,   307,   308,   309,     0,   105,     0,     0,   106,
     107,   108,   109,     0,   110,   111,   112,   113,     0,     0,
       0,   114,     0,   490,   115,     0,   116,   117,   118,   119,
     120,     0,   121,     0,     0,   122,     0,   123,     0,     0,
     124,     0,     0,   125,   126,   127,   128,   129,   130,     0,
       0,   131,   132,   133,   134,     0,     0,   135,     0,   136,
     137,   138,     0,   139,     0,   140,   141,     0,     0,     0,
       0,   142,   143,   144,   145,     0,   146,   147,   148,     0,
     149,     0,   150,   151,     0,     0,   152,   153,     0,   154,
       0,   155,     0,     0,   156,     0,     0,     0,     0,   157,
       0,     0,   158,     0,     0,   159,   160,   161,     0,   162,
     163,   164,     0,     0,   165,   166,     0,   167,     0,   168,
     169,   170,     0,     0,     0,     0,   171,     0,   172,     0,
     173,     0,     0,     0,   174,     0,     0,   175,   176,   177,
       0,   178,     0,   179,     0,     0,   180,     0,     0,   181,
       0,     0,     0,     0,     0,   182,   183,     0,     0,     0,
     184,     0,   185,     0,     0,     0,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,     0,   213,     0,   214,     0,
       0,   215,     0,   216,   217,     0,     0,     0,     0,     0,
       0,     0,   218,   219,   220,     0,     0,   221,   222,   223,
     224,   225,   226,   227,   228,     0,     0,     0,     0,   229,
       0,   230,     0,   231,   232,     0,     0,   233,   234,   235,
     236,     0,     0,   237,   238,   239,   240,   241,     0,     0,
       0,   242,   243,     0,   244,     0,     0,   245,   246,     0,
       0,     0,     0,   247,     0,   248,   249,     0,     0,     0,
       0,   250,     0,   251,   252,     0,   253,     0,     0,     0,
     254,     0,     0,   255,     0,   256,     0,     0,     0,     0,
     257,     0,   258,   259,   260,   261,     0,     0,     0,   262,
       0,   263,   264,     0,     0,   265,   266,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,   269,
     270,     0,   271,     0,   272,   273,   274,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,     0,     0,   278,   279,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   280,     0,     0,   281,     0,
       0,     0,   282,   283,     0,   284,     0,     0,     0,     0,
     285,     0,     0,     0,   286,     0,     0,   287,   288,   289,
     290,     0,     0,     0,   291,   292,   293,     0,   294,   295,
       0,   296,     0,   297,     0,     0,     0,     0,     0,     0,
     298,     0,   299,   300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   301,    99,   100,   101,   102,
     103,   302,     0,   303,   104,     0,   304,     0,     0,   305,
       0,   306,     0,   307,   308,   309,     0,   105,     0,     0,
     106,   107,   108,   109,     0,   110,   111,   112,   113,     0,
       0,     0,   114,     0,  2088,   115,     0,   116,   117,   118,
     119,   120,     0,   121,     0,     0,   122,     0,   123,     0,
       0,   124,     0,     0,   125,   126,   127,   128,   129,   130,
       0,     0,   131,   132,   133,   134,     0,     0,   135,     0,
     136,   137,   138,     0,   139,     0,   140,   141,     0,     0,
       0,     0,   142,   143,   144,   145,     0,   146,   147,   148,
       0,   149,     0,   150,   151,     0,     0,   152,   153,     0,
     154,     0,   155,     0,     0,   156,     0,     0,     0,     0,
     157,     0,     0,   158,     0,     0,   159,   160,   161,     0,
     162,   163,   164,     0,     0,   165,   166,     0,   167,     0,
     168,   169,   170,     0,     0,     0,     0,   171,     0,   172,
       0,   173,     0,     0,     0,   174,     0,     0,   175,   176,
     177,     0,   178,     0,   179,     0,     0,   180,     0,     0,
     181,     0,     0,     0,     0,     0,   182,   183,   345,     0,
       0,   184,     0,   185,     0,     0,     0,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,     0,   210,   211,   212,     0,   213,     0,   214,
       0,     0,   215,     0,   216,   217,     0,     0,     0,     0,
       0,     0,     0,   218,   219,   220,     0,     0,   221,   222,
     223,   224,   225,   226,   227,   228,     0,     0,     0,     0,
     229,     0,   230,     0,   231,   232,     0,     0,   233,   234,
     235,   236,     0,     0,   237,   238,   239,   240,   241,     0,
       0,     0,   242,   243,     0,   244,     0,     0,   245,   246,
       0,   346,     0,     0,   247,     0,   248,   249,     0,     0,
       0,     0,   250,     0,   251,   252,     0,   253,     0,     0,
       0,   254,     0,     0,   255,     0,   256,     0,     0,     0,
       0,   257,     0,   258,   259,   260,   261,     0,     0,     0,
     262,     0,   263,   264,     0,     0,   265,   266,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
     269,   270,     0,   271,     0,   272,   273,   274,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,   278,   279,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   280,     0,     0,   281,
       0,     0,     0,   282,   283,     0,   284,     0,     0,     0,
       0,   285,     0,     0,     0,   286,     0,     0,   287,   288,
     289,   290,     0,     0,     0,   291,   292,   293,     0,   294,
     295,     0,   296,     0,   297,     0,     0,     0,     0,     0,
       0,   298,     0,   299,   300,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,    99,   100,   101,
     102,   103,   302,     0,   303,   104,     0,   304,     0,     0,
     305,     0,   306,     0,   307,   308,   309,     0,   105,     0,
       0,   106,   107,   108,   109,     0,   110,   111,   112,   113,
       0,     0,     0,   114,   734,     0,   115,     0,   116,   117,
     118,   119,   120,     0,   121,     0,     0,   122,     0,   123,
       0,     0,   124,     0,     0,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,     0,     0,   135,
       0,   136,   137,   138,     0,   139,     0,   140,   141,     0,
       0,     0,     0,   142,   143,   144,   145,     0,   146,   147,
     148,     0,   149,     0,   150,   151,     0,     0,   152,   153,
       0,   154,     0,   155,     0,     0,   156,     0,     0,     0,
       0,   157,     0,     0,   158,     0,     0,   159,   160,   161,
       0,   162,   163,   164,     0,     0,   165,   166,     0,   167,
       0,   168,   169,   170,     0,     0,     0,     0,   171,     0,
     172,     0,   173,     0,     0,     0,   174,     0,     0,   175,
     176,   177,     0,   178,     0,   179,     0,     0,   180,     0,
       0,   181,     0,     0,     0,     0,     0,   182,   183,   345,
       0,     0,   184,     0,   185,     0,     0,     0,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,     0,   213,     0,
     214,     0,     0,   215,     0,   216,   217,     0,     0,     0,
       0,     0,     0,     0,   218,   219,   220,     0,     0,   221,
     222,   223,   224,   225,   226,   227,   228,     0,     0,     0,
       0,   229,     0,   230,     0,   231,   232,     0,     0,   233,
     234,   235,   236,     0,     0,   237,   238,   239,   240,   241,
       0,     0,     0,   242,   243,     0,   244,     0,     0,   245,
     246,     0,   346,     0,     0,   247,     0,   248,   249,     0,
       0,     0,     0,   250,     0,   251,   252,     0,   253,     0,
       0,     0,   254,     0,     0,   255,     0,   256,     0,     0,
       0,     0,   257,     0,   258,   259,   260,   261,     0,     0,
       0,   262,     0,   263,   264,     0,     0,   265,   266,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,   269,   270,     0,   271,     0,   272,   273,   274,   275,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,   278,   279,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   280,     0,     0,
     281,     0,     0,     0,   282,   283,     0,   284,     0,     0,
       0,     0,   285,     0,     0,     0,   286,     0,     0,   287,
     288,   289,   290,     0,     0,     0,   291,   292,   293,     0,
     294,   295,     0,   296,     0,   297,     0,     0,   750,     0,
       0,     0,   298,     0,   299,   300,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,     0,
       0,     0,     0,   302,     0,   303,     0,     0,   304,     0,
       0,   305,     0,   306,     0,   307,   308,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  2016,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,     0,     0,     0,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,     0,     0,     0,     0,   753,     0,     0,
       0,     0,     0,     0,   750,     0,     0,   754,   755,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,     0,     0,   756,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,     0,
       0,     0,     0,     0,   751,     0,     0,     0,     0,   750,
       0,   757,     0,     0,   752,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,   753,     0,   751,     0,     0,     0,   758,
       0,   750,     0,   754,   755,   752,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   756,     0,     0,     0,     0,   751,
       0,   753,     0,     0,     0,     0,     0,     0,     0,   752,
       0,   754,   755,     0,   753,     0,     0,   751,     0,     0,
       0,     0,     0,     0,   754,   755,     0,   752,     0,     0,
       0,     0,   756,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   756,     0,     0,   753,     0,
       0,   751,     0,     0,     0,     0,     0,   757,   754,   755,
       0,   752,     0,     0,     0,     0,   753,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   754,   755,     0,   756,
       0,     0,     0,     0,     0,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,   757,     0,   756,  1410,  1411,
     753,     0,     0,     0,     0,   758,     0,     0,   757,     0,
     754,   755,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   757,   758,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   758,     0,     0,     0,
     757,     0,     0,     0,     0,     0,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     758,     0,     0,     0,   757,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   758,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   759,   760,   761,   762,   763,   764,   765,
     766,     0,     0,     0,  1412,  1413,     0,     0,     0,     0,
       0,     0,   758,     0,   750,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
       0,   759,   760,   761,   762,   763,   764,   765,   766,     0,
       0,     0,  1414,  1415,   759,   760,   761,   762,   763,   764,
     765,   766,     0,     0,     0,  1424,  1425,     0,   750,     0,
       0,     0,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,     0,     0,     0,  1433,
    1434,     0,     0,   750,   751,     0,   759,   760,   761,   762,
     763,   764,   765,   766,   752,     0,     0,  1435,  1436,     0,
       0,     0,     0,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
     759,   760,   761,   762,   763,   764,   765,   766,   751,     0,
     750,  1437,  1438,   753,     0,     0,     0,     0,   752,     0,
       0,     0,     0,   754,   755,     0,     0,     0,     0,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   751,   756,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,     0,     0,   753,     0,     0,
       0,     0,     0,   750,     0,     0,     0,   754,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   756,     0,
     751,     0,   753,     0,     0,     0,     0,     0,     0,     0,
     752,     0,   754,   755,     0,     0,     0,   757,   750,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1483,     0,     0,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   753,
       0,     0,     0,   751,     0,     0,     0,     0,     0,   754,
     755,   757,     0,   752,     0,   758,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     756,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   757,     0,   751,     0,
       0,     0,   753,     0,     0,     0,     0,     0,   752,   758,
       0,     0,   754,   755,     0,     0,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   757,   758,     0,     0,   753,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   754,   755,     0,
       0,     0,     0,     0,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,     0,   756,     0,
       0,     0,     0,   759,   760,   761,   762,   763,   764,   765,
     766,   758,     0,     0,  1445,  1446,   757,   750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,     0,     0,     0,  1448,  1449,
       0,   757,   750,     0,   758,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,     0,  1479,  1480,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,     0,   758,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,     0,     0,     0,
    1502,  1503,   751,     0,     0,     0,   753,     0,     0,     0,
       0,     0,   752,     0,     0,     0,   754,   755,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,     0,     0,     0,     0,     0,   756,     0,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
       0,   753,     0,     0,  1484,     0,     0,     0,     0,     0,
       0,   754,   755,     0,     0,     0,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,   756,     0,     0,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,     0,   750,     0,  1505,  1506,
     757,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   757,     0,   750,   758,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,     0,     0,     0,
       0,     0,     0,   758,     0,     0,     0,     0,     0,   750,
       0,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   753,     0,   751,     0,     0,
       0,     0,     0,     0,     0,   754,   755,   752,     0,     0,
     750,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   756,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,   753,  2191,  2192,   751,
       0,   750,     0,     0,     0,     0,   754,   755,     0,   752,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   756,     0,     0,
       0,   759,   760,   761,   762,   763,   764,   765,   766,   757,
     751,     0,  2194,  2195,     0,     0,     0,     0,   753,     0,
     752,     0,     0,     0,     0,     0,     0,     0,   754,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   756,
       0,   751,     0,     0,     0,     0,     0,   758,     0,   753,
     757,   752,     0,     0,     0,     0,     0,     0,     0,   754,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     756,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     753,     0,     0,     0,     0,     0,     0,     0,   758,     0,
     754,   755,   757,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,     0,
       0,     0,     0,   757,     0,     0,     0,     0,     0,     0,
     758,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   759,   760,   761,   762,   763,
     764,   765,   766,     0,     0,     0,  2220,  2221,     0,     0,
       0,     0,     0,     0,   757,     0,     0,     0,     0,     0,
       0,   758,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   750,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,     0,  2651,  2652,     0,
       0,     0,   758,     0,     0,     0,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,     0,     0,     0,  2915,
    2916,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,   750,     0,     0,
       0,  1341,     0,     0,     0,     0,     0,     0,     0,     0,
     753,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     754,   755,     0,     0,     0,     0,     0,     0,     0,     0,
     759,   760,   761,   762,   763,   764,   765,   766,     0,     0,
       0,   756,  1347,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,     0,     0,   757,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,   753,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   754,   755,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,   758,     0,     0,     0,     0,   756,     0,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,     0,   750,     0,     0,     0,
     757,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,     0,     0,     0,     0,   751,     0,     0,     0,
       0,     0,   753,     0,     0,     0,   752,     0,     0,     0,
       0,     0,   754,   755,     0,     0,     0,   750,   758,     0,
     759,   760,   761,   762,   763,   764,   765,   766,     0,     0,
       0,     0,  1353,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   753,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,   756,     0,     0,   752,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   753,   757,   751,     0,     0,
       0,     0,     0,     0,     0,   754,   755,   752,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   753,     0,
       0,     0,     0,     0,     0,     0,   756,     0,   754,   755,
       0,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,   758,     0,   753,     0,  1354,   756,
       0,     0,     0,     0,     0,     0,   754,   755,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,     0,     0,     0,     0,     0,   756,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   758,     0,   757,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   757,     0,     0,     0,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,     0,     0,     0,   750,   758,     0,     0,
     757,     0,     0,     0,     0,     0,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
     758,     0,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,     0,     0,  1362,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,   758,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,   759,   760,   761,   762,   763,
     764,   765,   766,     0,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,   750,     0,   752,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,   759,   760,   761,   762,   763,
     764,   765,   766,     0,     0,   753,     0,  1363,     0,     0,
       0,     0,     0,     0,     0,   754,   755,   751,   759,   760,
     761,   762,   763,   764,   765,   766,     0,   752,     0,     0,
    1369,     0,     0,     0,     0,     0,   756,     0,     0,     0,
       0,     0,     0,     0,   751,     0,   759,   760,   761,   762,
     763,   764,   765,   766,   752,     0,     0,     0,  1371,     0,
       0,     0,     0,     0,     0,     0,   753,     0,     0,     0,
       0,     0,     0,   751,     0,     0,   754,   755,     0,     0,
       0,     0,     0,   752,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   753,     0,     0,     0,   756,     0,   757,
       0,     0,     0,   754,   755,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
       0,     0,   753,     0,   756,     0,     0,     0,     0,     0,
       0,     0,   754,   755,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   758,     0,     0,
       0,     0,     0,   756,     0,     0,     0,     0,     0,     0,
     757,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,   757,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   758,     0,
       0,     0,   750,     0,     0,     0,   757,     0,     0,     0,
       0,     0,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,   758,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   751,   758,   759,   760,   761,   762,   763,
     764,   765,   766,   752,     0,     0,     0,  1372,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   751,     0,     0,     0,     0,     0,     0,     0,
     750,     0,   752,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   753,     0,     0,     0,     0,   750,     0,     0,
       0,     0,   754,   755,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,     0,     0,  1389,     0,
       0,   753,     0,   756,     0,     0,     0,     0,     0,     0,
       0,   754,   755,   759,   760,   761,   762,   763,   764,   765,
     766,     0,     0,     0,     0,  1390,     0,     0,     0,     0,
       0,     0,   756,     0,     0,     0,     0,     0,     0,     0,
     751,     0,   759,   760,   761,   762,   763,   764,   765,   766,
     752,     0,     0,     0,  1392,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,   757,   752,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   753,
       0,     0,     0,     0,     0,   757,     0,     0,     0,   754,
     755,     0,     0,     0,     0,     0,   753,     0,     0,     0,
       0,     0,     0,     0,   758,     0,   754,   755,     0,     0,
     756,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,     0,   756,     0,     0,
       0,     0,     0,   758,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,   757,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   750,
     757,     0,     0,     0,     0,     0,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   758,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,     0,     0,  1393,   750,     0,     0,   758,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   759,   760,   761,   762,   763,   764,   765,   766,     0,
       0,     0,     0,  1395,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,   750,     0,     0,     0,   752,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,   753,     0,
       0,     0,     0,     0,     0,   752,     0,     0,   754,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,     0,     0,   756,
       0,  1400,     0,     0,     0,   751,   759,   760,   761,   762,
     763,   764,   765,   766,   753,   752,     0,     0,  1401,     0,
       0,   750,     0,     0,   754,   755,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,     0,     0,   756,     0,     0,     0,     0,
       0,     0,     0,     0,   753,     0,     0,     0,     0,     0,
       0,     0,   757,     0,   754,   755,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   756,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,   751,     0,     0,     0,     0,     0,   750,   757,     0,
     758,   752,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,     0,
     753,     0,     0,     0,     0,     0,   758,     0,     0,     0,
     754,   755,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,   758,   752,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,   753,     0,     0,     0,
    1404,     0,     0,     0,   757,   751,   754,   755,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   756,     0,     0,
       0,     0,     0,     0,   759,   760,   761,   762,   763,   764,
     765,   766,     0,   751,     0,     0,  1406,     0,     0,     0,
       0,   750,   758,   752,   753,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,   755,     0,     0,     0,     0,
       0,     0,     0,     0,   759,   760,   761,   762,   763,   764,
     765,   766,     0,     0,     0,   756,  1407,     0,     0,     0,
     757,     0,   753,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,   755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   756,     0,     0,     0,     0,     0,     0,
       0,   751,     0,     0,     0,     0,     0,     0,   758,     0,
       0,   752,     0,     0,     0,     0,     0,     0,   757,     0,
       0,     0,     0,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
     759,   760,   761,   762,   763,   764,   765,   766,     0,     0,
     753,     0,  1421,     0,     0,     0,   757,     0,     0,     0,
     754,   755,     0,     0,     0,     0,   758,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,     0,     0,     0,     0,
       0,     0,     0,   750,   758,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,     0,     0,  1423,     0,
       0,     0,     0,     0,   757,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     750,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   751,   759,   760,   761,   762,   763,   764,
     765,   766,   758,   752,     0,     0,  1427,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,   753,     0,  1428,     0,     0,     0,     0,   750,
     751,     0,   754,   755,     0,     0,     0,     0,     0,     0,
     752,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   756,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     759,   760,   761,   762,   763,   764,   765,   766,     0,   751,
     756,     0,  1429,     0,     0,     0,   757,     0,     0,   752,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,   753,     0,
       0,     0,     0,     0,   758,     0,     0,     0,   754,   755,
       0,     0,     0,   757,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   756,
       0,     0,     0,     0,     0,     0,   753,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   754,   755,     0,     0,
       0,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,   758,   752,     0,     0,     0,     0,   756,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   757,     0,     0,     0,     0,     0,     0,     0,
       0,   753,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   754,   755,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
     757,     0,   756,     0,  1447,     0,     0,     0,     0,     0,
     758,     0,     0,     0,     0,     0,   750,     0,     0,     0,
       0,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   758,   759,
     760,   761,   762,   763,   764,   765,   766,     0,     0,     0,
       0,  1477,     0,     0,     0,   757,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   751,     0,     0,     0,
       0,   750,     0,     0,     0,     0,   752,     0,     0,     0,
       0,     0,     0,   758,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,     0,   759,   760,
     761,   762,   763,   764,   765,   766,     0,     0,     0,     0,
    1478,     0,     0,     0,     0,   753,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   754,   755,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,   756,     0,  1482,     0,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     753,   759,   760,   761,   762,   763,   764,   765,   766,   757,
     754,   755,     0,  1485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,     0,   750,     0,     0,   758,     0,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,     0,     0,     0,
       0,   753,     0,     0,   757,     0,     0,     0,     0,     0,
       0,   754,   755,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   756,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   751,     0,     0,     0,     0,   750,
       0,     0,   758,     0,   752,     0,     0,     0,     0,     0,
       0,     0,   750,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   759,   760,   761,   762,   763,
     764,   765,   766,   753,     0,   757,     0,  2138,     0,     0,
       0,     0,     0,   754,   755,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   756,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,   750,   751,   758,     0,     0,     0,     0,     0,     0,
       0,     0,   752,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     759,   760,   761,   762,   763,   764,   765,   766,   753,     0,
       0,     0,  2159,     0,     0,     0,     0,   757,   754,   755,
       0,   753,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   754,   755,     0,     0,     0,     0,     0,     0,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   751,   756,     0,     0,     0,     0,     0,     0,     0,
       0,   752,     0,     0,     0,   758,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   759,   760,   761,   762,   763,   764,   765,   766,     0,
     753,     0,   757,  2167,     0,     0,     0,     0,     0,     0,
     754,   755,     0,     0,     0,   757,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,     0,     0,     0,     0,
     758,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   758,     0,     0,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
       0,     0,     0,   759,   760,   761,   762,   763,   764,   765,
     766,     0,     0,     0,   757,  2176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     750,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,     0,     0,     0,
       0,     0,   758,     0,   750,     0,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,     0,     0,     0,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,     0,     0,     0,     0,
    2188,   759,   760,   761,   762,   763,   764,   765,   766,   750,
     751,     0,     0,  2213,     0,     0,     0,     0,     0,     0,
     752,  1492,   750,     0,     0,     0,     0,     0,     0,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,   751,   750,     0,     0,     0,     0,
       0,     0,     0,     0,   752,     0,     0,     0,     0,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     759,   760,   761,   762,   763,   764,   765,   766,     0,   751,
     756,     0,  2222,   753,     0,     0,     0,     0,     0,   752,
       0,     0,   751,   754,   755,     0,     0,     0,     0,   750,
       0,     0,   752,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   756,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,   753,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,   755,
       0,   753,     0,   757,     0,     0,     0,     0,     0,     0,
       0,   754,   755,     0,     0,     0,     0,     0,     0,   756,
       0,     0,     0,     0,   753,     0,     0,     0,     0,     0,
       0,     0,   756,     0,   754,   755,     0,   757,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,   758,     0,     0,     0,   756,     0,     0,     0,     0,
       0,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,     0,
       0,     0,   757,     0,     0,   758,     0,     0,   753,     0,
       0,     0,     0,     0,     0,   757,     0,     0,   754,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     758,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   750,     0,   758,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  2218,     0,     0,     0,     0,   758,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,     0,     0,     0,
       0,  2642,   757,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   759,   760,   761,   762,   763,   764,   765,
     766,     0,     0,     0,  1493,     0,     0,     0,     0,     0,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
     758,   752,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,     0,     0,     0,  2219,
       0,   759,   760,   761,   762,   763,   764,   765,   766,     0,
     753,     0,  1356,     0,     0,     0,     0,     0,     0,     0,
     754,   755,     0,     0,   759,   760,   761,   762,   763,   764,
     765,   766,     0,     0,     0,  1357,     0,     0,     0,     0,
       0,   756,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,     0,     0,     0,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,   759,   760,
     761,   762,   763,   764,   765,   766,     0,     0,     0,  1370,
       0,     0,     0,     0,   757,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     750,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   758,   750,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,     0,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,     0,     0,     0,     0,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,     0,   750,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,     0,   750,     0,     0,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
     755,     0,     0,     0,     0,   750,     0,     0,   753,     0,
     759,   760,   761,   762,   763,   764,   765,   766,   754,   755,
     756,  1374,   753,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,   755,     0,     0,     0,     0,     0,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   751,   756,     0,     0,     0,     0,     0,     0,
       0,     0,   752,     0,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,     0,     0,     0,
       0,     0,     0,   757,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,     0,     0,
       0,   753,   757,     0,     0,     0,     0,     0,     0,     0,
       0,   754,   755,     0,     0,   753,   757,     0,     0,     0,
       0,     0,     0,     0,     0,   754,   755,     0,     0,     0,
       0,   758,   756,     0,   753,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,   755,   756,     0,     0,     0,
     758,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   758,   756,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   757,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   757,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   758,   750,     0,     0,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,   758,     0,     0,
    1377,     0,     0,     0,     0,     0,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,   758,     0,     0,  1380,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,     0,  1391,     0,     0,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,     0,     0,     0,     0,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,     0,     0,     0,     0,     0,     0,     0,
       0,   759,   760,   761,   762,   763,   764,   765,   766,     0,
       0,     0,  1394,   753,   750,   759,   760,   761,   762,   763,
     764,   765,   766,   754,   755,     0,  1420,     0,   750,     0,
       0,     0,     0,     0,   759,   760,   761,   762,   763,   764,
     765,   766,     0,     0,   756,  1422,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,     0,     0,   757,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,     0,     0,     0,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,     0,   751,     0,
       0,     0,     0,   753,     0,     0,     0,     0,   752,     0,
       0,     0,     0,   754,   755,   758,     0,   753,   750,     0,
       0,     0,     0,     0,     0,     0,     0,   754,   755,     0,
       0,     0,     0,     0,   756,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   753,   756,     0,
       0,     0,     0,     0,     0,     0,     0,   754,   755,     0,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   756,     0,
       0,     0,     0,     0,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   757,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,   757,     0,     0,     0,     0,     0,     0,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,     0,   759,   760,   761,   762,   763,   764,   765,
     766,   757,   751,     0,  1430,   758,     0,   753,     0,     0,
       0,     0,   752,     0,     0,     0,     0,   754,   755,   758,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   756,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   758,
       0,   753,     0,     0,     0,     0,   750,     0,     0,     0,
       0,   754,   755,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   756,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
       0,   757,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   759,   760,   761,   762,   763,   764,   765,
     766,     0,     0,     0,  1431,     0,   751,   759,   760,   761,
     762,   763,   764,   765,   766,   757,   752,     0,  1432,   758,
       0,   753,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   754,   755,   750,     0,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,     0,     0,     0,  1442,     0,
       0,     0,   756,     0,     0,   753,     0,     0,     0,     0,
       0,     0,     0,   758,     0,   754,   755,     0,     0,     0,
       0,     0,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,   756,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   751,     0,   757,     0,     0,     0,     0,
       0,     0,     0,   752,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,     0,     0,     0,  1443,   757,
     750,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   753,   758,     0,     0,     0,   750,     0,     0,
       0,     0,   754,   755,     0,     0,     0,     0,     0,     0,
       0,   759,   760,   761,   762,   763,   764,   765,   766,     0,
       0,     0,  1444,   756,     0,     0,     0,   758,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,   757,   752,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,     0,     0,   753,
       0,   759,   760,   761,   762,   763,   764,   765,   766,   754,
     755,     0,  1476,     0,     0,     0,   753,     0,     0,     0,
       0,     0,     0,     0,   758,     0,   754,   755,     0,     0,
     756,     0,     0,     0,     0,   759,   760,   761,   762,   763,
     764,   765,   766,     0,     0,     0,  1481,   756,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,     0,     0,     0,
       0,     0,     0,   757,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     757,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   753,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   754,   755,     0,     0,     0,
       0,   758,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,     0,  1501,     0,     0,   756,     0,   758,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,     0,     0,     0,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,   757,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   750,     0,     0,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,   758,     0,   750,
    1504,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,   750,     0,     0,  2136,     0,     0,
       0,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,     0,     0,
       0,     0,     0,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,     0,     0,     0,   751,     0,   753,     0,     0,     0,
       0,   750,     0,     0,   752,     0,   754,   755,     0,     0,
       0,     0,     0,   753,     0,   759,   760,   761,   762,   763,
     764,   765,   766,   754,   755,     0,  2140,   756,   753,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,   755,
       0,     0,     0,   753,   756,     0,     0,     0,     0,     0,
       0,     0,     0,   754,   755,     0,     0,     0,     0,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   756,     0,     0,     0,     0,     0,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
     757,   752,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   757,     0,     0,     0,     0,     0,     0,     0,
     753,     0,     0,     0,     0,     0,     0,   757,   758,     0,
     754,   755,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   758,     0,     0,     0,     0,
       0,   756,     0,     0,     0,     0,     0,     0,     0,     0,
     758,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,   758,     0,     0,     0,     0,
       0,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,     0,   757,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,     0,  2141,     0,     0,
       0,   750,   758,   759,   760,   761,   762,   763,   764,   765,
     766,     0,     0,     0,  2153,   750,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,     0,     0,     0,  2158,
       0,     0,     0,   759,   760,   761,   762,   763,   764,   765,
     766,     0,     0,     0,  2160,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,   753,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,   755,
     759,   760,   761,   762,   763,   764,   765,   766,     0,     0,
     753,  2161,     0,     0,     0,     0,     0,     0,     0,   756,
     754,   755,     0,     0,   753,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,   755,     0,     0,     0,     0,
       0,   756,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   756,     0,     0,     0,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,     0,     0,     0,     0,
       0,     0,   757,     0,     0,     0,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,     0,   757,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     758,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   758,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   758,   750,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,     0,     0,     0,     0,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,   750,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,   751,   759,   760,
     761,   762,   763,   764,   765,   766,     0,   752,     0,  2171,
       0,     0,     0,     0,     0,   750,     0,     0,   753,     0,
     759,   760,   761,   762,   763,   764,   765,   766,   754,   755,
       0,  2172,     0,   750,   759,   760,   761,   762,   763,   764,
     765,   766,     0,     0,     0,  2175,   753,     0,     0,   756,
       0,     0,     0,     0,     0,     0,   754,   755,     0,     0,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,     0,     0,     0,     0,   751,   756,     0,     0,
       0,     0,     0,     0,     0,     0,   752,     0,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,     0,     0,
     753,     0,   757,   751,     0,     0,     0,     0,     0,     0,
     754,   755,     0,   752,     0,   753,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   754,   755,     0,     0,     0,
     757,   756,     0,     0,   753,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,   755,   756,     0,     0,     0,
     758,     0,   753,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,   755,     0,   756,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   758,     0,
       0,     0,     0,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   757,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   757,
       0,     0,     0,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,   757,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   758,     0,     0,     0,   757,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   758,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,   758,     0,     0,  2177,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   750,   758,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,     0,  2180,     0,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,     0,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,     0,     0,     0,
       0,     0,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,     0,     0,   750,     0,
     759,   760,   761,   762,   763,   764,   765,   766,     0,     0,
       0,  2181,     0,   751,     0,   759,   760,   761,   762,   763,
     764,   765,   766,   752,     0,     0,  2182,     0,   750,     0,
       0,     0,     0,     0,   759,   760,   761,   762,   763,   764,
     765,   766,   750,     0,     0,  2183,     0,     0,     0,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
     750,     0,   753,  2187,     0,     0,     0,     0,     0,     0,
       0,     0,   754,   755,     0,     0,     0,     0,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,     0,     0,   756,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,     0,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,     0,   751,     0,     0,     0,     0,   753,     0,     0,
       0,     0,   752,     0,     0,     0,     0,   754,   755,     0,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,     0,     0,     0,     0,     0,   757,   753,   756,     0,
       0,     0,     0,     0,     0,     0,     0,   754,   755,     0,
       0,   753,   750,     0,     0,     0,     0,     0,     0,     0,
       0,   754,   755,     0,     0,     0,     0,     0,   756,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
     755,     0,   756,     0,   758,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     756,   757,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   757,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,     0,     0,   757,     0,     0,     0,   758,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   757,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   758,
       0,   753,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   754,   755,   758,     0,     0,     0,     0,     0,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
       0,   758,   756,  2189,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
       0,     0,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,     0,     0,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,   757,     0,     0,  2190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,     0,     0,     0,  2196,     0,
     750,   759,   760,   761,   762,   763,   764,   765,   766,     0,
       0,     0,  2197,   758,     0,   750,     0,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,     0,     0,     0,
    2198,     0,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,     0,     0,     0,     0,     0,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,     0,     0,
     750,     0,   753,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,   755,     0,     0,     0,   750,     0,   753,
       0,   759,   760,   761,   762,   763,   764,   765,   766,   754,
     755,     0,  2199,   756,   753,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,   755,     0,     0,     0,     0,
     756,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   756,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,     0,     0,     0,     0,     0,   757,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,     0,     0,
       0,     0,     0,   757,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
     755,     0,     0,     0,   758,     0,   753,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   754,   755,     0,     0,
     756,   758,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   758,   756,     0,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,     0,     0,     0,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   757,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     757,   750,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
       0,   758,     0,  2200,     0,   750,     0,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,     0,   758,     0,
    2201,     0,     0,     0,   759,   760,   761,   762,   763,   764,
     765,   766,     0,     0,     0,  2204,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,     0,     0,     0,     0,     0,     0,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,     0,     0,
     753,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     754,   755,     0,     0,     0,   750,     0,   753,     0,   759,
     760,   761,   762,   763,   764,   765,   766,   754,   755,     0,
    2214,   756,     0,     0,   753,     0,   759,   760,   761,   762,
     763,   764,   765,   766,   754,   755,     0,  2215,   756,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,   756,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   757,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,     0,     0,
       0,   757,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,   750,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   758,     0,   753,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,   755,     0,     0,     0,   758,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,   756,   758,     0,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
     749,     0,     0,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,     0,     0,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,   757,   750,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,     0,   753,     0,
     759,   760,   761,   762,   763,   764,   765,   766,   754,   755,
       0,  2217,     0,   750,     0,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,     0,   758,     0,  2223,   756,
       0,     0,     0,     0,   759,   760,   761,   762,   763,   764,
     765,   766,     0,   750,     0,  2225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,     0,     0,     0,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,     0,     0,     0,
       0,     0,   757,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,     0,     0,     0,   753,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,   755,
       0,     0,     0,   751,     0,   753,     0,     0,     0,     0,
       0,     0,     0,   752,     0,   754,   755,     0,     0,   756,
     758,     0,   753,     0,   759,   760,   761,   762,   763,   764,
     765,   766,   754,   755,     0,  2227,   756,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,   753,   756,     0,     0,     0,     0,     0,     0,
       0,     0,   754,   755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   757,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   757,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   757,   750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     758,     0,     0,     0,     0,     0,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,   757,   758,     0,  2229,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   758,     0,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,     0,   758,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,     0,     0,
       0,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,     0,     0,     0,   750,     0,   753,     0,   759,   760,
     761,   762,   763,   764,   765,   766,   754,   755,   750,  2231,
       0,     0,     0,     0,     0,   759,   760,   761,   762,   763,
     764,   765,   766,     0,     0,     0,  2232,   756,     0,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
       0,   750,     0,  2233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,     0,  2473,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,     0,     0,     0,   751,     0,
     757,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   751,     0,   753,     0,     0,     0,     0,     0,     0,
       0,   752,     0,   754,   755,   751,     0,   753,   758,     0,
       0,     0,     0,     0,     0,   752,     0,   754,   755,     0,
       0,     0,     0,     0,   756,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,   756,     0,
     753,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     754,   755,     0,     0,   753,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,   755,     0,     0,     0,     0,
       0,   756,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,   756,     0,   757,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   757,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,   757,   758,     0,  2474,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,   758,
     750,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,   758,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,     0,   758,     0,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,     0,
     751,     0,   753,     0,     0,     0,     0,     0,     0,     0,
     752,     0,   754,   755,     0,     0,     0,   750,     0,     0,
       0,     0,     0,   759,   760,   761,   762,   763,   764,   765,
     766,     0,     0,   756,  2475,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,     0,     0,   750,  2648,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
     755,   750,     0,     0,     0,     0,     0,     0,     0,     0,
     759,   760,   761,   762,   763,   764,   765,   766,     0,     0,
     756,  2650,     0,     0,   759,   760,   761,   762,   763,   764,
     765,   766,     0,     0,     0,  2656,   757,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,     0,     0,
       0,   751,     0,   757,   758,     0,   753,     0,     0,     0,
       0,   752,     0,     0,     0,     0,   754,   755,     0,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,   749,     0,     0,     0,     0,   753,   756,     0,     0,
       0,     0,     0,     0,     0,     0,   754,   755,     0,     0,
     753,   758,     0,     0,     0,     0,     0,     0,     0,     0,
     754,   755,     0,     0,     0,     0,     0,   756,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,     0,     0,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   750,     0,     0,
     757,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   759,   760,   761,   762,   763,   764,   765,   766,
     757,     0,     0,  2657,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   757,     0,     0,     0,   758,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   750,     0,     0,     0,     0,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,   751,   758,     0,
    2658,     0,     0,     0,     0,     0,     0,   752,     0,     0,
       0,     0,   758,     0,     0,     0,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,   753,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   754,   755,     0,     0,
       0,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,     0,     0,     0,     0,   756,     0,     0,
       0,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,   750,     0,  2660,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,   753,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,     0,  2662,     0,     0,
     759,   760,   761,   762,   763,   764,   765,   766,     0,     0,
     757,  2663,   756,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,   758,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   753,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,   755,     0,     0,   753,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,   755,
       0,     0,     0,   758,     0,   756,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   759,   760,   761,   762,
     763,   764,   765,   766,     0,     0,     0,  2664,   757,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   757,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   758,     0,     0,     0,
       0,   759,   760,   761,   762,   763,   764,   765,   766,     0,
     758,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   759,   760,   761,   762,   763,   764,
     765,   766,     0,     0,     0,  2914,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,     0,     0,     0,  3037,
      99,   100,   101,   102,   103,     0,     0,     0,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,     0,     0,   106,   107,   108,   109,     0,   110,
     111,   112,   113,     0,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,   120,     0,   121,     0,     0,
     122,     0,   123,     0,     0,   124,     0,     0,   125,   126,
     127,   128,   129,   130,     0,     0,   131,   132,   133,   134,
       0,     0,   135,     0,   136,   137,   138,     0,   139,     0,
     140,   141,     0,     0,     0,     0,   142,   143,   144,   145,
       0,   146,   147,   148,     0,   149,     0,   150,   151,     0,
       0,   152,   153,     0,   154,     0,   155,     0,     0,   156,
       0,     0,     0,     0,   157,     0,     0,   158,     0,     0,
     159,   160,   161,     0,   162,   163,   164,     0,     0,   165,
     166,     0,   167,     0,   168,   169,   170,     0,     0,     0,
       0,   171,     0,   172,     0,   173,     0,     0,     0,   174,
       0,     0,   175,   176,   177,     0,   178,     0,   179,     0,
       0,   180,     0,     0,   181,     0,     0,     0,     0,     0,
     182,   183,     0,     0,     0,   184,     0,   185,     0,     0,
       0,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,     0,   210,   211,   212,
       0,   213,     0,   214,     0,     0,   215,     0,   216,   217,
       0,     0,     0,     0,     0,     0,     0,   218,   219,   220,
       0,     0,   221,   222,   223,   224,   225,   226,   227,   228,
       0,     0,     0,     0,   229,     0,   230,     0,   231,   232,
       0,     0,   233,   234,   235,   236,     0,     0,   237,   238,
     239,   240,   241,     0,     0,     0,   242,   243,     0,   244,
       0,     0,   245,   246,     0,     0,     0,     0,   247,     0,
     248,   249,     0,     0,     0,     0,   250,     0,   251,   252,
       0,   253,     0,     0,     0,   254,     0,     0,   255,     0,
     256,     0,     0,     0,     0,   257,     0,   258,   259,   260,
     261,     0,     0,     0,   262,     0,   263,   264,     0,     0,
     265,   266,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,   269,   270,     0,   271,     0,   272,
     273,   274,   275,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,   278,   279,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     280,     0,     0,   281,     0,     0,     0,   282,   283,     0,
     284,     0,     0,     0,     0,   285,     0,     0,     0,   286,
       0,     0,   287,   288,   289,   290,     0,     0,     0,   291,
     292,   293,     0,   294,   295,     0,   296,     0,   297,     0,
       0,     0,     0,     0,     0,   298,     0,   299,   300,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,     0,     0,     0,     0,     0,   302,     0,   303,     0,
       0,   304,     0,     0,   305,     0,   306,     0,   307,   308,
     309,    99,   100,   101,   102,   103,  1747,     0,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,     0,     0,   106,   107,   108,   109,     0,
     110,   111,   112,   113,     0,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,   120,     0,   121,     0,
       0,   122,     0,   123,     0,     0,   124,     0,     0,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,     0,     0,   135,     0,   136,   137,   138,     0,   139,
       0,   140,   141,     0,     0,     0,     0,   142,   143,   144,
     145,     0,   146,   147,   148,     0,   149,     0,   150,   151,
       0,     0,   152,   153,     0,   154,     0,   155,     0,     0,
     156,     0,     0,     0,     0,   157,     0,     0,   158,     0,
       0,   159,   160,   161,     0,   162,   163,   164,     0,     0,
     165,   166,     0,   167,     0,   168,   169,   170,     0,     0,
       0,     0,   171,     0,   172,     0,   173,     0,     0,     0,
     174,     0,     0,   175,   176,   177,     0,   178,     0,   179,
       0,     0,   180,     0,     0,   181,     0,     0,     0,     0,
       0,   182,   183,     0,     0,     0,   184,     0,   185,     0,
       0,     0,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,     0,   213,     0,   214,     0,     0,   215,     0,   216,
     217,     0,     0,     0,     0,     0,     0,     0,   218,   219,
     220,     0,     0,   221,   222,   223,   224,   225,   226,   227,
     228,     0,     0,     0,     0,   229,     0,   230,     0,   231,
     232,     0,     0,   233,   234,   235,   236,     0,     0,   237,
     238,   239,   240,   241,     0,     0,     0,   242,   243,     0,
     244,     0,     0,   245,   246,     0,     0,     0,     0,   247,
       0,   248,   249,     0,     0,     0,     0,   250,     0,   251,
     252,     0,   253,     0,     0,     0,   254,     0,     0,   255,
       0,   256,     0,     0,     0,     0,   257,     0,   258,   259,
     260,   261,     0,     0,     0,   262,     0,   263,   264,     0,
       0,   265,   266,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,   269,   270,     0,   271,     0,
     272,   273,   274,   275,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,   278,
     279,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   280,     0,     0,   281,     0,     0,     0,   282,   283,
       0,   284,     0,     0,     0,     0,   285,     0,     0,     0,
     286,     0,     0,   287,   288,   289,   290,     0,     0,     0,
     291,   292,   293,     0,   294,   295,     0,   296,     0,   297,
       0,     0,     0,     0,     0,     0,   298,     0,   299,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,   302,     0,   303,
       0,     0,   304,     0,     0,   305,     0,   306,     0,   307,
     308,   309,    99,   100,   101,   102,   103,  2273,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,   106,   107,   108,   109,
       0,   110,   111,   112,   113,     0,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,   120,     0,   121,
       0,     0,   122,     0,   123,     0,     0,   124,     0,     0,
     125,   126,   127,   128,   129,   130,     0,     0,   131,   132,
     133,   134,     0,     0,   135,     0,   136,   137,   138,     0,
     139,     0,   140,   141,     0,     0,     0,     0,   142,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,     0,     0,   152,   153,     0,   154,     0,   155,     0,
       0,   156,     0,     0,     0,     0,   157,     0,     0,   158,
       0,     0,   159,   160,   161,     0,   162,   163,   164,     0,
       0,   165,   166,     0,   167,     0,   168,   169,   170,     0,
       0,     0,     0,   171,     0,   172,     0,   173,     0,     0,
       0,   174,     0,     0,   175,   176,   177,     0,   178,     0,
     179,     0,     0,   180,     0,     0,   181,     0,     0,     0,
       0,     0,   182,   183,     0,     0,     0,   184,     0,   185,
       0,     0,     0,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,     0,   210,
     211,   212,     0,   213,     0,   214,     0,     0,   215,     0,
     216,   217,     0,     0,     0,     0,     0,     0,     0,   218,
     219,   220,     0,     0,   221,   222,   223,   224,   225,   226,
     227,   228,     0,     0,     0,     0,   229,     0,   230,     0,
     231,   232,     0,     0,   233,   234,   235,   236,     0,     0,
     237,   238,   239,   240,   241,     0,     0,     0,   242,   243,
       0,   244,     0,     0,   245,   246,     0,     0,     0,     0,
     247,     0,   248,   249,     0,     0,     0,     0,   250,     0,
     251,   252,     0,   253,     0,     0,     0,   254,     0,     0,
     255,     0,   256,     0,     0,     0,     0,   257,     0,   258,
     259,   260,   261,     0,     0,     0,   262,     0,   263,   264,
       0,     0,   265,   266,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,   269,   270,     0,   271,
       0,   272,   273,   274,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,     0,     0,     0,     0,     0,     0,
     278,   279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   280,     0,     0,   281,     0,     0,     0,   282,
     283,     0,   284,     0,     0,     0,     0,   285,     0,     0,
       0,   286,     0,     0,   287,   288,   289,   290,     0,     0,
       0,   291,   292,   293,     0,   294,   295,     0,   296,     0,
     297,     0,     0,     0,     0,     0,     0,   298,     0,   299,
     300,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   301,     0,     0,     0,     0,     0,   302,     0,
     303,     0,     0,   304,     0,     0,   305,     0,   306,     0,
     307,   308,   309,    99,   100,   101,   102,   103,  2311,     0,
       0,   104,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   105,     0,     0,   106,   107,   108,
     109,     0,   110,   111,   112,   113,     0,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,   120,     0,
     121,     0,     0,   122,     0,   123,     0,     0,   124,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,   131,
     132,   133,   134,     0,     0,   135,     0,   136,   137,   138,
       0,   139,     0,   140,   141,     0,     0,     0,     0,   142,
     143,   144,   145,     0,   146,   147,   148,     0,   149,     0,
     150,   151,     0,     0,   152,   153,     0,   154,     0,   155,
       0,     0,   156,     0,     0,     0,     0,   157,     0,     0,
     158,     0,     0,   159,   160,   161,     0,   162,   163,   164,
       0,     0,   165,   166,     0,   167,     0,   168,   169,   170,
       0,     0,     0,     0,   171,     0,   172,     0,   173,     0,
       0,     0,   174,     0,     0,   175,   176,   177,     0,   178,
       0,   179,     0,     0,   180,     0,     0,   181,     0,     0,
       0,     0,     0,   182,   183,     0,     0,     0,   184,     0,
     185,     0,     0,     0,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,     0,   213,     0,   214,     0,     0,   215,
       0,   216,   217,     0,     0,     0,     0,     0,     0,     0,
     218,   219,   220,     0,     0,   221,   222,   223,   224,   225,
     226,   227,   228,     0,     0,     0,     0,   229,     0,   230,
       0,   231,   232,     0,     0,   233,   234,   235,   236,     0,
       0,   237,   238,   239,   240,   241,     0,     0,     0,   242,
     243,     0,   244,     0,     0,   245,   246,     0,     0,     0,
       0,   247,     0,   248,   249,     0,     0,     0,     0,   250,
       0,   251,   252,     0,   253,     0,     0,     0,   254,     0,
       0,   255,     0,   256,     0,     0,     0,     0,   257,     0,
     258,   259,   260,   261,     0,     0,     0,   262,     0,   263,
     264,     0,     0,   265,   266,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,   269,   270,     0,
     271,     0,   272,   273,   274,   275,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   276,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,     0,
       0,   278,   279,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   280,     0,     0,   281,     0,     0,     0,
     282,   283,     0,   284,     0,     0,     0,     0,   285,     0,
       0,     0,   286,     0,     0,   287,   288,   289,   290,     0,
       0,     0,   291,   292,   293,     0,   294,   295,     0,   296,
       0,   297,     0,     0,   750,     0,     0,     0,   298,     0,
     299,   300,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,     0,     0,     0,     0,     0,   302,
       0,   303,     0,     0,   304,     0,     0,   305,     0,   306,
       0,   307,   308,   309,     0,     0,     0,     0,     0,  2676,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,     0,     0,     0,     0,     0,
       0,     0,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   753,     0,     0,     0,     0,   750,  1355,
       0,     0,     0,   754,   755,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,     0,
       0,     0,     0,     0,   756,     0,     0,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,   749,
     750,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,     0,     0,   751,     0,
       0,     0,     0,   750,     0,     0,     0,   757,   752,     0,
       0,     0,     0,     0,     0,     0,     0,  1873,     0,     0,
       0,     0,     0,     0,     0,   750,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   748,   749,     0,
     751,     0,     0,     0,     0,     0,     0,   753,     0,     0,
     752,     0,     0,     0,     0,   758,     0,   754,   755,   750,
    1875,     0,     0,     0,     0,     0,     0,     0,   768,   769,
     770,   771,   772,     0,     0,     0,     0,     0,   756,     0,
       0,     0,  1877,   751,     0,     0,     0,     0,     0,   753,
       0,     0,     0,   752,     0,   773,   774,   775,   776,   754,
     755,     0,     0,     0,   750,   751,     0,     0,     0,     0,
     777,   778,   779,   780,     0,   752,   781,     0,     0,     0,
     756,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   753,     0,     0,     0,   782,   783,     0,  2154,
       0,   757,   754,   755,     0,     0,     0,     0,     0,   752,
       0,     0,     0,     0,   753,     0,     0,     0,   784,   785,
       0,     0,     0,   756,   754,   755,     0,     0,     0,     0,
       0,     0,     0,   759,   760,   761,   762,   763,   764,   765,
     766,     0,     0,   757,   751,   756,     0,     0,   753,   758,
       0,     0,     0,     0,   752,     0,     0,     0,   754,   755,
       0,     0,     0,     0,     0,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,   749,     0,   756,
       0,     0,     0,     0,     0,     0,   757,     0,     0,     0,
       0,   758,     0,   753,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   754,   755,     0,     0,     0,   757,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   756,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   758,     0,     0,     0,  2155,     0,
       0,     0,   757,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   758,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   759,   760,   761,
     762,   763,   764,   765,   766,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,     0,     0,
     758,     0,     0,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,     0,     0,     0,   759,
     760,   761,   762,   763,   764,   765,   766,     0,     0,     0,
       0,     0,     0,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,   758,     0,     0,     0,  1451,
     738,  1452,  1453,   741,   742,   743,   744,   745,  1454,  1455,
    1456,  1457,   759,   760,   761,   762,   763,   764,   765,   766,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   750,   753,     0,   759,   760,   761,   762,   763,   764,
     765,   766,   754,   755,     0,  1528,   738,  1529,  1530,   741,
     742,   743,   744,   745,  1531,  1532,  1533,  1534,     0,     0,
       0,     0,     0,   756,     0,     0,     0,     0,   759,   760,
     761,   762,   763,   764,   765,   766,     0,  1458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  2164,     0,     0,     0,     0,     0,     0,
       0,   751,     0,   759,   760,   761,   762,   763,   764,   765,
     766,   752,     0,  1535,     0,     0,   757,     0,     0,     0,
       0,     0,     0,  1528,   738,  1529,  1530,   741,   742,   743,
     744,   745,  1531,  1532,  1533,  1534,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1459,     0,     0,
     753,     0,     0,     0,     0,     0,     0,  1460,     0,     0,
     754,   755,     0,     0,   758,  1528,   738,  1529,  1530,   741,
     742,   743,   744,   745,  1531,  1532,  1533,  1534,     0,     0,
       0,   756,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1536,     0,     0,  1461,     0,     0,     0,
       0,  1891,     0,  1537,     0,     0,  1462,  1463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1464,     0,  1528,
     738,  1529,  1530,   741,   742,   743,   744,   745,  1531,  1532,
    1533,  1534,  1538,  2209,   757,     0,     0,     0,     0,     0,
       0,     0,  1539,  1540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  2649,  1541,     0,     0,     0,     0,     0,     0,
       0,  1536,   759,   760,   761,   762,   763,   764,   765,   766,
    1465,  1537,   758,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2246,     0,     0,
    1528,   738,  1529,  1530,   741,   742,   743,   744,   745,  1531,
    1532,  1533,  1534,  1536,     0,     0,     0,     0,     0,     0,
    1538,     0,     0,  1537,     0,     0,  1542,     0,  1466,     0,
    1539,  1540,  1528,   738,  1529,  1530,   741,   742,   743,   744,
     745,  1531,  1532,  1533,  1534,     0,     0,     0,     0,     0,
       0,  1541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1538,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1539,  1540,  1543,     0,     0,  1536,  2466,     0,
       0,     0,     0,     0,     0,     0,     0,  1537,     0,     0,
       0,     0,     0,  1541,     0,     0,     0,     0,     0,     0,
     759,   760,   761,   762,   763,   764,   765,   766,     0,     0,
    2480,     0,     0,     0,  1542,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1538,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1539,  1540,     0,     0,
       0,     0,     0,     0,     0,     0,  1467,  1468,  1469,  1470,
    1471,  1472,  1473,  1474,     0,     0,  1542,  1541,  1536,     0,
       0,     0,  1543,     0,     0,     0,     0,     0,  1537,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1536,     0,  1544,  1545,  1546,  1547,  1548,  1549,  1550,  1551,
    1537,     0,     0,     0,  1543,     0,     0,  1538,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1539,  1540,     0,
    1542,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1541,  1538,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1539,
    1540,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1543,     0,
    1541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1544,  1545,  1546,  1547,  1548,  1549,  1550,  1551,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1542,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1544,  1545,  1546,  1547,  1548,  1549,  1550,  1551,
       0,     0,     0,  1542,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1543,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1543,     0,     0,     0,     0,  1544,  1545,  1546,  1547,
    1548,  1549,  1550,  1551,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1544,  1545,  1546,
    1547,  1548,  1549,  1550,  1551,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,   100,   101,   102,
     103,     0,     0,     0,   104,     0,     0,     0,     0,  1544,
    1545,  1546,  1547,  1548,  1549,  1550,  1551,   105,     0,     0,
     106,   107,   108,   109,     0,   110,   111,   112,   113,     0,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,   120,     0,   121,     0,     0,   122,     0,   123,     0,
       0,   124,     0,     0,   125,   126,   127,   128,   129,   130,
       0,     0,   131,   132,   133,   134,     0,     0,   135,     0,
     136,   137,   138,     0,   139,     0,   140,   141,     0,     0,
       0,     0,   142,   143,   144,   145,  1782,   146,   147,   148,
       0,   149,     0,   150,   151,     0,     0,   152,   153,     0,
     154,     0,   155,     0,     0,   156,     0,     0,     0,     0,
     157,  2011,     0,   158,     0,     0,   159,   160,   161,     0,
     162,   163,   164,     0,     0,   165,   166,     0,   167,     0,
     168,   169,   170,     0,     0,     0,     0,   171,     0,   172,
       0,   173,     0,     0,     0,   174,     0,     0,   175,   176,
     177,     0,   178,     0,   179,     0,     0,   180,     0,     0,
     181,     0,     0,     0,     0,     0,   182,   183,   345,     0,
       0,   184,     0,   185,     0,     0,     0,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,     0,   210,   211,   212,     0,   213,     0,   214,
       0,     0,   215,     0,   216,   217,     0,     0,     0,     0,
       0,     0,     0,   218,   219,   220,     0,     0,   221,   222,
     223,   224,   225,   226,   227,   228,     0,     0,     0,     0,
     229,     0,   230,     0,   231,   232,     0,     0,   233,   234,
     235,   236,     0,     0,   237,   238,   239,   240,   241,     0,
       0,     0,   242,   243,     0,   244,     0,     0,   245,   246,
       0,   346,     0,     0,   247,     0,   248,   249,     0,     0,
       0,     0,   250,     0,   251,   252,     0,   253,     0,     0,
       0,   254,     0,     0,   255,     0,   256,     0,     0,     0,
       0,   257,     0,   258,   259,   260,   261,     0,     0,     0,
     262,     0,   263,   264,     0,     0,   265,   266,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
     269,   270,     0,   271,     0,   272,   273,   274,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,   278,   279,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   280,     0,     0,   281,
       0,     0,     0,   282,   283,     0,   284,     0,     0,     0,
       0,   285,     0,     0,     0,   286,     0,     0,   287,   288,
     289,   290,     0,     0,     0,   291,   292,   293,     0,   294,
     295,     0,   296,     0,   297,     0,     0,     0,     0,     0,
       0,   298,     0,   299,   300,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,    99,   100,   101,
     102,   103,   302,     0,   303,   104,     0,   304,     0,     0,
     305,     0,   306,     0,   307,   308,   309,     0,   105,     0,
       0,   106,   107,   108,   109,     0,   110,   111,   112,   113,
       0,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,   120,     0,   121,     0,     0,   122,     0,   123,
       0,     0,   124,     0,     0,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,     0,     0,   135,
       0,   136,   137,   138,     0,   139,     0,   140,   141,     0,
       0,     0,     0,   142,   143,   144,   145,  2018,   146,   147,
     148,     0,   149,     0,   150,   151,     0,     0,   152,   153,
       0,   154,     0,   155,     0,     0,   156,     0,     0,     0,
       0,   157,  2019,     0,   158,     0,     0,   159,   160,   161,
       0,   162,   163,   164,     0,     0,   165,   166,     0,   167,
       0,   168,   169,   170,     0,     0,     0,     0,   171,     0,
     172,     0,   173,     0,     0,     0,   174,     0,     0,   175,
     176,   177,     0,   178,     0,   179,     0,     0,   180,     0,
       0,   181,     0,     0,     0,     0,     0,   182,   183,   345,
       0,     0,   184,     0,   185,     0,     0,     0,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,     0,   213,     0,
     214,     0,     0,   215,     0,   216,   217,     0,     0,     0,
       0,     0,     0,     0,   218,   219,   220,     0,     0,   221,
     222,   223,   224,   225,   226,   227,   228,     0,     0,     0,
       0,   229,     0,   230,     0,   231,   232,     0,     0,   233,
     234,   235,   236,     0,     0,   237,   238,   239,   240,   241,
       0,     0,     0,   242,   243,     0,   244,     0,     0,   245,
     246,     0,   346,     0,     0,   247,     0,   248,   249,     0,
       0,     0,     0,   250,     0,   251,   252,     0,   253,     0,
       0,     0,   254,     0,     0,   255,     0,   256,     0,     0,
       0,     0,   257,     0,   258,   259,   260,   261,     0,     0,
       0,   262,     0,   263,   264,     0,     0,   265,   266,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,   269,   270,     0,   271,     0,   272,   273,   274,   275,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,   278,   279,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   280,     0,     0,
     281,     0,     0,     0,   282,   283,     0,   284,     0,     0,
       0,     0,   285,     0,     0,     0,   286,     0,     0,   287,
     288,   289,   290,     0,     0,     0,   291,   292,   293,     0,
     294,   295,     0,   296,     0,   297,     0,     0,     0,     0,
       0,     0,   298,     0,   299,   300,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,    99,   100,
     101,   102,   103,   302,     0,   303,   104,     0,   304,     0,
       0,   305,     0,   306,     0,   307,   308,   309,     0,   105,
       0,     0,   106,   107,   108,   109,     0,   110,   111,   112,
     113,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,   120,     0,   121,     0,     0,   122,     0,
     123,     0,     0,   124,     0,     0,   125,   126,   127,   128,
     129,   130,     0,     0,   131,   132,   133,   134,     0,     0,
     135,     0,   136,   137,   138,     0,   139,     0,   140,   141,
       0,     0,     0,     0,   142,   143,   144,   145,     0,   146,
     147,   148,     0,   149,     0,   150,   151,     0,     0,   152,
     153,     0,   154,     0,   155,     0,     0,   156,     0,     0,
     946,     0,   157,     0,     0,   158,     0,     0,   159,   160,
     161,     0,   162,   163,   164,     0,     0,   165,   166,     0,
     167,     0,   168,   169,   170,     0,     0,     0,     0,   171,
       0,   172,     0,   173,     0,     0,     0,   174,     0,     0,
     175,   176,   177,     0,   178,     0,   179,     0,     0,   180,
       0,     0,   181,     0,     0,     0,     0,     0,   182,   183,
     345,     0,     0,   184,     0,   185,     0,     0,     0,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,     0,   210,   211,   212,     0,   213,
       0,   214,     0,     0,   215,     0,   216,   217,     0,     0,
       0,     0,     0,     0,     0,   218,   219,   220,     0,     0,
     221,   222,   223,   224,   225,   226,   227,   228,     0,     0,
       0,     0,   229,     0,   230,     0,   231,   232,     0,     0,
     233,   234,   235,   236,     0,     0,   237,   238,   239,   240,
     241,     0,     0,     0,   242,   243,     0,   244,     0,     0,
     245,   246,     0,   346,     0,     0,   247,     0,   248,   249,
       0,     0,     0,     0,   250,     0,   251,   252,     0,   253,
       0,     0,     0,   254,     0,     0,   255,     0,   256,     0,
       0,     0,     0,   257,     0,   258,   259,   260,   261,     0,
       0,     0,   262,     0,   263,   264,     0,     0,   265,   266,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,   269,   270,     0,   271,     0,   272,   273,   274,
     275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,   278,   279,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   280,     0,
       0,   281,     0,     0,     0,   282,   283,     0,   284,     0,
       0,     0,     0,   285,     0,     0,     0,   286,     0,     0,
     287,   288,   289,   290,     0,     0,     0,   291,   292,   293,
       0,   294,   295,     0,   296,     0,   297,     0,     0,     0,
       0,     0,     0,   298,     0,   299,   300,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   301,    99,
     100,   101,   102,   103,   302,     0,   303,   104,     0,   304,
       0,     0,   305,     0,   306,     0,   307,   308,   309,     0,
     105,     0,     0,   106,   107,   108,   109,     0,   110,   111,
     112,   113,     0,     0,     0,   114,     0,     0,   115,     0,
     116,   117,   118,   119,   120,     0,   121,     0,     0,   122,
       0,   123,     0,     0,   124,     0,     0,   125,   126,   127,
     128,   129,   130,     0,     0,   131,   132,   133,   134,     0,
       0,   135,     0,   136,   137,   138,     0,   139,     0,   140,
     141,     0,     0,     0,     0,   142,   143,   144,   145,  1782,
     146,   147,   148,     0,   149,     0,   150,   151,     0,     0,
     152,   153,     0,   154,     0,   155,     0,     0,   156,     0,
       0,     0,     0,   157,     0,     0,   158,     0,     0,   159,
     160,   161,     0,   162,   163,   164,     0,     0,   165,   166,
       0,   167,     0,   168,   169,   170,     0,     0,     0,     0,
     171,     0,   172,     0,   173,     0,     0,     0,   174,     0,
       0,   175,   176,   177,     0,   178,     0,   179,     0,     0,
     180,     0,     0,   181,     0,     0,     0,     0,     0,   182,
     183,   345,     0,     0,   184,     0,   185,     0,     0,     0,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,     0,   202,   203,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,     0,
     213,     0,   214,     0,     0,   215,     0,   216,   217,     0,
       0,     0,     0,     0,     0,     0,   218,   219,   220,     0,
       0,   221,   222,   223,   224,   225,   226,   227,   228,     0,
       0,     0,     0,   229,     0,   230,     0,   231,   232,     0,
       0,   233,   234,   235,   236,     0,     0,   237,   238,   239,
     240,   241,     0,     0,     0,   242,   243,     0,   244,     0,
       0,   245,   246,     0,   346,     0,     0,   247,     0,   248,
     249,     0,     0,     0,     0,   250,     0,   251,   252,     0,
     253,     0,     0,     0,   254,     0,     0,   255,     0,   256,
       0,     0,     0,     0,   257,     0,   258,   259,   260,   261,
       0,     0,     0,   262,     0,   263,   264,     0,     0,   265,
     266,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,   269,   270,     0,   271,     0,   272,   273,
     274,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,   278,   279,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   280,
       0,     0,   281,     0,     0,     0,   282,   283,     0,   284,
       0,     0,     0,     0,   285,     0,     0,     0,   286,     0,
       0,   287,   288,   289,   290,     0,     0,     0,   291,   292,
     293,     0,   294,   295,     0,   296,     0,   297,     0,     0,
       0,     0,     0,     0,   298,     0,   299,   300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   301,
      99,   100,   101,   102,   103,   302,     0,   303,   104,     0,
     304,     0,     0,   305,     0,   306,     0,   307,   308,   309,
       0,   105,     0,     0,   106,   107,   108,   109,     0,   110,
     111,   112,   113,     0,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,   120,     0,   121,     0,     0,
     122,     0,   123,     0,     0,   124,     0,     0,   125,   126,
     127,   128,   129,   130,     0,     0,   131,   132,   133,   134,
       0,     0,   135,     0,   136,   137,   138,     0,   139,     0,
     140,   141,     0,     0,     0,     0,   142,   143,   144,   145,
       0,   146,   147,   148,     0,   149,     0,   150,   151,     0,
       0,   152,   153,     0,   154,     0,   155,     0,     0,   156,
       0,     0,     0,     0,   157,  2349,     0,   158,     0,     0,
     159,   160,   161,     0,   162,   163,   164,     0,     0,   165,
     166,     0,   167,     0,   168,   169,   170,     0,     0,     0,
       0,   171,     0,   172,     0,   173,     0,     0,     0,   174,
       0,     0,   175,   176,   177,     0,   178,     0,   179,     0,
       0,   180,     0,     0,   181,     0,     0,     0,     0,     0,
     182,   183,   345,     0,     0,   184,     0,   185,     0,     0,
       0,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,     0,   210,   211,   212,
       0,   213,     0,   214,     0,     0,   215,     0,   216,   217,
       0,     0,     0,     0,     0,     0,     0,   218,   219,   220,
       0,     0,   221,   222,   223,   224,   225,   226,   227,   228,
       0,     0,     0,     0,   229,     0,   230,     0,   231,   232,
       0,     0,   233,   234,   235,   236,     0,     0,   237,   238,
     239,   240,   241,     0,     0,     0,   242,   243,     0,   244,
       0,     0,   245,   246,     0,   346,     0,     0,   247,     0,
     248,   249,     0,     0,     0,     0,   250,     0,   251,   252,
       0,   253,     0,     0,     0,   254,     0,     0,   255,     0,
     256,     0,     0,     0,     0,   257,     0,   258,   259,   260,
     261,     0,     0,     0,   262,     0,   263,   264,     0,     0,
     265,   266,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,   269,   270,     0,   271,     0,   272,
     273,   274,   275,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,   278,   279,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     280,     0,     0,   281,     0,     0,     0,   282,   283,     0,
     284,     0,     0,     0,     0,   285,     0,     0,     0,   286,
       0,     0,   287,   288,   289,   290,     0,     0,     0,   291,
     292,   293,     0,   294,   295,     0,   296,     0,   297,     0,
       0,     0,     0,     0,     0,   298,     0,   299,   300,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,    99,   100,   101,   102,   103,   302,     0,   303,   104,
       0,   304,     0,     0,   305,     0,   306,     0,   307,   308,
     309,     0,   105,     0,     0,   106,   107,   108,   109,     0,
     110,   111,   112,   113,     0,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,   120,     0,   121,     0,
       0,   122,     0,   123,     0,     0,   124,     0,     0,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,     0,     0,   135,     0,   136,   137,   138,     0,   139,
       0,   140,   141,     0,     0,     0,     0,   142,   143,   144,
     145,     0,   146,   147,   148,     0,   149,     0,   150,   151,
       0,     0,   152,   153,     0,   154,     0,   155,     0,     0,
     156,     0,     0,     0,     0,   157,     0,     0,   158,     0,
       0,   159,   160,   161,     0,   162,   163,   164,     0,     0,
     165,   166,     0,   167,     0,   168,   169,   170,     0,     0,
       0,     0,   171,     0,   172,     0,   173,     0,     0,     0,
     174,     0,     0,   175,   176,   177,     0,   178,     0,   179,
       0,     0,   180,     0,     0,   181,     0,     0,     0,     0,
       0,   182,   183,   345,     0,     0,   184,     0,   185,     0,
       0,     0,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,     0,   213,     0,   214,     0,     0,   215,     0,   216,
     217,     0,     0,     0,     0,     0,     0,     0,   218,   219,
     220,     0,     0,   221,   222,   223,   224,   225,   226,   227,
     228,     0,     0,     0,     0,   229,     0,   230,     0,   231,
     232,     0,     0,   233,   234,   235,   236,     0,     0,   237,
     238,   239,   240,   241,     0,     0,     0,   242,   243,     0,
     244,     0,     0,   245,   246,     0,   346,     0,     0,   247,
       0,   248,   249,     0,     0,     0,     0,   250,     0,   251,
     252,     0,   253,     0,     0,     0,   254,     0,     0,   255,
       0,   256,     0,     0,     0,     0,   257,     0,   258,   259,
     260,   261,     0,     0,     0,   262,     0,   263,   264,     0,
       0,   265,   266,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,   269,   270,     0,   271,     0,
     272,   273,   274,   275,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,   278,
     279,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   280,     0,     0,   281,     0,     0,     0,   282,   283,
       0,   284,     0,     0,     0,     0,   285,     0,     0,     0,
     286,     0,     0,   287,   288,   289,   290,     0,     0,     0,
     291,   292,   293,     0,   294,   295,     0,   296,     0,   297,
       0,     0,     0,     0,     0,     0,   298,     0,   299,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   301,    99,   100,   101,   102,   103,   302,     0,   303,
     104,     0,   304,     0,     0,   305,     0,   306,     0,   307,
     308,   309,     0,   105,     0,     0,   106,   107,   108,   109,
       0,   110,   111,   112,   113,     0,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,   120,     0,   121,
       0,     0,   122,     0,   123,     0,     0,   124,     0,     0,
     125,   126,   127,   128,   129,   130,     0,     0,   131,   132,
     133,   134,     0,     0,   135,     0,   136,   137,   138,     0,
     139,     0,   140,   141,     0,     0,     0,     0,   142,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,     0,     0,   152,   153,     0,   154,     0,   155,     0,
       0,   156,     0,     0,     0,     0,   157,     0,     0,   158,
       0,     0,   159,   160,   161,     0,   162,   163,   164,     0,
       0,   165,   166,     0,   167,     0,   168,   169,   170,     0,
       0,     0,     0,   171,     0,   172,     0,   173,     0,     0,
       0,   174,  1274,     0,   175,   176,   177,     0,   178,     0,
     179,     0,     0,   180,     0,     0,   181,     0,     0,     0,
       0,     0,   182,   183,     0,     0,     0,   184,     0,   185,
       0,     0,     0,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,     0,   210,
     211,   212,     0,   213,     0,   214,     0,     0,   215,     0,
     216,   217,     0,     0,     0,     0,     0,     0,     0,   218,
     219,   220,     0,     0,   221,   222,   223,   224,   225,   226,
     227,   228,     0,     0,     0,     0,   229,     0,   230,     0,
     231,   232,     0,     0,   233,   234,   235,   236,     0,     0,
     237,   238,   239,   240,   241,     0,     0,     0,   242,   243,
       0,   244,     0,     0,   245,   246,     0,   384,     0,     0,
     247,     0,   248,   249,     0,     0,     0,     0,   250,     0,
     251,   252,     0,   253,     0,     0,     0,   254,     0,     0,
     255,     0,   256,     0,     0,     0,     0,   257,     0,   258,
     259,   260,   261,     0,     0,     0,   262,     0,   263,   264,
       0,     0,   265,   266,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,   269,   270,     0,   271,
       0,   272,   273,   274,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,     0,     0,     0,     0,     0,     0,
     278,   279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   280,     0,     0,   281,     0,     0,     0,   282,
     283,     0,   284,     0,     0,     0,     0,   285,     0,     0,
       0,   286,     0,     0,   287,   288,   289,   290,     0,     0,
       0,   291,   292,   293,     0,   294,   295,     0,   296,     0,
     297,     0,     0,     0,     0,     0,     0,   298,     0,   299,
     300,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   301,    99,   100,   101,   102,   103,   302,     0,
     303,   104,     0,   304,     0,     0,   305,     0,   306,     0,
     307,   308,   309,     0,   105,     0,     0,   106,   107,   108,
     109,     0,   110,   111,   112,   113,     0,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,   120,     0,
     121,     0,     0,   122,     0,   123,     0,     0,   124,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,   131,
     132,   133,   134,     0,     0,   135,     0,   136,   137,   138,
       0,   139,     0,   140,   141,     0,     0,  2269,     0,   142,
     143,   144,   145,     0,   146,   147,   148,     0,   149,     0,
     150,   151,     0,     0,   152,   153,     0,   154,     0,   155,
       0,     0,   156,     0,     0,     0,     0,   157,     0,     0,
     158,     0,     0,   159,   160,   161,     0,   162,   163,   164,
       0,     0,   165,   166,     0,   167,     0,   168,   169,   170,
       0,     0,     0,     0,   171,     0,   172,     0,   173,     0,
       0,     0,   174,     0,     0,   175,   176,   177,     0,   178,
       0,   179,     0,     0,   180,     0,     0,   181,     0,     0,
       0,     0,     0,   182,   183,     0,     0,     0,   184,     0,
     185,     0,     0,     0,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,     0,   213,     0,   214,     0,     0,   215,
       0,   216,   217,     0,     0,     0,     0,     0,     0,     0,
     218,   219,   220,     0,     0,   221,   222,   223,   224,   225,
     226,   227,   228,     0,     0,     0,     0,   229,     0,   230,
       0,   231,   232,     0,     0,   233,   234,   235,   236,     0,
       0,   237,   238,   239,   240,   241,     0,     0,     0,   242,
     243,     0,   244,     0,     0,   245,   246,     0,   346,     0,
       0,   247,     0,   248,   249,     0,     0,     0,     0,   250,
       0,   251,   252,     0,   253,     0,     0,     0,   254,     0,
       0,   255,     0,   256,     0,     0,     0,     0,   257,     0,
     258,   259,   260,   261,     0,     0,     0,   262,     0,   263,
     264,     0,     0,   265,   266,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,   269,   270,     0,
     271,     0,   272,   273,   274,   275,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,     0,
       0,   278,   279,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   280,     0,     0,   281,     0,     0,     0,
     282,   283,     0,   284,     0,     0,     0,     0,   285,     0,
       0,     0,   286,     0,     0,   287,   288,   289,   290,     0,
       0,     0,   291,   292,   293,     0,   294,   295,     0,   296,
       0,   297,     0,     0,     0,     0,     0,     0,   298,     0,
     299,   300,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,    99,   100,   101,   102,   103,   302,
       0,   303,   104,     0,   304,     0,     0,   305,     0,   306,
       0,   307,   308,   309,     0,   105,     0,     0,   106,   107,
     108,   109,     0,   110,   111,   112,   113,     0,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,   120,
       0,   121,     0,     0,   122,     0,   123,     0,     0,   124,
       0,     0,   125,   126,   127,   128,   129,   130,     0,     0,
     131,   132,   133,   134,     0,     0,   135,     0,   136,   137,
     138,     0,   139,     0,   140,   141,     0,     0,     0,     0,
     142,   143,   144,   145,     0,   146,   147,   148,     0,   149,
       0,   150,   151,     0,     0,   152,   153,     0,   154,     0,
     155,     0,     0,   156,     0,     0,     0,     0,   157,     0,
       0,   158,     0,     0,   159,   160,   161,     0,   162,   163,
     164,     0,     0,   165,   166,     0,   167,     0,   168,   169,
     170,     0,     0,     0,   935,   171,     0,   172,     0,   173,
       0,     0,     0,   174,     0,     0,   175,   176,   177,     0,
     178,     0,   179,     0,     0,   180,     0,     0,   181,     0,
       0,     0,     0,     0,   182,   183,     0,     0,     0,   184,
       0,   185,     0,     0,     0,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
       0,   210,   211,   212,     0,   213,     0,   214,     0,     0,
     215,     0,   216,   217,     0,     0,     0,     0,     0,     0,
       0,   218,   219,   220,     0,     0,   221,   222,   223,   224,
     225,   226,   227,   228,     0,     0,     0,     0,   229,     0,
     230,     0,   231,   232,     0,     0,   233,   234,   235,   236,
       0,     0,   237,   238,   239,   240,   241,     0,     0,     0,
     242,   243,     0,   244,     0,     0,   245,   246,     0,     0,
       0,     0,   247,     0,   248,   249,     0,     0,     0,     0,
     250,     0,   251,   252,     0,   253,     0,     0,     0,   254,
       0,     0,   255,     0,   256,     0,     0,     0,     0,   257,
       0,   258,   259,   260,   261,     0,     0,     0,   262,     0,
     263,   264,     0,     0,   265,   266,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,   269,   270,
       0,   271,     0,   272,   273,   274,   275,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,     0,     0,     0,
       0,     0,   278,   279,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   280,     0,     0,   281,     0,     0,
       0,   282,   283,     0,   284,     0,     0,     0,     0,   285,
       0,     0,     0,   286,     0,     0,   287,   288,   289,   290,
       0,     0,     0,   291,   292,   293,     0,   294,   295,     0,
     296,     0,   297,     0,     0,     0,     0,     0,     0,   298,
       0,   299,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,    99,   100,   101,   102,   103,
     302,     0,   303,   104,     0,   304,     0,     0,   305,     0,
     306,     0,   307,   308,   309,     0,   105,     0,     0,   106,
     107,   108,   109,     0,   110,   111,   112,   113,     0,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
     120,     0,   121,     0,     0,   122,     0,   123,     0,     0,
     124,     0,     0,   125,   126,   127,   128,   129,   130,     0,
       0,   131,   132,   133,   134,     0,     0,   135,     0,   136,
     137,   138,     0,   139,     0,   140,   141,     0,     0,     0,
       0,   142,   143,   144,   145,     0,   146,   147,   148,     0,
     149,     0,   150,   151,     0,     0,   152,   153,     0,   154,
       0,   155,     0,     0,   156,     0,     0,     0,     0,   157,
    1608,     0,   158,     0,     0,   159,   160,   161,     0,   162,
     163,   164,     0,     0,   165,   166,     0,   167,     0,   168,
     169,   170,     0,     0,     0,     0,   171,     0,   172,     0,
     173,     0,     0,     0,   174,     0,     0,   175,   176,   177,
       0,   178,     0,   179,     0,     0,   180,     0,     0,   181,
       0,     0,     0,     0,     0,   182,   183,     0,     0,     0,
     184,     0,   185,     0,     0,     0,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,     0,   213,     0,   214,     0,
       0,   215,     0,   216,   217,     0,     0,     0,     0,     0,
       0,     0,   218,   219,   220,     0,     0,   221,   222,   223,
     224,   225,   226,   227,   228,     0,     0,     0,     0,   229,
       0,   230,     0,   231,   232,     0,     0,   233,   234,   235,
     236,     0,     0,   237,   238,   239,   240,   241,     0,     0,
       0,   242,   243,     0,   244,     0,     0,   245,   246,     0,
       0,     0,     0,   247,     0,   248,   249,     0,     0,     0,
       0,   250,     0,   251,   252,     0,   253,     0,     0,     0,
     254,     0,     0,   255,     0,   256,     0,     0,     0,     0,
     257,     0,   258,   259,   260,   261,     0,     0,     0,   262,
       0,   263,   264,     0,     0,   265,   266,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,   269,
     270,     0,   271,     0,   272,   273,   274,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,     0,     0,   278,   279,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   280,     0,     0,   281,     0,
       0,     0,   282,   283,     0,   284,     0,     0,     0,     0,
     285,     0,     0,     0,   286,     0,     0,   287,   288,   289,
     290,     0,     0,     0,   291,   292,   293,     0,   294,   295,
       0,   296,     0,   297,     0,     0,     0,     0,     0,     0,
     298,     0,   299,   300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   301,    99,   100,   101,   102,
     103,   302,     0,   303,   104,     0,   304,     0,     0,   305,
       0,   306,     0,   307,   308,   309,     0,   105,     0,     0,
     106,   107,   108,   109,     0,   110,   111,   112,   113,     0,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,   120,     0,   121,     0,     0,   122,     0,   123,     0,
       0,   124,     0,     0,   125,   126,   127,   128,   129,   130,
       0,     0,   131,   132,   133,   134,     0,     0,   135,     0,
     136,   137,   138,     0,   139,     0,   140,   141,     0,     0,
       0,     0,   142,   143,   144,   145,     0,   146,   147,   148,
       0,   149,     0,   150,   151,     0,     0,   152,   153,     0,
     154,     0,   155,     0,     0,   156,     0,     0,     0,     0,
     157,  1645,     0,   158,     0,     0,   159,   160,   161,     0,
     162,   163,   164,     0,     0,   165,   166,     0,   167,     0,
     168,   169,   170,     0,     0,     0,     0,   171,     0,   172,
       0,   173,     0,     0,     0,   174,     0,     0,   175,   176,
     177,     0,   178,     0,   179,     0,     0,   180,     0,     0,
     181,     0,     0,     0,     0,     0,   182,   183,     0,     0,
       0,   184,     0,   185,     0,     0,     0,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,     0,   210,   211,   212,     0,   213,     0,   214,
       0,     0,   215,     0,   216,   217,     0,     0,     0,     0,
       0,     0,     0,   218,   219,   220,     0,     0,   221,   222,
     223,   224,   225,   226,   227,   228,     0,     0,     0,     0,
     229,     0,   230,     0,   231,   232,     0,     0,   233,   234,
     235,   236,     0,     0,   237,   238,   239,   240,   241,     0,
       0,     0,   242,   243,     0,   244,     0,     0,   245,   246,
       0,     0,     0,     0,  1646,     0,   248,   249,     0,     0,
       0,     0,   250,     0,   251,   252,     0,   253,     0,     0,
       0,   254,     0,     0,   255,     0,   256,     0,     0,     0,
       0,   257,     0,   258,   259,   260,   261,     0,     0,     0,
     262,     0,   263,   264,     0,     0,   265,   266,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
     269,   270,     0,   271,     0,   272,   273,   274,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,   278,   279,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   280,     0,     0,   281,
       0,     0,     0,   282,   283,     0,   284,     0,     0,     0,
       0,   285,     0,     0,     0,   286,     0,     0,   287,   288,
     289,   290,     0,     0,     0,   291,   292,   293,     0,   294,
     295,     0,   296,     0,   297,     0,     0,     0,     0,     0,
       0,   298,     0,   299,   300,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,    99,   100,   101,
     102,   103,   302,     0,   303,   104,     0,   304,     0,     0,
     305,     0,   306,     0,   307,   308,   309,     0,   105,     0,
       0,   106,   107,   108,   109,     0,   110,   111,   112,   113,
       0,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,   120,     0,   121,     0,     0,   122,     0,   123,
       0,     0,   124,     0,     0,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,     0,     0,   135,
       0,   136,   137,   138,     0,   139,     0,   140,   141,     0,
       0,     0,     0,   142,   143,   144,   145,     0,   146,   147,
     148,     0,   149,     0,   150,   151,     0,     0,   152,   153,
       0,   154,     0,   155,     0,     0,   156,     0,     0,     0,
       0,   157,     0,     0,   158,     0,     0,   159,   160,   161,
       0,   162,   163,   164,     0,     0,   165,   166,     0,   167,
       0,   168,   169,   170,     0,     0,     0,     0,   171,     0,
     172,     0,   173,     0,     0,     0,   174,     0,     0,   175,
     176,   177,     0,   178,     0,   179,     0,     0,   180,     0,
       0,   181,     0,     0,     0,     0,     0,   182,   183,     0,
       0,     0,   184,     0,   185,     0,     0,     0,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,     0,   213,     0,
     214,     0,     0,   215,     0,   216,   217,     0,     0,     0,
       0,     0,     0,     0,   218,   219,   220,  2305,     0,   221,
     222,   223,   224,   225,   226,   227,   228,     0,     0,     0,
       0,   229,     0,   230,     0,   231,   232,     0,     0,   233,
     234,   235,   236,     0,     0,   237,   238,   239,   240,   241,
       0,     0,     0,   242,   243,     0,   244,     0,     0,   245,
     246,     0,     0,     0,     0,   247,     0,   248,   249,     0,
       0,     0,     0,   250,     0,   251,   252,     0,   253,     0,
       0,     0,   254,     0,     0,   255,     0,   256,     0,     0,
       0,     0,   257,     0,   258,   259,   260,   261,     0,     0,
       0,   262,     0,   263,   264,     0,     0,   265,   266,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,   269,   270,     0,   271,     0,   272,   273,   274,   275,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,   278,   279,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   280,     0,     0,
     281,     0,     0,     0,   282,   283,     0,   284,     0,     0,
       0,     0,   285,     0,     0,     0,   286,     0,     0,   287,
     288,   289,   290,     0,     0,     0,   291,   292,   293,     0,
     294,   295,     0,   296,     0,   297,     0,     0,     0,     0,
       0,     0,   298,     0,   299,   300,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,    99,   100,
     101,   102,   103,   302,     0,   303,   104,     0,   304,     0,
       0,   305,     0,   306,     0,   307,   308,   309,     0,   105,
       0,     0,   106,   107,   108,   109,     0,   110,   111,   112,
     113,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,   120,     0,   121,     0,     0,   122,     0,
     123,     0,     0,   124,     0,     0,   125,   126,   127,   128,
     129,   130,     0,     0,   131,   132,   133,   134,     0,     0,
     135,     0,   136,   137,   138,     0,   139,     0,   140,   141,
       0,     0,     0,     0,   142,   143,   144,   145,     0,   146,
     147,   148,     0,   149,     0,   150,   151,     0,     0,   152,
     153,     0,   154,     0,   155,     0,     0,   156,     0,     0,
       0,     0,   157,     0,     0,   158,     0,     0,   159,   160,
     161,     0,   162,   163,   164,     0,     0,   165,   166,     0,
     167,     0,   168,   169,   170,     0,     0,     0,     0,   171,
       0,   172,     0,   173,     0,     0,     0,   174,     0,     0,
     175,   176,   177,     0,   178,     0,   179,     0,     0,   180,
       0,     0,   181,     0,     0,     0,     0,     0,   182,   183,
       0,     0,     0,   184,     0,   185,     0,     0,     0,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,     0,   210,   211,   212,     0,   213,
       0,   214,     0,     0,   215,     0,   216,   217,     0,     0,
       0,     0,     0,     0,     0,   218,   219,   220,     0,     0,
     221,   222,   223,   224,   225,   226,   227,   228,     0,     0,
       0,     0,   229,     0,   230,     0,   231,   232,     0,     0,
     233,   234,   235,   236,     0,     0,   237,   238,   239,   240,
     241,     0,     0,     0,   242,   243,     0,   244,     0,     0,
     245,   246,     0,   346,     0,     0,   247,     0,   248,   249,
       0,     0,     0,     0,   250,     0,   251,   252,     0,   253,
       0,     0,     0,   254,     0,     0,   255,     0,   256,     0,
       0,     0,     0,   257,     0,   258,   259,   260,   261,     0,
       0,     0,   262,     0,   263,   264,     0,     0,   265,   266,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,   269,   270,     0,   271,     0,   272,   273,   274,
     275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,   278,   279,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   280,     0,
       0,   281,     0,     0,     0,   282,   283,     0,   284,     0,
       0,     0,     0,   285,     0,     0,     0,   286,     0,     0,
     287,   288,   289,   290,     0,     0,     0,   291,   292,   293,
       0,   294,   295,     0,   296,     0,   297,     0,     0,     0,
       0,     0,     0,   298,     0,   299,   300,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   301,    99,
     100,   101,   102,   103,   302,     0,   303,   104,     0,   304,
       0,     0,   305,     0,   306,     0,   307,   308,   309,     0,
     105,     0,     0,   106,   107,   108,   109,     0,   110,   111,
     112,   113,     0,     0,     0,   114,     0,     0,   115,     0,
     116,   117,   118,   119,   120,     0,   121,     0,     0,   122,
       0,   123,     0,     0,   124,     0,     0,   125,   126,   127,
     128,   129,   130,     0,     0,   131,   132,   133,   134,     0,
       0,   135,     0,   136,   137,   138,     0,   139,     0,   140,
     141,     0,     0,     0,     0,   142,   143,   144,   145,     0,
     146,   147,   148,     0,   149,     0,   150,   151,     0,     0,
     152,   153,     0,   154,     0,   155,     0,     0,   156,     0,
       0,     0,     0,   157,  1645,     0,   158,     0,     0,   159,
     160,   161,     0,   162,   163,   164,     0,     0,   165,   166,
       0,   167,     0,   168,   169,   170,     0,     0,     0,     0,
     171,     0,   172,     0,   173,     0,     0,     0,   174,     0,
       0,   175,   176,   177,     0,   178,     0,   179,     0,     0,
     180,     0,     0,   181,     0,     0,     0,     0,     0,   182,
     183,     0,     0,     0,   184,     0,   185,     0,     0,     0,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,     0,   202,   203,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,     0,
     213,     0,   214,     0,     0,   215,     0,   216,   217,     0,
       0,     0,     0,     0,     0,     0,   218,   219,   220,     0,
       0,   221,   222,   223,   224,   225,   226,   227,   228,     0,
       0,     0,     0,   229,     0,   230,     0,   231,   232,     0,
       0,   233,   234,   235,   236,     0,     0,   237,   238,   239,
     240,   241,     0,     0,     0,   242,   243,     0,   244,     0,
       0,   245,   246,     0,     0,     0,     0,   247,     0,   248,
     249,     0,     0,     0,     0,   250,     0,   251,   252,     0,
     253,     0,     0,     0,   254,     0,     0,   255,     0,   256,
       0,     0,     0,     0,   257,     0,   258,   259,   260,   261,
       0,     0,     0,   262,     0,   263,   264,     0,     0,   265,
     266,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,   269,   270,     0,   271,     0,   272,   273,
     274,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,   278,   279,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   280,
       0,     0,   281,     0,     0,     0,   282,   283,     0,   284,
       0,     0,     0,     0,   285,     0,     0,     0,   286,     0,
       0,   287,   288,   289,   290,     0,     0,     0,   291,   292,
     293,     0,   294,   295,     0,   296,     0,   297,     0,     0,
       0,     0,     0,     0,   298,     0,   299,   300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   301,
      99,   100,   101,   102,   103,   302,     0,   303,   104,     0,
     304,     0,     0,   305,     0,   306,     0,   307,   308,   309,
       0,   105,     0,     0,   106,   107,   108,   109,     0,   110,
     111,   112,   113,     0,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,   120,     0,   121,     0,     0,
     122,     0,   123,     0,     0,   124,     0,     0,   125,   126,
     127,   128,   129,   130,     0,     0,   131,   132,   133,   134,
       0,     0,   135,     0,   136,   137,   138,     0,   139,     0,
     140,   141,     0,     0,     0,     0,   142,   143,   144,   145,
       0,   146,   147,   148,     0,   149,     0,   150,   151,     0,
       0,   152,   153,     0,   154,     0,   155,     0,     0,   156,
       0,     0,     0,     0,   157,     0,     0,   158,     0,     0,
     159,   160,   161,     0,   162,   163,   164,     0,     0,   165,
     166,     0,   167,     0,   168,   169,   170,     0,     0,     0,
       0,   171,     0,   172,     0,   173,     0,     0,     0,   174,
       0,     0,   175,   176,   177,     0,   178,     0,   179,     0,
       0,   180,     0,     0,   181,     0,     0,     0,     0,     0,
     182,   183,     0,     0,     0,   184,     0,   185,     0,     0,
       0,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,     0,   210,   211,   212,
       0,   213,     0,   214,     0,     0,   215,     0,   216,   217,
       0,     0,     0,     0,     0,     0,     0,   218,   219,   220,
       0,     0,   221,   222,   223,   224,   225,   226,   227,   228,
       0,     0,     0,     0,   229,     0,   230,     0,   231,   232,
       0,     0,   233,   234,   235,   236,     0,     0,   237,   238,
     239,   240,   241,     0,     0,     0,   242,   243,     0,   244,
       0,     0,   245,   246,     0,     0,     0,     0,   247,     0,
     248,   249,     0,     0,     0,     0,   250,     0,   251,   252,
       0,   253,     0,     0,     0,   254,     0,     0,   255,     0,
     256,     0,     0,     0,     0,   257,     0,   258,   259,   260,
     261,     0,     0,     0,   262,     0,   263,   264,     0,     0,
     265,   266,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,   269,   270,     0,   271,     0,   272,
     273,   274,   275,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,   278,   279,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     280,     0,     0,   281,     0,     0,     0,   282,   283,     0,
     284,     0,     0,     0,     0,   285,     0,     0,     0,   286,
       0,     0,   287,   288,   289,   290,     0,     0,     0,   291,
     292,   293,     0,   294,   295,     0,   296,     0,   297,     0,
       0,     0,     0,     0,     0,   298,     0,   299,   300,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,    99,   100,   101,  1280,   103,   302,     0,   303,   104,
       0,   304,     0,     0,   305,     0,   306,     0,   307,   308,
     309,     0,   105,     0,     0,   106,   107,   108,   109,     0,
     110,   111,   112,   113,     0,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,   120,     0,   121,     0,
       0,   122,     0,   123,     0,     0,   124,     0,     0,   125,
     126,   127,   128,   129,   130,     0,     0,   131,   132,   133,
     134,     0,     0,   135,     0,   136,   137,   138,     0,   139,
       0,   140,   141,     0,     0,     0,     0,   142,   143,   144,
     145,     0,   146,   147,   148,     0,   149,     0,   150,   151,
       0,     0,   152,   153,     0,   154,     0,   155,     0,     0,
     156,     0,     0,     0,     0,   157,     0,     0,   158,     0,
       0,   159,   160,   161,     0,   162,   163,   164,     0,     0,
     165,   166,     0,   167,     0,   168,  1281,   170,     0,     0,
       0,     0,   171,     0,   172,     0,   173,     0,     0,     0,
     174,     0,     0,   175,   176,   177,     0,   178,     0,   179,
       0,     0,   180,     0,     0,   181,     0,     0,     0,     0,
       0,   182,   183,     0,     0,     0,   184,     0,   185,     0,
       0,     0,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,     0,   213,     0,   214,     0,     0,   215,     0,   216,
     217,     0,     0,     0,     0,     0,     0,     0,   218,   219,
     220,     0,     0,   221,   222,   223,   224,   225,   226,   227,
     228,     0,     0,     0,     0,   229,     0,   230,     0,   231,
     232,     0,     0,   233,   234,   235,   236,     0,     0,   237,
     238,   239,   240,   241,     0,     0,     0,   242,   243,     0,
     244,     0,     0,   245,   246,     0,     0,     0,     0,   247,
       0,   248,   249,     0,     0,     0,     0,   250,     0,   251,
     252,     0,   253,     0,     0,     0,   254,     0,     0,   255,
       0,   256,     0,     0,     0,     0,   257,     0,   258,   259,
     260,   261,     0,     0,     0,   262,     0,   263,   264,     0,
       0,   265,   266,     0,   267,     0,     0,     0,  2575,  2576,
    2577,  2578,     0,     0,   268,   269,   270,     0,   271,     0,
     272,   273,   274,   275,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,  2579,     0,     0,     0,     0,   278,
     279,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   280,     0,     0,   281,     0,     0,     0,   282,   283,
       0,   284,     0,     0,     0,     0,   285,     0,     0,     0,
     286,     0,     0,   287,   288,   289,   290,  2580,     0,     0,
     291,   292,   293,     0,   294,   295,     0,   296,     0,   297,
       0,     0,     0,     0,     0,     0,   298,     0,   299,   300,
       0,     0,     0,  2581,     0,     0,     0,  2582,     0,  2583,
       0,   301,     0,     0,     0,     0,     0,   302,     0,   303,
       0,     0,   304,     0,     0,   305,     0,   306,     0,   307,
     308,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2584,     0,  2585,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   825,     0,
       0,     0,     0,     0,   826,   827,     0,     0,     0,  2586,
    2587,  2588,     0,     0,  2589,  2590,  2591,  2592,  2593,     0,
    2594,  2595,  2596,   828,  2597,  2598,  2599,  2600,  2601,  2602,
       0,     0,     0,     0,  2603,     0,  2604,   829,  2605,  2606,
    2607,  2608,  2609,  2610,     0,     0,  2611,  2612,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,   801,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   831,     0,     0,     0,     0,   832,     0,   833,
       0,     0,     0,     0,   834,     0,     0,     0,  2613,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  2614,     0,     0,     0,     0,     0,     0,
       0,     0,   835,     0,   836,     0,   837,  2615,  2616,  2617,
       0,     0,     0,     0,  2618,     0,  2619,     0,   838,   839,
       0,     0,   840,     0,     0,     0,     0,   841,     0,     0,
       0,     0,     0,     0,     0,     0,   842,     0,   843,     0,
       0,     0,     0,     0,     0,     0,  2620,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   844,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     845,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   846,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   847,     0,     0,     0,     0,     0,   848,   849,     0,
       0,   850,   851,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0, -1509,     0,     0,     0,     0,     0,     0,     0,   852,
     853,     0,     0,     0,   854,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   855,
   -1147
};

static const short yycheck[] =
{
       2,   498,   336,   407,   408,   811,    14,   395,   396,   397,
      12,   610,    14,   919,   916,   369,   663,   910,  1224,  1956,
     522,   620,    24,   852,   853,     2,  1358,  1163,   878,   879,
     629,   834,   853,  1649,   594,  1651,   596,   597,   598,   599,
     600,  1674,    44,    45,  1977,   848,   450,   529,  1072,  1192,
    1272,  1596,  1292,   856,  1678,   905,   761,   762,  1682,  2126,
    2027,   506,   569,  2345,  1132,  1129,  1525,  2511,  1774,  1775,
    1292,   990,   476,   477,   478,    12,  1743,  2047,   928,     3,
      82,   641,   642,   643,  1203,  1195,     3,    68,  1152,  1998,
      68,  1201,   701,    42,  2003,    10,     3,    68,    39,    68,
      68,   951,    68,    97,    68,    82,     9,   857,   103,    12,
     144,    59,  2041,  1137,   105,   926,    19,  2039,  2047,   926,
    2057,  2050,  2051,     3,    55,   136,  2055,  2056,     3,  2119,
       3,  2068,  2069,     3,    10,  1573,     3,  1748,   635,   159,
     637,   591,   639,   752,    18,   161,    22,   161,  2115,   758,
     167,   220,     4,   339,  2083,   171,   279,    31,     3,    33,
       9,    35,    36,    12,    34,    10,    40,    41,   613,   251,
      19,    20,    21,    88,   214,   223,   136,    10,  2799,  2733,
     201,  2042,  2043,    44,  2366,   159,   136,   280,    56,  1325,
     103,   105,   147,   144,    39,   109,   105,    10,    70,   279,
     221,   214,    70,  2339,    99,   181,   108,   214,  1293,   228,
      55,   107,   275,    12,    87,   279,   179,   208,   111,   279,
      90,    91,     9,   164,   179,    12,  2847,     3,   169,   144,
     184,   119,    19,    20,    21,     9,   106,   101,    12,   279,
      41,   105,  1985,    88,  2285,    19,    20,    21,   118,   119,
       9,   121,   159,    12,   294,    88,   206,   280,   153,   254,
      19,    86,   214,   277,     3,  2722,   279,   222,   296,   210,
     486,  2715,   279,    40,   490,    88,   120,    39,   292,   216,
     221,   294,    83,  2727,   672,    33,   169,   294,   299,   109,
     244,   161,  2333,    56,   270,  1188,   260,   631,   632,    66,
    2921,   280,   127,   296,   228,   368,   151,   152,   310,   264,
     329,   228,   646,   216,   134,   317,  1166,  1950,  1168,   164,
     268,   166,    70,   337,   169,   273,   317,   279,   339,   717,
     218,   272,   202,   302,   303,   337,   670,   207,   302,   303,
     184,   254,   294,   119,   237,   308,   680,   427,   428,    86,
     246,   184,  2809,   307,   368,   101,   690,   691,   692,   105,
     301,   309,   232,  2984,   259,   210,   700,   216,   318,   492,
     238,   241,   242,   243,    39,   325,   221,  2559,   329,   339,
     119,   305,    70,   228,  1843,   571,   301,   257,   305,   339,
     127,   293,   244,  2363,   368,   488,  1199,   751,  1466,   260,
    2370,  2017,   164,   317,  2958,   329,   488,   169,   317,  2545,
     282,   244,   329,  1216,   284,   260,   436,   216,   155,   488,
    3041,   296,   919,   873,   488,   305,   296,   272,   488,   216,
     912,   295,   275,   306,  2363,   136,   154,   428,   305,  2429,
     454,  2370,   216,  1272,  1903,   493,   486,   321,   210,   329,
     314,   296,   475,   317,   488,  2377,   301,   216,   389,   487,
     305,  2128,   329,  2087,  1214,   486,   407,   280,   342,  1709,
     486,   488,  1536,   486,   307,  1543,  1287,   488,  1563,   486,
    1287,   489,   306,  1593,   329,   967,   488,   489,   490,  1339,
     354,  2115,   486,   488,   487,  1142,   437,  1144,   260,   164,
    2361,   492,  1313,  1527,   169,   486,  1313,   509,   486,   490,
     272,   161,   382,   389,  1952,   486,  2127,   486,   486,   279,
     486,  1130,   486,  1132,   407,   473,   474,   529,   488,   436,
       6,   533,   509,  2990,   404,   272,   136,   486,   488,   224,
    1746,   490,   946,  1040,   389,   210,   483,   488,   550,   295,
    2293,  2294,   273,   154,   437,   147,   533,   559,   428,   145,
     562,   279,   407,   159,   317,     0,   568,  1475,   314,   493,
     572,   317,   156,   488,   576,  2542,   493,   480,   481,   482,
     483,   583,   584,    25,  2732,   562,  2556,   179,   184,   434,
      51,  2258,   437,   144,     3,   141,  1818,  1094,   299,    86,
     353,  1520,   142,  1722,  1723,  1724,   206,   272,    85,  2730,
      56,    72,  1731,   177,   144,   166,  2525,  2684,   488,   159,
     337,    39,   486,  2771,  2257,  3069,  2908,  2556,   477,   478,
     479,   480,   481,   482,   483,   228,   182,   167,   339,   513,
     127,   515,   487,   488,  1552,   407,   223,   233,    94,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    10,   139,   488,  1558,  2502,  2634,   428,   146,
     167,  1564,   264,   219,    22,   437,  1183,   228,   279,   265,
     177,  2297,  1127,   247,   161,  1578,   270,  1392,  1393,   476,
     477,   478,   479,   480,   481,   482,   483,   139,   103,   245,
    2536,   270,  2639,   296,   478,   479,   480,   481,   482,   483,
    2326,    73,   118,  1606,    76,   141,   285,   201,   318,   478,
     479,   480,   481,   482,   483,   325,   488,    87,  2849,  2850,
     467,   468,   469,   470,   471,   472,   329,   221,  2283,   339,
    1946,   217,   407,   339,  2811,   269,   164,  2206,  1641,    94,
     247,   169,   473,   474,   278,   161,   182,   112,  1260,   284,
     300,   275,    89,     3,  1469,  1470,  2433,   274,   275,  1368,
      10,    39,   437,   460,  1108,  1129,   301,  1983,  1846,   787,
    1986,  1987,  1264,  1586,  2243,   787,  1589,   789,   143,  1291,
     267,  1125,   210,   219,   358,   122,   798,   280,  1152,  1863,
     277,  3091,   275,   221,   350,   101,   161,   167,   810,   105,
     277,  3101,   217,   239,   240,   292,   171,   177,   403,   245,
     297,   798,   487,   488,  1717,  1718,  1719,  1851,  1721,  2796,
    1659,    39,  1725,  1726,  1727,  1728,  1729,  1730,  1659,  1907,
    1904,  1546,  1547,  2510,  1963,  1964,  2086,   855,    88,   254,
     328,  1460,  1988,   855,   272,   260,   216,  1466,   292,  1969,
    1924,   358,   340,    39,  2086,   343,   226,   227,   213,   275,
    2576,  2014,   874,   161,   403,   113,   878,   879,   244,   234,
     167,  2587,   161,   171,   250,  2818,   279,   247,  1912,   107,
     177,   368,  2516,  2517,  2518,   119,   164,   263,  1395,  2605,
    2606,   169,   904,   905,  1401,   129,   144,   119,   486,   107,
     912,   216,  1521,   218,  2620,   486,   140,   919,  2534,  2125,
     922,  2627,   924,  2539,   350,   143,   928,   904,  1537,   141,
     932,   157,   158,   935,  1543,   109,   141,   131,   132,  2058,
    2059,   943,   210,  2062,   486,   143,   184,   120,  2572,   951,
     310,  2070,  2071,  2072,  2073,   932,   164,  1238,   935,  1240,
     247,   169,   216,  1244,   218,   967,   943,    34,   486,   148,
     182,   341,   317,   230,   231,   320,   321,   182,    56,  2580,
     486,   351,   155,   486,  2755,   355,   486,  2758,   164,   407,
    2761,   141,    70,   169,  2130,  2766,   341,  2598,   358,   295,
    2601,   346,   210,   493,   272,    34,   351,   219,   306,  2610,
    1344,   184,   486,   221,   219,   486,   234,  2788,   314,   437,
    2791,   317,  2623,    90,   203,   204,   205,    94,  1362,  1363,
     486,   225,   182,   245,   210,  1369,   234,   305,  1679,  1680,
     245,  1682,   109,   312,   313,    13,   124,  2591,   320,   321,
    2594,    19,   119,    21,   486,  1389,   352,   353,  2602,   486,
    1953,    90,    91,   486,   272,  1706,  1400,   487,   488,   219,
     488,   358,  1406,   345,   346,   486,   105,   106,   156,   297,
     109,  2517,  2518,  1417,   278,   486,  2292,   311,   486,   118,
     119,   216,   121,   218,  1428,   245,   272,   187,   188,   297,
    2079,   486,  2081,   181,   171,  1113,   487,   488,   486,   199,
     200,  1113,   486,  1115,  3051,  2447,   476,   477,   478,   479,
     480,   481,   482,   483,   166,  1607,    67,   524,   525,   107,
     527,   491,   161,   486,   112,   486,   114,   486,   350,   407,
     712,   713,   714,   715,   716,   350,  2747,   487,   488,   216,
     486,   218,   487,   488,   221,  1802,  1803,   235,   486,  2052,
     238,  2762,   487,   488,  1166,   143,  1168,   234,   146,   437,
     487,   488,   486,   202,   252,   486,  1178,   486,   207,   486,
    1182,   184,  1536,   161,  2208,   487,   488,   353,   354,  1523,
    1192,  2084,   270,   171,   486,   262,   274,   275,  1200,   407,
     350,   486,   137,   232,   486,  1182,   141,   144,  1855,  1211,
    1857,   486,   241,   242,   243,   493,   487,   488,   216,   154,
     488,  2245,   487,   488,   486,  1227,  1228,   486,   257,   437,
     297,   407,   487,   488,  1211,  1844,   486,  1846,   487,   488,
     487,   488,   486,  1245,   487,   488,   487,   182,   487,   488,
    1252,  1228,   486,  1261,   486,   284,   234,  1259,   486,  1261,
     221,   437,  1264,   135,   487,   488,   486,   296,  1245,  1916,
     486,  1918,   487,   488,   301,  1252,   487,   488,   159,   214,
     488,    86,     3,   218,   219,   487,   488,   486,   317,   267,
    2923,   487,   488,  3007,  3008,   486,  1905,   486,  1907,  2855,
    2856,   486,   486,    24,   486,    26,  2512,  2513,   486,   486,
     245,    32,   488,    34,   486,   486,   486,   486,   486,   297,
    1322,  1329,  1324,    44,   486,  1333,   486,  1329,   486,  1331,
     486,  1333,     6,     7,     8,   486,    10,  1339,   486,    13,
      14,    15,    16,   486,   279,   486,   486,   486,   283,   486,
    1358,   486,   486,   382,   486,   486,  1358,   486,   486,   294,
     486,    68,   486,   486,   486,   486,  2259,  2260,  2504,    90,
      91,   486,   486,   486,   486,   404,  2269,  2978,   486,  2980,
     486,   486,   486,  1982,   486,   106,   321,   486,   486,   486,
     486,   486,   486,   486,   115,   486,   356,   118,   119,   428,
     121,   486,   486,   486,   125,   126,   486,   486,   279,   130,
     345,   346,   486,   486,   486,   350,   486,   486,   486,   459,
    3036,   486,   486,   486,  3025,   486,  3027,   486,  2027,   487,
     487,   487,   127,   487,   487,    22,   177,   486,   493,   160,
     161,   479,   487,    56,   179,   102,   144,  2276,   181,   102,
     296,   488,   480,    45,   486,  2276,   270,   181,   177,   488,
     285,   389,   270,    56,   274,   139,   329,   260,   181,   383,
     304,    36,   274,   277,   117,  2681,  2682,   235,   224,  2282,
     488,   202,   221,   221,   135,  2378,   207,  2380,   487,   493,
     492,   426,  2139,   488,   488,   402,   488,   102,   488,   492,
     306,   487,   487,   480,   487,   487,   487,   228,   487,  1863,
     486,   232,   487,   487,   487,   144,   488,   487,   487,   218,
     241,   242,   243,   487,   487,     3,   488,   493,   249,   487,
     487,   487,    10,   487,   487,   487,   257,   487,  2431,  2345,
     487,   487,   487,   478,   479,   487,   163,   487,  2398,   487,
    1904,   487,    56,  2459,   487,  1557,   488,  1559,  2060,   493,
    1562,    39,   493,   284,   486,   486,   493,  1901,   488,  2216,
    1924,   488,   167,   279,   480,   296,   389,    55,   488,    82,
    1582,    71,  1559,    34,   274,  1562,   486,   488,   280,   488,
     488,   305,  1594,    10,    10,    10,    10,    10,    10,    10,
    1602,    10,    10,  1605,    10,  1607,    10,    10,    10,    10,
      88,    10,   488,   290,   221,   144,   100,  1594,   488,   244,
     487,   405,   379,   219,   177,   219,   487,  1629,  1605,   219,
     487,   493,   177,   486,   493,   144,   487,  1639,  1640,    90,
      91,    70,  1650,   486,  1652,  1653,    34,   280,  1650,   493,
    1652,  1653,   493,   148,   105,   106,   144,   493,   109,   100,
     168,   382,  1639,  1640,   142,   493,   112,   118,   119,   280,
     121,   133,   170,   151,   152,   276,   170,   276,   137,   133,
     301,   159,   141,   404,   493,   488,   164,    55,   166,   488,
     486,   169,   328,   328,   465,   154,   487,   418,   487,   487,
     487,   487,    90,    91,   487,   459,   487,   428,   487,   487,
     161,  1713,   487,   487,   487,  2834,  2835,  2836,   106,   488,
     487,   228,   218,   182,   488,   487,   177,   218,   488,   307,
     118,   119,   210,   121,   486,  2334,   179,   144,   166,  1741,
     306,   488,  1744,   221,   169,   169,   229,   224,   480,   175,
     228,   202,   109,   171,   291,   214,   207,   486,   486,   218,
     219,   171,   493,   486,  1741,   480,   279,   488,   371,   372,
     373,   374,   375,   161,   221,   487,   487,   219,   238,   487,
     261,   232,   260,   487,  2677,   219,   245,   219,   159,   492,
     241,   242,   243,   487,   272,   398,   399,   400,   401,    83,
    2154,   167,   493,   187,   487,   407,   257,   488,   169,   169,
     413,   414,   415,   416,   202,   144,   419,  2719,   296,   207,
     279,   486,   300,   301,   283,   486,   488,   305,   487,   171,
     171,   487,    88,   284,   486,   294,   439,   440,   128,   487,
     329,   487,   417,    47,   232,   296,   487,   166,   488,   487,
     487,   329,   174,   241,   242,   243,   488,   151,   461,   462,
     488,   441,   321,    10,   229,   169,   317,   169,   306,   257,
     244,    68,   488,   488,  2767,   119,   486,   119,   486,   279,
     486,   279,  1884,   336,   493,   275,   345,   346,   486,   486,
     100,   350,  2794,   152,   111,   486,   284,   486,  1677,   487,
     486,   417,  1681,   169,  1683,  1684,  1685,  2241,   296,  1688,
    2509,   389,   486,   219,   487,  1694,  1695,  1696,  1697,  1698,
    1699,   279,  1701,  1702,  1703,  1704,  1705,   279,  1707,   407,
     279,   382,   487,   487,   219,   486,   488,   486,   486,   486,
     219,   218,    83,   119,   171,   171,   219,  2546,   219,   354,
     219,  1953,  1954,   404,   219,   219,   434,    16,   354,   437,
     486,    87,  2459,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   171,   428,   178,  3098,
     301,   487,   487,   301,   486,   224,   487,   487,   302,  1991,
     426,   488,    83,   100,   382,  1997,  1998,   219,   100,   131,
     487,  2003,   487,  2005,   100,  2007,   188,   487,   487,   487,
     488,   487,  2014,   221,   486,   486,   404,   219,   487,   478,
     479,   100,   100,   100,    10,  2027,   487,   246,    84,   487,
    2038,  2039,   488,   488,   159,   486,  2038,  2039,   219,  2041,
     428,    87,   434,    87,   178,  2047,   486,   486,  2050,  2051,
     192,    10,   487,  2055,  2056,   185,   186,   187,   188,   189,
     190,   191,  1759,   193,   194,   195,   196,   197,   198,   199,
     200,  2079,  1732,  2081,  2795,  2077,  1313,  2079,  1287,  2081,
     332,  2083,  2632,  1709,  2842,  2087,  2088,  2572,  2115,  2995,
    2996,   488,  2998,   487,  1713,  2655,  1678,  2563,  2090,  2045,
    2077,  2570,  2908,  2580,  2580,  2628,  2874,  2736,  2779,  2770,
    2566,  2324,  2114,  2115,  3013,  2714,  2963,  3022,  2866,  2910,
    3026,   167,  1317,  2880,  2086,  2373,  1598,  1943,  1627,  3031,
    1222,   177,  2251,  1929,  1639,  1944,   918,  2114,   823,  1228,
    3046,  2934,  3048,  3049,  1572,  1359,  1974,   617,  2944,   594,
    3101,   546,   545,  2821,  2674,  1949,   967,  1264,   585,     3,
    1764,     5,  3084,  2026,  2541,  2714,  2540,  2509,  3068,   544,
     216,   522,   935,  1322,  1338,  2250,  2919,  2928,  2988,  1170,
     226,   227,    26,    27,  3002,  2771,    30,  2546,    32,  1178,
     577,    35,  1644,  2016,    38,  2331,  2531,    41,  2712,    43,
      44,   247,    46,  1562,    48,    49,    50,   539,    52,    53,
     906,    55,  1246,  2832,  1331,  2309,    60,    61,    62,  1252,
      64,    65,  2837,    67,    68,    69,    70,    -1,    -1,    73,
      -1,    -1,    76,    -1,    -1,    -1,    80,    81,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,  2251,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,   106,   107,   310,    -1,    -1,  2269,    -1,    -1,
      -1,    -1,    -1,    -1,  2251,    -1,    -1,    -1,   122,   123,
      -1,    -1,  2061,    -1,    -1,    -1,    -1,    -1,  2067,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   362,   148,    -1,    -1,  2309,    -1,    -1,
      -1,    -1,   358,    -1,    -1,    -1,  2324,    -1,    -1,    -1,
     379,    -1,  2324,    -1,    -1,    -1,    -1,    -1,    -1,  2331,
      -1,  2665,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   403,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2363,    -1,    -1,    -1,    -1,    -1,    -1,  2370,  2377,
      -1,    -1,    -1,    -1,   433,  2377,    -1,    -1,    -1,    -1,
      -1,  2878,    -1,  2880,    -1,    -1,    -1,  2986,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2398,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   249,    -1,    -1,    -1,   253,
    2412,   255,    -1,  2415,    -1,    -1,   260,  2419,    -1,  2421,
      -1,    -1,    -1,    -1,   483,   484,   485,   486,   487,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,  2415,  2447,
      -1,   487,   488,    -1,  2421,  2447,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   300,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2852,  2853,  2854,    -1,    -1,  3068,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2490,    -1,  2888,  3084,    -1,    -1,  2490,    -1,
      -1,    -1,    -1,   337,   338,  2497,    -1,    -1,  2995,  2996,
      -1,  2998,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,  2521,
     579,    -1,    -1,  2525,    -1,    -1,    -1,    -1,    -1,  3026,
      -1,    -1,    -1,   592,    -1,    -1,   595,    -1,    -1,    -1,
      -1,    -1,   601,   602,  2546,   604,   605,   606,   607,  3046,
      -1,  3048,  3049,    -1,  2556,   614,   615,   616,    -1,    -1,
      -1,  2563,    -1,   622,    -1,   624,   625,   626,    -1,    -1,
    2572,   630,  2976,    -1,    -1,   634,    -1,   636,    -1,   638,
      87,   640,    -1,    -1,    -1,    -1,    -1,    -1,   647,   648,
      -1,   650,   436,    -1,   653,   654,   655,   656,   657,   658,
     659,   660,   661,   662,    -1,   664,   665,   666,   667,   668,
     669,    -1,    -1,    -1,    -1,   674,   675,    -1,   677,   678,
     679,    -1,   681,   682,   683,   684,   685,   686,   687,   688,
     689,    -1,    -1,    -1,   693,  2637,   695,   696,   697,   698,
     699,    -1,   486,   702,   703,   704,   705,   706,   707,   708,
     709,   710,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   718,
     167,    -1,    -1,    -1,   723,   724,   725,    -1,    -1,    -1,
     177,    -1,    -1,    -1,   733,    -1,    -1,    -1,   737,    -1,
     739,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   747,   748,
     749,   750,    -1,    -1,    -1,   754,   755,   756,   757,    -1,
     759,   760,   761,   762,   763,   764,   765,   766,   767,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2730,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     247,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2770,    -1,    -1,    -1,    -1,    -1,  2770,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
    2792,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    -1,  2808,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,  2826,    26,    -1,    -1,    -1,    -1,
      -1,  2808,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    46,    -1,  2849,  2850,    50,
      -1,   358,    -1,    87,    55,    -1,    -1,    -1,    -1,    -1,
      -1,  2863,    63,    -1,    -1,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    83,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2910,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    -1,   167,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    44,  2944,   177,    -1,    -1,    -1,   148,   216,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     161,    -1,    -1,    -1,   105,   106,    -1,    -1,   109,   476,
     477,   478,   479,   480,   481,   482,   483,   118,   119,   247,
     121,    -1,   216,    -1,   491,    -1,    -1,    90,    91,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,  3007,  3008,    -1,  3010,    -1,
      -1,    -1,   115,   247,    -1,   118,   119,    -1,   121,    -1,
     161,  3023,   125,    -1,  1083,  1084,  1085,   130,    -1,    -1,
      -1,    -1,    -1,   301,    -1,   236,    -1,    -1,    -1,    -1,
    3042,    -1,   310,    -1,    -1,   246,    -1,   248,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1114,    -1,    -1,   161,  1118,
      -1,   202,    -1,    -1,    -1,   266,   207,    -1,    -1,    -1,
      -1,    -1,  1131,    -1,    -1,    -1,   310,    -1,    -1,    -1,
      -1,    -1,  3084,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   232,    -1,    -1,  1153,    -1,    -1,   298,    -1,   202,
     241,   242,   243,    -1,   207,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   257,    -1,    -1,  1178,
      -1,    -1,    -1,    -1,   358,   228,    -1,    -1,    -1,   232,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   241,   242,
     243,    -1,    -1,   284,    -1,    -1,   249,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   257,   296,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   317,    -1,    -1,    -1,
      -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,    -1,
     488,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,   382,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,   487,   488,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   404,    -1,    -1,    -1,  1326,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   382,
      -1,    -1,  1341,    -1,  1343,    -1,    -1,   428,  1347,    -1,
      -1,    -1,    -1,    -1,  1353,  1354,    -1,    -1,    -1,    -1,
      -1,   404,    -1,    -1,    -1,    -1,  1365,    -1,  1367,    -1,
      -1,    -1,  1371,  1372,   167,   418,    -1,    -1,    -1,    -1,
    1379,    -1,    -1,  1382,   177,   428,    -1,    -1,  1387,    -1,
      -1,  1390,    -1,    -1,    -1,    -1,    -1,    -1,  1397,    -1,
    1399,    -1,    -1,    -1,  1403,  1404,  1405,    -1,    -1,    -1,
    1409,    -1,  1411,    -1,  1413,    -1,  1415,    -1,    -1,    -1,
    1419,    -1,  1421,   216,  1423,    -1,  1425,    -1,  1427,    -1,
    1429,    -1,    -1,   226,   227,  1434,    -1,  1436,    -1,  1438,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1446,  1447,    -1,
    1449,    -1,  1451,  1452,   247,    -1,  1455,  1456,  1457,  1458,
    1459,    -1,    -1,  1462,  1463,  1464,  1465,    -1,  1467,  1468,
    1469,  1470,  1471,  1472,  1473,  1474,  1475,    -1,  1477,  1478,
      -1,  1480,    -1,  1482,  1483,  1484,  1485,  1486,    -1,  1488,
      -1,  1490,    -1,  1492,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1503,    -1,    -1,  1506,  1507,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1528,
    1529,    -1,    -1,  1532,  1533,  1534,  1535,    -1,    -1,    -1,
    1539,  1540,  1541,  1542,    -1,  1544,  1545,  1546,  1547,  1548,
    1549,  1550,  1551,  1552,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,  1565,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    87,    -1,   487,   488,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    -1,    46,    -1,    -1,    -1,    50,   167,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,   177,    -1,    63,
     167,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,
     177,    -1,    -1,   167,    -1,    79,    -1,    -1,    -1,    83,
      -1,    -1,    86,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1770,   167,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,   167,   226,   227,    -1,   216,
      -1,    -1,    -1,    -1,    -1,   177,    -1,  1796,    -1,   226,
     227,    -1,   216,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,   226,   227,   138,    -1,    -1,    -1,    -1,    -1,
     247,   216,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   247,   216,    -1,  1835,   161,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,  1845,    -1,    -1,    -1,
      -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,  1867,    -1,
     310,    -1,    -1,    -1,  1873,    -1,  1875,    -1,  1877,    -1,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1891,    -1,    -1,  1894,   310,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1906,    -1,    -1,
      -1,    -1,   236,    -1,    -1,   310,    -1,    -1,   358,    -1,
      -1,    -1,   246,    -1,   248,    -1,    -1,    -1,   310,    -1,
      -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   266,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   298,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2010,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  2034,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,   488,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
     487,   488,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,   487,   488,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,   487,   488,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,   488,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    -1,    46,    -1,    -1,    -1,    50,  2138,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,
    2159,    -1,    -1,    -1,    -1,  2164,  2165,    79,  2167,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,  2176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2188,
      -1,    -1,    -1,  2192,    -1,    -1,  2195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    2209,    -1,    -1,    -1,  2213,    -1,    -1,    -1,    -1,  2218,
      -1,    -1,  2221,  2222,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2246,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    2319,    -1,    -1,    -1,   236,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2332,   246,    -1,   248,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2346,    -1,    -1,
      -1,    -1,    -1,    -1,   266,    -1,    -1,    -1,    -1,  2358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   298,    -1,    -1,    25,
      -1,    -1,    28,    29,    30,    31,    -1,    33,    34,    35,
      36,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    54,    -1,
      56,    -1,    -1,    59,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    73,    -1,    -1,
      76,    -1,    78,    79,    80,    -1,    82,    -1,    84,    85,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,   101,   102,  2466,    -1,   105,
     106,    -1,   108,    -1,   110,    -1,    -1,   113,   114,    -1,
      -1,  2480,   118,    -1,    -1,   121,    -1,    -1,   124,   125,
     126,    -1,   128,   129,   130,    -1,    -1,   133,   134,    -1,
     136,    -1,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
     146,   147,    -1,   149,    -1,  2514,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,   160,   161,   162,    -1,    -1,   165,
      -1,    -1,   168,    -1,    -1,   171,    -1,    -1,   174,   175,
      -1,   177,    -1,   179,    -1,   181,    -1,    -1,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,   203,   204,   205,
     206,   207,   208,   209,    -1,   211,   212,   213,    -1,   215,
      -1,   217,    -1,    -1,   220,    -1,   222,   223,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   231,   232,   233,    -1,    -1,
     236,   237,   238,   239,   240,   241,   242,   243,    -1,    -1,
      -1,    -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,
     256,   257,   258,   259,    -1,    -1,   262,   263,   264,   265,
     266,   267,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,
     276,   277,    -1,  2642,    -1,    -1,   282,    -1,   284,   285,
    2649,    -1,    -1,  2652,   290,    -1,   292,   293,    -1,   295,
      -1,    -1,    -1,   299,    -1,    -1,   302,    -1,   304,    -1,
      -1,    -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,
      -1,    -1,   318,    -1,   320,   321,    -1,    -1,   324,   325,
      -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   338,   339,    -1,   341,    -1,   343,   344,   345,
     346,    -1,    -1,  2712,    -1,    -1,    -1,    -1,    -1,    -1,
     356,    -1,    -1,  2722,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,
      -1,    -1,    -1,    -1,    -1,    -1,   382,   383,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,
      -1,   397,    -1,    -1,    -1,   401,   402,    -1,   404,    -1,
      -1,    -1,    -1,   409,    -1,    -1,    -1,   413,    -1,    -1,
     416,   417,   418,   419,    -1,    -1,    -1,   423,   424,   425,
      -1,   427,   428,    -1,   430,    -1,   432,    -1,    -1,    -1,
      -1,    -1,    -1,   439,    -1,   441,   442,    -1,    -1,    -1,
    2809,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,    -1,
      -1,    -1,    -1,  2822,   460,  2824,   462,    -1,    -1,   465,
      -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   493,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2916,    -1,    -1,
      -1,    -1,    -1,  2922,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,
      33,    34,    35,    36,    37,    -1,    -1,    40,    -1,    -1,
      43,  2990,    45,    46,    47,    48,    49,    50,    51,    -1,
      -1,    54,    -1,    56,    57,    58,    59,  3006,    -1,    62,
      63,    64,    65,    66,    67,    -1,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,   101,   102,
      -1,   104,   105,   106,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,   118,   119,    -1,   121,    -1,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,   145,    -1,   147,  3094,   149,   150,    -1,    -1,
     153,   154,    -1,   156,   157,   158,    -1,   160,    -1,   162,
      -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,   172,
     173,   174,   175,    -1,    -1,    -1,   179,   180,   181,   182,
      -1,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,    -1,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,    -1,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,
     233,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,   245,    -1,    -1,   248,    -1,   250,    -1,   252,
     253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,
     263,   264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,
     273,    -1,    -1,   276,   277,    -1,   279,    -1,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,
     303,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,
     313,   314,    -1,    -1,   317,   318,   319,   320,   321,    -1,
      -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,
     343,   344,   345,   346,    -1,    -1,    -1,   350,    -1,    -1,
      -1,    -1,    -1,   356,   357,    -1,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,    -1,    -1,
      -1,    -1,   375,   376,   377,   378,    -1,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,   390,   391,   392,
     393,   394,   395,   396,   397,    -1,    -1,    -1,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,    -1,    -1,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,    -1,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,    -1,   460,    -1,   462,
     463,   464,   465,    -1,    -1,   468,    -1,   470,    -1,   472,
     473,   474,    -1,    -1,    -1,   478,   479,    -1,    -1,    -1,
      -1,   484,    -1,   486,    -1,    -1,   489,   490,    -1,   492,
     493,   494,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,
      -1,    33,    34,    35,    36,    37,    -1,    -1,    40,    -1,
      -1,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      -1,    -1,    54,    -1,    56,    57,    58,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    -1,    84,    85,    86,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,   104,   105,   106,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,   115,   116,    -1,   118,   119,    -1,   121,
      -1,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,    -1,
      -1,   153,   154,    -1,   156,   157,   158,    -1,   160,    -1,
     162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,
      -1,   173,   174,   175,    -1,    -1,    -1,   179,   180,   181,
     182,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,    -1,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,
     232,   233,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,    -1,   245,    -1,    -1,   248,    -1,   250,    -1,
     252,   253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,
     262,   263,   264,   265,   266,    -1,    -1,    -1,   270,   271,
      -1,   273,    -1,    -1,   276,   277,    -1,   279,    -1,    -1,
     282,   283,   284,   285,   286,   287,   288,   289,   290,    -1,
     292,   293,   294,   295,    -1,    -1,    -1,   299,    -1,    -1,
     302,   303,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,
     312,   313,   314,    -1,    -1,   317,   318,   319,   320,   321,
      -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,
      -1,   343,   344,   345,   346,    -1,    -1,    -1,   350,    -1,
      -1,    -1,    -1,    -1,   356,   357,    -1,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,    -1,
      -1,    -1,    -1,   375,   376,   377,   378,    -1,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,   390,   391,
     392,   393,   394,   395,   396,   397,    -1,    -1,    -1,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,    -1,    -1,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,    -1,   435,   436,   437,   438,   439,    -1,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,    -1,   460,    -1,
     462,   463,   464,   465,    -1,    -1,   468,    -1,   470,    -1,
     472,   473,   474,    -1,    -1,    -1,   478,   479,    -1,    -1,
      -1,    -1,   484,    -1,   486,    -1,    -1,   489,   490,    -1,
     492,   493,   494,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    29,    30,
      31,    -1,    33,    34,    35,    36,    37,    -1,    -1,    40,
      -1,    -1,    43,    -1,    45,    46,    47,    48,    49,    50,
      51,    -1,    -1,    54,    -1,    56,    57,    58,    59,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
     101,   102,    -1,   104,   105,   106,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,   115,   116,    -1,   118,   119,    -1,
     121,    -1,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,   138,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,
      -1,    -1,   153,   154,    -1,   156,   157,   158,    -1,   160,
      -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,
      -1,    -1,   173,   174,   175,    -1,    -1,    -1,   179,   180,
     181,   182,    -1,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,    -1,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
      -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     231,   232,   233,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,    -1,   245,    -1,    -1,   248,    -1,   250,
      -1,   252,   253,    -1,    -1,   256,   257,   258,   259,    -1,
      -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,   270,
     271,    -1,   273,    -1,    -1,   276,   277,    -1,   279,    -1,
      -1,   282,   283,   284,   285,   286,   287,   288,   289,   290,
      -1,   292,   293,   294,   295,    -1,    -1,    -1,   299,    -1,
      -1,   302,   303,   304,    -1,    -1,    -1,    -1,   309,    -1,
     311,   312,   313,   314,    -1,    -1,   317,   318,   319,   320,
     321,    -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,
     341,    -1,   343,   344,   345,   346,    -1,    -1,    -1,   350,
      -1,    -1,    -1,    -1,    -1,   356,   357,    -1,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
      -1,    -1,    -1,    -1,   375,   376,   377,   378,    -1,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,   390,
     391,   392,   393,   394,   395,   396,   397,    -1,    -1,    -1,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,    -1,    -1,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,   433,    -1,   435,   436,   437,   438,   439,    -1,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,    -1,   460,
      -1,   462,   463,   464,   465,    -1,    -1,   468,    -1,   470,
      -1,   472,   473,   474,    -1,    -1,    -1,   478,   479,    -1,
      -1,    -1,    -1,   484,    -1,   486,    -1,    -1,   489,   490,
      -1,   492,   493,   494,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    29,
      30,    31,    -1,    33,    34,    35,    36,    37,    -1,    -1,
      40,    -1,    -1,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    -1,    -1,    54,    -1,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    65,    66,    67,    -1,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,   101,   102,    -1,   104,   105,   106,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,   118,   119,
      -1,   121,    -1,    -1,   124,   125,   126,    -1,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,
     150,    -1,    -1,   153,   154,    -1,   156,   157,   158,    -1,
     160,    -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,
      -1,    -1,    -1,   173,   174,   175,    -1,    -1,    -1,   179,
     180,   181,   182,    -1,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
      -1,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,    -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   231,   232,   233,    -1,    -1,   236,   237,   238,   239,
     240,   241,   242,   243,    -1,   245,    -1,    -1,   248,    -1,
     250,    -1,   252,   253,    -1,    -1,   256,   257,   258,   259,
      -1,    -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,
     270,   271,    -1,   273,    -1,    -1,   276,   277,    -1,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,   288,   289,
     290,    -1,   292,   293,   294,   295,    -1,    -1,    -1,   299,
      -1,    -1,   302,   303,   304,    -1,    -1,    -1,    -1,   309,
      -1,   311,   312,   313,   314,    -1,    -1,   317,   318,   319,
     320,   321,    -1,    -1,   324,   325,    -1,   327,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,
      -1,   341,    -1,   343,   344,   345,   346,    -1,    -1,    -1,
     350,    -1,    -1,    -1,    -1,    -1,   356,   357,    -1,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,    -1,    -1,    -1,    -1,   375,   376,   377,   378,    -1,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
     390,   391,   392,   393,   394,   395,   396,   397,    -1,    -1,
      -1,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,    -1,    -1,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,    -1,   435,   436,   437,   438,   439,
      -1,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,    -1,
     460,    -1,   462,   463,   464,   465,    -1,    -1,   468,    -1,
     470,    -1,   472,   473,   474,    -1,    -1,    -1,   478,   479,
      -1,    -1,    -1,    -1,   484,    -1,   486,   487,    -1,   489,
     490,    -1,   492,   493,   494,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,
      29,    30,    31,    -1,    33,    34,    35,    36,    37,    -1,
      -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    -1,    -1,    54,    -1,    56,    57,    58,
      59,    -1,    -1,    62,    63,    64,    65,    66,    67,    -1,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    -1,    84,    85,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,   104,   105,   106,    -1,   108,
      -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,   118,
     119,    -1,   121,    -1,    -1,   124,   125,   126,    -1,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,   150,    -1,    -1,   153,   154,    -1,   156,   157,   158,
      -1,   160,    -1,   162,    -1,    -1,   165,    -1,    -1,   168,
      -1,    -1,    -1,    -1,   173,   174,   175,    -1,    -1,    -1,
     179,   180,   181,   182,    -1,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,    -1,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,    -1,   222,   223,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   231,   232,   233,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,    -1,   245,    -1,    -1,   248,
      -1,   250,    -1,   252,   253,    -1,    -1,   256,   257,   258,
     259,    -1,    -1,   262,   263,   264,   265,   266,    -1,    -1,
      -1,   270,   271,    -1,   273,    -1,    -1,   276,   277,    -1,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,   288,
     289,   290,    -1,   292,   293,   294,   295,    -1,    -1,    -1,
     299,    -1,    -1,   302,   303,   304,    -1,    -1,    -1,    -1,
     309,    -1,   311,   312,   313,   314,    -1,    -1,   317,   318,
     319,   320,   321,    -1,    -1,   324,   325,    -1,   327,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,
     339,    -1,   341,    -1,   343,   344,   345,   346,    -1,    -1,
      -1,   350,    -1,    -1,    -1,    -1,    -1,   356,   357,    -1,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,    -1,    -1,    -1,    -1,   375,   376,   377,   378,
      -1,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,   390,   391,   392,   393,   394,   395,   396,   397,    -1,
      -1,    -1,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   413,    -1,    -1,   416,   417,   418,
     419,   420,   421,   422,   423,   424,   425,   426,   427,   428,
     429,   430,   431,   432,   433,    -1,   435,   436,   437,   438,
     439,    -1,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
      -1,   460,    -1,   462,   463,   464,   465,    -1,    -1,   468,
      -1,   470,    -1,   472,   473,   474,    -1,    -1,    -1,   478,
     479,    -1,    -1,    -1,    -1,   484,    -1,   486,   487,    -1,
     489,   490,    -1,   492,   493,   494,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      28,    29,    30,    31,    -1,    33,    34,    35,    36,    37,
      -1,    -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    -1,    -1,    54,    -1,    56,    57,
      58,    59,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    82,    -1,    84,    85,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,   104,   105,   106,    -1,
     108,    -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,
     118,   119,    -1,   121,    -1,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,    -1,    -1,   145,    -1,   147,
      -1,   149,   150,    -1,    -1,   153,   154,    -1,   156,   157,
     158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,    -1,
     168,    -1,    -1,    -1,    -1,   173,   174,   175,    -1,    -1,
      -1,   179,   180,   181,   182,    -1,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,    -1,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,    -1,   222,   223,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   231,   232,   233,    -1,    -1,   236,   237,
     238,   239,   240,   241,   242,   243,    -1,   245,    -1,    -1,
     248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,   257,
     258,   259,    -1,    -1,   262,   263,   264,   265,   266,    -1,
      -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,   277,
      -1,   279,    -1,    -1,   282,   283,   284,   285,   286,   287,
     288,   289,   290,    -1,   292,   293,   294,   295,    -1,    -1,
      -1,   299,    -1,    -1,   302,   303,   304,    -1,    -1,    -1,
      -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,   317,
     318,   319,   320,   321,    -1,    -1,   324,   325,    -1,   327,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,
     338,   339,    -1,   341,    -1,   343,   344,   345,   346,    -1,
      -1,    -1,   350,    -1,    -1,    -1,    -1,    -1,   356,   357,
      -1,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,    -1,    -1,    -1,    -1,   375,   376,   377,
     378,    -1,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,   390,   391,   392,   393,   394,   395,   396,   397,
      -1,    -1,    -1,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,    -1,    -1,   416,   417,
     418,   419,   420,   421,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,   433,    -1,   435,   436,   437,
     438,   439,    -1,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,   460,    -1,   462,   463,   464,   465,    -1,    -1,
     468,    -1,   470,    -1,   472,   473,   474,    -1,    -1,    -1,
     478,   479,    -1,    -1,    -1,    -1,   484,    -1,   486,   487,
      -1,   489,   490,    -1,   492,   493,   494,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    28,    29,    30,    31,    -1,    33,    34,    35,    36,
      37,    -1,    -1,    40,    -1,    -1,    43,    -1,    45,    46,
      47,    48,    49,    50,    51,    -1,    -1,    54,    -1,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    -1,    84,    85,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,   101,   102,    -1,   104,   105,   106,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,   118,   119,    -1,   121,    -1,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
     137,   138,   139,   140,   141,    -1,    -1,    -1,   145,    -1,
     147,    -1,   149,   150,    -1,    -1,   153,   154,    -1,   156,
     157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,
      -1,   168,    -1,    -1,    -1,    -1,   173,   174,   175,    -1,
      -1,    -1,   179,   180,   181,   182,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,    -1,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,    -1,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   231,   232,   233,    -1,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,    -1,   245,    -1,
      -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,
     257,   258,   259,    -1,    -1,   262,   263,   264,   265,   266,
      -1,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,
     277,    -1,   279,    -1,    -1,   282,   283,   284,   285,   286,
     287,   288,   289,   290,    -1,   292,   293,   294,   295,    -1,
      -1,    -1,   299,    -1,    -1,   302,   303,   304,    -1,    -1,
      -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,
     317,   318,   319,   320,   321,    -1,    -1,   324,   325,    -1,
     327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     337,   338,   339,    -1,   341,    -1,   343,   344,   345,   346,
      -1,    -1,    -1,   350,    -1,    -1,    -1,    -1,    -1,   356,
     357,    -1,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,    -1,    -1,    -1,    -1,   375,   376,
     377,   378,    -1,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,   390,   391,   392,   393,   394,   395,   396,
     397,    -1,    -1,    -1,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,    -1,    -1,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,    -1,   435,   436,
     437,   438,   439,    -1,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,    -1,   460,    -1,   462,   463,   464,   465,    -1,
      -1,   468,    -1,   470,    -1,   472,   473,   474,    -1,    -1,
      -1,   478,   479,    -1,    -1,    -1,    -1,   484,    -1,   486,
     487,    -1,   489,   490,    -1,   492,   493,   494,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    28,    29,    30,    31,    -1,    33,    34,    35,
      36,    37,    -1,    -1,    40,    -1,    -1,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    -1,    -1,    54,    -1,
      56,    57,    58,    59,    -1,    -1,    62,    63,    64,    65,
      66,    67,    -1,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    -1,    84,    85,
      86,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    99,    -1,   101,   102,    -1,   104,   105,
     106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,   118,   119,    -1,   121,    -1,    -1,   124,   125,
     126,    -1,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,   140,   141,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,   150,    -1,    -1,   153,   154,    -1,
     156,   157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,
      -1,    -1,   168,    -1,    -1,    -1,    -1,   173,   174,   175,
      -1,    -1,    -1,   179,   180,   181,   182,    -1,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,    -1,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,    -1,   222,   223,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   231,   232,   233,    -1,    -1,
     236,   237,   238,   239,   240,   241,   242,   243,    -1,   245,
      -1,    -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,
     256,   257,   258,   259,    -1,    -1,   262,   263,   264,   265,
     266,    -1,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,
     276,   277,    -1,   279,    -1,    -1,   282,   283,   284,   285,
     286,   287,   288,   289,   290,    -1,   292,   293,   294,   295,
      -1,    -1,    -1,   299,    -1,    -1,   302,   303,   304,    -1,
      -1,    -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,
      -1,   317,   318,   319,   320,   321,    -1,    -1,   324,   325,
      -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   338,   339,    -1,   341,    -1,   343,   344,   345,
     346,    -1,    -1,    -1,   350,    -1,    -1,    -1,    -1,    -1,
     356,   357,    -1,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,    -1,    -1,    -1,    -1,   375,
     376,   377,   378,    -1,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,   390,   391,   392,   393,   394,   395,
     396,   397,    -1,    -1,    -1,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,    -1,    -1,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,   433,    -1,   435,
     436,   437,   438,   439,    -1,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,    -1,   460,    -1,   462,   463,   464,   465,
      -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,    -1,
      -1,    -1,   478,   479,    -1,    -1,    -1,    -1,   484,    -1,
     486,    -1,    -1,   489,   490,    -1,   492,   493,   494,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    28,    29,    30,    31,    -1,    33,    34,
      35,    36,    37,    -1,    -1,    40,    -1,    -1,    43,    -1,
      45,    46,    47,    48,    49,    50,    51,    -1,    -1,    54,
      -1,    56,    57,    58,    59,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    -1,    84,
      85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,   101,   102,    -1,   104,
     105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,   118,   119,    -1,   121,    -1,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,   144,
     145,    -1,   147,    -1,   149,   150,    -1,    -1,   153,   154,
      -1,   156,   157,   158,    -1,   160,    -1,   162,    -1,    -1,
     165,    -1,    -1,   168,    -1,    -1,    -1,    -1,   173,   174,
     175,    -1,    -1,    -1,   179,   180,   181,   182,    -1,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,    -1,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,    -1,   222,   223,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,    -1,
     245,    -1,    -1,   248,    -1,   250,    -1,   252,   253,    -1,
      -1,   256,   257,   258,   259,    -1,    -1,   262,   263,   264,
     265,   266,    -1,    -1,    -1,   270,   271,    -1,   273,    -1,
      -1,   276,   277,    -1,   279,    -1,    -1,   282,   283,   284,
     285,   286,   287,   288,   289,   290,    -1,   292,   293,   294,
     295,    -1,    -1,    -1,   299,    -1,    -1,   302,   303,   304,
      -1,    -1,    -1,    -1,   309,    -1,   311,   312,   313,   314,
      -1,    -1,   317,   318,   319,   320,   321,    -1,    -1,   324,
     325,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   337,   338,   339,    -1,   341,    -1,   343,   344,
     345,   346,    -1,    -1,    -1,   350,    -1,    -1,    -1,    -1,
      -1,   356,   357,    -1,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,    -1,    -1,    -1,    -1,
     375,   376,   377,   378,    -1,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,   390,   391,   392,   393,   394,
     395,   396,   397,    -1,    -1,    -1,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,    -1,
      -1,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,    -1,
     435,   436,   437,   438,   439,    -1,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,    -1,   460,    -1,   462,   463,   464,
     465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,
      -1,    -1,    -1,   478,   479,    -1,    -1,    -1,    -1,   484,
      -1,   486,    -1,    -1,   489,   490,    -1,   492,   493,   494,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,    33,
      34,    35,    36,    37,    -1,    -1,    40,    -1,    -1,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    -1,    -1,
      54,    -1,    56,    57,    58,    59,    -1,    -1,    62,    63,
      64,    65,    66,    67,    -1,    -1,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    82,    -1,
      84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    99,    -1,   101,   102,    -1,
     104,   105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,   118,   119,    -1,   121,    -1,    -1,
     124,   125,   126,    -1,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,    -1,
     144,   145,    -1,   147,    -1,   149,   150,    -1,    -1,   153,
     154,    -1,   156,   157,   158,    -1,   160,    -1,   162,    -1,
      -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,   173,
     174,   175,    -1,    -1,    -1,   179,   180,   181,   182,    -1,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,    -1,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,    -1,   222,   223,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,
      -1,    -1,   236,   237,   238,   239,   240,   241,   242,   243,
      -1,   245,    -1,    -1,   248,    -1,   250,    -1,   252,   253,
      -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,   263,
     264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,   273,
      -1,    -1,   276,   277,    -1,   279,    -1,    -1,   282,   283,
     284,   285,   286,   287,   288,   289,   290,    -1,   292,   293,
     294,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,   303,
     304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,   313,
     314,    -1,    -1,   317,   318,   319,   320,   321,    -1,    -1,
     324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,   343,
     344,   345,   346,    -1,    -1,    -1,   350,    -1,    -1,    -1,
      -1,    -1,   356,   357,    -1,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,    -1,    -1,    -1,
      -1,   375,   376,   377,   378,    -1,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,   390,   391,   392,   393,
     394,   395,   396,   397,    -1,    -1,    -1,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
      -1,    -1,   416,   417,   418,   419,   420,   421,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   432,   433,
      -1,   435,   436,   437,   438,   439,    -1,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,    -1,   460,    -1,   462,   463,
     464,   465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,
     474,    -1,    -1,    -1,   478,   479,    -1,    -1,    -1,    -1,
     484,    -1,   486,    -1,    -1,   489,   490,    -1,   492,   493,
     494,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,
      33,    34,    35,    36,    37,    -1,    -1,    40,    -1,    -1,
      43,    -1,    45,    46,    47,    48,    49,    50,    51,    -1,
      -1,    54,    -1,    56,    57,    58,    59,    -1,    -1,    62,
      63,    64,    65,    66,    67,    -1,    -1,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    82,
      -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,   104,   105,   106,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,   118,   119,    -1,   121,    -1,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,   144,   145,    -1,   147,    -1,   149,   150,    -1,    -1,
     153,   154,    -1,   156,   157,   158,    -1,   160,    -1,   162,
      -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,
     173,   174,   175,    -1,    -1,    -1,   179,   180,   181,   182,
      -1,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,    -1,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,    -1,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,
     233,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,   245,    -1,    -1,   248,    -1,   250,    -1,   252,
     253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,
     263,   264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,
     273,    -1,    -1,   276,   277,    -1,   279,    -1,    -1,   282,
     283,   284,   285,   286,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,
     303,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,
     313,   314,    -1,    -1,   317,   318,   319,   320,   321,    -1,
      -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,
     343,   344,   345,   346,    -1,    -1,    -1,   350,    -1,    -1,
      -1,    -1,    -1,   356,   357,    -1,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,    -1,    -1,
      -1,    -1,   375,   376,   377,   378,    -1,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,   390,   391,   392,
     393,   394,   395,   396,   397,    -1,    -1,    -1,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,    -1,    -1,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,    -1,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,    -1,   460,    -1,   462,
     463,   464,   465,    -1,    -1,   468,    -1,   470,    -1,   472,
     473,   474,    -1,    -1,    -1,   478,   479,    -1,    -1,    -1,
      -1,   484,    -1,   486,    -1,    -1,   489,   490,    -1,   492,
     493,   494,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,
      -1,    33,    34,    35,    36,    37,    -1,    -1,    40,    -1,
      -1,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      -1,    -1,    54,    -1,    56,    57,    58,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    -1,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,   104,   105,   106,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,   115,   116,    -1,   118,   119,    -1,   121,
      -1,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,    -1,
      -1,   153,   154,    -1,   156,   157,   158,    -1,   160,    -1,
     162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,
      -1,   173,   174,   175,    -1,    -1,    -1,   179,   180,   181,
     182,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,    -1,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,    -1,
     222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,
     232,   233,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,    -1,   245,    -1,    -1,   248,    -1,   250,    -1,
     252,   253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,
     262,   263,   264,   265,   266,    -1,    -1,    -1,   270,   271,
      -1,   273,    -1,    -1,   276,   277,    -1,   279,    -1,    -1,
     282,   283,   284,   285,   286,   287,   288,   289,   290,    -1,
     292,   293,   294,   295,    -1,    -1,    -1,   299,    -1,    -1,
     302,   303,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,
     312,   313,   314,    -1,    -1,   317,   318,   319,   320,   321,
      -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,
      -1,   343,   344,   345,   346,    -1,    -1,    -1,   350,    -1,
      -1,    -1,    -1,    -1,   356,   357,    -1,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,    -1,
      -1,    -1,    -1,   375,   376,   377,   378,    -1,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,   390,   391,
     392,   393,   394,   395,   396,   397,    -1,    -1,    -1,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,    -1,    -1,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,    -1,   435,   436,   437,   438,   439,    -1,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,    -1,   460,    -1,
     462,   463,   464,   465,    -1,    -1,   468,    -1,   470,    -1,
     472,   473,   474,    -1,    -1,    -1,   478,   479,    -1,    -1,
      -1,    -1,   484,    -1,   486,    -1,    -1,   489,   490,    -1,
     492,   493,   494,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    29,    30,
      31,    -1,    33,    34,    35,    36,    37,    -1,    -1,    40,
      -1,    -1,    43,    -1,    45,    46,    47,    48,    49,    50,
      51,    -1,    -1,    54,    -1,    56,    57,    58,    59,    -1,
      -1,    62,    63,    64,    65,    66,    67,    -1,    -1,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    -1,    99,    -1,
     101,   102,    -1,   104,   105,   106,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,   115,   116,    -1,   118,   119,    -1,
     121,    -1,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,   137,   138,   139,   140,
     141,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,   150,
      -1,    -1,   153,   154,    -1,   156,   157,   158,    -1,   160,
      -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,
      -1,    -1,   173,   174,   175,    -1,    -1,    -1,   179,   180,
     181,   182,    -1,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,    -1,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
      -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     231,   232,   233,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,    -1,   245,    -1,    -1,   248,    -1,   250,
      -1,   252,   253,    -1,    -1,   256,   257,   258,   259,    -1,
      -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,   270,
     271,    -1,   273,    -1,    -1,   276,   277,    -1,   279,    -1,
      -1,   282,   283,   284,   285,   286,   287,   288,   289,   290,
      -1,   292,   293,   294,   295,    -1,    -1,    -1,   299,    -1,
      -1,   302,   303,   304,    -1,    -1,    -1,    -1,   309,    -1,
     311,   312,   313,   314,    -1,    -1,   317,   318,   319,   320,
     321,    -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,
     341,    -1,   343,   344,   345,   346,    -1,    -1,    -1,   350,
      -1,    -1,    -1,    -1,    -1,   356,   357,    -1,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
      -1,    -1,    -1,    -1,   375,   376,   377,   378,    -1,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,   390,
     391,   392,   393,   394,   395,   396,   397,    -1,    -1,    -1,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,    -1,    -1,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,   433,    -1,   435,   436,   437,   438,   439,    -1,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,    -1,   460,
      -1,   462,   463,   464,   465,    -1,    -1,   468,    -1,   470,
      -1,   472,   473,   474,    -1,    -1,    -1,   478,   479,    -1,
      -1,    -1,    -1,   484,    -1,   486,    -1,    -1,   489,   490,
      -1,   492,   493,   494,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    29,
      30,    31,    -1,    33,    34,    35,    36,    37,    -1,    -1,
      40,    -1,    -1,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    -1,    -1,    54,    -1,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    65,    66,    67,    -1,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    99,
      -1,   101,   102,    -1,   104,   105,   106,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,   118,   119,
      -1,   121,    -1,    -1,   124,   125,   126,    -1,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
     140,   141,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,
     150,    -1,    -1,   153,   154,    -1,   156,   157,   158,    -1,
     160,    -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,
      -1,    -1,    -1,   173,   174,   175,    -1,    -1,    -1,   179,
     180,   181,   182,    -1,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
      -1,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,    -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   231,   232,   233,    -1,    -1,   236,   237,   238,   239,
     240,   241,   242,   243,    -1,   245,    -1,    -1,   248,    -1,
     250,    -1,   252,   253,    -1,    -1,   256,   257,   258,   259,
      -1,    -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,
     270,   271,    -1,   273,    -1,    -1,   276,   277,    -1,   279,
      -1,    -1,   282,   283,   284,   285,   286,   287,   288,   289,
     290,    -1,   292,   293,   294,   295,    -1,    -1,    -1,   299,
      -1,    -1,   302,   303,   304,    -1,    -1,    -1,    -1,   309,
      -1,   311,   312,   313,   314,    -1,    -1,   317,   318,   319,
     320,   321,    -1,    -1,   324,   325,    -1,   327,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,
      -1,   341,    -1,   343,   344,   345,   346,    -1,    -1,    -1,
     350,    -1,    -1,    -1,    -1,    -1,   356,   357,    -1,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,    -1,    -1,    -1,    -1,   375,   376,   377,   378,    -1,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
     390,   391,   392,   393,   394,   395,   396,   397,    -1,    -1,
      -1,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,    -1,    -1,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,    -1,   435,   436,   437,   438,   439,
      -1,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,    -1,
     460,    -1,   462,   463,   464,   465,    -1,    -1,   468,    -1,
     470,    -1,   472,   473,   474,    -1,    -1,    -1,   478,   479,
      -1,    -1,    -1,    -1,   484,    -1,   486,    -1,    -1,   489,
     490,    -1,   492,   493,   494,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,
      29,    30,    31,    -1,    33,    34,    35,    36,    37,    -1,
      -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    -1,    -1,    54,    -1,    56,    57,    58,
      59,    -1,    -1,    62,    63,    64,    65,    66,    67,    -1,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    82,    -1,    84,    85,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,   104,   105,   106,    -1,   108,
      -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,   118,
     119,    -1,   121,    -1,    -1,   124,   125,   126,    -1,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,   150,    -1,    -1,   153,   154,    -1,   156,   157,   158,
      -1,   160,    -1,   162,    -1,    -1,   165,    -1,    -1,   168,
      -1,    -1,    -1,    -1,   173,   174,   175,    -1,    -1,    -1,
     179,   180,   181,   182,    -1,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,    -1,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,    -1,   222,   223,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   231,   232,   233,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,    -1,   245,    -1,    -1,   248,
      -1,   250,    -1,   252,   253,    -1,    -1,   256,   257,   258,
     259,    -1,    -1,   262,   263,   264,   265,   266,    -1,    -1,
      -1,   270,   271,    -1,   273,    -1,    -1,   276,   277,    -1,
     279,    -1,    -1,   282,   283,   284,   285,   286,   287,   288,
     289,   290,    -1,   292,   293,   294,   295,    -1,    -1,    -1,
     299,    -1,    -1,   302,   303,   304,    -1,    -1,    -1,    -1,
     309,    -1,   311,   312,   313,   314,    -1,    -1,   317,   318,
     319,   320,   321,    -1,    -1,   324,   325,    -1,   327,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,
     339,    -1,   341,    -1,   343,   344,   345,   346,    -1,    -1,
      -1,   350,    -1,    -1,    -1,    -1,    -1,   356,   357,    -1,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,    -1,    -1,    -1,    -1,   375,   376,   377,   378,
      -1,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,   390,   391,   392,   393,   394,   395,   396,   397,    -1,
      -1,    -1,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   413,    -1,    -1,   416,   417,   418,
     419,   420,   421,   422,   423,   424,   425,   426,   427,   428,
     429,   430,   431,   432,   433,    -1,   435,   436,   437,   438,
     439,    -1,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
      -1,   460,    -1,   462,   463,   464,   465,    -1,    -1,   468,
      -1,   470,    -1,   472,   473,   474,    -1,    -1,    -1,   478,
     479,    -1,    -1,    -1,    -1,   484,    -1,   486,    -1,    -1,
     489,   490,    -1,   492,   493,   494,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      28,    29,    30,    31,    -1,    33,    34,    35,    36,    -1,
      -1,    -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,
      48,    49,    -1,    51,    -1,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    76,    -1,
      78,    79,    80,    -1,    82,    -1,    84,    85,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,   110,    -1,    -1,   113,   114,    -1,    -1,    -1,
     118,    -1,    -1,   121,    -1,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,    -1,   133,   134,    -1,   136,    -1,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,   146,   147,
      -1,   149,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,   160,   161,   162,    -1,    -1,   165,    -1,    -1,
     168,    -1,    -1,   171,    -1,    -1,   174,   175,    -1,    -1,
      -1,   179,    -1,   181,    -1,    -1,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,   203,   204,   205,   206,   207,
     208,   209,    -1,   211,   212,   213,    -1,   215,    -1,   217,
      -1,    -1,   220,    -1,   222,   223,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   231,   232,   233,    -1,    -1,   236,   237,
     238,   239,   240,   241,   242,   243,    -1,    -1,    -1,    -1,
     248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,   257,
     258,   259,    -1,    -1,   262,   263,   264,   265,   266,   267,
      -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,   277,
      -1,    -1,    -1,    -1,   282,    -1,   284,   285,    -1,    -1,
      -1,    -1,   290,    -1,   292,   293,    -1,   295,    -1,    -1,
      -1,   299,    -1,    -1,   302,    -1,   304,    -1,    -1,    -1,
      -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,    -1,
     318,    -1,   320,   321,    -1,    -1,   324,   325,    -1,   327,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,
     338,   339,    -1,   341,    -1,   343,   344,   345,   346,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,
      -1,    -1,    -1,    -1,   382,   383,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,   397,
      -1,    -1,    -1,   401,   402,    -1,   404,    -1,    -1,    -1,
      -1,   409,    -1,    -1,    -1,   413,    -1,    -1,   416,   417,
     418,   419,    -1,    -1,    -1,   423,   424,   425,    -1,   427,
     428,    -1,   430,    -1,   432,    -1,    -1,    -1,    -1,    -1,
      -1,   439,    -1,   441,   442,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   454,     4,     5,     6,
       7,     8,   460,    -1,   462,    12,    -1,   465,    -1,    -1,
     468,    -1,   470,    -1,   472,   473,   474,    -1,    25,    -1,
      -1,    28,    29,    30,    31,    -1,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    -1,   493,    43,    -1,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    73,    -1,    -1,    76,
      -1,    78,    79,    80,    -1,    82,    -1,    84,    85,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,   121,    -1,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,    -1,   133,   134,    -1,   136,
      -1,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
     147,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,   174,   175,    -1,
      -1,    -1,   179,    -1,   181,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,   203,   204,   205,   206,
     207,   208,   209,    -1,   211,   212,   213,    -1,   215,    -1,
     217,    -1,    -1,   220,    -1,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   231,   232,   233,    -1,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,    -1,    -1,    -1,
      -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,
     257,   258,   259,    -1,    -1,   262,   263,   264,   265,   266,
      -1,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,
     277,    -1,    -1,    -1,    -1,   282,    -1,   284,   285,    -1,
      -1,    -1,    -1,   290,    -1,   292,   293,    -1,   295,    -1,
      -1,    -1,   299,    -1,    -1,   302,    -1,   304,    -1,    -1,
      -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,
      -1,   318,    -1,   320,   321,    -1,    -1,   324,   325,    -1,
     327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     337,   338,   339,    -1,   341,    -1,   343,   344,   345,   346,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,   382,   383,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,
     397,    -1,    -1,    -1,   401,   402,    -1,   404,    -1,    -1,
      -1,    -1,   409,    -1,    -1,    -1,   413,    -1,    -1,   416,
     417,   418,   419,    -1,    -1,    -1,   423,   424,   425,    -1,
     427,   428,    -1,   430,    -1,   432,    -1,    -1,    -1,    -1,
      -1,    -1,   439,    -1,   441,   442,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,    -1,    -1,
      -1,    -1,    -1,   460,    -1,   462,    -1,    -1,   465,    -1,
      -1,   468,    -1,   470,    -1,   472,   473,   474,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
     487,    -1,    -1,    -1,    -1,    -1,   493,    -1,    -1,    25,
      -1,    -1,    28,    29,    30,    31,    -1,    33,    34,    35,
      36,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    54,    -1,
      56,    -1,    -1,    59,    -1,    -1,    62,    63,    64,    65,
      66,    67,    -1,    -1,    70,    71,    72,    73,    -1,    -1,
      76,    -1,    78,    79,    80,    -1,    82,    -1,    84,    85,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,   105,
     106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,   121,    -1,    -1,   124,   125,
     126,    -1,   128,   129,   130,    -1,    -1,   133,   134,    -1,
     136,    -1,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,
     166,    -1,   168,    -1,    -1,    -1,    -1,    -1,   174,   175,
      -1,    -1,    -1,   179,    -1,   181,    -1,    -1,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,   203,   204,   205,
     206,   207,   208,   209,    -1,   211,   212,   213,    -1,   215,
      -1,   217,    -1,    -1,   220,    -1,   222,   223,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   231,   232,   233,    -1,    -1,
     236,   237,   238,   239,   240,   241,   242,   243,    -1,    -1,
      -1,    -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,
     256,   257,   258,   259,    -1,    -1,   262,   263,   264,   265,
     266,    -1,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,
     276,   277,    -1,    -1,    -1,    -1,   282,    -1,   284,   285,
      -1,    -1,    -1,    -1,   290,    -1,   292,   293,    -1,   295,
      -1,    -1,    -1,   299,    -1,    -1,   302,    -1,   304,    -1,
      -1,    -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,
      -1,    -1,   318,    -1,   320,   321,    -1,    -1,   324,   325,
      -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   338,   339,    -1,   341,    -1,   343,   344,   345,
     346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,
      -1,    -1,    -1,    -1,    -1,    -1,   382,   383,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,
      -1,   397,    -1,    -1,    -1,   401,   402,    -1,   404,    -1,
      -1,    -1,    -1,   409,    -1,    -1,    -1,   413,    -1,    -1,
     416,   417,   418,   419,    -1,    -1,    -1,   423,   424,   425,
      -1,   427,   428,    -1,   430,    -1,   432,    -1,    -1,    -1,
      -1,    -1,    -1,   439,    -1,   441,   442,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,     4,
       5,     6,     7,     8,   460,    -1,   462,    12,    -1,   465,
      -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,    -1,
      25,    -1,    -1,    28,    29,    30,    31,    -1,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    -1,   493,    43,    -1,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    70,    71,    72,    73,    -1,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,    84,
      85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    -1,
      95,    96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,
     105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,   121,    -1,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,    -1,   133,   134,
      -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,   147,    -1,   149,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,   160,    -1,   162,    -1,    -1,
     165,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,   174,
     175,    -1,    -1,    -1,   179,    -1,   181,    -1,    -1,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,   203,   204,
     205,   206,   207,   208,   209,    -1,   211,   212,   213,    -1,
     215,    -1,   217,    -1,    -1,   220,    -1,   222,   223,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,    -1,
      -1,    -1,    -1,   248,    -1,   250,    -1,   252,   253,    -1,
      -1,   256,   257,   258,   259,    -1,    -1,   262,   263,   264,
     265,   266,    -1,    -1,    -1,   270,   271,    -1,   273,    -1,
      -1,   276,   277,    -1,    -1,    -1,    -1,   282,    -1,   284,
     285,    -1,    -1,    -1,    -1,   290,    -1,   292,   293,    -1,
     295,    -1,    -1,    -1,   299,    -1,    -1,   302,    -1,   304,
      -1,    -1,    -1,    -1,   309,    -1,   311,   312,   313,   314,
      -1,    -1,    -1,   318,    -1,   320,   321,    -1,    -1,   324,
     325,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   337,   338,   339,    -1,   341,    -1,   343,   344,
     345,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     375,    -1,    -1,    -1,    -1,    -1,    -1,   382,   383,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,
      -1,    -1,   397,    -1,    -1,    -1,   401,   402,    -1,   404,
      -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,   413,    -1,
      -1,   416,   417,   418,   419,    -1,    -1,    -1,   423,   424,
     425,    -1,   427,   428,    -1,   430,    -1,   432,    -1,    -1,
      -1,    -1,    -1,    -1,   439,    -1,   441,   442,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,
      -1,    -1,    -1,    -1,    -1,   460,    -1,   462,    -1,    -1,
     465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,
       4,     5,     6,     7,     8,   480,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   493,    -1,
      -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,
      -1,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    63,
      64,    65,    66,    67,    -1,    -1,    70,    71,    72,    73,
      -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,
      84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    99,    -1,   101,   102,    -1,
      -1,   105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,    -1,
     124,   125,   126,    -1,   128,   129,   130,    -1,    -1,   133,
     134,    -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,   149,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,   160,    -1,   162,    -1,
      -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,
     174,   175,    -1,    -1,    -1,   179,    -1,   181,    -1,    -1,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,   203,
     204,   205,   206,   207,   208,   209,    -1,   211,   212,   213,
      -1,   215,    -1,   217,    -1,    -1,   220,    -1,   222,   223,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,
      -1,    -1,   236,   237,   238,   239,   240,   241,   242,   243,
      -1,    -1,    -1,    -1,   248,    -1,   250,    -1,   252,   253,
      -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,   263,
     264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,   273,
      -1,    -1,   276,   277,    -1,    -1,    -1,    -1,   282,    -1,
     284,   285,    -1,    -1,    -1,    -1,   290,    -1,   292,   293,
      -1,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,    -1,
     304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,   313,
     314,    -1,    -1,    -1,   318,    -1,   320,   321,    -1,    -1,
     324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,   343,
     344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,   382,   383,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     394,    -1,    -1,   397,    -1,    -1,    -1,   401,   402,    -1,
     404,    -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,   413,
      -1,    -1,   416,   417,   418,   419,    -1,    -1,    -1,   423,
     424,   425,    -1,   427,   428,    -1,   430,    -1,   432,    -1,
      -1,    -1,    -1,    -1,    -1,   439,    -1,   441,   442,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     454,    -1,    -1,    -1,    -1,    -1,   460,    -1,   462,    -1,
      -1,   465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,
     474,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,   486,    -1,    -1,    -1,    -1,    -1,    -1,   493,
      -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,
      33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,
      43,    -1,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,
      63,    64,    65,    66,    67,    -1,    -1,    70,    71,    72,
      73,    -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,
      -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,    -1,   105,   106,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,    -1,
     133,   134,    -1,   136,    -1,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,   149,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,   162,
      -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,   174,   175,    -1,    -1,    -1,   179,    -1,   181,    -1,
      -1,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
     203,   204,   205,   206,   207,   208,   209,    -1,   211,   212,
     213,    -1,   215,    -1,   217,    -1,    -1,   220,    -1,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,
     233,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,    -1,    -1,    -1,   248,    -1,   250,    -1,   252,
     253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,
     263,   264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,
     273,    -1,    -1,   276,   277,    -1,    -1,    -1,    -1,   282,
      -1,   284,   285,    -1,    -1,    -1,    -1,   290,    -1,   292,
     293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,
      -1,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,
     313,   314,    -1,    -1,    -1,   318,    -1,   320,   321,    -1,
      -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,
     343,   344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,   382,
     383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   394,    -1,    -1,   397,    -1,    -1,    -1,   401,   402,
      -1,   404,    -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,
     413,    -1,    -1,   416,   417,   418,   419,    -1,    -1,    -1,
     423,   424,   425,    -1,   427,   428,    -1,   430,    -1,   432,
      -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,   441,   442,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   454,    -1,    -1,    -1,    -1,    -1,   460,    -1,   462,
      -1,    -1,   465,    -1,    -1,   468,    -1,   470,    -1,   472,
     473,   474,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,   487,    -1,    -1,    -1,    -1,    -1,
     493,    -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,
      -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,
      -1,    43,    -1,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,   105,   106,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,
      -1,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
      -1,   133,   134,    -1,   136,    -1,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,   147,    -1,   149,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,
     162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,
      -1,    -1,   174,   175,    -1,    -1,    -1,   179,    -1,   181,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,   203,   204,   205,   206,   207,   208,   209,    -1,   211,
     212,   213,    -1,   215,    -1,   217,    -1,    -1,   220,    -1,
     222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,
     232,   233,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,    -1,    -1,    -1,    -1,   248,    -1,   250,    -1,
     252,   253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,
     262,   263,   264,   265,   266,    -1,    -1,    -1,   270,   271,
      -1,   273,    -1,    -1,   276,   277,    -1,    -1,    -1,    -1,
     282,    -1,   284,   285,    -1,    -1,    -1,    -1,   290,    -1,
     292,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,
     302,    -1,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,
     312,   313,   314,    -1,    -1,    -1,   318,    -1,   320,   321,
      -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,
      -1,   343,   344,   345,   346,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,
     382,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   394,    -1,    -1,   397,    -1,    -1,    -1,   401,
     402,    -1,   404,    -1,    -1,    -1,    -1,   409,    -1,    -1,
      -1,   413,    -1,    -1,   416,   417,   418,   419,    -1,    -1,
      -1,   423,   424,   425,    -1,   427,   428,    -1,   430,    -1,
     432,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,   441,
     442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   454,     4,     5,     6,     7,     8,   460,    -1,
     462,    12,    -1,   465,    -1,    -1,   468,    -1,   470,    -1,
     472,   473,   474,    -1,    25,    -1,    -1,    28,    29,    30,
      31,    -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,
      -1,   493,    43,    -1,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,
      -1,    62,    63,    64,    65,    66,    67,    -1,    -1,    70,
      71,    72,    73,    -1,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
     101,   102,    -1,    -1,   105,   106,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,    -1,   133,   134,    -1,   136,    -1,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,   160,
      -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,
      -1,    -1,    -1,   174,   175,    -1,    -1,    -1,   179,    -1,
     181,    -1,    -1,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,   203,   204,   205,   206,   207,   208,   209,    -1,
     211,   212,   213,    -1,   215,    -1,   217,    -1,    -1,   220,
      -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     231,   232,   233,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,    -1,    -1,    -1,    -1,   248,    -1,   250,
      -1,   252,   253,    -1,    -1,   256,   257,   258,   259,    -1,
      -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,   270,
     271,    -1,   273,    -1,    -1,   276,   277,    -1,    -1,    -1,
      -1,   282,    -1,   284,   285,    -1,    -1,    -1,    -1,   290,
      -1,   292,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,
      -1,   302,    -1,   304,    -1,    -1,    -1,    -1,   309,    -1,
     311,   312,   313,   314,    -1,    -1,    -1,   318,    -1,   320,
     321,    -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,
     341,    -1,   343,   344,   345,   346,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,
      -1,   382,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   394,    -1,    -1,   397,    -1,    -1,    -1,
     401,   402,    -1,   404,    -1,    -1,    -1,    -1,   409,    -1,
      -1,    -1,   413,    -1,    -1,   416,   417,   418,   419,    -1,
      -1,    -1,   423,   424,   425,    -1,   427,   428,    -1,   430,
      -1,   432,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,
     441,   442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   454,     4,     5,     6,     7,     8,   460,
      -1,   462,    12,    -1,   465,    -1,    -1,   468,    -1,   470,
      -1,   472,   473,   474,    -1,    25,    -1,    -1,    28,    29,
      30,    31,    -1,    33,    34,    35,    36,    -1,    -1,    -1,
      40,    -1,   493,    43,    -1,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    63,    64,    65,    66,    67,    -1,    -1,
      70,    71,    72,    73,    -1,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,   101,   102,    -1,    -1,   105,   106,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,   121,    -1,    -1,   124,   125,   126,    -1,   128,   129,
     130,    -1,    -1,   133,   134,    -1,   136,    -1,   138,   139,
     140,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
     160,    -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,
      -1,    -1,    -1,    -1,   174,   175,    -1,    -1,    -1,   179,
      -1,   181,    -1,    -1,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,   203,   204,   205,   206,   207,   208,   209,
      -1,   211,   212,   213,    -1,   215,    -1,   217,    -1,    -1,
     220,    -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   231,   232,   233,    -1,    -1,   236,   237,   238,   239,
     240,   241,   242,   243,    -1,    -1,    -1,    -1,   248,    -1,
     250,    -1,   252,   253,    -1,    -1,   256,   257,   258,   259,
      -1,    -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,
     270,   271,    -1,   273,    -1,    -1,   276,   277,    -1,    -1,
      -1,    -1,   282,    -1,   284,   285,    -1,    -1,    -1,    -1,
     290,    -1,   292,   293,    -1,   295,    -1,    -1,    -1,   299,
      -1,    -1,   302,    -1,   304,    -1,    -1,    -1,    -1,   309,
      -1,   311,   312,   313,   314,    -1,    -1,    -1,   318,    -1,
     320,   321,    -1,    -1,   324,   325,    -1,   327,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,
      -1,   341,    -1,   343,   344,   345,   346,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,    -1,    -1,
      -1,    -1,   382,   383,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   394,    -1,    -1,   397,    -1,    -1,
      -1,   401,   402,    -1,   404,    -1,    -1,    -1,    -1,   409,
      -1,    -1,    -1,   413,    -1,    -1,   416,   417,   418,   419,
      -1,    -1,    -1,   423,   424,   425,    -1,   427,   428,    -1,
     430,    -1,   432,    -1,    -1,    -1,    -1,    -1,    -1,   439,
      -1,   441,   442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   454,     4,     5,     6,     7,     8,
     460,    -1,   462,    12,    -1,   465,    -1,    -1,   468,    -1,
     470,    -1,   472,   473,   474,    -1,    25,    -1,    -1,    28,
      29,    30,    31,    -1,    33,    34,    35,    36,    -1,    -1,
      -1,    40,    -1,   493,    43,    -1,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    63,    64,    65,    66,    67,    -1,
      -1,    70,    71,    72,    73,    -1,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    -1,    84,    85,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,    -1,   105,   106,    -1,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,   121,    -1,    -1,   124,   125,   126,    -1,   128,
     129,   130,    -1,    -1,   133,   134,    -1,   136,    -1,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,   160,    -1,   162,    -1,    -1,   165,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,   174,   175,    -1,    -1,    -1,
     179,    -1,   181,    -1,    -1,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,   203,   204,   205,   206,   207,   208,
     209,    -1,   211,   212,   213,    -1,   215,    -1,   217,    -1,
      -1,   220,    -1,   222,   223,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   231,   232,   233,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,    -1,    -1,    -1,    -1,   248,
      -1,   250,    -1,   252,   253,    -1,    -1,   256,   257,   258,
     259,    -1,    -1,   262,   263,   264,   265,   266,    -1,    -1,
      -1,   270,   271,    -1,   273,    -1,    -1,   276,   277,    -1,
      -1,    -1,    -1,   282,    -1,   284,   285,    -1,    -1,    -1,
      -1,   290,    -1,   292,   293,    -1,   295,    -1,    -1,    -1,
     299,    -1,    -1,   302,    -1,   304,    -1,    -1,    -1,    -1,
     309,    -1,   311,   312,   313,   314,    -1,    -1,    -1,   318,
      -1,   320,   321,    -1,    -1,   324,   325,    -1,   327,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,
     339,    -1,   341,    -1,   343,   344,   345,   346,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,    -1,
      -1,    -1,    -1,   382,   383,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,   397,    -1,
      -1,    -1,   401,   402,    -1,   404,    -1,    -1,    -1,    -1,
     409,    -1,    -1,    -1,   413,    -1,    -1,   416,   417,   418,
     419,    -1,    -1,    -1,   423,   424,   425,    -1,   427,   428,
      -1,   430,    -1,   432,    -1,    -1,    -1,    -1,    -1,    -1,
     439,    -1,   441,   442,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   454,     4,     5,     6,     7,
       8,   460,    -1,   462,    12,    -1,   465,    -1,    -1,   468,
      -1,   470,    -1,   472,   473,   474,    -1,    25,    -1,    -1,
      28,    29,    30,    31,    -1,    33,    34,    35,    36,    -1,
      -1,    -1,    40,    -1,   493,    43,    -1,    45,    46,    47,
      48,    49,    -1,    51,    -1,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    76,    -1,
      78,    79,    80,    -1,    82,    -1,    84,    85,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,   121,    -1,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,    -1,   133,   134,    -1,   136,    -1,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,   147,
      -1,   149,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,    -1,
     168,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,    -1,
      -1,   179,    -1,   181,    -1,    -1,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,   203,   204,   205,   206,   207,
     208,   209,    -1,   211,   212,   213,    -1,   215,    -1,   217,
      -1,    -1,   220,    -1,   222,   223,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   231,   232,   233,    -1,    -1,   236,   237,
     238,   239,   240,   241,   242,   243,    -1,    -1,    -1,    -1,
     248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,   257,
     258,   259,    -1,    -1,   262,   263,   264,   265,   266,    -1,
      -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,   277,
      -1,   279,    -1,    -1,   282,    -1,   284,   285,    -1,    -1,
      -1,    -1,   290,    -1,   292,   293,    -1,   295,    -1,    -1,
      -1,   299,    -1,    -1,   302,    -1,   304,    -1,    -1,    -1,
      -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,    -1,
     318,    -1,   320,   321,    -1,    -1,   324,   325,    -1,   327,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,
     338,   339,    -1,   341,    -1,   343,   344,   345,   346,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,
      -1,    -1,    -1,    -1,   382,   383,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,   397,
      -1,    -1,    -1,   401,   402,    -1,   404,    -1,    -1,    -1,
      -1,   409,    -1,    -1,    -1,   413,    -1,    -1,   416,   417,
     418,   419,    -1,    -1,    -1,   423,   424,   425,    -1,   427,
     428,    -1,   430,    -1,   432,    -1,    -1,    -1,    -1,    -1,
      -1,   439,    -1,   441,   442,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   454,     4,     5,     6,
       7,     8,   460,    -1,   462,    12,    -1,   465,    -1,    -1,
     468,    -1,   470,    -1,   472,   473,   474,    -1,    25,    -1,
      -1,    28,    29,    30,    31,    -1,    33,    34,    35,    36,
      -1,    -1,    -1,    40,   492,    -1,    43,    -1,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    70,    71,    72,    73,    -1,    -1,    76,
      -1,    78,    79,    80,    -1,    82,    -1,    84,    85,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,   121,    -1,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,    -1,   133,   134,    -1,   136,
      -1,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
     147,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
      -1,    -1,   179,    -1,   181,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,   203,   204,   205,   206,
     207,   208,   209,    -1,   211,   212,   213,    -1,   215,    -1,
     217,    -1,    -1,   220,    -1,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   231,   232,   233,    -1,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,    -1,    -1,    -1,
      -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,
     257,   258,   259,    -1,    -1,   262,   263,   264,   265,   266,
      -1,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,
     277,    -1,   279,    -1,    -1,   282,    -1,   284,   285,    -1,
      -1,    -1,    -1,   290,    -1,   292,   293,    -1,   295,    -1,
      -1,    -1,   299,    -1,    -1,   302,    -1,   304,    -1,    -1,
      -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,
      -1,   318,    -1,   320,   321,    -1,    -1,   324,   325,    -1,
     327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     337,   338,   339,    -1,   341,    -1,   343,   344,   345,   346,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,   382,   383,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,
     397,    -1,    -1,    -1,   401,   402,    -1,   404,    -1,    -1,
      -1,    -1,   409,    -1,    -1,    -1,   413,    -1,    -1,   416,
     417,   418,   419,    -1,    -1,    -1,   423,   424,   425,    -1,
     427,   428,    -1,   430,    -1,   432,    -1,    -1,    87,    -1,
      -1,    -1,   439,    -1,   441,   442,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,    -1,    -1,
      -1,    -1,    -1,   460,    -1,   462,    -1,    -1,   465,    -1,
      -1,   468,    -1,   470,    -1,   472,   473,   474,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   492,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,   226,   227,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,   247,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    87,
      -1,   310,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   216,    -1,   167,    -1,    -1,    -1,   358,
      -1,    87,    -1,   226,   227,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,   167,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,   226,   227,    -1,   216,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,   177,    -1,    -1,
      -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,   216,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,   310,   226,   227,
      -1,   177,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,   310,    -1,   247,   487,   488,
     216,    -1,    -1,    -1,    -1,   358,    -1,    -1,   310,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,   487,   488,    -1,    -1,    -1,    -1,
      -1,    -1,   358,    -1,    87,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,   487,   488,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,   488,    -1,    87,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
     488,    -1,    -1,    87,   167,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   177,    -1,    -1,   487,   488,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,   167,    -1,
      87,   487,   488,   216,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   167,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,
     167,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   226,   227,    -1,    -1,    -1,   310,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,   226,
     227,   310,    -1,   177,    -1,   358,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,   167,    -1,
      -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,   177,   358,
      -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,   358,    -1,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,   247,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,   358,    -1,    -1,   487,   488,   310,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,   487,   488,
      -1,   310,    87,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,   487,   488,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
     487,   488,   167,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,   226,   227,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,   216,    -1,    -1,   488,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   247,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    87,    -1,   487,   488,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   310,    -1,    87,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   177,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,   216,   487,   488,   167,
      -1,    87,    -1,    -1,    -1,    -1,   226,   227,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,   310,
     167,    -1,   487,   488,    -1,    -1,    -1,    -1,   216,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,   167,    -1,    -1,    -1,    -1,    -1,   358,    -1,   216,
     310,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
     226,   227,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   487,   488,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,   488,    -1,
      -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
     488,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    87,    -1,    -1,
      -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,   247,   488,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,   358,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    87,    -1,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,   216,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    87,   358,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,    -1,   488,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   310,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   358,    -1,   216,    -1,   488,   247,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    87,   358,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
     358,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,    -1,   488,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,   216,    -1,   488,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   167,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,   177,    -1,    -1,
     488,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   177,    -1,    -1,    -1,   488,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    -1,   247,    -1,   310,
      -1,    -1,    -1,   226,   227,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,   216,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   358,    -1,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,    -1,    87,    -1,    -1,    -1,   310,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   358,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,   358,   476,   477,   478,   479,   480,
     481,   482,   483,   177,    -1,    -1,    -1,   488,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,    -1,   488,    -1,
      -1,   216,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,    -1,   488,    -1,    -1,    -1,    -1,
      -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
     177,    -1,    -1,    -1,   488,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   310,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,    -1,   226,   227,    -1,    -1,
     247,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
     310,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,    -1,   488,    87,    -1,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,    -1,   488,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,   247,
      -1,   488,    -1,    -1,    -1,   167,   476,   477,   478,   479,
     480,   481,   482,   483,   216,   177,    -1,    -1,   488,    -1,
      -1,    87,    -1,    -1,   226,   227,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   247,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    87,   310,    -1,
     358,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
     216,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   177,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,   216,    -1,    -1,    -1,
     488,    -1,    -1,    -1,   310,   167,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,   167,    -1,    -1,   488,    -1,    -1,    -1,
      -1,    87,   358,   177,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   247,   488,    -1,    -1,    -1,
     310,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
     216,    -1,   488,    -1,    -1,    -1,   310,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,    -1,   488,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,   476,   477,   478,   479,   480,   481,
     482,   483,   358,   177,    -1,    -1,   488,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,   216,    -1,   488,    -1,    -1,    -1,    -1,    87,
     167,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   247,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,   167,
     247,    -1,   488,    -1,    -1,    -1,   310,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   216,    -1,
      -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   177,    -1,    -1,    -1,    -1,   247,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
     310,    -1,   247,    -1,   488,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
      -1,   488,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,   358,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,    -1,
     488,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,   247,    -1,   488,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   476,   477,   478,   479,   480,   481,   482,   483,   310,
     226,   227,    -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    87,    -1,    -1,   358,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,   216,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    87,
      -1,    -1,   358,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,   216,    -1,   310,    -1,   488,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    87,   167,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,   216,    -1,
      -1,    -1,   488,    -1,    -1,    -1,    -1,   310,   226,   227,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
     216,    -1,   310,   488,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,   310,   488,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   358,    -1,    87,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,    -1,
     488,   476,   477,   478,   479,   480,   481,   482,   483,    87,
     167,    -1,    -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   144,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   167,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,   167,
     247,    -1,   488,   216,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,   167,   226,   227,    -1,    -1,    -1,    -1,    87,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   247,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,   216,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   247,    -1,   226,   227,    -1,   310,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,   358,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,   358,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   389,    -1,    -1,    -1,    -1,   358,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
      -1,   488,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,   487,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
     216,    -1,   487,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,    -1,    -1,    -1,    -1,
      -1,   247,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,    87,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    87,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    87,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    87,    -1,    -1,   216,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,   226,   227,
     247,   487,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,   216,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,   216,   310,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,   358,   247,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   247,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   247,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,    87,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,   358,    -1,    -1,
     487,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,   358,    -1,    -1,   487,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,   487,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,   487,   216,    87,   476,   477,   478,   479,   480,
     481,   482,   483,   226,   227,    -1,   487,    -1,    87,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,   247,   487,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   310,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,   167,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,   226,   227,   358,    -1,   216,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   310,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,   310,   167,    -1,   487,   358,    -1,   216,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,   226,   227,   358,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
      -1,   216,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   247,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,   310,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,   487,    -1,   167,   476,   477,   478,
     479,   480,   481,   482,   483,   310,   177,    -1,   487,   358,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    87,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,   487,    -1,
      -1,    -1,   247,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,   247,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   310,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,   487,   310,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   358,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,   487,   247,    -1,    -1,    -1,   358,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   310,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,   216,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,   226,
     227,    -1,   487,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,    -1,   226,   227,    -1,    -1,
     247,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   487,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,   358,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,   487,    -1,    -1,   247,    -1,   358,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,   358,    -1,    87,
     487,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    87,    -1,    -1,   487,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,   167,    -1,   216,    -1,    -1,    -1,
      -1,    87,    -1,    -1,   177,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,   216,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,   226,   227,    -1,   487,   247,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,   216,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,   310,   358,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,   358,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,    -1,    -1,
      -1,    87,   358,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,   487,    87,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,   487,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
     216,   487,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
     226,   227,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   358,    87,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,   167,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,   177,    -1,   487,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,   216,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,   226,   227,
      -1,   487,    -1,    87,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,   216,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,   167,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
     216,    -1,   310,   167,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,   177,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
     310,   247,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   247,    -1,    -1,    -1,
     358,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,    -1,    -1,    -1,   310,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,   358,    -1,    -1,   487,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    87,   358,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    87,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,   487,    -1,   167,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,   177,    -1,    -1,   487,    -1,    87,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    87,    -1,    -1,   487,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      87,    -1,   216,   487,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,   247,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,   216,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,   226,   227,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,   310,   216,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,   216,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,   247,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,   247,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     247,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   310,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,   310,    -1,    -1,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,   358,   247,   487,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,   310,    -1,    -1,   487,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,   487,    -1,
      87,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,   487,   358,    -1,    87,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
     487,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      87,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    87,    -1,   216,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,   226,
     227,    -1,   487,   247,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,   310,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,   358,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
     247,   358,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   358,   247,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,   358,    -1,   487,    -1,    87,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,   358,    -1,
     487,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    87,    -1,   216,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,   226,   227,    -1,
     487,   247,    -1,    -1,   216,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   226,   227,    -1,   487,   247,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,   358,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,   247,   358,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   310,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,   216,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,   226,   227,
      -1,   487,    -1,    87,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,   358,    -1,   487,   247,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    87,    -1,   487,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,   310,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,   167,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,   226,   227,    -1,    -1,   247,
     358,    -1,   216,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,   226,   227,    -1,   487,   247,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   216,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   310,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,   310,   358,    -1,   487,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,   358,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    87,    -1,   216,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,   226,   227,    87,   487,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   487,   247,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    87,    -1,   487,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,   487,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   167,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,   226,   227,   167,    -1,   216,   358,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,   247,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   247,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,   247,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   247,    -1,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   310,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   310,   358,    -1,   487,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,   358,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   358,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   358,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
     167,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   226,   227,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,   247,   487,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    87,   487,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
     247,   487,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,   310,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,   167,    -1,   310,   358,    -1,   216,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,   226,   227,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,   216,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
     216,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    87,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
     310,    -1,    -1,   487,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,   167,   358,    -1,
     487,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,   358,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    87,    -1,   487,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,   216,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
     310,   487,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   358,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,   358,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,
      -1,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    63,
      64,    65,    66,    67,    -1,    -1,    70,    71,    72,    73,
      -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,
      84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    99,    -1,   101,   102,    -1,
      -1,   105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,    -1,
     124,   125,   126,    -1,   128,   129,   130,    -1,    -1,   133,
     134,    -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,   149,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,   160,    -1,   162,    -1,
      -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,
     174,   175,    -1,    -1,    -1,   179,    -1,   181,    -1,    -1,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,   203,
     204,   205,   206,   207,   208,   209,    -1,   211,   212,   213,
      -1,   215,    -1,   217,    -1,    -1,   220,    -1,   222,   223,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,
      -1,    -1,   236,   237,   238,   239,   240,   241,   242,   243,
      -1,    -1,    -1,    -1,   248,    -1,   250,    -1,   252,   253,
      -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,   263,
     264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,   273,
      -1,    -1,   276,   277,    -1,    -1,    -1,    -1,   282,    -1,
     284,   285,    -1,    -1,    -1,    -1,   290,    -1,   292,   293,
      -1,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,    -1,
     304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,   313,
     314,    -1,    -1,    -1,   318,    -1,   320,   321,    -1,    -1,
     324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,   343,
     344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,   382,   383,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     394,    -1,    -1,   397,    -1,    -1,    -1,   401,   402,    -1,
     404,    -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,   413,
      -1,    -1,   416,   417,   418,   419,    -1,    -1,    -1,   423,
     424,   425,    -1,   427,   428,    -1,   430,    -1,   432,    -1,
      -1,    -1,    -1,    -1,    -1,   439,    -1,   441,   442,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     454,    -1,    -1,    -1,    -1,    -1,   460,    -1,   462,    -1,
      -1,   465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,
     474,     4,     5,     6,     7,     8,   480,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,
      33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,
      43,    -1,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,
      63,    64,    65,    66,    67,    -1,    -1,    70,    71,    72,
      73,    -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,
      -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,    -1,   105,   106,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,    -1,
     133,   134,    -1,   136,    -1,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,   149,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,   162,
      -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,   174,   175,    -1,    -1,    -1,   179,    -1,   181,    -1,
      -1,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
     203,   204,   205,   206,   207,   208,   209,    -1,   211,   212,
     213,    -1,   215,    -1,   217,    -1,    -1,   220,    -1,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,
     233,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,    -1,    -1,    -1,   248,    -1,   250,    -1,   252,
     253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,
     263,   264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,
     273,    -1,    -1,   276,   277,    -1,    -1,    -1,    -1,   282,
      -1,   284,   285,    -1,    -1,    -1,    -1,   290,    -1,   292,
     293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,
      -1,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,
     313,   314,    -1,    -1,    -1,   318,    -1,   320,   321,    -1,
      -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,
     343,   344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,   382,
     383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   394,    -1,    -1,   397,    -1,    -1,    -1,   401,   402,
      -1,   404,    -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,
     413,    -1,    -1,   416,   417,   418,   419,    -1,    -1,    -1,
     423,   424,   425,    -1,   427,   428,    -1,   430,    -1,   432,
      -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,   441,   442,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   454,    -1,    -1,    -1,    -1,    -1,   460,    -1,   462,
      -1,    -1,   465,    -1,    -1,   468,    -1,   470,    -1,   472,
     473,   474,     4,     5,     6,     7,     8,   480,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,
      -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,
      -1,    43,    -1,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,   105,   106,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,
      -1,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
      -1,   133,   134,    -1,   136,    -1,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,   147,    -1,   149,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,
     162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,
      -1,    -1,   174,   175,    -1,    -1,    -1,   179,    -1,   181,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,   203,   204,   205,   206,   207,   208,   209,    -1,   211,
     212,   213,    -1,   215,    -1,   217,    -1,    -1,   220,    -1,
     222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,
     232,   233,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,    -1,    -1,    -1,    -1,   248,    -1,   250,    -1,
     252,   253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,
     262,   263,   264,   265,   266,    -1,    -1,    -1,   270,   271,
      -1,   273,    -1,    -1,   276,   277,    -1,    -1,    -1,    -1,
     282,    -1,   284,   285,    -1,    -1,    -1,    -1,   290,    -1,
     292,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,
     302,    -1,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,
     312,   313,   314,    -1,    -1,    -1,   318,    -1,   320,   321,
      -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,
      -1,   343,   344,   345,   346,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,
     382,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   394,    -1,    -1,   397,    -1,    -1,    -1,   401,
     402,    -1,   404,    -1,    -1,    -1,    -1,   409,    -1,    -1,
      -1,   413,    -1,    -1,   416,   417,   418,   419,    -1,    -1,
      -1,   423,   424,   425,    -1,   427,   428,    -1,   430,    -1,
     432,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,   441,
     442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   454,    -1,    -1,    -1,    -1,    -1,   460,    -1,
     462,    -1,    -1,   465,    -1,    -1,   468,    -1,   470,    -1,
     472,   473,   474,     4,     5,     6,     7,     8,   480,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    29,    30,
      31,    -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,
      -1,    -1,    43,    -1,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,
      -1,    62,    63,    64,    65,    66,    67,    -1,    -1,    70,
      71,    72,    73,    -1,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
     101,   102,    -1,    -1,   105,   106,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,    -1,   133,   134,    -1,   136,    -1,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,   160,
      -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,
      -1,    -1,    -1,   174,   175,    -1,    -1,    -1,   179,    -1,
     181,    -1,    -1,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,   203,   204,   205,   206,   207,   208,   209,    -1,
     211,   212,   213,    -1,   215,    -1,   217,    -1,    -1,   220,
      -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     231,   232,   233,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,    -1,    -1,    -1,    -1,   248,    -1,   250,
      -1,   252,   253,    -1,    -1,   256,   257,   258,   259,    -1,
      -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,   270,
     271,    -1,   273,    -1,    -1,   276,   277,    -1,    -1,    -1,
      -1,   282,    -1,   284,   285,    -1,    -1,    -1,    -1,   290,
      -1,   292,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,
      -1,   302,    -1,   304,    -1,    -1,    -1,    -1,   309,    -1,
     311,   312,   313,   314,    -1,    -1,    -1,   318,    -1,   320,
     321,    -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,
     341,    -1,   343,   344,   345,   346,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   356,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,
      -1,   382,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   394,    -1,    -1,   397,    -1,    -1,    -1,
     401,   402,    -1,   404,    -1,    -1,    -1,    -1,   409,    -1,
      -1,    -1,   413,    -1,    -1,   416,   417,   418,   419,    -1,
      -1,    -1,   423,   424,   425,    -1,   427,   428,    -1,   430,
      -1,   432,    -1,    -1,    87,    -1,    -1,    -1,   439,    -1,
     441,   442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   454,    -1,    -1,    -1,    -1,    -1,   460,
      -1,   462,    -1,    -1,   465,    -1,    -1,   468,    -1,   470,
      -1,   472,   473,   474,    -1,    -1,    -1,    -1,    -1,   480,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,   226,   227,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   247,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,   310,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,
     177,    -1,    -1,    -1,    -1,   358,    -1,   226,   227,    87,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,   372,
     373,   374,   375,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,   144,   167,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,   177,    -1,   398,   399,   400,   401,   226,
     227,    -1,    -1,    -1,    87,   167,    -1,    -1,    -1,    -1,
     413,   414,   415,   416,    -1,   177,   419,    -1,    -1,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,    -1,    -1,    -1,   439,   440,    -1,   167,
      -1,   310,   226,   227,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,   461,   462,
      -1,    -1,    -1,   247,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,   310,   167,   247,    -1,    -1,   216,   358,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,
      -1,   358,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,   306,    -1,
      -1,    -1,   310,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,
     358,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,   358,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,   216,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,   226,   227,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   466,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,   177,    -1,    87,    -1,    -1,   310,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
     226,   227,    -1,    -1,   358,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    87,    -1,   177,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   216,    87,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   466,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,   476,   477,   478,   479,   480,   481,   482,   483,
     310,   177,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   167,    -1,    -1,    -1,    -1,    -1,    -1,
     216,    -1,    -1,   177,    -1,    -1,   310,    -1,   358,    -1,
     226,   227,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   358,    -1,    -1,   167,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      87,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,   310,   247,   167,    -1,
      -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
     177,    -1,    -1,    -1,   358,    -1,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    25,    -1,    -1,
      28,    29,    30,    31,    -1,    33,    34,    35,    36,    -1,
      -1,    -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,
      48,    49,    -1,    51,    -1,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    76,    -1,
      78,    79,    80,    -1,    82,    -1,    84,    85,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
     118,   119,    -1,   121,    -1,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,    -1,   133,   134,    -1,   136,    -1,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,   147,
      -1,   149,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,    -1,
     168,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,    -1,
      -1,   179,    -1,   181,    -1,    -1,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,   203,   204,   205,   206,   207,
     208,   209,    -1,   211,   212,   213,    -1,   215,    -1,   217,
      -1,    -1,   220,    -1,   222,   223,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   231,   232,   233,    -1,    -1,   236,   237,
     238,   239,   240,   241,   242,   243,    -1,    -1,    -1,    -1,
     248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,   257,
     258,   259,    -1,    -1,   262,   263,   264,   265,   266,    -1,
      -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,   277,
      -1,   279,    -1,    -1,   282,    -1,   284,   285,    -1,    -1,
      -1,    -1,   290,    -1,   292,   293,    -1,   295,    -1,    -1,
      -1,   299,    -1,    -1,   302,    -1,   304,    -1,    -1,    -1,
      -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,    -1,
     318,    -1,   320,   321,    -1,    -1,   324,   325,    -1,   327,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,
     338,   339,    -1,   341,    -1,   343,   344,   345,   346,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,
      -1,    -1,    -1,    -1,   382,   383,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,   397,
      -1,    -1,    -1,   401,   402,    -1,   404,    -1,    -1,    -1,
      -1,   409,    -1,    -1,    -1,   413,    -1,    -1,   416,   417,
     418,   419,    -1,    -1,    -1,   423,   424,   425,    -1,   427,
     428,    -1,   430,    -1,   432,    -1,    -1,    -1,    -1,    -1,
      -1,   439,    -1,   441,   442,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   454,     4,     5,     6,
       7,     8,   460,    -1,   462,    12,    -1,   465,    -1,    -1,
     468,    -1,   470,    -1,   472,   473,   474,    -1,    25,    -1,
      -1,    28,    29,    30,    31,    -1,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    -1,    -1,    43,    -1,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    70,    71,    72,    73,    -1,    -1,    76,
      -1,    78,    79,    80,    -1,    82,    -1,    84,    85,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,   118,   119,    -1,   121,    -1,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,    -1,   133,   134,    -1,   136,
      -1,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
     147,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
      -1,    -1,   179,    -1,   181,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,   203,   204,   205,   206,
     207,   208,   209,    -1,   211,   212,   213,    -1,   215,    -1,
     217,    -1,    -1,   220,    -1,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   231,   232,   233,    -1,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,    -1,    -1,    -1,
      -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,
     257,   258,   259,    -1,    -1,   262,   263,   264,   265,   266,
      -1,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,
     277,    -1,   279,    -1,    -1,   282,    -1,   284,   285,    -1,
      -1,    -1,    -1,   290,    -1,   292,   293,    -1,   295,    -1,
      -1,    -1,   299,    -1,    -1,   302,    -1,   304,    -1,    -1,
      -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,
      -1,   318,    -1,   320,   321,    -1,    -1,   324,   325,    -1,
     327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     337,   338,   339,    -1,   341,    -1,   343,   344,   345,   346,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,   382,   383,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,
     397,    -1,    -1,    -1,   401,   402,    -1,   404,    -1,    -1,
      -1,    -1,   409,    -1,    -1,    -1,   413,    -1,    -1,   416,
     417,   418,   419,    -1,    -1,    -1,   423,   424,   425,    -1,
     427,   428,    -1,   430,    -1,   432,    -1,    -1,    -1,    -1,
      -1,    -1,   439,    -1,   441,   442,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,     4,     5,
       6,     7,     8,   460,    -1,   462,    12,    -1,   465,    -1,
      -1,   468,    -1,   470,    -1,   472,   473,   474,    -1,    25,
      -1,    -1,    28,    29,    30,    31,    -1,    33,    34,    35,
      36,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    54,    -1,
      56,    -1,    -1,    59,    -1,    -1,    62,    63,    64,    65,
      66,    67,    -1,    -1,    70,    71,    72,    73,    -1,    -1,
      76,    -1,    78,    79,    80,    -1,    82,    -1,    84,    85,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,   105,
     106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,   118,    -1,    -1,   121,    -1,    -1,   124,   125,
     126,    -1,   128,   129,   130,    -1,    -1,   133,   134,    -1,
     136,    -1,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,
      -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,   174,   175,
     176,    -1,    -1,   179,    -1,   181,    -1,    -1,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,   203,   204,   205,
     206,   207,   208,   209,    -1,   211,   212,   213,    -1,   215,
      -1,   217,    -1,    -1,   220,    -1,   222,   223,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   231,   232,   233,    -1,    -1,
     236,   237,   238,   239,   240,   241,   242,   243,    -1,    -1,
      -1,    -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,
     256,   257,   258,   259,    -1,    -1,   262,   263,   264,   265,
     266,    -1,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,
     276,   277,    -1,   279,    -1,    -1,   282,    -1,   284,   285,
      -1,    -1,    -1,    -1,   290,    -1,   292,   293,    -1,   295,
      -1,    -1,    -1,   299,    -1,    -1,   302,    -1,   304,    -1,
      -1,    -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,
      -1,    -1,   318,    -1,   320,   321,    -1,    -1,   324,   325,
      -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   338,   339,    -1,   341,    -1,   343,   344,   345,
     346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,
      -1,    -1,    -1,    -1,    -1,    -1,   382,   383,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,
      -1,   397,    -1,    -1,    -1,   401,   402,    -1,   404,    -1,
      -1,    -1,    -1,   409,    -1,    -1,    -1,   413,    -1,    -1,
     416,   417,   418,   419,    -1,    -1,    -1,   423,   424,   425,
      -1,   427,   428,    -1,   430,    -1,   432,    -1,    -1,    -1,
      -1,    -1,    -1,   439,    -1,   441,   442,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,     4,
       5,     6,     7,     8,   460,    -1,   462,    12,    -1,   465,
      -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,    -1,
      25,    -1,    -1,    28,    29,    30,    31,    -1,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    70,    71,    72,    73,    -1,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,    84,
      85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,
     105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,   121,    -1,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,    -1,   133,   134,
      -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,   147,    -1,   149,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,   160,    -1,   162,    -1,    -1,
     165,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,    -1,    -1,   179,    -1,   181,    -1,    -1,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,   203,   204,
     205,   206,   207,   208,   209,    -1,   211,   212,   213,    -1,
     215,    -1,   217,    -1,    -1,   220,    -1,   222,   223,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,    -1,
      -1,    -1,    -1,   248,    -1,   250,    -1,   252,   253,    -1,
      -1,   256,   257,   258,   259,    -1,    -1,   262,   263,   264,
     265,   266,    -1,    -1,    -1,   270,   271,    -1,   273,    -1,
      -1,   276,   277,    -1,   279,    -1,    -1,   282,    -1,   284,
     285,    -1,    -1,    -1,    -1,   290,    -1,   292,   293,    -1,
     295,    -1,    -1,    -1,   299,    -1,    -1,   302,    -1,   304,
      -1,    -1,    -1,    -1,   309,    -1,   311,   312,   313,   314,
      -1,    -1,    -1,   318,    -1,   320,   321,    -1,    -1,   324,
     325,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   337,   338,   339,    -1,   341,    -1,   343,   344,
     345,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     375,    -1,    -1,    -1,    -1,    -1,    -1,   382,   383,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,
      -1,    -1,   397,    -1,    -1,    -1,   401,   402,    -1,   404,
      -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,   413,    -1,
      -1,   416,   417,   418,   419,    -1,    -1,    -1,   423,   424,
     425,    -1,   427,   428,    -1,   430,    -1,   432,    -1,    -1,
      -1,    -1,    -1,    -1,   439,    -1,   441,   442,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,
       4,     5,     6,     7,     8,   460,    -1,   462,    12,    -1,
     465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,
      -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,
      -1,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    63,
      64,    65,    66,    67,    -1,    -1,    70,    71,    72,    73,
      -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,
      84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    99,    -1,   101,   102,    -1,
      -1,   105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,   118,   119,    -1,   121,    -1,    -1,
     124,   125,   126,    -1,   128,   129,   130,    -1,    -1,   133,
     134,    -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,   149,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,   160,    -1,   162,    -1,
      -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,    -1,    -1,   179,    -1,   181,    -1,    -1,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,   203,
     204,   205,   206,   207,   208,   209,    -1,   211,   212,   213,
      -1,   215,    -1,   217,    -1,    -1,   220,    -1,   222,   223,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,
      -1,    -1,   236,   237,   238,   239,   240,   241,   242,   243,
      -1,    -1,    -1,    -1,   248,    -1,   250,    -1,   252,   253,
      -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,   263,
     264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,   273,
      -1,    -1,   276,   277,    -1,   279,    -1,    -1,   282,    -1,
     284,   285,    -1,    -1,    -1,    -1,   290,    -1,   292,   293,
      -1,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,    -1,
     304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,   313,
     314,    -1,    -1,    -1,   318,    -1,   320,   321,    -1,    -1,
     324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,   343,
     344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,   382,   383,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     394,    -1,    -1,   397,    -1,    -1,    -1,   401,   402,    -1,
     404,    -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,   413,
      -1,    -1,   416,   417,   418,   419,    -1,    -1,    -1,   423,
     424,   425,    -1,   427,   428,    -1,   430,    -1,   432,    -1,
      -1,    -1,    -1,    -1,    -1,   439,    -1,   441,   442,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     454,     4,     5,     6,     7,     8,   460,    -1,   462,    12,
      -1,   465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,
     474,    -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,
      33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,
      43,    -1,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,
      63,    64,    65,    66,    67,    -1,    -1,    70,    71,    72,
      73,    -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,
      -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,    -1,   105,   106,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,    -1,
     133,   134,    -1,   136,    -1,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,   149,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,   162,
      -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,    -1,    -1,   179,    -1,   181,    -1,
      -1,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
     203,   204,   205,   206,   207,   208,   209,    -1,   211,   212,
     213,    -1,   215,    -1,   217,    -1,    -1,   220,    -1,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,
     233,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,    -1,    -1,    -1,   248,    -1,   250,    -1,   252,
     253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,
     263,   264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,
     273,    -1,    -1,   276,   277,    -1,   279,    -1,    -1,   282,
      -1,   284,   285,    -1,    -1,    -1,    -1,   290,    -1,   292,
     293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,
      -1,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,
     313,   314,    -1,    -1,    -1,   318,    -1,   320,   321,    -1,
      -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,
     343,   344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,   382,
     383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   394,    -1,    -1,   397,    -1,    -1,    -1,   401,   402,
      -1,   404,    -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,
     413,    -1,    -1,   416,   417,   418,   419,    -1,    -1,    -1,
     423,   424,   425,    -1,   427,   428,    -1,   430,    -1,   432,
      -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,   441,   442,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   454,     4,     5,     6,     7,     8,   460,    -1,   462,
      12,    -1,   465,    -1,    -1,   468,    -1,   470,    -1,   472,
     473,   474,    -1,    25,    -1,    -1,    28,    29,    30,    31,
      -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,
      -1,    43,    -1,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    -1,    70,    71,
      72,    73,    -1,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,   105,   106,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,
      -1,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
      -1,   133,   134,    -1,   136,    -1,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,   147,    -1,   149,    -1,    -1,
      -1,   153,   154,    -1,   156,   157,   158,    -1,   160,    -1,
     162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,
      -1,    -1,   174,   175,    -1,    -1,    -1,   179,    -1,   181,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,   203,   204,   205,   206,   207,   208,   209,    -1,   211,
     212,   213,    -1,   215,    -1,   217,    -1,    -1,   220,    -1,
     222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,
     232,   233,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,    -1,    -1,    -1,    -1,   248,    -1,   250,    -1,
     252,   253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,
     262,   263,   264,   265,   266,    -1,    -1,    -1,   270,   271,
      -1,   273,    -1,    -1,   276,   277,    -1,   279,    -1,    -1,
     282,    -1,   284,   285,    -1,    -1,    -1,    -1,   290,    -1,
     292,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,
     302,    -1,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,
     312,   313,   314,    -1,    -1,    -1,   318,    -1,   320,   321,
      -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,   341,
      -1,   343,   344,   345,   346,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,
     382,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   394,    -1,    -1,   397,    -1,    -1,    -1,   401,
     402,    -1,   404,    -1,    -1,    -1,    -1,   409,    -1,    -1,
      -1,   413,    -1,    -1,   416,   417,   418,   419,    -1,    -1,
      -1,   423,   424,   425,    -1,   427,   428,    -1,   430,    -1,
     432,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,   441,
     442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   454,     4,     5,     6,     7,     8,   460,    -1,
     462,    12,    -1,   465,    -1,    -1,   468,    -1,   470,    -1,
     472,   473,   474,    -1,    25,    -1,    -1,    28,    29,    30,
      31,    -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,
      -1,    -1,    43,    -1,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,
      -1,    62,    63,    64,    65,    66,    67,    -1,    -1,    70,
      71,    72,    73,    -1,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    -1,    84,    85,    -1,    -1,    88,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
     101,   102,    -1,    -1,   105,   106,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,    -1,   133,   134,    -1,   136,    -1,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,   160,
      -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,
      -1,    -1,    -1,   174,   175,    -1,    -1,    -1,   179,    -1,
     181,    -1,    -1,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,   203,   204,   205,   206,   207,   208,   209,    -1,
     211,   212,   213,    -1,   215,    -1,   217,    -1,    -1,   220,
      -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     231,   232,   233,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,    -1,    -1,    -1,    -1,   248,    -1,   250,
      -1,   252,   253,    -1,    -1,   256,   257,   258,   259,    -1,
      -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,   270,
     271,    -1,   273,    -1,    -1,   276,   277,    -1,   279,    -1,
      -1,   282,    -1,   284,   285,    -1,    -1,    -1,    -1,   290,
      -1,   292,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,
      -1,   302,    -1,   304,    -1,    -1,    -1,    -1,   309,    -1,
     311,   312,   313,   314,    -1,    -1,    -1,   318,    -1,   320,
     321,    -1,    -1,   324,   325,    -1,   327,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,    -1,
     341,    -1,   343,   344,   345,   346,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,
      -1,   382,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   394,    -1,    -1,   397,    -1,    -1,    -1,
     401,   402,    -1,   404,    -1,    -1,    -1,    -1,   409,    -1,
      -1,    -1,   413,    -1,    -1,   416,   417,   418,   419,    -1,
      -1,    -1,   423,   424,   425,    -1,   427,   428,    -1,   430,
      -1,   432,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,
     441,   442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   454,     4,     5,     6,     7,     8,   460,
      -1,   462,    12,    -1,   465,    -1,    -1,   468,    -1,   470,
      -1,   472,   473,   474,    -1,    25,    -1,    -1,    28,    29,
      30,    31,    -1,    33,    34,    35,    36,    -1,    -1,    -1,
      40,    -1,    -1,    43,    -1,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    63,    64,    65,    66,    67,    -1,    -1,
      70,    71,    72,    73,    -1,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,   101,   102,    -1,    -1,   105,   106,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,   121,    -1,    -1,   124,   125,   126,    -1,   128,   129,
     130,    -1,    -1,   133,   134,    -1,   136,    -1,   138,   139,
     140,    -1,    -1,    -1,   144,   145,    -1,   147,    -1,   149,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
     160,    -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,
      -1,    -1,    -1,    -1,   174,   175,    -1,    -1,    -1,   179,
      -1,   181,    -1,    -1,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,   203,   204,   205,   206,   207,   208,   209,
      -1,   211,   212,   213,    -1,   215,    -1,   217,    -1,    -1,
     220,    -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   231,   232,   233,    -1,    -1,   236,   237,   238,   239,
     240,   241,   242,   243,    -1,    -1,    -1,    -1,   248,    -1,
     250,    -1,   252,   253,    -1,    -1,   256,   257,   258,   259,
      -1,    -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,
     270,   271,    -1,   273,    -1,    -1,   276,   277,    -1,    -1,
      -1,    -1,   282,    -1,   284,   285,    -1,    -1,    -1,    -1,
     290,    -1,   292,   293,    -1,   295,    -1,    -1,    -1,   299,
      -1,    -1,   302,    -1,   304,    -1,    -1,    -1,    -1,   309,
      -1,   311,   312,   313,   314,    -1,    -1,    -1,   318,    -1,
     320,   321,    -1,    -1,   324,   325,    -1,   327,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,   339,
      -1,   341,    -1,   343,   344,   345,   346,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,    -1,    -1,
      -1,    -1,   382,   383,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   394,    -1,    -1,   397,    -1,    -1,
      -1,   401,   402,    -1,   404,    -1,    -1,    -1,    -1,   409,
      -1,    -1,    -1,   413,    -1,    -1,   416,   417,   418,   419,
      -1,    -1,    -1,   423,   424,   425,    -1,   427,   428,    -1,
     430,    -1,   432,    -1,    -1,    -1,    -1,    -1,    -1,   439,
      -1,   441,   442,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   454,     4,     5,     6,     7,     8,
     460,    -1,   462,    12,    -1,   465,    -1,    -1,   468,    -1,
     470,    -1,   472,   473,   474,    -1,    25,    -1,    -1,    28,
      29,    30,    31,    -1,    33,    34,    35,    36,    -1,    -1,
      -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    63,    64,    65,    66,    67,    -1,
      -1,    70,    71,    72,    73,    -1,    -1,    76,    -1,    78,
      79,    80,    -1,    82,    -1,    84,    85,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,   101,   102,    -1,    -1,   105,   106,    -1,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,   118,
     119,    -1,   121,    -1,    -1,   124,   125,   126,    -1,   128,
     129,   130,    -1,    -1,   133,   134,    -1,   136,    -1,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,   160,    -1,   162,    -1,    -1,   165,    -1,    -1,   168,
      -1,    -1,    -1,    -1,    -1,   174,   175,    -1,    -1,    -1,
     179,    -1,   181,    -1,    -1,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,   203,   204,   205,   206,   207,   208,
     209,    -1,   211,   212,   213,    -1,   215,    -1,   217,    -1,
      -1,   220,    -1,   222,   223,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   231,   232,   233,    -1,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,    -1,    -1,    -1,    -1,   248,
      -1,   250,    -1,   252,   253,    -1,    -1,   256,   257,   258,
     259,    -1,    -1,   262,   263,   264,   265,   266,    -1,    -1,
      -1,   270,   271,    -1,   273,    -1,    -1,   276,   277,    -1,
      -1,    -1,    -1,   282,    -1,   284,   285,    -1,    -1,    -1,
      -1,   290,    -1,   292,   293,    -1,   295,    -1,    -1,    -1,
     299,    -1,    -1,   302,    -1,   304,    -1,    -1,    -1,    -1,
     309,    -1,   311,   312,   313,   314,    -1,    -1,    -1,   318,
      -1,   320,   321,    -1,    -1,   324,   325,    -1,   327,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,
     339,    -1,   341,    -1,   343,   344,   345,   346,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,    -1,
      -1,    -1,    -1,   382,   383,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,   397,    -1,
      -1,    -1,   401,   402,    -1,   404,    -1,    -1,    -1,    -1,
     409,    -1,    -1,    -1,   413,    -1,    -1,   416,   417,   418,
     419,    -1,    -1,    -1,   423,   424,   425,    -1,   427,   428,
      -1,   430,    -1,   432,    -1,    -1,    -1,    -1,    -1,    -1,
     439,    -1,   441,   442,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   454,     4,     5,     6,     7,
       8,   460,    -1,   462,    12,    -1,   465,    -1,    -1,   468,
      -1,   470,    -1,   472,   473,   474,    -1,    25,    -1,    -1,
      28,    29,    30,    31,    -1,    33,    34,    35,    36,    -1,
      -1,    -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,
      48,    49,    -1,    51,    -1,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    70,    71,    72,    73,    -1,    -1,    76,    -1,
      78,    79,    80,    -1,    82,    -1,    84,    85,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
     118,   119,    -1,   121,    -1,    -1,   124,   125,   126,    -1,
     128,   129,   130,    -1,    -1,   133,   134,    -1,   136,    -1,
     138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,   147,
      -1,   149,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,    -1,
     168,    -1,    -1,    -1,    -1,    -1,   174,   175,    -1,    -1,
      -1,   179,    -1,   181,    -1,    -1,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,   203,   204,   205,   206,   207,
     208,   209,    -1,   211,   212,   213,    -1,   215,    -1,   217,
      -1,    -1,   220,    -1,   222,   223,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   231,   232,   233,    -1,    -1,   236,   237,
     238,   239,   240,   241,   242,   243,    -1,    -1,    -1,    -1,
     248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,   257,
     258,   259,    -1,    -1,   262,   263,   264,   265,   266,    -1,
      -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,   277,
      -1,    -1,    -1,    -1,   282,    -1,   284,   285,    -1,    -1,
      -1,    -1,   290,    -1,   292,   293,    -1,   295,    -1,    -1,
      -1,   299,    -1,    -1,   302,    -1,   304,    -1,    -1,    -1,
      -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,    -1,
     318,    -1,   320,   321,    -1,    -1,   324,   325,    -1,   327,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,
     338,   339,    -1,   341,    -1,   343,   344,   345,   346,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,
      -1,    -1,    -1,    -1,   382,   383,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,   397,
      -1,    -1,    -1,   401,   402,    -1,   404,    -1,    -1,    -1,
      -1,   409,    -1,    -1,    -1,   413,    -1,    -1,   416,   417,
     418,   419,    -1,    -1,    -1,   423,   424,   425,    -1,   427,
     428,    -1,   430,    -1,   432,    -1,    -1,    -1,    -1,    -1,
      -1,   439,    -1,   441,   442,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   454,     4,     5,     6,
       7,     8,   460,    -1,   462,    12,    -1,   465,    -1,    -1,
     468,    -1,   470,    -1,   472,   473,   474,    -1,    25,    -1,
      -1,    28,    29,    30,    31,    -1,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    -1,    -1,    43,    -1,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    70,    71,    72,    73,    -1,    -1,    76,
      -1,    78,    79,    80,    -1,    82,    -1,    84,    85,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,   121,    -1,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,    -1,   133,   134,    -1,   136,
      -1,   138,   139,   140,    -1,    -1,    -1,    -1,   145,    -1,
     147,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,   174,   175,    -1,
      -1,    -1,   179,    -1,   181,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,   203,   204,   205,   206,
     207,   208,   209,    -1,   211,   212,   213,    -1,   215,    -1,
     217,    -1,    -1,   220,    -1,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   231,   232,   233,   234,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,    -1,    -1,    -1,
      -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,   256,
     257,   258,   259,    -1,    -1,   262,   263,   264,   265,   266,
      -1,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,   276,
     277,    -1,    -1,    -1,    -1,   282,    -1,   284,   285,    -1,
      -1,    -1,    -1,   290,    -1,   292,   293,    -1,   295,    -1,
      -1,    -1,   299,    -1,    -1,   302,    -1,   304,    -1,    -1,
      -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,    -1,
      -1,   318,    -1,   320,   321,    -1,    -1,   324,   325,    -1,
     327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     337,   338,   339,    -1,   341,    -1,   343,   344,   345,   346,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,   382,   383,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,
     397,    -1,    -1,    -1,   401,   402,    -1,   404,    -1,    -1,
      -1,    -1,   409,    -1,    -1,    -1,   413,    -1,    -1,   416,
     417,   418,   419,    -1,    -1,    -1,   423,   424,   425,    -1,
     427,   428,    -1,   430,    -1,   432,    -1,    -1,    -1,    -1,
      -1,    -1,   439,    -1,   441,   442,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,     4,     5,
       6,     7,     8,   460,    -1,   462,    12,    -1,   465,    -1,
      -1,   468,    -1,   470,    -1,   472,   473,   474,    -1,    25,
      -1,    -1,    28,    29,    30,    31,    -1,    33,    34,    35,
      36,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    54,    -1,
      56,    -1,    -1,    59,    -1,    -1,    62,    63,    64,    65,
      66,    67,    -1,    -1,    70,    71,    72,    73,    -1,    -1,
      76,    -1,    78,    79,    80,    -1,    82,    -1,    84,    85,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,   105,
     106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,   121,    -1,    -1,   124,   125,
     126,    -1,   128,   129,   130,    -1,    -1,   133,   134,    -1,
     136,    -1,   138,   139,   140,    -1,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,
      -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,   174,   175,
      -1,    -1,    -1,   179,    -1,   181,    -1,    -1,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,   203,   204,   205,
     206,   207,   208,   209,    -1,   211,   212,   213,    -1,   215,
      -1,   217,    -1,    -1,   220,    -1,   222,   223,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   231,   232,   233,    -1,    -1,
     236,   237,   238,   239,   240,   241,   242,   243,    -1,    -1,
      -1,    -1,   248,    -1,   250,    -1,   252,   253,    -1,    -1,
     256,   257,   258,   259,    -1,    -1,   262,   263,   264,   265,
     266,    -1,    -1,    -1,   270,   271,    -1,   273,    -1,    -1,
     276,   277,    -1,   279,    -1,    -1,   282,    -1,   284,   285,
      -1,    -1,    -1,    -1,   290,    -1,   292,   293,    -1,   295,
      -1,    -1,    -1,   299,    -1,    -1,   302,    -1,   304,    -1,
      -1,    -1,    -1,   309,    -1,   311,   312,   313,   314,    -1,
      -1,    -1,   318,    -1,   320,   321,    -1,    -1,   324,   325,
      -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   338,   339,    -1,   341,    -1,   343,   344,   345,
     346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   375,
      -1,    -1,    -1,    -1,    -1,    -1,   382,   383,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,
      -1,   397,    -1,    -1,    -1,   401,   402,    -1,   404,    -1,
      -1,    -1,    -1,   409,    -1,    -1,    -1,   413,    -1,    -1,
     416,   417,   418,   419,    -1,    -1,    -1,   423,   424,   425,
      -1,   427,   428,    -1,   430,    -1,   432,    -1,    -1,    -1,
      -1,    -1,    -1,   439,    -1,   441,   442,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,     4,
       5,     6,     7,     8,   460,    -1,   462,    12,    -1,   465,
      -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,    -1,
      25,    -1,    -1,    28,    29,    30,    31,    -1,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    70,    71,    72,    73,    -1,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,    84,
      85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    -1,
      95,    96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,
     105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,   118,   119,    -1,   121,    -1,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,    -1,   133,   134,
      -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,    -1,
     145,    -1,   147,    -1,   149,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,   160,    -1,   162,    -1,    -1,
     165,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,   174,
     175,    -1,    -1,    -1,   179,    -1,   181,    -1,    -1,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,   203,   204,
     205,   206,   207,   208,   209,    -1,   211,   212,   213,    -1,
     215,    -1,   217,    -1,    -1,   220,    -1,   222,   223,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,    -1,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,    -1,
      -1,    -1,    -1,   248,    -1,   250,    -1,   252,   253,    -1,
      -1,   256,   257,   258,   259,    -1,    -1,   262,   263,   264,
     265,   266,    -1,    -1,    -1,   270,   271,    -1,   273,    -1,
      -1,   276,   277,    -1,    -1,    -1,    -1,   282,    -1,   284,
     285,    -1,    -1,    -1,    -1,   290,    -1,   292,   293,    -1,
     295,    -1,    -1,    -1,   299,    -1,    -1,   302,    -1,   304,
      -1,    -1,    -1,    -1,   309,    -1,   311,   312,   313,   314,
      -1,    -1,    -1,   318,    -1,   320,   321,    -1,    -1,   324,
     325,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   337,   338,   339,    -1,   341,    -1,   343,   344,
     345,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     375,    -1,    -1,    -1,    -1,    -1,    -1,   382,   383,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,
      -1,    -1,   397,    -1,    -1,    -1,   401,   402,    -1,   404,
      -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,   413,    -1,
      -1,   416,   417,   418,   419,    -1,    -1,    -1,   423,   424,
     425,    -1,   427,   428,    -1,   430,    -1,   432,    -1,    -1,
      -1,    -1,    -1,    -1,   439,    -1,   441,   442,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,
       4,     5,     6,     7,     8,   460,    -1,   462,    12,    -1,
     465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,
      -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,
      -1,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    63,
      64,    65,    66,    67,    -1,    -1,    70,    71,    72,    73,
      -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,
      84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    99,    -1,   101,   102,    -1,
      -1,   105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,    -1,
     124,   125,   126,    -1,   128,   129,   130,    -1,    -1,   133,
     134,    -1,   136,    -1,   138,   139,   140,    -1,    -1,    -1,
      -1,   145,    -1,   147,    -1,   149,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,   160,    -1,   162,    -1,
      -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,
     174,   175,    -1,    -1,    -1,   179,    -1,   181,    -1,    -1,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,   203,
     204,   205,   206,   207,   208,   209,    -1,   211,   212,   213,
      -1,   215,    -1,   217,    -1,    -1,   220,    -1,   222,   223,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,
      -1,    -1,   236,   237,   238,   239,   240,   241,   242,   243,
      -1,    -1,    -1,    -1,   248,    -1,   250,    -1,   252,   253,
      -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,   263,
     264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,   273,
      -1,    -1,   276,   277,    -1,    -1,    -1,    -1,   282,    -1,
     284,   285,    -1,    -1,    -1,    -1,   290,    -1,   292,   293,
      -1,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,    -1,
     304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,   313,
     314,    -1,    -1,    -1,   318,    -1,   320,   321,    -1,    -1,
     324,   325,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   337,   338,   339,    -1,   341,    -1,   343,
     344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,   382,   383,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     394,    -1,    -1,   397,    -1,    -1,    -1,   401,   402,    -1,
     404,    -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,   413,
      -1,    -1,   416,   417,   418,   419,    -1,    -1,    -1,   423,
     424,   425,    -1,   427,   428,    -1,   430,    -1,   432,    -1,
      -1,    -1,    -1,    -1,    -1,   439,    -1,   441,   442,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     454,     4,     5,     6,     7,     8,   460,    -1,   462,    12,
      -1,   465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,
     474,    -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,
      33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,    -1,
      43,    -1,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,
      63,    64,    65,    66,    67,    -1,    -1,    70,    71,    72,
      73,    -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,
      -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    99,    -1,   101,   102,
      -1,    -1,   105,   106,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,
      -1,   124,   125,   126,    -1,   128,   129,   130,    -1,    -1,
     133,   134,    -1,   136,    -1,   138,   139,   140,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,   149,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,   162,
      -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,    -1,
      -1,   174,   175,    -1,    -1,    -1,   179,    -1,   181,    -1,
      -1,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
     203,   204,   205,   206,   207,   208,   209,    -1,   211,   212,
     213,    -1,   215,    -1,   217,    -1,    -1,   220,    -1,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,   232,
     233,    -1,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,    -1,    -1,    -1,   248,    -1,   250,    -1,   252,
     253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,   262,
     263,   264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,
     273,    -1,    -1,   276,   277,    -1,    -1,    -1,    -1,   282,
      -1,   284,   285,    -1,    -1,    -1,    -1,   290,    -1,   292,
     293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,   302,
      -1,   304,    -1,    -1,    -1,    -1,   309,    -1,   311,   312,
     313,   314,    -1,    -1,    -1,   318,    -1,   320,   321,    -1,
      -1,   324,   325,    -1,   327,    -1,    -1,    -1,    94,    95,
      96,    97,    -1,    -1,   337,   338,   339,    -1,   341,    -1,
     343,   344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   375,    -1,   140,    -1,    -1,    -1,    -1,   382,
     383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   394,    -1,    -1,   397,    -1,    -1,    -1,   401,   402,
      -1,   404,    -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,
     413,    -1,    -1,   416,   417,   418,   419,   183,    -1,    -1,
     423,   424,   425,    -1,   427,   428,    -1,   430,    -1,   432,
      -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,   441,   442,
      -1,    -1,    -1,   209,    -1,    -1,    -1,   213,    -1,   215,
      -1,   454,    -1,    -1,    -1,    -1,    -1,   460,    -1,   462,
      -1,    -1,   465,    -1,    -1,   468,    -1,   470,    -1,   472,
     473,   474,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   260,    -1,   262,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    -1,   315,
     316,   317,    -1,    -1,   320,   321,   322,   323,   324,    -1,
     326,   327,   328,    56,   330,   331,   332,   333,   334,   335,
      -1,    -1,    -1,    -1,   340,    -1,   342,    70,   344,   345,
     346,   347,   348,   349,    -1,    -1,   352,   353,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,    -1,    -1,    -1,    -1,   110,    -1,   112,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   394,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   409,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,   149,   423,   424,   425,
      -1,    -1,    -1,    -1,   430,    -1,   432,    -1,   161,   162,
      -1,    -1,   165,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   462,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   264,    -1,    -1,    -1,    -1,    -1,   270,   271,    -1,
      -1,   274,   275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   312,
     313,    -1,    -1,    -1,   317,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   382,
     383
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     3,     5,    26,    27,    30,    32,    35,    38,    41,
      43,    44,    46,    48,    49,    50,    52,    53,    55,    60,
      61,    62,    64,    65,    67,    68,    69,    70,    73,    76,
      80,    81,    83,    92,   102,   106,   107,   122,   123,   148,
     249,   253,   255,   260,   300,   337,   338,   436,   486,   496,
     497,   498,   499,   500,   502,   507,   508,   513,   611,   626,
     629,   637,   639,   641,   644,   649,   651,   656,   659,   663,
     667,   675,   676,   783,   785,   792,   795,   821,   829,   840,
     842,   856,   859,   862,   868,   872,   876,   877,   878,   910,
     925,   931,   932,   939,   942,   965,   968,   969,   970,     4,
       5,     6,     7,     8,    12,    25,    28,    29,    30,    31,
      33,    34,    35,    36,    40,    43,    45,    46,    47,    48,
      49,    51,    54,    56,    59,    62,    63,    64,    65,    66,
      67,    70,    71,    72,    73,    76,    78,    79,    80,    82,
      84,    85,    90,    91,    92,    93,    95,    96,    97,    99,
     101,   102,   105,   106,   108,   110,   113,   118,   121,   124,
     125,   126,   128,   129,   130,   133,   134,   136,   138,   139,
     140,   145,   147,   149,   153,   156,   157,   158,   160,   162,
     165,   168,   174,   175,   179,   181,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   202,   203,   204,   205,   206,   207,   208,   209,
     211,   212,   213,   215,   217,   220,   222,   223,   231,   232,
     233,   236,   237,   238,   239,   240,   241,   242,   243,   248,
     250,   252,   253,   256,   257,   258,   259,   262,   263,   264,
     265,   266,   270,   271,   273,   276,   277,   282,   284,   285,
     290,   292,   293,   295,   299,   302,   304,   309,   311,   312,
     313,   314,   318,   320,   321,   324,   325,   327,   337,   338,
     339,   341,   343,   344,   345,   346,   356,   375,   382,   383,
     394,   397,   401,   402,   404,   409,   413,   416,   417,   418,
     419,   423,   424,   425,   427,   428,   430,   432,   439,   441,
     442,   454,   460,   462,   465,   468,   470,   472,   473,   474,
     493,   901,   902,   903,   906,   909,   159,   368,   622,   179,
     308,   658,   966,    56,    85,   146,   267,   277,   297,   368,
     531,   532,   533,   546,   603,   830,   784,   161,   277,   292,
     368,   454,   791,   906,   658,   176,   279,   904,   906,   907,
     793,    19,    29,    37,    50,    57,    58,    74,    75,    77,
      78,    80,    94,   104,   105,   110,   115,   116,   119,   135,
     137,   141,   150,   154,   173,   180,   182,   201,   214,   216,
     218,   219,   245,   258,   279,   283,   286,   287,   288,   289,
     294,   303,   314,   317,   319,   321,   345,   346,   350,   357,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   375,   376,   377,   378,   380,   381,   384,   385,
     386,   387,   388,   390,   391,   392,   393,   394,   395,   396,
     397,   401,   403,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   416,   419,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   435,   436,
     437,   438,   439,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     462,   463,   464,   478,   479,   484,   486,   489,   490,   492,
     493,   494,   696,   699,   702,   703,   704,   707,   890,   892,
     894,   895,   899,   905,   906,   979,   985,   118,   161,   275,
     274,   275,   927,   658,   873,   658,   869,   280,   906,   678,
     680,   681,   843,    73,    76,    70,   282,    70,   275,   841,
     927,   822,   927,   161,   927,   927,   943,   927,   927,   940,
     224,   912,   906,   906,   796,    68,   486,   677,     0,     3,
     337,   136,   860,   901,   906,   906,   223,     4,   244,   493,
     614,   906,   275,   927,   460,   967,   280,   403,   534,   275,
     532,   292,   161,   159,   184,   339,   838,   839,   717,   718,
     906,   903,   403,   790,   787,   927,   503,   863,   120,   155,
     184,   798,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   696,   486,   486,   486,   486,   486,   695,
     486,   486,   981,   985,   486,   486,   486,   486,   720,   721,
     722,   696,   486,   486,   486,   486,   486,   154,   279,   486,
     486,   486,   486,   294,   486,   890,   486,   890,   486,   890,
     486,   486,   486,   486,   696,   724,   486,   486,   486,   695,
     486,   695,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   696,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   695,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     695,   695,   695,   486,   486,   486,   486,   696,   696,   696,
      68,   696,   696,   906,   492,   907,   906,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      87,   167,   177,   216,   226,   227,   247,   310,   358,   476,
     477,   478,   479,   480,   481,   482,   483,   697,   371,   372,
     373,   374,   375,   398,   399,   400,   401,   413,   414,   415,
     416,   419,   439,   440,   461,   462,   747,   109,   905,   493,
     678,   980,   113,   144,   184,   881,   166,   901,   926,   927,
      56,    94,   848,   874,   927,    56,    70,   238,   870,   871,
      67,   679,    86,   127,   155,   272,   467,   468,   469,   470,
     471,   472,   685,   686,   687,    31,    37,    38,    56,    70,
      93,   105,   110,   112,   117,   145,   147,   149,   161,   162,
     165,   170,   179,   181,   212,   223,   235,   264,   270,   271,
     274,   275,   312,   313,   317,   382,   579,   602,   844,   848,
     849,   852,   917,   631,   632,   631,   631,   306,   630,   631,
     789,   901,   184,   828,   640,   664,   665,   901,   642,   652,
      26,    38,    41,    44,    46,    50,    55,    63,    68,    69,
      79,    83,    86,   138,   148,   161,   236,   246,   248,   266,
     298,   944,   946,   947,   660,   638,    86,   941,   944,   911,
     144,   120,   799,   828,   680,   677,   487,   906,   858,   857,
      10,    88,   749,   750,   933,   906,   613,   901,   650,   509,
     216,   906,   534,   903,   906,   144,   831,   835,   836,   906,
     838,   696,   719,   221,   135,   906,   116,   907,   908,   956,
     957,   790,   301,   504,    56,    70,   124,   156,   181,   235,
     238,   252,   270,   274,   864,   865,   927,   622,   696,    86,
     714,   751,   127,   714,   751,   696,   714,   714,   714,   714,
     714,   696,   696,   696,   696,   696,   696,   487,   899,   678,
     982,   696,   696,   696,   721,   356,   723,   899,   696,   487,
     696,   696,   696,   899,   696,   717,   717,   696,   696,   696,
     696,   714,   714,   714,   459,   717,   696,   696,   696,   487,
     696,   696,   696,   696,   696,   696,   696,   696,   696,   696,
     747,   696,   696,   696,   696,   696,   696,   717,   127,   711,
     890,   320,   321,   345,   346,   748,   696,   696,   487,   696,
     696,   696,   717,   696,   696,   696,   696,   696,   696,   696,
     696,   696,   717,   717,   717,   696,   696,   696,   696,   696,
     696,   717,   700,   703,   487,   696,   696,   696,   696,   696,
     696,   696,   696,    98,   172,   281,   696,   706,   717,   706,
     706,   706,   706,   706,   890,   487,   696,   487,   696,   696,
     696,   141,   182,   219,   245,   350,   770,   487,   488,   696,
     147,   179,   264,   918,    22,   493,   696,   696,   177,   216,
     218,   696,   696,   696,   696,   486,   983,   985,   703,   167,
     177,   247,   358,   696,   696,   696,   696,   701,   703,   696,
     696,   696,   702,   696,   702,   696,   696,   696,   696,    28,
      86,   696,   698,   479,   907,   906,   487,   986,    56,   179,
     880,   102,   144,   901,   928,   929,   657,   181,   645,   102,
     488,   906,   296,   971,   972,   480,   689,   690,   691,   687,
      45,   486,   275,   368,   270,   156,   270,   181,   177,   851,
     285,   851,   389,   270,    56,   274,   851,   144,   167,   850,
     853,   270,   285,   329,   765,   767,   765,   260,   541,   907,
     851,   853,   181,   383,   111,   237,   304,   628,    51,    72,
     633,   634,   627,    36,   622,   788,   789,   167,   488,   671,
     672,   788,   788,   277,   949,   274,    33,    70,   948,   117,
     950,   235,   945,   224,   951,   221,   488,   661,   662,   901,
     788,   945,   221,   147,   179,   222,   264,   913,   914,   915,
     916,   492,   501,   904,   166,   789,   800,   801,   487,   972,
     974,   975,   976,   675,   154,   861,   891,   905,   906,   906,
       7,   139,   906,   934,   935,   119,   528,   529,   530,   539,
     540,   587,   612,   788,   185,   186,   187,   188,   189,   190,
     191,   193,   194,   195,   196,   197,   198,   199,   200,   510,
     511,   512,   135,   516,   901,   517,   284,   301,   604,   835,
     901,   906,   488,   833,   493,   837,   488,   901,   695,   492,
     402,   488,   788,   492,   505,   506,   102,   306,   488,   866,
     800,   488,   487,   715,   708,   487,   480,   488,   487,   487,
     487,   487,   487,   488,   488,    88,   487,   487,   301,   488,
     487,   986,   488,   488,   487,   488,   487,   486,   488,   488,
     487,   488,   488,   487,   487,   487,   487,   487,   487,   488,
     487,   487,   488,   487,   487,   487,   726,   727,   487,   488,
     488,   487,   488,   488,   487,   488,   487,   488,   487,   488,
     488,   488,   487,   488,   488,   144,   488,   488,   487,   488,
     487,   488,   487,   488,   487,   488,   487,   710,   488,   488,
     487,   488,   487,   488,   487,   488,   487,   488,   488,   488,
     487,   487,   487,   487,   488,   487,   488,   487,   488,   487,
     487,   487,   487,   487,   487,   487,   488,   488,   487,   488,
     487,     9,    11,    12,    18,    19,    20,    21,    87,   167,
     177,   216,   226,   227,   247,   310,   358,   476,   477,   478,
     479,   480,   481,   482,   483,   697,   487,   488,   488,   487,
     488,   487,   488,   144,   488,   488,   144,   696,   144,   696,
     144,   696,   144,   487,   487,   487,   487,   487,   487,   487,
     488,   487,   487,   488,   487,   487,   488,   488,   717,   491,
     493,   493,   493,   907,   696,   906,   696,   218,   717,   678,
     984,   134,   756,   486,   983,   703,   696,   701,     9,    11,
      12,    18,    19,    20,    21,    87,   167,   177,   216,   226,
     227,   247,   310,   358,   476,   477,   478,   479,   480,   481,
     482,   483,   697,   747,   747,   983,   696,   493,   163,   668,
      56,   750,   488,   788,   280,   475,   875,   788,   871,    86,
     127,   978,   488,   682,   693,   696,   897,   906,   167,   854,
     480,   901,   534,   904,   908,   389,   850,   851,   906,   850,
     768,   770,   845,   901,   853,   851,    82,   635,   488,   635,
      71,   486,   490,   729,   734,   735,   280,   488,   119,   666,
     906,   665,   161,   171,   600,   601,   673,   136,   339,   643,
     136,   206,   318,   325,   339,   653,   654,   655,   274,   486,
     958,   958,   958,   958,   480,   901,   906,   955,   947,   488,
     280,   144,   488,   955,   488,   119,   282,   906,   919,   921,
     208,   428,   492,   579,   920,   907,   801,   797,   228,   761,
     767,   977,     6,     7,     8,    10,    13,    14,    15,    16,
     139,   936,   938,   305,   752,   530,   109,   579,    24,    26,
      32,    34,    44,    90,    91,   106,   115,   118,   121,   125,
     126,   130,   160,   161,   202,   207,   232,   241,   242,   243,
     249,   257,   284,   296,   382,   404,   418,   428,   536,   538,
     539,   540,   615,   616,   617,   761,   653,    10,    10,    10,
      10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
      10,    10,   488,   528,   514,   290,    99,   153,   259,   605,
     605,   221,   834,   832,   493,   836,   144,   480,   906,   750,
     696,   786,   907,   100,   957,   103,   254,   623,   907,   488,
     244,   865,   788,   867,   794,   696,   696,   717,   487,   696,
     405,   696,   696,    94,   213,   317,   320,   321,   341,   346,
     351,   716,    94,   580,   907,   716,   717,   717,   696,   696,
     899,   717,   696,   696,   696,   696,   379,   725,   696,   728,
     717,   696,   702,   702,   905,   696,   696,   717,   905,   696,
     696,   696,   717,   219,   696,   696,   696,   696,   717,   219,
     696,   696,   696,   696,   696,   717,   696,   696,   696,   696,
     696,   696,   696,   696,   696,   177,   216,   218,   696,   696,
     696,   696,   696,   703,   177,   247,   358,   696,   696,   696,
     696,   701,   696,   696,   696,   702,   696,   702,   696,   696,
     696,   696,   696,   698,   696,   696,   696,   405,   696,   696,
     696,   696,   696,   144,   696,   144,   696,   144,   696,   219,
     696,   696,   696,   487,   493,   609,   487,   986,   703,   717,
     756,    87,   696,   696,   177,   216,   218,   696,   696,   696,
     696,   486,   983,   703,   167,   177,   247,   358,   696,   696,
     696,   696,   701,   696,   696,   696,   702,   696,   702,   696,
     696,   696,   696,   696,   698,   906,   904,   669,   670,   901,
     184,   244,   307,   930,   929,   653,   904,   696,   136,   299,
     339,   646,   647,   648,   973,   690,   144,   166,   683,   684,
     760,   761,   781,   692,   493,   904,   144,   855,   487,   906,
      70,   851,   851,   220,   488,   181,   270,   847,   850,   901,
     512,   636,   634,    68,   486,   729,   736,   906,    39,   164,
     169,   210,   260,   272,   407,   437,   488,   733,   901,   904,
     789,   486,   654,   906,   959,   960,   493,   493,   280,   662,
     901,   904,   148,   144,   914,   493,   168,   493,    10,    22,
     812,   119,   580,   581,   389,   812,   492,   907,    94,   119,
     582,   583,   907,    68,   260,   302,   303,   486,   524,   802,
     806,   100,   765,   937,   753,   766,   767,   812,   813,   813,
     112,   114,   146,   267,   551,   554,   555,   598,   600,   621,
     621,   621,   813,   143,   234,   600,   621,   813,   813,   813,
     280,   133,   813,   170,   276,   170,   276,   133,   813,   813,
     813,   813,   813,   813,    10,    88,   280,   625,   813,   813,
     813,   813,   813,   621,   813,   536,   488,   486,   493,   550,
     556,   900,   906,   904,   904,   904,   141,   182,   219,   245,
     350,   771,   904,   770,   770,   770,   904,   904,   904,   904,
     904,   904,   770,   511,   177,   486,   518,   535,   537,   538,
     328,   340,   343,   547,   901,   301,   752,   906,   729,   493,
     837,   279,   428,   506,    55,   802,   487,   709,   488,   696,
     487,   487,   486,   573,   573,   573,   328,   328,   487,   487,
     487,   487,   487,   487,   167,   306,   705,   487,   487,   488,
     487,   487,   696,   465,   466,   459,   487,   488,   747,   747,
     487,   487,   487,   487,   487,   487,   488,   487,   487,   487,
     487,   487,   487,   487,   713,   761,   488,   487,   488,   487,
     487,   487,   488,   487,   487,   488,   487,   487,   487,   487,
     487,   487,   696,   218,   487,   756,   703,   696,   701,    87,
     747,   747,   983,   488,   487,   487,   696,   487,   389,   487,
     487,   488,   488,   487,   696,   487,   696,   487,   696,   487,
     488,   487,   487,   487,   906,   487,   696,   696,   218,   717,
     756,   486,   983,   703,   696,   701,    87,   747,   747,   983,
     879,   488,   671,   307,   179,   647,   676,    42,   729,   230,
     231,   777,   778,    55,   389,   688,   781,   766,   684,    88,
     694,   904,   906,   480,   906,   771,   846,   312,   313,   306,
     770,   770,   850,   488,   737,   738,   736,   487,   974,   734,
     169,   169,   169,   407,   437,   824,   825,   899,   729,   229,
     740,   740,   729,   729,   750,   234,   745,   906,   487,   488,
     480,   480,   956,   224,   956,   906,   175,   906,    86,    94,
     119,   221,   696,   924,   109,   585,   908,   279,   427,   428,
     923,   918,   812,   525,   803,   486,   809,   814,   809,   487,
     524,   805,   896,   897,   899,   807,   762,   486,   696,   119,
     584,   586,   907,   581,   608,   900,   601,   601,   107,   143,
     234,   297,   553,   599,   552,   553,   608,   900,   900,   904,
     171,   171,   900,   900,   771,   770,   770,   579,   813,   770,
     813,   771,   771,   119,   770,   239,   240,   543,   770,   770,
     770,   901,   119,   129,   140,   311,   542,   541,   486,   541,
       6,   139,   217,   544,   900,   904,   617,   548,   549,   550,
     551,   906,    25,   139,   624,   246,   552,   590,   557,   493,
     901,   177,   519,   524,   548,   159,   436,   521,   882,   488,
     537,   291,   515,   729,   760,   752,   750,   279,   221,   819,
     487,   696,   747,   219,   101,   295,   314,   579,   588,    97,
     238,   487,   696,   696,   696,   696,   487,   487,   696,   261,
     712,   219,   696,   696,   696,   756,    87,   696,   696,   747,
     696,   696,   696,   487,   487,   487,   219,   487,   717,   756,
      87,   696,   882,   670,   159,   674,   766,   752,   904,   904,
     492,   779,   780,   167,    83,   904,   906,   493,   765,   187,
     851,   512,   685,   487,   487,   407,   729,   740,   740,   488,
     823,   812,   169,   169,   221,   301,   142,   159,   300,   741,
     487,   488,   960,   251,   961,   144,   244,   250,   263,   922,
     586,   812,   486,   486,   921,   696,   685,   810,   811,   899,
     815,   488,   486,   806,   487,   487,   488,   971,   696,   763,
     898,   119,   696,   817,   818,   696,   171,   171,   601,   608,
     604,    44,   260,   618,   608,   623,   581,   904,   904,   788,
     619,   487,   488,   906,   901,    94,    95,    96,    97,   140,
     183,   209,   213,   215,   260,   262,   315,   316,   317,   320,
     321,   322,   323,   324,   326,   327,   328,   330,   331,   332,
     333,   334,   335,   340,   342,   344,   345,   346,   347,   348,
     349,   352,   353,   394,   409,   423,   424,   425,   430,   432,
     462,   558,   561,   562,   563,   564,   565,   566,   567,   906,
     901,   487,   487,    88,   527,   537,   904,   486,   752,   329,
     769,   128,   488,   487,   487,   580,   417,    47,   487,   466,
     487,   487,   488,   891,   487,   488,   487,   487,   487,   696,
     487,   487,   487,   487,   487,   488,   487,   696,   166,   174,
     151,   757,   782,   907,   488,   441,   480,    10,   689,   750,
     229,   169,   169,   825,   752,   818,   729,   729,   696,   730,
     600,   742,   742,   742,   906,    59,   268,   273,   309,   473,
     474,   953,   954,   306,   962,   956,   108,   293,   244,   923,
     279,   279,   812,   689,   488,   812,   816,   817,   814,   808,
     806,   896,   488,    89,   122,   764,   487,   488,   608,   604,
     486,   119,   119,   556,   585,   487,   558,   549,   591,   486,
     573,   486,   568,   569,   317,   352,   562,   564,   588,   317,
     353,   353,   354,   560,   573,   568,   336,   559,   568,   588,
     588,   568,   573,   573,   588,   486,   573,    34,    90,    94,
     109,   119,   216,   218,   221,   234,   262,   297,   575,   576,
     577,   589,   354,   486,   588,   486,   486,   486,   573,   486,
     569,   574,   493,   487,   520,   535,   486,   524,   606,   607,
     906,   771,   820,   696,   696,   696,   714,   717,   275,   100,
     152,   754,   111,   883,   780,   417,   904,   739,   169,   729,
     729,   760,   221,   731,   221,   732,   486,   743,   279,   279,
     279,    87,   952,   148,   203,   204,   205,   963,   964,   487,
     487,   924,   526,   811,   818,   487,   974,   898,   818,   486,
     486,   606,   321,   345,   346,   478,   479,   893,   894,   624,
     575,   201,   221,   486,   592,   594,   595,   596,   219,   219,
     341,   351,   355,   570,   571,   572,   588,   354,   486,   570,
     486,   570,   570,   588,   219,   570,   904,   584,   426,   578,
     893,   218,    83,   119,   171,   577,   171,   219,   219,   219,
     219,   570,   219,   570,   906,   974,   521,   524,   522,   487,
     488,   764,   486,   171,   487,   487,   488,   487,   487,   901,
     758,   898,   755,   760,   131,   132,   225,   278,   884,   885,
     178,   886,    87,   545,   684,   766,   734,   769,   696,   301,
     696,   301,   746,   906,   486,   953,   224,   770,   770,   770,
     964,   545,   764,   606,   606,   487,   895,   895,   620,   145,
     233,   265,    41,    83,   593,   906,   596,   487,   487,   488,
     572,   610,   891,   610,   487,   695,   426,   302,   487,   487,
     487,   487,   487,   971,   607,   219,    83,   696,   883,   306,
     488,   759,   764,   696,   766,   100,   100,   131,   100,   885,
     269,   278,   887,   888,   188,   688,   221,   486,   486,   487,
     488,   744,   745,   487,   487,   624,   103,   217,   254,   260,
     597,   597,   487,   488,   219,   487,   488,   487,   695,   588,
     588,   523,   764,   487,   826,   827,   899,   487,   886,    40,
      66,   898,   434,   772,   891,   891,   100,   891,   100,   100,
     888,    10,   696,   746,   746,   906,   487,   590,    84,   119,
     218,   594,   906,   487,   588,   891,   588,   974,   488,   812,
     159,   889,   764,   906,   891,   891,   891,   771,   491,   487,
     487,   827,   818,   219,   486,   804,   773,    87,   178,   487,
     805,   486,   192,   487,   691,   774,   775,   776,    10,   696,
     487,   488,   770,   776
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 745 "sql_yacc.yy"
    {
	   THD *thd= YYTHD;
	   if (!thd->bootstrap &&
	      (!(thd->lex->select_lex.options & OPTION_FOUND_COMMENT)))
	   {
	     send_error(thd,ER_EMPTY_QUERY);
	     YYABORT;
	   }
	   else
	   {
	     thd->lex->sql_command= SQLCOM_EMPTY_QUERY;
	   }
	}
    break;

  case 3:
#line 758 "sql_yacc.yy"
    {}
    break;

  case 49:
#line 810 "sql_yacc.yy"
    {
          THD *thd=YYTHD;
          LEX *lex= thd->lex;
          if (thd->command == COM_PREPARE)
          {
            yyerror(ER(ER_SYNTAX_ERROR));
            YYABORT;
          }
          lex->sql_command= SQLCOM_DEALLOCATE_PREPARE;
          lex->prepared_stmt_name= yyvsp[0].lex_str;
        }
    break;

  case 52:
#line 830 "sql_yacc.yy"
    {
          THD *thd=YYTHD;
          LEX *lex= thd->lex;
          if (thd->command == COM_PREPARE)
          {
            yyerror(ER(ER_SYNTAX_ERROR));
            YYABORT;
          }
          lex->sql_command= SQLCOM_PREPARE;
          lex->prepared_stmt_name= yyvsp[-2].lex_str;
        }
    break;

  case 53:
#line 844 "sql_yacc.yy"
    {
          THD *thd=YYTHD;
          LEX *lex= thd->lex;
          lex->prepared_stmt_code= yyvsp[0].lex_str;
          lex->prepared_stmt_code_is_varref= FALSE;
        }
    break;

  case 54:
#line 851 "sql_yacc.yy"
    {
          THD *thd=YYTHD;
          LEX *lex= thd->lex;
          lex->prepared_stmt_code= yyvsp[0].lex_str;
          lex->prepared_stmt_code_is_varref= TRUE;
        }
    break;

  case 55:
#line 860 "sql_yacc.yy"
    {
          THD *thd=YYTHD;
          LEX *lex= thd->lex;
          if (thd->command == COM_PREPARE)
          {
            yyerror(ER(ER_SYNTAX_ERROR));
            YYABORT;
          }
          lex->sql_command= SQLCOM_EXECUTE;
          lex->prepared_stmt_name= yyvsp[0].lex_str;
        }
    break;

  case 56:
#line 872 "sql_yacc.yy"
    {}
    break;

  case 61:
#line 886 "sql_yacc.yy"
    {
          LEX *lex=Lex;
          LEX_STRING *lexstr= (LEX_STRING*)sql_memdup(&yyvsp[0].lex_str, sizeof(LEX_STRING));
          if (!lexstr || lex->prepared_stmt_params.push_back(lexstr))
              YYABORT;
        }
    break;

  case 62:
#line 898 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  lex->sql_command= SQLCOM_HELP;
	  lex->help_arg= yyvsp[0].lex_str.str;
       }
    break;

  case 63:
#line 908 "sql_yacc.yy"
    {
	  LEX *lex = Lex;
	  lex->sql_command = SQLCOM_CHANGE_MASTER;
	  bzero((char*) &lex->mi, sizeof(lex->mi));
        }
    break;

  case 64:
#line 914 "sql_yacc.yy"
    {}
    break;

  case 67:
#line 923 "sql_yacc.yy"
    {
	 Lex->mi.host = yyvsp[0].lex_str.str;
       }
    break;

  case 68:
#line 928 "sql_yacc.yy"
    {
	 Lex->mi.user = yyvsp[0].lex_str.str;
       }
    break;

  case 69:
#line 933 "sql_yacc.yy"
    {
	 Lex->mi.password = yyvsp[0].lex_str.str;
       }
    break;

  case 70:
#line 938 "sql_yacc.yy"
    {
	 Lex->mi.port = yyvsp[0].ulong_num;
       }
    break;

  case 71:
#line 943 "sql_yacc.yy"
    {
	 Lex->mi.connect_retry = yyvsp[0].ulong_num;
       }
    break;

  case 72:
#line 947 "sql_yacc.yy"
    {
           Lex->mi.ssl= yyvsp[0].ulong_num ? 
               LEX_MASTER_INFO::SSL_ENABLE : LEX_MASTER_INFO::SSL_DISABLE;
         }
    break;

  case 73:
#line 952 "sql_yacc.yy"
    {
           Lex->mi.ssl_ca= yyvsp[0].lex_str.str;
         }
    break;

  case 74:
#line 956 "sql_yacc.yy"
    {
           Lex->mi.ssl_capath= yyvsp[0].lex_str.str;
         }
    break;

  case 75:
#line 960 "sql_yacc.yy"
    {
           Lex->mi.ssl_cert= yyvsp[0].lex_str.str;
         }
    break;

  case 76:
#line 964 "sql_yacc.yy"
    {
           Lex->mi.ssl_cipher= yyvsp[0].lex_str.str;
         }
    break;

  case 77:
#line 968 "sql_yacc.yy"
    {
           Lex->mi.ssl_key= yyvsp[0].lex_str.str;
	 }
    break;

  case 79:
#line 977 "sql_yacc.yy"
    {
	 Lex->mi.log_file_name = yyvsp[0].lex_str.str;
       }
    break;

  case 80:
#line 981 "sql_yacc.yy"
    {
           Lex->mi.pos = yyvsp[0].ulonglong_number;
           /* 
              If the user specified a value < BIN_LOG_HEADER_SIZE, adjust it
              instead of causing subsequent errors. 
              We need to do it in this file, because only there we know that 
              MASTER_LOG_POS has been explicitely specified. On the contrary
              in change_master() (sql_repl.cc) we cannot distinguish between 0
              (MASTER_LOG_POS explicitely specified as 0) and 0 (unspecified),
              whereas we want to distinguish (specified 0 means "read the binlog
              from 0" (4 in fact), unspecified means "don't change the position
              (keep the preceding value)").
           */
           Lex->mi.pos = max(BIN_LOG_HEADER_SIZE, Lex->mi.pos);
         }
    break;

  case 81:
#line 997 "sql_yacc.yy"
    {
           Lex->mi.relay_log_name = yyvsp[0].lex_str.str;
         }
    break;

  case 82:
#line 1001 "sql_yacc.yy"
    {
           Lex->mi.relay_log_pos = yyvsp[0].ulong_num;
           /* Adjust if < BIN_LOG_HEADER_SIZE (same comment as Lex->mi.pos) */
           Lex->mi.relay_log_pos = max(BIN_LOG_HEADER_SIZE, Lex->mi.relay_log_pos);
         }
    break;

  case 83:
#line 1012 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_CREATE_TABLE;
	  if (!lex->select_lex.add_table_to_list(thd, yyvsp[0].table, NULL,
						 TL_OPTION_UPDATING,
						 (using_update_log ?
						  TL_READ_NO_INSERT:
						  TL_READ)))
	    YYABORT;
	  lex->create_list.empty();
	  lex->key_list.empty();
	  lex->col_list.empty();
	  lex->change=NullS;
	  bzero((char*) &lex->create_info,sizeof(lex->create_info));
	  lex->create_info.options=yyvsp[-3].num | yyvsp[-1].num;
	  lex->create_info.db_type= (enum db_type) lex->thd->variables.table_type;
	  lex->create_info.default_table_charset= NULL;
	  lex->name=0;
	}
    break;

  case 84:
#line 1033 "sql_yacc.yy"
    { Lex->current_select= &Lex->select_lex; }
    break;

  case 85:
#line 1035 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_CREATE_INDEX;
	    if (!lex->current_select->add_table_to_list(lex->thd, yyvsp[0].table, NULL,
							TL_OPTION_UPDATING))
	      YYABORT;
	    lex->create_list.empty();
	    lex->key_list.empty();
	    lex->col_list.empty();
	    lex->change=NullS;
	  }
    break;

  case 86:
#line 1047 "sql_yacc.yy"
    {
	    LEX *lex=Lex;

	    lex->key_list.push_back(new Key(yyvsp[-9].key_type,yyvsp[-7].lex_str.str, yyvsp[-6].key_alg, 0, lex->col_list));
	    lex->col_list.empty();
	  }
    break;

  case 87:
#line 1054 "sql_yacc.yy"
    {
             Lex->create_info.default_table_charset= NULL;
             Lex->create_info.used_fields= 0;
          }
    break;

  case 88:
#line 1059 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command=SQLCOM_CREATE_DB;
	    lex->name=yyvsp[-2].lex_str.str;
            lex->create_info.options=yyvsp[-3].num;
	  }
    break;

  case 89:
#line 1066 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command = SQLCOM_CREATE_FUNCTION;
	    lex->udf.name = yyvsp[0].lex_str;
	    lex->udf.type= yyvsp[-2].udf_type;
	  }
    break;

  case 90:
#line 1073 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->udf.returns=(Item_result) yyvsp[-2].num;
	    lex->udf.dl=yyvsp[0].lex_str.str;
	  }
    break;

  case 91:
#line 1081 "sql_yacc.yy"
    {}
    break;

  case 92:
#line 1082 "sql_yacc.yy"
    {}
    break;

  case 93:
#line 1084 "sql_yacc.yy"
    {
            LEX *lex=Lex;
            if (!(lex->name= (char *)yyvsp[0].table))
              YYABORT;
          }
    break;

  case 94:
#line 1090 "sql_yacc.yy"
    {
            LEX *lex=Lex;
            if (!(lex->name= (char *)yyvsp[-1].table))
              YYABORT;
          }
    break;

  case 95:
#line 1098 "sql_yacc.yy"
    {}
    break;

  case 96:
#line 1099 "sql_yacc.yy"
    { Select->set_braces(1);}
    break;

  case 97:
#line 1099 "sql_yacc.yy"
    {}
    break;

  case 98:
#line 1103 "sql_yacc.yy"
    {}
    break;

  case 99:
#line 1105 "sql_yacc.yy"
    { Select->set_braces(0);}
    break;

  case 100:
#line 1105 "sql_yacc.yy"
    {}
    break;

  case 101:
#line 1107 "sql_yacc.yy"
    { Select->set_braces(1);}
    break;

  case 102:
#line 1107 "sql_yacc.yy"
    {}
    break;

  case 103:
#line 1112 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->lock_option= using_update_log ? TL_READ_NO_INSERT : TL_READ;
	    if (lex->sql_command == SQLCOM_INSERT)
	      lex->sql_command= SQLCOM_INSERT_SELECT;
	    else if (lex->sql_command == SQLCOM_REPLACE)
	      lex->sql_command= SQLCOM_REPLACE_SELECT;
	    lex->current_select->table_list.save_and_clear(&lex->save_list);
	    mysql_init_select(lex);
	    lex->current_select->parsing_place= SELECT_LIST;
          }
    break;

  case 104:
#line 1124 "sql_yacc.yy"
    {
	    Select->parsing_place= NO_MATTER;
	  }
    break;

  case 105:
#line 1128 "sql_yacc.yy"
    { Lex->current_select->table_list.push_front(&Lex->save_list); }
    break;

  case 106:
#line 1132 "sql_yacc.yy"
    {}
    break;

  case 107:
#line 1133 "sql_yacc.yy"
    {}
    break;

  case 108:
#line 1136 "sql_yacc.yy"
    {}
    break;

  case 109:
#line 1137 "sql_yacc.yy"
    {}
    break;

  case 110:
#line 1140 "sql_yacc.yy"
    {}
    break;

  case 111:
#line 1141 "sql_yacc.yy"
    {}
    break;

  case 112:
#line 1144 "sql_yacc.yy"
    {}
    break;

  case 113:
#line 1145 "sql_yacc.yy"
    {}
    break;

  case 114:
#line 1148 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 115:
#line 1149 "sql_yacc.yy"
    { yyval.num= yyvsp[0].num;}
    break;

  case 116:
#line 1152 "sql_yacc.yy"
    { yyval.num=yyvsp[0].num; }
    break;

  case 117:
#line 1153 "sql_yacc.yy"
    { yyval.num= yyvsp[-1].num | yyvsp[0].num; }
    break;

  case 118:
#line 1156 "sql_yacc.yy"
    { yyval.num=HA_LEX_CREATE_TMP_TABLE; }
    break;

  case 119:
#line 1159 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 120:
#line 1160 "sql_yacc.yy"
    { yyval.num=HA_LEX_CREATE_IF_NOT_EXISTS; }
    break;

  case 128:
#line 1176 "sql_yacc.yy"
    { Lex->create_info.db_type= yyvsp[0].db_type; }
    break;

  case 129:
#line 1177 "sql_yacc.yy"
    { Lex->create_info.db_type= yyvsp[0].db_type; WARN_DEPRECATED("TYPE=storage_engine","ENGINE=storage_engine"); }
    break;

  case 130:
#line 1178 "sql_yacc.yy"
    { Lex->create_info.max_rows= yyvsp[0].ulonglong_number; Lex->create_info.used_fields|= HA_CREATE_USED_MAX_ROWS;}
    break;

  case 131:
#line 1179 "sql_yacc.yy"
    { Lex->create_info.min_rows= yyvsp[0].ulonglong_number; Lex->create_info.used_fields|= HA_CREATE_USED_MIN_ROWS;}
    break;

  case 132:
#line 1180 "sql_yacc.yy"
    { Lex->create_info.avg_row_length=yyvsp[0].ulong_num; Lex->create_info.used_fields|= HA_CREATE_USED_AVG_ROW_LENGTH;}
    break;

  case 133:
#line 1181 "sql_yacc.yy"
    { Lex->create_info.password=yyvsp[0].lex_str.str; }
    break;

  case 134:
#line 1182 "sql_yacc.yy"
    { Lex->create_info.comment=yyvsp[0].lex_str.str; }
    break;

  case 135:
#line 1183 "sql_yacc.yy"
    { Lex->create_info.auto_increment_value=yyvsp[0].ulonglong_number; Lex->create_info.used_fields|= HA_CREATE_USED_AUTO;}
    break;

  case 136:
#line 1184 "sql_yacc.yy"
    { Lex->create_info.table_options|= yyvsp[0].ulong_num ? HA_OPTION_PACK_KEYS : HA_OPTION_NO_PACK_KEYS; Lex->create_info.used_fields|= HA_CREATE_USED_PACK_KEYS;}
    break;

  case 137:
#line 1185 "sql_yacc.yy"
    { Lex->create_info.table_options&= ~(HA_OPTION_PACK_KEYS | HA_OPTION_NO_PACK_KEYS); Lex->create_info.used_fields|= HA_CREATE_USED_PACK_KEYS;}
    break;

  case 138:
#line 1186 "sql_yacc.yy"
    { Lex->create_info.table_options|= yyvsp[0].ulong_num ? HA_OPTION_CHECKSUM : HA_OPTION_NO_CHECKSUM; }
    break;

  case 139:
#line 1187 "sql_yacc.yy"
    { Lex->create_info.table_options|= yyvsp[0].ulong_num ? HA_OPTION_DELAY_KEY_WRITE : HA_OPTION_NO_DELAY_KEY_WRITE; }
    break;

  case 140:
#line 1188 "sql_yacc.yy"
    { Lex->create_info.row_type= yyvsp[0].row_type; }
    break;

  case 141:
#line 1189 "sql_yacc.yy"
    { Lex->create_info.raid_type= yyvsp[0].ulong_num; Lex->create_info.used_fields|= HA_CREATE_USED_RAID;}
    break;

  case 142:
#line 1190 "sql_yacc.yy"
    { Lex->create_info.raid_chunks= yyvsp[0].ulong_num; Lex->create_info.used_fields|= HA_CREATE_USED_RAID;}
    break;

  case 143:
#line 1191 "sql_yacc.yy"
    { Lex->create_info.raid_chunksize= yyvsp[0].ulong_num*RAID_BLOCK_SIZE; Lex->create_info.used_fields|= HA_CREATE_USED_RAID;}
    break;

  case 144:
#line 1193 "sql_yacc.yy"
    {
	    /* Move the union list to the merge_list */
	    LEX *lex=Lex;
	    TABLE_LIST *table_list= lex->select_lex.get_table_list();
	    lex->create_info.merge_list= lex->select_lex.table_list;
	    lex->create_info.merge_list.elements--;
	    lex->create_info.merge_list.first= (byte*) (table_list->next);
	    lex->select_lex.table_list.elements=1;
	    lex->select_lex.table_list.next= (byte**) &(table_list->next);
	    table_list->next=0;
	    lex->create_info.used_fields|= HA_CREATE_USED_UNION;
	  }
    break;

  case 147:
#line 1207 "sql_yacc.yy"
    { Lex->create_info.merge_insert_method= yyvsp[0].ulong_num; Lex->create_info.used_fields|= HA_CREATE_USED_INSERT_METHOD;}
    break;

  case 148:
#line 1209 "sql_yacc.yy"
    { Lex->create_info.data_file_name= yyvsp[0].lex_str.str; }
    break;

  case 149:
#line 1210 "sql_yacc.yy"
    { Lex->create_info.index_file_name= yyvsp[0].lex_str.str; }
    break;

  case 150:
#line 1214 "sql_yacc.yy"
    {
          HA_CREATE_INFO *cinfo= &Lex->create_info;
          if ((cinfo->used_fields & HA_CREATE_USED_DEFAULT_CHARSET) &&
               cinfo->default_table_charset && yyvsp[0].charset &&
               !my_charset_same(cinfo->default_table_charset,yyvsp[0].charset))
          {
            net_printf(YYTHD, ER_CONFLICTING_DECLARATIONS,
                       "CHARACTER SET ", cinfo->default_table_charset->csname,
                       "CHARACTER SET ", yyvsp[0].charset->csname);
            YYABORT;
          }
	  Lex->create_info.default_table_charset= yyvsp[0].charset;
          Lex->create_info.used_fields|= HA_CREATE_USED_DEFAULT_CHARSET;
        }
    break;

  case 151:
#line 1231 "sql_yacc.yy"
    {
          HA_CREATE_INFO *cinfo= &Lex->create_info;
          if ((cinfo->used_fields & HA_CREATE_USED_DEFAULT_CHARSET) &&
               cinfo->default_table_charset && yyvsp[0].charset &&
               !my_charset_same(cinfo->default_table_charset,yyvsp[0].charset))
            {
              net_printf(YYTHD,ER_COLLATION_CHARSET_MISMATCH,
                         yyvsp[0].charset->name, cinfo->default_table_charset->csname);
              YYABORT;
            }
            Lex->create_info.default_table_charset= yyvsp[0].charset;
            Lex->create_info.used_fields|= HA_CREATE_USED_DEFAULT_CHARSET;
        }
    break;

  case 152:
#line 1247 "sql_yacc.yy"
    {
	  yyval.db_type = ha_resolve_by_name(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length);
	  if (yyval.db_type == DB_TYPE_UNKNOWN) {
	    net_printf(YYTHD, ER_UNKNOWN_STORAGE_ENGINE, yyvsp[0].lex_str.str);
	    YYABORT;
	  }
	}
    break;

  case 153:
#line 1256 "sql_yacc.yy"
    { yyval.row_type= ROW_TYPE_DEFAULT; }
    break;

  case 154:
#line 1257 "sql_yacc.yy"
    { yyval.row_type= ROW_TYPE_FIXED; }
    break;

  case 155:
#line 1258 "sql_yacc.yy"
    { yyval.row_type= ROW_TYPE_DYNAMIC; }
    break;

  case 156:
#line 1259 "sql_yacc.yy"
    { yyval.row_type= ROW_TYPE_COMPRESSED; }
    break;

  case 157:
#line 1262 "sql_yacc.yy"
    { yyval.ulong_num= RAID_TYPE_0; }
    break;

  case 158:
#line 1263 "sql_yacc.yy"
    { yyval.ulong_num= RAID_TYPE_0; }
    break;

  case 159:
#line 1264 "sql_yacc.yy"
    { yyval.ulong_num=yyvsp[0].ulong_num;}
    break;

  case 160:
#line 1267 "sql_yacc.yy"
    { yyval.ulong_num= MERGE_INSERT_DISABLED; }
    break;

  case 161:
#line 1268 "sql_yacc.yy"
    { yyval.ulong_num= MERGE_INSERT_TO_FIRST; }
    break;

  case 162:
#line 1269 "sql_yacc.yy"
    { yyval.ulong_num= MERGE_INSERT_TO_LAST; }
    break;

  case 163:
#line 1272 "sql_yacc.yy"
    {}
    break;

  case 165:
#line 1276 "sql_yacc.yy"
    { yyval.udf_type = UDFTYPE_FUNCTION; }
    break;

  case 166:
#line 1277 "sql_yacc.yy"
    { yyval.udf_type = UDFTYPE_AGGREGATE; }
    break;

  case 167:
#line 1280 "sql_yacc.yy"
    {yyval.num = (int) STRING_RESULT; }
    break;

  case 168:
#line 1281 "sql_yacc.yy"
    {yyval.num = (int) REAL_RESULT; }
    break;

  case 169:
#line 1282 "sql_yacc.yy"
    {yyval.num = (int) INT_RESULT; }
    break;

  case 175:
#line 1297 "sql_yacc.yy"
    {
	    Lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 176:
#line 1304 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->key_list.push_back(new Key(yyvsp[-5].key_type,yyvsp[-4].simple_string, yyvsp[-3].key_alg, 0, lex->col_list));
	    lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 177:
#line 1310 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    const char *key_name= yyvsp[-4].simple_string ? yyvsp[-4].simple_string:yyvsp[-6].simple_string;
	    lex->key_list.push_back(new Key(yyvsp[-5].key_type, key_name, yyvsp[-3].key_alg, 0,
				    lex->col_list));
	    lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 178:
#line 1318 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->key_list.push_back(new foreign_key(yyvsp[-4].simple_string ? yyvsp[-4].simple_string:yyvsp[-7].simple_string, lex->col_list,
				    yyvsp[0].table,
				    lex->ref_list,
				    lex->fk_delete_opt,
				    lex->fk_update_opt,
				    lex->fk_match_option));
	    lex->key_list.push_back(new Key(Key::MULTIPLE, yyvsp[-4].simple_string ? yyvsp[-4].simple_string : yyvsp[-7].simple_string,
					    HA_KEY_ALG_UNDEF, 1,
					    lex->col_list));
	    lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 179:
#line 1332 "sql_yacc.yy"
    {
	    Lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 180:
#line 1336 "sql_yacc.yy"
    {
	    Lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 184:
#line 1351 "sql_yacc.yy"
    { yyval.simple_string=(char*) 0; }
    break;

  case 185:
#line 1352 "sql_yacc.yy"
    { yyval.simple_string= yyvsp[0].simple_string; }
    break;

  case 186:
#line 1356 "sql_yacc.yy"
    { yyval.simple_string=yyvsp[0].simple_string; }
    break;

  case 187:
#line 1361 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->length=lex->dec=0; lex->type=0;
	   lex->default_value= lex->on_update_value= 0;
	   lex->comment=0;
	   lex->charset=NULL;
	 }
    break;

  case 188:
#line 1369 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (add_field_to_list(lex->thd, yyvsp[-3].lex_str.str,
				(enum enum_field_types) yyvsp[-1].num,
				lex->length,lex->dec,lex->type,
				lex->default_value, lex->on_update_value, 
                                lex->comment,
				lex->change,&lex->interval_list,lex->charset,
				lex->uint_geom_type))
	    YYABORT;
	}
    break;

  case 189:
#line 1382 "sql_yacc.yy"
    { yyval.num=yyvsp[-2].num; }
    break;

  case 190:
#line 1383 "sql_yacc.yy"
    { yyval.num=yyvsp[-2].num; }
    break;

  case 191:
#line 1384 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_FLOAT; }
    break;

  case 192:
#line 1385 "sql_yacc.yy"
    { Lex->length=(char*) "1";
					  yyval.num=FIELD_TYPE_TINY; }
    break;

  case 193:
#line 1387 "sql_yacc.yy"
    { Lex->length=(char*) "1";
					  yyval.num=FIELD_TYPE_TINY; }
    break;

  case 194:
#line 1389 "sql_yacc.yy"
    { Lex->length=(char*) "1";
					  yyval.num=FIELD_TYPE_TINY; }
    break;

  case 195:
#line 1391 "sql_yacc.yy"
    { Lex->length=yyvsp[-2].lex_str.str;
					  yyval.num=FIELD_TYPE_STRING; }
    break;

  case 196:
#line 1393 "sql_yacc.yy"
    { Lex->length=(char*) "1";
					  yyval.num=FIELD_TYPE_STRING; }
    break;

  case 197:
#line 1395 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].lex_str.str;
					  yyval.num=FIELD_TYPE_STRING;
					  Lex->charset=national_charset_info; }
    break;

  case 198:
#line 1398 "sql_yacc.yy"
    { Lex->length=(char*) "1";
					  yyval.num=FIELD_TYPE_STRING;
					  Lex->charset=national_charset_info; }
    break;

  case 199:
#line 1401 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].lex_str.str;
					  Lex->charset=&my_charset_bin;
					  yyval.num=FIELD_TYPE_STRING; }
    break;

  case 200:
#line 1404 "sql_yacc.yy"
    { Lex->length= (char*) "1";
					  Lex->charset=&my_charset_bin;
					  yyval.num=FIELD_TYPE_STRING; }
    break;

  case 201:
#line 1407 "sql_yacc.yy"
    { Lex->length=yyvsp[-2].lex_str.str;
					  yyval.num=FIELD_TYPE_VAR_STRING; }
    break;

  case 202:
#line 1409 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].lex_str.str;
					  yyval.num=FIELD_TYPE_VAR_STRING;
					  Lex->charset=national_charset_info; }
    break;

  case 203:
#line 1412 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].lex_str.str;
					  Lex->charset=&my_charset_bin;
					  yyval.num=FIELD_TYPE_VAR_STRING; }
    break;

  case 204:
#line 1415 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_YEAR; }
    break;

  case 205:
#line 1416 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DATE; }
    break;

  case 206:
#line 1417 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_TIME; }
    break;

  case 207:
#line 1419 "sql_yacc.yy"
    {
	    if (YYTHD->variables.sql_mode & MODE_MAXDB)
	      yyval.num=FIELD_TYPE_DATETIME;
	    else
            {
              /* 
                Unlike other types TIMESTAMP fields are NOT NULL by default.
              */
              Lex->type|= NOT_NULL_FLAG;
	      yyval.num=FIELD_TYPE_TIMESTAMP;
            }
	   }
    break;

  case 208:
#line 1431 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DATETIME; }
    break;

  case 209:
#line 1432 "sql_yacc.yy"
    { Lex->charset=&my_charset_bin;
					  yyval.num=FIELD_TYPE_TINY_BLOB; }
    break;

  case 210:
#line 1434 "sql_yacc.yy"
    { Lex->charset=&my_charset_bin;
					  yyval.num=FIELD_TYPE_BLOB; }
    break;

  case 211:
#line 1436 "sql_yacc.yy"
    { 
#ifdef HAVE_SPATIAL
					  Lex->charset=&my_charset_bin;
					  Lex->uint_geom_type= (uint)yyvsp[0].num;
					  yyval.num=FIELD_TYPE_GEOMETRY;
#else
	                                  net_printf(Lex->thd, ER_FEATURE_DISABLED,
			                             sym_group_geom.name,
	                                             sym_group_geom.needed_define);
					  YYABORT;
#endif
					}
    break;

  case 212:
#line 1448 "sql_yacc.yy"
    { Lex->charset=&my_charset_bin;
					  yyval.num=FIELD_TYPE_MEDIUM_BLOB; }
    break;

  case 213:
#line 1450 "sql_yacc.yy"
    { Lex->charset=&my_charset_bin;
					  yyval.num=FIELD_TYPE_LONG_BLOB; }
    break;

  case 214:
#line 1452 "sql_yacc.yy"
    { Lex->charset=&my_charset_bin;
					  yyval.num=FIELD_TYPE_MEDIUM_BLOB; }
    break;

  case 215:
#line 1454 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_MEDIUM_BLOB; }
    break;

  case 216:
#line 1455 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_TINY_BLOB; }
    break;

  case 217:
#line 1456 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_BLOB; }
    break;

  case 218:
#line 1457 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_MEDIUM_BLOB; }
    break;

  case 219:
#line 1458 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_LONG_BLOB; }
    break;

  case 220:
#line 1460 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DECIMAL;}
    break;

  case 221:
#line 1462 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DECIMAL;}
    break;

  case 222:
#line 1464 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DECIMAL;}
    break;

  case 223:
#line 1465 "sql_yacc.yy"
    {Lex->interval_list.empty();}
    break;

  case 224:
#line 1466 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_ENUM; }
    break;

  case 225:
#line 1467 "sql_yacc.yy"
    { Lex->interval_list.empty();}
    break;

  case 226:
#line 1468 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_SET; }
    break;

  case 227:
#line 1469 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_MEDIUM_BLOB; }
    break;

  case 228:
#line 1471 "sql_yacc.yy"
    {
	    yyval.num=FIELD_TYPE_LONGLONG;
	    Lex->type|= (AUTO_INCREMENT_FLAG | NOT_NULL_FLAG | UNSIGNED_FLAG |
		         UNIQUE_FLAG);
	  }
    break;

  case 229:
#line 1479 "sql_yacc.yy"
    { yyval.num= Field::GEOM_GEOMETRY; }
    break;

  case 230:
#line 1480 "sql_yacc.yy"
    { yyval.num= Field::GEOM_GEOMETRYCOLLECTION; }
    break;

  case 231:
#line 1481 "sql_yacc.yy"
    { yyval.num= Field::GEOM_POINT; }
    break;

  case 232:
#line 1482 "sql_yacc.yy"
    { yyval.num= Field::GEOM_MULTIPOINT; }
    break;

  case 233:
#line 1483 "sql_yacc.yy"
    { yyval.num= Field::GEOM_LINESTRING; }
    break;

  case 234:
#line 1484 "sql_yacc.yy"
    { yyval.num= Field::GEOM_MULTILINESTRING; }
    break;

  case 235:
#line 1485 "sql_yacc.yy"
    { yyval.num= Field::GEOM_POLYGON; }
    break;

  case 236:
#line 1486 "sql_yacc.yy"
    { yyval.num= Field::GEOM_MULTIPOLYGON; }
    break;

  case 237:
#line 1490 "sql_yacc.yy"
    {}
    break;

  case 238:
#line 1494 "sql_yacc.yy"
    {}
    break;

  case 239:
#line 1495 "sql_yacc.yy"
    {}
    break;

  case 240:
#line 1499 "sql_yacc.yy"
    {}
    break;

  case 241:
#line 1500 "sql_yacc.yy"
    {}
    break;

  case 242:
#line 1504 "sql_yacc.yy"
    {}
    break;

  case 243:
#line 1505 "sql_yacc.yy"
    {}
    break;

  case 244:
#line 1506 "sql_yacc.yy"
    {}
    break;

  case 245:
#line 1507 "sql_yacc.yy"
    {}
    break;

  case 246:
#line 1508 "sql_yacc.yy"
    {}
    break;

  case 247:
#line 1512 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_LONG; }
    break;

  case 248:
#line 1513 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_TINY; }
    break;

  case 249:
#line 1514 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_SHORT; }
    break;

  case 250:
#line 1515 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_INT24; }
    break;

  case 251:
#line 1516 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_LONGLONG; }
    break;

  case 252:
#line 1519 "sql_yacc.yy"
    { yyval.num= YYTHD->variables.sql_mode & MODE_REAL_AS_FLOAT ?
			      FIELD_TYPE_FLOAT : FIELD_TYPE_DOUBLE; }
    break;

  case 253:
#line 1521 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DOUBLE; }
    break;

  case 254:
#line 1522 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DOUBLE; }
    break;

  case 255:
#line 1526 "sql_yacc.yy"
    {}
    break;

  case 256:
#line 1527 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].lex_str.str; }
    break;

  case 257:
#line 1528 "sql_yacc.yy"
    {}
    break;

  case 258:
#line 1532 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->length=yyvsp[-3].lex_str.str; lex->dec=yyvsp[-1].lex_str.str;
	}
    break;

  case 259:
#line 1538 "sql_yacc.yy"
    {}
    break;

  case 260:
#line 1539 "sql_yacc.yy"
    {}
    break;

  case 261:
#line 1542 "sql_yacc.yy"
    {}
    break;

  case 262:
#line 1543 "sql_yacc.yy"
    {}
    break;

  case 263:
#line 1546 "sql_yacc.yy"
    {}
    break;

  case 264:
#line 1547 "sql_yacc.yy"
    { Lex->type|= UNSIGNED_FLAG;}
    break;

  case 265:
#line 1548 "sql_yacc.yy"
    { Lex->type|= UNSIGNED_FLAG | ZEROFILL_FLAG; }
    break;

  case 266:
#line 1551 "sql_yacc.yy"
    { Lex->length=(char*) 0; }
    break;

  case 267:
#line 1552 "sql_yacc.yy"
    { Lex->length= yyvsp[-1].lex_str.str; }
    break;

  case 268:
#line 1555 "sql_yacc.yy"
    {}
    break;

  case 269:
#line 1556 "sql_yacc.yy"
    {}
    break;

  case 270:
#line 1559 "sql_yacc.yy"
    {}
    break;

  case 271:
#line 1560 "sql_yacc.yy"
    {}
    break;

  case 272:
#line 1563 "sql_yacc.yy"
    {}
    break;

  case 274:
#line 1567 "sql_yacc.yy"
    { Lex->type&= ~ NOT_NULL_FLAG; }
    break;

  case 275:
#line 1568 "sql_yacc.yy"
    { Lex->type|= NOT_NULL_FLAG; }
    break;

  case 276:
#line 1569 "sql_yacc.yy"
    { Lex->default_value=yyvsp[0].item; }
    break;

  case 277:
#line 1571 "sql_yacc.yy"
    { Lex->on_update_value= new Item_func_now_local(); }
    break;

  case 278:
#line 1572 "sql_yacc.yy"
    { Lex->type|= AUTO_INCREMENT_FLAG | NOT_NULL_FLAG; }
    break;

  case 279:
#line 1574 "sql_yacc.yy"
    { 
	    LEX *lex=Lex;
	    lex->type|= AUTO_INCREMENT_FLAG | NOT_NULL_FLAG | UNIQUE_FLAG; 
	    lex->alter_info.flags|= ALTER_ADD_INDEX; 
	  }
    break;

  case 280:
#line 1580 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->type|= PRI_KEY_FLAG | NOT_NULL_FLAG; 
	    lex->alter_info.flags|= ALTER_ADD_INDEX; 
	  }
    break;

  case 281:
#line 1586 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->type|= UNIQUE_FLAG; 
	    lex->alter_info.flags|= ALTER_ADD_INDEX; 
	  }
    break;

  case 282:
#line 1592 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->type|= UNIQUE_KEY_FLAG; 
	    lex->alter_info.flags|= ALTER_ADD_INDEX; 
	  }
    break;

  case 283:
#line 1597 "sql_yacc.yy"
    { Lex->comment= &yyvsp[0].lex_str; }
    break;

  case 284:
#line 1598 "sql_yacc.yy"
    { Lex->type|= BINCMP_FLAG; }
    break;

  case 285:
#line 1600 "sql_yacc.yy"
    {
	    if (Lex->charset && !my_charset_same(Lex->charset,yyvsp[0].charset))
	    {
	      net_printf(YYTHD,ER_COLLATION_CHARSET_MISMATCH,
			 yyvsp[0].charset->name,Lex->charset->csname);
	      YYABORT;
	    }
	    else
	    {
	      Lex->charset=yyvsp[0].charset;
	    }
	  }
    break;

  case 286:
#line 1615 "sql_yacc.yy"
    { yyval.item= new Item_func_now_local(); }
    break;

  case 287:
#line 1616 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 288:
#line 1620 "sql_yacc.yy"
    {}
    break;

  case 289:
#line 1621 "sql_yacc.yy"
    {}
    break;

  case 290:
#line 1626 "sql_yacc.yy"
    {
	  if (!(yyval.charset=get_charset_by_csname(yyvsp[0].lex_str.str,MY_CS_PRIMARY,MYF(0))))
	  {
	    net_printf(YYTHD,ER_UNKNOWN_CHARACTER_SET,yyvsp[0].lex_str.str);
	    YYABORT;
	  }
	}
    break;

  case 291:
#line 1633 "sql_yacc.yy"
    { yyval.charset= &my_charset_bin; }
    break;

  case 292:
#line 1637 "sql_yacc.yy"
    { yyval.charset=yyvsp[0].charset;   }
    break;

  case 293:
#line 1638 "sql_yacc.yy"
    { yyval.charset=NULL; }
    break;

  case 294:
#line 1643 "sql_yacc.yy"
    {
	  if (!(yyval.charset=get_charset_by_csname(yyvsp[0].lex_str.str,MY_CS_PRIMARY,MYF(0))) &&
	      !(yyval.charset=get_old_charset_by_name(yyvsp[0].lex_str.str)))
	  {
	    net_printf(YYTHD,ER_UNKNOWN_CHARACTER_SET,yyvsp[0].lex_str.str);
	    YYABORT;
	  }
	}
    break;

  case 295:
#line 1651 "sql_yacc.yy"
    { yyval.charset= &my_charset_bin; }
    break;

  case 296:
#line 1655 "sql_yacc.yy"
    { yyval.charset=yyvsp[0].charset;   }
    break;

  case 297:
#line 1656 "sql_yacc.yy"
    { yyval.charset=NULL; }
    break;

  case 298:
#line 1660 "sql_yacc.yy"
    {
	  if (!(yyval.charset=get_charset_by_name(yyvsp[0].lex_str.str,MYF(0))))
	  {
	    net_printf(YYTHD,ER_UNKNOWN_COLLATION,yyvsp[0].lex_str.str);
	    YYABORT;
	  }
	}
    break;

  case 299:
#line 1669 "sql_yacc.yy"
    { yyval.charset=NULL; }
    break;

  case 300:
#line 1670 "sql_yacc.yy"
    { yyval.charset=yyvsp[0].charset; }
    break;

  case 301:
#line 1674 "sql_yacc.yy"
    { yyval.charset=yyvsp[0].charset;   }
    break;

  case 302:
#line 1675 "sql_yacc.yy"
    { yyval.charset=NULL; }
    break;

  case 303:
#line 1678 "sql_yacc.yy"
    {}
    break;

  case 304:
#line 1679 "sql_yacc.yy"
    {}
    break;

  case 305:
#line 1682 "sql_yacc.yy"
    { Lex->charset=NULL; }
    break;

  case 306:
#line 1683 "sql_yacc.yy"
    { Lex->charset=&my_charset_latin1; }
    break;

  case 307:
#line 1684 "sql_yacc.yy"
    { Lex->charset=&my_charset_bin; }
    break;

  case 308:
#line 1686 "sql_yacc.yy"
    {
	  if (!(Lex->charset=get_charset_by_csname("ucs2",MY_CS_PRIMARY,MYF(0))))
	  {
	    net_printf(YYTHD,ER_UNKNOWN_CHARACTER_SET,"ucs2");
	    YYABORT;
	  }
	}
    break;

  case 309:
#line 1693 "sql_yacc.yy"
    { Lex->charset=yyvsp[0].charset; }
    break;

  case 312:
#line 1702 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->fk_delete_opt= lex->fk_update_opt= lex->fk_match_option= 0;
	  lex->ref_list.empty();
	}
    break;

  case 313:
#line 1708 "sql_yacc.yy"
    {
	  yyval.table=yyvsp[-2].table;
	}
    break;

  case 314:
#line 1713 "sql_yacc.yy"
    {}
    break;

  case 315:
#line 1714 "sql_yacc.yy"
    {}
    break;

  case 316:
#line 1717 "sql_yacc.yy"
    { Lex->ref_list.push_back(new key_part_spec(yyvsp[0].lex_str.str)); }
    break;

  case 317:
#line 1718 "sql_yacc.yy"
    { Lex->ref_list.push_back(new key_part_spec(yyvsp[0].lex_str.str)); }
    break;

  case 318:
#line 1722 "sql_yacc.yy"
    {}
    break;

  case 319:
#line 1723 "sql_yacc.yy"
    {}
    break;

  case 320:
#line 1726 "sql_yacc.yy"
    {}
    break;

  case 321:
#line 1727 "sql_yacc.yy"
    {}
    break;

  case 322:
#line 1730 "sql_yacc.yy"
    { Lex->fk_delete_opt= yyvsp[0].num; }
    break;

  case 323:
#line 1731 "sql_yacc.yy"
    { Lex->fk_update_opt= yyvsp[0].num; }
    break;

  case 324:
#line 1732 "sql_yacc.yy"
    { Lex->fk_match_option= foreign_key::FK_MATCH_FULL; }
    break;

  case 325:
#line 1733 "sql_yacc.yy"
    { Lex->fk_match_option= foreign_key::FK_MATCH_PARTIAL; }
    break;

  case 326:
#line 1734 "sql_yacc.yy"
    { Lex->fk_match_option= foreign_key::FK_MATCH_SIMPLE; }
    break;

  case 327:
#line 1737 "sql_yacc.yy"
    { yyval.num= (int) foreign_key::FK_OPTION_RESTRICT; }
    break;

  case 328:
#line 1738 "sql_yacc.yy"
    { yyval.num= (int) foreign_key::FK_OPTION_CASCADE; }
    break;

  case 329:
#line 1739 "sql_yacc.yy"
    { yyval.num= (int) foreign_key::FK_OPTION_SET_NULL; }
    break;

  case 330:
#line 1740 "sql_yacc.yy"
    { yyval.num= (int) foreign_key::FK_OPTION_NO_ACTION; }
    break;

  case 331:
#line 1741 "sql_yacc.yy"
    { yyval.num= (int) foreign_key::FK_OPTION_DEFAULT;  }
    break;

  case 332:
#line 1744 "sql_yacc.yy"
    { yyval.key_type= Key::MULTIPLE; }
    break;

  case 333:
#line 1745 "sql_yacc.yy"
    { yyval.key_type= Key::FULLTEXT; }
    break;

  case 334:
#line 1747 "sql_yacc.yy"
    {
#ifdef HAVE_SPATIAL
	    yyval.key_type= Key::SPATIAL;
#else
	    net_printf(Lex->thd, ER_FEATURE_DISABLED,
		       sym_group_geom.name, sym_group_geom.needed_define);
	    YYABORT;
#endif
	  }
    break;

  case 335:
#line 1758 "sql_yacc.yy"
    { yyval.key_type= Key::PRIMARY; }
    break;

  case 336:
#line 1759 "sql_yacc.yy"
    { yyval.key_type= Key::UNIQUE; }
    break;

  case 337:
#line 1762 "sql_yacc.yy"
    {}
    break;

  case 338:
#line 1763 "sql_yacc.yy"
    {}
    break;

  case 339:
#line 1766 "sql_yacc.yy"
    {}
    break;

  case 341:
#line 1771 "sql_yacc.yy"
    {}
    break;

  case 342:
#line 1772 "sql_yacc.yy"
    {}
    break;

  case 343:
#line 1773 "sql_yacc.yy"
    {}
    break;

  case 344:
#line 1776 "sql_yacc.yy"
    { yyval.key_type= Key::MULTIPLE; }
    break;

  case 345:
#line 1777 "sql_yacc.yy"
    { yyval.key_type= Key::UNIQUE; }
    break;

  case 346:
#line 1778 "sql_yacc.yy"
    { yyval.key_type= Key::FULLTEXT;}
    break;

  case 347:
#line 1780 "sql_yacc.yy"
    {
#ifdef HAVE_SPATIAL
	    yyval.key_type= Key::SPATIAL;
#else
	    net_printf(Lex->thd, ER_FEATURE_DISABLED,
	               sym_group_geom.name, sym_group_geom.needed_define);
	    YYABORT;
#endif
	  }
    break;

  case 348:
#line 1792 "sql_yacc.yy"
    { yyval.key_alg= HA_KEY_ALG_UNDEF; }
    break;

  case 349:
#line 1793 "sql_yacc.yy"
    { yyval.key_alg= yyvsp[0].key_alg; }
    break;

  case 350:
#line 1794 "sql_yacc.yy"
    { yyval.key_alg= yyvsp[0].key_alg; }
    break;

  case 351:
#line 1797 "sql_yacc.yy"
    { yyval.key_alg= HA_KEY_ALG_BTREE; }
    break;

  case 352:
#line 1799 "sql_yacc.yy"
    {
	    yyval.key_alg= HA_KEY_ALG_RTREE;
	  }
    break;

  case 353:
#line 1802 "sql_yacc.yy"
    { yyval.key_alg= HA_KEY_ALG_HASH; }
    break;

  case 354:
#line 1805 "sql_yacc.yy"
    { Lex->col_list.push_back(yyvsp[-1].key_part); }
    break;

  case 355:
#line 1806 "sql_yacc.yy"
    { Lex->col_list.push_back(yyvsp[-1].key_part); }
    break;

  case 356:
#line 1809 "sql_yacc.yy"
    { yyval.key_part=new key_part_spec(yyvsp[0].lex_str.str); }
    break;

  case 357:
#line 1811 "sql_yacc.yy"
    {
          int key_part_len= atoi(yyvsp[-1].lex_str.str);
#if MYSQL_VERSION_ID < 50000
          if (!key_part_len)
          {
            my_printf_error(ER_UNKNOWN_ERROR,
                            "Key part '%s' length cannot be 0",
                            MYF(0), yyvsp[-3].lex_str.str);
          }
#endif
          yyval.key_part=new key_part_spec(yyvsp[-3].lex_str.str,(uint) key_part_len);
        }
    break;

  case 358:
#line 1825 "sql_yacc.yy"
    { yyval.simple_string=(char*) 0; }
    break;

  case 359:
#line 1826 "sql_yacc.yy"
    { yyval.simple_string=yyvsp[0].lex_str.str; }
    break;

  case 360:
#line 1829 "sql_yacc.yy"
    { yyval.lex_str.str= 0; yyval.lex_str.length= 0; }
    break;

  case 361:
#line 1830 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 362:
#line 1833 "sql_yacc.yy"
    { Lex->interval_list.push_back(yyvsp[0].string); }
    break;

  case 363:
#line 1834 "sql_yacc.yy"
    { Lex->interval_list.push_back(yyvsp[0].string); }
    break;

  case 364:
#line 1842 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  LEX *lex= thd->lex;
	  lex->sql_command= SQLCOM_ALTER_TABLE;
	  lex->name= 0;
	  lex->duplicates= DUP_ERROR; 
	  if (!lex->select_lex.add_table_to_list(thd, yyvsp[0].table, NULL,
						 TL_OPTION_UPDATING))
	    YYABORT;
	  lex->create_list.empty();
	  lex->key_list.empty();
	  lex->col_list.empty();
          lex->select_lex.init_order();
	  lex->select_lex.db=lex->name=0;
	  bzero((char*) &lex->create_info,sizeof(lex->create_info));
	  lex->create_info.db_type= DB_TYPE_DEFAULT;
	  lex->create_info.default_table_charset= NULL;
	  lex->create_info.row_type= ROW_TYPE_NOT_USED;
	  lex->alter_info.reset();          
	  lex->alter_info.is_simple= 1;
	  lex->alter_info.flags= 0;
	}
    break;

  case 365:
#line 1865 "sql_yacc.yy"
    {}
    break;

  case 366:
#line 1867 "sql_yacc.yy"
    {
            Lex->create_info.default_table_charset= NULL;
            Lex->create_info.used_fields= 0;
          }
    break;

  case 367:
#line 1872 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command=SQLCOM_ALTER_DB;
	    lex->name= yyvsp[-2].simple_string;
	  }
    break;

  case 368:
#line 1880 "sql_yacc.yy"
    { yyval.simple_string= 0; }
    break;

  case 369:
#line 1881 "sql_yacc.yy"
    { yyval.simple_string= yyvsp[0].lex_str.str; }
    break;

  case 371:
#line 1885 "sql_yacc.yy"
    { Lex->alter_info.tablespace_op= DISCARD_TABLESPACE; }
    break;

  case 372:
#line 1886 "sql_yacc.yy"
    { Lex->alter_info.tablespace_op= IMPORT_TABLESPACE; }
    break;

  case 375:
#line 1892 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->change=0; 
	  lex->alter_info.flags|= ALTER_ADD_COLUMN; 
	}
    break;

  case 376:
#line 1899 "sql_yacc.yy"
    { Lex->alter_info.is_simple= 0; }
    break;

  case 377:
#line 1901 "sql_yacc.yy"
    { 
	    LEX *lex=Lex;
	    lex->alter_info.is_simple= 0; 
	    lex->alter_info.flags|= ALTER_ADD_INDEX; 
	  }
    break;

  case 378:
#line 1906 "sql_yacc.yy"
    { Lex->alter_info.is_simple= 0; }
    break;

  case 379:
#line 1908 "sql_yacc.yy"
    {
	     LEX *lex=Lex;
	     lex->change= yyvsp[0].lex_str.str; 
	     lex->alter_info.is_simple= 0;
	     lex->alter_info.flags|= ALTER_CHANGE_COLUMN;
	  }
    break;

  case 381:
#line 1916 "sql_yacc.yy"
    {
            LEX *lex=Lex;
            lex->length=lex->dec=0; lex->type=0;
            lex->default_value= lex->on_update_value= 0;
	    lex->comment=0;
	    lex->charset= NULL;
            lex->alter_info.is_simple= 0;
	    lex->alter_info.flags|= ALTER_CHANGE_COLUMN;
          }
    break;

  case 382:
#line 1926 "sql_yacc.yy"
    {
            LEX *lex=Lex;
            if (add_field_to_list(lex->thd,yyvsp[-3].lex_str.str,
                                  (enum enum_field_types) yyvsp[-1].num,
                                  lex->length,lex->dec,lex->type,
                                  lex->default_value, lex->on_update_value,
                                  lex->comment,
				  yyvsp[-3].lex_str.str, &lex->interval_list, lex->charset,
				  lex->uint_geom_type))
	       YYABORT;
          }
    break;

  case 384:
#line 1939 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->alter_info.drop_list.push_back(new Alter_drop(Alter_drop::COLUMN,
	    			                               yyvsp[-1].lex_str.str)); 
	    lex->alter_info.is_simple= 0;
	    lex->alter_info.flags|= ALTER_DROP_COLUMN;
	  }
    break;

  case 385:
#line 1946 "sql_yacc.yy"
    { Lex->alter_info.is_simple= 0; }
    break;

  case 386:
#line 1948 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->alter_info.drop_list.push_back(new Alter_drop(Alter_drop::KEY,
				               primary_key_name));
	    lex->alter_info.is_simple= 0;
	    lex->alter_info.flags|= ALTER_DROP_INDEX;
	  }
    break;

  case 387:
#line 1956 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->alter_info.drop_list.push_back(new Alter_drop(Alter_drop::KEY,
					                       yyvsp[0].lex_str.str));
	    lex->alter_info.is_simple= 0;
	    lex->alter_info.flags|= ALTER_DROP_INDEX;
	  }
    break;

  case 388:
#line 1963 "sql_yacc.yy"
    { Lex->alter_info.keys_onoff= DISABLE; }
    break;

  case 389:
#line 1964 "sql_yacc.yy"
    { Lex->alter_info.keys_onoff= ENABLE; }
    break;

  case 390:
#line 1966 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->alter_info.alter_list.push_back(new Alter_column(yyvsp[-3].lex_str.str,yyvsp[0].item));
	    lex->alter_info.is_simple= 0;
	    lex->alter_info.flags|= ALTER_CHANGE_COLUMN;
	  }
    break;

  case 391:
#line 1973 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->alter_info.alter_list.push_back(new Alter_column(yyvsp[-2].lex_str.str,
                                                                  (Item*) 0));
	    lex->alter_info.is_simple= 0;
	    lex->alter_info.flags|= ALTER_CHANGE_COLUMN;
	  }
    break;

  case 392:
#line 1981 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->select_lex.db=yyvsp[0].table->db.str;
	    lex->name= yyvsp[0].table->table.str;
            if (check_table_name(yyvsp[0].table->table.str,yyvsp[0].table->table.length) ||
                yyvsp[0].table->db.str && check_db_name(yyvsp[0].table->db.str))
            {
              net_printf(lex->thd,ER_WRONG_TABLE_NAME,yyvsp[0].table->table.str);
              YYABORT;
            }
	    lex->alter_info.flags|= ALTER_RENAME;
	  }
    break;

  case 393:
#line 1994 "sql_yacc.yy"
    {
	    if (!yyvsp[-1].charset)
	    {
	      THD *thd= YYTHD;
	      yyvsp[-1].charset= thd->variables.collation_database;
	    }
	    yyvsp[0].charset= yyvsp[0].charset ? yyvsp[0].charset : yyvsp[-1].charset;
	    if (!my_charset_same(yyvsp[-1].charset,yyvsp[0].charset))
	    {
	      net_printf(YYTHD,ER_COLLATION_CHARSET_MISMATCH,
			 yyvsp[0].charset->name,yyvsp[-1].charset->csname);
	      YYABORT;
	    }
	    LEX *lex= Lex;
	    lex->create_info.table_charset= 
	      lex->create_info.default_table_charset= yyvsp[0].charset;
	    lex->create_info.used_fields|= (HA_CREATE_USED_CHARSET |
					    HA_CREATE_USED_DEFAULT_CHARSET);
	    lex->alter_info.is_simple= 0;
	  }
    break;

  case 394:
#line 2015 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->alter_info.is_simple= 0; 
	    lex->alter_info.flags|= ALTER_OPTIONS;
	  }
    break;

  case 395:
#line 2021 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->alter_info.is_simple= 0; 
	    lex->alter_info.flags|= ALTER_ORDER;
	  }
    break;

  case 396:
#line 2028 "sql_yacc.yy"
    {}
    break;

  case 397:
#line 2029 "sql_yacc.yy"
    {}
    break;

  case 398:
#line 2032 "sql_yacc.yy"
    { Lex->ignore= 0;}
    break;

  case 399:
#line 2033 "sql_yacc.yy"
    { Lex->ignore= 1;}
    break;

  case 400:
#line 2037 "sql_yacc.yy"
    {}
    break;

  case 401:
#line 2038 "sql_yacc.yy"
    {}
    break;

  case 402:
#line 2039 "sql_yacc.yy"
    {}
    break;

  case 403:
#line 2042 "sql_yacc.yy"
    {}
    break;

  case 404:
#line 2043 "sql_yacc.yy"
    { store_position_for_column(yyvsp[0].lex_str.str); }
    break;

  case 405:
#line 2044 "sql_yacc.yy"
    { store_position_for_column(first_keyword); }
    break;

  case 406:
#line 2047 "sql_yacc.yy"
    {}
    break;

  case 407:
#line 2048 "sql_yacc.yy"
    {}
    break;

  case 408:
#line 2049 "sql_yacc.yy"
    {}
    break;

  case 409:
#line 2050 "sql_yacc.yy"
    {}
    break;

  case 410:
#line 2058 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
            lex->sql_command = SQLCOM_SLAVE_START;
	    lex->type = 0;
	    /* We'll use mi structure for UNTIL options */
	    bzero((char*) &lex->mi, sizeof(lex->mi));
            /* If you change this code don't forget to update SLAVE START too */
          }
    break;

  case 411:
#line 2067 "sql_yacc.yy"
    {}
    break;

  case 412:
#line 2069 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
            lex->sql_command = SQLCOM_SLAVE_STOP;
	    lex->type = 0;
            /* If you change this code don't forget to update SLAVE STOP too */
          }
    break;

  case 413:
#line 2076 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
           lex->sql_command = SQLCOM_SLAVE_START;
	   lex->type = 0;
	    /* We'll use mi structure for UNTIL options */
	    bzero((char*) &lex->mi, sizeof(lex->mi));
          }
    break;

  case 414:
#line 2084 "sql_yacc.yy"
    {}
    break;

  case 415:
#line 2086 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
           lex->sql_command = SQLCOM_SLAVE_STOP;
	   lex->type = 0;
         }
    break;

  case 416:
#line 2096 "sql_yacc.yy"
    {
           Lex->sql_command = SQLCOM_BEGIN;
           Lex->start_transaction_opt= yyvsp[0].num;
        }
    break;

  case 417:
#line 2103 "sql_yacc.yy"
    { yyval.num = 0; }
    break;

  case 418:
#line 2105 "sql_yacc.yy"
    {
           yyval.num= MYSQL_START_TRANS_OPT_WITH_CONS_SNAPSHOT;
        }
    break;

  case 419:
#line 2111 "sql_yacc.yy"
    { Lex->slave_thd_opt= 0; }
    break;

  case 420:
#line 2113 "sql_yacc.yy"
    {}
    break;

  case 423:
#line 2122 "sql_yacc.yy"
    {}
    break;

  case 424:
#line 2123 "sql_yacc.yy"
    { Lex->slave_thd_opt|=SLAVE_SQL; }
    break;

  case 425:
#line 2124 "sql_yacc.yy"
    { Lex->slave_thd_opt|=SLAVE_IO; }
    break;

  case 426:
#line 2128 "sql_yacc.yy"
    {}
    break;

  case 427:
#line 2130 "sql_yacc.yy"
    {
            LEX *lex=Lex;
            if ((lex->mi.log_file_name || lex->mi.pos) &&
                (lex->mi.relay_log_name || lex->mi.relay_log_pos) ||
                !((lex->mi.log_file_name && lex->mi.pos) ||
                  (lex->mi.relay_log_name && lex->mi.relay_log_pos)))
            {
               send_error(lex->thd, ER_BAD_SLAVE_UNTIL_COND);
               YYABORT;
            }

          }
    break;

  case 430:
#line 2151 "sql_yacc.yy"
    {
	   Lex->sql_command = SQLCOM_RESTORE_TABLE;
	}
    break;

  case 431:
#line 2155 "sql_yacc.yy"
    {
	  Lex->backup_dir = yyvsp[0].lex_str.str;
        }
    break;

  case 432:
#line 2161 "sql_yacc.yy"
    {
	   Lex->sql_command = SQLCOM_BACKUP_TABLE;
	}
    break;

  case 433:
#line 2165 "sql_yacc.yy"
    {
	  Lex->backup_dir = yyvsp[0].lex_str.str;
        }
    break;

  case 434:
#line 2171 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->sql_command = SQLCOM_CHECKSUM;
	}
    break;

  case 435:
#line 2176 "sql_yacc.yy"
    {}
    break;

  case 436:
#line 2180 "sql_yacc.yy"
    { Lex->check_opt.flags= 0; }
    break;

  case 437:
#line 2181 "sql_yacc.yy"
    { Lex->check_opt.flags= T_QUICK; }
    break;

  case 438:
#line 2182 "sql_yacc.yy"
    { Lex->check_opt.flags= T_EXTEND; }
    break;

  case 439:
#line 2187 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->sql_command = SQLCOM_REPAIR;
           lex->no_write_to_binlog= yyvsp[-1].num;
	   lex->check_opt.init();
	}
    break;

  case 440:
#line 2194 "sql_yacc.yy"
    {}
    break;

  case 441:
#line 2198 "sql_yacc.yy"
    { Lex->check_opt.flags = T_MEDIUM; }
    break;

  case 442:
#line 2199 "sql_yacc.yy"
    {}
    break;

  case 443:
#line 2202 "sql_yacc.yy"
    {}
    break;

  case 444:
#line 2203 "sql_yacc.yy"
    {}
    break;

  case 445:
#line 2206 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_QUICK; }
    break;

  case 446:
#line 2207 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_EXTEND; }
    break;

  case 447:
#line 2208 "sql_yacc.yy"
    { Lex->check_opt.sql_flags|= TT_USEFRM; }
    break;

  case 448:
#line 2212 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->sql_command = SQLCOM_ANALYZE;
           lex->no_write_to_binlog= yyvsp[-1].num;
	   lex->check_opt.init();
	}
    break;

  case 449:
#line 2219 "sql_yacc.yy"
    {}
    break;

  case 450:
#line 2224 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->sql_command = SQLCOM_CHECK;
	   lex->check_opt.init();
	}
    break;

  case 451:
#line 2230 "sql_yacc.yy"
    {}
    break;

  case 452:
#line 2234 "sql_yacc.yy"
    { Lex->check_opt.flags = T_MEDIUM; }
    break;

  case 453:
#line 2235 "sql_yacc.yy"
    {}
    break;

  case 454:
#line 2238 "sql_yacc.yy"
    {}
    break;

  case 455:
#line 2239 "sql_yacc.yy"
    {}
    break;

  case 456:
#line 2242 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_QUICK; }
    break;

  case 457:
#line 2243 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_FAST; }
    break;

  case 458:
#line 2244 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_MEDIUM; }
    break;

  case 459:
#line 2245 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_EXTEND; }
    break;

  case 460:
#line 2246 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_CHECK_ONLY_CHANGED; }
    break;

  case 461:
#line 2250 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->sql_command = SQLCOM_OPTIMIZE;
           lex->no_write_to_binlog= yyvsp[-1].num;
	   lex->check_opt.init();
	}
    break;

  case 462:
#line 2257 "sql_yacc.yy"
    {}
    break;

  case 463:
#line 2261 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 464:
#line 2262 "sql_yacc.yy"
    { yyval.num= 1; }
    break;

  case 465:
#line 2263 "sql_yacc.yy"
    { yyval.num= 1; }
    break;

  case 466:
#line 2268 "sql_yacc.yy"
    {
	   Lex->sql_command=SQLCOM_RENAME_TABLE;
	}
    break;

  case 467:
#line 2272 "sql_yacc.yy"
    {}
    break;

  case 470:
#line 2281 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  SELECT_LEX *sl= lex->current_select;
	  if (!sl->add_table_to_list(lex->thd, yyvsp[-2].table,NULL,TL_OPTION_UPDATING,
				     TL_IGNORE) ||
	      !sl->add_table_to_list(lex->thd, yyvsp[0].table,NULL,TL_OPTION_UPDATING,
				     TL_IGNORE))
	    YYABORT;
	}
    break;

  case 471:
#line 2293 "sql_yacc.yy"
    {
          LEX *lex=Lex;
          lex->sql_command= SQLCOM_ASSIGN_TO_KEYCACHE;
	  lex->name_and_length= yyvsp[0].lex_str;
        }
    break;

  case 474:
#line 2306 "sql_yacc.yy"
    {
          LEX *lex=Lex;
          SELECT_LEX *sel= &lex->select_lex;
          if (!sel->add_table_to_list(lex->thd, yyvsp[-1].table, NULL, 0,
                                      TL_READ,
                                      sel->get_use_index(),
                                      (List<String> *)0))
            YYABORT;
        }
    break;

  case 475:
#line 2318 "sql_yacc.yy"
    { yyval.lex_str= yyvsp[0].lex_str; }
    break;

  case 476:
#line 2319 "sql_yacc.yy"
    { yyval.lex_str = default_key_cache_base; }
    break;

  case 477:
#line 2324 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command=SQLCOM_PRELOAD_KEYS;
	}
    break;

  case 478:
#line 2329 "sql_yacc.yy"
    {}
    break;

  case 481:
#line 2338 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  SELECT_LEX *sel= &lex->select_lex;
	  if (!sel->add_table_to_list(lex->thd, yyvsp[-2].table, NULL, yyvsp[0].num,
                                      TL_READ,
                                      sel->get_use_index(),
                                      (List<String> *)0))
            YYABORT;
	}
    break;

  case 482:
#line 2350 "sql_yacc.yy"
    { Select->interval_list.empty(); }
    break;

  case 483:
#line 2352 "sql_yacc.yy"
    {
          LEX *lex=Lex;
          SELECT_LEX *sel= &lex->select_lex;
          sel->use_index= sel->interval_list;
        }
    break;

  case 484:
#line 2360 "sql_yacc.yy"
    { Lex->select_lex.use_index_ptr= 0; }
    break;

  case 485:
#line 2362 "sql_yacc.yy"
    {
            SELECT_LEX *sel= &Lex->select_lex;
	    sel->use_index_ptr= &sel->use_index;
	  }
    break;

  case 486:
#line 2370 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 487:
#line 2371 "sql_yacc.yy"
    { yyval.num= TL_OPTION_IGNORE_LEAVES; }
    break;

  case 488:
#line 2381 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  lex->sql_command= SQLCOM_SELECT;
	  lex->select_lex.resolve_mode= SELECT_LEX::SELECT_MODE;
	}
    break;

  case 491:
#line 2396 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
            SELECT_LEX * sel= lex->current_select;
	    if (sel->set_braces(1))
	    {
	      yyerror(ER(ER_SYNTAX_ERROR));
	      YYABORT;
	    }
	  if (sel->linkage == UNION_TYPE &&
	      !sel->master_unit()->first_select()->braces)
	  {
	    yyerror(ER(ER_SYNTAX_ERROR));
	    YYABORT;
	  }
            /* select in braces, can't contain global parameters */
	    if (sel->master_unit()->fake_select_lex)
              sel->master_unit()->global_parameters=
                 sel->master_unit()->fake_select_lex;
          }
    break;

  case 493:
#line 2419 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
          SELECT_LEX * sel= lex->current_select;
          if (lex->current_select->set_braces(0))
	  {
	    yyerror(ER(ER_SYNTAX_ERROR));
	    YYABORT;
	  }
	  if (sel->linkage == UNION_TYPE &&
	      sel->master_unit()->first_select()->braces)
	  {
	    yyerror(ER(ER_SYNTAX_ERROR));
	    YYABORT;
	  }
	}
    break;

  case 495:
#line 2438 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  SELECT_LEX *sel= lex->current_select;
	  if (sel->linkage != UNION_TYPE)
	    mysql_init_select(lex);
	  lex->current_select->parsing_place= SELECT_LIST;
	}
    break;

  case 496:
#line 2446 "sql_yacc.yy"
    {
	  Select->parsing_place= NO_MATTER;
	}
    break;

  case 498:
#line 2452 "sql_yacc.yy"
    {}
    break;

  case 506:
#line 2471 "sql_yacc.yy"
    {
	    if (test_all_bits(Select->options, SELECT_ALL | SELECT_DISTINCT))
	    {
	      net_printf(Lex->thd, ER_WRONG_USAGE, "ALL", "DISTINCT");
              YYABORT;
	    }
          }
    break;

  case 509:
#line 2485 "sql_yacc.yy"
    { Select->options|= SELECT_STRAIGHT_JOIN; }
    break;

  case 510:
#line 2487 "sql_yacc.yy"
    {
	    if (check_simple_select())
	      YYABORT;
	    Lex->lock_option= TL_READ_HIGH_PRIORITY;
	  }
    break;

  case 511:
#line 2492 "sql_yacc.yy"
    { Select->options|= SELECT_DISTINCT; }
    break;

  case 512:
#line 2493 "sql_yacc.yy"
    { Select->options|= SELECT_SMALL_RESULT; }
    break;

  case 513:
#line 2494 "sql_yacc.yy"
    { Select->options|= SELECT_BIG_RESULT; }
    break;

  case 514:
#line 2496 "sql_yacc.yy"
    {
	    if (check_simple_select())
	      YYABORT;
	    Select->options|= OPTION_BUFFER_RESULT;
	  }
    break;

  case 515:
#line 2502 "sql_yacc.yy"
    {
	    if (check_simple_select())
	      YYABORT;
	    Select->options|= OPTION_FOUND_ROWS;
	  }
    break;

  case 516:
#line 2507 "sql_yacc.yy"
    { Lex->safe_to_cache_query=0; }
    break;

  case 517:
#line 2509 "sql_yacc.yy"
    {
	    Lex->select_lex.options|= OPTION_TO_QUERY_CACHE;
	  }
    break;

  case 518:
#line 2512 "sql_yacc.yy"
    { Select->options|= SELECT_ALL; }
    break;

  case 520:
#line 2518 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->current_select->set_lock_for_tables(TL_WRITE);
	    lex->safe_to_cache_query=0;
	  }
    break;

  case 521:
#line 2524 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->current_select->
	      set_lock_for_tables(TL_READ_WITH_SHARED_LOCKS);
	    lex->safe_to_cache_query=0;
	  }
    break;

  case 524:
#line 2536 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    if (add_item_to_list(thd, new Item_field(NULL, NULL, "*")))
	      YYABORT;
	    (thd->lex->current_select->with_wild)++;
	  }
    break;

  case 525:
#line 2546 "sql_yacc.yy"
    {
	    if (add_item_to_list(YYTHD, yyvsp[-2].item))
	      YYABORT;
	    if (yyvsp[0].lex_str.str)
	      yyvsp[-2].item->set_name(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,system_charset_info);
	    else if (!yyvsp[-2].item->name)
	      yyvsp[-2].item->set_name(yyvsp[-3].simple_string,(uint) (yyvsp[-1].simple_string - yyvsp[-3].simple_string), YYTHD->charset());
	  }
    break;

  case 526:
#line 2556 "sql_yacc.yy"
    { yyval.simple_string=(char*) Lex->tok_start; }
    break;

  case 527:
#line 2559 "sql_yacc.yy"
    { yyval.simple_string=(char*) Lex->tok_end; }
    break;

  case 528:
#line 2562 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 529:
#line 2563 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 530:
#line 2566 "sql_yacc.yy"
    { yyval.lex_str.str=0;}
    break;

  case 531:
#line 2567 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 532:
#line 2568 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 533:
#line 2569 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 534:
#line 2570 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 535:
#line 2574 "sql_yacc.yy"
    {}
    break;

  case 536:
#line 2575 "sql_yacc.yy"
    {}
    break;

  case 537:
#line 2579 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 538:
#line 2580 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 539:
#line 2583 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_eq_creator; }
    break;

  case 540:
#line 2584 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_ge_creator; }
    break;

  case 541:
#line 2585 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_gt_creator; }
    break;

  case 542:
#line 2586 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_le_creator; }
    break;

  case 543:
#line 2587 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_lt_creator; }
    break;

  case 544:
#line 2588 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_ne_creator; }
    break;

  case 545:
#line 2591 "sql_yacc.yy"
    { yyval.num = 1; }
    break;

  case 546:
#line 2592 "sql_yacc.yy"
    { yyval.num = 0; }
    break;

  case 547:
#line 2598 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-4].item); yyval.item= new Item_func_in(*yyvsp[-1].item_list); }
    break;

  case 548:
#line 2600 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-5].item); yyval.item= new Item_func_not(new Item_func_in(*yyvsp[-1].item_list)); }
    break;

  case 549:
#line 2602 "sql_yacc.yy"
    { yyval.item= new Item_in_subselect(yyvsp[-2].item, yyvsp[0].select_lex); }
    break;

  case 550:
#line 2604 "sql_yacc.yy"
    {
            yyval.item= new Item_func_not(new Item_in_subselect(yyvsp[-3].item, yyvsp[0].select_lex));
          }
    break;

  case 551:
#line 2608 "sql_yacc.yy"
    { yyval.item= new Item_func_between(yyvsp[-4].item,yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 552:
#line 2610 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_between(yyvsp[-5].item,yyvsp[-2].item,yyvsp[0].item)); }
    break;

  case 553:
#line 2611 "sql_yacc.yy"
    { yyval.item= or_or_concat(YYTHD, yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 554:
#line 2612 "sql_yacc.yy"
    { yyval.item= new Item_cond_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 555:
#line 2613 "sql_yacc.yy"
    { yyval.item= new Item_cond_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 556:
#line 2614 "sql_yacc.yy"
    { yyval.item= new Item_cond_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 557:
#line 2616 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_eq(new Item_func_soundex(yyvsp[-3].item),
				 new Item_func_soundex(yyvsp[0].item));
	  }
    break;

  case 558:
#line 2621 "sql_yacc.yy"
    { yyval.item= new Item_func_like(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].item); }
    break;

  case 559:
#line 2623 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_like(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].item));}
    break;

  case 560:
#line 2624 "sql_yacc.yy"
    { yyval.item= new Item_func_regex(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 561:
#line 2626 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_regex(yyvsp[-3].item,yyvsp[0].item)); }
    break;

  case 562:
#line 2627 "sql_yacc.yy"
    { yyval.item= new Item_func_isnull(yyvsp[-2].item); }
    break;

  case 563:
#line 2628 "sql_yacc.yy"
    { yyval.item= new Item_func_isnotnull(yyvsp[-3].item); }
    break;

  case 564:
#line 2629 "sql_yacc.yy"
    { yyval.item= new Item_func_equal(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 565:
#line 2630 "sql_yacc.yy"
    { yyval.item= (*yyvsp[-1].boolfunc2creator)(0)->create(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 566:
#line 2632 "sql_yacc.yy"
    {
	  yyval.item= all_any_subquery_creator(yyvsp[-3].item, yyvsp[-2].boolfunc2creator, yyvsp[-1].num, yyvsp[0].select_lex);
	}
    break;

  case 567:
#line 2635 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_left(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 568:
#line 2636 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_right(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 569:
#line 2637 "sql_yacc.yy"
    { yyval.item= new Item_func_plus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 570:
#line 2638 "sql_yacc.yy"
    { yyval.item= new Item_func_minus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 571:
#line 2639 "sql_yacc.yy"
    { yyval.item= new Item_func_mul(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 572:
#line 2640 "sql_yacc.yy"
    { yyval.item= new Item_func_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 573:
#line 2641 "sql_yacc.yy"
    { yyval.item= new Item_func_int_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 574:
#line 2642 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 575:
#line 2643 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 576:
#line 2644 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 577:
#line 2645 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 578:
#line 2646 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 579:
#line 2648 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,0); }
    break;

  case 580:
#line 2650 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,1); }
    break;

  case 581:
#line 2656 "sql_yacc.yy"
    { yyval.item= new Item_func_between(yyvsp[-4].item,yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 582:
#line 2658 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_between(yyvsp[-5].item,yyvsp[-2].item,yyvsp[0].item)); }
    break;

  case 583:
#line 2659 "sql_yacc.yy"
    { yyval.item= or_or_concat(YYTHD, yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 584:
#line 2660 "sql_yacc.yy"
    { yyval.item= new Item_cond_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 585:
#line 2661 "sql_yacc.yy"
    { yyval.item= new Item_cond_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 586:
#line 2662 "sql_yacc.yy"
    { yyval.item= new Item_cond_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 587:
#line 2664 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_eq(new Item_func_soundex(yyvsp[-3].item),
				 new Item_func_soundex(yyvsp[0].item));
	  }
    break;

  case 588:
#line 2669 "sql_yacc.yy"
    { yyval.item= new Item_func_like(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].item); }
    break;

  case 589:
#line 2671 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_like(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].item)); }
    break;

  case 590:
#line 2672 "sql_yacc.yy"
    { yyval.item= new Item_func_regex(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 591:
#line 2674 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_regex(yyvsp[-3].item,yyvsp[0].item)); }
    break;

  case 592:
#line 2675 "sql_yacc.yy"
    { yyval.item= new Item_func_isnull(yyvsp[-2].item); }
    break;

  case 593:
#line 2676 "sql_yacc.yy"
    { yyval.item= new Item_func_isnotnull(yyvsp[-3].item); }
    break;

  case 594:
#line 2677 "sql_yacc.yy"
    { yyval.item= new Item_func_equal(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 595:
#line 2678 "sql_yacc.yy"
    { yyval.item= (*yyvsp[-1].boolfunc2creator)(0)->create(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 596:
#line 2680 "sql_yacc.yy"
    {
	  all_any_subquery_creator(yyvsp[-3].item, yyvsp[-2].boolfunc2creator, yyvsp[-1].num, yyvsp[0].select_lex);
	}
    break;

  case 597:
#line 2683 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_left(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 598:
#line 2684 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_right(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 599:
#line 2685 "sql_yacc.yy"
    { yyval.item= new Item_func_plus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 600:
#line 2686 "sql_yacc.yy"
    { yyval.item= new Item_func_minus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 601:
#line 2687 "sql_yacc.yy"
    { yyval.item= new Item_func_mul(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 602:
#line 2688 "sql_yacc.yy"
    { yyval.item= new Item_func_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 603:
#line 2689 "sql_yacc.yy"
    { yyval.item= new Item_func_int_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 604:
#line 2690 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 605:
#line 2691 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 606:
#line 2692 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 607:
#line 2693 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 608:
#line 2694 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 609:
#line 2696 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,0); }
    break;

  case 610:
#line 2698 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,1); }
    break;

  case 612:
#line 2704 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-4].item); yyval.item= new Item_func_in(*yyvsp[-1].item_list); }
    break;

  case 613:
#line 2706 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-5].item); yyval.item= new Item_func_not(new Item_func_in(*yyvsp[-1].item_list)); }
    break;

  case 614:
#line 2708 "sql_yacc.yy"
    { yyval.item= new Item_in_subselect(yyvsp[-2].item, yyvsp[0].select_lex); }
    break;

  case 615:
#line 2710 "sql_yacc.yy"
    {
            yyval.item= new Item_func_not(new Item_in_subselect(yyvsp[-3].item, yyvsp[0].select_lex));
          }
    break;

  case 616:
#line 2714 "sql_yacc.yy"
    { yyval.item= new Item_func_between(yyvsp[-4].item,yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 617:
#line 2716 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_between(yyvsp[-5].item,yyvsp[-2].item,yyvsp[0].item)); }
    break;

  case 618:
#line 2717 "sql_yacc.yy"
    { yyval.item= or_or_concat(YYTHD, yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 619:
#line 2718 "sql_yacc.yy"
    { yyval.item= new Item_cond_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 620:
#line 2719 "sql_yacc.yy"
    { yyval.item= new Item_cond_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 621:
#line 2721 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_eq(new Item_func_soundex(yyvsp[-3].item),
				 new Item_func_soundex(yyvsp[0].item));
	  }
    break;

  case 622:
#line 2726 "sql_yacc.yy"
    { yyval.item= new Item_func_like(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].item); }
    break;

  case 623:
#line 2728 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_like(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].item)); }
    break;

  case 624:
#line 2729 "sql_yacc.yy"
    { yyval.item= new Item_func_regex(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 625:
#line 2731 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_regex(yyvsp[-3].item,yyvsp[0].item)); }
    break;

  case 626:
#line 2732 "sql_yacc.yy"
    { yyval.item= new Item_func_isnull(yyvsp[-2].item); }
    break;

  case 627:
#line 2733 "sql_yacc.yy"
    { yyval.item= new Item_func_isnotnull(yyvsp[-3].item); }
    break;

  case 628:
#line 2734 "sql_yacc.yy"
    { yyval.item= new Item_func_equal(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 629:
#line 2735 "sql_yacc.yy"
    { yyval.item= (*yyvsp[-1].boolfunc2creator)(0)->create(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 630:
#line 2737 "sql_yacc.yy"
    {
	  all_any_subquery_creator(yyvsp[-3].item, yyvsp[-2].boolfunc2creator, yyvsp[-1].num, yyvsp[0].select_lex);
	}
    break;

  case 631:
#line 2740 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_left(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 632:
#line 2741 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_right(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 633:
#line 2742 "sql_yacc.yy"
    { yyval.item= new Item_func_plus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 634:
#line 2743 "sql_yacc.yy"
    { yyval.item= new Item_func_minus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 635:
#line 2744 "sql_yacc.yy"
    { yyval.item= new Item_func_mul(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 636:
#line 2745 "sql_yacc.yy"
    { yyval.item= new Item_func_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 637:
#line 2746 "sql_yacc.yy"
    { yyval.item= new Item_func_int_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 638:
#line 2747 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 639:
#line 2748 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 640:
#line 2749 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 641:
#line 2750 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 642:
#line 2751 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 643:
#line 2753 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,0); }
    break;

  case 644:
#line 2755 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,1); }
    break;

  case 646:
#line 2759 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 648:
#line 2765 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_set_collation(yyvsp[-2].item,
					    new Item_string(yyvsp[0].lex_str.str,
							    yyvsp[0].lex_str.length,
                                                            YYTHD->charset()));
	  }
    break;

  case 651:
#line 2774 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_set_user_var(yyvsp[-2].lex_str,yyvsp[0].item);
	    Lex->uncacheable(UNCACHEABLE_RAND);
	  }
    break;

  case 652:
#line 2779 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_get_user_var(yyvsp[0].lex_str);
	    Lex->uncacheable(UNCACHEABLE_RAND);
	  }
    break;

  case 653:
#line 2784 "sql_yacc.yy"
    {

            if (yyvsp[-1].lex_str.str && yyvsp[0].lex_str.str && check_reserved_words(&yyvsp[-1].lex_str))
            {
              yyerror(ER(ER_SYNTAX_ERROR));
              YYABORT;
            }
	    if (!(yyval.item= get_system_var(YYTHD, (enum_var_type) yyvsp[-2].num, yyvsp[-1].lex_str, yyvsp[0].lex_str)))
	      YYABORT;
	  }
    break;

  case 655:
#line 2795 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 656:
#line 2796 "sql_yacc.yy"
    { yyval.item= new Item_func_neg(yyvsp[0].item); }
    break;

  case 657:
#line 2797 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_neg(yyvsp[0].item); }
    break;

  case 658:
#line 2799 "sql_yacc.yy"
    {
            yyval.item= negate_expression(YYTHD, yyvsp[0].item);
          }
    break;

  case 659:
#line 2803 "sql_yacc.yy"
    {
            yyval.item= negate_expression(YYTHD, yyvsp[0].item);
          }
    break;

  case 660:
#line 2806 "sql_yacc.yy"
    { yyval.item= yyvsp[-1].item; }
    break;

  case 661:
#line 2808 "sql_yacc.yy"
    {
	    yyvsp[-1].item_list->push_front(yyvsp[-3].item);
	    yyval.item= new Item_row(*yyvsp[-1].item_list);
	  }
    break;

  case 662:
#line 2813 "sql_yacc.yy"
    {
	    yyvsp[-1].item_list->push_front(yyvsp[-3].item);
	    yyval.item= new Item_row(*yyvsp[-1].item_list);
	  }
    break;

  case 663:
#line 2817 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 664:
#line 2818 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 665:
#line 2819 "sql_yacc.yy"
    { yyval.item= yyvsp[-1].item; }
    break;

  case 666:
#line 2821 "sql_yacc.yy"
    { yyvsp[-5].item_list->push_front(yyvsp[-2].item);
            Select->add_ftfunc_to_list((Item_func_match*)
                                        (yyval.item=new Item_func_match(*yyvsp[-5].item_list,yyvsp[-1].num))); }
    break;

  case 667:
#line 2824 "sql_yacc.yy"
    { yyval.item= new Item_func_ascii(yyvsp[-1].item); }
    break;

  case 668:
#line 2826 "sql_yacc.yy"
    {
	    yyval.item= create_func_cast(yyvsp[0].item, ITEM_CAST_CHAR, -1, &my_charset_bin);
	  }
    break;

  case 669:
#line 2830 "sql_yacc.yy"
    {
	    yyval.item= create_func_cast(yyvsp[-3].item, yyvsp[-1].cast_type,
				 Lex->length ? atoi(Lex->length) : -1,
				 Lex->charset);
	  }
    break;

  case 670:
#line 2836 "sql_yacc.yy"
    { yyval.item= new Item_func_case(* yyvsp[-2].item_list, yyvsp[-4].item, yyvsp[-1].item ); }
    break;

  case 671:
#line 2838 "sql_yacc.yy"
    {
	    yyval.item= create_func_cast(yyvsp[-3].item, yyvsp[-1].cast_type,
				 Lex->length ? atoi(Lex->length) : -1,
				 Lex->charset);
	  }
    break;

  case 672:
#line 2844 "sql_yacc.yy"
    { yyval.item= new Item_func_conv_charset(yyvsp[-3].item,yyvsp[-1].charset); }
    break;

  case 673:
#line 2846 "sql_yacc.yy"
    { yyval.item= new Item_default_value(yyvsp[-1].item); }
    break;

  case 674:
#line 2848 "sql_yacc.yy"
    { yyval.item= new Item_insert_value(yyvsp[-1].item); }
    break;

  case 675:
#line 2850 "sql_yacc.yy"
    {
	    if (!yyvsp[-2].symbol.symbol->create_func)
	    {
	      net_printf(Lex->thd, ER_FEATURE_DISABLED,
			 yyvsp[-2].symbol.symbol->group->name,
	                 yyvsp[-2].symbol.symbol->group->needed_define);
	      YYABORT;
	    }
	    yyval.item= ((Item*(*)(void))(yyvsp[-2].symbol.symbol->create_func))();
	  }
    break;

  case 676:
#line 2861 "sql_yacc.yy"
    {
	    if (!yyvsp[-3].symbol.symbol->create_func)
	    {
	      net_printf(Lex->thd, ER_FEATURE_DISABLED,
			 yyvsp[-3].symbol.symbol->group->name,
	                 yyvsp[-3].symbol.symbol->group->needed_define);
	      YYABORT;
	    }
	    yyval.item= ((Item*(*)(Item*))(yyvsp[-3].symbol.symbol->create_func))(yyvsp[-1].item);
	  }
    break;

  case 677:
#line 2872 "sql_yacc.yy"
    {
	    if (!yyvsp[-5].symbol.symbol->create_func)
	    {
	      net_printf(Lex->thd, ER_FEATURE_DISABLED,
			 yyvsp[-5].symbol.symbol->group->name,
	                 yyvsp[-5].symbol.symbol->group->needed_define);
	      YYABORT;
	    }
	    yyval.item= ((Item*(*)(Item*,Item*))(yyvsp[-5].symbol.symbol->create_func))(yyvsp[-3].item,yyvsp[-1].item);
	  }
    break;

  case 678:
#line 2883 "sql_yacc.yy"
    {
	    if (!yyvsp[-7].symbol.symbol->create_func)
	    {
	      net_printf(Lex->thd, ER_FEATURE_DISABLED,
			 yyvsp[-7].symbol.symbol->group->name,
	                 yyvsp[-7].symbol.symbol->group->needed_define);
	      YYABORT;
	    }
	    yyval.item= ((Item*(*)(Item*,Item*,Item*))(yyvsp[-7].symbol.symbol->create_func))(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item);
	  }
    break;

  case 679:
#line 2894 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item, yyvsp[-1].item, INTERVAL_DAY, 0);}
    break;

  case 680:
#line 2896 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-5].item, yyvsp[-2].item, yyvsp[-1].interval, 0); }
    break;

  case 681:
#line 2898 "sql_yacc.yy"
    { yyval.item= new Item_func_atan(yyvsp[-1].item); }
    break;

  case 682:
#line 2900 "sql_yacc.yy"
    { yyval.item= new Item_func_atan(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 683:
#line 2902 "sql_yacc.yy"
    { yyval.item= new Item_func_char(*yyvsp[-1].item_list); }
    break;

  case 684:
#line 2904 "sql_yacc.yy"
    { yyval.item= new Item_func_charset(yyvsp[-1].item); }
    break;

  case 685:
#line 2906 "sql_yacc.yy"
    { yyval.item= new Item_func_coalesce(* yyvsp[-1].item_list); }
    break;

  case 686:
#line 2908 "sql_yacc.yy"
    { yyval.item= new Item_func_collation(yyvsp[-1].item); }
    break;

  case 687:
#line 2910 "sql_yacc.yy"
    { yyval.item= new Item_func_concat(* yyvsp[-1].item_list); }
    break;

  case 688:
#line 2912 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_concat_ws(*yyvsp[-1].item_list); }
    break;

  case 689:
#line 2914 "sql_yacc.yy"
    {
	    Lex->time_zone_tables_used= &fake_time_zone_tables_list;
	    yyval.item= new Item_func_convert_tz(yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item);
	  }
    break;

  case 690:
#line 2919 "sql_yacc.yy"
    { yyval.item= new Item_func_curdate_local(); Lex->safe_to_cache_query=0; }
    break;

  case 691:
#line 2921 "sql_yacc.yy"
    { yyval.item= new Item_func_curtime_local(); Lex->safe_to_cache_query=0; }
    break;

  case 692:
#line 2923 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_curtime_local(yyvsp[-1].item);
	    Lex->safe_to_cache_query=0;
	  }
    break;

  case 693:
#line 2928 "sql_yacc.yy"
    { yyval.item= create_func_current_user(); }
    break;

  case 694:
#line 2930 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-2].item,yyvsp[-1].interval,0); }
    break;

  case 695:
#line 2932 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-2].item,yyvsp[-1].interval,1); }
    break;

  case 696:
#line 2934 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_database();
            Lex->safe_to_cache_query=0;
	  }
    break;

  case 697:
#line 2939 "sql_yacc.yy"
    { yyval.item= new Item_date_typecast(yyvsp[-1].item); }
    break;

  case 698:
#line 2941 "sql_yacc.yy"
    { yyval.item= new Item_func_dayofmonth(yyvsp[-1].item); }
    break;

  case 699:
#line 2943 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_elt(*yyvsp[-1].item_list); }
    break;

  case 700:
#line 2945 "sql_yacc.yy"
    { yyval.item= new Item_func_make_set(yyvsp[-3].item, *yyvsp[-1].item_list); }
    break;

  case 701:
#line 2947 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_encrypt(yyvsp[-1].item);
	    Lex->uncacheable(UNCACHEABLE_RAND);
	  }
    break;

  case 702:
#line 2951 "sql_yacc.yy"
    { yyval.item= new Item_func_encrypt(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 703:
#line 2953 "sql_yacc.yy"
    { yyval.item= new Item_func_decode(yyvsp[-3].item,yyvsp[-1].lex_str.str); }
    break;

  case 704:
#line 2955 "sql_yacc.yy"
    { yyval.item= new Item_func_encode(yyvsp[-3].item,yyvsp[-1].lex_str.str); }
    break;

  case 705:
#line 2957 "sql_yacc.yy"
    { yyval.item= new Item_func_des_decrypt(yyvsp[-1].item); }
    break;

  case 706:
#line 2959 "sql_yacc.yy"
    { yyval.item= new Item_func_des_decrypt(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 707:
#line 2961 "sql_yacc.yy"
    { yyval.item= new Item_func_des_encrypt(yyvsp[-1].item); }
    break;

  case 708:
#line 2963 "sql_yacc.yy"
    { yyval.item= new Item_func_des_encrypt(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 709:
#line 2965 "sql_yacc.yy"
    { yyval.item= new Item_func_export_set(yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 710:
#line 2967 "sql_yacc.yy"
    { yyval.item= new Item_func_export_set(yyvsp[-7].item, yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 711:
#line 2969 "sql_yacc.yy"
    { yyval.item= new Item_func_export_set(yyvsp[-9].item, yyvsp[-7].item, yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 712:
#line 2971 "sql_yacc.yy"
    { yyval.item= new Item_func_format(yyvsp[-3].item,atoi(yyvsp[-1].lex_str.str)); }
    break;

  case 713:
#line 2973 "sql_yacc.yy"
    { yyval.item= new Item_func_from_unixtime(yyvsp[-1].item); }
    break;

  case 714:
#line 2975 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_date_format (new Item_func_from_unixtime(yyvsp[-3].item),yyvsp[-1].item,0);
	  }
    break;

  case 715:
#line 2979 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_field(*yyvsp[-1].item_list); }
    break;

  case 716:
#line 2981 "sql_yacc.yy"
    {
#ifdef HAVE_SPATIAL
	    yyval.item= yyvsp[0].item;
#else
	    net_printf(Lex->thd, ER_FEATURE_DISABLED,
	               sym_group_geom.name, sym_group_geom.needed_define);
	    YYABORT;
#endif
	  }
    break;

  case 717:
#line 2991 "sql_yacc.yy"
    { yyval.item= new Item_func_get_format(yyvsp[-3].date_time_type, yyvsp[-1].item); }
    break;

  case 718:
#line 2993 "sql_yacc.yy"
    { yyval.item= new Item_func_hour(yyvsp[-1].item); }
    break;

  case 719:
#line 2995 "sql_yacc.yy"
    { yyval.item= new Item_func_if(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 720:
#line 2997 "sql_yacc.yy"
    { yyval.item= new Item_func_insert(yyvsp[-7].item,yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 721:
#line 3000 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[0].item,yyvsp[-3].item,yyvsp[-2].interval,0); }
    break;

  case 722:
#line 3002 "sql_yacc.yy"
    {
            if (yyvsp[0].item->type() != Item::ROW_ITEM)
            {
              yyerror(ER(ER_SYNTAX_ERROR));
              YYABORT;
            }
            yyval.item= new Item_func_interval((Item_row *)yyvsp[0].item);
          }
    break;

  case 723:
#line 3011 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_last_insert_id();
	    Lex->safe_to_cache_query= 0;
	  }
    break;

  case 724:
#line 3016 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_last_insert_id(yyvsp[-1].item);
	    Lex->safe_to_cache_query= 0;
	  }
    break;

  case 725:
#line 3021 "sql_yacc.yy"
    { yyval.item= new Item_func_left(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 726:
#line 3023 "sql_yacc.yy"
    { yyval.item= new Item_func_locate(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 727:
#line 3025 "sql_yacc.yy"
    { yyval.item= new Item_func_locate(yyvsp[-3].item,yyvsp[-5].item,yyvsp[-1].item); }
    break;

  case 728:
#line 3027 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_max(*yyvsp[-1].item_list); }
    break;

  case 729:
#line 3029 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_min(*yyvsp[-1].item_list); }
    break;

  case 730:
#line 3031 "sql_yacc.yy"
    { yyval.item= new Item_func_log(yyvsp[-1].item); }
    break;

  case 731:
#line 3033 "sql_yacc.yy"
    { yyval.item= new Item_func_log(yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 732:
#line 3035 "sql_yacc.yy"
    {
	    yyval.item= new Item_master_pos_wait(yyvsp[-3].item, yyvsp[-1].item);
	    Lex->safe_to_cache_query=0;
		  }
    break;

  case 733:
#line 3040 "sql_yacc.yy"
    {
	    yyval.item= new Item_master_pos_wait(yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item);
	    Lex->safe_to_cache_query=0;
	  }
    break;

  case 734:
#line 3045 "sql_yacc.yy"
    { yyval.item= new Item_func_microsecond(yyvsp[-1].item); }
    break;

  case 735:
#line 3047 "sql_yacc.yy"
    { yyval.item= new Item_func_minute(yyvsp[-1].item); }
    break;

  case 736:
#line 3049 "sql_yacc.yy"
    { yyval.item = new Item_func_mod( yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 737:
#line 3051 "sql_yacc.yy"
    { yyval.item= new Item_func_month(yyvsp[-1].item); }
    break;

  case 738:
#line 3053 "sql_yacc.yy"
    { yyval.item= new Item_func_now_local(); Lex->safe_to_cache_query=0;}
    break;

  case 739:
#line 3055 "sql_yacc.yy"
    { yyval.item= new Item_func_now_local(yyvsp[-1].item); Lex->safe_to_cache_query=0;}
    break;

  case 740:
#line 3057 "sql_yacc.yy"
    {
	    yyval.item= YYTHD->variables.old_passwords ?
              (Item *) new Item_func_old_password(yyvsp[-1].item) :
	      (Item *) new Item_func_password(yyvsp[-1].item);
	  }
    break;

  case 741:
#line 3063 "sql_yacc.yy"
    { yyval.item=  new Item_func_old_password(yyvsp[-1].item); }
    break;

  case 742:
#line 3065 "sql_yacc.yy"
    { yyval.item = new Item_func_locate(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 743:
#line 3067 "sql_yacc.yy"
    { yyval.item= new Item_func_rand(yyvsp[-1].item); Lex->uncacheable(UNCACHEABLE_RAND);}
    break;

  case 744:
#line 3069 "sql_yacc.yy"
    { yyval.item= new Item_func_rand(); Lex->uncacheable(UNCACHEABLE_RAND);}
    break;

  case 745:
#line 3071 "sql_yacc.yy"
    { yyval.item= new Item_func_replace(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 746:
#line 3073 "sql_yacc.yy"
    { yyval.item= new Item_func_right(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 747:
#line 3075 "sql_yacc.yy"
    { yyval.item= new Item_func_round(yyvsp[-1].item, new Item_int((char*)"0",0,1),0); }
    break;

  case 748:
#line 3076 "sql_yacc.yy"
    { yyval.item= new Item_func_round(yyvsp[-3].item,yyvsp[-1].item,0); }
    break;

  case 749:
#line 3078 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item, yyvsp[-1].item, INTERVAL_DAY, 1);}
    break;

  case 750:
#line 3080 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-5].item, yyvsp[-2].item, yyvsp[-1].interval, 1); }
    break;

  case 751:
#line 3082 "sql_yacc.yy"
    { yyval.item= new Item_func_second(yyvsp[-1].item); }
    break;

  case 752:
#line 3084 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 753:
#line 3086 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 754:
#line 3088 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 755:
#line 3090 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 756:
#line 3092 "sql_yacc.yy"
    { yyval.item= new Item_func_substr_index(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 757:
#line 3094 "sql_yacc.yy"
    { yyval.item= new Item_time_typecast(yyvsp[-1].item); }
    break;

  case 758:
#line 3096 "sql_yacc.yy"
    { yyval.item= new Item_datetime_typecast(yyvsp[-1].item); }
    break;

  case 759:
#line 3098 "sql_yacc.yy"
    { yyval.item= new Item_func_add_time(yyvsp[-3].item, yyvsp[-1].item, 1, 0); }
    break;

  case 760:
#line 3100 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item); }
    break;

  case 761:
#line 3102 "sql_yacc.yy"
    { yyval.item= new Item_func_ltrim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 762:
#line 3104 "sql_yacc.yy"
    { yyval.item= new Item_func_rtrim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 763:
#line 3106 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 764:
#line 3108 "sql_yacc.yy"
    { yyval.item= new Item_func_ltrim(yyvsp[-1].item); }
    break;

  case 765:
#line 3110 "sql_yacc.yy"
    { yyval.item= new Item_func_rtrim(yyvsp[-1].item); }
    break;

  case 766:
#line 3112 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item); }
    break;

  case 767:
#line 3114 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 768:
#line 3116 "sql_yacc.yy"
    { yyval.item= new Item_func_round(yyvsp[-3].item,yyvsp[-1].item,1); }
    break;

  case 769:
#line 3118 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_sum_udf_str(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_sum_udf_str(yyvsp[-3].udf);
	  }
    break;

  case 770:
#line 3125 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_sum_udf_float(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_sum_udf_float(yyvsp[-3].udf);
	  }
    break;

  case 771:
#line 3132 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_sum_udf_int(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_sum_udf_int(yyvsp[-3].udf);
	  }
    break;

  case 772:
#line 3139 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_func_udf_str(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_func_udf_str(yyvsp[-3].udf);
	  }
    break;

  case 773:
#line 3146 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_func_udf_float(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_func_udf_float(yyvsp[-3].udf);
	  }
    break;

  case 774:
#line 3153 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_func_udf_int(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_func_udf_int(yyvsp[-3].udf);
	  }
    break;

  case 775:
#line 3160 "sql_yacc.yy"
    {
            yyval.item= new Item_func_unique_users(yyvsp[-7].item,atoi(yyvsp[-5].lex_str.str),atoi(yyvsp[-3].lex_str.str), * yyvsp[-1].item_list);
	  }
    break;

  case 776:
#line 3164 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_unix_timestamp();
	    Lex->safe_to_cache_query=0;
	  }
    break;

  case 777:
#line 3169 "sql_yacc.yy"
    { yyval.item= new Item_func_unix_timestamp(yyvsp[-1].item); }
    break;

  case 778:
#line 3171 "sql_yacc.yy"
    { yyval.item= new Item_func_user(); Lex->safe_to_cache_query=0; }
    break;

  case 779:
#line 3173 "sql_yacc.yy"
    { yyval.item= new Item_func_curdate_utc(); Lex->safe_to_cache_query=0;}
    break;

  case 780:
#line 3175 "sql_yacc.yy"
    { yyval.item= new Item_func_curtime_utc(); Lex->safe_to_cache_query=0;}
    break;

  case 781:
#line 3177 "sql_yacc.yy"
    { yyval.item= new Item_func_now_utc(); Lex->safe_to_cache_query=0;}
    break;

  case 782:
#line 3179 "sql_yacc.yy"
    {
            yyval.item= new Item_func_week(yyvsp[-1].item,new Item_int((char*) "0",
				   YYTHD->variables.default_week_format,1));
          }
    break;

  case 783:
#line 3184 "sql_yacc.yy"
    { yyval.item= new Item_func_week(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 784:
#line 3186 "sql_yacc.yy"
    { yyval.item= new Item_func_year(yyvsp[-1].item); }
    break;

  case 785:
#line 3188 "sql_yacc.yy"
    { yyval.item= new Item_func_yearweek(yyvsp[-1].item,new Item_int((char*) "0",0,1)); }
    break;

  case 786:
#line 3190 "sql_yacc.yy"
    { yyval.item= new Item_func_yearweek(yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 787:
#line 3192 "sql_yacc.yy"
    {
	    yyval.item=new Item_func_benchmark(yyvsp[-3].ulong_num,yyvsp[-1].item);
	    Lex->uncacheable(UNCACHEABLE_SIDEEFFECT);
	  }
    break;

  case 788:
#line 3197 "sql_yacc.yy"
    { yyval.item=new Item_extract( yyvsp[-3].interval, yyvsp[-1].item); }
    break;

  case 789:
#line 3201 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 790:
#line 3203 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 791:
#line 3205 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_wkb(yyvsp[-1].item)); }
    break;

  case 792:
#line 3207 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_wkb(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 793:
#line 3209 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
                           Geometry::wkb_geometrycollection,
                           Geometry::wkb_point)); }
    break;

  case 794:
#line 3213 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
                  Geometry::wkb_linestring, Geometry::wkb_point)); }
    break;

  case 795:
#line 3216 "sql_yacc.yy"
    { yyval.item= GEOM_NEW( Item_func_spatial_collection(* yyvsp[-1].item_list,
                   Geometry::wkb_multilinestring, Geometry::wkb_linestring)); }
    break;

  case 796:
#line 3219 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 797:
#line 3221 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 798:
#line 3223 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 799:
#line 3225 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 800:
#line 3227 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 801:
#line 3229 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 802:
#line 3231 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
                  Geometry::wkb_multipoint, Geometry::wkb_point)); }
    break;

  case 803:
#line 3234 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
                  Geometry::wkb_multipolygon, Geometry::wkb_polygon)); }
    break;

  case 804:
#line 3237 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_point(yyvsp[-3].item,yyvsp[-1].item)); }
    break;

  case 805:
#line 3239 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 806:
#line 3241 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 807:
#line 3243 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 808:
#line 3245 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 809:
#line 3247 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
	          Geometry::wkb_polygon, Geometry::wkb_linestring)); }
    break;

  case 810:
#line 3250 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 811:
#line 3252 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 812:
#line 3254 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 813:
#line 3256 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 814:
#line 3260 "sql_yacc.yy"
    { yyval.num= FT_NL;  }
    break;

  case 815:
#line 3261 "sql_yacc.yy"
    { yyval.num= FT_NL | FT_EXPAND; }
    break;

  case 816:
#line 3262 "sql_yacc.yy"
    { yyval.num= FT_BOOL; }
    break;

  case 817:
#line 3266 "sql_yacc.yy"
    { yyval.item_list= NULL; }
    break;

  case 818:
#line 3267 "sql_yacc.yy"
    { yyval.item_list= yyvsp[0].item_list;}
    break;

  case 819:
#line 3271 "sql_yacc.yy"
    { yyval.item=new Item_sum_avg(yyvsp[-1].item); }
    break;

  case 820:
#line 3273 "sql_yacc.yy"
    { yyval.item=new Item_sum_and(yyvsp[-1].item); }
    break;

  case 821:
#line 3275 "sql_yacc.yy"
    { yyval.item=new Item_sum_or(yyvsp[-1].item); }
    break;

  case 822:
#line 3277 "sql_yacc.yy"
    { yyval.item=new Item_sum_xor(yyvsp[-1].item); }
    break;

  case 823:
#line 3279 "sql_yacc.yy"
    { yyval.item=new Item_sum_count(new Item_int((int32) 0L,1)); }
    break;

  case 824:
#line 3281 "sql_yacc.yy"
    { yyval.item=new Item_sum_count(yyvsp[-1].item); }
    break;

  case 825:
#line 3283 "sql_yacc.yy"
    { Select->in_sum_expr++; }
    break;

  case 826:
#line 3285 "sql_yacc.yy"
    { Select->in_sum_expr--; }
    break;

  case 827:
#line 3287 "sql_yacc.yy"
    { yyval.item=new Item_sum_count_distinct(* yyvsp[-2].item_list); }
    break;

  case 828:
#line 3289 "sql_yacc.yy"
    { yyval.item= new Item_sum_unique_users(yyvsp[-7].item,atoi(yyvsp[-5].lex_str.str),atoi(yyvsp[-3].lex_str.str),yyvsp[-1].item); }
    break;

  case 829:
#line 3291 "sql_yacc.yy"
    { yyval.item=new Item_sum_min(yyvsp[-1].item); }
    break;

  case 830:
#line 3293 "sql_yacc.yy"
    { yyval.item=new Item_sum_max(yyvsp[-1].item); }
    break;

  case 831:
#line 3295 "sql_yacc.yy"
    { yyval.item=new Item_sum_std(yyvsp[-1].item); }
    break;

  case 832:
#line 3297 "sql_yacc.yy"
    { yyval.item=new Item_sum_variance(yyvsp[-1].item); }
    break;

  case 833:
#line 3299 "sql_yacc.yy"
    { yyval.item=new Item_sum_sum(yyvsp[-1].item); }
    break;

  case 834:
#line 3301 "sql_yacc.yy"
    { Select->in_sum_expr++; }
    break;

  case 835:
#line 3305 "sql_yacc.yy"
    {
	    Select->in_sum_expr--;
	    yyval.item=new Item_func_group_concat(yyvsp[-5].num,yyvsp[-3].item_list,Select->gorder_list,yyvsp[-1].string);
	    yyvsp[-3].item_list->empty();
	  }
    break;

  case 836:
#line 3312 "sql_yacc.yy"
    { yyval.num = 0; }
    break;

  case 837:
#line 3313 "sql_yacc.yy"
    { yyval.num = 1; }
    break;

  case 838:
#line 3316 "sql_yacc.yy"
    { yyval.string = new (YYTHD->mem_root) String(",",1,default_charset_info); }
    break;

  case 839:
#line 3317 "sql_yacc.yy"
    { yyval.string = yyvsp[0].string; }
    break;

  case 840:
#line 3322 "sql_yacc.yy"
    {
            Select->gorder_list = NULL;
	  }
    break;

  case 841:
#line 3326 "sql_yacc.yy"
    {
            SELECT_LEX *select= Select;
            select->gorder_list=
	      (SQL_LIST*) sql_memdup((char*) &select->order_list,
				     sizeof(st_sql_list));
	    select->order_list.empty();
	  }
    break;

  case 842:
#line 3337 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  if (lex->current_select->inc_in_sum_expr())
	  {
	    yyerror(ER(ER_SYNTAX_ERROR));
	    YYABORT;
	  }
	}
    break;

  case 843:
#line 3346 "sql_yacc.yy"
    {
	  Select->in_sum_expr--;
	  yyval.item= yyvsp[0].item;
	}
    break;

  case 844:
#line 3352 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_CHAR; Lex->charset= &my_charset_bin; }
    break;

  case 845:
#line 3353 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_CHAR; }
    break;

  case 846:
#line 3354 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_CHAR; Lex->charset= national_charset_info; }
    break;

  case 847:
#line 3355 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_SIGNED_INT; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 848:
#line 3356 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_SIGNED_INT; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 849:
#line 3357 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_UNSIGNED_INT; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 850:
#line 3358 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_UNSIGNED_INT; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 851:
#line 3359 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_DATE; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 852:
#line 3360 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_TIME; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 853:
#line 3361 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_DATETIME; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 854:
#line 3365 "sql_yacc.yy"
    { Select->expr_list.push_front(new List<Item>); }
    break;

  case 855:
#line 3367 "sql_yacc.yy"
    { yyval.item_list= Select->expr_list.pop(); }
    break;

  case 856:
#line 3370 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 857:
#line 3371 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 858:
#line 3374 "sql_yacc.yy"
    { yyval.item_list= yyvsp[0].item_list; }
    break;

  case 859:
#line 3375 "sql_yacc.yy"
    { yyval.item_list= yyvsp[-1].item_list; }
    break;

  case 860:
#line 3378 "sql_yacc.yy"
    { Select->expr_list.push_front(new List<Item>); }
    break;

  case 861:
#line 3380 "sql_yacc.yy"
    { yyval.item_list= Select->expr_list.pop(); }
    break;

  case 862:
#line 3383 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 863:
#line 3384 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 864:
#line 3387 "sql_yacc.yy"
    { yyval.item= NULL; }
    break;

  case 865:
#line 3388 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 866:
#line 3391 "sql_yacc.yy"
    { yyval.item= NULL; }
    break;

  case 867:
#line 3392 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 868:
#line 3395 "sql_yacc.yy"
    { Select->when_list.push_front(new List<Item>); }
    break;

  case 869:
#line 3397 "sql_yacc.yy"
    { yyval.item_list= Select->when_list.pop(); }
    break;

  case 870:
#line 3401 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->when_list.head()->push_back(yyvsp[-2].item);
	    sel->when_list.head()->push_back(yyvsp[0].item);
	}
    break;

  case 871:
#line 3407 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->when_list.head()->push_back(yyvsp[-2].item);
	    sel->when_list.head()->push_back(yyvsp[0].item);
	  }
    break;

  case 872:
#line 3414 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list; }
    break;

  case 873:
#line 3415 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list; }
    break;

  case 874:
#line 3416 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list; }
    break;

  case 875:
#line 3418 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list ; yyvsp[-2].table_list->next->straight=1; }
    break;

  case 876:
#line 3420 "sql_yacc.yy"
    { add_join_on(yyvsp[-2].table_list,yyvsp[0].item); yyval.table_list=yyvsp[-2].table_list; }
    break;

  case 877:
#line 3423 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->db1=yyvsp[-3].table_list->db; sel->table1=yyvsp[-3].table_list->alias;
	    sel->db2=yyvsp[-1].table_list->db; sel->table2=yyvsp[-1].table_list->alias;
	  }
    break;

  case 878:
#line 3429 "sql_yacc.yy"
    { add_join_on(yyvsp[-5].table_list,yyvsp[-1].item); yyval.table_list=yyvsp[-5].table_list; }
    break;

  case 879:
#line 3432 "sql_yacc.yy"
    { add_join_on(yyvsp[-2].table_list,yyvsp[0].item); yyvsp[-2].table_list->outer_join|=JOIN_TYPE_LEFT; yyval.table_list=yyvsp[-2].table_list; }
    break;

  case 880:
#line 3434 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->db1=yyvsp[-4].table_list->db; sel->table1=yyvsp[-4].table_list->alias;
	    sel->db2=yyvsp[0].table_list->db; sel->table2=yyvsp[0].table_list->alias;
	  }
    break;

  case 881:
#line 3440 "sql_yacc.yy"
    { add_join_on(yyvsp[-5].table_list,yyvsp[-1].item); yyvsp[-5].table_list->outer_join|=JOIN_TYPE_LEFT; yyval.table_list=yyvsp[-5].table_list; }
    break;

  case 882:
#line 3442 "sql_yacc.yy"
    {
	    add_join_natural(yyvsp[-5].table_list,yyvsp[-5].table_list->next);
	    yyvsp[-5].table_list->next->outer_join|=JOIN_TYPE_LEFT;
	    yyval.table_list=yyvsp[0].table_list;
	  }
    break;

  case 883:
#line 3448 "sql_yacc.yy"
    { add_join_on(yyvsp[-6].table_list,yyvsp[0].item); yyvsp[-6].table_list->outer_join|=JOIN_TYPE_RIGHT; yyval.table_list=yyvsp[-2].table_list; }
    break;

  case 884:
#line 3450 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->db1=yyvsp[-4].table_list->db; sel->table1=yyvsp[-4].table_list->alias;
	    sel->db2=yyvsp[0].table_list->db; sel->table2=yyvsp[0].table_list->alias;
	  }
    break;

  case 885:
#line 3456 "sql_yacc.yy"
    { add_join_on(yyvsp[-9].table_list,yyvsp[-1].item); yyvsp[-9].table_list->outer_join|=JOIN_TYPE_RIGHT; yyval.table_list=yyvsp[-5].table_list; }
    break;

  case 886:
#line 3458 "sql_yacc.yy"
    {
	    add_join_natural(yyvsp[-5].table_list->next,yyvsp[-5].table_list);
	    yyvsp[-5].table_list->outer_join|=JOIN_TYPE_RIGHT;
	    yyval.table_list=yyvsp[0].table_list;
	  }
    break;

  case 887:
#line 3464 "sql_yacc.yy"
    { add_join_natural(yyvsp[-3].table_list,yyvsp[-3].table_list->next); yyval.table_list=yyvsp[0].table_list; }
    break;

  case 888:
#line 3467 "sql_yacc.yy"
    {}
    break;

  case 889:
#line 3468 "sql_yacc.yy"
    {}
    break;

  case 890:
#line 3469 "sql_yacc.yy"
    {}
    break;

  case 891:
#line 3473 "sql_yacc.yy"
    {
	  SELECT_LEX *sel= Select;
	  sel->use_index_ptr=sel->ignore_index_ptr=0;
	  sel->table_join_options= 0;
	}
    break;

  case 892:
#line 3479 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  SELECT_LEX *sel= lex->current_select;
	  if (!(yyval.table_list= sel->add_table_to_list(lex->thd, yyvsp[-2].table, yyvsp[-1].lex_str_ptr,
					   sel->get_table_join_options(),
					   lex->lock_option,
					   sel->get_use_index(),
					   sel->get_ignore_index())))
	    YYABORT;
	}
    break;

  case 893:
#line 3490 "sql_yacc.yy"
    { add_join_on(yyvsp[-3].table_list,yyvsp[-1].item); yyvsp[-3].table_list->outer_join|=JOIN_TYPE_LEFT; yyval.table_list=yyvsp[-3].table_list; }
    break;

  case 894:
#line 3492 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  SELECT_LEX_UNIT *unit= lex->current_select->master_unit();
	  lex->current_select= unit->outer_select();
	  if (!(yyval.table_list= lex->current_select->
                add_table_to_list(lex->thd, new Table_ident(unit), yyvsp[0].lex_str_ptr, 0,
				  TL_READ,(List<String> *)0,
	                          (List<String> *)0)))

	    YYABORT;
	}
    break;

  case 895:
#line 3503 "sql_yacc.yy"
    { yyval.table_list=yyvsp[-1].table_list; }
    break;

  case 897:
#line 3508 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
            SELECT_LEX * sel= lex->current_select;
	    if (sel->set_braces(1))
	    {
	      yyerror(ER(ER_SYNTAX_ERROR));
	      YYABORT;
	    }
            /* select in braces, can't contain global parameters */
	    if (sel->master_unit()->fake_select_lex)
              sel->master_unit()->global_parameters=
                 sel->master_unit()->fake_select_lex;
	  }
    break;

  case 898:
#line 3523 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  lex->derived_tables= 1;
	  if (((int)lex->sql_command >= (int)SQLCOM_HA_OPEN &&
	       lex->sql_command <= (int)SQLCOM_HA_READ) ||
	       lex->sql_command == (int)SQLCOM_KILL)
	  {
	    yyerror(ER(ER_SYNTAX_ERROR));
	    YYABORT;
	  }
	  if (lex->current_select->linkage == GLOBAL_OPTIONS_TYPE ||
              mysql_new_select(lex, 1))
	    YYABORT;
	  mysql_init_select(lex);
	  lex->current_select->linkage= DERIVED_TABLE_TYPE;
	  lex->current_select->parsing_place= SELECT_LIST;
	}
    break;

  case 899:
#line 3541 "sql_yacc.yy"
    {
	  Select->parsing_place= NO_MATTER;
	}
    break;

  case 901:
#line 3548 "sql_yacc.yy"
    {}
    break;

  case 902:
#line 3549 "sql_yacc.yy"
    {}
    break;

  case 903:
#line 3552 "sql_yacc.yy"
    {}
    break;

  case 904:
#line 3554 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->use_index= *yyvsp[0].string_list;
	    sel->use_index_ptr= &sel->use_index;
	  }
    break;

  case 905:
#line 3560 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->use_index= *yyvsp[0].string_list;
	    sel->use_index_ptr= &sel->use_index;
	    sel->table_join_options|= TL_OPTION_FORCE_INDEX;
	  }
    break;

  case 906:
#line 3567 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->ignore_index= *yyvsp[0].string_list;
	    sel->ignore_index_ptr= &sel->ignore_index;
	  }
    break;

  case 907:
#line 3574 "sql_yacc.yy"
    { Select->interval_list.empty(); }
    break;

  case 908:
#line 3576 "sql_yacc.yy"
    { yyval.string_list= &Select->interval_list; }
    break;

  case 909:
#line 3580 "sql_yacc.yy"
    {}
    break;

  case 910:
#line 3581 "sql_yacc.yy"
    {}
    break;

  case 911:
#line 3586 "sql_yacc.yy"
    { Select->
	    interval_list.push_back(new (YYTHD->mem_root) String((const char*) yyvsp[0].lex_str.str, yyvsp[0].lex_str.length,
				    system_charset_info)); }
    break;

  case 912:
#line 3590 "sql_yacc.yy"
    { Select->
	    interval_list.push_back(new (YYTHD->mem_root) String((const char*) yyvsp[0].lex_str.str, yyvsp[0].lex_str.length,
				    system_charset_info)); }
    break;

  case 913:
#line 3594 "sql_yacc.yy"
    { Select->
	    interval_list.push_back(new (YYTHD->mem_root) String("PRIMARY", 7,
				    system_charset_info)); }
    break;

  case 914:
#line 3600 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    if (!(yyval.item= new Item_func_eq(new Item_field(sel->db1, sel->table1,
						      yyvsp[0].lex_str.str),
				       new Item_field(sel->db2, sel->table2,
						      yyvsp[0].lex_str.str))))
	      YYABORT;
	  }
    break;

  case 915:
#line 3609 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    if (!(yyval.item= new Item_cond_and(new Item_func_eq(new Item_field(sel->db1,sel->table1,yyvsp[0].lex_str.str), new Item_field(sel->db2,sel->table2,yyvsp[0].lex_str.str)), yyvsp[-2].item)))
	      YYABORT;
	  }
    break;

  case 916:
#line 3616 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_HOUR; }
    break;

  case 917:
#line 3617 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_MICROSECOND; }
    break;

  case 918:
#line 3618 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_MINUTE; }
    break;

  case 919:
#line 3619 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_SECOND; }
    break;

  case 920:
#line 3620 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY; }
    break;

  case 921:
#line 3621 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR_MICROSECOND; }
    break;

  case 922:
#line 3622 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR_MINUTE; }
    break;

  case 923:
#line 3623 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR_SECOND; }
    break;

  case 924:
#line 3624 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR; }
    break;

  case 925:
#line 3625 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MICROSECOND; }
    break;

  case 926:
#line 3626 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MINUTE_MICROSECOND; }
    break;

  case 927:
#line 3627 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MINUTE_SECOND; }
    break;

  case 928:
#line 3628 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MINUTE; }
    break;

  case 929:
#line 3629 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MONTH; }
    break;

  case 930:
#line 3630 "sql_yacc.yy"
    { yyval.interval=INTERVAL_SECOND_MICROSECOND; }
    break;

  case 931:
#line 3631 "sql_yacc.yy"
    { yyval.interval=INTERVAL_SECOND; }
    break;

  case 932:
#line 3632 "sql_yacc.yy"
    { yyval.interval=INTERVAL_YEAR_MONTH; }
    break;

  case 933:
#line 3633 "sql_yacc.yy"
    { yyval.interval=INTERVAL_YEAR; }
    break;

  case 934:
#line 3637 "sql_yacc.yy"
    {yyval.date_time_type=MYSQL_TIMESTAMP_DATE;}
    break;

  case 935:
#line 3638 "sql_yacc.yy"
    {yyval.date_time_type=MYSQL_TIMESTAMP_TIME;}
    break;

  case 936:
#line 3639 "sql_yacc.yy"
    {yyval.date_time_type=MYSQL_TIMESTAMP_DATETIME;}
    break;

  case 937:
#line 3640 "sql_yacc.yy"
    {yyval.date_time_type=MYSQL_TIMESTAMP_DATETIME;}
    break;

  case 941:
#line 3649 "sql_yacc.yy"
    { yyval.lex_str_ptr=0; }
    break;

  case 942:
#line 3651 "sql_yacc.yy"
    { yyval.lex_str_ptr= (LEX_STRING*) sql_memdup(&yyvsp[0].lex_str,sizeof(LEX_STRING)); }
    break;

  case 945:
#line 3659 "sql_yacc.yy"
    { Select->where= 0; }
    break;

  case 946:
#line 3661 "sql_yacc.yy"
    {
            Select->parsing_place= IN_WHERE;
          }
    break;

  case 947:
#line 3665 "sql_yacc.yy"
    {
            SELECT_LEX *select= Select;
	    select->where= yyvsp[0].item;
            select->parsing_place= NO_MATTER;
	    if (yyvsp[0].item)
	      yyvsp[0].item->top_level_item();
	  }
    break;

  case 949:
#line 3677 "sql_yacc.yy"
    {
	    Select->parsing_place= IN_HAVING;
          }
    break;

  case 950:
#line 3681 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->having= yyvsp[0].item;
	    sel->parsing_place= NO_MATTER;
	    if (yyvsp[0].item)
	      yyvsp[0].item->top_level_item();
	  }
    break;

  case 951:
#line 3691 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 952:
#line 3693 "sql_yacc.yy"
    { 
            yyval.item= new Item_string("\\", 1, &my_charset_latin1);
          }
    break;

  case 955:
#line 3709 "sql_yacc.yy"
    { if (add_group_to_list(YYTHD, yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 956:
#line 3711 "sql_yacc.yy"
    { if (add_group_to_list(YYTHD, yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 957:
#line 3714 "sql_yacc.yy"
    {}
    break;

  case 958:
#line 3716 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (lex->current_select->linkage == GLOBAL_OPTIONS_TYPE)
	    {
	      net_printf(lex->thd, ER_WRONG_USAGE, "WITH CUBE",
		       "global union parameters");
	      YYABORT;
	    }
	    lex->current_select->olap= CUBE_TYPE;
	    net_printf(lex->thd, ER_NOT_SUPPORTED_YET, "CUBE");
	    YYABORT;	/* To be deleted in 5.1 */
	  }
    break;

  case 959:
#line 3729 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    if (lex->current_select->linkage == GLOBAL_OPTIONS_TYPE)
	    {
	      net_printf(lex->thd, ER_WRONG_USAGE, "WITH ROLLUP",
		       "global union parameters");
	      YYABORT;
	    }
	    lex->current_select->olap= ROLLUP_TYPE;
	  }
    break;

  case 962:
#line 3751 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->current_select->linkage != GLOBAL_OPTIONS_TYPE &&
	      lex->current_select->olap !=
	      UNSPECIFIED_OLAP_TYPE)
	  {
	    net_printf(lex->thd, ER_WRONG_USAGE,
		       "CUBE/ROLLUP",
		       "ORDER BY");
	    YYABORT;
	  }
	}
    break;

  case 964:
#line 3766 "sql_yacc.yy"
    { if (add_order_to_list(YYTHD, yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 965:
#line 3768 "sql_yacc.yy"
    { if (add_order_to_list(YYTHD, yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 966:
#line 3771 "sql_yacc.yy"
    { yyval.num =  1; }
    break;

  case 967:
#line 3772 "sql_yacc.yy"
    { yyval.num =1; }
    break;

  case 968:
#line 3773 "sql_yacc.yy"
    { yyval.num =0; }
    break;

  case 969:
#line 3778 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  SELECT_LEX *sel= lex->current_select;
          sel->offset_limit= 0L;
          sel->select_limit= HA_POS_ERROR;
	}
    break;

  case 970:
#line 3784 "sql_yacc.yy"
    {}
    break;

  case 971:
#line 3788 "sql_yacc.yy"
    {}
    break;

  case 972:
#line 3789 "sql_yacc.yy"
    {}
    break;

  case 973:
#line 3793 "sql_yacc.yy"
    {}
    break;

  case 974:
#line 3798 "sql_yacc.yy"
    {
            SELECT_LEX *sel= Select;
            sel->select_limit= yyvsp[0].ulong_num;
            sel->offset_limit= 0L;
	    sel->explicit_limit= 1;
	  }
    break;

  case 975:
#line 3805 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->select_limit= yyvsp[0].ulong_num;
	    sel->offset_limit= yyvsp[-2].ulong_num;
	    sel->explicit_limit= 1;
	  }
    break;

  case 976:
#line 3812 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->select_limit= yyvsp[-2].ulong_num;
	    sel->offset_limit= yyvsp[0].ulong_num;
	    sel->explicit_limit= 1;
	  }
    break;

  case 977:
#line 3823 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->current_select->select_limit= HA_POS_ERROR;
	}
    break;

  case 978:
#line 3828 "sql_yacc.yy"
    {
	  SELECT_LEX *sel= Select;
	  sel->select_limit= (ha_rows) yyvsp[0].ulonglong_number;
	  sel->explicit_limit= 1;
	}
    break;

  case 979:
#line 3835 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 980:
#line 3836 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 981:
#line 3837 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 982:
#line 3838 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 983:
#line 3839 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 984:
#line 3843 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 985:
#line 3844 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 986:
#line 3845 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 987:
#line 3846 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 988:
#line 3847 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 990:
#line 3853 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (&lex->select_lex != lex->current_select)
	    {
	      net_printf(lex->thd, ER_WRONG_USAGE,
			  "PROCEDURE",
			  "subquery");
	      YYABORT;
	    }
	    lex->proc_list.elements=0;
	    lex->proc_list.first=0;
	    lex->proc_list.next= (byte**) &lex->proc_list.first;
	    if (add_proc_to_list(lex->thd, new Item_field(NULL,NULL,yyvsp[0].lex_str.str)))
	      YYABORT;
	    Lex->uncacheable(UNCACHEABLE_SIDEEFFECT);
	  }
    break;

  case 992:
#line 3873 "sql_yacc.yy"
    {}
    break;

  case 993:
#line 3874 "sql_yacc.yy"
    {}
    break;

  case 996:
#line 3882 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    if (add_proc_to_list(lex->thd, yyvsp[0].item))
	      YYABORT;
	    if (!yyvsp[0].item->name)
	      yyvsp[0].item->set_name(yyvsp[-1].simple_string,(uint) ((char*) lex->tok_end - yyvsp[-1].simple_string), YYTHD->charset());
	  }
    break;

  case 997:
#line 3893 "sql_yacc.yy"
    {
             LEX *lex=Lex;
	     if (!lex->describe && (!(lex->result= new select_dumpvar())))
	        YYABORT;
	   }
    break;

  case 998:
#line 3899 "sql_yacc.yy"
    {}
    break;

  case 1000:
#line 3904 "sql_yacc.yy"
    {}
    break;

  case 1001:
#line 3908 "sql_yacc.yy"
    {
             LEX *lex=Lex;
	     if (lex->result && ((select_dumpvar *)lex->result)->var_list.push_back((LEX_STRING*) sql_memdup(&yyvsp[0].lex_str,sizeof(LEX_STRING))))
	       YYABORT;
	   }
    break;

  case 1002:
#line 3917 "sql_yacc.yy"
    {
          LEX *lex= Lex;
          lex->uncacheable(UNCACHEABLE_SIDEEFFECT);
          if (!(lex->exchange= new sql_exchange(yyvsp[0].lex_str.str, 0)) ||
              !(lex->result= new select_export(lex->exchange)))
            YYABORT;
	}
    break;

  case 1004:
#line 3926 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (!lex->describe)
	  {
	    lex->uncacheable(UNCACHEABLE_SIDEEFFECT);
	    if (!(lex->exchange= new sql_exchange(yyvsp[0].lex_str.str,1)))
	      YYABORT;
	    if (!(lex->result= new select_dump(lex->exchange)))
	      YYABORT;
	  }
	}
    break;

  case 1005:
#line 3938 "sql_yacc.yy"
    {
	  Lex->uncacheable(UNCACHEABLE_SIDEEFFECT);
	}
    break;

  case 1006:
#line 3948 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_DO;
	  mysql_init_select(lex);
	}
    break;

  case 1007:
#line 3954 "sql_yacc.yy"
    {
	  Lex->insert_list= yyvsp[0].item_list;
	}
    break;

  case 1008:
#line 3965 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_DROP_TABLE;
	  lex->drop_temporary= yyvsp[-4].num;
	  lex->drop_if_exists= yyvsp[-2].num;
	}
    break;

  case 1009:
#line 3971 "sql_yacc.yy"
    {}
    break;

  case 1010:
#line 3972 "sql_yacc.yy"
    {
	     LEX *lex=Lex;
	     lex->sql_command= SQLCOM_DROP_INDEX;
	     lex->alter_info.drop_list.empty();
	     lex->alter_info.drop_list.push_back(new Alter_drop(Alter_drop::KEY,
                                                                yyvsp[-3].lex_str.str));
	     if (!lex->current_select->add_table_to_list(lex->thd, yyvsp[-1].table, NULL,
							TL_OPTION_UPDATING))
	      YYABORT;
	  }
    break;

  case 1011:
#line 3983 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_DROP_DB;
	    lex->drop_if_exists=yyvsp[-1].num;
	    lex->name=yyvsp[0].lex_str.str;
	 }
    break;

  case 1012:
#line 3990 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command = SQLCOM_DROP_FUNCTION;
	    lex->udf.name = yyvsp[0].lex_str;
	  }
    break;

  case 1013:
#line 3996 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command = SQLCOM_DROP_USER;
	    lex->users_list.empty();
	  }
    break;

  case 1014:
#line 4002 "sql_yacc.yy"
    {}
    break;

  case 1017:
#line 4012 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, yyvsp[0].table, NULL, TL_OPTION_UPDATING))
	    YYABORT;
	}
    break;

  case 1018:
#line 4019 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 1019:
#line 4020 "sql_yacc.yy"
    { yyval.num= 1; }
    break;

  case 1020:
#line 4024 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 1021:
#line 4025 "sql_yacc.yy"
    { yyval.num= 1; }
    break;

  case 1022:
#line 4033 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  lex->sql_command= SQLCOM_INSERT;
	  lex->duplicates= DUP_ERROR;
          mysql_init_select(lex);
	  /* for subselects */
          lex->lock_option= (using_update_log) ? TL_READ_NO_INSERT : TL_READ;
	  lex->select_lex.resolve_mode= SELECT_LEX::INSERT_MODE;
	}
    break;

  case 1023:
#line 4043 "sql_yacc.yy"
    {
	  Select->set_lock_for_tables(yyvsp[-2].lock_type);
	  Lex->current_select= &Lex->select_lex;
	}
    break;

  case 1024:
#line 4048 "sql_yacc.yy"
    {}
    break;

  case 1025:
#line 4053 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_REPLACE;
	  lex->duplicates= DUP_REPLACE;
          mysql_init_select(lex);
	  lex->select_lex.resolve_mode= SELECT_LEX::INSERT_MODE;
	}
    break;

  case 1026:
#line 4061 "sql_yacc.yy"
    {
	  Select->set_lock_for_tables(yyvsp[-1].lock_type);
	  Lex->current_select= &Lex->select_lex;
	}
    break;

  case 1027:
#line 4066 "sql_yacc.yy"
    {}
    break;

  case 1028:
#line 4070 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_CONCURRENT_INSERT; }
    break;

  case 1029:
#line 4071 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_LOW_PRIORITY; }
    break;

  case 1030:
#line 4072 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_DELAYED; }
    break;

  case 1031:
#line 4073 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE; }
    break;

  case 1032:
#line 4077 "sql_yacc.yy"
    { yyval.lock_type= yyvsp[0].lock_type; }
    break;

  case 1033:
#line 4078 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_DELAYED; }
    break;

  case 1034:
#line 4081 "sql_yacc.yy"
    {}
    break;

  case 1035:
#line 4082 "sql_yacc.yy"
    {}
    break;

  case 1036:
#line 4086 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->field_list.empty();
	  lex->many_values.empty();
	  lex->insert_list=0;
	}
    break;

  case 1037:
#line 4094 "sql_yacc.yy"
    {}
    break;

  case 1038:
#line 4095 "sql_yacc.yy"
    {}
    break;

  case 1039:
#line 4096 "sql_yacc.yy"
    {}
    break;

  case 1040:
#line 4098 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (!(lex->insert_list = new List_item) ||
		lex->many_values.push_back(lex->insert_list))
	      YYABORT;
	   }
    break;

  case 1042:
#line 4107 "sql_yacc.yy"
    { }
    break;

  case 1043:
#line 4108 "sql_yacc.yy"
    { }
    break;

  case 1044:
#line 4109 "sql_yacc.yy"
    { }
    break;

  case 1045:
#line 4112 "sql_yacc.yy"
    { Lex->field_list.push_back(yyvsp[0].item); }
    break;

  case 1046:
#line 4113 "sql_yacc.yy"
    { Lex->field_list.push_back(yyvsp[0].item); }
    break;

  case 1047:
#line 4116 "sql_yacc.yy"
    {}
    break;

  case 1048:
#line 4117 "sql_yacc.yy"
    {}
    break;

  case 1049:
#line 4118 "sql_yacc.yy"
    { Select->set_braces(0);}
    break;

  case 1050:
#line 4118 "sql_yacc.yy"
    {}
    break;

  case 1051:
#line 4119 "sql_yacc.yy"
    { Select->set_braces(1);}
    break;

  case 1052:
#line 4119 "sql_yacc.yy"
    {}
    break;

  case 1057:
#line 4133 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->field_list.push_back(yyvsp[-2].item) ||
	      lex->insert_list->push_back(yyvsp[0].item))
	    YYABORT;
	 }
    break;

  case 1058:
#line 4140 "sql_yacc.yy"
    {}
    break;

  case 1059:
#line 4141 "sql_yacc.yy"
    {}
    break;

  case 1060:
#line 4145 "sql_yacc.yy"
    {}
    break;

  case 1061:
#line 4146 "sql_yacc.yy"
    {}
    break;

  case 1062:
#line 4151 "sql_yacc.yy"
    {
	    if (!(Lex->insert_list = new List_item))
	      YYABORT;
	 }
    break;

  case 1063:
#line 4156 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->many_values.push_back(lex->insert_list))
	    YYABORT;
	 }
    break;

  case 1064:
#line 4163 "sql_yacc.yy"
    {}
    break;

  case 1066:
#line 4168 "sql_yacc.yy"
    {
	  if (Lex->insert_list->push_back(yyvsp[0].item))
	    YYABORT;
	}
    break;

  case 1067:
#line 4173 "sql_yacc.yy"
    {
	    if (Lex->insert_list->push_back(yyvsp[0].item))
	      YYABORT;
	  }
    break;

  case 1068:
#line 4180 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item;}
    break;

  case 1069:
#line 4181 "sql_yacc.yy"
    {yyval.item= new Item_default_value(); }
    break;

  case 1071:
#line 4186 "sql_yacc.yy"
    { Lex->duplicates= DUP_UPDATE; }
    break;

  case 1073:
#line 4194 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  mysql_init_select(lex);
          lex->sql_command= SQLCOM_UPDATE;
	  lex->lock_option= TL_UNLOCK; 	/* Will be set later */
	  lex->duplicates= DUP_ERROR; 
        }
    break;

  case 1074:
#line 4203 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
          if (lex->select_lex.table_list.elements > 1)
	  {
            lex->sql_command= SQLCOM_UPDATE_MULTI;
	    lex->multi_lock_option= yyvsp[-4].lock_type;
	  }
	  else if (lex->select_lex.get_table_list()->derived)
	  {
	    /* it is single table update and it is update of derived table */
	    net_printf(lex->thd, ER_NON_UPDATABLE_TABLE,
		       lex->select_lex.get_table_list()->alias, "UPDATE");
	    YYABORT;
	  }
	  else
	    Select->set_lock_for_tables(yyvsp[-4].lock_type);
	}
    break;

  case 1075:
#line 4220 "sql_yacc.yy"
    {}
    break;

  case 1078:
#line 4229 "sql_yacc.yy"
    {
	  if (add_item_to_list(YYTHD, yyvsp[-2].item) || add_value_to_list(YYTHD, yyvsp[0].item))
	    YYABORT;
	}
    break;

  case 1081:
#line 4240 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  if (lex->update_list.push_back(yyvsp[-2].item) || 
	      lex->value_list.push_back(yyvsp[0].item))
	    YYABORT;
	}
    break;

  case 1082:
#line 4248 "sql_yacc.yy"
    { yyval.lock_type= YYTHD->update_lock_default; }
    break;

  case 1083:
#line 4249 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_LOW_PRIORITY; }
    break;

  case 1084:
#line 4255 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  lex->sql_command= SQLCOM_DELETE;
          mysql_init_select(lex);
	  lex->lock_option= lex->thd->update_lock_default;
	  lex->ignore= 0;
	  lex->select_lex.init_order();
	}
    break;

  case 1085:
#line 4263 "sql_yacc.yy"
    {}
    break;

  case 1086:
#line 4268 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, yyvsp[0].table, NULL, TL_OPTION_UPDATING,
					 Lex->lock_option))
	    YYABORT;
	}
    break;

  case 1087:
#line 4274 "sql_yacc.yy"
    {}
    break;

  case 1088:
#line 4276 "sql_yacc.yy"
    { mysql_init_multi_delete(Lex); }
    break;

  case 1090:
#line 4279 "sql_yacc.yy"
    { mysql_init_multi_delete(Lex); }
    break;

  case 1091:
#line 4281 "sql_yacc.yy"
    {}
    break;

  case 1092:
#line 4285 "sql_yacc.yy"
    {}
    break;

  case 1093:
#line 4286 "sql_yacc.yy"
    {}
    break;

  case 1094:
#line 4290 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, new Table_ident(yyvsp[-2].lex_str), yyvsp[0].lex_str_ptr,
					 TL_OPTION_UPDATING, Lex->lock_option))
	    YYABORT;
        }
    break;

  case 1095:
#line 4296 "sql_yacc.yy"
    {
	    if (!Select->add_table_to_list(YYTHD,
					   new Table_ident(YYTHD, yyvsp[-4].lex_str, yyvsp[-2].lex_str, 0),
					   yyvsp[0].lex_str_ptr, TL_OPTION_UPDATING,
					   Lex->lock_option))
	      YYABORT;
	  }
    break;

  case 1096:
#line 4306 "sql_yacc.yy"
    {}
    break;

  case 1097:
#line 4307 "sql_yacc.yy"
    {}
    break;

  case 1098:
#line 4311 "sql_yacc.yy"
    {}
    break;

  case 1099:
#line 4312 "sql_yacc.yy"
    {}
    break;

  case 1100:
#line 4315 "sql_yacc.yy"
    { Select->options|= OPTION_QUICK; }
    break;

  case 1101:
#line 4316 "sql_yacc.yy"
    { Lex->lock_option= TL_WRITE_LOW_PRIORITY; }
    break;

  case 1102:
#line 4317 "sql_yacc.yy"
    { Lex->ignore= 1; }
    break;

  case 1103:
#line 4321 "sql_yacc.yy"
    {
	  LEX* lex= Lex;
	  lex->sql_command= SQLCOM_TRUNCATE;
	  lex->select_lex.options= 0;
	  lex->select_lex.init_order();
	}
    break;

  case 1106:
#line 4336 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->wild=0;
	  bzero((char*) &lex->create_info,sizeof(lex->create_info));
	}
    break;

  case 1107:
#line 4342 "sql_yacc.yy"
    {}
    break;

  case 1108:
#line 4347 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_DATABASES; }
    break;

  case 1109:
#line 4349 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->sql_command= SQLCOM_SHOW_TABLES;
	    lex->select_lex.db= yyvsp[-1].simple_string;
	   }
    break;

  case 1110:
#line 4355 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->sql_command= SQLCOM_SHOW_TABLES;
	    lex->describe= DESCRIBE_EXTENDED;
	    lex->select_lex.db= yyvsp[-1].simple_string;
	  }
    break;

  case 1111:
#line 4362 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->sql_command= SQLCOM_SHOW_OPEN_TABLES;
	    lex->select_lex.db= yyvsp[-1].simple_string;
	  }
    break;

  case 1112:
#line 4368 "sql_yacc.yy"
    { Lex->create_info.db_type= yyvsp[0].db_type; }
    break;

  case 1114:
#line 4371 "sql_yacc.yy"
    {
	    Lex->sql_command= SQLCOM_SHOW_FIELDS;
	    if (yyvsp[-1].simple_string)
	      yyvsp[-2].table->change_db(yyvsp[-1].simple_string);
	    if (!Select->add_table_to_list(YYTHD, yyvsp[-2].table, NULL, 0))
	      YYABORT;
	  }
    break;

  case 1115:
#line 4382 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_NEW_MASTER;
	    Lex->mi.log_file_name = yyvsp[-8].lex_str.str;
	    Lex->mi.pos = yyvsp[-4].ulonglong_number;
	    Lex->mi.server_id = yyvsp[0].ulong_num;
          }
    break;

  case 1116:
#line 4389 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_BINLOGS;
          }
    break;

  case 1117:
#line 4393 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_SLAVE_HOSTS;
          }
    break;

  case 1118:
#line 4397 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->sql_command= SQLCOM_SHOW_BINLOG_EVENTS;
          }
    break;

  case 1120:
#line 4402 "sql_yacc.yy"
    {
	    Lex->sql_command= SQLCOM_SHOW_KEYS;
	    if (yyvsp[0].simple_string)
	      yyvsp[-1].table->change_db(yyvsp[0].simple_string);
	    if (!Select->add_table_to_list(YYTHD, yyvsp[-1].table, NULL, 0))
	      YYABORT;
	  }
    break;

  case 1121:
#line 4410 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_COLUMN_TYPES;
	  }
    break;

  case 1122:
#line 4415 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_STORAGE_ENGINES;
	    WARN_DEPRECATED("SHOW TABLE TYPES", "SHOW [STORAGE] ENGINES");
	  }
    break;

  case 1123:
#line 4421 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_STORAGE_ENGINES;
	  }
    break;

  case 1124:
#line 4426 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_PRIVILEGES;
	  }
    break;

  case 1125:
#line 4431 "sql_yacc.yy"
    { (void) create_select_for_variable("warning_count"); }
    break;

  case 1126:
#line 4433 "sql_yacc.yy"
    { (void) create_select_for_variable("error_count"); }
    break;

  case 1127:
#line 4435 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_SHOW_WARNS;}
    break;

  case 1128:
#line 4437 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_SHOW_ERRORS;}
    break;

  case 1129:
#line 4439 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_STATUS; }
    break;

  case 1130:
#line 4441 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_SHOW_INNODB_STATUS; WARN_DEPRECATED("SHOW INNODB STATUS", "SHOW ENGINE INNODB STATUS"); }
    break;

  case 1131:
#line 4443 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_PROCESSLIST;}
    break;

  case 1132:
#line 4445 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    thd->lex->sql_command= SQLCOM_SHOW_VARIABLES;
	    thd->lex->option_type= (enum_var_type) yyvsp[-2].num;
	  }
    break;

  case 1133:
#line 4451 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_CHARSETS; }
    break;

  case 1134:
#line 4453 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_COLLATIONS; }
    break;

  case 1135:
#line 4455 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_LOGS; WARN_DEPRECATED("SHOW BDB LOGS", "SHOW ENGINE BDB LOGS"); }
    break;

  case 1136:
#line 4457 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_LOGS; WARN_DEPRECATED("SHOW LOGS", "SHOW ENGINE BDB LOGS"); }
    break;

  case 1137:
#line 4459 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_GRANTS;
	    THD *thd= lex->thd;
	    LEX_USER *curr_user;
            if (!(curr_user= (LEX_USER*) thd->alloc(sizeof(st_lex_user))))
              YYABORT;
            curr_user->user.str= thd->priv_user;
            curr_user->user.length= strlen(thd->priv_user);
            if (*thd->priv_host != 0)
            {
              curr_user->host.str= thd->priv_host;
              curr_user->host.length= strlen(thd->priv_host);
            }
            else
            {
              curr_user->host.str= (char *) "%";
              curr_user->host.length= 1;
            }
            curr_user->password.str=NullS;
	    lex->grant_user= curr_user;
	  }
    break;

  case 1138:
#line 4482 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_GRANTS;
	    lex->grant_user=yyvsp[0].lex_user;
	    lex->grant_user->password.str=NullS;
	  }
    break;

  case 1139:
#line 4489 "sql_yacc.yy"
    {
	    Lex->sql_command=SQLCOM_SHOW_CREATE_DB;
	    Lex->create_info.options=yyvsp[-1].num;
	    Lex->name=yyvsp[0].lex_str.str;
	  }
    break;

  case 1140:
#line 4495 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_CREATE;
	    if (!Select->add_table_to_list(YYTHD, yyvsp[0].table, NULL,0))
	      YYABORT;
	  }
    break;

  case 1141:
#line 4501 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_MASTER_STAT;
          }
    break;

  case 1142:
#line 4505 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_SLAVE_STAT;
          }
    break;

  case 1143:
#line 4511 "sql_yacc.yy"
    {
	    switch (Lex->create_info.db_type) {
	    case DB_TYPE_INNODB:
	      Lex->sql_command = SQLCOM_SHOW_INNODB_STATUS;
	      break;
	    default:
	      net_printf(YYTHD, ER_NOT_SUPPORTED_YET, "STATUS");
	      YYABORT;
	    }
	  }
    break;

  case 1144:
#line 4522 "sql_yacc.yy"
    {
	    switch (Lex->create_info.db_type) {
	    case DB_TYPE_BERKELEY_DB:
	      Lex->sql_command = SQLCOM_SHOW_LOGS;
	      break;
	    default:
	      net_printf(YYTHD, ER_NOT_SUPPORTED_YET, "LOGS");
	      YYABORT;
	    }
	  }
    break;

  case 1149:
#line 4542 "sql_yacc.yy"
    { yyval.simple_string= 0; }
    break;

  case 1150:
#line 4543 "sql_yacc.yy"
    { yyval.simple_string= yyvsp[0].lex_str.str; }
    break;

  case 1152:
#line 4548 "sql_yacc.yy"
    { Lex->wild=  new (YYTHD->mem_root) String(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length,
                                                      system_charset_info); }
    break;

  case 1153:
#line 4552 "sql_yacc.yy"
    { Lex->verbose=0; }
    break;

  case 1154:
#line 4553 "sql_yacc.yy"
    { Lex->verbose=1; }
    break;

  case 1157:
#line 4560 "sql_yacc.yy"
    { Lex->mi.log_file_name = 0; }
    break;

  case 1158:
#line 4561 "sql_yacc.yy"
    { Lex->mi.log_file_name = yyvsp[0].lex_str.str; }
    break;

  case 1159:
#line 4564 "sql_yacc.yy"
    { Lex->mi.pos = 4; /* skip magic number */ }
    break;

  case 1160:
#line 4565 "sql_yacc.yy"
    { Lex->mi.pos = yyvsp[0].ulonglong_number; }
    break;

  case 1161:
#line 4571 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->wild=0;
	  lex->verbose=0;
	  lex->sql_command=SQLCOM_SHOW_FIELDS;
	  if (!Select->add_table_to_list(lex->thd, yyvsp[0].table, NULL,0))
	    YYABORT;
	}
    break;

  case 1162:
#line 4579 "sql_yacc.yy"
    {}
    break;

  case 1163:
#line 4581 "sql_yacc.yy"
    { Lex->describe|= DESCRIBE_NORMAL; }
    break;

  case 1164:
#line 4583 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->select_lex.options|= SELECT_DESCRIBE;
	  }
    break;

  case 1167:
#line 4594 "sql_yacc.yy"
    {}
    break;

  case 1168:
#line 4595 "sql_yacc.yy"
    { Lex->describe|= DESCRIBE_EXTENDED; }
    break;

  case 1169:
#line 4599 "sql_yacc.yy"
    {}
    break;

  case 1170:
#line 4600 "sql_yacc.yy"
    { Lex->wild= yyvsp[0].string; }
    break;

  case 1171:
#line 4602 "sql_yacc.yy"
    { Lex->wild= new (YYTHD->mem_root) String((const char*) yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,system_charset_info); }
    break;

  case 1172:
#line 4609 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_FLUSH; lex->type=0;
          lex->no_write_to_binlog= yyvsp[0].num;
	}
    break;

  case 1173:
#line 4615 "sql_yacc.yy"
    {}
    break;

  case 1176:
#line 4623 "sql_yacc.yy"
    { Lex->type|= REFRESH_TABLES; }
    break;

  case 1177:
#line 4623 "sql_yacc.yy"
    {}
    break;

  case 1178:
#line 4624 "sql_yacc.yy"
    { Lex->type|= REFRESH_TABLES | REFRESH_READ_LOCK; }
    break;

  case 1179:
#line 4625 "sql_yacc.yy"
    { Lex->type|= REFRESH_QUERY_CACHE_FREE; }
    break;

  case 1180:
#line 4626 "sql_yacc.yy"
    { Lex->type|= REFRESH_HOSTS; }
    break;

  case 1181:
#line 4627 "sql_yacc.yy"
    { Lex->type|= REFRESH_GRANT; }
    break;

  case 1182:
#line 4628 "sql_yacc.yy"
    { Lex->type|= REFRESH_LOG; }
    break;

  case 1183:
#line 4629 "sql_yacc.yy"
    { Lex->type|= REFRESH_STATUS; }
    break;

  case 1184:
#line 4630 "sql_yacc.yy"
    { Lex->type|= REFRESH_SLAVE; }
    break;

  case 1185:
#line 4631 "sql_yacc.yy"
    { Lex->type|= REFRESH_MASTER; }
    break;

  case 1186:
#line 4632 "sql_yacc.yy"
    { Lex->type|= REFRESH_DES_KEY_FILE; }
    break;

  case 1187:
#line 4633 "sql_yacc.yy"
    { Lex->type|= REFRESH_USER_RESOURCES; }
    break;

  case 1188:
#line 4636 "sql_yacc.yy"
    {;}
    break;

  case 1189:
#line 4637 "sql_yacc.yy"
    {;}
    break;

  case 1190:
#line 4641 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_RESET; lex->type=0;
	}
    break;

  case 1191:
#line 4645 "sql_yacc.yy"
    {}
    break;

  case 1194:
#line 4653 "sql_yacc.yy"
    { Lex->type|= REFRESH_SLAVE; }
    break;

  case 1195:
#line 4654 "sql_yacc.yy"
    { Lex->type|= REFRESH_MASTER; }
    break;

  case 1196:
#line 4655 "sql_yacc.yy"
    { Lex->type|= REFRESH_QUERY_CACHE;}
    break;

  case 1197:
#line 4659 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->type=0;
	}
    break;

  case 1198:
#line 4663 "sql_yacc.yy"
    {}
    break;

  case 1200:
#line 4672 "sql_yacc.yy"
    {
	   Lex->sql_command = SQLCOM_PURGE;
	   Lex->to_log = yyvsp[0].lex_str.str;
        }
    break;

  case 1201:
#line 4677 "sql_yacc.yy"
    {
	  if (yyvsp[0].item->check_cols(1) || yyvsp[0].item->fix_fields(Lex->thd, 0, &yyvsp[0].item))
	  {
	    net_printf(Lex->thd, ER_WRONG_ARGUMENTS, "PURGE LOGS BEFORE");
	    YYABORT;
	  }
	  Item *tmp= new Item_func_unix_timestamp(yyvsp[0].item);
	  /*
	    it is OK only emulate fix_fieds, because we need only
            value of constant
	  */
	  tmp->quick_fix_field();
	  Lex->sql_command = SQLCOM_PURGE_BEFORE;
	  Lex->purge_time= (ulong) tmp->val_int();
	}
    break;

  case 1202:
#line 4698 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (yyvsp[0].item->fix_fields(lex->thd, 0, &yyvsp[0].item) || yyvsp[0].item->check_cols(1))
	  {
	    send_error(lex->thd, ER_SET_CONSTANTS_ONLY);
	    YYABORT;
	  }
          lex->sql_command=SQLCOM_KILL;
	  lex->thread_id= (ulong) yyvsp[0].item->val_int();
	}
    break;

  case 1203:
#line 4712 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command=SQLCOM_CHANGE_DB;
	  lex->select_lex.db= yyvsp[0].lex_str.str;
	}
    break;

  case 1204:
#line 4721 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_LOAD;
	  lex->lock_option= yyvsp[-3].lock_type;
	  lex->local_file=  yyvsp[-2].num;
	  lex->duplicates= DUP_ERROR;
	  lex->ignore= 0;
	  if (!(lex->exchange= new sql_exchange(yyvsp[0].lex_str.str,0)))
	    YYABORT;
	  lex->field_list.empty();
	}
    break;

  case 1205:
#line 4734 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, yyvsp[-4].table, NULL, TL_OPTION_UPDATING))
	    YYABORT;
	}
    break;

  case 1206:
#line 4740 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_LOAD_MASTER_TABLE;
	  if (!Select->add_table_to_list(YYTHD, yyvsp[-2].table, NULL, TL_OPTION_UPDATING))
	    YYABORT;

        }
    break;

  case 1207:
#line 4748 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_LOAD_MASTER_DATA;
        }
    break;

  case 1208:
#line 4753 "sql_yacc.yy"
    { yyval.num=0;}
    break;

  case 1209:
#line 4754 "sql_yacc.yy"
    { yyval.num=1;}
    break;

  case 1210:
#line 4757 "sql_yacc.yy"
    { yyval.lock_type= YYTHD->update_lock_default; }
    break;

  case 1211:
#line 4758 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_CONCURRENT_INSERT ; }
    break;

  case 1212:
#line 4759 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_LOW_PRIORITY; }
    break;

  case 1213:
#line 4763 "sql_yacc.yy"
    { Lex->duplicates=DUP_ERROR; }
    break;

  case 1214:
#line 4764 "sql_yacc.yy"
    { Lex->duplicates=DUP_REPLACE; }
    break;

  case 1215:
#line 4765 "sql_yacc.yy"
    { Lex->ignore= 1; }
    break;

  case 1220:
#line 4777 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->field_term= yyvsp[0].string;
          }
    break;

  case 1221:
#line 4782 "sql_yacc.yy"
    {
            LEX *lex= Lex;
            DBUG_ASSERT(lex->exchange);
            lex->exchange->enclosed= yyvsp[0].string;
            lex->exchange->opt_enclosed= 1;
	  }
    break;

  case 1222:
#line 4789 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->enclosed= yyvsp[0].string;
          }
    break;

  case 1223:
#line 4794 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->escaped= yyvsp[0].string;
          }
    break;

  case 1228:
#line 4809 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->line_term= yyvsp[0].string;
          }
    break;

  case 1229:
#line 4814 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->line_start= yyvsp[0].string;
          }
    break;

  case 1231:
#line 4822 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->skip_lines= atol(yyvsp[-1].lex_str.str);
          }
    break;

  case 1232:
#line 4831 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  yyval.item = new Item_string(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,thd->variables.collation_connection);
	}
    break;

  case 1233:
#line 4836 "sql_yacc.yy"
    { yyval.item=  new Item_string(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,national_charset_info); }
    break;

  case 1234:
#line 4838 "sql_yacc.yy"
    { yyval.item = new Item_string(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,Lex->charset); }
    break;

  case 1235:
#line 4840 "sql_yacc.yy"
    { ((Item_string*) yyvsp[-1].item)->append(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length); }
    break;

  case 1236:
#line 4845 "sql_yacc.yy"
    { yyval.string=  new (YYTHD->mem_root) String(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,YYTHD->variables.collation_connection); }
    break;

  case 1237:
#line 4847 "sql_yacc.yy"
    {
	    Item *tmp = new Item_varbinary(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length);
	    /*
	      it is OK only emulate fix_fieds, because we need only
              value of constant
	    */
	    yyval.string= tmp ?
	      tmp->quick_fix_field(), tmp->val_str((String*) 0) :
	      (String*) 0;
	  }
    break;

  case 1238:
#line 4861 "sql_yacc.yy"
    {
          THD *thd=YYTHD;
	  LEX *lex= thd->lex;
          if (thd->command == COM_PREPARE)
          {
            Item_param *item= new Item_param((uint) (lex->tok_start -
                                                     (uchar *) thd->query));
            if (!(yyval.item= item) || lex->param_list.push_back(item))
            {
	      send_error(thd, ER_OUT_OF_RESOURCES);
	      YYABORT;
            }
          }
          else
          {
            yyerror(ER(ER_SYNTAX_ERROR));
            YYABORT;
          }
        }
    break;

  case 1239:
#line 4883 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 1240:
#line 4884 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item_num; }
    break;

  case 1241:
#line 4886 "sql_yacc.yy"
    {
	    yyvsp[0].item_num->max_length++;
	    yyval.item= yyvsp[0].item_num->neg();
	  }
    break;

  case 1242:
#line 4894 "sql_yacc.yy"
    { yyval.item =	yyvsp[0].item; }
    break;

  case 1243:
#line 4895 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item_num; }
    break;

  case 1244:
#line 4896 "sql_yacc.yy"
    { yyval.item =	new Item_null();
			  Lex->next_state=MY_LEX_OPERATOR_OR_IDENT;}
    break;

  case 1245:
#line 4898 "sql_yacc.yy"
    { yyval.item= new Item_int((char*) "FALSE",0,1); }
    break;

  case 1246:
#line 4899 "sql_yacc.yy"
    { yyval.item= new Item_int((char*) "TRUE",1,1); }
    break;

  case 1247:
#line 4900 "sql_yacc.yy"
    { yyval.item =	new Item_varbinary(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length);}
    break;

  case 1248:
#line 4902 "sql_yacc.yy"
    {
	    Item *tmp= new Item_varbinary(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length);
	    /*
	      it is OK only emulate fix_fieds, because we need only
              value of constant
	    */
	    String *str= tmp ?
	      tmp->quick_fix_field(), tmp->val_str((String*) 0) :
	      (String*) 0;
	    yyval.item= new Item_string(str ? str->ptr() : "",
				str ? str->length() : 0,
				Lex->charset);
	  }
    break;

  case 1249:
#line 4915 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 1250:
#line 4916 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 1251:
#line 4917 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 1252:
#line 4920 "sql_yacc.yy"
    { int error; yyval.item_num = new Item_int(yyvsp[0].lex_str.str, (longlong) my_strtoll10(yyvsp[0].lex_str.str, NULL, &error), yyvsp[0].lex_str.length); }
    break;

  case 1253:
#line 4921 "sql_yacc.yy"
    { int error; yyval.item_num = new Item_int(yyvsp[0].lex_str.str, (longlong) my_strtoll10(yyvsp[0].lex_str.str, NULL, &error), yyvsp[0].lex_str.length); }
    break;

  case 1254:
#line 4922 "sql_yacc.yy"
    { yyval.item_num =	new Item_uint(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length); }
    break;

  case 1255:
#line 4923 "sql_yacc.yy"
    { yyval.item_num =	new Item_real(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length); }
    break;

  case 1256:
#line 4924 "sql_yacc.yy"
    { yyval.item_num =	new Item_float(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length); }
    break;

  case 1257:
#line 4932 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 1258:
#line 4933 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 1259:
#line 4937 "sql_yacc.yy"
    {
	  yyval.item = new Item_field(NullS,yyvsp[-2].lex_str.str,"*");
	  Lex->current_select->with_wild++;
	}
    break;

  case 1260:
#line 4942 "sql_yacc.yy"
    {
	  yyval.item = new Item_field((YYTHD->client_capabilities &
   			     CLIENT_NO_SCHEMA ? NullS : yyvsp[-4].lex_str.str),
			     yyvsp[-2].lex_str.str,"*");
	  Lex->current_select->with_wild++;
	}
    break;

  case 1261:
#line 4951 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 1262:
#line 4955 "sql_yacc.yy"
    {
	  SELECT_LEX *sel=Select;
	  yyval.item= (sel->parsing_place != IN_HAVING ||
	       sel->get_in_sum_expr() > 0) ?
              (Item*) new Item_field(NullS,NullS,yyvsp[0].lex_str.str) :
	      (Item*) new Item_ref(NullS, NullS, yyvsp[0].lex_str.str);
	}
    break;

  case 1263:
#line 4963 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  LEX *lex= thd->lex;
	  SELECT_LEX *sel= lex->current_select;
	  if (sel->no_table_names_allowed)
	  {
	    my_printf_error(ER_TABLENAME_NOT_ALLOWED_HERE,
			    ER(ER_TABLENAME_NOT_ALLOWED_HERE),
			    MYF(0), yyvsp[-2].lex_str.str, thd->where);
	  }
	  yyval.item= (sel->parsing_place != IN_HAVING ||
	       sel->get_in_sum_expr() > 0) ?
	      (Item*) new Item_field(NullS,yyvsp[-2].lex_str.str,yyvsp[0].lex_str.str) :
	      (Item*) new Item_ref(NullS, yyvsp[-2].lex_str.str, yyvsp[0].lex_str.str);
	}
    break;

  case 1264:
#line 4979 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  LEX *lex= thd->lex;
	  SELECT_LEX *sel= lex->current_select;
	  if (sel->no_table_names_allowed)
	  {
	    my_printf_error(ER_TABLENAME_NOT_ALLOWED_HERE,
			    ER(ER_TABLENAME_NOT_ALLOWED_HERE),
			    MYF(0), yyvsp[-2].lex_str.str, thd->where);
	  }
	  yyval.item= (sel->parsing_place != IN_HAVING ||
	       sel->get_in_sum_expr() > 0) ?
	      (Item*) new Item_field(NullS,yyvsp[-2].lex_str.str,yyvsp[0].lex_str.str) :
              (Item*) new Item_ref(NullS, yyvsp[-2].lex_str.str, yyvsp[0].lex_str.str);
	}
    break;

  case 1265:
#line 4995 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  LEX *lex= thd->lex;
	  SELECT_LEX *sel= lex->current_select;
	  if (sel->no_table_names_allowed)
	  {
	    my_printf_error(ER_TABLENAME_NOT_ALLOWED_HERE,
			    ER(ER_TABLENAME_NOT_ALLOWED_HERE),
			    MYF(0), yyvsp[-2].lex_str.str, thd->where);
	  }
	  yyval.item= (sel->parsing_place != IN_HAVING ||
	       sel->get_in_sum_expr() > 0) ?
	      (Item*) new Item_field((YYTHD->client_capabilities &
				      CLIENT_NO_SCHEMA ? NullS : yyvsp[-4].lex_str.str),
				     yyvsp[-2].lex_str.str, yyvsp[0].lex_str.str) :
	      (Item*) new Item_ref((YYTHD->client_capabilities &
				    CLIENT_NO_SCHEMA ? NullS : yyvsp[-4].lex_str.str),
                                   yyvsp[-2].lex_str.str, yyvsp[0].lex_str.str);
	}
    break;

  case 1266:
#line 5017 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1267:
#line 5019 "sql_yacc.yy"
    {
            TABLE_LIST *table= (TABLE_LIST*) Select->table_list.first;
            if (my_strcasecmp(table_alias_charset, yyvsp[-4].lex_str.str, table->db))
            {
              net_printf(YYTHD, ER_WRONG_DB_NAME, yyvsp[-4].lex_str.str);
              YYABORT;
            }
            if (my_strcasecmp(table_alias_charset, yyvsp[-2].lex_str.str, table->real_name))
            {
              net_printf(YYTHD, ER_WRONG_TABLE_NAME, yyvsp[-2].lex_str.str);
              YYABORT;
            }
            yyval.lex_str=yyvsp[0].lex_str;
          }
    break;

  case 1268:
#line 5034 "sql_yacc.yy"
    {
            TABLE_LIST *table= (TABLE_LIST*) Select->table_list.first;
            if (my_strcasecmp(table_alias_charset, yyvsp[-2].lex_str.str, table->alias))
            {
              net_printf(YYTHD, ER_WRONG_TABLE_NAME, yyvsp[-2].lex_str.str);
              YYABORT;
            }
            yyval.lex_str=yyvsp[0].lex_str;
          }
    break;

  case 1269:
#line 5043 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1270:
#line 5046 "sql_yacc.yy"
    { yyval.table=new Table_ident(yyvsp[0].lex_str); }
    break;

  case 1271:
#line 5047 "sql_yacc.yy"
    { yyval.table=new Table_ident(YYTHD, yyvsp[-2].lex_str,yyvsp[0].lex_str,0);}
    break;

  case 1272:
#line 5048 "sql_yacc.yy"
    { yyval.table=new Table_ident(yyvsp[0].lex_str);}
    break;

  case 1273:
#line 5052 "sql_yacc.yy"
    { LEX_STRING db={(char*) any_db,3}; yyval.table=new Table_ident(YYTHD, db,yyvsp[0].lex_str,0); }
    break;

  case 1274:
#line 5056 "sql_yacc.yy"
    { yyval.lex_str= yyvsp[0].lex_str; }
    break;

  case 1275:
#line 5058 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    if (thd->charset_is_system_charset)
            {
              CHARSET_INFO *cs= system_charset_info;
              int dummy_error;
              uint wlen= cs->cset->well_formed_len(cs, yyvsp[0].lex_str.str,
                                                   yyvsp[0].lex_str.str+yyvsp[0].lex_str.length,
                                                   yyvsp[0].lex_str.length, &dummy_error);
              if (wlen < yyvsp[0].lex_str.length)
              {
                net_printf(YYTHD, ER_INVALID_CHARACTER_STRING, cs->csname,
                           yyvsp[0].lex_str.str + wlen);
                YYABORT;
              }
	      yyval.lex_str= yyvsp[0].lex_str;
            }
	    else
	      thd->convert_string(&yyval.lex_str, system_charset_info,
				  yyvsp[0].lex_str.str, yyvsp[0].lex_str.length, thd->charset());
	  }
    break;

  case 1276:
#line 5083 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  if (thd->charset_is_system_charset)
	    yyval.lex_str= yyvsp[0].lex_str;
	  else
	    thd->convert_string(&yyval.lex_str, system_charset_info,
				yyvsp[0].lex_str.str, yyvsp[0].lex_str.length, thd->charset());
	}
    break;

  case 1277:
#line 5095 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  if (thd->charset_is_collation_connection)
	    yyval.lex_str= yyvsp[0].lex_str;
	  else
	    thd->convert_string(&yyval.lex_str, thd->variables.collation_connection,
				yyvsp[0].lex_str.str, yyvsp[0].lex_str.length, thd->charset());
	}
    break;

  case 1278:
#line 5107 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 1279:
#line 5109 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  yyval.lex_str.str=    thd->strmake(yyvsp[0].symbol.str, yyvsp[0].symbol.length);
	  yyval.lex_str.length= yyvsp[0].symbol.length;
	}
    break;

  case 1280:
#line 5117 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1281:
#line 5118 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1282:
#line 5119 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1283:
#line 5123 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  if (!(yyval.lex_user=(LEX_USER*) thd->alloc(sizeof(st_lex_user))))
	    YYABORT;
	  yyval.lex_user->user = yyvsp[0].lex_str;
	  yyval.lex_user->host.str= (char *) "%";
	  yyval.lex_user->host.length= 1;
	}
    break;

  case 1284:
#line 5132 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    if (!(yyval.lex_user=(LEX_USER*) thd->alloc(sizeof(st_lex_user))))
	      YYABORT;
	    yyval.lex_user->user = yyvsp[-2].lex_str; yyval.lex_user->host=yyvsp[0].lex_str;
	  }
    break;

  case 1285:
#line 5139 "sql_yacc.yy"
    {
          THD *thd= YYTHD;
          if (!(yyval.lex_user=(LEX_USER*) thd->alloc(sizeof(st_lex_user))))
            YYABORT;
          yyval.lex_user->user.str= thd->priv_user;
          yyval.lex_user->user.length= strlen(thd->priv_user);
          if (*thd->priv_host != 0)
          {
            yyval.lex_user->host.str= thd->priv_host;
            yyval.lex_user->host.length= strlen(thd->priv_host);
          }
          else
          {
            yyval.lex_user->host.str= (char *) "%";
            yyval.lex_user->host.length= 1;
          }
	}
    break;

  case 1286:
#line 5160 "sql_yacc.yy"
    {}
    break;

  case 1287:
#line 5161 "sql_yacc.yy"
    {}
    break;

  case 1288:
#line 5162 "sql_yacc.yy"
    {}
    break;

  case 1289:
#line 5163 "sql_yacc.yy"
    {}
    break;

  case 1290:
#line 5164 "sql_yacc.yy"
    {}
    break;

  case 1291:
#line 5165 "sql_yacc.yy"
    {}
    break;

  case 1292:
#line 5166 "sql_yacc.yy"
    {}
    break;

  case 1293:
#line 5167 "sql_yacc.yy"
    {}
    break;

  case 1294:
#line 5168 "sql_yacc.yy"
    {}
    break;

  case 1295:
#line 5169 "sql_yacc.yy"
    {}
    break;

  case 1296:
#line 5170 "sql_yacc.yy"
    {}
    break;

  case 1297:
#line 5171 "sql_yacc.yy"
    {}
    break;

  case 1298:
#line 5172 "sql_yacc.yy"
    {}
    break;

  case 1299:
#line 5173 "sql_yacc.yy"
    {}
    break;

  case 1300:
#line 5174 "sql_yacc.yy"
    {}
    break;

  case 1301:
#line 5175 "sql_yacc.yy"
    {}
    break;

  case 1302:
#line 5176 "sql_yacc.yy"
    {}
    break;

  case 1303:
#line 5177 "sql_yacc.yy"
    {}
    break;

  case 1304:
#line 5178 "sql_yacc.yy"
    {}
    break;

  case 1305:
#line 5179 "sql_yacc.yy"
    {}
    break;

  case 1306:
#line 5180 "sql_yacc.yy"
    {}
    break;

  case 1307:
#line 5181 "sql_yacc.yy"
    {}
    break;

  case 1308:
#line 5182 "sql_yacc.yy"
    {}
    break;

  case 1309:
#line 5183 "sql_yacc.yy"
    {}
    break;

  case 1310:
#line 5184 "sql_yacc.yy"
    {}
    break;

  case 1311:
#line 5185 "sql_yacc.yy"
    {}
    break;

  case 1312:
#line 5186 "sql_yacc.yy"
    {}
    break;

  case 1313:
#line 5187 "sql_yacc.yy"
    {}
    break;

  case 1314:
#line 5188 "sql_yacc.yy"
    {}
    break;

  case 1315:
#line 5189 "sql_yacc.yy"
    {}
    break;

  case 1316:
#line 5190 "sql_yacc.yy"
    {}
    break;

  case 1317:
#line 5191 "sql_yacc.yy"
    {}
    break;

  case 1318:
#line 5192 "sql_yacc.yy"
    {}
    break;

  case 1319:
#line 5193 "sql_yacc.yy"
    {}
    break;

  case 1320:
#line 5194 "sql_yacc.yy"
    {}
    break;

  case 1321:
#line 5195 "sql_yacc.yy"
    {}
    break;

  case 1322:
#line 5196 "sql_yacc.yy"
    {}
    break;

  case 1323:
#line 5197 "sql_yacc.yy"
    {}
    break;

  case 1324:
#line 5198 "sql_yacc.yy"
    {}
    break;

  case 1325:
#line 5199 "sql_yacc.yy"
    {}
    break;

  case 1326:
#line 5200 "sql_yacc.yy"
    {}
    break;

  case 1327:
#line 5201 "sql_yacc.yy"
    {}
    break;

  case 1328:
#line 5202 "sql_yacc.yy"
    {}
    break;

  case 1329:
#line 5203 "sql_yacc.yy"
    {}
    break;

  case 1330:
#line 5204 "sql_yacc.yy"
    {}
    break;

  case 1331:
#line 5205 "sql_yacc.yy"
    {}
    break;

  case 1332:
#line 5206 "sql_yacc.yy"
    {}
    break;

  case 1333:
#line 5207 "sql_yacc.yy"
    {}
    break;

  case 1334:
#line 5208 "sql_yacc.yy"
    {}
    break;

  case 1335:
#line 5209 "sql_yacc.yy"
    {}
    break;

  case 1336:
#line 5210 "sql_yacc.yy"
    {}
    break;

  case 1337:
#line 5211 "sql_yacc.yy"
    {}
    break;

  case 1338:
#line 5212 "sql_yacc.yy"
    {}
    break;

  case 1339:
#line 5213 "sql_yacc.yy"
    {}
    break;

  case 1340:
#line 5214 "sql_yacc.yy"
    {}
    break;

  case 1341:
#line 5215 "sql_yacc.yy"
    {}
    break;

  case 1342:
#line 5216 "sql_yacc.yy"
    {}
    break;

  case 1343:
#line 5217 "sql_yacc.yy"
    {}
    break;

  case 1344:
#line 5218 "sql_yacc.yy"
    {}
    break;

  case 1345:
#line 5219 "sql_yacc.yy"
    {}
    break;

  case 1346:
#line 5220 "sql_yacc.yy"
    {}
    break;

  case 1347:
#line 5221 "sql_yacc.yy"
    {}
    break;

  case 1348:
#line 5222 "sql_yacc.yy"
    {}
    break;

  case 1349:
#line 5223 "sql_yacc.yy"
    {}
    break;

  case 1350:
#line 5224 "sql_yacc.yy"
    {}
    break;

  case 1351:
#line 5225 "sql_yacc.yy"
    {}
    break;

  case 1352:
#line 5226 "sql_yacc.yy"
    {}
    break;

  case 1353:
#line 5227 "sql_yacc.yy"
    {}
    break;

  case 1354:
#line 5228 "sql_yacc.yy"
    {}
    break;

  case 1355:
#line 5229 "sql_yacc.yy"
    {}
    break;

  case 1356:
#line 5230 "sql_yacc.yy"
    {}
    break;

  case 1357:
#line 5231 "sql_yacc.yy"
    {}
    break;

  case 1358:
#line 5232 "sql_yacc.yy"
    {}
    break;

  case 1359:
#line 5233 "sql_yacc.yy"
    {}
    break;

  case 1360:
#line 5234 "sql_yacc.yy"
    {}
    break;

  case 1361:
#line 5235 "sql_yacc.yy"
    {}
    break;

  case 1362:
#line 5236 "sql_yacc.yy"
    {}
    break;

  case 1363:
#line 5237 "sql_yacc.yy"
    {}
    break;

  case 1364:
#line 5238 "sql_yacc.yy"
    {}
    break;

  case 1365:
#line 5239 "sql_yacc.yy"
    {}
    break;

  case 1366:
#line 5240 "sql_yacc.yy"
    {}
    break;

  case 1367:
#line 5241 "sql_yacc.yy"
    {}
    break;

  case 1368:
#line 5242 "sql_yacc.yy"
    {}
    break;

  case 1369:
#line 5243 "sql_yacc.yy"
    {}
    break;

  case 1370:
#line 5244 "sql_yacc.yy"
    {}
    break;

  case 1371:
#line 5245 "sql_yacc.yy"
    {}
    break;

  case 1372:
#line 5246 "sql_yacc.yy"
    {}
    break;

  case 1373:
#line 5247 "sql_yacc.yy"
    {}
    break;

  case 1374:
#line 5248 "sql_yacc.yy"
    {}
    break;

  case 1375:
#line 5249 "sql_yacc.yy"
    {}
    break;

  case 1376:
#line 5250 "sql_yacc.yy"
    {}
    break;

  case 1377:
#line 5251 "sql_yacc.yy"
    {}
    break;

  case 1378:
#line 5252 "sql_yacc.yy"
    {}
    break;

  case 1379:
#line 5253 "sql_yacc.yy"
    {}
    break;

  case 1380:
#line 5254 "sql_yacc.yy"
    {}
    break;

  case 1381:
#line 5255 "sql_yacc.yy"
    {}
    break;

  case 1382:
#line 5256 "sql_yacc.yy"
    {}
    break;

  case 1383:
#line 5257 "sql_yacc.yy"
    {}
    break;

  case 1384:
#line 5258 "sql_yacc.yy"
    {}
    break;

  case 1385:
#line 5259 "sql_yacc.yy"
    {}
    break;

  case 1386:
#line 5260 "sql_yacc.yy"
    {}
    break;

  case 1387:
#line 5261 "sql_yacc.yy"
    {}
    break;

  case 1388:
#line 5262 "sql_yacc.yy"
    {}
    break;

  case 1389:
#line 5263 "sql_yacc.yy"
    {}
    break;

  case 1390:
#line 5264 "sql_yacc.yy"
    {}
    break;

  case 1391:
#line 5265 "sql_yacc.yy"
    {}
    break;

  case 1392:
#line 5266 "sql_yacc.yy"
    {}
    break;

  case 1393:
#line 5267 "sql_yacc.yy"
    {}
    break;

  case 1394:
#line 5268 "sql_yacc.yy"
    {}
    break;

  case 1395:
#line 5269 "sql_yacc.yy"
    {}
    break;

  case 1396:
#line 5270 "sql_yacc.yy"
    {}
    break;

  case 1397:
#line 5271 "sql_yacc.yy"
    {}
    break;

  case 1398:
#line 5272 "sql_yacc.yy"
    {}
    break;

  case 1399:
#line 5273 "sql_yacc.yy"
    {}
    break;

  case 1400:
#line 5274 "sql_yacc.yy"
    {}
    break;

  case 1401:
#line 5275 "sql_yacc.yy"
    {}
    break;

  case 1402:
#line 5276 "sql_yacc.yy"
    {}
    break;

  case 1403:
#line 5277 "sql_yacc.yy"
    {}
    break;

  case 1404:
#line 5278 "sql_yacc.yy"
    {}
    break;

  case 1405:
#line 5279 "sql_yacc.yy"
    {}
    break;

  case 1406:
#line 5280 "sql_yacc.yy"
    {}
    break;

  case 1407:
#line 5281 "sql_yacc.yy"
    {}
    break;

  case 1408:
#line 5282 "sql_yacc.yy"
    {}
    break;

  case 1409:
#line 5283 "sql_yacc.yy"
    {}
    break;

  case 1410:
#line 5284 "sql_yacc.yy"
    {}
    break;

  case 1411:
#line 5285 "sql_yacc.yy"
    {}
    break;

  case 1412:
#line 5286 "sql_yacc.yy"
    {}
    break;

  case 1413:
#line 5287 "sql_yacc.yy"
    {}
    break;

  case 1414:
#line 5288 "sql_yacc.yy"
    {}
    break;

  case 1415:
#line 5289 "sql_yacc.yy"
    {}
    break;

  case 1416:
#line 5290 "sql_yacc.yy"
    {}
    break;

  case 1417:
#line 5291 "sql_yacc.yy"
    {}
    break;

  case 1418:
#line 5292 "sql_yacc.yy"
    {}
    break;

  case 1419:
#line 5293 "sql_yacc.yy"
    {}
    break;

  case 1420:
#line 5294 "sql_yacc.yy"
    {}
    break;

  case 1421:
#line 5295 "sql_yacc.yy"
    {}
    break;

  case 1422:
#line 5296 "sql_yacc.yy"
    {}
    break;

  case 1423:
#line 5297 "sql_yacc.yy"
    {}
    break;

  case 1424:
#line 5298 "sql_yacc.yy"
    {}
    break;

  case 1425:
#line 5299 "sql_yacc.yy"
    {}
    break;

  case 1426:
#line 5300 "sql_yacc.yy"
    {}
    break;

  case 1427:
#line 5301 "sql_yacc.yy"
    {}
    break;

  case 1428:
#line 5302 "sql_yacc.yy"
    {}
    break;

  case 1429:
#line 5303 "sql_yacc.yy"
    {}
    break;

  case 1430:
#line 5304 "sql_yacc.yy"
    {}
    break;

  case 1431:
#line 5305 "sql_yacc.yy"
    {}
    break;

  case 1432:
#line 5306 "sql_yacc.yy"
    {}
    break;

  case 1433:
#line 5307 "sql_yacc.yy"
    {}
    break;

  case 1434:
#line 5308 "sql_yacc.yy"
    {}
    break;

  case 1435:
#line 5309 "sql_yacc.yy"
    {}
    break;

  case 1436:
#line 5310 "sql_yacc.yy"
    {}
    break;

  case 1437:
#line 5311 "sql_yacc.yy"
    {}
    break;

  case 1438:
#line 5312 "sql_yacc.yy"
    {}
    break;

  case 1439:
#line 5313 "sql_yacc.yy"
    {}
    break;

  case 1440:
#line 5314 "sql_yacc.yy"
    {}
    break;

  case 1441:
#line 5315 "sql_yacc.yy"
    {}
    break;

  case 1442:
#line 5316 "sql_yacc.yy"
    {}
    break;

  case 1443:
#line 5317 "sql_yacc.yy"
    {}
    break;

  case 1444:
#line 5318 "sql_yacc.yy"
    {}
    break;

  case 1445:
#line 5319 "sql_yacc.yy"
    {}
    break;

  case 1446:
#line 5320 "sql_yacc.yy"
    {}
    break;

  case 1447:
#line 5321 "sql_yacc.yy"
    {}
    break;

  case 1448:
#line 5322 "sql_yacc.yy"
    {}
    break;

  case 1449:
#line 5323 "sql_yacc.yy"
    {}
    break;

  case 1450:
#line 5324 "sql_yacc.yy"
    {}
    break;

  case 1451:
#line 5325 "sql_yacc.yy"
    {}
    break;

  case 1452:
#line 5326 "sql_yacc.yy"
    {}
    break;

  case 1453:
#line 5327 "sql_yacc.yy"
    {}
    break;

  case 1454:
#line 5328 "sql_yacc.yy"
    {}
    break;

  case 1455:
#line 5329 "sql_yacc.yy"
    {}
    break;

  case 1456:
#line 5330 "sql_yacc.yy"
    {}
    break;

  case 1457:
#line 5331 "sql_yacc.yy"
    {}
    break;

  case 1458:
#line 5332 "sql_yacc.yy"
    {}
    break;

  case 1459:
#line 5333 "sql_yacc.yy"
    {}
    break;

  case 1460:
#line 5334 "sql_yacc.yy"
    {}
    break;

  case 1461:
#line 5335 "sql_yacc.yy"
    {}
    break;

  case 1462:
#line 5336 "sql_yacc.yy"
    {}
    break;

  case 1463:
#line 5337 "sql_yacc.yy"
    {}
    break;

  case 1464:
#line 5338 "sql_yacc.yy"
    {}
    break;

  case 1465:
#line 5339 "sql_yacc.yy"
    {}
    break;

  case 1466:
#line 5340 "sql_yacc.yy"
    {}
    break;

  case 1467:
#line 5341 "sql_yacc.yy"
    {}
    break;

  case 1468:
#line 5342 "sql_yacc.yy"
    {}
    break;

  case 1469:
#line 5343 "sql_yacc.yy"
    {}
    break;

  case 1470:
#line 5344 "sql_yacc.yy"
    {}
    break;

  case 1471:
#line 5345 "sql_yacc.yy"
    {}
    break;

  case 1472:
#line 5346 "sql_yacc.yy"
    {}
    break;

  case 1473:
#line 5347 "sql_yacc.yy"
    {}
    break;

  case 1474:
#line 5348 "sql_yacc.yy"
    {}
    break;

  case 1475:
#line 5349 "sql_yacc.yy"
    {}
    break;

  case 1476:
#line 5350 "sql_yacc.yy"
    {}
    break;

  case 1477:
#line 5351 "sql_yacc.yy"
    {}
    break;

  case 1478:
#line 5352 "sql_yacc.yy"
    {}
    break;

  case 1479:
#line 5353 "sql_yacc.yy"
    {}
    break;

  case 1480:
#line 5354 "sql_yacc.yy"
    {}
    break;

  case 1481:
#line 5355 "sql_yacc.yy"
    {}
    break;

  case 1482:
#line 5356 "sql_yacc.yy"
    {}
    break;

  case 1483:
#line 5357 "sql_yacc.yy"
    {}
    break;

  case 1484:
#line 5358 "sql_yacc.yy"
    {}
    break;

  case 1485:
#line 5359 "sql_yacc.yy"
    {}
    break;

  case 1486:
#line 5360 "sql_yacc.yy"
    {}
    break;

  case 1487:
#line 5361 "sql_yacc.yy"
    {}
    break;

  case 1488:
#line 5362 "sql_yacc.yy"
    {}
    break;

  case 1489:
#line 5363 "sql_yacc.yy"
    {}
    break;

  case 1490:
#line 5364 "sql_yacc.yy"
    {}
    break;

  case 1491:
#line 5365 "sql_yacc.yy"
    {}
    break;

  case 1492:
#line 5366 "sql_yacc.yy"
    {}
    break;

  case 1493:
#line 5367 "sql_yacc.yy"
    {}
    break;

  case 1494:
#line 5368 "sql_yacc.yy"
    {}
    break;

  case 1495:
#line 5375 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_SET_OPTION;
          mysql_init_select(lex);
	  lex->option_type=OPT_SESSION;
	  lex->var_list.empty();
          lex->one_shot_set= 0;
	}
    break;

  case 1496:
#line 5384 "sql_yacc.yy"
    {}
    break;

  case 1497:
#line 5388 "sql_yacc.yy"
    {}
    break;

  case 1498:
#line 5389 "sql_yacc.yy"
    {}
    break;

  case 1501:
#line 5396 "sql_yacc.yy"
    {}
    break;

  case 1502:
#line 5397 "sql_yacc.yy"
    {}
    break;

  case 1503:
#line 5401 "sql_yacc.yy"
    {}
    break;

  case 1504:
#line 5402 "sql_yacc.yy"
    { Lex->option_type= OPT_GLOBAL; }
    break;

  case 1505:
#line 5403 "sql_yacc.yy"
    { Lex->option_type= OPT_SESSION; }
    break;

  case 1506:
#line 5404 "sql_yacc.yy"
    { Lex->option_type= OPT_SESSION; }
    break;

  case 1507:
#line 5408 "sql_yacc.yy"
    {}
    break;

  case 1508:
#line 5409 "sql_yacc.yy"
    { Lex->option_type= OPT_SESSION; Lex->one_shot_set= 1; }
    break;

  case 1509:
#line 5413 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1510:
#line 5414 "sql_yacc.yy"
    { yyval.num=OPT_GLOBAL; }
    break;

  case 1511:
#line 5415 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1512:
#line 5416 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1513:
#line 5420 "sql_yacc.yy"
    { yyval.num=OPT_DEFAULT; }
    break;

  case 1514:
#line 5421 "sql_yacc.yy"
    { yyval.num=OPT_GLOBAL; }
    break;

  case 1515:
#line 5422 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1516:
#line 5423 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1517:
#line 5428 "sql_yacc.yy"
    {
          LEX *lex=Lex;
          lex->var_list.push_back(new set_var(lex->option_type, yyvsp[-2].variable.var,
                                  &yyvsp[-2].variable.base_name, yyvsp[0].item));
        }
    break;

  case 1518:
#line 5434 "sql_yacc.yy"
    {
          LEX *lex=Lex;
          LEX_STRING tmp;
          tmp.str=0;
          tmp.length=0;
          lex->var_list.push_back(new set_var(lex->option_type,
                                              find_sys_var("tx_isolation"),
                                              &tmp,
                                              new Item_int((int32) yyvsp[0].tx_isolation)));
        }
    break;

  case 1519:
#line 5448 "sql_yacc.yy"
    {
	  Lex->var_list.push_back(new set_var_user(new Item_func_set_user_var(yyvsp[-2].lex_str,yyvsp[0].item)));
	}
    break;

  case 1520:
#line 5452 "sql_yacc.yy"
    {
          LEX *lex=Lex;
          lex->var_list.push_back(new set_var((enum_var_type) yyvsp[-3].num, yyvsp[-2].variable.var,
                                  &yyvsp[-2].variable.base_name, yyvsp[0].item));
        }
    break;

  case 1521:
#line 5458 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  LEX *lex= Lex;
	  yyvsp[0].charset= yyvsp[0].charset ? yyvsp[0].charset: global_system_variables.character_set_client;
	  lex->var_list.push_back(new set_var_collation_client(yyvsp[0].charset,thd->variables.collation_database,yyvsp[0].charset));
	}
    break;

  case 1522:
#line 5465 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  LEX *lex= Lex;
	  yyvsp[-1].charset= yyvsp[-1].charset ? yyvsp[-1].charset : global_system_variables.character_set_client;
	  yyvsp[0].charset= yyvsp[0].charset ? yyvsp[0].charset : yyvsp[-1].charset;
	  if (!my_charset_same(yyvsp[-1].charset,yyvsp[0].charset))
	  {
	    net_printf(thd,ER_COLLATION_CHARSET_MISMATCH,yyvsp[0].charset->name,yyvsp[-1].charset->csname);
	    YYABORT;
	  }
	  lex->var_list.push_back(new set_var_collation_client(yyvsp[0].charset,yyvsp[0].charset,yyvsp[0].charset));
	}
    break;

  case 1523:
#line 5478 "sql_yacc.yy"
    {
	    THD *thd=YYTHD;
	    LEX_USER *user;
	    if (!(user=(LEX_USER*) thd->alloc(sizeof(LEX_USER))))
	      YYABORT;
	    user->host.str=0;
	    user->user.str=thd->priv_user;
	    thd->lex->var_list.push_back(new set_var_password(user, yyvsp[0].simple_string));
	  }
    break;

  case 1524:
#line 5488 "sql_yacc.yy"
    {
	    Lex->var_list.push_back(new set_var_password(yyvsp[-2].lex_user,yyvsp[0].simple_string));
	  }
    break;

  case 1525:
#line 5495 "sql_yacc.yy"
    {
	  sys_var *tmp=find_sys_var(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length);
	  if (!tmp)
	    YYABORT;
	  yyval.variable.var= tmp;
	  yyval.variable.base_name.str=0;
	  yyval.variable.base_name.length=0;
          /*
            If this is time_zone variable we should open time zone
            describing tables 
          */
          if (tmp == &sys_time_zone)
	    Lex->time_zone_tables_used= &fake_time_zone_tables_list;
	}
    break;

  case 1526:
#line 5510 "sql_yacc.yy"
    {
            if (check_reserved_words(&yyvsp[-2].lex_str))
            {
	      yyerror(ER(ER_SYNTAX_ERROR));
              YYABORT;
            }
	    sys_var *tmp=find_sys_var(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length);
	    if (!tmp)
	      YYABORT;
	    if (!tmp->is_struct())
	      net_printf(YYTHD, ER_VARIABLE_IS_NOT_STRUCT, yyvsp[0].lex_str.str);
	    yyval.variable.var= tmp;
	    yyval.variable.base_name= yyvsp[-2].lex_str;
	  }
    break;

  case 1527:
#line 5525 "sql_yacc.yy"
    {
	    sys_var *tmp=find_sys_var(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length);
	    if (!tmp)
	      YYABORT;
	    if (!tmp->is_struct())
	      net_printf(YYTHD, ER_VARIABLE_IS_NOT_STRUCT, yyvsp[0].lex_str.str);
	    yyval.variable.var= tmp;
	    yyval.variable.base_name.str=    (char*) "default";
	    yyval.variable.base_name.length= 7;
	  }
    break;

  case 1528:
#line 5538 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_READ_UNCOMMITTED; }
    break;

  case 1529:
#line 5539 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_READ_COMMITTED; }
    break;

  case 1530:
#line 5540 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_REPEATABLE_READ; }
    break;

  case 1531:
#line 5541 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_SERIALIZABLE; }
    break;

  case 1532:
#line 5545 "sql_yacc.yy"
    { yyval.simple_string=yyvsp[0].lex_str.str;}
    break;

  case 1533:
#line 5547 "sql_yacc.yy"
    {
	    yyval.simple_string= yyvsp[-1].lex_str.length ? YYTHD->variables.old_passwords ?
	        Item_func_old_password::alloc(YYTHD, yyvsp[-1].lex_str.str) :
	        Item_func_password::alloc(YYTHD, yyvsp[-1].lex_str.str) :
	      yyvsp[-1].lex_str.str;
	  }
    break;

  case 1534:
#line 5554 "sql_yacc.yy"
    {
	    yyval.simple_string= yyvsp[-1].lex_str.length ? Item_func_old_password::alloc(YYTHD, yyvsp[-1].lex_str.str) :
	      yyvsp[-1].lex_str.str;
	  }
    break;

  case 1535:
#line 5562 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 1536:
#line 5563 "sql_yacc.yy"
    { yyval.item=0; }
    break;

  case 1537:
#line 5564 "sql_yacc.yy"
    { yyval.item=new Item_string("ON",  2, system_charset_info); }
    break;

  case 1538:
#line 5565 "sql_yacc.yy"
    { yyval.item=new Item_string("ALL", 3, system_charset_info); }
    break;

  case 1539:
#line 5566 "sql_yacc.yy"
    { yyval.item=new Item_string("binary", 6, system_charset_info); }
    break;

  case 1540:
#line 5574 "sql_yacc.yy"
    {
	  Lex->sql_command=SQLCOM_LOCK_TABLES;
	}
    break;

  case 1541:
#line 5578 "sql_yacc.yy"
    {}
    break;

  case 1546:
#line 5591 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, yyvsp[-2].table, yyvsp[-1].lex_str_ptr, 0, (thr_lock_type) yyvsp[0].num))
	   YYABORT;
	}
    break;

  case 1547:
#line 5598 "sql_yacc.yy"
    { yyval.num=TL_READ_NO_INSERT; }
    break;

  case 1548:
#line 5599 "sql_yacc.yy"
    { yyval.num=YYTHD->update_lock_default; }
    break;

  case 1549:
#line 5600 "sql_yacc.yy"
    { yyval.num=TL_WRITE_LOW_PRIORITY; }
    break;

  case 1550:
#line 5601 "sql_yacc.yy"
    { yyval.num= TL_READ; }
    break;

  case 1551:
#line 5605 "sql_yacc.yy"
    { Lex->sql_command=SQLCOM_UNLOCK_TABLES; }
    break;

  case 1552:
#line 5615 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  lex->sql_command = SQLCOM_HA_OPEN;
	  if (!lex->current_select->add_table_to_list(lex->thd, yyvsp[-2].table, yyvsp[0].lex_str_ptr, 0))
	    YYABORT;
	}
    break;

  case 1553:
#line 5622 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  lex->sql_command = SQLCOM_HA_CLOSE;
	  if (!lex->current_select->add_table_to_list(lex->thd, yyvsp[-1].table, 0, 0))
	    YYABORT;
	}
    break;

  case 1554:
#line 5629 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_HA_READ;
	  lex->ha_rkey_mode= HA_READ_KEY_EXACT;	/* Avoid purify warnings */
	  lex->current_select->select_limit= 1;
	  lex->current_select->offset_limit= 0L;
	  if (!lex->current_select->add_table_to_list(lex->thd, yyvsp[-1].table, 0, 0))
	    YYABORT;
        }
    break;

  case 1555:
#line 5638 "sql_yacc.yy"
    {}
    break;

  case 1556:
#line 5642 "sql_yacc.yy"
    { Lex->backup_dir= 0; }
    break;

  case 1557:
#line 5643 "sql_yacc.yy"
    { Lex->backup_dir= yyvsp[-1].lex_str.str; }
    break;

  case 1558:
#line 5647 "sql_yacc.yy"
    { Lex->ha_read_mode = RFIRST; }
    break;

  case 1559:
#line 5648 "sql_yacc.yy"
    { Lex->ha_read_mode = RNEXT;  }
    break;

  case 1560:
#line 5652 "sql_yacc.yy"
    { Lex->ha_read_mode = RFIRST; }
    break;

  case 1561:
#line 5653 "sql_yacc.yy"
    { Lex->ha_read_mode = RNEXT;  }
    break;

  case 1562:
#line 5654 "sql_yacc.yy"
    { Lex->ha_read_mode = RPREV;  }
    break;

  case 1563:
#line 5655 "sql_yacc.yy"
    { Lex->ha_read_mode = RLAST;  }
    break;

  case 1564:
#line 5657 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->ha_read_mode = RKEY;
	  lex->ha_rkey_mode=yyvsp[0].ha_rkey_mode;
	  if (!(lex->insert_list = new List_item))
	    YYABORT;
	}
    break;

  case 1565:
#line 5663 "sql_yacc.yy"
    { }
    break;

  case 1566:
#line 5667 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_KEY_EXACT;   }
    break;

  case 1567:
#line 5668 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_KEY_OR_NEXT; }
    break;

  case 1568:
#line 5669 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_KEY_OR_PREV; }
    break;

  case 1569:
#line 5670 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_AFTER_KEY;   }
    break;

  case 1570:
#line 5671 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_BEFORE_KEY;  }
    break;

  case 1571:
#line 5678 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_REVOKE;
	  lex->users_list.empty();
	  lex->columns.empty();
	  lex->grant= lex->grant_tot_col=0;
	  lex->select_lex.db=0;
	  lex->ssl_type= SSL_TYPE_NOT_SPECIFIED;
	  lex->ssl_cipher= lex->x509_subject= lex->x509_issuer= 0;
	  bzero((char*) &lex->mqh, sizeof(lex->mqh));
	}
    break;

  case 1572:
#line 5690 "sql_yacc.yy"
    {}
    break;

  case 1573:
#line 5695 "sql_yacc.yy"
    {}
    break;

  case 1574:
#line 5698 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_REVOKE_ALL;
	}
    break;

  case 1575:
#line 5705 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->users_list.empty();
	  lex->columns.empty();
	  lex->sql_command = SQLCOM_GRANT;
	  lex->grant= lex->grant_tot_col= 0;
	  lex->select_lex.db= 0;
	  lex->ssl_type= SSL_TYPE_NOT_SPECIFIED;
	  lex->ssl_cipher= lex->x509_subject= lex->x509_issuer= 0;
	  bzero((char *)&(lex->mqh),sizeof(lex->mqh));
	}
    break;

  case 1576:
#line 5718 "sql_yacc.yy"
    {}
    break;

  case 1577:
#line 5722 "sql_yacc.yy"
    {}
    break;

  case 1578:
#line 5723 "sql_yacc.yy"
    { Lex->grant = GLOBAL_ACLS;}
    break;

  case 1583:
#line 5736 "sql_yacc.yy"
    { Lex->which_columns = SELECT_ACL;}
    break;

  case 1584:
#line 5736 "sql_yacc.yy"
    {}
    break;

  case 1585:
#line 5737 "sql_yacc.yy"
    { Lex->which_columns = INSERT_ACL;}
    break;

  case 1586:
#line 5737 "sql_yacc.yy"
    {}
    break;

  case 1587:
#line 5738 "sql_yacc.yy"
    { Lex->which_columns = UPDATE_ACL; }
    break;

  case 1588:
#line 5738 "sql_yacc.yy"
    {}
    break;

  case 1589:
#line 5739 "sql_yacc.yy"
    { Lex->which_columns = REFERENCES_ACL;}
    break;

  case 1590:
#line 5739 "sql_yacc.yy"
    {}
    break;

  case 1591:
#line 5740 "sql_yacc.yy"
    { Lex->grant |= DELETE_ACL;}
    break;

  case 1592:
#line 5741 "sql_yacc.yy"
    {}
    break;

  case 1593:
#line 5742 "sql_yacc.yy"
    { Lex->grant |= INDEX_ACL;}
    break;

  case 1594:
#line 5743 "sql_yacc.yy"
    { Lex->grant |= ALTER_ACL;}
    break;

  case 1595:
#line 5744 "sql_yacc.yy"
    { Lex->grant |= CREATE_ACL;}
    break;

  case 1596:
#line 5745 "sql_yacc.yy"
    { Lex->grant |= DROP_ACL;}
    break;

  case 1597:
#line 5746 "sql_yacc.yy"
    { Lex->grant |= EXECUTE_ACL;}
    break;

  case 1598:
#line 5747 "sql_yacc.yy"
    { Lex->grant |= RELOAD_ACL;}
    break;

  case 1599:
#line 5748 "sql_yacc.yy"
    { Lex->grant |= SHUTDOWN_ACL;}
    break;

  case 1600:
#line 5749 "sql_yacc.yy"
    { Lex->grant |= PROCESS_ACL;}
    break;

  case 1601:
#line 5750 "sql_yacc.yy"
    { Lex->grant |= FILE_ACL;}
    break;

  case 1602:
#line 5751 "sql_yacc.yy"
    { Lex->grant |= GRANT_ACL;}
    break;

  case 1603:
#line 5752 "sql_yacc.yy"
    { Lex->grant |= SHOW_DB_ACL;}
    break;

  case 1604:
#line 5753 "sql_yacc.yy"
    { Lex->grant |= SUPER_ACL;}
    break;

  case 1605:
#line 5754 "sql_yacc.yy"
    { Lex->grant |= CREATE_TMP_ACL;}
    break;

  case 1606:
#line 5755 "sql_yacc.yy"
    { Lex->grant |= LOCK_TABLES_ACL; }
    break;

  case 1607:
#line 5756 "sql_yacc.yy"
    { Lex->grant |= REPL_SLAVE_ACL;}
    break;

  case 1608:
#line 5757 "sql_yacc.yy"
    { Lex->grant |= REPL_CLIENT_ACL;}
    break;

  case 1609:
#line 5762 "sql_yacc.yy"
    {}
    break;

  case 1610:
#line 5763 "sql_yacc.yy"
    {}
    break;

  case 1613:
#line 5773 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->x509_subject)
	  {
	    net_printf(lex->thd,ER_DUP_ARGUMENT, "SUBJECT");
	    YYABORT;
	  }
	  lex->x509_subject=yyvsp[0].lex_str.str;
	}
    break;

  case 1614:
#line 5783 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->x509_issuer)
	  {
	    net_printf(lex->thd,ER_DUP_ARGUMENT, "ISSUER");
	    YYABORT;
	  }
	  lex->x509_issuer=yyvsp[0].lex_str.str;
	}
    break;

  case 1615:
#line 5793 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->ssl_cipher)
	  {
	    net_printf(lex->thd,ER_DUP_ARGUMENT, "CIPHER");
	    YYABORT;
	  }
	  lex->ssl_cipher=yyvsp[0].lex_str.str;
	}
    break;

  case 1616:
#line 5806 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->current_select->db= lex->thd->db;
	    if (lex->grant == GLOBAL_ACLS)
	      lex->grant = DB_ACLS & ~GRANT_ACL;
	    else if (lex->columns.elements)
	    {
	      send_error(lex->thd,ER_ILLEGAL_GRANT_FOR_TABLE);
	      YYABORT;
	    }
	  }
    break;

  case 1617:
#line 5818 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->current_select->db = yyvsp[-2].lex_str.str;
	    if (lex->grant == GLOBAL_ACLS)
	      lex->grant = DB_ACLS & ~GRANT_ACL;
	    else if (lex->columns.elements)
	    {
	      send_error(lex->thd,ER_ILLEGAL_GRANT_FOR_TABLE);
	      YYABORT;
	    }
	  }
    break;

  case 1618:
#line 5830 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->current_select->db = NULL;
	    if (lex->grant == GLOBAL_ACLS)
	      lex->grant= GLOBAL_ACLS & ~GRANT_ACL;
	    else if (lex->columns.elements)
	    {
	      send_error(lex->thd,ER_ILLEGAL_GRANT_FOR_TABLE);
	      YYABORT;
	    }
	  }
    break;

  case 1619:
#line 5842 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (!lex->current_select->add_table_to_list(lex->thd, yyvsp[0].table,NULL,0))
	      YYABORT;
	    if (lex->grant == GLOBAL_ACLS)
	      lex->grant =  TABLE_ACLS & ~GRANT_ACL;
	  }
    break;

  case 1620:
#line 5853 "sql_yacc.yy"
    { if (Lex->users_list.push_back(yyvsp[0].lex_user)) YYABORT;}
    break;

  case 1621:
#line 5855 "sql_yacc.yy"
    {
	    if (Lex->users_list.push_back(yyvsp[0].lex_user))
	      YYABORT;
	  }
    break;

  case 1622:
#line 5864 "sql_yacc.yy"
    {
	   yyval.lex_user=yyvsp[-3].lex_user; yyvsp[-3].lex_user->password=yyvsp[0].lex_str;
	   if (yyvsp[0].lex_str.length)
	   {
             if (YYTHD->variables.old_passwords)
             {
               char *buff= 
                 (char *) YYTHD->alloc(SCRAMBLED_PASSWORD_CHAR_LENGTH_323+1);
               if (buff)
                 make_scrambled_password_323(buff, yyvsp[0].lex_str.str);
               yyvsp[-3].lex_user->password.str= buff;
               yyvsp[-3].lex_user->password.length= SCRAMBLED_PASSWORD_CHAR_LENGTH_323;
             }
             else
             {
               char *buff= 
                 (char *) YYTHD->alloc(SCRAMBLED_PASSWORD_CHAR_LENGTH+1);
               if (buff)
                 make_scrambled_password(buff, yyvsp[0].lex_str.str);
               yyvsp[-3].lex_user->password.str= buff;
               yyvsp[-3].lex_user->password.length= SCRAMBLED_PASSWORD_CHAR_LENGTH;
             }
	  }
	}
    break;

  case 1623:
#line 5889 "sql_yacc.yy"
    { yyval.lex_user=yyvsp[-4].lex_user; yyvsp[-4].lex_user->password=yyvsp[0].lex_str ; }
    break;

  case 1624:
#line 5891 "sql_yacc.yy"
    { yyval.lex_user=yyvsp[0].lex_user; yyvsp[0].lex_user->password.str=NullS; }
    break;

  case 1625:
#line 5897 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->grant |= lex->which_columns;
	}
    break;

  case 1629:
#line 5909 "sql_yacc.yy"
    {
	  String *new_str = new (YYTHD->mem_root) String((const char*) yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,system_charset_info);
	  List_iterator <LEX_COLUMN> iter(Lex->columns);
	  class LEX_COLUMN *point;
	  LEX *lex=Lex;
	  while ((point=iter++))
	  {
	    if (!my_strcasecmp(system_charset_info,
                               point->column.ptr(), new_str->ptr()))
		break;
	  }
	  lex->grant_tot_col|= lex->which_columns;
	  if (point)
	    point->rights |= lex->which_columns;
	  else
	    lex->columns.push_back(new LEX_COLUMN (*new_str,lex->which_columns));
	}
    break;

  case 1631:
#line 5931 "sql_yacc.yy"
    {
            Lex->ssl_type=SSL_TYPE_SPECIFIED;
          }
    break;

  case 1632:
#line 5935 "sql_yacc.yy"
    {
            Lex->ssl_type=SSL_TYPE_ANY;
          }
    break;

  case 1633:
#line 5939 "sql_yacc.yy"
    {
            Lex->ssl_type=SSL_TYPE_X509;
          }
    break;

  case 1634:
#line 5943 "sql_yacc.yy"
    {
	    Lex->ssl_type=SSL_TYPE_NONE;
	  }
    break;

  case 1635:
#line 5949 "sql_yacc.yy"
    {}
    break;

  case 1637:
#line 5953 "sql_yacc.yy"
    {}
    break;

  case 1638:
#line 5954 "sql_yacc.yy"
    {}
    break;

  case 1639:
#line 5958 "sql_yacc.yy"
    { Lex->grant |= GRANT_ACL;}
    break;

  case 1640:
#line 5960 "sql_yacc.yy"
    {
	  Lex->mqh.questions=yyvsp[0].ulong_num;
	  Lex->mqh.bits |= 1;
	}
    break;

  case 1641:
#line 5965 "sql_yacc.yy"
    {
	  Lex->mqh.updates=yyvsp[0].ulong_num;
	  Lex->mqh.bits |= 2;
	}
    break;

  case 1642:
#line 5970 "sql_yacc.yy"
    {
	  Lex->mqh.connections=yyvsp[0].ulong_num;
	  Lex->mqh.bits |= 4;
	}
    break;

  case 1643:
#line 5977 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_BEGIN; Lex->start_transaction_opt= 0;}
    break;

  case 1644:
#line 5977 "sql_yacc.yy"
    {}
    break;

  case 1645:
#line 5981 "sql_yacc.yy"
    {}
    break;

  case 1646:
#line 5982 "sql_yacc.yy"
    {;}
    break;

  case 1647:
#line 5986 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_COMMIT;}
    break;

  case 1648:
#line 5990 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_ROLLBACK;
	}
    break;

  case 1649:
#line 5994 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_ROLLBACK_TO_SAVEPOINT;
	  Lex->savepoint_name = yyvsp[0].lex_str.str;
	}
    break;

  case 1650:
#line 6000 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_SAVEPOINT;
	  Lex->savepoint_name = yyvsp[0].lex_str.str;
	}
    break;

  case 1651:
#line 6011 "sql_yacc.yy"
    {}
    break;

  case 1653:
#line 6017 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->exchange)
	  {
	    /* Only the last SELECT can have  INTO...... */
	    net_printf(lex->thd, ER_WRONG_USAGE, "UNION", "INTO");
	    YYABORT;
	  }
	  if (lex->current_select->linkage == GLOBAL_OPTIONS_TYPE)
	  {
            yyerror(ER(ER_SYNTAX_ERROR));
	    YYABORT;
	  }
	  if (mysql_new_select(lex, 0))
	    YYABORT;
          mysql_init_select(lex);
	  lex->current_select->linkage=UNION_TYPE;
          if (yyvsp[0].num) /* UNION DISTINCT - remember position */
            lex->current_select->master_unit()->union_distinct=
                                                      lex->current_select;
	}
    break;

  case 1654:
#line 6038 "sql_yacc.yy"
    {}
    break;

  case 1655:
#line 6042 "sql_yacc.yy"
    {}
    break;

  case 1656:
#line 6043 "sql_yacc.yy"
    {}
    break;

  case 1657:
#line 6047 "sql_yacc.yy"
    {}
    break;

  case 1658:
#line 6049 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    LEX *lex= thd->lex;
	    DBUG_ASSERT(lex->current_select->linkage != GLOBAL_OPTIONS_TYPE);
	    SELECT_LEX *sel= lex->current_select;
	    SELECT_LEX_UNIT *unit= sel->master_unit();
	    SELECT_LEX *fake= unit->fake_select_lex;
	    if (fake)
	    {
	      unit->global_parameters= fake;
	      fake->no_table_names_allowed= 1;
	      lex->current_select= fake;
	    }
	    thd->where= "global ORDER clause";
	  }
    break;

  case 1659:
#line 6065 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    thd->lex->current_select->no_table_names_allowed= 0;
	    thd->where= "";
          }
    break;

  case 1662:
#line 6078 "sql_yacc.yy"
    { yyval.num=1; }
    break;

  case 1663:
#line 6079 "sql_yacc.yy"
    { yyval.num=1; }
    break;

  case 1664:
#line 6080 "sql_yacc.yy"
    { yyval.num=0; }
    break;

  case 1665:
#line 6086 "sql_yacc.yy"
    {
	  yyval.item= yyvsp[-1].item;
	}
    break;

  case 1666:
#line 6092 "sql_yacc.yy"
    {
	  yyval.item= new Item_singlerow_subselect(Lex->current_select->
					   master_unit()->first_select());
	}
    break;

  case 1667:
#line 6100 "sql_yacc.yy"
    {
	  yyval.item= yyvsp[-1].item;
	}
    break;

  case 1668:
#line 6106 "sql_yacc.yy"
    {
	  yyval.item= new Item_exists_subselect(Lex->current_select->master_unit()->
					first_select());
	}
    break;

  case 1669:
#line 6114 "sql_yacc.yy"
    {
    yyval.select_lex= yyvsp[-1].select_lex;
  }
    break;

  case 1670:
#line 6120 "sql_yacc.yy"
    {
    yyval.select_lex= Lex->current_select->master_unit()->first_select();
  }
    break;

  case 1671:
#line 6126 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (((int)lex->sql_command >= (int)SQLCOM_HA_OPEN &&
	       lex->sql_command <= (int)SQLCOM_HA_READ) ||
	       lex->sql_command == (int)SQLCOM_KILL)
	  {
            yyerror(ER(ER_SYNTAX_ERROR));
	    YYABORT;
	  }
	  if (mysql_new_select(Lex, 1))
	    YYABORT;
	}
    break;

  case 1672:
#line 6141 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->current_select = lex->current_select->return_after_parsing();
	}
    break;


    }

/* Line 1000 of yacc.c.  */
#line 21762 "sql_yacc.cc"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}



