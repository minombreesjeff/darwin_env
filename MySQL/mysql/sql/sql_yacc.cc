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
#define YYFINAL  546
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   39097

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  495
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  489
/* YYNRULES -- Number of rules. */
#define YYNRULES  1666
/* YYNRULES -- Number of states. */
#define YYNSTATES  3095

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
    1371,  1372,  1379,  1380,  1384,  1385,  1386,  1393,  1395,  1397,
    1399,  1402,  1405,  1414,  1418,  1419,  1421,  1424,  1426,  1428,
    1430,  1432,  1434,  1436,  1438,  1440,  1442,  1444,  1446,  1447,
    1450,  1455,  1459,  1461,  1463,  1468,  1469,  1470,  1472,  1474,
    1475,  1478,  1481,  1483,  1485,  1486,  1489,  1491,  1493,  1495,
    1497,  1499,  1501,  1503,  1505,  1507,  1509,  1515,  1522,  1526,
    1531,  1537,  1544,  1548,  1552,  1556,  1560,  1565,  1570,  1576,
    1580,  1585,  1589,  1594,  1598,  1602,  1607,  1611,  1615,  1619,
    1623,  1627,  1631,  1635,  1639,  1643,  1647,  1651,  1655,  1660,
    1665,  1671,  1678,  1682,  1686,  1690,  1694,  1699,  1704,  1710,
    1714,  1719,  1723,  1728,  1732,  1736,  1741,  1745,  1749,  1753,
    1757,  1761,  1765,  1769,  1773,  1777,  1781,  1785,  1789,  1794,
    1799,  1801,  1807,  1814,  1818,  1823,  1829,  1836,  1840,  1844,
    1848,  1853,  1858,  1864,  1868,  1873,  1877,  1882,  1886,  1890,
    1895,  1899,  1903,  1907,  1911,  1915,  1919,  1923,  1927,  1931,
    1935,  1939,  1943,  1948,  1953,  1955,  1958,  1960,  1964,  1966,
    1968,  1973,  1976,  1982,  1984,  1987,  1990,  1993,  1996,  1999,
    2003,  2009,  2016,  2019,  2021,  2026,  2034,  2039,  2042,  2049,
    2056,  2063,  2070,  2075,  2080,  2084,  2089,  2096,  2105,  2112,
    2121,  2126,  2133,  2138,  2143,  2148,  2153,  2158,  2165,  2174,
    2177,  2180,  2185,  2188,  2196,  2204,  2208,  2213,  2218,  2225,
    2232,  2237,  2244,  2251,  2258,  2263,  2270,  2275,  2282,  2291,
    2302,  2315,  2317,  2324,  2329,  2336,  2343,  2345,  2352,  2357,
    2366,  2377,  2382,  2384,  2388,  2393,  2400,  2407,  2416,  2423,
    2430,  2435,  2442,  2449,  2458,  2463,  2468,  2475,  2480,  2483,
    2488,  2493,  2498,  2505,  2510,  2514,  2523,  2530,  2535,  2542,
    2549,  2558,  2563,  2572,  2579,  2588,  2595,  2604,  2609,  2614,
    2621,  2626,  2634,  2642,  2650,  2657,  2664,  2671,  2678,  2685,
    2687,  2692,  2697,  2702,  2707,  2712,  2717,  2728,  2732,  2737,
    2741,  2744,  2747,  2750,  2755,  2762,  2767,  2772,  2779,  2786,
    2793,  2798,  2805,  2810,  2817,  2822,  2827,  2832,  2837,  2844,
    2849,  2856,  2861,  2868,  2873,  2878,  2885,  2890,  2897,  2902,
    2909,  2914,  2919,  2926,  2931,  2938,  2939,  2943,  2947,  2948,
    2950,  2955,  2960,  2965,  2970,  2976,  2981,  2982,  2983,  2991,
    3002,  3007,  3012,  3017,  3022,  3027,  3028,  3037,  3038,  3040,
    3041,  3044,  3045,  3047,  3048,  3052,  3055,  3059,  3062,  3064,
    3067,  3069,  3072,  3074,  3076,  3078,  3079,  3082,  3084,  3088,
    3090,  3094,  3095,  3098,  3100,  3104,  3105,  3107,  3108,  3111,
    3112,  3115,  3119,  3125,  3129,  3131,  3135,  3139,  3143,  3149,
    3150,  3159,  3167,  3168,  3179,  3186,  3194,  3195,  3206,  3213,
    3218,  3220,  3223,  3226,  3227,  3232,  3243,  3249,  3250,  3251,
    3258,  3259,  3261,  3262,  3265,  3268,  3271,  3272,  3278,  3279,
    3281,  3285,  3287,  3289,  3291,  3295,  3297,  3299,  3301,  3303,
    3305,  3307,  3309,  3311,  3313,  3315,  3317,  3319,  3321,  3323,
    3325,  3327,  3329,  3331,  3333,  3335,  3337,  3339,  3340,  3342,
    3344,  3345,  3348,  3349,  3351,  3352,  3353,  3357,  3358,  3359,
    3363,  3366,  3367,  3368,  3373,  3378,  3381,  3382,  3385,  3388,
    3389,  3391,  3392,  3397,  3402,  3405,  3406,  3408,  3410,  3411,
    3413,  3414,  3416,  3419,  3421,  3425,  3429,  3430,  3433,  3435,
    3437,  3439,  3441,  3443,  3445,  3447,  3449,  3451,  3453,  3454,
    3455,  3462,  3463,  3465,  3469,  3471,  3474,  3475,  3478,  3482,
    3484,  3487,  3488,  3495,  3499,  3502,  3503,  3507,  3514,  3515,
    3522,  3527,  3531,  3532,  3537,  3539,  3543,  3545,  3546,  3549,
    3550,  3552,  3553,  3554,  3563,  3564,  3565,  3566,  3574,  3575,
    3577,  3579,  3581,  3583,  3585,  3588,  3590,  3592,  3594,  3598,
    3603,  3604,  3608,  3609,  3613,  3616,  3620,  3622,  3625,  3628,
    3629,  3633,  3634,  3640,  3644,  3646,  3650,  3652,  3656,  3658,
    3660,  3661,  3663,  3664,  3669,  3670,  3672,  3676,  3678,  3680,
    3682,  3683,  3684,  3691,  3692,  3693,  3705,  3709,  3711,  3715,
    3719,  3721,  3725,  3726,  3728,  3729,  3734,  3735,  3742,  3743,
    3749,  3750,  3757,  3759,  3763,  3767,  3773,  3774,  3777,  3778,
    3781,  3783,  3785,  3787,  3791,  3792,  3794,  3795,  3799,  3802,
    3806,  3811,  3816,  3817,  3822,  3829,  3846,  3849,  3852,  3853,
    3860,  3865,  3868,  3871,  3874,  3876,  3882,  3888,  3891,  3894,
    3897,  3900,  3903,  3907,  3910,  3913,  3916,  3918,  3920,  3924,
    3929,  3933,  3936,  3939,  3941,  3943,  3945,  3947,  3948,  3950,
    3951,  3954,  3955,  3958,  3959,  3961,  3963,  3965,  3966,  3969,
    3970,  3973,  3974,  3979,  3980,  3985,  3987,  3989,  3990,  3992,
    3993,  3995,  3997,  3998,  4003,  4007,  4009,  4010,  4014,  4019,
    4022,  4024,  4026,  4028,  4030,  4032,  4034,  4036,  4038,  4039,
    4041,  4042,  4046,  4050,  4052,  4054,  4056,  4059,  4060,  4064,
    4068,  4071,  4074,  4077,  4080,  4081,  4097,  4103,  4108,  4109,
    4111,  4112,  4114,  4116,  4117,  4119,  4121,  4122,  4125,  4128,
    4130,  4134,  4139,  4143,  4147,  4148,  4151,  4154,  4156,  4160,
    4164,  4165,  4169,  4171,  4173,  4176,  4179,  4181,  4183,  4185,
    4187,  4190,  4193,  4195,  4197,  4199,  4201,  4204,  4207,  4210,
    4213,  4215,  4217,  4219,  4221,  4223,  4225,  4227,  4231,  4237,
    4239,  4241,  4245,  4250,  4256,  4258,  4262,  4265,  4267,  4271,
    4274,  4276,  4278,  4280,  4282,  4284,  4286,  4288,  4290,  4292,
    4294,  4296,  4300,  4303,  4305,  4307,  4309,  4311,  4313,  4315,
    4317,  4319,  4321,  4323,  4325,  4327,  4329,  4331,  4333,  4335,
    4337,  4339,  4341,  4343,  4345,  4347,  4349,  4351,  4353,  4355,
    4357,  4359,  4361,  4363,  4365,  4367,  4369,  4371,  4373,  4375,
    4377,  4379,  4381,  4383,  4385,  4387,  4389,  4391,  4393,  4395,
    4397,  4399,  4401,  4403,  4405,  4407,  4409,  4411,  4413,  4415,
    4417,  4419,  4421,  4423,  4425,  4427,  4429,  4431,  4433,  4435,
    4437,  4439,  4441,  4443,  4445,  4447,  4449,  4451,  4453,  4455,
    4457,  4459,  4461,  4463,  4465,  4467,  4469,  4471,  4473,  4475,
    4477,  4479,  4481,  4483,  4485,  4487,  4489,  4491,  4493,  4495,
    4497,  4499,  4501,  4503,  4505,  4507,  4509,  4511,  4513,  4515,
    4517,  4519,  4521,  4523,  4525,  4527,  4529,  4531,  4533,  4535,
    4537,  4539,  4541,  4543,  4545,  4547,  4549,  4551,  4553,  4555,
    4557,  4559,  4561,  4563,  4565,  4567,  4569,  4571,  4573,  4575,
    4577,  4579,  4581,  4583,  4585,  4587,  4589,  4591,  4593,  4595,
    4597,  4599,  4601,  4603,  4605,  4607,  4609,  4611,  4613,  4615,
    4617,  4619,  4621,  4623,  4625,  4627,  4629,  4631,  4633,  4635,
    4637,  4639,  4641,  4643,  4645,  4647,  4649,  4651,  4653,  4655,
    4657,  4659,  4661,  4663,  4665,  4667,  4669,  4671,  4673,  4675,
    4677,  4679,  4681,  4683,  4685,  4687,  4689,  4691,  4693,  4695,
    4697,  4699,  4701,  4703,  4705,  4707,  4709,  4711,  4713,  4715,
    4717,  4719,  4721,  4722,  4727,  4728,  4730,  4733,  4738,  4739,
    4741,  4743,  4745,  4747,  4748,  4750,  4752,  4754,  4755,  4758,
    4761,  4764,  4769,  4773,  4780,  4785,  4788,  4792,  4796,  4802,
    4804,  4808,  4812,  4815,  4818,  4821,  4823,  4825,  4830,  4835,
    4837,  4839,  4841,  4843,  4845,  4846,  4851,  4853,  4855,  4857,
    4861,  4865,  4867,  4869,  4872,  4875,  4878,  4883,  4887,  4888,
    4896,  4898,  4901,  4903,  4905,  4907,  4909,  4911,  4913,  4914,
    4920,  4922,  4924,  4926,  4928,  4930,  4931,  4935,  4941,  4949,
    4950,  4960,  4962,  4965,  4966,  4968,  4970,  4974,  4975,  4979,
    4980,  4984,  4985,  4989,  4990,  4994,  4996,  4998,  5000,  5002,
    5004,  5006,  5008,  5010,  5012,  5014,  5016,  5019,  5022,  5024,
    5028,  5031,  5034,  5037,  5038,  5040,  5044,  5046,  5049,  5052,
    5055,  5057,  5061,  5065,  5067,  5069,  5073,  5078,  5084,  5086,
    5087,  5091,  5095,  5097,  5099,  5100,  5103,  5106,  5109,  5112,
    5113,  5116,  5119,  5121,  5124,  5127,  5130,  5133,  5134,  5138,
    5139,  5141,  5143,  5145,  5150,  5153,  5154,  5156,  5157,  5162,
    5164,  5166,  5167,  5168,  5171,  5174,  5176,  5177,  5179,  5181,
    5185,  5187,  5191,  5193,  5197,  5199,  5202
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     496,     0,    -1,     3,    -1,   497,     3,    -1,   611,    -1,
     649,    -1,   639,    -1,   962,    -1,   508,    -1,   651,    -1,
     641,    -1,   965,    -1,   513,    -1,   498,    -1,   829,    -1,
     856,    -1,   782,    -1,   784,    -1,   502,    -1,   862,    -1,
     939,    -1,   929,    -1,   507,    -1,   791,    -1,   876,    -1,
     878,    -1,   922,    -1,   656,    -1,   663,    -1,   667,    -1,
     500,    -1,   872,    -1,   659,    -1,   644,    -1,   794,    -1,
     868,    -1,   637,    -1,   936,    -1,   966,    -1,   967,    -1,
     675,    -1,   910,    -1,   842,    -1,   626,    -1,   629,    -1,
     840,    -1,   928,    -1,   821,    -1,   877,    -1,   499,   337,
     906,    -1,   338,    -1,    44,    -1,   337,   906,   144,   501,
      -1,   904,    -1,   492,   907,    -1,    -1,    46,   906,   503,
     504,    -1,    -1,   301,   505,    -1,   505,   488,   506,    -1,
     506,    -1,   492,   907,    -1,    49,   907,    -1,    -1,    32,
      56,   280,   509,   510,    -1,   511,    -1,   510,   488,   511,
      -1,   185,    10,   904,    -1,   186,    10,   904,    -1,   189,
      10,   904,    -1,   190,    10,   769,    -1,   191,    10,   769,
      -1,   193,    10,   769,    -1,   194,    10,   904,    -1,   195,
      10,   904,    -1,   196,    10,   904,    -1,   197,    10,   904,
      -1,   198,    10,   904,    -1,   512,    -1,   187,    10,   904,
      -1,   188,    10,   770,    -1,   199,    10,   904,    -1,   200,
      10,   769,    -1,    -1,    38,   531,   275,   534,   901,   514,
     518,    -1,    -1,    38,   603,   161,   906,   604,   221,   901,
     515,   486,   606,   487,    -1,    -1,    38,   368,   534,   906,
     516,   528,    -1,    -1,    38,   546,   292,   903,   517,   290,
     547,   291,   904,    -1,   486,   519,    -1,   535,   521,    -1,
     177,   901,    -1,   486,   177,   901,   487,    -1,   548,   487,
     535,   521,    -1,    -1,   524,   487,   520,   971,    -1,    -1,
      -1,   882,   527,   524,   522,   968,    -1,    -1,   882,   527,
     486,   524,   487,   523,   971,    -1,    -1,    -1,    68,   525,
     685,   689,   526,   545,    -1,    -1,    88,    -1,    -1,   529,
      -1,   530,    -1,   529,   530,    -1,   540,    -1,   539,    -1,
      -1,   532,    -1,   533,    -1,   533,   532,    -1,   277,    -1,
      -1,   403,   216,   135,    -1,    -1,   537,    -1,   538,    -1,
     538,   536,    -1,   538,    -1,   538,   537,    -1,   538,   488,
     537,    -1,   382,   813,   541,    -1,   284,   813,   541,    -1,
     202,   813,   770,    -1,   207,   813,   770,    -1,    91,   813,
     769,    -1,   428,   813,   904,    -1,    34,   813,   904,    -1,
      90,   813,   770,    -1,   232,   813,   769,    -1,   232,   813,
     119,    -1,   106,   813,   769,    -1,   121,   813,   769,    -1,
     257,   813,   542,    -1,   241,   813,   543,    -1,   242,   813,
     769,    -1,   243,   813,   769,    -1,   296,   813,   486,   787,
     487,    -1,   539,    -1,   540,    -1,   404,   813,   544,    -1,
     118,   133,   813,   904,    -1,   161,   133,   813,   904,    -1,
     587,   579,   813,   581,    -1,   587,   109,   813,   586,    -1,
     907,    -1,   119,    -1,   140,    -1,   129,    -1,   311,    -1,
     240,    -1,   239,    -1,   769,    -1,   217,    -1,   139,    -1,
       6,    -1,   765,    -1,   684,   688,    -1,    -1,    85,    -1,
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
     488,   607,   763,    -1,   607,   763,    -1,   906,    -1,   906,
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
      -1,   760,    -1,    -1,   112,    -1,    -1,   159,    -1,    -1,
     254,    -1,   103,    -1,    -1,    25,   906,    -1,   139,    -1,
      -1,   280,    -1,    10,    -1,    88,    -1,    -1,    73,    70,
     631,   627,   635,    -1,    76,    70,   631,    -1,    -1,    70,
      73,   631,   628,   635,    -1,    70,    76,   631,    -1,    73,
     282,   630,    -1,    -1,   306,    36,    71,    -1,    -1,   632,
     633,    -1,   634,    -1,   633,   488,   634,    -1,    -1,    72,
      -1,    51,    -1,    -1,    82,   636,    -1,   512,    -1,   636,
     488,   512,    -1,    -1,   253,   924,   638,   787,   144,   904,
      -1,    -1,    92,   924,   640,   787,   280,   904,    -1,    -1,
     106,   924,   642,   787,   643,    -1,    -1,   339,    -1,   136,
      -1,    -1,    62,   658,   924,   645,   787,   646,    -1,    -1,
     647,    -1,   648,    -1,   648,   647,    -1,   339,    -1,   136,
      -1,   299,    -1,    -1,    27,   658,   924,   650,   787,   653,
      -1,    -1,   107,   924,   652,   787,   653,    -1,    -1,   654,
      -1,   655,    -1,   655,   654,    -1,   339,    -1,   325,    -1,
     206,    -1,   136,    -1,   318,    -1,    -1,    60,   658,   924,
     657,   787,   653,    -1,    -1,   308,    -1,   179,    -1,    -1,
     249,   924,   660,   661,    -1,   662,    -1,   661,   488,   662,
      -1,   901,   280,   901,    -1,   102,   161,   664,   167,   666,
      -1,   665,    -1,   664,   488,   665,    -1,   901,   671,    -1,
     906,    -1,   119,    -1,    -1,    53,   161,   166,   102,   668,
     669,    -1,   670,    -1,   669,   488,   670,    -1,   901,   671,
     674,    -1,    -1,   672,   673,    -1,    -1,   601,   486,   744,
     487,    -1,    -1,   159,   174,    -1,   676,    -1,    68,   678,
      -1,    -1,   486,    68,   680,   487,   677,   971,    -1,    -1,
     680,   679,   968,    -1,    -1,    -1,   681,   685,   689,   682,
     683,   688,    -1,   765,    -1,   780,    -1,   684,    -1,   780,
     684,    -1,   684,   780,    -1,   144,   729,   751,   756,   753,
     759,   765,   771,    -1,   144,    42,   765,    -1,    -1,   686,
      -1,   686,   687,    -1,   687,    -1,   272,    -1,   155,    -1,
     127,    -1,   471,    -1,   467,    -1,   472,    -1,   469,    -1,
     470,    -1,   468,    -1,    86,    -1,    -1,   389,    83,    -1,
      55,   167,   441,   417,    -1,   689,   488,   690,    -1,   690,
      -1,   480,    -1,   691,   693,   692,   694,    -1,    -1,    -1,
     897,    -1,   696,    -1,    -1,    88,   906,    -1,    88,   904,
      -1,   906,    -1,   904,    -1,    -1,   486,   487,    -1,   699,
      -1,   703,    -1,    10,    -1,    13,    -1,    14,    -1,    15,
      -1,    16,    -1,    17,    -1,    86,    -1,    28,    -1,   696,
     167,   486,   717,   487,    -1,   696,   216,   167,   486,   717,
     487,    -1,   696,   167,   980,    -1,   696,   216,   167,   980,
      -1,   696,   358,   701,    87,   696,    -1,   696,   216,   358,
     701,    87,   696,    -1,   696,   227,   696,    -1,   696,   226,
     696,    -1,   696,   310,   696,    -1,   696,    87,   696,    -1,
     696,    12,   177,   696,    -1,   696,   177,   703,   755,    -1,
     696,   216,   177,   703,   755,    -1,   696,   247,   696,    -1,
     696,   216,   247,   696,    -1,   696,    18,   218,    -1,   696,
      18,   216,   218,    -1,   696,    11,   696,    -1,   696,   697,
     696,    -1,   696,   697,   698,   980,    -1,   696,    20,   696,
      -1,   696,    21,   696,    -1,   696,   479,   696,    -1,   696,
     478,   696,    -1,   696,   480,   696,    -1,   696,   481,   696,
      -1,   696,     9,   696,    -1,   696,    19,   696,    -1,   696,
     476,   696,    -1,   696,   483,   696,    -1,   696,   477,   696,
      -1,   696,   482,   696,    -1,   696,   479,   702,   746,    -1,
     696,   478,   702,   746,    -1,   700,   358,   701,    87,   696,
      -1,   700,   216,   358,   701,    87,   696,    -1,   700,   227,
     696,    -1,   700,   226,   696,    -1,   700,   310,   696,    -1,
     700,    87,   696,    -1,   700,    12,   177,   696,    -1,   700,
     177,   703,   755,    -1,   700,   216,   177,   703,   755,    -1,
     700,   247,   696,    -1,   700,   216,   247,   696,    -1,   700,
      18,   218,    -1,   700,    18,   216,   218,    -1,   700,    11,
     696,    -1,   700,   697,   696,    -1,   700,   697,   698,   980,
      -1,   700,    20,   696,    -1,   700,    21,   696,    -1,   700,
     479,   696,    -1,   700,   478,   696,    -1,   700,   480,   696,
      -1,   700,   481,   696,    -1,   700,     9,   696,    -1,   700,
     476,   696,    -1,   700,   483,   696,    -1,   700,   477,   696,
      -1,   700,   482,   696,    -1,   700,    19,   696,    -1,   700,
     479,   702,   746,    -1,   700,   478,   702,   746,    -1,   703,
      -1,   701,   167,   486,   717,   487,    -1,   701,   216,   167,
     486,   717,   487,    -1,   701,   167,   980,    -1,   701,   216,
     167,   980,    -1,   701,   358,   701,    87,   696,    -1,   701,
     216,   358,   701,    87,   696,    -1,   701,   227,   696,    -1,
     701,   226,   696,    -1,   701,   310,   696,    -1,   701,    12,
     177,   696,    -1,   701,   177,   703,   755,    -1,   701,   216,
     177,   703,   755,    -1,   701,   247,   696,    -1,   701,   216,
     247,   696,    -1,   701,    18,   218,    -1,   701,    18,   216,
     218,    -1,   701,    11,   696,    -1,   701,   697,   696,    -1,
     701,   697,   698,   980,    -1,   701,    20,   696,    -1,   701,
      21,   696,    -1,   701,   479,   696,    -1,   701,   478,   696,
      -1,   701,   480,   696,    -1,   701,   481,   696,    -1,   701,
       9,   696,    -1,   701,   476,   696,    -1,   701,   483,   696,
      -1,   701,   477,   696,    -1,   701,   482,   696,    -1,   701,
      19,   696,    -1,   701,   479,   702,   746,    -1,   701,   478,
     702,   746,    -1,   703,    -1,   405,   696,    -1,   899,    -1,
     703,   109,   907,    -1,   894,    -1,   892,    -1,   492,   907,
      22,   696,    -1,   492,   907,    -1,   492,   492,   916,   907,
     609,    -1,   707,    -1,   479,   696,    -1,   478,   696,    -1,
     484,   696,    -1,   216,   696,    -1,   489,   696,    -1,   486,
     696,   487,    -1,   486,   696,   488,   717,   487,    -1,   258,
     486,   696,   488,   717,   487,    -1,   135,   978,    -1,   976,
      -1,   490,   906,   696,   491,    -1,   201,   720,   356,   486,
     696,   705,   487,    -1,   314,   486,   696,   487,    -1,    94,
     696,    -1,   104,   486,   696,    88,   716,   487,    -1,   362,
     724,   459,   726,   725,   465,    -1,   115,   486,   696,   488,
     716,   487,    -1,   115,   486,   696,   301,   580,   487,    -1,
     119,   486,   899,   487,    -1,   303,   486,   899,   487,    -1,
     286,   486,   487,    -1,   287,   486,   696,   487,    -1,   288,
     486,   696,   488,   696,   487,    -1,   289,   486,   696,   488,
     696,   488,   696,   487,    -1,    78,   486,   696,   488,   696,
     487,    -1,    78,   486,   696,   488,   405,   696,   746,   487,
      -1,   357,   486,   696,   487,    -1,   357,   486,   696,   488,
     696,   487,    -1,   317,   486,   717,   487,    -1,   105,   486,
     696,   487,    -1,   319,   486,   717,   487,    -1,   110,   486,
     696,   487,    -1,   363,   486,   717,   487,    -1,   364,   486,
     696,   488,   717,   487,    -1,   365,   486,   696,   488,   696,
     488,   696,   487,    -1,   366,   695,    -1,   367,   695,    -1,
     367,   486,   696,   487,    -1,   116,   695,    -1,   369,   486,
     696,   488,   702,   746,   487,    -1,   370,   486,   696,   488,
     702,   746,   487,    -1,   368,   486,   487,    -1,   321,   486,
     696,   487,    -1,   375,   486,   696,   487,    -1,   380,   486,
     696,   488,   717,   487,    -1,   411,   486,   696,   488,   717,
     487,    -1,   384,   486,   696,   487,    -1,   384,   486,   696,
     488,   696,   487,    -1,   376,   486,   696,   488,   905,   487,
      -1,   381,   486,   696,   488,   905,   487,    -1,   378,   486,
     696,   487,    -1,   378,   486,   696,   488,   696,   487,    -1,
     377,   486,   696,   487,    -1,   377,   486,   696,   488,   696,
     487,    -1,   385,   486,   696,   488,   696,   488,   696,   487,
      -1,   385,   486,   696,   488,   696,   488,   696,   488,   696,
     487,    -1,   385,   486,   696,   488,   696,   488,   696,   488,
     696,   488,   696,   487,    -1,   137,    -1,   388,   486,   696,
     488,   219,   487,    -1,   390,   486,   696,   487,    -1,   390,
     486,   696,   488,   696,   487,    -1,   387,   486,   696,   488,
     717,   487,    -1,   704,    -1,   397,   486,   747,   488,   696,
     487,    -1,   401,   486,   696,   487,    -1,   403,   486,   696,
     488,   696,   488,   696,   487,    -1,    50,   486,   696,   488,
     696,   488,   696,   488,   696,   487,    -1,   702,   746,   479,
     696,    -1,   702,    -1,   406,   486,   487,    -1,   406,   486,
     696,   487,    -1,   407,   486,   696,   488,   696,   487,    -1,
     410,   486,   696,   488,   696,   487,    -1,   410,   486,   696,
     488,   696,   488,   696,   487,    -1,   150,   486,   696,   488,
     717,   487,    -1,   173,   486,   696,   488,   717,   487,    -1,
     180,   486,   696,   487,    -1,   180,   486,   696,   488,   696,
     487,    -1,   412,   486,   696,   488,   696,   487,    -1,   412,
     486,   696,   488,   696,   488,   696,   487,    -1,   413,   486,
     696,   487,    -1,   416,   486,   696,   487,    -1,    19,   486,
     696,   488,   696,   487,    -1,   419,   486,   696,   487,    -1,
     426,   695,    -1,   426,   486,   696,   487,    -1,   428,   486,
     696,   487,    -1,   427,   486,   696,   487,    -1,   433,   486,
     700,   167,   696,   487,    -1,   435,   486,   696,   487,    -1,
     435,   486,   487,    -1,   436,   486,   696,   488,   696,   488,
     696,   487,    -1,   437,   486,   696,   488,   696,   487,    -1,
     438,   486,   696,   487,    -1,   438,   486,   696,   488,   696,
     487,    -1,   442,   486,   696,   488,   696,   487,    -1,   442,
     486,   696,   488,   405,   696,   746,   487,    -1,   439,   486,
     696,   487,    -1,   443,   486,   696,   488,   696,   488,   696,
     487,    -1,   443,   486,   696,   488,   696,   487,    -1,   443,
     486,   696,   144,   696,   389,   696,   487,    -1,   443,   486,
     696,   144,   696,   487,    -1,   444,   486,   696,   488,   696,
     488,   696,   487,    -1,   346,   486,   696,   487,    -1,   345,
     486,   696,   487,    -1,   345,   486,   696,   488,   696,   487,
      -1,   445,   486,   696,   487,    -1,   445,   486,   172,   696,
     144,   696,   487,    -1,   445,   486,   281,   696,   144,   696,
     487,    -1,   445,   486,    98,   696,   144,   696,   487,    -1,
     445,   486,   172,   144,   696,   487,    -1,   445,   486,   281,
     144,   696,   487,    -1,   445,   486,    98,   144,   696,   487,
      -1,   445,   486,   696,   144,   696,   487,    -1,    80,   486,
     696,   488,   696,   487,    -1,   283,    -1,   446,   486,   706,
     487,    -1,   447,   486,   706,   487,    -1,   448,   486,   706,
     487,    -1,   449,   486,   706,   487,    -1,   450,   486,   706,
     487,    -1,   451,   486,   706,   487,    -1,   452,   486,   890,
     488,   219,   488,   219,   488,   717,   487,    -1,   453,   486,
     487,    -1,   453,   486,   696,   487,    -1,   454,   486,   487,
      -1,   455,   695,    -1,   456,   695,    -1,   457,   695,    -1,
     458,   486,   696,   487,    -1,   458,   486,   696,   488,   696,
     487,    -1,   462,   486,   696,   487,    -1,   463,   486,   696,
     487,    -1,   463,   486,   696,   488,   696,   487,    -1,   464,
     486,   769,   488,   696,   487,    -1,   386,   486,   746,   144,
     696,   487,    -1,   392,   486,   696,   487,    -1,   392,   486,
     696,   488,   696,   487,    -1,   393,   486,   696,   487,    -1,
     393,   486,   696,   488,   696,   487,    -1,   394,   486,   717,
     487,    -1,   409,   486,   717,   487,    -1,   423,   486,   717,
     487,    -1,   420,   486,   696,   487,    -1,   420,   486,   696,
     488,   696,   487,    -1,   421,   486,   696,   487,    -1,   421,
     486,   696,   488,   696,   487,    -1,   422,   486,   696,   487,
      -1,   422,   486,   696,   488,   696,   487,    -1,   424,   486,
     717,   487,    -1,   425,   486,   717,   487,    -1,   430,   486,
     696,   488,   696,   487,    -1,   429,   486,   696,   487,    -1,
     429,   486,   696,   488,   696,   487,    -1,   431,   486,   696,
     487,    -1,   431,   486,   696,   488,   696,   487,    -1,   432,
     486,   717,   487,    -1,   391,   486,   696,   487,    -1,   391,
     486,   696,   488,   696,   487,    -1,   408,   486,   696,   487,
      -1,   408,   486,   696,   488,   696,   487,    -1,    -1,   306,
     238,    47,    -1,   167,    97,   417,    -1,    -1,   717,    -1,
      29,   486,   714,   487,    -1,   359,   486,   714,   487,    -1,
     360,   486,   714,   487,    -1,   361,   486,   714,   487,    -1,
      37,   486,   750,   480,   487,    -1,    37,   486,   714,   487,
      -1,    -1,    -1,    37,   486,   127,   708,   717,   709,   487,
      -1,   396,   486,   890,   488,   219,   488,   219,   488,   714,
     487,    -1,    58,   486,   714,   487,    -1,    57,   486,   714,
     487,    -1,    74,   486,   714,   487,    -1,    75,   486,   714,
     487,    -1,    77,   486,   714,   487,    -1,    -1,   395,   486,
     711,   710,   717,   713,   712,   487,    -1,    -1,   127,    -1,
      -1,   261,   891,    -1,    -1,   760,    -1,    -1,   750,   715,
     696,    -1,    94,   573,    -1,   317,   573,   588,    -1,   213,
     573,    -1,   341,    -1,   341,   328,    -1,   351,    -1,   351,
     328,    -1,   321,    -1,   346,    -1,   320,    -1,    -1,   718,
     719,    -1,   696,    -1,   719,   488,   696,    -1,   721,    -1,
     486,   721,   487,    -1,    -1,   722,   723,    -1,   899,    -1,
     723,   488,   899,    -1,    -1,   696,    -1,    -1,   379,   696,
      -1,    -1,   727,   728,    -1,   696,   466,   696,    -1,   728,
     459,   696,   466,   696,    -1,   486,   729,   487,    -1,   734,
      -1,   729,   488,   729,    -1,   729,   733,   729,    -1,   729,
     272,   729,    -1,   729,   733,   729,   221,   696,    -1,    -1,
     729,   733,   729,   301,   730,   486,   745,   487,    -1,   729,
     407,   739,   169,   729,   221,   696,    -1,    -1,   729,   407,
     739,   169,   729,   731,   301,   486,   745,   487,    -1,   729,
     210,   407,   739,   169,   729,    -1,   729,   437,   739,   169,
     729,   221,   696,    -1,    -1,   729,   437,   739,   169,   729,
     732,   301,   486,   745,   487,    -1,   729,   210,   437,   739,
     169,   729,    -1,   729,   210,   169,   729,    -1,   169,    -1,
     164,   169,    -1,    39,   169,    -1,    -1,   735,   901,   749,
     740,    -1,   490,   906,   734,   407,   229,   169,   734,   221,
     696,   491,    -1,   486,    68,   736,   487,   749,    -1,    -1,
      -1,   737,   685,   689,   738,   545,   971,    -1,    -1,   229,
      -1,    -1,   300,   741,    -1,   142,   741,    -1,   159,   741,
      -1,    -1,   600,   742,   486,   743,   487,    -1,    -1,   744,
      -1,   744,   488,   906,    -1,   906,    -1,   234,    -1,   906,
      -1,   745,   488,   906,    -1,   371,    -1,   372,    -1,   373,
      -1,   374,    -1,   375,    -1,   398,    -1,   399,    -1,   400,
      -1,   401,    -1,   413,    -1,   414,    -1,   415,    -1,   416,
      -1,   419,    -1,   440,    -1,   439,    -1,   461,    -1,   462,
      -1,   321,    -1,   346,    -1,   320,    -1,   345,    -1,    -1,
      88,    -1,    10,    -1,    -1,   748,   906,    -1,    -1,    86,
      -1,    -1,    -1,   305,   752,   696,    -1,    -1,    -1,   152,
     754,   696,    -1,   134,   703,    -1,    -1,    -1,   151,   100,
     757,   758,    -1,   757,   488,   898,   763,    -1,   898,   763,
      -1,    -1,   306,    40,    -1,   306,    66,    -1,    -1,   760,
      -1,    -1,   228,   100,   761,   762,    -1,   762,   488,   898,
     763,    -1,   898,   763,    -1,    -1,    89,    -1,   122,    -1,
      -1,   766,    -1,    -1,   766,    -1,   329,   767,    -1,   769,
      -1,   769,   488,   769,    -1,   769,   220,   769,    -1,    -1,
     329,   770,    -1,   219,    -1,   182,    -1,   350,    -1,   245,
      -1,   141,    -1,   219,    -1,   350,    -1,   182,    -1,   245,
      -1,   141,    -1,    -1,    -1,   434,   906,   772,   486,   773,
     487,    -1,    -1,   774,    -1,   774,   488,   775,    -1,   775,
      -1,   691,   696,    -1,    -1,   777,   778,    -1,   778,   488,
     779,    -1,   779,    -1,   492,   907,    -1,    -1,   166,   230,
     904,   781,   883,   886,    -1,   166,   231,   904,    -1,   166,
     776,    -1,    -1,    43,   783,   717,    -1,    44,   790,   924,
     789,   787,   623,    -1,    -1,    44,   161,   906,   221,   901,
     785,    -1,    44,   368,   789,   906,    -1,    44,   292,   903,
      -1,    -1,    44,   454,   786,   953,    -1,   788,    -1,   787,
     488,   788,    -1,   901,    -1,    -1,   403,   135,    -1,    -1,
     277,    -1,    -1,    -1,    50,   792,   798,   622,   800,   793,
     802,   819,    -1,    -1,    -1,    -1,   436,   795,   799,   800,
     796,   802,   797,    -1,    -1,   184,    -1,   120,    -1,   155,
      -1,   828,    -1,   120,    -1,   166,   801,    -1,   801,    -1,
     788,    -1,   806,    -1,   486,   487,   806,    -1,   486,   805,
     487,   806,    -1,    -1,   260,   803,   810,    -1,    -1,   486,
     805,   487,    -1,   486,   487,    -1,   805,   488,   896,    -1,
     896,    -1,   303,   809,    -1,   302,   809,    -1,    -1,   524,
     807,   968,    -1,    -1,   486,   524,   487,   808,   971,    -1,
     809,   488,   814,    -1,   814,    -1,   810,   488,   811,    -1,
     811,    -1,   899,   812,   818,    -1,    10,    -1,    22,    -1,
      -1,   812,    -1,    -1,   486,   815,   816,   487,    -1,    -1,
     817,    -1,   817,   488,   818,    -1,   818,    -1,   696,    -1,
     119,    -1,    -1,    -1,   221,   128,   820,   171,    83,   826,
      -1,    -1,    -1,    83,   822,   828,   622,   729,   260,   824,
     823,   751,   759,   768,    -1,   824,   488,   825,    -1,   825,
      -1,   899,   812,   818,    -1,   826,   488,   827,    -1,   827,
      -1,   899,   812,   818,    -1,    -1,   184,    -1,    -1,    41,
     830,   838,   831,    -1,    -1,   144,   901,   832,   751,   759,
     768,    -1,    -1,   835,   833,   144,   729,   751,    -1,    -1,
     144,   835,   834,   301,   729,   751,    -1,   836,    -1,   835,
     488,   836,    -1,   906,   837,   749,    -1,   906,   493,   906,
     837,   749,    -1,    -1,   493,   480,    -1,    -1,   839,   838,
      -1,   339,    -1,   184,    -1,   159,    -1,    80,   841,   788,
      -1,    -1,   275,    -1,    -1,    69,   843,   844,    -1,   117,
     851,    -1,   274,   850,   851,    -1,   275,   270,   850,   851,
      -1,   223,   274,   850,   851,    -1,    -1,   382,   541,   845,
     847,    -1,   852,   111,   853,   901,   850,   851,    -1,   212,
      56,   389,    70,   306,   187,    10,   904,    87,   188,    10,
     770,    87,   192,    10,   769,    -1,   848,   181,    -1,    70,
     156,    -1,    -1,    31,    45,   854,   855,   846,   764,    -1,
     602,   853,   901,   850,    -1,   112,   285,    -1,   275,   285,
      -1,   849,   383,    -1,   235,    -1,    37,   486,   480,   487,
     313,    -1,    37,   486,   480,   487,   312,    -1,   313,   764,
      -1,   312,   764,    -1,   270,   851,    -1,   165,   270,    -1,
     852,   237,    -1,   915,   304,   851,    -1,   579,   851,    -1,
     110,   851,    -1,    93,   181,    -1,   181,    -1,   149,    -1,
     149,   389,   908,    -1,    38,   368,   534,   906,    -1,    38,
     275,   901,    -1,    56,   270,    -1,    70,   270,    -1,   270,
      -1,   181,    -1,    56,    -1,    94,    -1,    -1,   271,    -1,
      -1,   853,   906,    -1,    -1,   177,   904,    -1,    -1,   145,
      -1,   144,    -1,   167,    -1,    -1,   167,   904,    -1,    -1,
     144,   770,    -1,    -1,   859,   901,   857,   861,    -1,    -1,
     859,   860,   858,   675,    -1,   122,    -1,   123,    -1,    -1,
     136,    -1,    -1,   891,    -1,   906,    -1,    -1,    48,   658,
     863,   864,    -1,   864,   488,   865,    -1,   865,    -1,    -1,
     924,   866,   867,    -1,   274,   306,   244,    55,    -1,   238,
     102,    -1,   156,    -1,   235,    -1,   181,    -1,   270,    -1,
      70,    -1,    56,    -1,   124,    -1,   252,    -1,    -1,   787,
      -1,    -1,    64,   869,   870,    -1,   870,   488,   871,    -1,
     871,    -1,    70,    -1,    56,    -1,   238,   102,    -1,    -1,
      61,   873,   874,    -1,   848,   181,   875,    -1,   280,   904,
      -1,   475,   696,    -1,    52,   696,    -1,   300,   906,    -1,
      -1,    53,   118,   881,   880,   163,   904,   879,   882,   166,
     275,   901,   883,   886,   889,   804,    -1,    53,   275,   901,
     144,    56,    -1,    53,   118,   144,    56,    -1,    -1,   179,
      -1,    -1,   113,    -1,   184,    -1,    -1,   436,    -1,   159,
      -1,    -1,   111,   884,    -1,   884,   885,    -1,   885,    -1,
     278,   100,   891,    -1,   225,   131,   100,   891,    -1,   131,
     100,   891,    -1,   132,   100,   891,    -1,    -1,   178,   887,
      -1,   887,   888,    -1,   888,    -1,   278,   100,   891,    -1,
     269,   100,   891,    -1,    -1,   159,   219,   178,    -1,   905,
      -1,   214,    -1,   294,   279,    -1,   890,   905,    -1,   905,
      -1,   154,    -1,   494,    -1,   894,    -1,   479,   895,    -1,
     478,   895,    -1,   890,    -1,   895,    -1,   218,    -1,   154,
      -1,   294,   154,    -1,   321,   890,    -1,   346,   890,    -1,
     345,   890,    -1,   219,    -1,   182,    -1,   350,    -1,   245,
      -1,   141,    -1,   899,    -1,   897,    -1,   906,   493,   480,
      -1,   906,   493,   906,   493,   480,    -1,   696,    -1,   906,
      -1,   906,   493,   906,    -1,   493,   906,   493,   906,    -1,
     906,   493,   906,   493,   906,    -1,   906,    -1,   906,   493,
     906,    -1,   493,   906,    -1,   906,    -1,   906,   493,   906,
      -1,   493,   906,    -1,   906,    -1,   157,    -1,   158,    -1,
     279,    -1,   279,    -1,   903,    -1,   909,    -1,   906,    -1,
     904,    -1,   176,    -1,   907,    -1,   907,   492,   907,    -1,
     116,   695,    -1,    84,    -1,    78,    -1,    25,    -1,   356,
      -1,    85,    -1,    28,    -1,   314,    -1,    90,    -1,    91,
      -1,    29,    -1,    92,    -1,    30,    -1,    93,    -1,    31,
      -1,    95,    -1,    96,    -1,    97,    -1,   101,    -1,    99,
      -1,   102,    -1,   318,    -1,   105,    -1,   106,    -1,   474,
      -1,    33,    -1,     4,    -1,   110,    -1,    34,    -1,   108,
      -1,    35,    -1,   311,    -1,   113,    -1,    36,    -1,    40,
      -1,   118,    -1,   320,    -1,   321,    -1,   375,    -1,   338,
      -1,   121,    -1,   124,    -1,   133,    -1,   126,    -1,    43,
      -1,   231,    -1,   128,    -1,   129,    -1,   465,    -1,   324,
      -1,   382,    -1,   383,    -1,   312,    -1,   134,    -1,    45,
      -1,    46,    -1,    47,    -1,   136,    -1,   325,    -1,   125,
      -1,   130,    -1,   145,    -1,   138,    -1,   139,    -1,   140,
      -1,    48,    -1,   327,    -1,   394,    -1,   397,    -1,   149,
      -1,   147,    -1,     5,    -1,   153,    -1,    49,    -1,   156,
      -1,   401,    -1,   402,    -1,   160,    -1,   162,    -1,   168,
      -1,   473,    -1,   165,    -1,   404,    -1,    51,    -1,     6,
      -1,   174,    -1,   175,    -1,   409,    -1,   179,    -1,    54,
      -1,   181,    -1,   202,    -1,    56,    -1,   185,    -1,   190,
      -1,   187,    -1,   188,    -1,   186,    -1,   189,    -1,   192,
      -1,   191,    -1,   193,    -1,   194,    -1,   195,    -1,   196,
      -1,   197,    -1,   198,    -1,   203,    -1,   204,    -1,   205,
      -1,   206,    -1,   413,    -1,   416,    -1,   207,    -1,   418,
      -1,   417,    -1,   419,    -1,   423,    -1,   424,    -1,   425,
      -1,   208,    -1,   209,    -1,   213,    -1,   211,    -1,     7,
      -1,   212,    -1,   217,    -1,    59,    -1,   215,    -1,   220,
      -1,   427,    -1,   222,    -1,   223,    -1,   232,    -1,   233,
      -1,   428,    -1,   430,    -1,   432,    -1,   337,    -1,     8,
      -1,   236,    -1,   237,    -1,   238,    -1,   339,    -1,   239,
      -1,   242,    -1,   243,    -1,   240,    -1,   241,    -1,   199,
      -1,   200,    -1,   248,    -1,    62,    -1,   250,    -1,    63,
      -1,    64,    -1,   252,    -1,   253,    -1,    65,    -1,    66,
      -1,   256,    -1,   257,    -1,   258,    -1,   259,    -1,    67,
      -1,   439,    -1,   262,    -1,   263,    -1,   264,    -1,   341,
      -1,   265,    -1,   441,    -1,   266,    -1,    70,    -1,    71,
      -1,    12,    -1,   468,    -1,   472,    -1,   470,    -1,    72,
      -1,    73,    -1,   270,    -1,    76,    -1,   271,    -1,   343,
      -1,   442,    -1,   273,    -1,    79,    -1,   276,    -1,   277,
      -1,   344,    -1,   282,    -1,    80,    -1,   345,    -1,   346,
      -1,   284,    -1,   285,    -1,   290,    -1,   292,    -1,   293,
      -1,   295,    -1,    82,    -1,   454,    -1,   299,    -1,   304,
      -1,   302,    -1,   313,    -1,   460,    -1,   309,    -1,   462,
      -1,    -1,   260,   912,   911,   913,    -1,    -1,   224,    -1,
     914,   917,    -1,   913,   488,   914,   917,    -1,    -1,   147,
      -1,   179,    -1,   264,    -1,   222,    -1,    -1,   147,    -1,
     179,    -1,   264,    -1,    -1,   147,   493,    -1,   179,   493,
      -1,   264,   493,    -1,   492,   907,   812,   696,    -1,   918,
     812,   921,    -1,   492,   492,   916,   918,   812,   921,    -1,
     282,   168,   175,   919,    -1,   579,   583,    -1,   208,   581,
     585,    -1,   428,   812,   920,    -1,   428,   389,   908,   812,
     920,    -1,   906,    -1,   906,   493,   906,    -1,   119,   493,
     906,    -1,   244,   293,    -1,   244,   108,    -1,   250,   244,
      -1,   263,    -1,   279,    -1,   428,   486,   279,   487,    -1,
     427,   486,   279,   487,    -1,   696,    -1,   119,    -1,   221,
      -1,    86,    -1,    94,    -1,    -1,    55,   924,   923,   925,
      -1,   275,    -1,   274,    -1,   926,    -1,   925,   488,   926,
      -1,   901,   749,   927,    -1,   244,    -1,   307,    -1,   184,
     307,    -1,   244,   179,    -1,    81,   924,    -1,     5,   901,
     223,   749,    -1,     5,   902,     4,    -1,    -1,     5,   902,
     244,   930,   931,   751,   765,    -1,   932,    -1,   906,   933,
      -1,   139,    -1,     7,    -1,   139,    -1,     7,    -1,     8,
      -1,     6,    -1,    -1,   935,   934,   486,   817,   487,    -1,
      10,    -1,    13,    -1,    15,    -1,    14,    -1,    16,    -1,
      -1,   255,   937,   938,    -1,   941,   221,   952,   144,   953,
      -1,    86,   942,   488,   148,   224,   144,   953,    -1,    -1,
     148,   940,   941,   221,   952,   280,   953,   958,   959,    -1,
     943,    -1,    86,   942,    -1,    -1,   235,    -1,   944,    -1,
     943,   488,   944,    -1,    -1,    68,   945,   955,    -1,    -1,
      50,   946,   955,    -1,    -1,    83,   947,   955,    -1,    -1,
     246,   948,   955,    -1,    41,    -1,   298,    -1,   161,    -1,
      26,    -1,    38,    -1,    44,    -1,    46,    -1,   248,    -1,
     266,    -1,   236,    -1,   138,    -1,   148,   224,    -1,    69,
     117,    -1,    79,    -1,    38,   277,   274,    -1,    55,   274,
      -1,    63,    70,    -1,    63,    33,    -1,    -1,    87,    -1,
     951,   949,   950,    -1,   951,    -1,   273,   279,    -1,   473,
     279,    -1,   474,   279,    -1,   480,    -1,   906,   493,   480,
      -1,   480,   493,   480,    -1,   901,    -1,   954,    -1,   953,
     488,   954,    -1,   908,   402,   100,   279,    -1,   908,   402,
     100,   428,   279,    -1,   908,    -1,    -1,   486,   956,   487,
      -1,   956,   488,   957,    -1,   957,    -1,   906,    -1,    -1,
     251,   950,    -1,   251,   268,    -1,   251,   309,    -1,   251,
      59,    -1,    -1,   306,   960,    -1,   960,   961,    -1,   961,
      -1,   148,   224,    -1,   204,   769,    -1,   205,   769,    -1,
     203,   769,    -1,    -1,    30,   963,   964,    -1,    -1,   460,
      -1,    35,    -1,    65,    -1,    65,   280,    67,   906,    -1,
      67,   906,    -1,    -1,   969,    -1,    -1,   296,   975,   970,
     676,    -1,   969,    -1,   972,    -1,    -1,    -1,   973,   974,
      -1,   760,   764,    -1,   766,    -1,    -1,   127,    -1,    86,
      -1,   982,   977,   983,    -1,   678,    -1,   982,   979,   983,
      -1,   678,    -1,   982,   981,   983,    -1,   678,    -1,   486,
      68,    -1,   487,    -1
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
    2393,  2392,  2415,  2414,  2434,  2443,  2434,  2449,  2450,  2451,
    2452,  2453,  2456,  2458,  2465,  2467,  2470,  2471,  2474,  2475,
    2481,  2482,  2483,  2484,  2490,  2496,  2497,  2501,  2504,  2506,
    2512,  2522,  2523,  2524,  2534,  2545,  2548,  2551,  2552,  2555,
    2556,  2557,  2558,  2559,  2563,  2564,  2568,  2569,  2572,  2573,
    2574,  2575,  2576,  2577,  2580,  2581,  2586,  2588,  2590,  2592,
    2596,  2598,  2600,  2601,  2602,  2603,  2604,  2609,  2611,  2613,
    2614,  2616,  2617,  2618,  2619,  2620,  2624,  2625,  2626,  2627,
    2628,  2629,  2630,  2631,  2632,  2633,  2634,  2635,  2636,  2638,
    2644,  2646,  2648,  2649,  2650,  2651,  2652,  2657,  2659,  2661,
    2662,  2664,  2665,  2666,  2667,  2668,  2672,  2673,  2674,  2675,
    2676,  2677,  2678,  2679,  2680,  2681,  2682,  2683,  2684,  2686,
    2688,  2692,  2694,  2696,  2698,  2702,  2704,  2706,  2707,  2708,
    2709,  2714,  2716,  2718,  2719,  2721,  2722,  2723,  2724,  2725,
    2729,  2730,  2731,  2732,  2733,  2734,  2735,  2736,  2737,  2738,
    2739,  2740,  2741,  2743,  2745,  2748,  2752,  2753,  2760,  2761,
    2762,  2767,  2772,  2783,  2784,  2785,  2786,  2787,  2791,  2795,
    2796,  2801,  2806,  2807,  2808,  2809,  2813,  2814,  2818,  2824,
    2826,  2832,  2834,  2836,  2838,  2849,  2860,  2871,  2882,  2884,
    2886,  2888,  2890,  2892,  2894,  2896,  2898,  2900,  2902,  2907,
    2909,  2911,  2916,  2918,  2920,  2922,  2927,  2929,  2931,  2933,
    2935,  2940,  2941,  2943,  2945,  2947,  2949,  2951,  2953,  2955,
    2957,  2959,  2961,  2963,  2965,  2969,  2971,  2981,  2983,  2985,
    2987,  2989,  2992,  3001,  3006,  3011,  3013,  3015,  3017,  3019,
    3021,  3023,  3025,  3030,  3035,  3037,  3039,  3041,  3043,  3045,
    3047,  3053,  3055,  3057,  3059,  3061,  3063,  3065,  3067,  3068,
    3070,  3072,  3074,  3076,  3078,  3080,  3082,  3084,  3086,  3088,
    3090,  3092,  3094,  3096,  3098,  3100,  3102,  3104,  3106,  3108,
    3110,  3117,  3124,  3131,  3138,  3145,  3152,  3156,  3161,  3163,
    3165,  3167,  3169,  3171,  3176,  3178,  3180,  3182,  3184,  3189,
    3193,  3195,  3197,  3199,  3201,  3205,  3208,  3211,  3213,  3215,
    3217,  3219,  3221,  3223,  3226,  3229,  3231,  3233,  3235,  3237,
    3239,  3242,  3244,  3246,  3248,  3253,  3254,  3255,  3259,  3260,
    3263,  3265,  3267,  3269,  3271,  3273,  3276,  3278,  3275,  3281,
    3283,  3285,  3287,  3289,  3291,  3294,  3293,  3305,  3306,  3309,
    3310,  3315,  3318,  3330,  3329,  3345,  3346,  3347,  3348,  3349,
    3350,  3351,  3352,  3353,  3354,  3358,  3358,  3363,  3364,  3367,
    3368,  3371,  3371,  3376,  3377,  3380,  3381,  3384,  3385,  3388,
    3388,  3393,  3399,  3407,  3408,  3409,  3410,  3411,  3413,  3417,
    3415,  3425,  3428,  3427,  3435,  3441,  3444,  3443,  3451,  3457,
    3461,  3462,  3463,  3467,  3467,  3483,  3485,  3499,  3517,  3499,
    3524,  3525,  3528,  3529,  3535,  3542,  3550,  3550,  3556,  3557,
    3561,  3565,  3569,  3575,  3584,  3592,  3593,  3594,  3595,  3596,
    3597,  3598,  3599,  3600,  3601,  3602,  3603,  3604,  3605,  3606,
    3607,  3608,  3609,  3613,  3614,  3615,  3616,  3619,  3621,  3622,
    3625,  3626,  3629,  3631,  3635,  3637,  3636,  3650,  3653,  3652,
    3667,  3669,  3679,  3681,  3684,  3686,  3690,  3691,  3704,  3721,
    3723,  3727,  3726,  3741,  3743,  3747,  3748,  3749,  3754,  3760,
    3764,  3765,  3769,  3773,  3780,  3787,  3799,  3803,  3811,  3812,
    3813,  3814,  3815,  3819,  3820,  3821,  3822,  3823,  3826,  3829,
    3828,  3849,  3850,  3853,  3854,  3857,  3869,  3869,  3879,  3880,
    3883,  3893,  3892,  3901,  3913,  3924,  3923,  3940,  3947,  3947,
    3958,  3965,  3972,  3971,  3983,  3984,  3987,  3995,  3996,  4000,
    4001,  4009,  4019,  4008,  4029,  4037,  4042,  4028,  4047,  4048,
    4049,  4050,  4054,  4055,  4058,  4059,  4062,  4071,  4072,  4073,
    4075,  4074,  4084,  4085,  4086,  4089,  4090,  4093,  4094,  4095,
    4095,  4096,  4096,  4100,  4101,  4104,  4106,  4109,  4117,  4118,
    4122,  4123,  4128,  4127,  4140,  4141,  4144,  4149,  4157,  4158,
    4161,  4163,  4163,  4171,  4180,  4170,  4201,  4202,  4205,  4212,
    4213,  4216,  4225,  4226,  4232,  4231,  4245,  4244,  4253,  4252,
    4256,  4255,  4262,  4263,  4266,  4272,  4283,  4284,  4288,  4289,
    4292,  4293,  4294,  4297,  4306,  4308,  4313,  4312,  4323,  4325,
    4331,  4338,  4345,  4344,  4347,  4355,  4365,  4369,  4374,  4373,
    4378,  4386,  4391,  4397,  4402,  4407,  4409,  4411,  4413,  4415,
    4417,  4419,  4421,  4427,  4429,  4431,  4433,  4435,  4458,  4465,
    4471,  4477,  4481,  4487,  4498,  4511,  4512,  4514,  4516,  4519,
    4520,  4522,  4524,  4529,  4530,  4533,  4534,  4537,  4538,  4541,
    4542,  4548,  4547,  4558,  4557,  4567,  4568,  4571,  4572,  4576,
    4577,  4578,  4586,  4585,  4596,  4597,  4600,  4600,  4601,  4602,
    4603,  4604,  4605,  4606,  4607,  4608,  4609,  4610,  4613,  4614,
    4618,  4617,  4626,  4627,  4630,  4631,  4632,  4636,  4635,  4644,
    4648,  4653,  4674,  4688,  4698,  4697,  4716,  4724,  4730,  4731,
    4734,  4735,  4736,  4740,  4741,  4742,  4744,  4746,  4749,  4750,
    4753,  4758,  4765,  4770,  4776,  4778,  4781,  4782,  4785,  4790,
    4796,  4798,  4807,  4812,  4814,  4816,  4821,  4823,  4837,  4860,
    4861,  4862,  4871,  4872,  4873,  4875,  4876,  4890,  4891,  4892,
    4895,  4896,  4897,  4898,  4899,  4907,  4908,  4911,  4916,  4926,
    4929,  4937,  4953,  4969,  4992,  4993,  4994,  4997,  4998,  4999,
    5003,  5007,  5008,  5032,  5044,  5057,  5058,  5067,  5068,  5069,
    5072,  5081,  5088,  5110,  5111,  5112,  5113,  5114,  5115,  5116,
    5117,  5118,  5119,  5120,  5121,  5122,  5123,  5124,  5125,  5126,
    5127,  5128,  5129,  5130,  5131,  5132,  5133,  5134,  5135,  5136,
    5137,  5138,  5139,  5140,  5141,  5142,  5143,  5144,  5145,  5146,
    5147,  5148,  5149,  5150,  5151,  5152,  5153,  5154,  5155,  5156,
    5157,  5158,  5159,  5160,  5161,  5162,  5163,  5164,  5165,  5166,
    5167,  5168,  5169,  5170,  5171,  5172,  5173,  5174,  5175,  5176,
    5177,  5178,  5179,  5180,  5181,  5182,  5183,  5184,  5185,  5186,
    5187,  5188,  5189,  5190,  5191,  5192,  5193,  5194,  5195,  5196,
    5197,  5198,  5199,  5200,  5201,  5202,  5203,  5204,  5205,  5206,
    5207,  5208,  5209,  5210,  5211,  5212,  5213,  5214,  5215,  5216,
    5217,  5218,  5219,  5220,  5221,  5222,  5223,  5224,  5225,  5226,
    5227,  5228,  5229,  5230,  5231,  5232,  5233,  5234,  5235,  5236,
    5237,  5238,  5239,  5240,  5241,  5242,  5243,  5244,  5245,  5246,
    5247,  5248,  5249,  5250,  5251,  5252,  5253,  5254,  5255,  5256,
    5257,  5258,  5259,  5260,  5261,  5262,  5263,  5264,  5265,  5266,
    5267,  5268,  5269,  5270,  5271,  5272,  5273,  5274,  5275,  5276,
    5277,  5278,  5279,  5280,  5281,  5282,  5283,  5284,  5285,  5286,
    5287,  5288,  5289,  5290,  5291,  5292,  5293,  5294,  5295,  5296,
    5297,  5298,  5299,  5300,  5301,  5302,  5303,  5304,  5305,  5306,
    5307,  5308,  5309,  5310,  5311,  5312,  5313,  5314,  5315,  5316,
    5317,  5318,  5325,  5324,  5338,  5339,  5342,  5343,  5346,  5347,
    5348,  5349,  5350,  5354,  5355,  5356,  5357,  5361,  5362,  5363,
    5364,  5368,  5372,  5378,  5384,  5395,  5402,  5415,  5425,  5432,
    5447,  5462,  5476,  5477,  5478,  5479,  5483,  5484,  5491,  5500,
    5501,  5502,  5503,  5504,  5512,  5511,  5520,  5521,  5524,  5525,
    5528,  5536,  5537,  5538,  5539,  5543,  5552,  5559,  5567,  5566,
    5580,  5581,  5585,  5586,  5590,  5591,  5592,  5593,  5595,  5594,
    5605,  5606,  5607,  5608,  5609,  5616,  5615,  5632,  5635,  5643,
    5642,  5660,  5661,  5664,  5666,  5670,  5671,  5674,  5674,  5675,
    5675,  5676,  5676,  5677,  5677,  5678,  5679,  5680,  5681,  5682,
    5683,  5684,  5685,  5686,  5687,  5688,  5689,  5690,  5691,  5692,
    5693,  5694,  5695,  5700,  5701,  5705,  5706,  5710,  5720,  5730,
    5743,  5755,  5767,  5779,  5791,  5792,  5801,  5826,  5828,  5835,
    5839,  5842,  5843,  5846,  5867,  5868,  5872,  5876,  5880,  5887,
    5888,  5891,  5892,  5896,  5897,  5902,  5907,  5915,  5915,  5919,
    5920,  5924,  5927,  5931,  5937,  5949,  5950,  5955,  5954,  5980,
    5981,  5985,  5987,  5987,  6011,  6012,  6016,  6017,  6018,  6022,
    6029,  6036,  6043,  6050,  6057,  6063,  6078
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
  "@34", "select_init2", "@35", "select_part2", "@36", "@37",
  "select_into", "select_from", "select_options", "select_option_list",
  "select_option", "select_lock_type", "select_item_list", "select_item",
  "remember_name", "remember_end", "select_item2", "select_alias",
  "optional_braces", "expr", "comp_op", "all_or_any", "expr_expr",
  "no_in_expr", "no_and_expr", "interval_expr", "simple_expr",
  "geometry_function", "fulltext_options", "udf_expr_list", "sum_expr",
  "@38", "@39", "@40", "opt_distinct", "opt_gconcat_separator",
  "opt_gorder_clause", "in_sum_expr", "@41", "cast_type", "expr_list",
  "@42", "expr_list2", "ident_list_arg", "ident_list", "@43",
  "ident_list2", "opt_expr", "opt_else", "when_list", "@44", "when_list2",
  "join_table_list", "@45", "@46", "@47", "normal_join", "join_table",
  "@48", "select_derived", "@49", "@50", "opt_outer", "opt_key_definition",
  "key_usage_list", "@51", "key_list_or_empty", "key_usage_list2",
  "using_list", "interval", "date_time_type", "table_alias",
  "opt_table_alias", "opt_all", "where_clause", "@52", "having_clause",
  "@53", "opt_escape", "group_clause", "group_list", "olap_opt",
  "opt_order_clause", "order_clause", "@54", "order_list", "order_dir",
  "opt_limit_clause_init", "opt_limit_clause", "limit_clause",
  "limit_options", "delete_limit_clause", "ULONG_NUM", "ulonglong_num",
  "procedure_clause", "@55", "procedure_list", "procedure_list2",
  "procedure_item", "select_var_list_init", "@56", "select_var_list",
  "select_var_ident", "into", "@57", "do", "@58", "drop", "@59", "@60",
  "table_list", "table_name", "if_exists", "opt_temporary", "insert",
  "@61", "@62", "replace", "@63", "@64", "@65", "insert_lock_option",
  "replace_lock_option", "insert2", "insert_table", "insert_field_spec",
  "@66", "opt_field_spec", "fields", "insert_values", "@67", "@68",
  "values_list", "ident_eq_list", "ident_eq_value", "equal", "opt_equal",
  "no_braces", "@69", "opt_values", "values", "expr_or_default",
  "opt_insert_update", "@70", "update", "@71", "@72", "update_list",
  "update_elem", "insert_update_list", "insert_update_elem",
  "opt_low_priority", "delete", "@73", "single_multi", "@74", "@75", "@76",
  "table_wild_list", "table_wild_one", "opt_wild", "opt_delete_options",
  "opt_delete_option", "truncate", "opt_table_sym", "show", "@77",
  "show_param", "@78", "@79", "show_engine_param", "master_or_binary",
  "opt_storage", "opt_db", "wild", "opt_full", "from_or_in", "binlog_in",
  "binlog_from", "describe", "@80", "@81", "describe_command",
  "opt_extended_describe", "opt_describe_column", "flush", "@82",
  "flush_options", "flush_option", "@83", "opt_table_list", "reset", "@84",
  "reset_options", "reset_option", "purge", "@85", "purge_options",
  "purge_option", "kill", "use", "load", "@86", "opt_local",
  "load_data_lock", "opt_duplicate", "opt_field_term", "field_term_list",
  "field_term", "opt_line_term", "line_term_list", "line_term",
  "opt_ignore_lines", "text_literal", "text_string", "param_marker",
  "signed_literal", "literal", "NUM_literal", "insert_ident", "table_wild",
  "order_ident", "simple_ident", "field_ident", "table_ident",
  "table_ident_nodb", "IDENT_sys", "TEXT_STRING_sys",
  "TEXT_STRING_literal", "ident", "ident_or_text", "user", "keyword",
  "set", "@87", "opt_option", "option_value_list", "option_type",
  "opt_var_type", "opt_var_ident_type", "option_value",
  "internal_variable_name", "isolation_types", "text_or_password",
  "set_expr_or_default", "lock", "@88", "table_or_tables",
  "table_lock_list", "table_lock", "lock_option", "unlock", "handler",
  "@89", "handler_read_or_scan", "handler_scan_function",
  "handler_rkey_function", "@90", "handler_rkey_mode", "revoke", "@91",
  "revoke_command", "grant", "@92", "grant_privileges", "opt_privileges",
  "grant_privilege_list", "grant_privilege", "@93", "@94", "@95", "@96",
  "opt_and", "require_list", "require_list_element", "opt_table",
  "user_list", "grant_user", "opt_column_list", "column_list",
  "column_list_id", "require_clause", "grant_options", "grant_option_list",
  "grant_option", "begin", "@97", "opt_work", "commit", "rollback",
  "savepoint", "union_clause", "union_list", "@98", "union_opt",
  "optional_order_or_limit", "@99", "order_or_limit", "union_option",
  "singlerow_subselect", "singlerow_subselect_init", "exists_subselect",
  "exists_subselect_init", "in_subselect", "in_subselect_init",
  "subselect_start", "subselect_end", 0
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
     677,   676,   679,   678,   681,   682,   680,   683,   683,   683,
     683,   683,   684,   684,   685,   685,   686,   686,   687,   687,
     687,   687,   687,   687,   687,   687,   687,   687,   688,   688,
     688,   689,   689,   689,   690,   691,   692,   693,   693,   694,
     694,   694,   694,   694,   695,   695,   696,   696,   697,   697,
     697,   697,   697,   697,   698,   698,   699,   699,   699,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     700,   700,   700,   700,   700,   700,   700,   700,   700,   700,
     700,   700,   700,   700,   700,   700,   700,   700,   700,   700,
     700,   700,   700,   700,   700,   700,   700,   700,   700,   700,
     700,   701,   701,   701,   701,   701,   701,   701,   701,   701,
     701,   701,   701,   701,   701,   701,   701,   701,   701,   701,
     701,   701,   701,   701,   701,   701,   701,   701,   701,   701,
     701,   701,   701,   701,   701,   702,   703,   703,   703,   703,
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
     703,   703,   703,   703,   703,   703,   703,   703,   703,   703,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   704,   704,   705,   705,   705,   706,   706,
     707,   707,   707,   707,   707,   707,   708,   709,   707,   707,
     707,   707,   707,   707,   707,   710,   707,   711,   711,   712,
     712,   713,   713,   715,   714,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   718,   717,   719,   719,   720,
     720,   722,   721,   723,   723,   724,   724,   725,   725,   727,
     726,   728,   728,   729,   729,   729,   729,   729,   729,   730,
     729,   729,   731,   729,   729,   729,   732,   729,   729,   729,
     733,   733,   733,   735,   734,   734,   734,   737,   738,   736,
     739,   739,   740,   740,   740,   740,   742,   741,   743,   743,
     744,   744,   744,   745,   745,   746,   746,   746,   746,   746,
     746,   746,   746,   746,   746,   746,   746,   746,   746,   746,
     746,   746,   746,   747,   747,   747,   747,   748,   748,   748,
     749,   749,   750,   750,   751,   752,   751,   753,   754,   753,
     755,   755,   756,   756,   757,   757,   758,   758,   758,   759,
     759,   761,   760,   762,   762,   763,   763,   763,   764,   764,
     765,   765,   766,   767,   767,   767,   768,   768,   769,   769,
     769,   769,   769,   770,   770,   770,   770,   770,   771,   772,
     771,   773,   773,   774,   774,   775,   777,   776,   778,   778,
     779,   781,   780,   780,   780,   783,   782,   784,   785,   784,
     784,   784,   786,   784,   787,   787,   788,   789,   789,   790,
     790,   792,   793,   791,   795,   796,   797,   794,   798,   798,
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
     895,   895,   895,   895,   895,   896,   896,   897,   897,   898,
     899,   899,   899,   899,   900,   900,   900,   901,   901,   901,
     902,   903,   903,   904,   905,   906,   906,   907,   907,   907,
     908,   908,   908,   909,   909,   909,   909,   909,   909,   909,
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
     909,   909,   911,   910,   912,   912,   913,   913,   914,   914,
     914,   914,   914,   915,   915,   915,   915,   916,   916,   916,
     916,   917,   917,   917,   917,   917,   917,   917,   917,   918,
     918,   918,   919,   919,   919,   919,   920,   920,   920,   921,
     921,   921,   921,   921,   923,   922,   924,   924,   925,   925,
     926,   927,   927,   927,   927,   928,   929,   929,   930,   929,
     931,   931,   932,   932,   933,   933,   933,   933,   934,   933,
     935,   935,   935,   935,   935,   937,   936,   938,   938,   940,
     939,   941,   941,   942,   942,   943,   943,   945,   944,   946,
     944,   947,   944,   948,   944,   944,   944,   944,   944,   944,
     944,   944,   944,   944,   944,   944,   944,   944,   944,   944,
     944,   944,   944,   949,   949,   950,   950,   951,   951,   951,
     952,   952,   952,   952,   953,   953,   954,   954,   954,   955,
     955,   956,   956,   957,   958,   958,   958,   958,   958,   959,
     959,   960,   960,   961,   961,   961,   961,   963,   962,   964,
     964,   965,   966,   966,   967,   968,   968,   970,   969,   971,
     971,   972,   973,   972,   974,   974,   975,   975,   975,   976,
     977,   978,   979,   980,   981,   982,   983
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
       0,     6,     0,     3,     0,     0,     6,     1,     1,     1,
       2,     2,     8,     3,     0,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     2,
       4,     3,     1,     1,     4,     0,     0,     1,     1,     0,
       2,     2,     1,     1,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     6,     3,     4,
       5,     6,     3,     3,     3,     3,     4,     4,     5,     3,
       4,     3,     4,     3,     3,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     4,
       5,     6,     3,     3,     3,     3,     4,     4,     5,     3,
       4,     3,     4,     3,     3,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     4,
       1,     5,     6,     3,     4,     5,     6,     3,     3,     3,
       4,     4,     5,     3,     4,     3,     4,     3,     3,     4,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     1,     2,     1,     3,     1,     1,
       4,     2,     5,     1,     2,     2,     2,     2,     2,     3,
       5,     6,     2,     1,     4,     7,     4,     2,     6,     6,
       6,     6,     4,     4,     3,     4,     6,     8,     6,     8,
       4,     6,     4,     4,     4,     4,     4,     6,     8,     2,
       2,     4,     2,     7,     7,     3,     4,     4,     6,     6,
       4,     6,     6,     6,     4,     6,     4,     6,     8,    10,
      12,     1,     6,     4,     6,     6,     1,     6,     4,     8,
      10,     4,     1,     3,     4,     6,     6,     8,     6,     6,
       4,     6,     6,     8,     4,     4,     6,     4,     2,     4,
       4,     4,     6,     4,     3,     8,     6,     4,     6,     6,
       8,     4,     8,     6,     8,     6,     8,     4,     4,     6,
       4,     7,     7,     7,     6,     6,     6,     6,     6,     1,
       4,     4,     4,     4,     4,     4,    10,     3,     4,     3,
       2,     2,     2,     4,     6,     4,     4,     6,     6,     6,
       4,     6,     4,     6,     4,     4,     4,     4,     6,     4,
       6,     4,     6,     4,     4,     6,     4,     6,     4,     6,
       4,     4,     6,     4,     6,     0,     3,     3,     0,     1,
       4,     4,     4,     4,     5,     4,     0,     0,     7,    10,
       4,     4,     4,     4,     4,     0,     8,     0,     1,     0,
       2,     0,     1,     0,     3,     2,     3,     2,     1,     2,
       1,     2,     1,     1,     1,     0,     2,     1,     3,     1,
       3,     0,     2,     1,     3,     0,     1,     0,     2,     0,
       2,     3,     5,     3,     1,     3,     3,     3,     5,     0,
       8,     7,     0,    10,     6,     7,     0,    10,     6,     4,
       1,     2,     2,     0,     4,    10,     5,     0,     0,     6,
       0,     1,     0,     2,     2,     2,     0,     5,     0,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     1,
       0,     2,     0,     1,     0,     0,     3,     0,     0,     3,
       2,     0,     0,     4,     4,     2,     0,     2,     2,     0,
       1,     0,     4,     4,     2,     0,     1,     1,     0,     1,
       0,     1,     2,     1,     3,     3,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     0,
       6,     0,     1,     3,     1,     2,     0,     2,     3,     1,
       2,     0,     6,     3,     2,     0,     3,     6,     0,     6,
       4,     3,     0,     4,     1,     3,     1,     0,     2,     0,
       1,     0,     0,     8,     0,     0,     0,     7,     0,     1,
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
       2,     2,     1,     1,     1,     1,     2,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     3,     5,     1,
       1,     3,     4,     5,     1,     3,     2,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     1,
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
       1,     1,     0,     4,     0,     1,     2,     4,     0,     1,
       1,     1,     1,     0,     1,     1,     1,     0,     2,     2,
       2,     4,     3,     6,     4,     2,     3,     3,     5,     1,
       3,     3,     2,     2,     2,     1,     1,     4,     4,     1,
       1,     1,     1,     1,     0,     4,     1,     1,     1,     3,
       3,     1,     1,     2,     2,     2,     4,     3,     0,     7,
       1,     2,     1,     1,     1,     1,     1,     1,     0,     5,
       1,     1,     1,     1,     1,     0,     3,     5,     7,     0,
       9,     1,     2,     0,     1,     1,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     3,
       2,     2,     2,     0,     1,     3,     1,     2,     2,     2,
       1,     3,     3,     1,     1,     3,     4,     5,     1,     0,
       3,     3,     1,     1,     0,     2,     2,     2,     2,     0,
       2,     2,     1,     2,     2,     2,     2,     0,     3,     0,
       1,     1,     1,     4,     2,     0,     1,     0,     4,     1,
       1,     0,     0,     2,     2,     1,     0,     1,     1,     3,
       1,     3,     1,     3,     1,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     2,     0,   398,   463,  1637,     0,  1641,   114,  1084,
    1005,  1019,     0,   463,     0,  1021,     0,     0,     0,   463,
    1197,   463,  1190,  1642,     0,   494,  1106,     0,     0,     0,
    1104,     0,  1073,     0,     0,     0,     0,  1165,  1166,  1569,
       0,     0,  1565,  1494,     0,     0,    50,  1024,     0,     0,
       0,    13,     0,    30,    18,    22,     8,    12,     4,    43,
      44,    36,     6,    10,    33,     5,     9,    27,    32,    28,
      29,    40,   488,    16,    17,    23,    34,    47,    14,    45,
      42,    15,  1167,    19,    35,    31,    24,    48,    25,    41,
      26,    46,    21,    37,    20,     7,    11,    38,    39,  1308,
    1353,  1366,  1406,  1421,  1457,  1285,  1288,  1292,  1294,  1296,
    1307,  1310,  1312,  1315,  1316,  1326,  1336,  1337,  1338,  1347,
    1355,  1365,  1371,  1374,  1409,  1434,  1436,  1437,  1440,  1441,
    1446,  1455,  1456,  1461,  1462,  1464,  1284,  1469,  1474,  1483,
    1283,  1287,  1290,  1291,  1293,  1295,  1297,  1298,  1299,  1301,
    1300,  1302,  1304,  1305,  1311,  1309,  1314,  1317,  1322,  1323,
    1341,  1325,  1328,  1329,  1342,  1324,  1335,  1339,  1344,  1345,
    1346,  1343,  1352,  1351,  1354,  1356,  1271,  1272,  1359,  1360,
    1363,  1361,  1367,  1368,  1370,  1372,  1375,  1379,  1377,  1378,
    1380,  1376,  1382,  1381,  1383,  1384,  1385,  1386,  1387,  1388,
    1431,  1432,  1373,  1389,  1390,  1391,  1392,  1395,  1402,  1403,
    1405,  1407,  1404,  1410,  1408,  1411,  1413,  1414,  1327,  1415,
    1416,  1422,  1423,  1424,  1426,  1429,  1430,  1427,  1428,  1433,
    1435,  1438,  1439,  1442,  1443,  1444,  1445,  1448,  1449,  1450,
    1452,  1454,  1463,  1465,  1468,  1470,  1471,  1473,  1477,  1478,
    1479,  1480,  1481,  1482,  1485,  1487,  1486,  1490,  1313,  1334,
    1488,  1289,  1303,  1318,  1319,  1331,  1340,  1348,  1420,  1321,
    1425,  1451,  1466,  1472,  1475,  1476,  1286,  1320,  1332,  1333,
    1349,  1350,  1357,  1358,  1364,  1369,  1393,  1394,  1397,  1396,
    1398,  1399,  1400,  1401,  1412,  1417,  1418,  1419,  1447,  1453,
    1467,  1484,  1489,  1491,  1330,  1458,  1460,  1459,  1362,  1306,
       0,     0,     0,  1275,  1270,  1276,   399,   368,     0,   465,
     464,     0,  1639,     0,   166,   346,   347,   118,   345,   119,
       0,   115,   116,     0,     0,  1098,   855,     0,  1020,     0,
    1017,  1012,     0,    55,  1172,  1279,  1273,  1278,  1277,    62,
    1028,     0,  1292,     0,     0,     0,     0,     0,     0,     0,
    1284,  1474,     0,     0,  1304,  1309,     0,   534,     0,     0,
     711,  1254,     0,  1245,     0,     0,  1251,   861,  1233,     0,
    1244,  1250,  1253,  1444,  1274,   769,     0,     0,     0,     0,
       0,     0,  1289,     0,     0,  1319,  1475,  1476,  1252,     0,
       0,     0,     0,   865,     0,     0,     0,   534,   534,     0,
       0,     0,  1320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1349,     0,     0,
    1350,  1357,     0,     0,     0,     0,     0,  1369,     0,     0,
       0,  1393,  1394,  1398,     0,     0,     0,  1399,  1400,  1401,
     534,  1412,  1417,     0,  1418,     0,  1419,     0,     0,     0,
       0,     0,  1447,  1467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1484,   534,   534,   534,     0,
    1491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1238,  1202,   536,   722,   537,   716,   653,  1242,   649,
     648,  1243,   646,  1232,  1260,   663,   494,  1210,     0,     0,
    1537,  1536,  1534,     0,     0,     0,     0,     0,  1644,   489,
     492,   504,  1153,   419,   419,   419,   417,   419,  1105,     0,
    1545,  1082,   432,     0,   434,   450,     0,   466,   430,     0,
    1495,  1492,  1203,     0,  1082,   494,     1,     3,     0,  1339,
    1163,  1161,  1267,  1269,   937,  1547,  1548,     0,   366,   369,
       0,   448,  1640,  1638,    63,     0,     0,   119,   117,     0,
       0,  1102,  1101,  1100,     0,  1098,  1006,     0,     0,  1011,
       0,     0,     0,  1017,    57,     0,  1030,  1031,  1029,   398,
       0,   942,   942,     0,   942,   942,   942,   942,   942,     0,
       0,   667,     0,     0,     0,     0,     0,   692,     0,     0,
     662,   494,     0,     0,     0,   861,     0,   859,     0,   657,
       0,     0,     0,     0,     0,  1246,  1234,     0,     0,   855,
     855,     0,     0,  1247,     0,  1249,     0,  1248,     0,   942,
     942,   942,   866,     0,   855,     0,     0,   689,     0,   690,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   855,   837,
       0,     0,     0,     0,   645,     0,     0,     0,   855,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   855,   855,
     855,     0,   738,     0,     0,     0,     0,     0,   855,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   855,
     855,   855,   855,   855,   855,     0,     0,     0,   780,   781,
     782,     0,     0,     0,     0,   655,   654,   656,  1665,     0,
     658,     0,  1507,   651,     0,     0,   538,     0,     0,   539,
     540,   541,   542,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   915,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,   926,   927,   928,
     930,   929,   931,   932,     0,     0,  1235,     0,  1660,     0,
    1211,     0,  1212,  1208,     0,     0,     0,   461,  1145,  1146,
       0,  1198,   439,  1195,  1194,     0,  1191,  1193,     0,  1645,
     517,   510,   509,   508,   512,   516,   514,   515,   511,   513,
     525,   505,   507,     0,     0,     0,  1145,     0,     0,   289,
    1151,     0,  1151,  1154,  1504,  1137,   342,   343,     0,   341,
    1505,  1136,     0,     0,  1124,  1506,  1151,  1148,  1149,     0,
     968,   968,     0,     0,  1151,     0,  1107,     0,     0,     0,
       0,   413,   423,   415,   410,     0,   416,   412,  1103,  1016,
    1083,   398,     0,     0,   472,   482,     0,     0,  1588,  1589,
    1585,  1590,  1591,  1579,     0,     0,  1577,     0,  1598,  1581,
    1573,  1595,     0,  1587,  1594,  1583,  1592,  1593,  1586,     0,
    1571,  1575,     0,     0,  1573,  1566,     0,  1498,     0,  1033,
       0,  1032,     0,    49,     0,  1169,   939,   938,     0,  1546,
       0,  1268,   303,   364,     0,     0,     0,    87,     0,    89,
     348,     0,  1085,  1088,  1092,  1096,  1099,   857,   856,     0,
    1018,  1010,   534,  1280,  1618,  1013,  1614,     0,     0,    56,
    1185,  1184,  1186,  1180,  1182,  1181,     0,  1187,  1183,  1537,
    1173,  1175,  1176,     0,     0,   943,     0,   843,   826,     0,
     843,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   535,     0,  1662,     0,     0,     0,     0,
       0,     0,   862,   863,     0,   674,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     869,     0,     0,     0,     0,   695,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   838,   835,     0,   935,   933,   936,
     934,     0,     0,     0,   723,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   610,
     744,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   819,     0,     0,     0,     0,     0,
       0,   777,     0,   779,     0,     0,     0,   982,   979,   978,
     981,   980,     0,   659,   855,     0,     0,     0,     0,     0,
       0,     0,   572,   563,     0,     0,   561,   573,   566,   567,
     555,   855,   548,   494,   951,     0,     0,     0,     0,   553,
     552,   559,   554,     0,   644,   574,   576,   569,   722,   568,
     722,   570,   571,   577,   575,  1288,   544,   564,     0,     0,
     647,  1261,  1666,  1659,  1207,  1209,     0,   477,     0,   937,
    1535,  1538,     0,     0,     0,  1196,     0,  1643,  1656,   493,
    1646,   523,   495,   522,     0,   506,  1157,     0,     0,   119,
    1141,  1117,  1142,  1135,     0,  1134,  1121,  1108,     0,  1130,
       0,  1149,  1129,  1155,  1156,  1151,     0,  1149,  1122,     0,
    1128,   969,  1127,   288,  1112,   152,  1133,     0,  1116,  1123,
       0,  1131,  1151,   426,   425,   424,   420,   421,   426,     0,
     893,     0,  1014,     0,     0,   474,   484,   436,   452,     0,
    1619,  1600,  1602,  1601,  1619,  1597,  1619,  1574,  1572,  1596,
    1619,     0,     0,   467,   468,     0,     0,  1572,     0,  1499,
    1500,  1502,  1501,  1493,     0,     0,    52,    53,     0,  1036,
    1025,  1035,   490,  1164,  1237,  1162,  1170,  1236,  1171,   941,
    1406,  1345,     0,   944,  1550,   304,   367,   303,   110,   113,
     112,     0,   303,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      65,    78,   120,   303,    83,     0,     0,     0,     0,  1090,
    1086,  1096,     0,     0,     0,   937,     0,  1008,  1282,     0,
       0,     0,   400,     0,    58,    60,  1179,     0,     0,  1188,
    1022,     0,   820,     0,   855,   825,     0,     0,   831,   830,
     832,   833,   834,     0,     0,     0,   683,   685,     0,     0,
     672,  1661,   855,   855,   730,     0,   860,     0,     0,   855,
     675,     0,     0,   673,   666,   682,   684,   696,   758,     0,
     757,   680,     0,   821,   822,   823,   867,     0,   686,   855,
       0,   691,     0,     0,   697,     0,   706,     0,   704,     0,
     855,     0,   700,     0,     0,     0,   855,     0,   713,     0,
     811,     0,   790,     0,   792,     0,   794,   855,     0,     0,
     718,     0,   724,     0,   813,     0,   795,     0,   855,     0,
     734,   735,   737,   797,     0,   799,     0,   801,     0,   796,
     803,   804,   739,   741,   740,   806,     0,     0,   808,     0,
     810,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   743,     0,     0,   747,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   760,   770,   771,   772,   773,   774,   775,
       0,   778,   783,     0,   785,   786,     0,     0,     0,   664,
    1508,  1509,  1510,   360,   650,  1262,   556,   562,     0,  1664,
       0,     0,   557,   855,   549,   951,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   579,   578,   565,   721,     0,     0,     0,
    1206,     0,     0,   452,     0,     0,  1199,   441,  1192,  1658,
    1657,  1647,   525,   970,   526,   528,   527,  1260,     0,  1159,
       0,  1140,     0,  1152,  1138,     0,  1151,  1109,  1150,  1151,
     972,   973,     0,  1149,     0,  1132,     0,   414,   423,   411,
     418,   893,     0,     0,   874,     0,     0,     0,   476,   471,
     475,   473,   338,   337,   340,     0,   483,   438,   437,   435,
     459,   458,   460,   457,   456,   451,   453,   454,  1599,     0,
    1580,  1578,  1582,  1584,  1610,  1613,  1267,     0,  1576,     0,
       0,     0,     0,     0,  1498,   289,     0,  1402,  1473,  1417,
       0,     0,  1519,  1496,     0,    54,  1034,     0,  1651,  1557,
    1555,  1556,  1560,  1561,  1563,  1562,  1564,  1554,  1551,  1558,
     945,   970,   111,  1060,  1060,   396,   396,   396,  1060,   396,
    1060,  1060,  1060,     0,     0,  1060,     0,     0,     0,     0,
       0,  1060,  1060,     0,  1060,  1060,  1060,  1060,   406,  1060,
    1060,  1060,  1060,  1060,   396,  1060,   394,   303,   145,   146,
     365,     0,   373,   395,   449,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,   121,     0,   351,   353,   352,   350,   349,     0,
       0,   944,     0,  1093,   893,  1097,  1096,  1094,   858,  1009,
    1281,     0,  1615,   402,   401,  1007,    61,     0,     0,  1174,
    1189,  1177,     0,     0,   844,   827,   824,     0,     0,     0,
       0,   266,   266,   266,   854,   852,   848,   853,   850,     0,
     291,     0,   290,     0,     0,     0,     0,   815,   864,     0,
       0,     0,     0,     0,     0,     0,     0,   870,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   841,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   602,   593,     0,     0,   591,   607,   596,   597,   585,
       0,   951,     0,     0,     0,   583,   582,   589,   584,     0,
     603,   605,   599,   722,   598,   722,   600,   601,   606,   604,
     594,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   660,     0,   652,   546,  1663,   950,     0,   558,     0,
     636,   627,     0,     0,   625,   641,   630,   631,   550,   855,
     613,   951,     0,     0,     0,     0,   618,   617,   623,   619,
       0,   637,   639,   633,   722,   632,   722,   634,   635,   640,
     638,   628,     0,  1263,  1204,   478,   479,   482,     0,  1541,
    1542,  1540,  1539,   462,  1200,  1201,   446,   447,   445,   440,
     442,   443,     0,   521,   893,   996,   518,   499,   497,   971,
     498,   529,     0,  1158,     0,  1118,     0,  1139,     0,  1111,
    1110,     0,     0,  1144,  1143,  1113,  1120,  1149,   428,   427,
     422,   897,     0,   893,     0,     0,   890,     0,     0,   893,
     900,   900,   893,   893,   937,   433,  1015,     0,   455,  1623,
       0,  1622,     0,     0,     0,   469,   470,   431,     0,     0,
       0,     0,   293,   292,   299,     0,  1058,  1059,     0,     0,
    1507,     0,   295,   297,   296,  1515,   294,     0,     0,   103,
    1040,     0,     0,     0,  1049,  1026,  1037,  1649,   491,  1650,
       0,     0,     0,  1549,  1061,     0,     0,   397,   358,   339,
     339,   377,     0,   181,   358,   332,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1060,     0,
     388,   371,   389,   372,  1060,     0,     0,   961,     0,     0,
       0,     0,   408,   409,   407,     0,     0,     0,     0,     0,
       0,     0,     0,   124,   303,   184,     0,   403,   181,   187,
    1264,    67,    68,    79,   987,   985,   983,   986,   984,    80,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    81,
      82,    66,     0,   184,    84,  1213,   122,   125,   169,   168,
     167,     0,    85,   893,   959,  1096,   944,     0,   937,  1616,
       0,    59,  1178,  1070,   736,     0,     0,   645,   678,   768,
       0,   845,   847,   305,   849,   851,   668,   671,   670,   728,
     729,   731,     0,     0,     0,   661,   676,     0,   759,   681,
     868,   669,     0,     0,   687,     0,     0,     0,   702,   707,
     705,   698,   703,   701,     0,   789,   715,   712,   714,   812,
     791,   793,   839,   842,     0,   717,     0,   725,   814,   726,
       0,   699,   732,     0,   798,   800,   802,   807,   805,   809,
     586,   592,   742,   587,   951,   590,     0,     0,   609,   608,
     595,     0,   746,   748,   645,   749,     0,   755,   753,     0,
       0,   766,     0,   764,     0,   765,     0,   767,     0,   784,
     787,   788,   361,   547,   551,   620,   626,     0,   621,   855,
     614,   951,   624,     0,     0,   643,   642,   629,  1213,     0,
     486,  1543,  1544,   444,  1648,   970,   944,     0,     0,  1004,
       0,     0,     0,   496,   501,   500,     0,   524,   533,   532,
    1257,  1261,  1160,   968,  1126,  1125,     0,   975,   974,  1151,
       0,     0,   504,   873,     0,     0,   892,   891,   893,   900,
     900,  1074,  1077,     0,   877,   901,     0,     0,   875,   876,
     902,   912,     0,   911,  1620,     0,  1612,  1611,  1624,     0,
    1567,  1497,  1521,     0,  1516,     0,     0,  1526,     0,     0,
    1517,     0,     0,  1520,  1532,  1533,  1530,  1531,  1529,  1512,
     504,     0,  1062,  1048,  1054,  1047,     0,     0,     0,  1046,
    1256,  1255,  1645,  1027,   968,  1655,  1653,     0,   946,   302,
     301,   151,   298,   150,   186,   359,   333,   334,     0,     0,
       0,   339,   180,   358,   179,   182,   348,     0,   379,   134,
     358,   386,   387,   400,   135,   132,   138,     0,     0,   139,
       0,   130,   131,     0,   137,   136,   158,   157,   141,   159,
     142,   143,   392,   153,   155,   154,   156,   140,   129,     0,
     128,   162,   161,   160,   147,   381,   133,   374,     0,   170,
     172,   173,  1266,     0,   405,   376,     0,   174,   175,     0,
       0,    93,     0,    91,     0,     0,  1215,  1214,    92,   106,
     303,   126,     0,     0,   944,   976,   960,  1089,  1095,  1617,
       0,  1023,   828,     0,     0,     0,   307,   308,   306,     0,
     846,     0,     0,   665,     0,   871,     0,     0,   693,   694,
       0,     0,     0,     0,     0,     0,     0,   588,     0,   580,
       0,     0,     0,     0,     0,   763,   761,   762,     0,   611,
       0,   622,     0,   615,     0,   480,     0,   481,   503,   952,
    1001,  1003,     0,   997,   999,     0,   519,   531,   530,     0,
    1119,     0,  1114,   429,   937,   525,     0,   889,     0,     0,
       0,   944,     0,   893,   893,     0,   879,     0,     0,     0,
     894,   485,     0,  1621,     0,  1629,     0,   300,     0,     0,
    1525,  1514,     0,     0,     0,     0,  1511,   525,  1041,  1056,
       0,  1064,     0,     0,  1038,  1051,     0,     0,  1050,  1654,
    1069,  1068,     0,  1067,   183,   358,   335,   336,   348,     0,
       0,     0,     0,   385,   384,   299,   148,   149,  1259,   962,
     965,     0,     0,   378,   184,   404,   312,   200,   266,   193,
     194,   255,   305,     0,   238,   243,   225,   228,   251,   266,
     237,   208,   205,   255,   253,   223,   255,   229,   247,   213,
     305,   212,   250,   305,   255,   252,   249,   266,   266,   206,
     209,   248,   305,     0,   241,   230,   233,   234,   232,   236,
     231,   235,   266,   270,   211,   305,   198,     0,     0,   266,
     268,  1265,     0,    96,   121,   107,     0,   127,    90,     0,
    1091,     0,  1087,  1071,     0,   679,   267,   309,   817,   816,
     677,     0,   688,   708,     0,   840,   836,   942,   719,   727,
     733,   581,   745,   750,   754,   752,   756,   855,   612,   616,
       0,   487,     0,   947,  1216,  1000,     0,     0,  1258,     0,
     896,   898,     0,   893,   893,  1076,   959,  1078,   882,   886,
     878,     0,   906,   904,   905,   903,   910,  1628,  1626,     0,
    1627,     0,     0,  1625,  1603,     0,  1570,  1568,  1523,  1522,
    1524,  1518,     0,     0,     0,   104,     0,     0,     0,  1065,
    1053,  1651,  1039,  1045,  1559,     0,     0,     0,     0,   391,
       0,   403,   393,     0,   966,   967,   964,   144,   270,   171,
     318,     0,   192,     0,   259,   257,   237,   214,     0,   305,
     227,   239,   242,   244,   246,     0,   210,   259,   254,     0,
     259,   219,   218,   259,   305,   207,   216,     0,   259,     0,
     278,   284,     0,     0,     0,   274,     0,   311,     0,   281,
     188,   271,   273,     0,   240,     0,   196,     0,     0,     0,
     259,     0,   269,   259,    94,  1652,  1213,     0,    99,     0,
     965,   356,   977,     0,     0,   872,     0,     0,     0,     0,
       0,   948,   959,     0,  1224,   998,   520,     0,   970,   893,
     884,   888,   976,     0,     0,     0,     0,     0,     0,  1607,
    1608,  1609,  1604,     0,     0,     0,     0,     0,  1630,  1632,
    1528,  1527,  1513,   970,  1055,  1057,  1063,  1052,  1066,     0,
       0,     0,     0,     0,     0,     0,     0,   390,  1239,   380,
     965,   382,     0,     0,     0,   313,   314,   319,   321,     0,
       0,   263,   264,   265,   222,   260,   262,   215,   245,     0,
     220,     0,   191,   221,   217,     0,   204,   283,   285,   534,
     276,   287,   275,     0,     0,   282,   272,   280,     0,     0,
       0,     0,   189,     0,   190,    97,    95,     0,  1645,    86,
       0,   355,     0,     0,   720,   709,     0,   829,   776,  1216,
     956,   965,     0,   970,     0,     0,     0,     0,  1217,  1219,
       0,  1002,     0,  1652,   518,   163,     0,  1075,   881,     0,
     885,     0,     0,   913,   908,  1605,  1633,  1636,  1634,  1635,
    1631,   105,     0,     0,   176,  1241,  1240,   963,   403,   324,
     325,   326,     0,     0,     0,   317,   320,   199,   256,     0,
     261,     0,   362,     0,   203,   286,   534,   279,   305,   197,
     305,   202,   101,   100,   965,     0,     0,     0,  1224,     0,
       0,   953,   955,   949,   988,     0,     0,     0,     0,  1218,
       0,     0,  1225,  1227,     0,   899,   164,     0,     0,     0,
     880,     0,     0,   909,     0,   177,   383,   328,     0,   327,
       0,   322,   323,   318,     0,     0,   305,     0,   305,   277,
     195,   201,  1652,   354,   357,  1072,  1080,     0,   710,  1230,
     957,   958,   965,     0,   502,  1222,  1223,     0,  1220,     0,
       0,  1226,     0,     0,     0,     0,   914,   907,   178,   330,
     331,   329,   315,   316,   258,   226,   363,   224,   102,     0,
       0,     0,  1042,   954,   989,  1221,  1229,  1228,     0,   895,
     883,   887,  1079,  1081,     0,     0,  1205,     0,     0,  1231,
    1044,     0,   525,     0,  1043,     0,     0,   992,   994,     0,
     995,   990,   525,  1115,   993
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    49,    50,    51,    52,    53,  1256,    54,   584,   949,
    1324,  1325,    55,    56,   925,  1299,  1300,  1301,    57,  1722,
    2423,  1303,  1305,  2104,  2413,  2785,  2418,  2898,  3022,  2014,
    2320,  2833,  2626,  1276,  1277,  1278,   330,   331,   332,   566,
    2105,  1696,  2106,  2107,  1698,  1699,  1204,  2387,  2378,  2394,
    2923,   333,  2111,  2398,  2399,  2400,  2401,  2354,  2355,  2032,
    2033,  2078,  2409,  2613,  2749,  2745,  2614,  2615,  2616,  2617,
    2618,  2619,  2620,  2734,  2735,  2864,  2865,  2866,  2131,  2783,
    2770,  2771,  2772,  2880,  2439,  1993,  1994,  2004,  2005,  2340,
    2304,  2341,  1281,  2440,  2773,  2408,  2730,  2855,  2954,  2856,
    2857,  2858,  3011,  2034,  2353,  1604,  1605,   855,   334,  1308,
    1727,  2789,  2790,  2344,  1873,  2961,    58,  1282,   922,   558,
    1700,  1701,  1702,  2552,  2562,  2948,  2036,   318,  1745,  2405,
    2065,    59,  1218,  1213,    60,   866,   861,   862,  1216,  1217,
    1587,  1959,    61,   903,    62,   872,    63,   876,  1609,    64,
    1164,  1929,  1930,  1931,    65,   924,    66,   877,  1615,  1616,
    1617,    67,  1162,   321,    68,   902,  1243,  1244,    69,   873,
     874,  1599,    70,  1549,  1915,  1916,  1225,  1226,  1606,  2477,
      71,    72,  1648,   519,   809,   520,   521,  1563,  1936,  2924,
     820,   821,   822,  2253,  1172,  1173,  1174,  1941,  1564,  2257,
     607,  2541,   765,  1148,   493,  1068,  1133,   494,   495,   496,
    2144,  1083,   497,  1334,  2125,  1407,  1035,  2452,  2172,   966,
    1333,  1769,  1084,   577,   938,   616,   617,   618,   992,   643,
    1785,  1376,  1377,  1787,  1593,  2681,  2814,  2816,  1973,  1594,
    1595,  2271,  2272,  2808,  2286,  2510,  2683,  2818,  3002,  2292,
    2932,   784,  1041,   918,   919,   967,  1661,  2022,  2802,  2912,
    1512,  2663,  2910,  2981,  2425,  2426,  2373,  2559,  2726,  1200,
    2925,  1939,  1580,  2632,  1102,  2089,  3034,  3077,  3086,  3087,
    3088,  2249,  2250,  2483,  2484,  1940,  2664,    73,   336,    74,
    1739,   582,  1221,  1222,   581,   342,    75,   350,  1752,    76,
     544,  1647,  2333,   589,   910,  1260,  1261,  2015,  2321,  3076,
    2328,  2016,  2332,  2711,  2323,  2528,  2529,  2024,  2025,  2324,
    2531,  2708,  2542,  2543,  2431,  2793,    77,   531,  2501,  2281,
    2282,  3025,  3026,   871,    78,   335,   932,  1731,  1313,  1730,
     933,   934,  1315,   574,   575,    79,   529,    80,   522,   856,
    1582,  2263,  1955,   800,   858,  1195,  1185,   859,  1196,  1569,
    1945,    81,   915,   914,    82,   550,  1265,    83,   585,   960,
     961,  1329,  1751,    84,   516,   806,   807,    85,   514,   801,
    1556,    86,    87,    88,  2238,  1156,   793,  2419,  2804,  2918,
    2919,  2921,  2992,  2993,  3062,   498,  2962,   499,  2847,   500,
     501,  2329,  2330,  2560,   502,  2079,   869,   312,   313,   347,
     503,   504,   943,   944,   315,    89,   907,   541,  1253,  1254,
     860,  1109,  1643,  1644,  2521,  2310,  2319,    90,   796,   962,
    1160,  1161,  1921,    91,    92,   920,  1273,  1274,  1658,  2021,
    1659,    93,   539,   905,    94,   536,   899,  1238,   900,   901,
    1234,  1230,  1236,  1240,  2823,  2693,  2694,  1627,   945,   946,
    1620,  1980,  1981,  2515,  2696,  2828,  2829,    95,   322,   563,
      96,    97,    98,  1169,  2017,  1932,  2018,  2019,  2020,  2336,
    1561,   505,   789,   610,   986,  1122,  1510,   506,  1153
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -2624
static const int yypact[] =
{
    2657, -2624, 13925,   109,    37, -2624,   221, -2624,   643, -2624,
   -2624,   143, 37638,    37, 34030, -2624,  8724,    67,   363,    37,
   -2624,    37, -2624,   255, 37638, -2624, -2624,   729,   131,   233,
      42,   363, -2624,   363,   164,   363,   363, -2624, -2624, -2624,
     363,   363, -2624,   452, 37638, 37638, -2624, -2624,   558,   663,
     602, -2624,   414, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, 14376, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   37638,   534,   198, -2624,   -70, -2624, -2624, 37638,   487, -2624,
   -2624,   363,   308,   501, -2624, -2624, -2624, -2624, -2624,   383,
     522, -2624,   550,   560,   694,   111, -2624, 37638, -2624,   792,
     468, -2624,   363, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
     679,   397,   407,   413,   416,   473,   481,   506,   514,   516,
     520,   524,  8724,   526,   528,   530,   532,   569,   573,   583,
   -2624, -2624,   609, -2624,   615,   619, -2624,   624, -2624,  8724,
   -2624, -2624, -2624,   629, -2624, -2624,   631,   645,   673,   677,
      70,   684,   689,   691,   696,   -54,   -34,   -14, -2624,   698,
     714,   717,   734,  8724,   737,   745,   747,   569,   749,   757,
     758,   762,   764,   768,   772,   773,   776,   780,   781,   782,
     786,   789,   798,   799,   800,   803,   804,   805,   806,   808,
     810,   811,   813,  8724,   815,   818,   821,   825,   829,   830,
     832,   834,   836,   838,   839,   842,   844,   845,   846,   847,
     848,   850,   852,   853,   855,   856,   857,   861,   863,   864,
     866,   867,   868,   869,   870,   872,   873,   874,   877,   880,
     884,   886,   887,   890,   895,   898,   569,   569,   569,   901,
     905,   906,   908,  8724,  8724,  8724,  4305,  8724, 37638, 15729,
   37638, -2624, 30917, -2624,  1599,   767, -2624, -2624,   628, -2624,
   -2624, -2624, -2624, -2624,   480, -2624, -2624,   656,   722, 13925,
   -2624, -2624, -2624,   363,   270,   363,   264,   920, -2624, -2624,
   -2624,   340, 38714, -2624, -2624, -2624,   608, -2624, -2624, 13925,
   -2624,   824, -2624, 13925, -2624, -2624,  1219, -2624, -2624,  2992,
   -2624, -2624, -2624,   879,   129, -2624, -2624, -2624, 37638,    19,
   -2624, -2624,   549, -2624,   744, -2624, -2624, 37638, -2624, -2624,
   13925, -2624, -2624, -2624, -2624,   935, 37638,   383, -2624,   792,
   37638, -2624, -2624, -2624, 35383,   111, -2624,  8724,   945, -2624,
     938, 37638, 32677,   468,   910,  1127, -2624, -2624, -2624,  1057,
    8724,  1309,   180,  8724,  1309,  1309,  1309,  1309,  1309,  8724,
    8724,   170,  8724,  8724,  8724,  8724,   911, -2624, 14827,  1332,
   -2624, -2624,  8724,  8724,  8724, -2624,  1047, -2624, 14827,   170,
    8724,   917,  8724,  8724,  8724, -2624, -2624, 14827,  8724, -2624,
   -2624,  1128,  8724,   628,  8724,   628,  8724,   628,  8724,  1309,
    1309,  1309, 30917,   949, -2624,  8724,  8724, -2624,  4796, -2624,
     922,  8724,  8724,  8724,  8724,  8724,  8724,  8724,  8724,  8724,
    8724,  1599,  8724,  8724,  8724,  8724,  8724,  8724, -2624,  1284,
     544,   539,  8724,  8724, 30917,  5287,  8724,  8724, -2624,  8724,
    8724,  8724,  8724,  8724,  8724,  8724,  8724,  8724, -2624, -2624,
   -2624,  4796, -2624,  8724,  8724,  8724,  8724,  8724, -2624,  8724,
    5778,  8724,  8724,  8724,  8724,  8724,  8724,  8724,  3323,   925,
     925,   925,   925,   925,   925,   544,  6269,   926, -2624, -2624,
   -2624,  8724,  8724,  8724,   556,   170,   170,   170, -2624, 17153,
     170,  8724,    90,  1393,   924,  8724, -2624,  8724,  1241, -2624,
   -2624, -2624, -2624, -2624,   716,  8724,  8724,  8724,  8724,   933,
    8724,   366,  8724,  8724,  8724,  8724,  8724,  8724,  8724,  8724,
    8724,  8724,  8724,  8724,  8724,  6760, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624,   942, 34030, -2624, 37638, -2624,   936,
   -2624,  1366, -2624,  1245,  1323,  1283, 13925, -2624, -2624, -2624,
    1253, -2624, -2624, -2624, -2624,  1333,   954, -2624, 37638,  1147,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
     964,   340, -2624,  1401,   961,   296,  1178,   421,  1268, -2624,
    1273,  1166,  1273, -2624, -2624,  1063, -2624, -2624,  1183, -2624,
   -2624, -2624,  1398,  1184, -2624, -2624,  1273, -2624,   440,    16,
    1130,  1130,  1197, 34030,  1273,   440, -2624,  1282,  1081,    81,
    1162, -2624,   536, -2624, -2624,  1432, -2624, -2624, -2624, -2624,
   -2624,  1057, 13925,   -44, -2624, -2624, 13925, 13925, -2624,  1192,
   -2624, -2624, -2624, -2624,  1196,   213, -2624,  1355, -2624, -2624,
    1238, -2624,  1250, -2624, -2624, -2624, -2624, -2624, -2624,  1255,
     990, -2624, 13925, 13925,  1238, -2624,  1258,   565,  -125, -2624,
   12061, -2624,   993, -2624,    20, 34481, -2624, -2624, 37638, -2624,
   38089, -2624,   567, -2624, 13925,  1697,  1346, -2624, 13925, -2624,
     445, 13925, -2624,   994, -2624,   991, -2624, 30917,   995, 13925,
   -2624, -2624,   569,   997,  1084,  1002, -2624, 13925,  1001, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624,  1395, -2624, -2624,  1198,
    1014, -2624, -2624, 12061, 19120, -2624,  1019, -2624, -2624,  1020,
    1030, 19390,  1024,  1025,  1026,  1027,  1028, 19427, 19488, 30424,
   22524, 22537, 17166, -2624,  1029, -2624,   936, 19506, 19614, 17200,
    1031,  1033,  1032, -2624, 19838, -2624, 22616, 19872, 19933,  1035,
   22796,  1037,  1038, 22858, 17218, 22923, 17252,  1040,  1041,  1042,
   -2624,  1046, 20059, 20112, 22940, -2624, 20155, 20253, 22970, 20429,
   17525, 17569, 20442, 20472, 17604, 20749,  1390, 20792, 20805, 17651,
   17704, 17749, 17928,  1048, -2624, -2624,   -81, -2624, -2624, -2624,
   -2624,  1050, 23209, 20828, -2624, 23291, 20845, 17973,  1049, 21162,
   21178, 21202, 23304, 23337, 23478, 18147, 18198, 18250,  1052,  1053,
    1056, 23576, 23612, 23658, 18291, 21243, 18332,  1059, 30963,   767,
   -2624, 23703, 21485, 21528, 18574, 23720, 21566, 18688, 21613,  7251,
    7742,  8233, 22344,  1061, -2624,  1066,  1067,  1069,  1071,  1072,
       2, -2624, 23979, -2624, 18701, 24091, 18881, -2624, -2624, -2624,
   -2624, -2624,  1073, -2624, -2624, 16979,  1051,  1070,  1074, 34030,
    8724, 37638,    56,   175,  8724,  1342, -2624,    56,   235,   235,
    2619,  1332, -2624, -2624,   509,  1078,  8724,  8724,  8724, 24475,
   24475,   175, 24475, 31009,   767,   152,   222,   239,  1599,   239,
    1599,    56,    56,    56,   170,  1080, -2624,   175,   583,  8724,
   -2624,  1075, -2624, -2624, -2624, -2624,  1408, -2624,  1520,   197,
    1089, -2624, 13925,   -18, 13925, -2624,   264, -2624,   205, -2624,
   -2624, -2624,  1091, -2624,  8724, -2624,  1413,  1101, 13925,   383,
   -2624, -2624, -2624, -2624,  1304, -2624, -2624, -2624, 32677, -2624,
    1199,   440, -2624, -2624, -2624,  1273, 37638,   440, -2624,   556,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, 13925, -2624, -2624,
     440, -2624,  1273,  1503, -2624, -2624,  1098, -2624,  1503,  1516,
     433,  -122, -2624, 35834, 13925, -2624,    17,   -12,   -20,  1315,
    1105, -2624, -2624, -2624,  1105, -2624,  1105, -2624, -2624, -2624,
    1105, 12512,  2810,  1104, -2624,  1316,   -24,  1109, 12512, -2624,
   -2624, -2624, -2624,  1110, 16180, 34030, -2624, -2624, 13925, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
     125,   140,  1423,  1294, -2624, -2624, -2624,   587, -2624, -2624,
   -2624,   110,  2181,   -20,  1590,  1591,  1592,  1593,  1594,  1595,
    1596,  1598,  1600,  1601,  1602,  1603,  1605,  1606,  1609,  1121,
   -2624, -2624, -2624,   567, -2624,  1330,   100,   100,  1400,   994,
   -2624,    82, 37638,  1478, 28529,    69,  8724, -2624, -2624, 34030,
    1523, 32677,    10, 34030,  1136, -2624, -2624,  1381,  1127, 13925,
   -2624,  8724, -2624,  8724, -2624, -2624,  1139,  8724, -2624, -2624,
   -2624, -2624, -2624,  9215,  8724,   390, -2624, -2624, 33128,   390,
   -2624, -2624, -2624, -2624, -2624,  8724, -2624,  8724, 14827, -2624,
   -2624,  8724,  8724, -2624, -2624, -2624, -2624, -2624, -2624,  8724,
   -2624, -2624,  8724, -2624, -2624, -2624,  1251,  8724, -2624, -2624,
    8724, -2624,  1224,  1224, -2624,   628, -2624,  8724, -2624,  8724,
   -2624,   628, -2624,  8724,  8724,  8724, -2624,  1412, -2624,  8724,
   -2624,  8724, -2624,  8724, -2624,  8724, -2624, -2624,  1414,  8724,
   -2624,  8724, -2624,  8724, -2624,  8724, -2624,  8724, -2624,  8724,
   -2624, -2624, -2624, -2624,  8724, -2624,  8724, -2624,  8724, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624,  8724,  8724, -2624,  8724,
   -2624,  8724,  8724,  1455,   720,  8724,  8724,  8724,  8724,  8724,
    8724,   331,  8724,  8724,  8724,  8724,  8724,  8724,  8724,  8724,
    8724,  8724,  8724,  8724,  8724,  6760, -2624,  8724,  8724, -2624,
    8724, -2624,  9706,  8724,  8724,  8724,  8724, 30466,  8724, 30509,
    8724, 30531,  8724, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
    1415, -2624, -2624,  8724, -2624, -2624,  8724,  8724,  1149, -2624,
   -2624, -2624, -2624,  1156, 30917, -2624,   175, -2624,  1153, -2624,
     936,  8724, -2624,  1332, -2624,   509,   175, 31097,  8724,  8724,
    1465,   730,  8724,  8724,  8724,  8724,  1164,  8724,   392,  8724,
    8724,  8724,  8724,  8724,  8724,  8724,  8724,  8724,  8724,  8724,
    8724,  8724,  6760, -2624, -2624, -2624,   239, 37638,  1304, 13925,
   -2624,   360, 13925,   -20,  1304,  8724, -2624,    -9, -2624, -2624,
   -2624, -2624, -2624,   187, -2624, 30917, -2624,  1158,  1304,  1512,
    1174, -2624, 37638, -2624, -2624,  1597,  1273, -2624, -2624,  1273,
   -2624,   -71,    36,   440, 13925, -2624,   654, -2624,   536, -2624,
   -2624,    27, 37638,   237, -2624, 13925,  1304, 13925, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624,  1185, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624,   417, -2624, 37638,
   -2624, -2624, -2624, -2624,  1176, -2624,  1177,  1392, -2624, 13925,
   13925,  1304,  1525,  1530,   565,    68,  1182, 31775,  1508,    93,
   16631, 32226,  1186, -2624,   605, -2624, -2624,    34,   370, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624,  1130, -2624,   605,   605,  1380,  1566,  1566,   605,   660,
     605,   605,   605,  1403,  1547,   605,  1511,  1411,  1514,  1416,
    1555,   605,   605,  1589,   605,   605,   605,   605,   201,   605,
     605,   605,   605,   605,  1566,   605, -2624,   114, -2624, -2624,
    1202, 12983, -2624, -2624, -2624,  1304,  1304,  1304,   929,  1304,
     556,   556,   556,  1304,  1304,  1304,  1304,  1304,  1304,   556,
    1697, -2624, 28098,   448, -2624, -2624, -2624, -2624, -2624, 13925,
    1394,  1294, 28529, -2624,   433, -2624,  1201, -2624, 30917, -2624,
   -2624,   -76, -2624, -2624, -2624, -2624, -2624,  1001,  1643, -2624,
    1212, -2624,    34, 24105, 30917, -2624, -2624, 21805,  8724, 24200,
   24218,  1215,  1215,  1215, -2624, -2624,  1374, -2624,  1375,  1217,
   -2624,  1218, -2624,  1220,  1221,  1223, 24330, 30565, -2624,  1226,
   24588, 21835, 24602, 24615,  8724,  1246, 30610,  1247,  1228, 21852,
    1599,  1599,  1229, 24649, 24700,  1230,  1232, 24714, 21890, 24972,
    1233,  1234, 25033, 25098, 25150, 25211,  1495,  1240, 25457, 21931,
   25479, 25519, 18946,  1239, 18976, 25536, 25583, 25855, 25890, 25906,
   25971,    56,   175,  8724,  1513, -2624,    56,   235,   235,  2619,
   26080,   509,  8724,  8724,  8724, 24475, 24475,   175, 24475, 31139,
     152,   222,   239,  1599,   239,  1599,    56,    56,    56,   170,
     175,   583, 22170, 26214, 26256,  8724, 26397, 22429, 19008, 22207,
   26495,  8724, 26531,  8724, 26577,  8724, 26622,  1244, 26639, 26898,
   27010, -2624, 37638, -2624, -2624, -2624,   767,  1242, -2624,  8724,
      56,   175,  8724,  1515, -2624,    56,   235,   235,  2619,  1332,
   -2624,   509,  1248,  8724,  8724,  8724, 24475, 24475,   175, 24475,
   31203,   152,   222,   239,  1599,   239,  1599,    56,    56,    56,
     170,   175,   583, -2624, -2624,  1252, -2624, -2624,  1428,  1560,
   -2624, -2624, -2624, -2624, -2624, 30917, -2624, -2624, -2624, -2624,
   -2624,   202,    20, -2624,    39,   725,    76,  1575, -2624, -2624,
    1604, 34932, 29000, -2624,   929, -2624,   665, -2624,  1438, -2624,
   -2624,   556,   556, -2624, -2624, -2624, -2624,   440, -2624,  1257,
   -2624, -2624,    86,   438,  1577,  1578, -2624,   115, 14827,   433,
    1522,  1522,   433,   433,  2513, -2624, -2624, 36285, -2624, -2624,
     533, -2624,  1269, 29471, 32677, -2624, -2624, -2624,  1535, 32677,
   16180, 37638, -2624, -2624,  1645,  1585, -2624, -2624, 32677,   182,
      90,   605, -2624, -2624, -2624, -2624, -2624, 37638,  3814, -2624,
   -2624,  1276,  1276, 11590, -2624, -2624, -2624, -2624, -2624, -2624,
      46,  1277,  8724, -2624, -2624, 33579, 31775, -2624, 15278,   428,
     428, -2624,   458,   518, 15278, -2624, -2624, 15278, 15278,  1304,
    1607,  1608, 15278, 15278,   929,   556,   556,    85,   605,   556,
   -2624, -2624, -2624, -2624,   605,   929,   929, -2624,   120,   973,
     556,   556, -2624, -2624, -2624, 13925,    89, 34030,  1278, 34030,
     173, 15278,  1304, -2624, 38450, 11139, 37638,   570,   116, -2624,
    1272, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, 13925, 10688, -2624,    79, -2624,  2825, -2624, -2624,
   -2624,  1476, -2624,   433,  1495,    83,   693,  1286,    69, -2624,
    1490, -2624, -2624,  1550, -2624,  1290,  8724, 30290, -2624, -2624,
    1554, -2624, -2624,   268, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624,    -3,  1543,  1296, -2624, -2624,  8724, -2624, -2624,
   30917, -2624,  8724,  8724, -2624,  8724,  1298,  1301, -2624, -2624,
   -2624, -2624, -2624, -2624,  8724, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624,  1528, -2624,  1572, -2624,  8724, -2624, -2624, -2624,
    8724, -2624, -2624,  8724, -2624, -2624, -2624, -2624, -2624, -2624,
     175, -2624, -2624, -2624,   509,   175, 31284,  8724, -2624, -2624,
   -2624,  8724, -2624, -2624, 30290, -2624,  8724, -2624, -2624,  8724,
    8724, -2624, 27024, -2624, 27119, -2624, 27137, -2624,  1574, -2624,
   -2624, -2624, -2624, -2624,  2619,   175, -2624,  1308, -2624,  1332,
   -2624,   509,   175, 31316,  8724, -2624, -2624, -2624,    -7, 13925,
    1638, -2624, -2624, -2624, -2624,  1130,   693,  1304,  1304, -2624,
    1310,  1634,  1720, -2624, -2624, -2624, 36736, -2624, -2624, -2624,
   -2624,  1311, -2624,  1130, -2624, -2624,  1619, -2624, -2624,  1273,
     654,  1321,   340, -2624,  1742,  1404, -2624, -2624,   433,  1522,
    1522,  1324, -2624,   605,   989, -2624,  1644,  1646,   989,   123,
     395, -2624,   537, -2624, -2624, 37638, -2624, -2624,   -96,  1670,
    1002, -2624, -2624, 33579, -2624,   527,   605, -2624,  1335,  1338,
   -2624, 37187,  8724, -2624, -2624,  8724,   573, -2624, 30917, -2624,
     340, 14827, -2624,  1328, -2624,  1328,    38,  1344,   543, -2624,
   -2624, -2624,  1147, -2624,  1130, -2624, -2624, 10197, 30917, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,  8724,  1648,
    1654,   428, -2624, 15278, -2624, -2624,   445,   133, -2624, -2624,
   15278, -2624, -2624,   107, -2624, -2624, -2624, 31775,  1304, -2624,
    1304, -2624, -2624,  8724, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, 13925,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,   562, -2624,
   -2624, -2624, -2624, 37638, -2624, -2624, 13925, -2624, -2624, 38441,
   37638, -2624, 13925, -2624,  1345,   564, -2624, -2624, -2624,  1749,
    2778, -2624,  1304,  1361,   693,  1521, -2624, -2624, -2624, -2624,
    1724, -2624, -2624, 22249,  1371,  1373, -2624, -2624, -2624, 33128,
   -2624,  1445,  1817, -2624, 27249, 30917, 30789, 27507, -2624, -2624,
   19055,   386,  1378,  1379, 27521, 27534, 27568, -2624,  8724,  2619,
   27619,  1382, 27633, 27891, 27952, -2624, -2624, -2624,  1383, -2624,
    1385, -2624,  8724,  2619,  1700, -2624,  1694, -2624, -2624,  1722,
   -2624, -2624, 34030,  1387, -2624,  1435, -2624, -2624, -2624, 29942,
   -2624,  1869, -2624, -2624,  2007,   964,  1651,   989,  1729,  1733,
   14827,  1294, 10197,   433,   433,  8724, -2624,   428,   428,   428,
   -2624, -2624, 37638, -2624,    75,  1618, 32677, -2624,   106,  1663,
   -2624, -2624,   182,  1629,  1630,   605, 30917,   964,  1426, -2624,
     605, 10197,  1276,  1857, -2624, -2624,    38, 14827, -2624, -2624,
     573, 30917,   592, -2624, 30917, 15278, -2624, -2624,   445,  1440,
    1808,  1809, 15278, -2624, -2624,  1645, -2624, -2624, 30917,  1441,
     492,   603, 38441, -2624, 11139, -2624, -2624,  1444,  1215, -2624,
   -2624,  1446,   578,   -60,   740, -2624, -2624, -2624, -2624,  1215,
   -2624, -2624, -2624,  1446,  1610, -2624,  1446, -2624, -2624, -2624,
     268, -2624, -2624,   268,  1446, -2624, -2624,  1215,  1215, -2624,
   -2624, -2624,   268,  1447, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624,  1215,  1475, -2624,    65,  1448,  1449,  1451,  1215,
    1456, -2624,  1454, -2624,  2884, -2624,    33, -2624, -2624, 37638,
   -2624,   929, -2624, -2624,  8724, -2624, -2624, -2624, -2624, -2624,
   -2624,  8724, -2624, -2624,  8724, -2624, -2624,  1309, -2624, -2624,
   -2624,  2619, -2624, -2624, -2624, -2624, -2624, -2624, -2624,  2619,
    1656, -2624,  1843,  1792,  1834, -2624,  1310,  1531, -2624,  1304,
   -2624,  1091,  1778,   433,   433, -2624,  1495, -2624,   968,   983,
   30917,  1463, -2624, -2624, -2624, -2624, -2624, -2624, -2624,  1671,
   -2624,  1672,  1674, -2624,   172,   575, -2624,  1002, -2624, -2624,
   -2624, -2624,  1467,  1468,  3814,  1091, 14827, 10197,  1469,  1470,
   -2624,   370, -2624, -2624, -2624, 10197,  1471,  1473, 37638, -2624,
    1811,   570, -2624,  8724, -2624, -2624, -2624, -2624,  1475, -2624,
       5,  1741, -2624,  1743,   441, -2624,  1197, -2624,  1612,   268,
   -2624,  1613, -2624, -2624, -2624,  1477, -2624,   441, -2624,  1482,
     441, -2624, -2624,   441,   268, -2624, -2624,  1745,   441,  1304,
   -2624, -2624, 34030,  1095,  1751, -2624,  1878, -2624,  1858,  1805,
   -2624,  1475, -2624,  1807, -2624,  1760, -2624,  1761,  1764,  1765,
     441,  1766, -2624,   441, -2624,   144,    79,  1857, -2624,   611,
     492,  1500, -2624,  1819, 28017, 30917, 19073,  1504,  1505, 13925,
    8724, -2624,  1495,   462,  1816, -2624, -2624,  1908,   119,   438,
     989,   989,  1521,  8724,  1701,  8724,  1702, 37638,  1518, -2624,
   -2624, -2624, -2624,   -97,  1777,   556,   556,   556,   575, -2624,
   -2624, -2624, -2624,   119, -2624, -2624, -2624, -2624, -2624, 37638,
   37638,   637,   544,   544,   544,   979,   979, -2624, -2624, -2624,
     492, -2624,   500,   226, 37638, -2624, -2624,   385, -2624,  1519,
     662, -2624, -2624, -2624, -2624,   441, -2624, -2624, -2624,   386,
   -2624,   386, -2624, -2624, -2624,  1529, -2624, -2624, -2624,   569,
   -2624, -2624, -2624,  1582,  1707, -2624, -2624, -2624,  1533,  1541,
    1544,  1546, -2624,  1534, -2624, -2624, -2624,  1556,  1147, -2624,
   37638, -2624,  1815,  1954, -2624, -2624,  8724, -2624, -2624,  1834,
      57,   492,  8724,  1130,  1942,  1944,  1914,  1947,   462, -2624,
     618, -2624,  1860,  -115,    76, -2624,  1828, -2624, 30917,  1564,
   30917,  1565,   669, -2624, 36285, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624,   680,   692, -2624, -2624, -2624, -2624,   570, -2624,
   -2624, -2624,   513,   513,   700, -2624, -2624, -2624, -2624,  1833,
   -2624,   703, -2624,   708, -2624, -2624,   569, -2624,   268, -2624,
     268, -2624, -2624, -2624,   492,  1567, 14827, 28069,  1816,   216,
    8724, -2624, -2624, 30917,  1621,   386,   386,  1953,   386, -2624,
    1957,  1958,   618, -2624,  2049, -2624, -2624,  8724, 37638, 37638,
   -2624, 37638,  1579,  1576,  1822, -2624, -2624, -2624,  1988, -2624,
     402, -2624, -2624,   385, 37638,  1588,   268,   386,   268, -2624,
   -2624, -2624,   144, -2624, -2624,  1611, -2624,   605, -2624,  1919,
   -2624, -2624,   492, 37638, -2624, -2624, -2624,   386, -2624,   386,
     386, -2624,   929, 17115,   719,   721, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, 14827,
   10197,  1861,  1616, -2624, -2624, -2624, -2624, -2624,  1994, -2624,
   -2624, -2624, -2624, -2624,  1905, 13454, -2624,  1617,  1892, -2624,
   -2624,   727,  1622,  2075, -2624,  8724,  1623,  1625, -2624,   556,
   30917, -2624, -2624, -2624, -2624
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624,   339, -2624, -2624, -2624, -2624,   367, -1536, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624,  -697, -2624, -2624, -1967,
   -2624, -2624, -2624,   788, -2624,   816, -2624,  1762, -2624,  -506,
    -532,   399, -1977, -1241,  -804,  -803, -1099, -2624, -2624, -2624,
    -735, -2624, -2624,     1,  -458,   410,   450,    40,    84, -2624,
   -2624,  -435, -2624,  -442, -2624, -2624, -2624,  -453, -2624,  -451,
   -2624, -2624, -2624, -1697,  -498, -1477, -2624,  -742, -1707, -2624,
    -604, -2624,  -645, -2624,  -515, -1331, -1914, -2624, -2624,  -635,
    -426,  -170, -2624, -1633, -2624,  -869, -2624, -2624, -2624,  -879,
   -2624,  -721,  -816, -2624, -2624, -1565, -1888, -2624, -2624, -2159,
     831, -2171,  -760, -1957, -2624,  -730, -2624, -2624, -2624, -2624,
   -2624, -2624,    71, -2624, -2624, -2624, -1019,  -443,  -221, -2512,
   -2624, -2624, -2624, -2624, -2624, -2624,   439, -2624, -2624,   555,
     928, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624,   217, -2624, -2624, -2624, -2624, -2624, -1120,   535,
   -2624, -2624, -2624,   675, -2624, -2624, -2624,   521, -2624, -2624,
     923, -2624, -2624, -2624, -2624,   -95,   232, -2624, -2624, -2624,
    1237,   223, -2624,  -446, -2624,  1615, -2624, -2624, -2624, -1455,
   -2020, -2624,  1341,  -771, -2198,   601, -2213, -2624, -2624, -2624,
    -406,   381, -1023, -1243, -2624, -2624, -1071,  -711,  -600, -2624,
   -2624,   515, -2624, -2624, -2624, -2624, -2624, -2624, -2624,  -565,
   -2624,   817,    -8, -2624, -2624, -2624,  1539, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -1551, -2624, -2624, -2624, -2624, -1929,
   -2624, -2624, -2624, -2624, -1820, -2624, -1512, -2624, -2624,  -770,
   -1761,  -626, -2624, -2624, -1146,  1580, -1679, -2624, -2624, -2624,
   -1457, -2624, -2624, -2624, -2455, -1256, -2624, -2624, -2623,  -831,
   -1552,  -835, -2624,  -644, -1070, -1923, -2624, -2624, -2624, -2624,
    -925, -2624, -2624, -2624,  -497,   238, -2624, -2624, -2624, -2624,
   -2624, -2624,  -840,  -505,  1587, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624,  1211,   919,   426, -2624, -2624,
    -896, -2158, -2624, -2624,   168, -2624,  -525, -1621,   -27,  -350,
   -2624, -2624,  -348, -2436, -2624, -2624, -2624, -2624, -2624, -2624,
    -315, -2624,  -873,  1647, -2624, -2624, -2624, -2624, -2624, -2624,
    1256,   876, -1640,  1614, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624,  1668, -2624, -1144,  -781, -2624,  -744, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
     865, -2624, -2624, -2624, -2624, -2624,  1034, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624, -2624,   -46,  -715, -2624,
    -723,  -782, -2624,  -795, -2624,  -392,  -909, -2624,  -564, -2490,
   -1604,  -339,  1036, -2564,  -599, -1945,    23, -2624,  -165,  -870,
    -444,    -2,    45, -1149, -2624, -2624, -2624, -2624, -2624,   568,
   -2624,   203,   211,  -105, -2624,  -314,  -495, -2624, -2624,   104,
   -2624,   659, -2624, -2624, -2624, -2624, -2624, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -2624,  1673,  1312, -2624,   976,
   -2624, -2624, -2624, -2624, -2624,  -603, -2624,   971, -1920,   902,
    -319, -2624,   -73, -2624, -2624, -2624,  -602, -2624, -2624, -2624,
   -2624, -2624, -2624, -2270,  -801, -2624, -2573, -2624, -2624, -2624,
   -2624, -2624, -2624, -2624, -2624, -1034, -2624,  -355,  -919
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1653
static const short yytable[] =
{
     314,   647,   649,   633,   635,   637,  1266,   854,  1170,   984,
     343,  1938,   348,  1551,   611,  1201,  1201,  1771,  1999,   993,
    1202,  2262,   518,  2008,   868,   311,  1703,   969,   999,   972,
     973,   974,   975,   976,  2275,  1026,  1227,  1228,  1257,  1574,
    1962,  1697,   542,   543,   692,  1465,  2327,  1576,  1138,  1140,
    1958,  1187,  2114,  1579,   786,  2132,  2133,  1517,  1878,   349,
     788,   928,  2538,  1246,  2298,  1192,  2677,  1351,   738,  2300,
     718,   719,   720,  1206,  1007,  1008,  1009,  2356, -1304,   916,
     552,  2245,   -98,  2345,  1283, -1267, -1268, -1168,    25,  2345,
   -1304,  1514,  2357,  2358,  2441,  1961,  2118,  2362,  2363,  1069,
    2035,  2009,  2009,  1996,  2042,   551,  2009,  1322,  1937,  2023,
    1542,  1207,  2343,  1743,  1545,  1997,  1610,  -123,  1279,  1280,
    1631,  2364,   512,  1223,  1607,  1964,  2395,  1926, -1553,  1581,
    2421,  2251,  2371,  2372,  2687,   530,  2414,   532,  2837,   534,
     535,  2346,  2347, -1552,   537,   538,   963, -1651,  1668,  1951,
    1124,  2287,  2416, -1267,   346,  2514,  1134,   917,  1596,  2850,
     378,   735,  1964,  1704,   738,   985,  2436,  2901,  2534,  1737,
     829,   745,   746,   747,   579, -1606,  2689,  2550,  1602,  2391,
     378,  1168,   738,  2116,   735,   507,  1611,   738,  1603,   786,
     829,   786,  1210,   786,   745,   746,   747,  2549,   384,  1724,
     378,   525,   555,  2119,  1670,  1671,  2852,   916,  2383,  2849,
    1743,  2062,  2895,  -940,  2698,   829,   319,  1953,  2384,  1663,
    1672,  2812,  1851,  2348,   625,   384,  2853,  2947,   508,  2385,
    2848,   735,  1674,  1275,   738,  1675,  2911,  1106,  2416,  2374,
     631,   745,   746,   747,   735,   384,  1232,   738,   735,   909,
    1965,   738,  2495,  1725,   745,  1966,  3030,  2741,   745,  2822,
     631,  1097,  1554,  1934,  1744,   384,   965,  2952,   316,  1107,
     571,  2835,   751,  2848,  1683,  1680,  1964,   323,  1036,  2838,
     631,   384,  3031,  1233,  2278,   917,  1197,  1965,  2982,  2063,
    1927,  1559,  1966,  2742,  2010,   572,  1967,  2671,  1612,  1912,
    2527,  1198,  1098,   527,   337,  1613,  1954,   968,   553,  2953,
   -1267, -1268,  2392,   870,  1573,   559,  1681,   528,  1211,  1614,
     803,  1682,  1553,  1090,  1557,   533,   798,  1608,   576,  2705,
    1928,  1934,  1560,  1967,   804,   578,  2011,  2012,  1926,  1099,
    2011,  2012,   509,  2688,  2505,   320,  1684,  2913,  2689,   626,
    2995,  3023,  2120,  1935,  1108,  1685,  1686,  1687,  1969,  1726,
    2437,  1744,  2406,  2979,   799,  1100,  1597,  1255,   751,  2436,
    -940,  1689, -1651,   829,  2193,  1199,  2691,  2692,  2712,  2438,
    2312,  -940,   852,  2246,  2690,  1839,   751, -1267, -1268,  2717,
    2393,   751,  1321,  2551,  1123,  1969,  2548,   492,  1690,  2699,
    2386,  1965,   852,  2553,   929,  1259,  1966,  1408,  2345,  3063,
    1691, -1267, -1268,   526,  1577,  2345,  3032,  1952,  2284,  2774,
     338,  2288,  2289,   557,  2506,   561,   810,   852,  1220,  2417,
   -1553,  1585,   632,  1923,  2228,   339,  3006,  2427,   751,  1956,
    1168,  -940,   556,  2627,  1224, -1552,   583,  1967,  1199,  3058,
     573,   751,   634,  2555, -1553,   751,  1697,  1555,  1259,  2498,
    2499,  2307,  1900,  2547,  1597,  2252,  1584,   811,  1597, -1552,
    1101,  1267,   636,  1279,  1280,  2118,  1597,   317, -1606,  1597,
     -51,  2064,  1998,  1121,  1761,  2255,   731,   348,   734,  1750,
    1490,  2854,  1890,  1970,  1542,   812,  1692,  1968,  1597,  1279,
    1280,  1927,   805,  -339,  -940, -1168,    48,   552,  1832,  1969,
    2035,   340,  1543,  1591,  1544,  2417,  1199,  1592,  1693,  2787,
    2013,  3050,  2279,  1971,  2533,  1591,  1515,   552,  1134,  1592,
    1970,   552,   795,  1125,   733,   517,  1318,  2507,  2035,   764,
    1264,  1928,  1695,  1126,  1918,  2980,   913,  2841,  2691,  2692,
    2173,  2775,  2280,  1610,  2508,   921,   875,  -940,   552,  1892,
    1971, -1304,  2424,  2437,   927,  2348,  2327,  2479,   930,  1893,
    -108,  1178,   935,  2273,  1972,  1732,  2117,  1181,  1833,   941,
     348,  2724,  2438,   923,  1193,   852,  2852,  1214,  2716,  1602,
    -109,  1875,   786,  2914,  2915,  2403,  2697,   341, -1652,  1603,
    2345,  2349,  -123,  1762,  1919,   547,  2853,  1194,  1215,  2308,
    2309,  1972,   813,  1127,  2725,  1996,  3007,   797,   785,   802,
    3051,  1001,  1002,  1611,  3073,  2348,   545,  1997,  2973,   758,
     759,   760,   761,   762,   763,   764,  1011,   510,   511,  1894,
    2091,  2092,  2093,  1511,  1970,  2949,   786,  2037,  2038,  2100,
    2043,   757,   758,   759,   760,   761,   762,   763,   764,  2788,
    1033,  -185,  2502,   546,  1179,   384,  1168,  1920,  2942,  2943,
    1048,  1790,  1791,  1572,  1971,  2071,   540,  1509,  1914,  2436,
    1058,  1059,  1060,   829,  1924,  2522,  1275,  2916,   344,  1834,
    1067,  1182,  2350,  2478,   513,  2509,   515,  1097,  1943, -1652,
     759,   760,   761,   762,   763,   764,  1275,  1763,  2792,  2404,
    1764,  1765,  1249,   759,   760,   761,   762,   763,   764,   761,
     762,   763,   764,  2824,  1128,  1972,  1975,  2497,   324,  1306,
    3008,  1766,  1964,  2950,  2493,  1612,  1767,  2457,  1098,  1641,
    2917,  1768,  1613,   601,  1250,  2630,  1307,  -940,  1843,  1845,
    1895,   548,  -185,  1259,   916,  2351,  1614,   554,   378,  1778,
     619,  1987,   560,  2196,  2334,  2951,  1664,  3009,   562,   790,
    1123,  2518,  2027,  3010,  2471,  1099,  2108,  2519,  2825,  2826,
    2827,   564,  2861,   348,   642,  1151,   565,  1251,  2109,   325,
    2520,  2110,  2862,  1123,   552,  1949,  2863,   567,  1950,   586,
     791,  1100,   523,  2040,  -344,   524,  1167,   814,   815,   816,
     817,   818,   819,  2269,   674,  -185,  1542,  2200,  1703,  1159,
    2897,  1602,  2676,   384,  2233,  1904,  1906,   327,  2290,  1252,
    1150,  1603,   917,  1697,   587,  2081,  2082,  2083,   631,  2090,
     792,  1286,  1287,  2094,  2095,  2096,  2097,  2098,  2099,  2306,
    1831,   348,   569,  1297,  1298,   570,  1134,  1965,  2230,  1037,
    1038,  2732,  1966,   588,   725,   726,   727,   729,   730,  3085,
     552,   580,  2746,  2437,   552,   552,   785,  1542,  2237,  3085,
    2926,  2267,  2268,   590,  1039,  1040,  2747,  2990,   794,  2750,
    2754,  2755,  2438,   591,  2041,  2736,  2991,  2753,  1205,   592,
     552,   552,   593,  1967,  2704,  2758,  1101,   384,   552,  2707,
     326,  1876,  2780,  1268,   865,  1621,  1269,  1622,  1272,  1591,
     327,  1623,   552,  1592,  2274,  1245,   552,  1891,  1592,  1311,
    2737,  2604,  1115,  1134,  1116,  -165,  1824,   552,  1825,  2740,
     328,  1792,  2682,  2682,  2682,   552,  1883,  1796,  1884,   176,
     177,  1304,  2678,  2679,  1310,  2247,  2248,  2751,   937,   594,
    2752,   552,  1317,   863,   864,  1969,   867,   595,  2388,  2756,
    2390,   964,  2428,   787,   971,  2365,  2366,  2264,  2265,  2369,
     977,   978,  2776,   979,   980,   981,   982,   808,  2375,  2379,
    2380,  2381,   596,   987,   988,   989,  2684,  2685,  1660,  2035,
     597,   994,   598,   996,   997,   998,   599,  1964,   870,  1000,
     600,   329,   602,  1003,   603,  1004,   604,  1005,   605,  1006,
    2294,  2295,  1964,   908,  2511,  2512,  1012,  1013,  1964,  1014,
    2536,  2537,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,   557,  1027,  1028,  1029,  1030,  1031,  1032,  2563,
    2564,  2624,  2564,  1042,  1043,   606,  1045,  1046,  1047,   608,
    1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,   609,
    2084,  2258,  1061,   940,  1062,  1063,  1064,  1065,  1066,  2714,
    2715,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1082,
    2727,  1597,  1976,  2743,  2744,   612,  1498,  1092,  2899,  2900,
    1970,   613,  1094,  1095,  1096,   614,  2867,   348,  2637,  1505,
     615,  2085,  1105,  1508,  1097,   620,  1112,   621,  1113,  3068,
     371,  2874,  2810,  2811,  2944,  2900,  1117,  1118,  1119,  1120,
    1971,   622,  1965,  1129,  1130,  1131,  1132,  1966,  1135,  1136,
    1137,  1139,  1141,  1142,  1143,  1144,  1147,  1965,  2086,  2958,
    2959,   926,  1966,  1965,  1503,  1098,  3000,  3001,  1966,   623,
     552,   376,   552,   624,  2156,  2157,   939,  3004,  2900,  2359,
     627,  1123,  1567,  1542,  2087,   628,   552,   629,  1967,  3005,
    2900,  1972,   630,   950,   638,  2335,   348,  3013,  3014,  2813,
    3016,  3017,  1099,  1967,  1578,  3018,  3017,   951,   381,  1967,
     639,  1571,  2396,   640,  2815,   552,  3070,  3001,  3071,  3001,
    1542,   948,  2376,  2377,  3084,  2537,   316,  2198,  1100,  2199,
     641,  1600,   552,   644,   382,  1085,  1086,  1087,  1088,  1089,
    1583,   645,  2194,   646,  1134,   648,   371,  3044,  3045,  1626,
    1969,  2945,  2946,   650,   651,   878,  1626,   875,   652,   373,
     653,   952,  1642,   348,   654,  1969,   552,   879,   655,   656,
     880,  1969,   657,   881,  1625,   882,   658,   659,   660,   883,
    2870,  1625,   661,  2872,   884,   662,  2873,   376,  2235,  2088,
    2236,  2876,   885,   953,   663,   664,   665,   886,   887,   666,
     667,   668,   669,  2231,   670,  1134,   671,   672,   888,   673,
    1645,   675,   889,  2892,   676,   890,  2894,   677,   954,   378,
     935,   678,  1736,   380,   381,   679,   680,   348,   681,   348,
     682,   348,   683,  1101,   684,   685,  1755,   552,   686,   398,
     687,   688,   689,   690,   691,  3020,   693,  3021,   694,   695,
     382,   696,   697,   698,  1774,  1775,   348,   699,  2670,   700,
     701,  1779,   702,   703,   704,   705,   706,   891,   707,   708,
     709,  2984,   955,   710,  1740,   956,   711,   892,  1746,  2283,
     712,  1788,   713,   714,   384,  1970,   715,  2480,  2481,   957,
     893,   716,  1795,  3055,   717,  3057,  2487,   721,  1800,   390,
    1970,   722,   723,  1772,   724,   965,  1970,   958,   983,  1806,
     728,   959,   511,   991,   995,  1971,  3060,   626,  1010,  1015,
    1813,  1034,  -818,  1093,  2331,  1110,  2842,  1111,  1114,  1121,
    1971,  1149,  1154,  1152,  1155,  1157,  1971,  1158,  1201,  1649,
    1650,  1651,  2490,  1652,  1163,  1165,  1653,  1654,  1655,  1656,
    2843,  2844,  1166,  1168,  1171,   398,  1176,  1177,  1180,  1183,
    1184,  1186,  1188,  1189,  1190,   894,  1972,  1203,  1191,  1199,
    1477,  1479,  1481,  1208,  1209,   895,  1212,   896,  1219,  1229,
    1231,  1972,  1235,  1237,  1239,  1641,  1241,  1972,  1242,  1248,
    1262,  1302,  1312,  1316,  1314,   897,  1320,  -184,  2492,  1319,
    1321,  1504,  2027,  1323,  2028,  1506,  1123,  1326,  2556,  1201,
    2557,  2434,  1328,  2539,  1327,  1877,  1332,  1335,  1516,  2759,
    1336,  1338,  1339,  1340,  1341,  1342,  1350,   898,  1356,  1357,
    1358,  2879,  1363,  -184,  1365,  1366,  2029,  1373,  1374,  1375,
    1546,  1170,  2367,  1378,  1395,  1406,  1416,  1123,  1409,  1429,
    1430,  1602,  2645,  1431,  1500,  1913,  1440,   552,  1484,  2561,
     552,  1603,  2628,  1485,  1486,  1565,  1487,  1123,  1488,  1489,
    1507,  1497,  1657,  1501,  1513,  2760,  -545,  1502,  1547,  2761,
    1947,  1548,  1917,  2845,  2846,  1159,  1550,  1552,  2461,  1562,
    1568,  1570,   552,   346,  2762,  1586,  1588,  1590,  1575,  1618,
    1963,  1619,  1629,   552,  2763,   552,  1630,  1632,  1634,  1660,
    1705,  1706,  1707,  1708,  1709,  1710,  1711,  1957,  1712,  1720,
    1713,  1714,  1715,  1716,  -184,  1717,  1718,  1979,  1974,  1719,
    1723,  1729,  1734,  1741,  1747,  1748,  1756,   552,   552,   433,
    1784,  1801,  1823,  1807,  1867,   348,  1871,  2026,   348,   348,
    1874,  2039,  1882,  2044,  2045,  2046,  -310,  2030,  2049,  1872,
    1889,  1942,  1245,  1986,  2055,  2056,  1944,  2058,  2059,  2060,
    2061,  1946,  2066,  2067,  2068,  2069,  2070,  1948,  2072,  1982,
    1983,  1977,  1984,  1988,  1989,  1991,  1995,  -184,  2027,  2007,
    2048,  2050,  1772,  2047,  2052,  2001,  2006,  2051,  2054,  2057,
    2074,  2764,  2053,  2765,  2117,  2113,  2766,  1738,  2122,  2080,
    1597,  2130,  2134,  2135,  2136,  2137,  2153,  2138,  2139,  2767,
    2140,  2151,  1753,  2145,  1754,  2154,  2158,  2161,  1757,  2162,
    2166,  2167,  2530,  1683,  1759,  1760,  2181,   552,  2174,  2223,
    2115,  2191,  2218,  2226,  2229,  2241,  1776,  2768,  1777,  2242,
    2239,  1935,  1780,  1781,  2266,  2270,  2276,  2277,  1934,  2296,
    1782,  2285,  2112,  1783,  2303,  2937,  2938,  2939,  1786,  2299,
    2305,  1789,  2322,  2337,  2389,  2410,  1735,  2422,  1793,  2429,
    1794,  2430,  2769,  2435,  1797,  1798,  1799,  2432,  2360,  2361,
    1802,  2442,  1803,  2443,  1804,  2448,  1805,  1123,  2449,  2451,
    1808,  2453,  1809,  2468,  1810,  2469,  1811,  2476,  1812,  2807,
    1814,  2485,  2482,  2486,  2489,  1815,  2491,  1816,  2494,  1817,
    1961,  2496,  2500,  2503,  2516,  2504,  2532,  1818,  1819,  2545,
    1820,  2523,  1821,  1822,  2524,  2546,  1826,  1827,  1828,  1829,
    1830,  2535,  2623,  1835,  1836,  1837,  1838,  2625,  1840,  1841,
    1842,  1844,  1846,  1847,  1848,  1849,  1850,  2629,  1852,  1853,
    2631,  1854,  2633,  1856,  1857,  1858,  1859,  1860,  2635,  1862,
    2636,  1864,  2638,  1866,  2639,  2646,  2660,  2647,  2661,  2653,
    2222,  2657,  2658,  2662,  1868,  2666,  2667,  1869,  1870,  2669,
    2672,  2227,  1284,  1285,  1286,  1287,  1288,  1289,  1290,  2877,
    1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  2673,  1880,
    1881,  2283,  2674,  1885,  1886,  1887,  1888,  2700,  2702,  2703,
    1896,  1897,  1898,  1899,  2706,  1901,  1902,  1903,  1905,  1907,
    1908,  1909,  1910,  1911,  2695,  2009,  2718,  2719,  2720,  2723,
    2731,  2799,  2733,  2757,  2777,  2778,  1925,  2779,  2331,  2259,
    2261,  2784,  2781,  2800,  2801,  2803,  2748,  2809,  2806,  2817,
    2819,  2820,   371,  2821,  2830,  2831,  2836,  2839,  2715,  2840,
    2859,  2883,  2860,  2869,  2875,   373,  2774,  2868,  2871,  2882,
     766,   767,   768,   769,   770,  2293,  2885,  2884,  2887,  2888,
    2889,   921,   348,  2890,  2891,  2893,  2902,   348,  1642,  2302,
    2903,  2907,  2908,   376,  2920,  2922,   348,   771,   772,   773,
     774,  2936,  2929,  2931,  2934,  2313,  2957,  1267,  2966,  2967,
    -940,  1567,   775,   776,   777,   778,  2964,   916,   779,  3093,
    2968,  2368,  2959,   348,   348,   378,  2080,  2370,  2969,   380,
     381,  2970,  2080,  2971,  2975,  2080,  2080,  2976,   780,   781,
    2080,  2080,  2985,  2972,  2986,  2987,  -940,  2988,  2994,  2997,
    2998,  2999,  3015,  3037,  3024,  3033,   382,  3039,  3040,  3042,
     782,   783,  -940,   552,  2512,   348,  3047,   348,  2406,  2080,
    2342,  1772,  3049,  2080,  2402,  3054,  3035,  3036,  3061,  3038,
    3074,  3078,  2797,  3079,  3083,  3089,  2121,  2101,  2382,  2896,
     384,  1721,  2786,  1662,   568,   917,  2073,  1170,  2941,  3059,
     552,  2080,  3075,  3082,  2415,   390,  2729,  2530,  3056,  -991,
    3091,  2077,  1205,  3092,  1205,  2031,  2352,  2721,  2407,  2738,
    2728,  2739,  2782,  2960,  2851,  2411,  2886,  2878,  3065,  2722,
    3066,  3067,  2842,  2517,  3052,  3048,  2956,  3012,  1728,  2127,
    2974,  2963,  2554,  1960,  2475,  2397,  1589,  1601,  2243,  2240,
    1985,  1263,  1978,  2996,   990,  2244,  2843,  2844,  -940,  -940,
     912,   398,  1175,  1933,  3003,  2150,  1773,  3094,  2927,  2805,
     947,  -940,   970,  -940,  1330,  2254,  -940,  1646,  2123,  3081,
    2325,  2834,  2710,  2709,  -370,  2675,  3072,  1309,  1733,   936,
     857,   911,  2474,  1749,  2978,  2989,  3029,  3041,  2713,  2881,
    1558,  2301,  1990,  2311,  2190,  1665,  2525,  1666,  2701,  2832,
    1566,  1922,   906,  1667,  2195,  1668,  1247,  -940,  1628,  1633,
    2935,  2470,  2513,  1742,     0,  1669,  2940,     0,  -940,     0,
       0,     0,     0,     0,     0,  -940,  2204,   552,     0,     0,
       0,     0,  2212,     0,  2214,     0,  2216,     0,     0,     0,
       0,     0,     0,     0,  2488,     0,     0,     0,     0,     0,
    2224,     0,  1917,  2225,     0,     0,     0,  -940,     0,     0,
       0,  1670,  1671,     0,     0,  2232,     0,     0,     0,  -940,
       0,     0,     0,     0,     0,     0,     0,  1672,     0,  2845,
    2846,     0,     0,  1979,     0,     0,  1673,     0,     0,  1674,
    1275,   348,  1675,  -940,     0,     0,  1676,  1677,  -940,  1642,
       0,  1678,  -940,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -940,     0,     0,     0,
       0,  1679,  1680,     0,     0,     0,     0,     0,  2342,     0,
       0,  2080,     0,     0,     0,     0,     0,     0,  2080,     0,
       0,     0,     0,     0,     0,   348,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  3027,     0,     0,
       0,     0,     0,  1681,     0,     0,     0,   552,  1682,  2318,
       0,     0,     0,     0,     0,     0,  -940,     0,     0,     0,
       0,  2565,     0,  2338,   552,     0,     0,     0,  2621,  1683,
     552,     0,  1772,  1684,  -940,     0,     0,     0,     0,     0,
       0,     0,  1685,  1686,  1687,  1267,     0,  1267,     0,  2566,
    1688,     0,     0,     0,     0,  2622,     0,   348,  1689,     0,
       0,  -940,     0,     0,  -940,     0,     0,     0,     0,     0,
     633,   635,   637,     0,     0,     0,     0,     0,     0,     0,
    3027,     0,     0,     0,     0,  1690,     0,     0,     0,     0,
       0,     0,     0,  2965,     0,     0,  2331,  1691,     0,     0,
     348,     0,     0,     0,  1772,     0,     0,  1913,     0,     0,
       0,     0,     0,     0,  -940,  -940,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2433,     0,     0,
    2686,     0,     0,     0,   348,     0,  -940,     0,     0,     0,
       0,     0,     0,   916,     0,     0,     0,  2665,  2444,     0,
       0,     0,     0,  2445,  2446,  1567,  2447,     0,     0,     0,
       0,  1267,  1267,  2080,  1267,  2450,     0,     0,     0,     0,
    2080,     0,  -940,     0,     0,     0,     0,  2454,     0,     0,
    3019,  2455,  2080,  1692,  2456,     0,     0,     0,  -940,     0,
       0,     0,     0,  1267,     0,     0,     0,     0,  2459,     0,
       0,     0,  2460,     0,     0,  1693,     0,  2462,     0,     0,
    2463,  2464,     0,  1267,     0,  1267,  1267,     0,     0,  1694,
       0,   917,     0,     0,     0,     0,     0,     0,     0,  1695,
       0,     0,     0,     0,     0,  2473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2791,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,     0,     0,     0,     0,  2798,
       0,     0,     0,     0,     0,  -940,     0,     0,     0,     0,
       1,     0,     2,     0,  -940,  -940,     0,     0,     0,  -370,
       0,     0,  -940,     0,     0,     0,     0,  -940,     0,  -940,
       0,     0,  -940,     3,     4,     0,     0,     5,     0,     6,
       0,     0,     7,  2526,     0,     8,   601,     0,     9,     0,
      10,    11,     0,    12,     0,    13,    14,    15,     0,    16,
      17,     0,    18,     0,     0,     0,  2791,    19,    20,    21,
       0,    22,    23,  -940,    24,    25,    26,    27,     0,  2544,
      28,     0,     0,    29,  -940,     0,     0,    30,    31,     0,
      32,  -940,     0,     0,     0,     0,     0,     0,     0,    33,
       0,     0,     0,     0,  2558,     0,     0,     0,     0,    34,
     348,     0,     0,    35,    36,     0,     0,     0,     0,     0,
       0,     0,     0,  -940,     0,     0,     0,     0,     0,    37,
      38,     0,     0,     0,     0,  -940,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,   552,     0,     0,
       0,     0,     0,     0,     0,    39,     0,  2342,     0,  -940,
       0,     0,  1668,  -940,  -940,  2933,     0,     0,  -940,     0,
       0,     0,  2909,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   751,   878,  2791,  2791,  2651,
       0,     0,  -940,     0,     0,     0,     0,     0,   879,     0,
       0,   880,  2955,  2659,   881,     0,   882,     0,     0,  1668,
     883,     0,     0,     0,     0,   884,   754,     0,  1670,  1671,
       0,     0,     0,   885,     0,     0,     0,     0,   886,   887,
       0,     0,     0,     0,  1672,     0,  2680,     0,     0,   888,
       0,     0,     0,   889,     0,     0,  1674,  1275,  2791,  1675,
       0,     0,  -940,     0,     0,     0,    40,     0,     0,     0,
      41,     0,    42,     0,     0,  1670,  1671,    43,  1668,     0,
    -940,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -303,  1672,  2293,     0,  -303,     0,     0,     0,     0,  1680,
       0,     0,     0,  1674,  1275,     0,  1675,  -940,   891,     0,
    -940,     0,     0,     0,     0,     0,     0,    44,   892,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   893,     0,     0,  1670,  1671,     0,   756,     0,     0,
    1681,     0,     0,     0,     0,  1682,  1680,     0,     0,  -303,
    1672,     0,     0,  -303,    45,    46,  2933,  2933,     0,  3046,
    -940,  -940,  1674,  1275,     0,  1675,     0,     0,     0,     0,
    1684,     0,  3053,     0,     0,  2794,     0,     0,   878,  1685,
    1686,  1687,  2795,     0,     0,  2796,     0,  1681,     0,     0,
     879,  3064,  1682,   880,     0,  1689,   881,     0,   882,     0,
       0,     0,   883,     0,     0,  1680,   894,   884,     0,     0,
       0,     0,     0,     0,     0,   885,   895,  1684,   896,     0,
     886,   887,  1690,     0,     0,     0,  1685,  1686,  1687,     0,
       0,   888,     0,  1567,  1691,   889,   897,     0,   904,     0,
       0,     0,  1689,     0,     0,  2318,  1681,     0,     0,     0,
       0,  1682,     0,    47,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,  2558,     0,     0,     0,   898,  1690,
       0,     0,     0,     0,     0,     0,  1684,     0,     0,     0,
       0,  1691,     0,     0,     0,  1685,  1686,  1687,     0,     0,
     891,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     892,  1689,  -303,    48,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   893,     0,     0,     0,     0,     0,     0,
    1692,     0,     0,     0,     0,     0,     0,     0,  1690,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1691,  2558,  1693,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2928,     0,  2930,     0,     0,     0,
       0,  -303,     0,     0,     0,     0,  1695,  1692,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   894,  1693,
       0,     0,     0,     0,     0,     0,     0,     0,   895,     0,
     896,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1695,     0,     0,     0,     0,   897,     0,
       0,     0,     0,     0,     0,     0,  1692,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2977,  1693,     0,
     898,     0,     0,  2983,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1695,  2420,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,   100,   101,
     102,   103,     0,     0,     0,   104,     0,     0,     0,     0,
       0,     0,   351,     0,     0,     0,     0,     0,   105,     0,
       0,   106,   352,   108,   109,     0,   110,   111,   112,   113,
     353,  2558,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,   120,   354,   121,     0,     0,   122,  3043,   123,
     355,   356,   124,     0,     0,   125,   126,   127,   128,   129,
     130,     0,     0,   131,   132,   133,   134,   357,   358,   135,
     359,   360,   137,   361,     0,   139,     0,   140,   141,     0,
       0,     0,     0,   142,   143,   144,   145,   362,   146,   147,
     148,  1079,   149,     0,   150,   151,     0,   363,   364,   153,
       0,   154,     0,   365,     0,     0,   156,     0,   366,   367,
       0,   157,   368,     0,   158,     0,     0,   159,   160,   161,
       0,   162,   163,   164,     0,     0,   165,   166,   369,   167,
     370,   168,   169,   170,   371,     0,  3090,     0,   171,     0,
     172,     0,   173,   372,     0,     0,   174,   373,     0,   175,
     176,   177,     0,   178,     0,   179,     0,     0,   180,     0,
       0,   181,     0,     0,     0,  1080,   374,   182,   183,     0,
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
     246,     0,   384,     0,  1081,   247,   385,   248,   249,   386,
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
       0,     0,   487,   488,     0,   489,   490,   491,    99,   100,
     101,   102,   103,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,   351,     0,     0,     0,     0,     0,   105,
       0,     0,   106,   352,   108,   109,     0,   110,   111,   112,
     113,   353,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,   120,   354,   121,     0,     0,   122,     0,
     123,   355,   356,   124,     0,     0,   125,   126,   127,   128,
     129,   130,     0,     0,   131,   132,   133,   134,   357,   358,
     135,   359,   360,   137,   361,     0,   139,     0,   140,   141,
    2314,     0,     0,     0,   142,   143,   144,   145,  2315,   146,
     147,   148,     0,   149,     0,   150,   151,     0,   363,   364,
     153,     0,   154,     0,   365,     0,     0,   156,     0,   366,
     367,     0,   157,  2316,     0,   158,     0,     0,   159,   160,
     161,     0,   162,   163,   164,     0,     0,   165,   166,   369,
     167,   370,   168,   169,   170,   371,     0,     0,     0,   171,
       0,   172,     0,   173,   372,     0,     0,   174,   373,     0,
     175,   176,   177,     0,   178,     0,   179,     0,     0,   180,
       0,     0,   181,     0,     0,     0,     0,   374,   182,   183,
       0,     0,     0,   184,   375,   185,   376,     0,     0,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   377,   202,   203,   204,   205,
     206,   207,   208,   209,     0,   210,   211,   212,   378,   213,
     379,   214,   380,   381,   215,  2317,   216,   217,     0,     0,
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
     128,   129,   130,   728,     0,   131,   132,   133,   134,   357,
     358,   135,   359,   360,   137,   361,     0,   139,     0,   140,
     141,     0,     0,     0,     0,   142,   143,   144,   145,   362,
     146,   147,   148,     0,   149,     0,   150,   151,     0,   363,
     364,   153,     0,   154,     0,   365,     0,     0,   156,     0,
     366,   367,     0,   157,   368,     0,   158,     0,     0,   159,
     160,   161,     0,   162,   163,   164,     0,     0,   165,   166,
     369,   167,   370,   168,   169,   170,   371,     0,     0,     0,
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
       0,   171,     0,   172,     0,   173,   372,     0,     0,   174,
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
     485,     0,   486,   983,     0,   487,   488,     0,   489,   490,
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
       0,     0,   171,     0,   172,     0,   173,   372,     0,     0,
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
       0,   485,     0,   486,  1044,     0,   487,   488,     0,   489,
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
       0,     0,   485,     0,   486,  1070,     0,   487,   488,     0,
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
     431,   283,   432,   284,   433,   434,   435,   436,   437,   438,
     439,   440,   441,     0,     0,   442,   288,   289,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,     0,   458,   459,   460,   461,   462,     0,
     299,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,     0,   302,
       0,   480,   481,   482,   304,     0,     0,   305,     0,   306,
       0,   307,   308,   309,     0,     0,     0,   483,   484,     0,
       0,     0,     0,   485,     0,   486,  1091,     0,   487,   488,
       0,   489,   490,   491,    99,   100,   101,   102,   103,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,   351,
       0,     0,     0,     0,     0,   105,     0,     0,  1145,   352,
     108,   109,     0,   110,   111,   112,   113,   353,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,   120,
     354,   121,     0,     0,   122,     0,   123,   355,   356,   124,
       0,     0,   125,   126,   127,   128,   129,   130,     0,     0,
     131,   132,   133,   134,   357,   358,   135,   359,   360,   137,
     361,     0,   139,     0,   140,   141,  1146,     0,     0,     0,
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
     368,     0,   158,     0,     0,   159,   160,   161,     0,   162,
     163,   164,     0,     0,   165,   166,   369,   167,   370,   168,
     169,   170,   371,     0,     0,  1476,   171,     0,   172,     0,
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
     168,   169,   170,   371,     0,     0,  1478,   171,     0,   172,
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
     483,   484,     0,     0,     0,     0,   485,     0,   486,     0,
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
     370,   168,   169,   170,   371,     0,     0,  1480,   171,     0,
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
       0,     0,   487,   488,     0,   489,   490,   491,    99,   100,
     101,   102,   103,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,   351,     0,     0,     0,     0,     0,   105,
       0,     0,   106,   352,   108,   109,     0,   110,   111,   112,
     113,   353,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,   120,   354,   121,     0,     0,   122,     0,
     123,   355,   356,   124,     0,     0,   125,   126,   127,   128,
     129,   130,     0,     0,   131,   132,   133,   134,   357,   358,
     135,   359,   360,   137,   361,     0,   139,     0,   140,   141,
       0,     0,     0,     0,   142,   143,   144,   145,   362,   146,
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
     369,   167,   370,   168,   169,   170,   371,     0,     0,     0,
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
    1758,   434,   435,   436,   437,   438,   439,   440,   441,     0,
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
       0,   171,     0,   172,     0,   173,   372,     0,     0,   174,
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
     284,  1855,   434,   435,   436,   437,   438,   439,   440,   441,
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
     156,     0,   366,   367,     0,   157,  2540,     0,   158,     0,
       0,   159,   160,   161,     0,   162,   163,   164,     0,     0,
     165,   166,   369,   167,   370,   168,   169,   170,   371,     0,
       0,     0,   171,     0,   172,     0,   173,   372,     0,     0,
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
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,   106,   107,   108,   109,
       0,   110,   111,   112,   113,     0,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,   120,     0,   121,
       0,     0,   122,     0,   123,     0,     0,   124,     0,     0,
     125,   126,   127,   128,   129,   130,  2009,     0,   131,   132,
     133,   134,     0,     0,   135,     0,   136,   137,   138,     0,
     139,     0,   140,   141,     0,     0,     0,     0,   142,   143,
     144,   145,     0,   146,   147,   148,     0,   149,     0,   150,
     151,     0,     0,   152,   153,     0,   154,     0,   155,     0,
       0,   156,  2028,     0,     0,     0,   157,     0,     0,   158,
       0,     0,   159,   160,   161,     0,   162,   163,   164,     0,
       0,   165,   166,     0,   167,     0,   168,   169,   170,     0,
       0,     0,     0,   171,  2029,   172,     0,   173,     0,     0,
       0,   174,     0,     0,   175,   176,   177,     0,   178,  1602,
     179,     0,     0,   180,     0,     0,   181,     0,     0,  1603,
       0,     0,   182,   183,     0,  2412,     0,   184,     0,   185,
       0,     0,     0,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,     0,   210,
     211,   212,     0,   213,     0,   214,     0,     0,   215,     0,
     216,   217,     0,     0,     0,     0,     0,     0,     0,   218,
     219,   220,     0,     0,   221,   222,   223,   224,   225,   226,
     227,   228,     0,     0,     0,     0,   229,     0,   230,     0,
     231,   232,     0,     0,   233,   234,   235,   236,     0,     0,
     237,   238,   239,   240,   241,  2030,     0,     0,   242,   243,
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
       0,  2076,   115,     0,   116,   117,   118,   119,   120,     0,
     121,     0,     0,   122,     0,   123,     0,     0,   124,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,   131,
     132,   133,   134,     0,     0,   135,     0,   136,   137,   138,
       0,   139,     0,   140,   141,     0,     0,     0,     0,   142,
     143,   144,   145,     0,   146,   147,   148,     0,   149,     0,
     150,   151,     0,     0,   152,   153,     0,   154,     0,   155,
       0,     0,   156,  2028,     0,     0,     0,   157,     0,     0,
     158,     0,     0,   159,   160,   161,     0,   162,   163,   164,
       0,     0,   165,   166,     0,   167,     0,   168,   169,   170,
       0,     0,     0,     0,   171,  2029,   172,     0,   173,     0,
       0,     0,   174,     0,     0,   175,   176,   177,     0,   178,
    1602,   179,     0,     0,   180,     0,     0,   181,     0,     0,
    1603,     0,     0,   182,   183,     0,     0,     0,   184,     0,
     185,     0,     0,     0,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,     0,   213,     0,   214,     0,     0,   215,
       0,   216,   217,     0,     0,     0,     0,     0,     0,     0,
     218,   219,   220,     0,     0,   221,   222,   223,   224,   225,
     226,   227,   228,     0,     0,     0,     0,   229,     0,   230,
       0,   231,   232,     0,     0,   233,   234,   235,   236,     0,
       0,   237,   238,   239,   240,   241,  2030,     0,     0,   242,
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
     114,     0,  2076,   115,     0,   116,   117,   118,   119,   120,
       0,   121,     0,     0,   122,     0,   123,     0,     0,   124,
       0,     0,   125,   126,   127,   128,   129,   130,  2009,     0,
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
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
     302,     0,   303,     0,     0,   304,     0,     0,   305,     0,
     306,     0,   307,   308,   309,    99,   100,   101,   102,   103,
       0,     0,     0,   104,     0,     0,     0,  2326,     0,     0,
       0,     0,     0,   490,     0,     0,   105,     0,     0,   106,
     107,   108,   109,     0,   110,   111,   112,   113,     0,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
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
       0,   178,     0,   179,     0,     0,   180,  1258,     0,   181,
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
       0,     0,   114,     0,   310,   115,     0,   116,   117,   118,
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
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
       0,     0,   302,     0,   303,     0,     0,   304,     0,     0,
     305,     0,   306,     0,   307,   308,   309,    99,   100,   101,
     102,   103,  1624,     0,     0,   104,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,     0,     0,   105,     0,
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
     101,   102,   103,     0,     0,     0,   104,     0,     0,  2075,
       0,     0,     0,     0,     0,     0,  2076,     0,     0,   105,
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
       0,     0,     0,     0,     0,     0,     0,     0,   301,     0,
       0,     0,     0,     0,   302,     0,   303,     0,     0,   304,
       0,     0,   305,     0,   306,     0,   307,   308,   309,    99,
     100,   101,   102,   103,     0,     0,     0,   104,     0,     0,
       0,  3080,     0,     0,     0,     0,     0,   490,     0,     0,
     105,     0,     0,   106,   107,   108,   109,     0,   110,   111,
     112,   113,     0,     0,     0,   114,     0,     0,   115,     0,
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
      99,   100,   101,   102,   103,   302,     0,   303,   104,     0,
     304,     0,     0,   305,     0,   306,     0,   307,   308,   309,
       0,   105,     0,     0,   106,   107,   108,   109,     0,   110,
     111,   112,   113,     0,     0,     0,   114,     0,   310,   115,
       0,   116,   117,   118,   119,   120,     0,   121,     0,     0,
     122,     0,   123,     0,     0,   124,     0,     0,   125,   126,
     127,   128,   129,   130,     0,     0,   131,   132,   133,   134,
       0,     0,   135,     0,   136,   137,   138,     0,   139,     0,
     140,   141,     0,     0,     0,     0,   142,   143,   144,   145,
       0,   146,   147,   148,     0,   149,     0,   150,   151,     0,
       0,   152,   153,     0,   154,     0,   155,     0,     0,   156,
       0,     0,     0,     0,   157,     0,     0,   158,     0,     0,
     159,   160,   161,     0,   162,   163,   164,     0,     0,   165,
     166,     0,   549,     0,   168,   169,   170,     0,     0,     0,
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
     301,    99,   100,   101,   102,   103,   302,     0,   303,   104,
       0,   304,     0,     0,   305,     0,   306,     0,   307,   308,
     309,     0,   105,     0,     0,   106,   107,   108,   109,     0,
     110,   111,   112,   113,     0,     0,     0,   114,     0,   310,
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
       0,   301,    99,   100,   101,   102,   103,   302,     0,   303,
     104,     0,   304,     0,     0,   305,     0,   306,     0,   307,
     308,   309,     0,   105,     0,     0,   106,   107,   108,   109,
       0,   110,   111,   112,   113,     0,     0,     0,   114,     0,
     490,   115,     0,   116,   117,   118,   119,   120,     0,   121,
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
       0,  2076,   115,     0,   116,   117,   118,   119,   120,     0,
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
       0,     0,     0,   182,   183,   345,     0,     0,   184,     0,
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
     114,   732,     0,   115,     0,   116,   117,   118,   119,   120,
       0,   121,     0,     0,   122,     0,   123,     0,     0,   124,
       0,     0,   125,   126,   127,   128,   129,   130,     0,     0,
     131,   132,   133,   134,     0,     0,   135,     0,   136,   137,
     138,     0,   139,     0,   140,   141,     0,     0,     0,     0,
     142,   143,   144,   145,     0,   146,   147,   148,     0,   149,
       0,   150,   151,     0,     0,  1635,   153,     0,   154,     0,
     155,     0,     0,   156,     0,     0,     0,     0,   157,  1636,
       0,   158,     0,     0,   159,   160,   161,     0,   162,   163,
     164,     0,     0,   165,   166,     0,   167,     0,   168,   169,
     170,     0,     0,     0,     0,   171,     0,   172,     0,   173,
       0,     0,     0,   174,     0,     0,   175,   176,   177,     0,
     178,     0,   179,     0,     0,   180,     0,     0,   181,     0,
       0,     0,     0,     0,   182,   183,     0,     0,     0,   184,
       0,   185,     0,     0,     0,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,  1637,   209,
       0,   210,   211,   212,     0,   213,     0,   214,     0,     0,
     215,     0,   216,   217,     0,     0,     0,     0,     0,     0,
       0,   218,   219,   220,     0,     0,   221,   222,   223,   224,
     225,   226,   227,   228,     0,     0,     0,     0,   229,     0,
     230,     0,   231,   232,     0,     0,   233,   234,   235,   236,
       0,     0,   237,   238,   239,   240,   241,     0,     0,     0,
     242,   243,     0,   244,     0,     0,   245,   246,     0,     0,
       0,     0,  1638,     0,   248,   249,     0,     0,     0,     0,
     250,     0,   251,   252,     0,   253,     0,     0,     0,   254,
       0,     0,   255,     0,   256,     0,     0,     0,     0,   257,
       0,   258,   259,   260,   261,     0,     0,   852,   262,     0,
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
       0,     0,     0,   291,   292,   293,     0,   294,  1639,     0,
     296,     0,   297,     0,     0,     0,     0,     0,     0,   298,
       0,   299,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,    99,   100,   101,   102,   103,
     302,     0,   303,   104,     0,   304,     0,     0,   305,     0,
     306,     0,   307,   308,   309,     0,   105,     0,     0,   106,
     107,   108,   109,     0,   110,   111,   112,   113,     0,     0,
       0,   114,  1640,     0,   115,     0,   116,   117,   118,   119,
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
       0,     0,     0,     0,     0,   182,   183,   345,     0,     0,
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
     346,     0,     0,   247,     0,   248,   249,     0,     0,     0,
       0,   250,     0,   251,   252,     0,   253,     0,     0,     0,
     254,     0,     0,   255,     0,   256,     0,     0,     0,     0,
     257,     0,   258,   259,   260,   261,     0,     0,     0,   262,
       0,   263,   264,     0,     0,   265,   266,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,   269,
     270,     0,   271,     0,   272,   273,   274,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,     0,     0,   278,   279,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   280,     0,     0,   281,     0,
       0,     0,   282,   283,     0,   284,     0,     0,     0,     0,
     285,     0,     0,     0,   286,     0,     0,   287,   288,   289,
     290,     0,     0,     0,   291,   292,   293,     0,   294,   295,
       0,   296,     0,   297,     0,     0,   748,     0,     0,     0,
     298,     0,   299,   300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   301,     0,     0,     0,     0,
       0,   302,     0,   303,     0,     0,   304,     0,     0,   305,
       0,   306,     0,   307,   308,   309,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  2000,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,   748,     0,     0,   752,   753,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,     0,     0,     0,   754,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   748,     0,     0,     0,     0,     0,     0,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,     0,     0,     0,     0,
       0,     0,   749,     0,     0,     0,     0,   748,     0,   755,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   748,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     749,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     750,   751,     0,   749,     0,     0,     0,   756,     0,   748,
       0,   752,   753,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,     0,     0,     0,     0,   749,     0,   751,
       0,     0,     0,     0,     0,     0,     0,   750,     0,   752,
     753,     0,   751,     0,     0,   749,     0,     0,     0,     0,
       0,     0,   752,   753,     0,   750,     0,     0,     0,     0,
     754,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   754,     0,     0,   751,     0,     0,   749,
       0,     0,     0,     0,     0,   755,   752,   753,     0,   750,
       0,     0,     0,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   753,     0,   754,     0,     0,
       0,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,   755,     0,   754,     0,  1348,   751,     0,
    1499,     0,     0,   756,     0,     0,   755,     0,   752,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     755,   756,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   756,     0,     0,     0,   755,     0,
       0,     0,     0,     0,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   756,     0,
       0,     0,   755,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   756,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   757,   758,   759,   760,   761,   762,   763,   764,     0,
       0,     0,     0,     0,     0,     0,  3069,     0,     0,     0,
     756,     0,   748,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,     0,     0,     0,   757,
     758,   759,   760,   761,   762,   763,   764,     0,     0,     0,
    1103,  1104,   757,   758,   759,   760,   761,   762,   763,   764,
       0,     0,     0,     0,  1349,     0,   748,     0,     0,     0,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,     0,     0,     0,   757,   758,   759,   760,
     761,   762,   763,   764,     0,     0,     0,  1354,  1355,     0,
       0,   748,   749,     0,   757,   758,   759,   760,   761,   762,
     763,   764,   750,     0,     0,  1368,  1369,     0,     0,     0,
       0,     0,     0,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,   749,     0,   748,  1371,
    1372,   751,     0,     0,     0,     0,   750,     0,     0,     0,
       0,   752,   753,     0,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   749,   754,     0,     0,     0,     0,     0,     0,     0,
       0,   750,     0,     0,     0,   751,     0,     0,     0,     0,
       0,   748,     0,     0,     0,   752,   753,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   754,     0,   749,     0,
     751,     0,     0,     0,     0,     0,     0,     0,   750,     0,
     752,   753,     0,     0,     0,   755,   748,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   754,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,   749,     0,     0,     0,     0,     0,   752,   753,   755,
       0,   750,     0,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   755,     0,   749,     0,     0,     0,
     751,     0,     0,     0,     0,     0,   750,   756,     0,     0,
     752,   753,     0,     0,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,   754,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   755,   756,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,   753,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,     0,   754,     0,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,   756,
       0,     0,  1386,  1387,   755,   748,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,  1388,  1389,     0,   755,
     748,     0,   756,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     757,   758,   759,   760,   761,   762,   763,   764,     0,     0,
       0,  1392,  1393,     0,     0,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,   756,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,     0,     0,     0,  1398,  1399,
     749,     0,     0,     0,   751,     0,     0,     0,     0,     0,
     750,     0,     0,     0,   752,   753,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
       0,     0,     0,     0,     0,   754,     0,     0,     0,     0,
     757,   758,   759,   760,   761,   762,   763,   764,     0,   751,
       0,  1400,  1401,     0,     0,     0,     0,     0,     0,   752,
     753,     0,     0,     0,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     754,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,   748,     0,  1402,  1403,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   755,     0,   748,   756,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,     0,   749,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   750,     0,     0,     0,     0,     0,
       0,   756,     0,     0,     0,     0,     0,   748,     0,     0,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   751,     0,   749,     0,     0,     0,     0,
       0,     0,     0,   752,   753,   750,     0,     0,   748,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,     0,     0,   751,  1404,  1405,   749,     0,   748,
       0,     0,     0,     0,   752,   753,     0,   750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   754,     0,     0,     0,   757,
     758,   759,   760,   761,   762,   763,   764,   755,   749,     0,
    1414,  1415,     0,     0,     0,     0,   751,     0,   750,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   754,     0,   749,
       0,     0,     0,     0,     0,   756,     0,   751,   755,   750,
       0,     0,     0,     0,     0,     0,     0,   752,   753,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   751,     0,
       0,     0,     0,     0,     0,     0,   756,     0,   752,   753,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
       0,     0,     0,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,     0,     0,     0,     0,
       0,   755,     0,     0,     0,     0,     0,     0,   756,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,     0,     0,     0,  1423,  1424,     0,     0,     0,     0,
       0,     0,   755,     0,     0,     0,     0,     0,     0,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   748,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,     0,     0,     0,  1425,  1426,     0,     0,     0,
     756,     0,     0,     0,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,     0,     0,     0,   757,   758,   759,   760,
     761,   762,   763,   764,     0,     0,     0,  1427,  1428,     0,
       0,   749,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   750,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,   748,     0,     0,  1435,  1436,
       0,     0,     0,     0,     0,     0,     0,     0,   748,     0,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,   753,     0,     0,     0,     0,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,  1438,
    1439,   754,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1473,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,     0,
       0,     0,     0,     0,   755,     0,     0,     0,     0,     0,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   753,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,   753,     0,
       0,     0,   756,     0,     0,   754,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,     0,
       0,     0,     0,     0,     0,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   748,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   755,     0,     0,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,   748,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   756,     0,   749,     0,
     757,   758,   759,   760,   761,   762,   763,   764,   750,   756,
       0,  1469,  1470,   748,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,   753,     0,
       0,     0,     0,   749,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,   754,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
     748,     0,   751,     0,   757,   758,   759,   760,   761,   762,
     763,   764,   752,   753,     0,   749,  1474,   757,   758,   759,
     760,   761,   762,   763,   764,   750,     0,     0,  1492,  1493,
       0,   755,   751,   754,     0,     0,     0,     0,     0,     0,
       0,     0,   752,   753,     0,     0,     0,   748,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   749,   754,   751,     0,     0,     0,     0,     0,
       0,     0,   750,     0,   752,   753,     0,     0,     0,   756,
     749,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     750,     0,     0,     0,     0,   754,   755,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,   753,     0,     0,     0,   755,   749,     0,   751,
       0,     0,     0,     0,     0,     0,     0,   750,     0,   752,
     753,     0,   754,     0,   756,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   755,     0,
     754,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   756,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,   755,   756,   754,  1495,  1496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,   756,     0,     0,     0,     0,     0,     0,
       0,     0,   757,   758,   759,   760,   761,   762,   763,   764,
     755,   756,     0,  2179,  2180,     0,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
       0,     0,   757,   758,   759,   760,   761,   762,   763,   764,
       0,     0,     0,  2182,  2183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   748,   756,     0,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,     0,     0,     0,  2208,  2209,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,     0,   748,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,     0,
       0,     0,  2643,  2644,     0,     0,     0,     0,     0,   757,
     758,   759,   760,   761,   762,   763,   764,   749,     0,     0,
    2905,  2906,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,   748,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   748,   749,     0,   757,   758,   759,   760,
     761,   762,   763,   764,   750,     0,   751,     0,  1331,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
       0,     0,     0,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,     0,   754,     0,     0,
       0,     0,     0,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,   753,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,     0,   749,   754,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     755,   748,     0,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   753,     0,     0,     0,     0,
       0,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,   753,     0,   754,     0,   755,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   756,     0,
       0,     0,     0,   754,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   749,     0,     0,     0,   756,     0,     0,     0,     0,
       0,   750,     0,     0,     0,     0,     0,     0,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   755,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,   753,     0,     0,     0,     0,   756,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,   754,     0,     0,   756,     0,   757,   758,   759,   760,
     761,   762,   763,   764,     0,     0,     0,     0,  1337,     0,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,     0,     0,     0,     0,  1343,     0,     0,     0,     0,
       0,     0,     0,     0,   755,   748,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,     0,     0,     0,     0,   748,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,   756,     0,     0,     0,  1344,     0,     0,     0,
       0,     0,   757,   758,   759,   760,   761,   762,   763,   764,
       0,     0,     0,     0,  1352,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
     748,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   749,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,     0,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   753,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,   754,     0,     0,   751,     0,
     757,   758,   759,   760,   761,   762,   763,   764,   752,   753,
     749,     0,  1353,     0,     0,     0,     0,     0,     0,     0,
     750,     0,     0,     0,     0,     0,     0,     0,     0,   754,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   748,     0,   755,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
     753,     0,     0,     0,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
     754,     0,   755,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   756,     0,     0,   748,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,     0,     0,
     756,     0,     0,     0,     0,     0,   750,     0,     0,     0,
       0,     0,   748,   755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   751,     0,     0,     0,   749,
       0,     0,     0,     0,     0,   752,   753,     0,     0,   750,
       0,   756,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   754,     0,     0,     0,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,   749,     0,     0,     0,  1359,     0,   751,     0,
       0,     0,   750,     0,     0,     0,     0,     0,   752,   753,
     748,     0,     0,     0,     0,     0,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,   754,
    1361,     0,     0,     0,     0,     0,     0,     0,     0,   755,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,   753,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,     0,     0,     0,     0,     0,     0,   757,
     758,   759,   760,   761,   762,   763,   764,   756,     0,     0,
     749,  1362,   755,     0,     0,     0,     0,     0,     0,     0,
     750,     0,     0,     0,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,   755,     0,     0,     0,   751,
     756,     0,     0,     0,     0,     0,     0,     0,     0,   752,
     753,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,     0,     0,     0,     0,
     754,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   756,     0,     0,   748,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   748,
       0,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,     0,  1379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   748,
       0,     0,     0,   755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,   749,     0,     0,     0,
    1380,     0,     0,     0,     0,     0,   750,     0,     0,   749,
       0,   756,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,   749,
       0,     0,     0,  1382,     0,   751,     0,     0,     0,   750,
       0,     0,     0,     0,     0,   752,   753,     0,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,   753,
       0,     0,     0,     0,     0,     0,   754,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   751,   754,
       0,     0,     0,     0,     0,     0,     0,     0,   752,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   757,
     758,   759,   760,   761,   762,   763,   764,     0,     0,   755,
       0,  1383,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   755,     0,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   755,     0,     0,     0,     0,   756,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     756,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
     756,     0,     0,     0,     0,     0,   748,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,     0,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   748,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   748,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,   748,   749,  1385,   757,   758,
     759,   760,   761,   762,   763,   764,   750,     0,     0,     0,
    1390,     0,   748,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,   749,
    1391,     0,     0,     0,     0,   751,     0,     0,     0,   750,
       0,     0,   749,     0,     0,   752,   753,     0,     0,     0,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   749,   754,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,   751,     0,
       0,     0,   749,     0,     0,     0,     0,     0,   752,   753,
       0,   751,   750,     0,     0,     0,     0,     0,     0,     0,
       0,   752,   753,     0,     0,     0,     0,     0,     0,   754,
       0,     0,     0,     0,   751,     0,     0,     0,     0,     0,
       0,     0,   754,     0,   752,   753,     0,     0,     0,   755,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,   753,     0,     0,   754,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   755,     0,     0,     0,     0,   756,     0,     0,
       0,     0,     0,     0,     0,   755,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     756,     0,     0,     0,     0,   755,     0,     0,     0,     0,
       0,     0,     0,   756,     0,     0,     0,     0,     0,     0,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,   756,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,   756,     0,     0,     0,     0,     0,     0,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,     0,  1394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   748,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,     0,
    1396,   757,   758,   759,   760,   761,   762,   763,   764,   748,
       0,     0,     0,  1397,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,     0,     0,     0,     0,  1411,     0,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,   749,
     748,     0,     0,  1413,     0,     0,     0,     0,     0,   750,
       0,     0,     0,     0,     0,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   749,
       0,     0,     0,     0,     0,     0,     0,     0,   751,   750,
       0,     0,     0,     0,     0,     0,     0,     0,   752,   753,
       0,     0,     0,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   753,     0,     0,     0,   754,
     749,     0,     0,     0,     0,     0,     0,     0,   751,     0,
     750,     0,     0,     0,     0,   754,     0,     0,   752,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
     753,     0,   755,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   755,     0,
     754,     0,     0,     0,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
       0,     0,   755,     0,     0,     0,     0,     0,     0,     0,
     756,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   756,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,   755,     0,     0,     0,     0,     0,     0,
     756,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   748,     0,     0,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   748,     0,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,     0,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,     0,
    1417,     0,   749,   748,   757,   758,   759,   760,   761,   762,
     763,   764,   750,     0,     0,     0,  1418,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,     0,
    1419,     0,     0,     0,     0,   749,     0,     0,     0,     0,
     748,   751,     0,     0,     0,   750,     0,     0,     0,     0,
       0,   752,   753,     0,     0,     0,     0,     0,     0,   757,
     758,   759,   760,   761,   762,   763,   764,     0,     0,     0,
       0,  1437,   754,   749,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   753,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   754,     0,     0,     0,     0,
     749,     0,   751,     0,     0,     0,     0,     0,     0,     0,
     750,     0,   752,   753,     0,   755,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   754,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,   755,   752,
     753,     0,     0,   756,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
     754,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,   755,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   756,     0,     0,     0,
       0,     0,   748,     0,     0,     0,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   748,   755,   756,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   748,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,     0,     0,     0,     0,     0,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,     0,
       0,   756,   749,  1467,     0,     0,     0,   748,     0,     0,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   749,     0,   757,   758,   759,   760,   761,   762,
     763,   764,   750,     0,     0,     0,  1468,     0,   748,   749,
       0,   751,     0,     0,     0,     0,     0,     0,     0,   750,
       0,   752,   753,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   757,   758,   759,   760,   761,   762,   763,   764,
       0,   751,   754,     0,  1472,     0,     0,   749,     0,     0,
       0,   752,   753,     0,     0,     0,     0,   750,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,   753,
       0,     0,   754,     0,     0,     0,     0,     0,     0,   757,
     758,   759,   760,   761,   762,   763,   764,     0,   749,   754,
       0,  1475,     0,     0,     0,     0,   751,     0,   750,     0,
       0,     0,     0,     0,     0,   755,   752,   753,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   754,     0,     0,
       0,     0,     0,     0,     0,   755,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,   753,     0,
       0,     0,   755,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,   756,     0,     0,     0,     0,     0,     0,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     756,     0,     0,     0,     0,     0,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   755,     0,     0,     0,     0,     0,     0,   756,     0,
       0,     0,     0,     0,     0,     0,     0,   748,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,   756,
       0,     0,     0,  2126,   748,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,     0,
       0,     0,     0,  2147,     0,     0,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,   748,   749,     0,     0,
    2155,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   757,   758,   759,   760,
     761,   762,   763,   764,   749,     0,     0,     0,  2164,     0,
       0,     0,     0,     0,   750,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,     0,   749,   754,     0,  2176,
       0,     0,     0,   751,     0,     0,   750,     0,     0,     0,
       0,   748,     0,   752,   753,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,   754,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,   753,     0,     0,     0,
     755,     0,     0,     0,     0,     0,     0,     0,  1482,     0,
       0,     0,     0,     0,     0,     0,   754,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   749,     0,     0,     0,     0,   748,   755,     0,     0,
       0,   750,     0,     0,     0,     0,     0,     0,   756,     0,
       0,     0,     0,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   755,
     751,     0,     0,     0,     0,   756,     0,     0,     0,     0,
     752,   753,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   754,     0,     0,     0,     0,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,   756,     0,     0,
       0,   748,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   748,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,     0,     0,
       0,     0,     0,     0,     0,   751,   757,   758,   759,   760,
     761,   762,   763,   764,   755,   752,   753,     0,  2201,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   754,     0,     0,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,   749,     0,     0,     0,  2210,     0,     0,     0,     0,
       0,   750,   756,   748,   749,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   750,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,     0,  2634,     0,   755,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,   753,     0,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,   753,     0,     0,     0,     0,     0,
       0,   754,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   749,   754,     0,     0,   756,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,  2206,     0,
     757,   758,   759,   760,   761,   762,   763,   764,     0,     0,
       0,  1483,   751,     0,   755,     0,     0,     0,     0,     0,
       0,     0,   752,   753,     0,     0,     0,   755,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   754,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,   756,   748,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   756,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,  2207,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   755,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,   749,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,   756,     0,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,     0,     0,     0,     0,     0,     0,     0,
     757,   758,   759,   760,   761,   762,   763,   764,     0,     0,
     748,  1346,   751,   757,   758,   759,   760,   761,   762,   763,
     764,     0,   752,   753,  1347,   749,     0,   748,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,     0,   754,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   748,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   753,     0,     0,     0,     0,
     749,     0,   757,   758,   759,   760,   761,   762,   763,   764,
     750,     0,     0,  1360,     0,   754,   755,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,     0,   751,
       0,     0,     0,     0,     0,     0,     0,   750,     0,   752,
     753,     0,     0,     0,   756,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,   755,     0,
     754,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   751,   754,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   756,   754,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,   755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   757,   758,   759,   760,   761,   762,   763,   764,
     755,   756,     0,  1364,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   748,     0,   756,     0,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,     0,     0,   756,     0,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,     0,     0,     0,  1367,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,   748,     0,
       0,     0,     0,     0,     0,     0,   750,     0,     0,     0,
       0,   748,     0,     0,     0,     0,     0,     0,     0,   757,
     758,   759,   760,   761,   762,   763,   764,     0,     0,     0,
    1370,     0,     0,     0,     0,     0,   757,   758,   759,   760,
     761,   762,   763,   764,   748,   751,     0,  1381,     0,     0,
       0,     0,     0,     0,     0,   752,   753,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   757,   758,   759,   760,
     761,   762,   763,   764,     0,     0,   754,  1384,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,     0,
       0,   749,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   750,     0,     0,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,     0,   749,     0,     0,   751,     0,     0,
       0,     0,     0,     0,   750,     0,     0,   752,   753,   755,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,   753,     0,     0,     0,     0,     0,     0,   754,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   754,     0,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,   753,   748,     0,   756,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,     0,     0,
       0,   755,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   755,     0,     0,     0,     0,     0,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   749,     0,   755,     0,   756,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,   756,   748,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,   751,   756,  1410,     0,     0,   748,
       0,     0,     0,     0,   752,   753,     0,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   754,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,   749,     0,   748,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,     0,     0,     0,  1412,   749,
     757,   758,   759,   760,   761,   762,   763,   764,   755,   750,
     748,  1420,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,   753,     0,     0,     0,   748,     0,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,     0,     0,   754,  1421,   749,     0,     0,   751,     0,
       0,     0,     0,     0,     0,   750,   756,     0,   752,   753,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   754,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     749,     0,     0,     0,   751,     0,     0,     0,     0,     0,
     750,     0,     0,     0,   752,   753,   755,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,   754,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,   755,     0,     0,     0,     0,     0,     0,   752,
     753,     0,     0,     0,   756,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
     754,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,     0,     0,     0,  1422,     0,   754,   755,     0,
     756,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   755,     0,     0,   756,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   757,   758,   759,   760,   761,   762,   763,   764,
       0,   756,     0,  1432,     0,     0,   748,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   756,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,  1433,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,     0,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,     0,     0,     0,  1434,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   748,   757,
     758,   759,   760,   761,   762,   763,   764,     0,     0,     0,
    1466,     0,   748,     0,     0,   751,   757,   758,   759,   760,
     761,   762,   763,   764,     0,   752,   753,  1471,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,     0,     0,     0,   754,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,     0,
       0,     0,   749,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   750,     0,     0,     0,     0,   748,     0,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   748,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,   753,     0,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,   753,     0,     0,     0,     0,   756,   754,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   754,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,   755,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   755,   751,   748,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
       0,     0,     0,     0,   751,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,   753,     0,   754,     0,   756,
       0,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,   756,     0,   754,  1491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,   756,     0,
       0,     0,   748,     0,     0,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,     0,   756,   754,  1494,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,     0,
       0,     0,  2124,     0,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
     755,     0,   749,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   750,     0,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,   748,   757,   758,   759,   760,
     761,   762,   763,   764,     0,     0,     0,  2128,   756,   748,
       0,   751,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,   748,     0,     0,  2129,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   754,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,   748,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,   749,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,   749,     0,     0,     0,     0,   748,     0,     0,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,   748,     0,     0,   751,     0,   757,   758,   759,   760,
     761,   762,   763,   764,   752,   753,   749,  2141,   751,     0,
       0,     0,     0,     0,     0,     0,   750,     0,   752,   753,
       0,   751,     0,   756,     0,   754,     0,     0,     0,     0,
       0,   752,   753,     0,     0,     0,     0,     0,     0,   754,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,     0,     0,   751,     0,   749,     0,     0,
       0,     0,     0,     0,     0,   752,   753,   750,     0,     0,
       0,   749,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   750,     0,     0,     0,     0,   754,     0,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   755,     0,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,   755,   752,   753,     0,     0,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,   753,     0,     0,     0,     0,   756,   754,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,   755,
     756,   754,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   756,     0,     0,     0,     0,     0,     0,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   756,     0,     0,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   755,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,     0,     0,     0,   756,   748,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,   756,     0,     0,  2146,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,  2148,
       0,   757,   758,   759,   760,   761,   762,   763,   764,     0,
       0,     0,  2149,     0,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
     748,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,  2159,     0,     0,   749,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,     0,     0,     0,   757,   758,   759,   760,
     761,   762,   763,   764,     0,   748,     0,  2160,   751,     0,
     757,   758,   759,   760,   761,   762,   763,   764,   752,   753,
     749,  2163,     0,     0,     0,     0,     0,     0,     0,     0,
     750,     0,     0,     0,     0,     0,     0,     0,     0,   754,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,     0,     0,     0,     0,   748,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
     753,     0,     0,     0,     0,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
     754,     0,   755,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   748,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   751,     0,     0,   749,     0,     0,
       0,     0,     0,     0,   752,   753,     0,   750,     0,     0,
     756,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   755,     0,   754,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,     0,
       0,   756,     0,     0,     0,     0,     0,   754,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,   753,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   757,   758,
     759,   760,   761,   762,   763,   764,   756,     0,   754,  2165,
     755,     0,     0,     0,     0,     0,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,     0,     0,     0,   756,   757,
     758,   759,   760,   761,   762,   763,   764,     0,     0,     0,
    2168,   755,     0,     0,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,   748,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,     0,     0,
       0,     0,     0,     0,     0,     0,   748,     0,     0,   756,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,     0,     0,     0,  2169,     0,     0,     0,     0,
       0,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,     0,   748,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   748,   749,     0,   757,   758,   759,   760,
     761,   762,   763,   764,   750,     0,     0,  2170,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     748,     0,     0,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,   753,     0,   749,   757,   758,   759,
     760,   761,   762,   763,   764,   751,   750,     0,  2171,     0,
       0,     0,     0,   749,   754,   752,   753,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   754,     0,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,   753,     0,     0,     0,
     749,     0,   751,     0,     0,     0,     0,     0,     0,     0,
     750,     0,   752,   753,     0,     0,   754,   755,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   754,     0,     0,     0,     0,     0,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
     753,     0,     0,     0,     0,   756,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   755,
     754,     0,     0,     0,     0,     0,     0,   756,     0,     0,
       0,     0,     0,     0,     0,     0,   755,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,   756,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   755,   756,     0,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,     0,     0,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,     0,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,   756,   748,     0,  2175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,  2177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   748,     0,     0,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,  2178,     0,     0,     0,
       0,     0,   757,   758,   759,   760,   761,   762,   763,   764,
       0,     0,   749,  2184,     0,     0,     0,     0,     0,     0,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,   748,   757,
     758,   759,   760,   761,   762,   763,   764,   750,     0,     0,
    2185,   751,     0,   749,     0,     0,     0,     0,     0,     0,
       0,   752,   753,   750,     0,     0,     0,     0,     0,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   754,     0,     0,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
       0,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,   753,     0,     0,     0,   754,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,     0,
       0,     0,     0,   754,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   755,     0,   748,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   752,   753,     0,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   756,     0,     0,   755,     0,   754,     0,
       0,     0,     0,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,   756,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,   756,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,     0,     0,
       0,   755,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   751,     0,     0,     0,
       0,   748,     0,     0,     0,     0,   752,   753,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   754,     0,   756,
       0,   757,   758,   759,   760,   761,   762,   763,   764,     0,
       0,     0,  2186,   748,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   757,   758,   759,   760,
     761,   762,   763,   764,     0,     0,     0,  2187,     0,     0,
       0,   749,   757,   758,   759,   760,   761,   762,   763,   764,
     755,   750,     0,  2188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
       0,     0,     0,   749,     0,     0,     0,     0,     0,     0,
     751,     0,     0,   750,     0,     0,     0,     0,   756,     0,
     752,   753,     0,     0,     0,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,     0,     0,     0,  2189,     0,
       0,   754,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,   753,   748,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   754,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
       0,     0,     0,     0,   755,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,     0,     0,     0,   757,   758,   759,   760,
     761,   762,   763,   764,   749,     0,   755,  2192,     0,     0,
       0,     0,   756,     0,   750,     0,     0,     0,     0,     0,
       0,     0,   748,     0,     0,     0,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   751,   756,     0,     0,     0,   748,     0,
       0,     0,     0,   752,   753,     0,     0,     0,     0,     0,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   754,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,   749,     0,   748,     0,     0,     0,     0,     0,
       0,     0,   750,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     757,   758,   759,   760,   761,   762,   763,   764,   749,     0,
       0,  2202,     0,     0,     0,     0,     0,   755,   750,   748,
       0,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   752,   753,     0,     0,     0,   748,     0,     0,     0,
       0,     0,   757,   758,   759,   760,   761,   762,   763,   764,
       0,     0,   754,  2203,   749,     0,     0,   751,     0,     0,
       0,     0,     0,     0,   750,   756,     0,   752,   753,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   754,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   749,
       0,     0,     0,   751,     0,     0,     0,     0,     0,   750,
       0,     0,     0,   752,   753,   755,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,     0,     0,     0,
       0,     0,     0,     0,   754,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   751,     0,
       0,   755,     0,     0,     0,     0,     0,     0,   752,   753,
       0,     0,     0,   756,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,   753,     0,     0,   754,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,     0,     0,     0,  2205,     0,   754,   755,     0,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   755,     0,     0,   756,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,     0,
     756,     0,  2211,     0,     0,   748,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   756,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,     0,     0,     0,  2213,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,     0,     0,     0,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,     0,     0,     0,  2215,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   748,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,  2217,
       0,   748,     0,     0,   751,   757,   758,   759,   760,   761,
     762,   763,   764,     0,   752,   753,  2219,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,   754,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,   749,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   750,     0,     0,     0,     0,   748,     0,   755,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   748,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
     751,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     752,   753,     0,     0,     0,     0,   756,   754,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   754,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,     0,     0,     0,
       0,     0,     0,     0,   749,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   750,     0,     0,     0,     0,     0,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   755,   751,   748,     0,     0,     0,
       0,     0,     0,     0,     0,   752,   753,     0,     0,     0,
       0,     0,     0,   751,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,   753,     0,   754,     0,   756,     0,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,   756,     0,   754,  2220,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,     0,     0,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   755,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,   753,   756,     0,     0,
       0,     0,     0,     0,     0,     0,   757,   758,   759,   760,
     761,   762,   763,   764,     0,   756,   754,  2221,     0,     0,
     757,   758,   759,   760,   761,   762,   763,   764,     0,     0,
       0,  2465,     0,     0,     0,     0,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,     0,     0,     0,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   747,
       0,     0,     0,     0,   748,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,  2466,   756,   748,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,   748,     0,     0,  2467,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   749,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   750,     0,     0,     0,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,     0,
       0,   749,     0,     0,     0,     0,   748,     0,     0,     0,
       0,   750,     0,     0,     0,     0,     0,     0,     0,     0,
     748,     0,     0,   751,     0,   757,   758,   759,   760,   761,
     762,   763,   764,   752,   753,   749,  2640,   751,     0,     0,
       0,     0,     0,     0,     0,   750,     0,   752,   753,     0,
     751,     0,     0,     0,   754,     0,     0,     0,     0,     0,
     752,   753,     0,     0,     0,     0,     0,     0,   754,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   754,     0,     0,   751,     0,   749,     0,     0,     0,
       0,     0,     0,     0,   752,   753,   750,     0,     0,     0,
     749,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     750,     0,     0,     0,     0,   754,     0,   755,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   755,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,   755,   752,   753,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
     753,     0,     0,     0,     0,   756,   754,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   755,   756,
     754,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   756,     0,     0,     0,     0,     0,     0,     0,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   756,     0,     0,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,     0,     0,     0,   756,   748,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,   756,     0,     0,  2642,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,     0,     0,     0,  2648,     0,
     757,   758,   759,   760,   761,   762,   763,   764,     0,     0,
       0,  2649,     0,     0,     0,     0,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,   748,
       0,     0,     0,     0,   757,   758,   759,   760,   761,   762,
     763,   764,     0,     0,     0,  2650,     0,     0,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   750,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,   748,     0,  2652,   751,     0,   757,
     758,   759,   760,   761,   762,   763,   764,   752,   753,   749,
    2654,     0,     0,     0,     0,     0,     0,     0,     0,   750,
       0,     0,  1668,     0,     0,     0,     0,     0,   754,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   748,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   751,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,   753,
       0,     0,     0,     0,   749,     0,     0,     0,  1670,  1671,
       0,     0,     0,     0,   750,     0,     0,     0,     0,   754,
       0,   755,     0,  -303,  1672,     0,     0,  -303,     0,     0,
       0,     0,     0,     0,     0,     0,  1674,  1275,     0,  1675,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   751,     0,     0,   749,     0,     0,     0,
       0,     0,     0,   752,   753,     0,   750,     0,     0,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1680,
       0,     0,   755,     0,   754,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  2102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,   753,     0,     0,     0,
    1681,     0,     0,     0,     0,  1682,     0,     0,     0,     0,
     756,     0,     0,     0,     0,     0,   754,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   755,     0,     0,
    1684,     0,     0,     0,     0,     0,     0,     0,     0,  1685,
    1686,  1687,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1689,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   757,   758,   759,
     760,   761,   762,   763,   764,   756,     0,     0,  2655,   755,
       0,     0,  1690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1691,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   756,   757,   758,
     759,   760,   761,   762,   763,   764,     0,     0,     0,  2656,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1692,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,     0,  1693,     0,  2904,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1695,     0,     0,     0,
       0,     0,     0,    99,   100,   101,   102,   103,     0,     0,
       0,   104,     0,     0,     0,   757,   758,   759,   760,   761,
     762,   763,   764,     0,   105,     0,  3028,   106,   107,   108,
     109,     0,   110,   111,   112,   113,     0,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,   120,     0,
     121,     0,     0,   122,  2103,   123,     0,     0,   124,     0,
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
       0,     0,     0,   301,     0,     0,     0,     0,     0,   302,
       0,   303,     0,     0,   304,     0,     0,   305,     0,   306,
       0,   307,   308,   309,    99,   100,   101,   102,   103,  1735,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   105,     0,     0,   106,   107,
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
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
     302,     0,   303,     0,     0,   304,     0,     0,   305,     0,
     306,     0,   307,   308,   309,    99,   100,   101,   102,   103,
    2260,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   105,     0,     0,   106,
     107,   108,   109,     0,   110,   111,   112,   113,     0,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
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
       0,     0,     0,     0,     0,   301,     0,     0,     0,     0,
       0,   302,     0,   303,     0,     0,   304,     0,     0,   305,
       0,   306,     0,   307,   308,   309,    99,   100,   101,   102,
     103,  2297,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,     0,
     106,   107,   108,   109,     0,   110,   111,   112,   113,     0,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
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
       0,     0,     0,     0,   247,     0,   248,   249,     0,     0,
       0,     0,   250,     0,   251,   252,     0,   253,     0,     0,
       0,   254,     0,     0,   255,     0,   256,     0,     0,     0,
       0,   257,     0,   258,   259,   260,   261,     0,     0,     0,
     262,     0,   263,   264,     0,     0,   265,   266,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
     269,   270,     0,   271,     0,   272,   273,   274,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,   278,   279,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   280,     0,     0,   281,
       0,     0,     0,   282,   283,     0,   284,     0,     0,     0,
       0,   285,     0,     0,     0,   286,     0,     0,   287,   288,
     289,   290,     0,     0,     0,   291,   292,   293,     0,   294,
     295,     0,   296,     0,   297,     0,     0,   748,     0,     0,
       0,   298,     0,   299,   300,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
       0,     0,   302,     0,   303,     0,     0,   304,     0,     0,
     305,     0,   306,     0,   307,   308,   309,     0,     0,     0,
       0,     0,  2668,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   735,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   746,   747,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,   735,   736,   737,   738,   739,
     740,   741,   742,   743,   744,   745,   746,   747,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   751,     0,     0,     0,
       0,   748,  1345,     0,     0,     0,   752,   753,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,     0,     0,     0,     0,     0,   754,     0,     0,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   735,   736,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,   747,     0,     0,     0,
       0,   749,     0,     0,     0,     0,   748,     0,     0,     0,
     755,   750,     0,     0,     0,     0,     0,     0,     0,     0,
    1861,     0,     0,     0,     0,     0,     0,     0,   748,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,     0,   749,     0,     0,     0,     0,     0,     0,
     751,     0,     0,   750,     0,     0,     0,     0,   756,     0,
     752,   753,   748,  1863,     0,     0,     0,     0,     0,     0,
       0,   766,   767,   768,   769,   770,     0,     0,     0,     0,
       0,   754,     0,     0,     0,  1865,   749,     0,     0,     0,
       0,     0,   751,     0,     0,     0,   750,     0,   771,   772,
     773,   774,   752,   753,     0,     0,     0,   748,   749,     0,
       0,     0,     0,   775,   776,   777,   778,     0,   750,   779,
       0,     0,     0,   754,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,   780,
     781,     0,  2142,     0,   755,   752,   753,     0,     0,     0,
       0,     0,   750,     0,     0,     0,     0,   751,     0,     0,
       0,   782,   783,     0,     0,     0,   754,   752,   753,     0,
       0,     0,     0,     0,     0,     0,   757,   758,   759,   760,
     761,   762,   763,   764,     0,     0,   755,   749,   754,     0,
       0,   751,   756,     0,     0,     0,     0,   750,     0,     0,
       0,   752,   753,     0,     0,     0,     0,     0,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,     0,   754,     0,     0,     0,     0,     0,     0,   755,
       0,     0,     0,     0,   756,     0,   751,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,   753,     0,     0,
       0,   755,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   754,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   756,     0,     0,
       0,  2143,     0,     0,     0,   755,   748,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     757,   758,   759,   760,   761,   762,   763,   764,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     755,     0,     0,   756,     0,     0,   735,   736,   737,   738,
     739,   740,   741,   742,   743,   744,   745,   746,   747,     0,
       0,     0,   757,   758,   759,   760,   761,   762,   763,   764,
       0,     0,     0,     0,     0,     0,   749,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   750,     0,   756,     0,
       0,     0,  1441,   736,  1442,  1443,   739,   740,   741,   742,
     743,  1444,  1445,  1446,  1447,   757,   758,   759,   760,   761,
     762,   763,   764,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   748,   751,     0,   757,   758,   759,
     760,   761,   762,   763,   764,   752,   753,     0,  1518,   736,
    1519,  1520,   739,   740,   741,   742,   743,  1521,  1522,  1523,
    1524,     0,     0,     0,     0,     0,   754,     0,     0,     0,
       0,   757,   758,   759,   760,   761,   762,   763,   764,     0,
    1448,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  2152,     0,     0,     0,
       0,     0,     0,     0,   749,     0,   757,   758,   759,   760,
     761,   762,   763,   764,   750,     0,  1525,     0,     0,   755,
       0,     0,     0,     0,     0,     0,  1518,   736,  1519,  1520,
     739,   740,   741,   742,   743,  1521,  1522,  1523,  1524,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1449,     0,     0,   751,     0,     0,     0,     0,     0,     0,
    1450,     0,     0,   752,   753,     0,     0,   756,  1518,   736,
    1519,  1520,   739,   740,   741,   742,   743,  1521,  1522,  1523,
    1524,     0,     0,     0,   754,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1526,     0,     0,  1451,
       0,     0,     0,     0,  1879,     0,  1527,     0,     0,  1452,
    1453,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1454,     0,  1518,   736,  1519,  1520,   739,   740,   741,   742,
     743,  1521,  1522,  1523,  1524,  1528,  2197,   755,     0,     0,
       0,     0,     0,     0,     0,  1529,  1530,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  2641,  1531,     0,     0,     0,
       0,     0,     0,     0,  1526,   757,   758,   759,   760,   761,
     762,   763,   764,  1455,  1527,   756,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    2234,     0,     0,  1518,   736,  1519,  1520,   739,   740,   741,
     742,   743,  1521,  1522,  1523,  1524,  1526,     0,     0,     0,
       0,     0,     0,  1528,     0,     0,  1527,     0,     0,  1532,
       0,  1456,     0,  1529,  1530,  1518,   736,  1519,  1520,   739,
     740,   741,   742,   743,  1521,  1522,  1523,  1524,     0,     0,
       0,     0,     0,     0,  1531,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1528,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1529,  1530,  1533,     0,     0,
    1526,  2458,     0,     0,     0,     0,     0,     0,     0,     0,
    1527,     0,     0,     0,     0,     0,  1531,     0,     0,     0,
       0,     0,     0,   757,   758,   759,   760,   761,   762,   763,
     764,     0,     0,  2472,     0,     0,     0,  1532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1528,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1529,
    1530,     0,     0,     0,     0,     0,     0,     0,     0,  1457,
    1458,  1459,  1460,  1461,  1462,  1463,  1464,     0,     0,  1532,
    1531,  1526,     0,     0,     0,  1533,     0,     0,     0,     0,
       0,  1527,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1526,     0,  1534,  1535,  1536,  1537,  1538,
    1539,  1540,  1541,  1527,     0,     0,     0,  1533,     0,     0,
    1528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1529,  1530,     0,  1532,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1531,  1528,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1529,  1530,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1533,     0,  1531,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1534,  1535,  1536,  1537,  1538,  1539,  1540,
    1541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1532,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1534,  1535,  1536,  1537,  1538,
    1539,  1540,  1541,     0,     0,     0,  1532,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1533,     0,     0,     0,     0,  1534,
    1535,  1536,  1537,  1538,  1539,  1540,  1541,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1534,  1535,  1536,  1537,  1538,  1539,  1540,  1541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
     100,   101,   102,   103,     0,     0,     0,   104,     0,     0,
       0,     0,  1534,  1535,  1536,  1537,  1538,  1539,  1540,  1541,
     105,     0,     0,   106,   107,   108,   109,     0,   110,   111,
     112,   113,     0,     0,     0,   114,     0,     0,   115,     0,
     116,   117,   118,   119,   120,     0,   121,     0,     0,   122,
       0,   123,     0,     0,   124,     0,     0,   125,   126,   127,
     128,   129,   130,     0,     0,   131,   132,   133,   134,     0,
       0,   135,     0,   136,   137,   138,     0,   139,     0,   140,
     141,     0,     0,     0,     0,   142,   143,   144,   145,  1770,
     146,   147,   148,     0,   149,     0,   150,   151,     0,     0,
     152,   153,     0,   154,     0,   155,     0,     0,   156,     0,
       0,     0,     0,   157,  1992,     0,   158,     0,     0,   159,
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
    2002,   146,   147,   148,     0,   149,     0,   150,   151,     0,
       0,   152,   153,     0,   154,     0,   155,     0,     0,   156,
       0,     0,     0,     0,   157,  2003,     0,   158,     0,     0,
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
     156,     0,     0,   942,     0,   157,     0,     0,   158,     0,
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
     144,   145,  1770,   146,   147,   148,     0,   149,     0,   150,
     151,     0,     0,   152,   153,     0,   154,     0,   155,     0,
       0,   156,     0,     0,     0,     0,   157,     0,     0,   158,
       0,     0,   159,   160,   161,     0,   162,   163,   164,     0,
       0,   165,   166,     0,   167,     0,   168,   169,   170,     0,
       0,     0,     0,   171,     0,   172,     0,   173,     0,     0,
       0,   174,     0,     0,   175,   176,   177,     0,   178,     0,
     179,     0,     0,   180,     0,     0,   181,     0,     0,     0,
       0,     0,   182,   183,   345,     0,     0,   184,     0,   185,
       0,     0,     0,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,     0,   210,
     211,   212,     0,   213,     0,   214,     0,     0,   215,     0,
     216,   217,     0,     0,     0,     0,     0,     0,     0,   218,
     219,   220,     0,     0,   221,   222,   223,   224,   225,   226,
     227,   228,     0,     0,     0,     0,   229,     0,   230,     0,
     231,   232,     0,     0,   233,   234,   235,   236,     0,     0,
     237,   238,   239,   240,   241,     0,     0,     0,   242,   243,
       0,   244,     0,     0,   245,   246,     0,   346,     0,     0,
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
       0,   139,     0,   140,   141,     0,     0,     0,     0,   142,
     143,   144,   145,     0,   146,   147,   148,     0,   149,     0,
     150,   151,     0,     0,   152,   153,     0,   154,     0,   155,
       0,     0,   156,     0,     0,     0,     0,   157,  2339,     0,
     158,     0,     0,   159,   160,   161,     0,   162,   163,   164,
       0,     0,   165,   166,     0,   167,     0,   168,   169,   170,
       0,     0,     0,     0,   171,     0,   172,     0,   173,     0,
       0,     0,   174,     0,     0,   175,   176,   177,     0,   178,
       0,   179,     0,     0,   180,     0,     0,   181,     0,     0,
       0,     0,     0,   182,   183,   345,     0,     0,   184,     0,
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
     170,     0,     0,     0,     0,   171,     0,   172,     0,   173,
       0,     0,     0,   174,     0,     0,   175,   176,   177,     0,
     178,     0,   179,     0,     0,   180,     0,     0,   181,     0,
       0,     0,     0,     0,   182,   183,   345,     0,     0,   184,
       0,   185,     0,     0,     0,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
       0,   210,   211,   212,     0,   213,     0,   214,     0,     0,
     215,     0,   216,   217,     0,     0,     0,     0,     0,     0,
       0,   218,   219,   220,     0,     0,   221,   222,   223,   224,
     225,   226,   227,   228,     0,     0,     0,     0,   229,     0,
     230,     0,   231,   232,     0,     0,   233,   234,   235,   236,
       0,     0,   237,   238,   239,   240,   241,     0,     0,     0,
     242,   243,     0,   244,     0,     0,   245,   246,     0,   346,
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
       0,     0,   158,     0,     0,   159,   160,   161,     0,   162,
     163,   164,     0,     0,   165,   166,     0,   167,     0,   168,
     169,   170,     0,     0,     0,     0,   171,     0,   172,     0,
     173,     0,     0,     0,   174,  1264,     0,   175,   176,   177,
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
     384,     0,     0,   247,     0,   248,   249,     0,     0,     0,
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
    2256,     0,   142,   143,   144,   145,     0,   146,   147,   148,
       0,   149,     0,   150,   151,     0,     0,   152,   153,     0,
     154,     0,   155,     0,     0,   156,     0,     0,     0,     0,
     157,     0,     0,   158,     0,     0,   159,   160,   161,     0,
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
       0,     0,     0,   142,   143,   144,   145,     0,   146,   147,
     148,     0,   149,     0,   150,   151,     0,     0,   152,   153,
       0,   154,     0,   155,     0,     0,   156,     0,     0,     0,
       0,   157,     0,     0,   158,     0,     0,   159,   160,   161,
       0,   162,   163,   164,     0,     0,   165,   166,     0,   167,
       0,   168,   169,   170,     0,     0,     0,   931,   171,     0,
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
       0,     0,   157,  1598,     0,   158,     0,     0,   159,   160,
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
     112,   113,     0,     0,     0,   114,     0,     0,   115,     0,
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
       0,     0,     0,     0,     0,     0,   218,   219,   220,  2291,
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
     156,     0,     0,     0,     0,   157,  1636,     0,   158,     0,
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
       0,     0,   301,    99,   100,   101,  1270,   103,   302,     0,
     303,   104,     0,   304,     0,     0,   305,     0,   306,     0,
     307,   308,   309,     0,   105,     0,     0,   106,   107,   108,
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
       0,     0,   165,   166,     0,   167,     0,   168,  1271,   170,
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
       0,   278,   279,     0,  1665,     0,  1666,     0,     0,     0,
       0,     0,  1667,   280,  1668,     0,   281,     0,     0,     0,
     282,   283,     0,   284,  1669,     0,     0,     0,   285,     0,
       0,     0,   286,     0,     0,   287,   288,   289,   290,     0,
       0,     0,   291,   292,   293,     0,   294,   295,     0,   296,
       0,   297,     0,     0,     0,     0,     0,     0,   298,     0,
     299,   300,     0,     0,     0,  2567,  2568,  2569,  2570,     0,
    1670,  1671,     0,   301,     0,     0,     0,     0,     0,   302,
       0,   303,     0,     0,   304,     0,  1672,   305,     0,   306,
       0,   307,   308,   309,     0,  1673,     0,     0,  1674,  1275,
       0,  1675,     0,     0,     0,  1676,     0,     0,     0,     0,
    1678,  2571,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1680,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2572,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    2573,     0,  1681,     0,  2574,     0,  2575,  1682,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1683,     0,
       0,     0,  1684,     0,     0,     0,     0,     0,     0,     0,
       0,  1685,  1686,  1687,     0,     0,     0,     0,     0,  1688,
       0,  2576,     0,  2577,     0,     0,     0,  1689,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1690,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   823,  1691,     0,     0,     0,
       0,   824,   825,     0,     0,     0,  2578,  2579,  2580,     0,
       0,  2581,  2582,  2583,  2584,  2585,     0,  2586,  2587,  2588,
     826,  2589,  2590,  2591,  2592,  2593,  2594,     0,     0,     0,
       0,  2595,     0,  2596,   827,  2597,  2598,  2599,  2600,  2601,
    2602,     0,     0,  2603,  2604,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,   799,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   829,
       0,     0,     0,     0,   830,     0,   831,     0,     0,     0,
       0,   832,  1692,     0,     0,  2605,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    2606,     0,     0,     0,  1693,     0,     0,     0,     0,   833,
       0,   834,     0,   835,  2607,  2608,  2609,     0,  1694,     0,
       0,  2610,     0,  2611,     0,   836,   837,     0,  1695,   838,
       0,     0,     0,     0,   839,     0,     0,     0,     0,     0,
       0,     0,     0,   840,     0,   841,     0,     0,     0,     0,
       0,     0,     0,  2612,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   842,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   843,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   844,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   845,     0,
       0,     0,     0,     0,   846,   847,     0,     0,   848,   849,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0, -1503,     0,
       0,     0,     0,     0,     0,     0,   850,   851,     0,     0,
       0,   852,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   853, -1147
};

static const short yycheck[] =
{
       2,   407,   408,   395,   396,   397,   915,   522,   809,   608,
      12,  1563,    14,  1159,   369,   850,   851,  1348,  1639,   618,
     851,  1944,    24,  1644,   529,     2,  1282,   592,   627,   594,
     595,   596,   597,   598,  1963,   661,   876,   877,   908,  1188,
    1591,  1282,    44,    45,   450,  1068,  2013,  1191,   759,   760,
    1586,   832,  1731,  1197,   498,  1762,  1763,  1128,  1515,    14,
     506,   567,  2332,   903,  1984,   846,  2502,   986,    12,  1989,
     476,   477,   478,   854,   639,   640,   641,  2034,    10,    10,
      82,    42,     3,  2028,   924,     3,     3,    68,    68,  2034,
      22,  1125,  2037,  2038,    97,    68,  1736,  2042,  2043,   699,
    1665,    68,    68,    10,  1669,    82,    68,   947,  1563,  1661,
    1133,   855,  2026,   103,  1148,    22,   136,     3,   922,   922,
     144,  2044,    18,   167,   136,    39,  2071,   136,     3,  1199,
    2107,    55,  2055,  2056,    59,    31,  2103,    33,  2711,    35,
      36,  2029,  2030,     3,    40,    41,   589,     3,    34,   220,
     750,  1971,   159,   223,   279,   251,   756,    88,   280,  2723,
     214,     9,    39,  1283,    12,   611,   101,  2790,  2326,  1315,
     105,    19,    20,    21,   339,     3,   273,    44,   161,     6,
     214,   296,    12,  1734,     9,   118,   206,    12,   171,   633,
     105,   635,   111,   637,    19,    20,    21,  2356,   279,    99,
     214,    70,     4,   279,    90,    91,   201,    10,   119,  2721,
     103,    10,  2785,   144,   108,   105,   179,   181,   129,   109,
     106,  2676,  1465,   107,   154,   279,   221,  2850,   161,   140,
    2720,     9,   118,   119,    12,   121,  2800,   147,   159,   119,
     294,    19,    20,    21,     9,   279,    33,    12,     9,   120,
     164,    12,  2272,   153,    19,   169,    40,   317,    19,    87,
     294,   141,   280,   144,   254,   279,    86,    41,   159,   179,
     159,  2707,   216,  2763,   228,   161,    39,    56,   670,  2715,
     294,   279,    66,    70,   169,    88,   270,   164,  2911,    88,
     299,    86,   169,   353,   260,   184,   210,  2495,   318,  1542,
    2320,   285,   182,    70,   161,   325,   270,   127,   310,    83,
     228,   228,   139,   184,  1184,   317,   202,   275,   237,   339,
      56,   207,  1162,   715,  1164,   161,    56,   339,   336,  2527,
     339,   144,   127,   210,    70,   337,   302,   303,   136,   219,
     302,   303,   275,   268,   221,   308,   232,  2802,   273,   279,
    2923,  2974,   428,   166,   264,   241,   242,   243,   272,   259,
     295,   254,   246,   306,    94,   245,   488,   492,   216,   101,
     301,   257,   487,   105,  1831,   329,   473,   474,  2536,   314,
    2001,   184,   317,  1934,   309,  1456,   216,   305,   305,  2548,
     217,   216,   488,   260,   749,   272,  2353,    16,   284,   293,
     311,   164,   317,  2360,   569,   910,   169,   488,  2353,  3032,
     296,   329,   329,   282,  1195,  2360,  2980,   488,  1969,   354,
     277,  1972,  1973,   493,   301,   321,    86,   317,   871,   436,
     305,  1212,   486,  1553,  1891,   292,  2948,  2116,   216,  1583,
     296,   244,   244,  2420,   488,   305,   342,   210,   329,  3022,
     339,   216,   486,  2367,   329,   216,  1697,   475,   963,  2279,
    2280,   279,  1533,  2351,   488,   389,  1210,   127,   488,   329,
     350,   915,   486,  1277,  1277,  2115,   488,   368,   306,   488,
     337,   280,   389,   486,    94,  1940,   488,   489,   490,  1329,
     488,   486,  1526,   407,  1517,   155,   382,   260,   488,  1303,
    1303,   299,   238,   486,   307,   486,   486,   509,   177,   272,
    2075,   368,  1138,   486,  1140,   436,   329,   490,   404,   486,
     486,   119,   407,   437,   486,   486,  1126,   529,  1128,   490,
     407,   533,   509,   167,   489,   280,   942,   142,  2103,   483,
     154,   339,   428,   177,   184,   488,   548,  2718,   473,   474,
    1806,   486,   437,   136,   159,   557,   533,   488,   560,   167,
     437,   493,  2113,   295,   566,   107,  2533,  2246,   570,   177,
       3,   275,   574,   487,   488,   493,   493,   156,   247,   581,
     582,    89,   314,   560,   144,   317,   201,    51,  2545,   161,
       3,  1510,  1036,   131,   132,    25,  2516,   454,   228,   171,
    2545,   143,   488,   213,   244,     3,   221,   167,    72,   427,
     428,   488,   272,   247,   122,    10,   103,   513,   109,   515,
     218,   629,   630,   206,  3060,   107,    68,    22,  2898,   477,
     478,   479,   480,   481,   482,   483,   644,   274,   275,   247,
    1710,  1711,  1712,   134,   407,   145,  1090,  1666,  1667,  1719,
    1669,   476,   477,   478,   479,   480,   481,   482,   483,  2626,
     668,   143,  2283,     0,   368,   279,   296,   307,  2839,  2840,
     678,  1382,  1383,  1179,   437,  1694,   224,  1123,  1548,   101,
     688,   689,   690,   105,  1554,  2306,   119,   225,    13,   358,
     698,   270,   234,  2245,    19,   300,    21,   141,  1568,   329,
     478,   479,   480,   481,   482,   483,   119,   317,  2631,   139,
     320,   321,   147,   478,   479,   480,   481,   482,   483,   480,
     481,   482,   483,   148,   358,   488,  1596,  2278,    85,   284,
     217,   341,    39,   233,  2270,   318,   346,  2194,   182,  1254,
     278,   351,   325,   362,   179,  2424,   301,     3,  1459,  1460,
     358,   337,   234,  1258,    10,   297,   339,   223,   214,  1358,
     379,  1631,   275,  1834,  2020,   265,  1281,   254,   460,   113,
    1125,   244,   112,   260,  2231,   219,   328,   250,   203,   204,
     205,   280,   341,   785,   403,   787,   403,   222,   340,   146,
     263,   343,   351,  1148,   796,  1576,   355,   275,  1579,   120,
     144,   245,    73,   143,   161,    76,   808,   467,   468,   469,
     470,   471,   472,  1957,   433,   297,  1839,  1851,  2074,   796,
    2787,   161,  2501,   279,  1895,  1536,  1537,   277,  1974,   264,
     785,   171,    88,  2074,   155,  1705,  1706,  1707,   294,  1709,
     184,   187,   188,  1713,  1714,  1715,  1716,  1717,  1718,  1998,
    1450,   853,   292,   199,   200,   161,  1456,   164,  1892,   320,
     321,  2568,   169,   184,   483,   484,   485,   486,   487,  3082,
     872,   403,  2579,   295,   876,   877,   109,  1900,  1912,  3092,
    2809,  1951,  1952,   486,   345,   346,  2583,   269,   166,  2586,
    2597,  2598,   314,   486,   234,   317,   278,  2594,   853,   486,
     902,   903,   486,   210,  2525,  2612,   350,   279,   910,  2530,
     267,  1511,  2619,   915,   306,  1234,   918,  1236,   920,   486,
     277,  1240,   924,   490,   486,   902,   928,  1527,   490,   931,
     352,   353,   216,  1533,   218,   292,   216,   939,   218,  2572,
     297,  1385,  2507,  2508,  2509,   947,   216,  1391,   218,   157,
     158,   928,  2503,  2504,   931,   230,   231,  2590,   577,   486,
    2593,   963,   939,   524,   525,   272,   527,   486,  2067,  2602,
    2069,   590,  2118,   493,   593,  2045,  2046,   312,   313,  2049,
     599,   600,  2615,   602,   603,   604,   605,    67,  2058,  2059,
    2060,  2061,   486,   612,   613,   614,  2508,  2509,   305,  2564,
     486,   620,   486,   622,   623,   624,   486,    39,   184,   628,
     486,   368,   486,   632,   486,   634,   486,   636,   486,   638,
     487,   488,    39,   144,   487,   488,   645,   646,    39,   648,
     487,   488,   651,   652,   653,   654,   655,   656,   657,   658,
     659,   660,   493,   662,   663,   664,   665,   666,   667,   487,
     488,   487,   488,   672,   673,   486,   675,   676,   677,   486,
     679,   680,   681,   682,   683,   684,   685,   686,   687,   486,
     141,  1941,   691,   135,   693,   694,   695,   696,   697,   487,
     488,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     487,   488,  1597,   353,   354,   486,  1104,   716,   487,   488,
     407,   486,   721,   722,   723,   486,  2739,  1109,  2439,  1111,
     486,   182,   731,  1121,   141,   486,   735,   486,   737,  3042,
     141,  2754,  2673,  2674,   487,   488,   745,   746,   747,   748,
     437,   486,   164,   752,   753,   754,   755,   169,   757,   758,
     759,   760,   761,   762,   763,   764,   765,   164,   219,   487,
     488,   216,   169,   164,  1109,   182,   487,   488,   169,   486,
    1162,   182,  1164,   486,  1790,  1791,   221,   487,   488,  2039,
     486,  1526,  1174,  2196,   245,   486,  1178,   486,   210,   487,
     488,   488,   486,    56,   486,  2020,  1188,   487,   488,   221,
     487,   488,   219,   210,  1196,   487,   488,    70,   219,   210,
     486,  1178,  2072,   486,   221,  1207,   487,   488,   487,   488,
    2233,   301,   239,   240,   487,   488,   159,  1843,   245,  1845,
     486,  1223,  1224,   486,   245,   710,   711,   712,   713,   714,
    1207,   486,  1832,   486,  1834,   486,   141,  2998,  2999,  1241,
     272,  2845,  2846,   486,   486,    26,  1248,  1224,   486,   154,
     486,   124,  1254,  1255,   486,   272,  1258,    38,   486,   486,
      41,   272,   486,    44,  1241,    46,   486,   486,   486,    50,
    2747,  1248,   486,  2750,    55,   486,  2753,   182,  1904,   350,
    1906,  2758,    63,   156,   486,   486,   486,    68,    69,   486,
     486,   486,   486,  1893,   486,  1895,   486,   486,    79,   486,
    1255,   486,    83,  2780,   486,    86,  2783,   486,   181,   214,
    1312,   486,  1314,   218,   219,   486,   486,  1319,   486,  1321,
     486,  1323,   486,   350,   486,   486,  1334,  1329,   486,   350,
     486,   486,   486,   486,   486,  2968,   486,  2970,   486,   486,
     245,   486,   486,   486,  1352,  1353,  1348,   486,  2494,   486,
     486,  1359,   486,   486,   486,   486,   486,   138,   486,   486,
     486,  2913,   235,   486,  1319,   238,   486,   148,  1323,  1968,
     486,  1379,   486,   486,   279,   407,   486,  2247,  2248,   252,
     161,   486,  1390,  3016,   486,  3018,  2256,   486,  1396,   294,
     407,   486,   486,  1348,   486,    86,   407,   270,   487,  1407,
      68,   274,   275,   356,   487,   437,  3027,   279,   459,   487,
    1418,   127,   487,   487,  2013,    22,   321,   493,   177,   486,
     437,   479,    56,   487,   179,   102,   437,   144,  2263,     6,
       7,     8,  2263,    10,   181,   102,    13,    14,    15,    16,
     345,   346,   488,   296,   480,   350,    45,   486,   270,   181,
     177,   285,   389,   270,    56,   236,   488,   260,   274,   329,
    1079,  1080,  1081,   181,   383,   246,   304,   248,    36,   277,
     274,   488,   117,   235,   224,  1990,   221,   488,   488,   221,
     487,   135,   488,   488,   493,   266,   402,   107,  2269,   492,
     488,  1110,   112,   492,   114,  1114,  1851,   102,  2368,  2334,
    2370,  2127,   488,  2334,   306,  1513,   487,   487,  1127,    34,
     480,   487,   487,   487,   487,   487,   487,   298,   487,   486,
     488,   426,   487,   143,   487,   487,   146,   487,   487,   487,
    1149,  2332,  2047,   487,   144,   487,   487,  1892,   488,   487,
     487,   161,  2451,   487,   493,  1547,   487,  1549,   487,  2389,
    1552,   171,  2422,   487,   487,  1174,   487,  1912,   487,   487,
     218,   488,   139,   493,   486,    90,   486,   493,   493,    94,
    1572,   163,  1549,   478,   479,  1552,    56,   488,  2204,   488,
     167,   480,  1584,   279,   109,    82,   488,    71,   389,   274,
    1592,   486,   488,  1595,   119,  1597,   280,   488,   488,   305,
      10,    10,    10,    10,    10,    10,    10,  1584,    10,   488,
      10,    10,    10,    10,   234,    10,    10,  1619,  1595,    10,
     290,   221,   144,   100,   488,   244,   487,  1629,  1630,   405,
     379,   219,   177,   219,   219,  1637,   487,  1664,  1640,  1641,
     487,  1668,   177,  1670,  1671,  1672,   171,   267,  1675,   493,
     486,   493,  1629,  1630,  1681,  1682,   144,  1684,  1685,  1686,
    1687,   487,  1689,  1690,  1691,  1692,  1693,    70,  1695,   493,
     493,   486,   280,   148,   144,   493,   168,   297,   112,   493,
     133,   170,  1637,   280,   170,  1640,  1641,   276,   133,   100,
     488,   216,   276,   218,   493,   301,   221,  1316,    55,  1701,
     488,   486,   328,   328,   487,   487,   459,   487,   487,   234,
     487,   465,  1331,   487,  1333,   487,   487,   487,  1337,   487,
     487,   487,  2321,   228,  1343,  1344,   487,  1729,   488,   487,
    1732,   218,   488,   218,   486,   307,  1355,   262,  1357,   179,
     488,   166,  1361,  1362,   306,   488,   169,   169,   144,   480,
    1369,   229,  1729,  1372,   109,  2825,  2826,  2827,  1377,   224,
     175,  1380,   486,   486,   486,   493,   480,   291,  1387,   279,
    1389,   221,   297,   219,  1393,  1394,  1395,   487,   171,   171,
    1399,   238,  1401,   487,  1403,   487,  1405,  2142,   487,   261,
    1409,   219,  1411,   219,  1413,   487,  1415,   159,  1417,  2669,
    1419,   167,   492,    83,   493,  1424,   187,  1426,   487,  1428,
      68,   407,   488,   169,   144,   169,   488,  1436,  1437,   171,
    1439,   486,  1441,  1442,   486,   171,  1445,  1446,  1447,  1448,
    1449,   487,   487,  1452,  1453,  1454,  1455,    88,  1457,  1458,
    1459,  1460,  1461,  1462,  1463,  1464,  1465,   486,  1467,  1468,
     329,  1470,   128,  1472,  1473,  1474,  1475,  1476,   487,  1478,
     487,  1480,   417,  1482,    47,   487,   166,   488,   174,   487,
    1872,   488,   487,   151,  1493,   488,   441,  1496,  1497,    10,
     229,  1889,   185,   186,   187,   188,   189,   190,   191,  2759,
     193,   194,   195,   196,   197,   198,   199,   200,   169,  1518,
    1519,  2500,   169,  1522,  1523,  1524,  1525,   244,   279,   279,
    1529,  1530,  1531,  1532,   488,  1534,  1535,  1536,  1537,  1538,
    1539,  1540,  1541,  1542,   306,    68,   486,   119,   119,   488,
     486,   275,   486,   486,   486,   486,  1555,   486,  2537,  1941,
    1942,   487,   486,   100,   152,   111,   336,   169,   417,   486,
     279,   279,   141,   279,   487,   487,   487,   486,   488,   486,
     219,    83,   219,   486,   219,   154,   354,   354,   486,   218,
     371,   372,   373,   374,   375,  1977,   171,   119,   171,   219,
     219,  1983,  1984,   219,   219,   219,   486,  1989,  1990,  1991,
     171,   487,   487,   182,   178,    87,  1998,   398,   399,   400,
     401,   224,   301,   301,   486,  2007,   487,  2451,   426,   302,
       3,  2013,   413,   414,   415,   416,   487,    10,   419,  3089,
     487,  2048,   488,  2025,  2026,   214,  2028,  2054,   487,   218,
     219,   487,  2034,   487,   219,  2037,  2038,    83,   439,   440,
    2042,  2043,   100,   487,   100,   131,    39,   100,   188,   221,
     486,   486,   219,   100,   487,   434,   245,   100,   100,    10,
     461,   462,    55,  2065,   488,  2067,   487,  2069,   246,  2071,
    2025,  2026,    84,  2075,  2076,   487,  2985,  2986,   159,  2988,
     219,    87,  2647,   178,   192,    10,  1747,  1720,  2065,  2786,
     279,  1303,  2624,  1277,   332,    88,  1697,  2898,  2833,   488,
    2102,  2103,   486,   486,  2103,   294,  2564,  2706,  3017,   487,
     487,  1701,  2067,   488,  2069,  1665,  2032,  2552,  2078,  2572,
    2562,  2572,  2620,  2865,  2728,  2102,  2771,  2762,  3037,  2555,
    3039,  3040,   321,  2303,  3013,  3004,  2857,  2953,  1307,  1758,
    2900,  2871,  2363,  1588,  2239,  2074,  1218,  1224,  1931,  1917,
    1629,   914,  1617,  2924,   615,  1932,   345,   346,   151,   152,
     545,   350,   821,  1562,  2934,  1784,  1349,  3092,  2812,  2666,
     583,   164,   592,   166,   963,  1937,   169,  1258,  1752,  3075,
    2012,  2706,  2532,  2531,     3,  2500,  3059,   931,  1312,   575,
     522,   544,  2238,  1328,  2909,  2918,  2978,  2992,  2537,  2763,
    1166,  1990,  1634,  2000,  1823,    24,  2311,    26,  2522,  2704,
    1174,  1552,   539,    32,  1833,    34,   904,   210,  1242,  1248,
    2823,  2229,  2295,  1321,    -1,    44,  2828,    -1,   221,    -1,
      -1,    -1,    -1,    -1,    -1,   228,  1855,  2239,    -1,    -1,
      -1,    -1,  1861,    -1,  1863,    -1,  1865,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2256,    -1,    -1,    -1,    -1,    -1,
    1879,    -1,  2239,  1882,    -1,    -1,    -1,   260,    -1,    -1,
      -1,    90,    91,    -1,    -1,  1894,    -1,    -1,    -1,   272,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   478,
     479,    -1,    -1,  2295,    -1,    -1,   115,    -1,    -1,   118,
     119,  2303,   121,   296,    -1,    -1,   125,   126,   301,  2311,
      -1,   130,   305,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   329,    -1,    -1,    -1,
      -1,   160,   161,    -1,    -1,    -1,    -1,    -1,  2303,    -1,
      -1,  2353,    -1,    -1,    -1,    -1,    -1,    -1,  2360,    -1,
      -1,    -1,    -1,    -1,    -1,  2367,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2976,    -1,    -1,
      -1,    -1,    -1,   202,    -1,    -1,    -1,  2389,   207,  2008,
      -1,    -1,    -1,    -1,    -1,    -1,   389,    -1,    -1,    -1,
      -1,  2403,    -1,  2022,  2406,    -1,    -1,    -1,  2410,   228,
    2412,    -1,  2367,   232,   407,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   241,   242,   243,  2869,    -1,  2871,    -1,  2406,
     249,    -1,    -1,    -1,    -1,  2412,    -1,  2439,   257,    -1,
      -1,   434,    -1,    -1,   437,    -1,    -1,    -1,    -1,    -1,
    2842,  2843,  2844,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    3059,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2879,    -1,    -1,  3075,   296,    -1,    -1,
    2482,    -1,    -1,    -1,  2439,    -1,    -1,  2489,    -1,    -1,
      -1,    -1,    -1,    -1,   487,   488,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2126,    -1,    -1,
    2512,    -1,    -1,    -1,  2516,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,  2482,  2147,    -1,
      -1,    -1,    -1,  2152,  2153,  2537,  2155,    -1,    -1,    -1,
      -1,  2985,  2986,  2545,  2988,  2164,    -1,    -1,    -1,    -1,
    2552,    -1,    39,    -1,    -1,    -1,    -1,  2176,    -1,    -1,
    2966,  2180,  2564,   382,  2183,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,  3017,    -1,    -1,    -1,    -1,  2197,    -1,
      -1,    -1,  2201,    -1,    -1,   404,    -1,  2206,    -1,    -1,
    2209,  2210,    -1,  3037,    -1,  3039,  3040,    -1,    -1,   418,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   428,
      -1,    -1,    -1,    -1,    -1,  2234,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2629,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2657,
      -1,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
       3,    -1,     5,    -1,   151,   152,    -1,    -1,    -1,   488,
      -1,    -1,   159,    -1,    -1,    -1,    -1,   164,    -1,   166,
      -1,    -1,   169,    26,    27,    -1,    -1,    30,    -1,    32,
      -1,    -1,    35,  2312,    -1,    38,  2315,    -1,    41,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    -1,    -1,    -1,  2718,    60,    61,    62,
      -1,    64,    65,   210,    67,    68,    69,    70,    -1,  2348,
      73,    -1,    -1,    76,   221,    -1,    -1,    80,    81,    -1,
      83,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,  2373,    -1,    -1,    -1,    -1,   102,
    2762,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   260,    -1,    -1,    -1,    -1,    -1,   122,
     123,    -1,    -1,    -1,    -1,   272,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,  2799,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    -1,  2762,    -1,   296,
      -1,    -1,    34,   300,   301,  2817,    -1,    -1,   305,    -1,
      -1,    -1,  2799,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    26,  2839,  2840,  2458,
      -1,    -1,   329,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,  2854,  2472,    44,    -1,    46,    -1,    -1,    34,
      50,    -1,    -1,    -1,    -1,    55,   247,    -1,    90,    91,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,   106,    -1,  2505,    -1,    -1,    79,
      -1,    -1,    -1,    83,    -1,    -1,   118,   119,  2900,   121,
      -1,    -1,   389,    -1,    -1,    -1,   249,    -1,    -1,    -1,
     253,    -1,   255,    -1,    -1,    90,    91,   260,    34,    -1,
     407,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,  2934,    -1,   109,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,   118,   119,    -1,   121,   434,   138,    -1,
     437,    -1,    -1,    -1,    -1,    -1,    -1,   300,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    90,    91,    -1,   358,    -1,    -1,
     202,    -1,    -1,    -1,    -1,   207,   161,    -1,    -1,   105,
     106,    -1,    -1,   109,   337,   338,  2998,  2999,    -1,  3001,
     487,   488,   118,   119,    -1,   121,    -1,    -1,    -1,    -1,
     232,    -1,  3014,    -1,    -1,  2634,    -1,    -1,    26,   241,
     242,   243,  2641,    -1,    -1,  2644,    -1,   202,    -1,    -1,
      38,  3033,   207,    41,    -1,   257,    44,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,   161,   236,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,   246,   232,   248,    -1,
      68,    69,   284,    -1,    -1,    -1,   241,   242,   243,    -1,
      -1,    79,    -1,  3075,   296,    83,   266,    -1,    86,    -1,
      -1,    -1,   257,    -1,    -1,  2704,   202,    -1,    -1,    -1,
      -1,   207,    -1,   436,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,  2723,    -1,    -1,    -1,   298,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,
      -1,   296,    -1,    -1,    -1,   241,   242,   243,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   257,   317,   486,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
     382,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     296,  2800,   404,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2813,    -1,  2815,    -1,    -1,    -1,
      -1,   317,    -1,    -1,    -1,    -1,   428,   382,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   236,   404,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   246,    -1,
     248,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   428,    -1,    -1,    -1,    -1,   266,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   382,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2906,   404,    -1,
     298,    -1,    -1,  2912,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   428,   488,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    28,    29,    30,    31,    -1,    33,    34,    35,    36,
      37,  2980,    -1,    40,    -1,    -1,    43,    -1,    45,    46,
      47,    48,    49,    50,    51,    -1,    -1,    54,  2997,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    82,    -1,    84,    85,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,   101,   102,    -1,   104,   105,   106,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,   118,   119,    -1,   121,    -1,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
     137,   138,   139,   140,   141,    -1,  3085,    -1,   145,    -1,
     147,    -1,   149,   150,    -1,    -1,   153,   154,    -1,   156,
     157,   158,    -1,   160,    -1,   162,    -1,    -1,   165,    -1,
      -1,   168,    -1,    -1,    -1,   172,   173,   174,   175,    -1,
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
     277,    -1,   279,    -1,   281,   282,   283,   284,   285,   286,
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
      -1,    -1,   489,   490,    -1,   492,   493,   494,     4,     5,
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
     216,   217,   218,   219,   220,   221,   222,   223,    -1,    -1,
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
      65,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    82,    -1,    84,
      85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,   101,   102,    -1,   104,
     105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,   118,   119,    -1,   121,    -1,    -1,   124,
     125,   126,    -1,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,    -1,
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
      -1,   145,    -1,   147,    -1,   149,   150,    -1,    -1,   153,
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
     484,    -1,   486,   487,    -1,   489,   490,    -1,   492,   493,
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
      -1,    -1,   145,    -1,   147,    -1,   149,   150,    -1,    -1,
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
      -1,   484,    -1,   486,   487,    -1,   489,   490,    -1,   492,
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
      -1,    -1,   484,    -1,   486,   487,    -1,   489,   490,    -1,
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
      -1,    -1,    -1,   484,    -1,   486,   487,    -1,   489,   490,
      -1,   492,   493,   494,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    29,
      30,    31,    -1,    33,    34,    35,    36,    37,    -1,    -1,
      40,    -1,    -1,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    -1,    -1,    54,    -1,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    65,    66,    67,    -1,    -1,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    82,    -1,    84,    85,    86,    -1,    -1,    -1,
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
     139,   140,   141,    -1,    -1,   144,   145,    -1,   147,    -1,
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
     138,   139,   140,   141,    -1,    -1,   144,   145,    -1,   147,
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
     478,   479,    -1,    -1,    -1,    -1,   484,    -1,   486,    -1,
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
     137,   138,   139,   140,   141,    -1,    -1,   144,   145,    -1,
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
      -1,    -1,   489,   490,    -1,   492,   493,   494,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    28,    29,    30,    31,    -1,    33,    34,    35,
      36,    37,    -1,    -1,    40,    -1,    -1,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    -1,    -1,    54,    -1,
      56,    57,    58,    59,    -1,    -1,    62,    63,    64,    65,
      66,    67,    -1,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    82,    -1,    84,    85,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
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
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,    -1,
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
      -1,   145,    -1,   147,    -1,   149,   150,    -1,    -1,   153,
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
      -1,    -1,   145,    -1,   147,    -1,   149,   150,    -1,    -1,
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
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,
      -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,
      -1,    43,    -1,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    73,    -1,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,   105,   106,    -1,   108,    -1,   110,    -1,
      -1,   113,   114,    -1,    -1,    -1,   118,    -1,    -1,   121,
      -1,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
      -1,   133,   134,    -1,   136,    -1,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,   146,   147,    -1,   149,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,   160,   161,
     162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,   171,
      -1,    -1,   174,   175,    -1,   177,    -1,   179,    -1,   181,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,   203,   204,   205,   206,   207,   208,   209,    -1,   211,
     212,   213,    -1,   215,    -1,   217,    -1,    -1,   220,    -1,
     222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   231,
     232,   233,    -1,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,    -1,    -1,    -1,    -1,   248,    -1,   250,    -1,
     252,   253,    -1,    -1,   256,   257,   258,   259,    -1,    -1,
     262,   263,   264,   265,   266,   267,    -1,    -1,   270,   271,
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
      -1,    -1,   113,   114,    -1,    -1,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,    -1,   133,   134,    -1,   136,    -1,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,   146,   147,    -1,   149,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,   160,
     161,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,
     171,    -1,    -1,   174,   175,    -1,    -1,    -1,   179,    -1,
     181,    -1,    -1,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,   203,   204,   205,   206,   207,   208,   209,    -1,
     211,   212,   213,    -1,   215,    -1,   217,    -1,    -1,   220,
      -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     231,   232,   233,    -1,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,    -1,    -1,    -1,    -1,   248,    -1,   250,
      -1,   252,   253,    -1,    -1,   256,   257,   258,   259,    -1,
      -1,   262,   263,   264,   265,   266,   267,    -1,    -1,   270,
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
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
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
      -1,    -1,    -1,    -1,   454,    -1,    -1,    -1,    -1,    -1,
     460,    -1,   462,    -1,    -1,   465,    -1,    -1,   468,    -1,
     470,    -1,   472,   473,   474,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,   487,    -1,    -1,
      -1,    -1,    -1,   493,    -1,    -1,    25,    -1,    -1,    28,
      29,    30,    31,    -1,    33,    34,    35,    36,    -1,    -1,
      -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,    48,
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
      -1,   160,    -1,   162,    -1,    -1,   165,   166,    -1,   168,
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
      -1,    -1,    -1,    -1,    -1,    -1,   454,    -1,    -1,    -1,
      -1,    -1,   460,    -1,   462,    -1,    -1,   465,    -1,    -1,
     468,    -1,   470,    -1,   472,   473,   474,     4,     5,     6,
       7,     8,   480,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   493,    -1,    -1,    25,    -1,
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
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,   486,
      -1,    -1,    -1,    -1,    -1,    -1,   493,    -1,    -1,    25,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   454,    -1,
      -1,    -1,    -1,    -1,   460,    -1,   462,    -1,    -1,   465,
      -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,   487,    -1,    -1,    -1,    -1,    -1,   493,    -1,    -1,
      25,    -1,    -1,    28,    29,    30,    31,    -1,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,
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
       4,     5,     6,     7,     8,   460,    -1,   462,    12,    -1,
     465,    -1,    -1,   468,    -1,   470,    -1,   472,   473,   474,
      -1,    25,    -1,    -1,    28,    29,    30,    31,    -1,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    -1,   493,    43,
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
      33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,   493,
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
      -1,   454,     4,     5,     6,     7,     8,   460,    -1,   462,
      12,    -1,   465,    -1,    -1,   468,    -1,   470,    -1,   472,
     473,   474,    -1,    25,    -1,    -1,    28,    29,    30,    31,
      -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,
     493,    43,    -1,    45,    46,    47,    48,    49,    -1,    51,
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
      -1,    -1,    -1,   174,   175,   176,    -1,    -1,   179,    -1,
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
      40,   492,    -1,    43,    -1,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    63,    64,    65,    66,    67,    -1,    -1,
      70,    71,    72,    73,    -1,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,   101,   102,    -1,    -1,   105,   106,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,   118,   119,
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
      -1,   311,   312,   313,   314,    -1,    -1,   317,   318,    -1,
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
      -1,    40,   492,    -1,    43,    -1,    45,    46,    47,    48,
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
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,    -1,    -1,
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
     279,    -1,    -1,   282,    -1,   284,   285,    -1,    -1,    -1,
      -1,   290,    -1,   292,   293,    -1,   295,    -1,    -1,    -1,
     299,    -1,    -1,   302,    -1,   304,    -1,    -1,    -1,    -1,
     309,    -1,   311,   312,   313,   314,    -1,    -1,    -1,   318,
      -1,   320,   321,    -1,    -1,   324,   325,    -1,   327,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,   338,
     339,    -1,   341,    -1,   343,   344,   345,   346,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,    -1,
      -1,    -1,    -1,   382,   383,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,   397,    -1,
      -1,    -1,   401,   402,    -1,   404,    -1,    -1,    -1,    -1,
     409,    -1,    -1,    -1,   413,    -1,    -1,   416,   417,   418,
     419,    -1,    -1,    -1,   423,   424,   425,    -1,   427,   428,
      -1,   430,    -1,   432,    -1,    -1,    87,    -1,    -1,    -1,
     439,    -1,   441,   442,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   454,    -1,    -1,    -1,    -1,
      -1,   460,    -1,   462,    -1,    -1,   465,    -1,    -1,   468,
      -1,   470,    -1,   472,   473,   474,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   492,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,   226,   227,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,   247,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    87,    -1,   310,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   216,    -1,   167,    -1,    -1,    -1,   358,    -1,    87,
      -1,   226,   227,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   247,    -1,    -1,    -1,    -1,   167,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   226,
     227,    -1,   216,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,   177,    -1,    -1,    -1,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   247,    -1,    -1,   216,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,   310,   226,   227,    -1,   177,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,   310,    -1,   247,    -1,   301,   216,    -1,
     491,    -1,    -1,   358,    -1,    -1,   310,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,   310,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   491,    -1,    -1,    -1,
     358,    -1,    87,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
     487,   488,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,    -1,   488,    -1,    87,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,   488,    -1,
      -1,    87,   167,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,   177,    -1,    -1,   487,   488,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,   167,    -1,    87,   487,
     488,   216,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   167,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,   167,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     226,   227,    -1,    -1,    -1,   310,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,   226,   227,   310,
      -1,   177,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    -1,   167,    -1,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,   177,   358,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   310,   358,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,   247,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,   358,
      -1,    -1,   487,   488,   310,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   487,   488,    -1,   310,
      87,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,   487,   488,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   358,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,   487,   488,
     167,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,   226,   227,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,   216,
      -1,   487,   488,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     247,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    87,    -1,   487,   488,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    87,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,   358,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   177,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,   216,   487,   488,   167,    -1,    87,
      -1,    -1,    -1,    -1,   226,   227,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,   310,   167,    -1,
     487,   488,    -1,    -1,    -1,    -1,   216,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,   167,
      -1,    -1,    -1,    -1,    -1,   358,    -1,   216,   310,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,   226,   227,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,   487,   488,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,   488,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,   488,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    87,    -1,    -1,   487,   488,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
     488,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,   358,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,   167,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,   177,   358,
      -1,   487,   488,    87,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    87,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,   247,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    87,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,   216,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,   226,   227,    -1,   167,   488,   476,   477,   478,
     479,   480,   481,   482,   483,   177,    -1,    -1,   487,   488,
      -1,   310,   216,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,   247,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   226,   227,    -1,    -1,    -1,   358,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,   247,   310,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,   310,   167,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   226,
     227,    -1,   247,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,   310,   358,   247,   487,   488,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
     310,   358,    -1,   487,   488,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,   487,   488,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,   358,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,   488,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    87,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,   487,   488,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,   167,    -1,    -1,
     487,   488,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,   167,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   177,    -1,   216,    -1,   488,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,   247,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    87,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,   247,    -1,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,   358,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,   247,    -1,    -1,   358,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,    -1,   488,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,    -1,   488,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,   358,    -1,    -1,    -1,   488,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,    -1,   488,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,   247,    -1,    -1,   216,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,   226,   227,
     167,    -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,   310,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
     247,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    87,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   216,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,   177,
      -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,   167,    -1,    -1,    -1,   488,    -1,   216,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,   226,   227,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   247,
     488,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,   358,    -1,    -1,
     167,   488,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,   310,    -1,    -1,    -1,   216,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,    -1,   488,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,   167,    -1,    -1,    -1,
     488,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   167,
      -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,   167,
      -1,    -1,    -1,   488,    -1,   216,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,   310,
      -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,   358,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    87,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    87,   167,   488,   476,   477,
     478,   479,   480,   481,   482,   483,   177,    -1,    -1,    -1,
     488,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   167,
     488,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,   177,
      -1,    -1,   167,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   216,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,   216,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   247,    -1,   226,   227,    -1,    -1,    -1,   310,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,   358,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,   358,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,    -1,   488,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    87,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,    -1,
     488,   476,   477,   478,   479,   480,   481,   482,   483,    87,
      -1,    -1,    -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,    -1,   488,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,   167,
      87,    -1,    -1,   488,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,   247,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,
     177,    -1,    -1,    -1,    -1,   247,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
     247,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,    -1,
     488,    -1,   167,    87,   476,   477,   478,   479,   480,   481,
     482,   483,   177,    -1,    -1,    -1,   488,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,    -1,
     488,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      87,   216,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
      -1,   488,   247,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,
     167,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   226,   227,    -1,   310,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   247,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,   226,
     227,    -1,    -1,   358,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
     247,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,   310,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,   310,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,   358,   167,   488,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,   177,    -1,    -1,    -1,   488,    -1,    87,   167,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,   216,   247,    -1,   488,    -1,    -1,   167,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,   177,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,   167,   247,
      -1,   488,    -1,    -1,    -1,    -1,   216,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,   310,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   310,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,   310,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,   358,
      -1,    -1,    -1,   488,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,    -1,   488,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    87,   167,    -1,    -1,
     488,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   476,   477,   478,   479,
     480,   481,   482,   483,   167,    -1,    -1,    -1,   488,    -1,
      -1,    -1,    -1,    -1,   177,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,   167,   247,    -1,   488,
      -1,    -1,    -1,   216,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    87,    -1,   226,   227,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    87,   310,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   310,
     216,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   358,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   476,   477,   478,   479,
     480,   481,   482,   483,   310,   226,   227,    -1,   488,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,   167,    -1,    -1,    -1,   488,    -1,    -1,    -1,    -1,
      -1,   177,   358,    87,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,    -1,   488,    -1,   310,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,   247,    -1,    -1,   358,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   389,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,   487,   216,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,   358,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   487,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    87,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   358,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      87,   487,   216,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,   226,   227,   487,   167,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
     167,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
     177,    -1,    -1,   487,    -1,   247,   310,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   226,
     227,    -1,    -1,    -1,   358,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   310,    -1,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   247,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
     310,   358,    -1,   487,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,   358,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,   358,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
     487,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    87,   216,    -1,   487,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,   247,   487,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,   167,    -1,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   226,   227,   310,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    87,    -1,   358,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   247,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,   310,    -1,   358,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,   358,    87,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,   216,   358,   487,    -1,    -1,    87,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   247,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,   167,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,   487,   167,
     476,   477,   478,   479,   480,   481,   482,   483,   310,   177,
      87,   487,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,   247,   487,   167,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   358,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,   226,   227,   310,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,   358,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
     247,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,    -1,   247,   310,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,   358,    -1,   487,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
     487,    -1,    87,    -1,    -1,   216,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,   226,   227,   487,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,   247,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    87,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,   358,   247,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   310,   216,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,   247,    -1,   358,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,   358,    -1,   247,   487,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   358,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,   358,   247,   487,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,   487,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
     310,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    87,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,   358,    87,
      -1,   216,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    87,    -1,    -1,   487,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   247,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,   216,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   226,   227,   167,   487,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   226,   227,
      -1,   216,    -1,   358,    -1,   247,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   247,    -1,    -1,   216,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   177,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,   247,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   310,   226,   227,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,   358,   247,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,   310,
     358,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,   358,    87,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,   358,    -1,    -1,   487,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,   487,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      87,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   487,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    87,    -1,   487,   216,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,   226,   227,
     167,   487,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
     247,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,   177,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,   247,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,   358,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,
     478,   479,   480,   481,   482,   483,   358,    -1,   247,   487,
     310,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
     487,   310,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    87,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,   358,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,   167,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   177,    -1,    -1,   487,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,   167,   476,   477,   478,
     479,   480,   481,   482,   483,   216,   177,    -1,   487,    -1,
      -1,    -1,    -1,   167,   247,   226,   227,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
     167,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,   226,   227,    -1,    -1,   247,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,
     247,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   358,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,   358,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,   358,    87,    -1,   487,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   487,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    87,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   487,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,   167,   487,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    87,   476,
     477,   478,   479,   480,   481,   482,   483,   177,    -1,    -1,
     487,   216,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   177,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   247,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,   247,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   310,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,    -1,    -1,   310,    -1,   247,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,   358,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,   358,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      -1,    -1,   487,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,    -1,   487,    -1,    -1,
      -1,   167,   476,   477,   478,   479,   480,   481,   482,   483,
     310,   177,    -1,   487,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
     216,    -1,    -1,   177,    -1,    -1,    -1,    -1,   358,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,   487,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   247,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   167,    -1,   310,   487,    -1,    -1,
      -1,    -1,   358,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   358,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   247,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,   167,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,   167,    -1,
      -1,   487,    -1,    -1,    -1,    -1,    -1,   310,   177,    87,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,   247,   487,   167,    -1,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   358,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,   226,   227,   310,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,   358,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,   247,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,   487,    -1,   247,   310,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,   358,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
     358,    -1,   487,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,   487,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,   487,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
      -1,    87,    -1,    -1,   216,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,   226,   227,   487,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,   247,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    87,    -1,   310,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,   358,   247,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
     310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,   216,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,   247,    -1,   358,    -1,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,   358,    -1,   247,   487,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   358,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,   358,   247,   487,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,   487,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    87,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   487,   358,    87,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    87,    -1,    -1,   487,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,   216,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,   226,   227,   167,   487,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   226,   227,    -1,
     216,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,   216,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   177,    -1,    -1,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,   247,    -1,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   310,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,   226,   227,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,   358,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,   358,
     247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,   358,    87,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,   358,    -1,    -1,   487,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,    -1,    -1,    -1,   487,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,   487,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    87,
      -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,   481,
     482,   483,    -1,    -1,    -1,   487,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    87,    -1,   487,   216,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,   226,   227,   167,
     487,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    90,    91,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,   247,
      -1,   310,    -1,   105,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,   177,    -1,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,   310,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
     202,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
     358,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,
     232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   241,
     242,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   257,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,   358,    -1,    -1,   487,   310,
      -1,    -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   317,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   476,   477,
     478,   479,   480,   481,   482,   483,    -1,    -1,    -1,   487,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     382,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,   404,    -1,   487,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   428,    -1,    -1,    -1,
      -1,    -1,    -1,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    25,    -1,   487,    28,    29,    30,
      31,    -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,
      -1,    -1,    43,    -1,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    54,   486,    56,    -1,    -1,    59,    -1,
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
      -1,    -1,    -1,   454,    -1,    -1,    -1,    -1,    -1,   460,
      -1,   462,    -1,    -1,   465,    -1,    -1,   468,    -1,   470,
      -1,   472,   473,   474,     4,     5,     6,     7,     8,   480,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    29,
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
      -1,    -1,    -1,    -1,   454,    -1,    -1,    -1,    -1,    -1,
     460,    -1,   462,    -1,    -1,   465,    -1,    -1,   468,    -1,
     470,    -1,   472,   473,   474,     4,     5,     6,     7,     8,
     480,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,
      29,    30,    31,    -1,    33,    34,    35,    36,    -1,    -1,
      -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,    48,
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
      -1,    -1,    -1,    -1,    -1,   454,    -1,    -1,    -1,    -1,
      -1,   460,    -1,   462,    -1,    -1,   465,    -1,    -1,   468,
      -1,   470,    -1,   472,   473,   474,     4,     5,     6,     7,
       8,   480,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      28,    29,    30,    31,    -1,    33,    34,    35,    36,    -1,
      -1,    -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,   375,    -1,    -1,
      -1,    -1,    -1,    -1,   382,   383,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   394,    -1,    -1,   397,
      -1,    -1,    -1,   401,   402,    -1,   404,    -1,    -1,    -1,
      -1,   409,    -1,    -1,    -1,   413,    -1,    -1,   416,   417,
     418,   419,    -1,    -1,    -1,   423,   424,   425,    -1,   427,
     428,    -1,   430,    -1,   432,    -1,    -1,    87,    -1,    -1,
      -1,   439,    -1,   441,   442,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   454,    -1,    -1,    -1,
      -1,    -1,   460,    -1,   462,    -1,    -1,   465,    -1,    -1,
     468,    -1,   470,    -1,   472,   473,   474,    -1,    -1,    -1,
      -1,    -1,   480,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,   226,   227,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,   167,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
     310,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,
     216,    -1,    -1,   177,    -1,    -1,    -1,    -1,   358,    -1,
     226,   227,    87,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   371,   372,   373,   374,   375,    -1,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,   144,   167,    -1,    -1,    -1,
      -1,    -1,   216,    -1,    -1,    -1,   177,    -1,   398,   399,
     400,   401,   226,   227,    -1,    -1,    -1,    87,   167,    -1,
      -1,    -1,    -1,   413,   414,   415,   416,    -1,   177,   419,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,   439,
     440,    -1,   167,    -1,   310,   226,   227,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,   216,    -1,    -1,
      -1,   461,   462,    -1,    -1,    -1,   247,   226,   227,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,    -1,    -1,   310,   167,   247,    -1,
      -1,   216,   358,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,   310,
      -1,    -1,    -1,    -1,   358,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,
      -1,   306,    -1,    -1,    -1,   310,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    -1,    -1,   358,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   358,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,   216,    -1,   476,   477,   478,
     479,   480,   481,   482,   483,   226,   227,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,   476,   477,   478,   479,   480,   481,   482,   483,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   466,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,    -1,   476,   477,   478,   479,
     480,   481,   482,   483,   177,    -1,    87,    -1,    -1,   310,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,   226,   227,    -1,    -1,   358,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    87,    -1,   177,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     247,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   216,    87,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   466,   247,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   167,   476,   477,   478,   479,   480,
     481,   482,   483,   310,   177,   358,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   167,    -1,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,   177,    -1,    -1,   310,
      -1,   358,    -1,   226,   227,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   358,    -1,    -1,
     167,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    87,    -1,    -1,    -1,   310,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,   310,
     247,   167,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,   177,    -1,    -1,    -1,   358,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,   310,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   247,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,    -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,   477,   478,   479,   480,   481,   482,
     483,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   476,   477,   478,   479,   480,
     481,   482,   483,    -1,    -1,    -1,   310,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,   476,
     477,   478,   479,   480,   481,   482,   483,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     476,   477,   478,   479,   480,   481,   482,   483,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   476,   477,   478,   479,   480,   481,   482,   483,
      25,    -1,    -1,    28,    29,    30,    31,    -1,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    70,    71,    72,    73,    -1,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,    84,
      85,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    -1,    99,    -1,   101,   102,    -1,    -1,
     105,   106,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,   118,   119,    -1,   121,    -1,    -1,   124,
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
      94,    95,    96,    97,    -1,    99,    -1,   101,   102,    -1,
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
     113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,
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
      92,    93,    94,    95,    96,    97,    -1,    99,    -1,   101,
     102,    -1,    -1,   105,   106,    -1,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,
      -1,    -1,   124,   125,   126,    -1,   128,   129,   130,    -1,
      -1,   133,   134,    -1,   136,    -1,   138,   139,   140,    -1,
      -1,    -1,    -1,   145,    -1,   147,    -1,   149,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,   160,    -1,
     162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,
      -1,    -1,   174,   175,   176,    -1,    -1,   179,    -1,   181,
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
      -1,    82,    -1,    84,    85,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
     101,   102,    -1,    -1,   105,   106,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,   118,   119,    -1,
     121,    -1,    -1,   124,   125,   126,    -1,   128,   129,   130,
      -1,    -1,   133,   134,    -1,   136,    -1,   138,   139,   140,
      -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,   160,
      -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,    -1,    -1,   179,    -1,
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
     140,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,   149,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
     160,    -1,   162,    -1,    -1,   165,    -1,    -1,   168,    -1,
      -1,    -1,    -1,    -1,   174,   175,   176,    -1,    -1,   179,
      -1,   181,    -1,    -1,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,   203,   204,   205,   206,   207,   208,   209,
      -1,   211,   212,   213,    -1,   215,    -1,   217,    -1,    -1,
     220,    -1,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   231,   232,   233,    -1,    -1,   236,   237,   238,   239,
     240,   241,   242,   243,    -1,    -1,    -1,    -1,   248,    -1,
     250,    -1,   252,   253,    -1,    -1,   256,   257,   258,   259,
      -1,    -1,   262,   263,   264,   265,   266,    -1,    -1,    -1,
     270,   271,    -1,   273,    -1,    -1,   276,   277,    -1,   279,
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
      -1,    -1,   121,    -1,    -1,   124,   125,   126,    -1,   128,
     129,   130,    -1,    -1,   133,   134,    -1,   136,    -1,   138,
     139,   140,    -1,    -1,    -1,    -1,   145,    -1,   147,    -1,
     149,    -1,    -1,    -1,   153,   154,    -1,   156,   157,   158,
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
     279,    -1,    -1,   282,    -1,   284,   285,    -1,    -1,    -1,
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
      88,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,    -1,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,   121,    -1,    -1,   124,   125,   126,    -1,
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
      -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,   101,   102,    -1,    -1,   105,   106,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,   121,    -1,    -1,   124,   125,   126,
      -1,   128,   129,   130,    -1,    -1,   133,   134,    -1,   136,
      -1,   138,   139,   140,    -1,    -1,    -1,   144,   145,    -1,
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
      -1,    -1,   118,   119,    -1,   121,    -1,    -1,   124,   125,
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
      35,    36,    -1,    -1,    -1,    40,    -1,    -1,    43,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   231,   232,   233,   234,
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
     113,    -1,    -1,    -1,    -1,   118,   119,    -1,   121,    -1,
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
      -1,    -1,    -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   375,    -1,    -1,    -1,    -1,    -1,
      -1,   382,   383,    -1,    24,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    32,   394,    34,    -1,   397,    -1,    -1,    -1,
     401,   402,    -1,   404,    44,    -1,    -1,    -1,   409,    -1,
      -1,    -1,   413,    -1,    -1,   416,   417,   418,   419,    -1,
      -1,    -1,   423,   424,   425,    -1,   427,   428,    -1,   430,
      -1,   432,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,
     441,   442,    -1,    -1,    -1,    94,    95,    96,    97,    -1,
      90,    91,    -1,   454,    -1,    -1,    -1,    -1,    -1,   460,
      -1,   462,    -1,    -1,   465,    -1,   106,   468,    -1,   470,
      -1,   472,   473,   474,    -1,   115,    -1,    -1,   118,   119,
      -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
     130,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,    -1,   202,    -1,   213,    -1,   215,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   228,    -1,
      -1,    -1,   232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   241,   242,   243,    -1,    -1,    -1,    -1,    -1,   249,
      -1,   260,    -1,   262,    -1,    -1,    -1,   257,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,   296,    -1,    -1,    -1,
      -1,    37,    38,    -1,    -1,    -1,   315,   316,   317,    -1,
      -1,   320,   321,   322,   323,   324,    -1,   326,   327,   328,
      56,   330,   331,   332,   333,   334,   335,    -1,    -1,    -1,
      -1,   340,    -1,   342,    70,   344,   345,   346,   347,   348,
     349,    -1,    -1,   352,   353,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,    -1,
      -1,   117,   382,    -1,    -1,   394,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     409,    -1,    -1,    -1,   404,    -1,    -1,    -1,    -1,   145,
      -1,   147,    -1,   149,   423,   424,   425,    -1,   418,    -1,
      -1,   430,    -1,   432,    -1,   161,   162,    -1,   428,   165,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   462,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   264,    -1,
      -1,    -1,    -1,    -1,   270,   271,    -1,    -1,   274,   275,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   304,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   312,   313,    -1,    -1,
      -1,   317,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   382,   383
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
     667,   675,   676,   782,   784,   791,   794,   821,   829,   840,
     842,   856,   859,   862,   868,   872,   876,   877,   878,   910,
     922,   928,   929,   936,   939,   962,   965,   966,   967,     4,
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
     308,   658,   963,    56,    85,   146,   267,   277,   297,   368,
     531,   532,   533,   546,   603,   830,   783,   161,   277,   292,
     368,   454,   790,   906,   658,   176,   279,   904,   906,   907,
     792,    19,    29,    37,    50,    57,    58,    74,    75,    77,
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
     894,   895,   899,   905,   906,   976,   982,   118,   161,   275,
     274,   275,   924,   658,   873,   658,   869,   280,   906,   678,
     680,   681,   843,    73,    76,    70,   282,    70,   275,   841,
     924,   822,   924,   161,   924,   924,   940,   924,   924,   937,
     224,   912,   906,   906,   795,    68,     0,     3,   337,   136,
     860,   901,   906,   906,   223,     4,   244,   493,   614,   906,
     275,   924,   460,   964,   280,   403,   534,   275,   532,   292,
     161,   159,   184,   339,   838,   839,   717,   718,   906,   903,
     403,   789,   786,   924,   503,   863,   120,   155,   184,   798,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   696,   486,   486,   486,   486,   486,   695,   486,   486,
     978,   982,   486,   486,   486,   486,   720,   721,   722,   696,
     486,   486,   486,   486,   486,   154,   279,   486,   486,   486,
     486,   294,   486,   890,   486,   890,   486,   890,   486,   486,
     486,   486,   696,   724,   486,   486,   486,   695,   486,   695,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   696,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   695,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   695,   695,
     695,   486,   486,   486,   486,   696,   696,   696,    68,   696,
     696,   906,   492,   907,   906,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    87,   167,
     177,   216,   226,   227,   247,   310,   358,   476,   477,   478,
     479,   480,   481,   482,   483,   697,   371,   372,   373,   374,
     375,   398,   399,   400,   401,   413,   414,   415,   416,   419,
     439,   440,   461,   462,   746,   109,   905,   493,   678,   977,
     113,   144,   184,   881,   166,   901,   923,   924,    56,    94,
     848,   874,   924,    56,    70,   238,   870,   871,    67,   679,
      86,   127,   155,   272,   467,   468,   469,   470,   471,   472,
     685,   686,   687,    31,    37,    38,    56,    70,    93,   105,
     110,   112,   117,   145,   147,   149,   161,   162,   165,   170,
     179,   181,   212,   223,   235,   264,   270,   271,   274,   275,
     312,   313,   317,   382,   579,   602,   844,   848,   849,   852,
     915,   631,   632,   631,   631,   306,   630,   631,   788,   901,
     184,   828,   640,   664,   665,   901,   642,   652,    26,    38,
      41,    44,    46,    50,    55,    63,    68,    69,    79,    83,
      86,   138,   148,   161,   236,   246,   248,   266,   298,   941,
     943,   944,   660,   638,    86,   938,   941,   911,   144,   120,
     799,   828,   680,   906,   858,   857,    10,    88,   748,   749,
     930,   906,   613,   901,   650,   509,   216,   906,   534,   903,
     906,   144,   831,   835,   836,   906,   838,   696,   719,   221,
     135,   906,   116,   907,   908,   953,   954,   789,   301,   504,
      56,    70,   124,   156,   181,   235,   238,   252,   270,   274,
     864,   865,   924,   622,   696,    86,   714,   750,   127,   714,
     750,   696,   714,   714,   714,   714,   714,   696,   696,   696,
     696,   696,   696,   487,   899,   678,   979,   696,   696,   696,
     721,   356,   723,   899,   696,   487,   696,   696,   696,   899,
     696,   717,   717,   696,   696,   696,   696,   714,   714,   714,
     459,   717,   696,   696,   696,   487,   696,   696,   696,   696,
     696,   696,   696,   696,   696,   696,   746,   696,   696,   696,
     696,   696,   696,   717,   127,   711,   890,   320,   321,   345,
     346,   747,   696,   696,   487,   696,   696,   696,   717,   696,
     696,   696,   696,   696,   696,   696,   696,   696,   717,   717,
     717,   696,   696,   696,   696,   696,   696,   717,   700,   703,
     487,   696,   696,   696,   696,   696,   696,   696,   696,    98,
     172,   281,   696,   706,   717,   706,   706,   706,   706,   706,
     890,   487,   696,   487,   696,   696,   696,   141,   182,   219,
     245,   350,   769,   487,   488,   696,   147,   179,   264,   916,
      22,   493,   696,   696,   177,   216,   218,   696,   696,   696,
     696,   486,   980,   982,   703,   167,   177,   247,   358,   696,
     696,   696,   696,   701,   703,   696,   696,   696,   702,   696,
     702,   696,   696,   696,   696,    28,    86,   696,   698,   479,
     907,   906,   487,   983,    56,   179,   880,   102,   144,   901,
     925,   926,   657,   181,   645,   102,   488,   906,   296,   968,
     969,   480,   689,   690,   691,   687,    45,   486,   275,   368,
     270,   156,   270,   181,   177,   851,   285,   851,   389,   270,
      56,   274,   851,   144,   167,   850,   853,   270,   285,   329,
     764,   766,   764,   260,   541,   907,   851,   853,   181,   383,
     111,   237,   304,   628,    51,    72,   633,   634,   627,    36,
     622,   787,   788,   167,   488,   671,   672,   787,   787,   277,
     946,   274,    33,    70,   945,   117,   947,   235,   942,   224,
     948,   221,   488,   661,   662,   901,   787,   942,   221,   147,
     179,   222,   264,   913,   914,   492,   501,   904,   166,   788,
     800,   801,   487,   675,   154,   861,   891,   905,   906,   906,
       7,   139,   906,   931,   932,   119,   528,   529,   530,   539,
     540,   587,   612,   787,   185,   186,   187,   188,   189,   190,
     191,   193,   194,   195,   196,   197,   198,   199,   200,   510,
     511,   512,   135,   516,   901,   517,   284,   301,   604,   835,
     901,   906,   488,   833,   493,   837,   488,   901,   695,   492,
     402,   488,   787,   492,   505,   506,   102,   306,   488,   866,
     800,   488,   487,   715,   708,   487,   480,   488,   487,   487,
     487,   487,   487,   488,   488,    88,   487,   487,   301,   488,
     487,   983,   488,   488,   487,   488,   487,   486,   488,   488,
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
     981,   134,   755,   486,   980,   703,   696,   701,     9,    11,
      12,    18,    19,    20,    21,    87,   167,   177,   216,   226,
     227,   247,   310,   358,   476,   477,   478,   479,   480,   481,
     482,   483,   697,   746,   746,   980,   696,   493,   163,   668,
      56,   749,   488,   787,   280,   475,   875,   787,   871,    86,
     127,   975,   488,   682,   693,   696,   897,   906,   167,   854,
     480,   901,   534,   904,   908,   389,   850,   851,   906,   850,
     767,   769,   845,   901,   853,   851,    82,   635,   488,   635,
      71,   486,   490,   729,   734,   735,   280,   488,   119,   666,
     906,   665,   161,   171,   600,   601,   673,   136,   339,   643,
     136,   206,   318,   325,   339,   653,   654,   655,   274,   486,
     955,   955,   955,   955,   480,   901,   906,   952,   944,   488,
     280,   144,   488,   952,   488,   105,   119,   208,   282,   428,
     492,   579,   906,   917,   918,   907,   801,   796,   677,     6,
       7,     8,    10,    13,    14,    15,    16,   139,   933,   935,
     305,   751,   530,   109,   579,    24,    26,    32,    34,    44,
      90,    91,   106,   115,   118,   121,   125,   126,   130,   160,
     161,   202,   207,   228,   232,   241,   242,   243,   249,   257,
     284,   296,   382,   404,   418,   428,   536,   538,   539,   540,
     615,   616,   617,   760,   653,    10,    10,    10,    10,    10,
      10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
     488,   528,   514,   290,    99,   153,   259,   605,   605,   221,
     834,   832,   493,   836,   144,   480,   906,   749,   696,   785,
     907,   100,   954,   103,   254,   623,   907,   488,   244,   865,
     787,   867,   793,   696,   696,   717,   487,   696,   405,   696,
     696,    94,   213,   317,   320,   321,   341,   346,   351,   716,
      94,   580,   907,   716,   717,   717,   696,   696,   899,   717,
     696,   696,   696,   696,   379,   725,   696,   728,   717,   696,
     702,   702,   905,   696,   696,   717,   905,   696,   696,   696,
     717,   219,   696,   696,   696,   696,   717,   219,   696,   696,
     696,   696,   696,   717,   696,   696,   696,   696,   696,   696,
     696,   696,   696,   177,   216,   218,   696,   696,   696,   696,
     696,   703,   177,   247,   358,   696,   696,   696,   696,   701,
     696,   696,   696,   702,   696,   702,   696,   696,   696,   696,
     696,   698,   696,   696,   696,   405,   696,   696,   696,   696,
     696,   144,   696,   144,   696,   144,   696,   219,   696,   696,
     696,   487,   493,   609,   487,   983,   703,   717,   755,    87,
     696,   696,   177,   216,   218,   696,   696,   696,   696,   486,
     980,   703,   167,   177,   247,   358,   696,   696,   696,   696,
     701,   696,   696,   696,   702,   696,   702,   696,   696,   696,
     696,   696,   698,   906,   904,   669,   670,   901,   184,   244,
     307,   927,   926,   653,   904,   696,   136,   299,   339,   646,
     647,   648,   970,   690,   144,   166,   683,   684,   765,   766,
     780,   692,   493,   904,   144,   855,   487,   906,    70,   851,
     851,   220,   488,   181,   270,   847,   850,   901,   512,   636,
     634,    68,   729,   906,    39,   164,   169,   210,   260,   272,
     407,   437,   488,   733,   901,   904,   788,   486,   654,   906,
     956,   957,   493,   493,   280,   662,   901,   904,   148,   144,
     914,   493,   119,   580,   581,   168,    10,    22,   389,   812,
     492,   907,    94,   119,   582,   583,   907,   493,   812,    68,
     260,   302,   303,   486,   524,   802,   806,   969,   971,   972,
     973,   934,   752,   765,   812,   813,   813,   112,   114,   146,
     267,   551,   554,   555,   598,   600,   621,   621,   621,   813,
     143,   234,   600,   621,   813,   813,   813,   280,   133,   813,
     170,   276,   170,   276,   133,   813,   813,   100,   813,   813,
     813,   813,    10,    88,   280,   625,   813,   813,   813,   813,
     813,   621,   813,   536,   488,   486,   493,   550,   556,   900,
     906,   904,   904,   904,   141,   182,   219,   245,   350,   770,
     904,   769,   769,   769,   904,   904,   904,   904,   904,   904,
     769,   511,   177,   486,   518,   535,   537,   538,   328,   340,
     343,   547,   901,   301,   751,   906,   729,   493,   837,   279,
     428,   506,    55,   802,   487,   709,   488,   696,   487,   487,
     486,   573,   573,   573,   328,   328,   487,   487,   487,   487,
     487,   487,   167,   306,   705,   487,   487,   488,   487,   487,
     696,   465,   466,   459,   487,   488,   746,   746,   487,   487,
     487,   487,   487,   487,   488,   487,   487,   487,   487,   487,
     487,   487,   713,   760,   488,   487,   488,   487,   487,   487,
     488,   487,   487,   488,   487,   487,   487,   487,   487,   487,
     696,   218,   487,   755,   703,   696,   701,    87,   746,   746,
     980,   488,   487,   487,   696,   487,   389,   487,   487,   488,
     488,   487,   696,   487,   696,   487,   696,   487,   488,   487,
     487,   487,   906,   487,   696,   696,   218,   717,   755,   486,
     980,   703,   696,   701,    87,   746,   746,   980,   879,   488,
     671,   307,   179,   647,   676,    42,   729,   230,   231,   776,
     777,    55,   389,   688,   780,   684,    88,   694,   904,   906,
     480,   906,   770,   846,   312,   313,   306,   769,   769,   850,
     488,   736,   737,   487,   486,   734,   169,   169,   169,   407,
     437,   824,   825,   899,   729,   229,   739,   739,   729,   729,
     749,   234,   744,   906,   487,   488,   480,   480,   953,   224,
     953,   917,   906,   109,   585,   175,   908,   279,   427,   428,
     920,   916,   812,   906,    86,    94,   119,   221,   696,   921,
     525,   803,   486,   809,   814,   809,   487,   524,   805,   896,
     897,   899,   807,   797,   760,   766,   974,   486,   696,   119,
     584,   586,   907,   581,   608,   900,   601,   601,   107,   143,
     234,   297,   553,   599,   552,   553,   608,   900,   900,   904,
     171,   171,   900,   900,   770,   769,   769,   579,   813,   769,
     813,   770,   770,   761,   119,   769,   239,   240,   543,   769,
     769,   769,   901,   119,   129,   140,   311,   542,   541,   486,
     541,     6,   139,   217,   544,   900,   904,   617,   548,   549,
     550,   551,   906,    25,   139,   624,   246,   552,   590,   557,
     493,   901,   177,   519,   524,   548,   159,   436,   521,   882,
     488,   537,   291,   515,   729,   759,   760,   751,   749,   279,
     221,   819,   487,   696,   746,   219,   101,   295,   314,   579,
     588,    97,   238,   487,   696,   696,   696,   696,   487,   487,
     696,   261,   712,   219,   696,   696,   696,   755,    87,   696,
     696,   746,   696,   696,   696,   487,   487,   487,   219,   487,
     717,   755,    87,   696,   882,   670,   159,   674,   765,   751,
     904,   904,   492,   778,   779,   167,    83,   904,   906,   493,
     764,   187,   851,   512,   487,   685,   407,   729,   739,   739,
     488,   823,   812,   169,   169,   221,   301,   142,   159,   300,
     740,   487,   488,   957,   251,   958,   144,   586,   244,   250,
     263,   919,   812,   486,   486,   918,   696,   685,   810,   811,
     899,   815,   488,   486,   806,   487,   487,   488,   968,   764,
     119,   696,   817,   818,   696,   171,   171,   601,   608,   604,
      44,   260,   618,   608,   623,   581,   904,   904,   696,   762,
     898,   787,   619,   487,   488,   906,   901,    94,    95,    96,
      97,   140,   183,   209,   213,   215,   260,   262,   315,   316,
     317,   320,   321,   322,   323,   324,   326,   327,   328,   330,
     331,   332,   333,   334,   335,   340,   342,   344,   345,   346,
     347,   348,   349,   352,   353,   394,   409,   423,   424,   425,
     430,   432,   462,   558,   561,   562,   563,   564,   565,   566,
     567,   906,   901,   487,   487,    88,   527,   537,   904,   486,
     751,   329,   768,   128,   488,   487,   487,   580,   417,    47,
     487,   466,   487,   487,   488,   891,   487,   488,   487,   487,
     487,   696,   487,   487,   487,   487,   487,   488,   487,   696,
     166,   174,   151,   756,   781,   907,   488,   441,   480,    10,
     749,   689,   229,   169,   169,   825,   751,   818,   729,   729,
     696,   730,   600,   741,   741,   741,   906,    59,   268,   273,
     309,   473,   474,   950,   951,   306,   959,   953,   108,   293,
     244,   920,   279,   279,   812,   689,   488,   812,   816,   817,
     814,   808,   806,   896,   487,   488,   608,   604,   486,   119,
     119,   556,   585,   488,    89,   122,   763,   487,   558,   549,
     591,   486,   573,   486,   568,   569,   317,   352,   562,   564,
     588,   317,   353,   353,   354,   560,   573,   568,   336,   559,
     568,   588,   588,   568,   573,   573,   588,   486,   573,    34,
      90,    94,   109,   119,   216,   218,   221,   234,   262,   297,
     575,   576,   577,   589,   354,   486,   588,   486,   486,   486,
     573,   486,   569,   574,   487,   520,   535,   486,   524,   606,
     607,   906,   770,   820,   696,   696,   696,   714,   717,   275,
     100,   152,   753,   111,   883,   779,   417,   904,   738,   169,
     729,   729,   759,   221,   731,   221,   732,   486,   742,   279,
     279,   279,    87,   949,   148,   203,   204,   205,   960,   961,
     487,   487,   921,   526,   811,   818,   487,   971,   818,   486,
     486,   606,   321,   345,   346,   478,   479,   893,   894,   624,
     898,   575,   201,   221,   486,   592,   594,   595,   596,   219,
     219,   341,   351,   355,   570,   571,   572,   588,   354,   486,
     570,   486,   570,   570,   588,   219,   570,   904,   584,   426,
     578,   893,   218,    83,   119,   171,   577,   171,   219,   219,
     219,   219,   570,   219,   570,   971,   521,   524,   522,   487,
     488,   763,   486,   171,   487,   487,   488,   487,   487,   901,
     757,   898,   754,   759,   131,   132,   225,   278,   884,   885,
     178,   886,    87,   545,   684,   765,   734,   768,   696,   301,
     696,   301,   745,   906,   486,   950,   224,   769,   769,   769,
     961,   545,   606,   606,   487,   895,   895,   763,   620,   145,
     233,   265,    41,    83,   593,   906,   596,   487,   487,   488,
     572,   610,   891,   610,   487,   695,   426,   302,   487,   487,
     487,   487,   487,   968,   607,   219,    83,   696,   883,   306,
     488,   758,   763,   696,   765,   100,   100,   131,   100,   885,
     269,   278,   887,   888,   188,   971,   688,   221,   486,   486,
     487,   488,   743,   744,   487,   487,   624,   103,   217,   254,
     260,   597,   597,   487,   488,   219,   487,   488,   487,   695,
     588,   588,   523,   763,   487,   826,   827,   899,   487,   886,
      40,    66,   898,   434,   771,   891,   891,   100,   891,   100,
     100,   888,    10,   696,   745,   745,   906,   487,   590,    84,
     119,   218,   594,   906,   487,   588,   891,   588,   971,   488,
     812,   159,   889,   763,   906,   891,   891,   891,   770,   491,
     487,   487,   827,   818,   219,   486,   804,   772,    87,   178,
     487,   805,   486,   192,   487,   691,   773,   774,   775,    10,
     696,   487,   488,   769,   775
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
	    lex->lock_option= (using_update_log) ? TL_READ_NO_INSERT : TL_READ;
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

  case 490:
#line 2393 "sql_yacc.yy"
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

  case 492:
#line 2415 "sql_yacc.yy"
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

  case 494:
#line 2434 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  SELECT_LEX *sel= lex->current_select;
	  lex->lock_option= TL_READ;
	  if (sel->linkage != UNION_TYPE)
	    mysql_init_select(lex);
	  lex->current_select->parsing_place= SELECT_LIST;
	}
    break;

  case 495:
#line 2443 "sql_yacc.yy"
    {
	  Select->parsing_place= NO_MATTER;
	}
    break;

  case 497:
#line 2449 "sql_yacc.yy"
    {}
    break;

  case 508:
#line 2474 "sql_yacc.yy"
    { Select->options|= SELECT_STRAIGHT_JOIN; }
    break;

  case 509:
#line 2476 "sql_yacc.yy"
    {
	    if (check_simple_select())
	      YYABORT;
	    Lex->lock_option= TL_READ_HIGH_PRIORITY;
	  }
    break;

  case 510:
#line 2481 "sql_yacc.yy"
    { Select->options|= SELECT_DISTINCT; }
    break;

  case 511:
#line 2482 "sql_yacc.yy"
    { Select->options|= SELECT_SMALL_RESULT; }
    break;

  case 512:
#line 2483 "sql_yacc.yy"
    { Select->options|= SELECT_BIG_RESULT; }
    break;

  case 513:
#line 2485 "sql_yacc.yy"
    {
	    if (check_simple_select())
	      YYABORT;
	    Select->options|= OPTION_BUFFER_RESULT;
	  }
    break;

  case 514:
#line 2491 "sql_yacc.yy"
    {
	    if (check_simple_select())
	      YYABORT;
	    Select->options|= OPTION_FOUND_ROWS;
	  }
    break;

  case 515:
#line 2496 "sql_yacc.yy"
    { Lex->safe_to_cache_query=0; }
    break;

  case 516:
#line 2498 "sql_yacc.yy"
    {
	    Lex->select_lex.options|= OPTION_TO_QUERY_CACHE;
	  }
    break;

  case 517:
#line 2501 "sql_yacc.yy"
    {}
    break;

  case 519:
#line 2507 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->current_select->set_lock_for_tables(TL_WRITE);
	    lex->safe_to_cache_query=0;
	  }
    break;

  case 520:
#line 2513 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->current_select->
	      set_lock_for_tables(TL_READ_WITH_SHARED_LOCKS);
	    lex->safe_to_cache_query=0;
	  }
    break;

  case 523:
#line 2525 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    if (add_item_to_list(thd, new Item_field(NULL, NULL, "*")))
	      YYABORT;
	    (thd->lex->current_select->with_wild)++;
	  }
    break;

  case 524:
#line 2535 "sql_yacc.yy"
    {
	    if (add_item_to_list(YYTHD, yyvsp[-2].item))
	      YYABORT;
	    if (yyvsp[0].lex_str.str)
	      yyvsp[-2].item->set_name(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,system_charset_info);
	    else if (!yyvsp[-2].item->name)
	      yyvsp[-2].item->set_name(yyvsp[-3].simple_string,(uint) (yyvsp[-1].simple_string - yyvsp[-3].simple_string), YYTHD->charset());
	  }
    break;

  case 525:
#line 2545 "sql_yacc.yy"
    { yyval.simple_string=(char*) Lex->tok_start; }
    break;

  case 526:
#line 2548 "sql_yacc.yy"
    { yyval.simple_string=(char*) Lex->tok_end; }
    break;

  case 527:
#line 2551 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 528:
#line 2552 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 529:
#line 2555 "sql_yacc.yy"
    { yyval.lex_str.str=0;}
    break;

  case 530:
#line 2556 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 531:
#line 2557 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 532:
#line 2558 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 533:
#line 2559 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 534:
#line 2563 "sql_yacc.yy"
    {}
    break;

  case 535:
#line 2564 "sql_yacc.yy"
    {}
    break;

  case 536:
#line 2568 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 537:
#line 2569 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 538:
#line 2572 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_eq_creator; }
    break;

  case 539:
#line 2573 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_ge_creator; }
    break;

  case 540:
#line 2574 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_gt_creator; }
    break;

  case 541:
#line 2575 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_le_creator; }
    break;

  case 542:
#line 2576 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_lt_creator; }
    break;

  case 543:
#line 2577 "sql_yacc.yy"
    { yyval.boolfunc2creator = &comp_ne_creator; }
    break;

  case 544:
#line 2580 "sql_yacc.yy"
    { yyval.num = 1; }
    break;

  case 545:
#line 2581 "sql_yacc.yy"
    { yyval.num = 0; }
    break;

  case 546:
#line 2587 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-4].item); yyval.item= new Item_func_in(*yyvsp[-1].item_list); }
    break;

  case 547:
#line 2589 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-5].item); yyval.item= new Item_func_not(new Item_func_in(*yyvsp[-1].item_list)); }
    break;

  case 548:
#line 2591 "sql_yacc.yy"
    { yyval.item= new Item_in_subselect(yyvsp[-2].item, yyvsp[0].select_lex); }
    break;

  case 549:
#line 2593 "sql_yacc.yy"
    {
            yyval.item= new Item_func_not(new Item_in_subselect(yyvsp[-3].item, yyvsp[0].select_lex));
          }
    break;

  case 550:
#line 2597 "sql_yacc.yy"
    { yyval.item= new Item_func_between(yyvsp[-4].item,yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 551:
#line 2599 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_between(yyvsp[-5].item,yyvsp[-2].item,yyvsp[0].item)); }
    break;

  case 552:
#line 2600 "sql_yacc.yy"
    { yyval.item= or_or_concat(YYTHD, yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 553:
#line 2601 "sql_yacc.yy"
    { yyval.item= new Item_cond_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 554:
#line 2602 "sql_yacc.yy"
    { yyval.item= new Item_cond_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 555:
#line 2603 "sql_yacc.yy"
    { yyval.item= new Item_cond_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 556:
#line 2605 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_eq(new Item_func_soundex(yyvsp[-3].item),
				 new Item_func_soundex(yyvsp[0].item));
	  }
    break;

  case 557:
#line 2610 "sql_yacc.yy"
    { yyval.item= new Item_func_like(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].item); }
    break;

  case 558:
#line 2612 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_like(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].item));}
    break;

  case 559:
#line 2613 "sql_yacc.yy"
    { yyval.item= new Item_func_regex(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 560:
#line 2615 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_regex(yyvsp[-3].item,yyvsp[0].item)); }
    break;

  case 561:
#line 2616 "sql_yacc.yy"
    { yyval.item= new Item_func_isnull(yyvsp[-2].item); }
    break;

  case 562:
#line 2617 "sql_yacc.yy"
    { yyval.item= new Item_func_isnotnull(yyvsp[-3].item); }
    break;

  case 563:
#line 2618 "sql_yacc.yy"
    { yyval.item= new Item_func_equal(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 564:
#line 2619 "sql_yacc.yy"
    { yyval.item= (*yyvsp[-1].boolfunc2creator)(0)->create(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 565:
#line 2621 "sql_yacc.yy"
    {
	  yyval.item= all_any_subquery_creator(yyvsp[-3].item, yyvsp[-2].boolfunc2creator, yyvsp[-1].num, yyvsp[0].select_lex);
	}
    break;

  case 566:
#line 2624 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_left(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 567:
#line 2625 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_right(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 568:
#line 2626 "sql_yacc.yy"
    { yyval.item= new Item_func_plus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 569:
#line 2627 "sql_yacc.yy"
    { yyval.item= new Item_func_minus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 570:
#line 2628 "sql_yacc.yy"
    { yyval.item= new Item_func_mul(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 571:
#line 2629 "sql_yacc.yy"
    { yyval.item= new Item_func_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 572:
#line 2630 "sql_yacc.yy"
    { yyval.item= new Item_func_int_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 573:
#line 2631 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 574:
#line 2632 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 575:
#line 2633 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 576:
#line 2634 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 577:
#line 2635 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 578:
#line 2637 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,0); }
    break;

  case 579:
#line 2639 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,1); }
    break;

  case 580:
#line 2645 "sql_yacc.yy"
    { yyval.item= new Item_func_between(yyvsp[-4].item,yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 581:
#line 2647 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_between(yyvsp[-5].item,yyvsp[-2].item,yyvsp[0].item)); }
    break;

  case 582:
#line 2648 "sql_yacc.yy"
    { yyval.item= or_or_concat(YYTHD, yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 583:
#line 2649 "sql_yacc.yy"
    { yyval.item= new Item_cond_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 584:
#line 2650 "sql_yacc.yy"
    { yyval.item= new Item_cond_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 585:
#line 2651 "sql_yacc.yy"
    { yyval.item= new Item_cond_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 586:
#line 2653 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_eq(new Item_func_soundex(yyvsp[-3].item),
				 new Item_func_soundex(yyvsp[0].item));
	  }
    break;

  case 587:
#line 2658 "sql_yacc.yy"
    { yyval.item= new Item_func_like(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].item); }
    break;

  case 588:
#line 2660 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_like(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].item)); }
    break;

  case 589:
#line 2661 "sql_yacc.yy"
    { yyval.item= new Item_func_regex(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 590:
#line 2663 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_regex(yyvsp[-3].item,yyvsp[0].item)); }
    break;

  case 591:
#line 2664 "sql_yacc.yy"
    { yyval.item= new Item_func_isnull(yyvsp[-2].item); }
    break;

  case 592:
#line 2665 "sql_yacc.yy"
    { yyval.item= new Item_func_isnotnull(yyvsp[-3].item); }
    break;

  case 593:
#line 2666 "sql_yacc.yy"
    { yyval.item= new Item_func_equal(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 594:
#line 2667 "sql_yacc.yy"
    { yyval.item= (*yyvsp[-1].boolfunc2creator)(0)->create(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 595:
#line 2669 "sql_yacc.yy"
    {
	  all_any_subquery_creator(yyvsp[-3].item, yyvsp[-2].boolfunc2creator, yyvsp[-1].num, yyvsp[0].select_lex);
	}
    break;

  case 596:
#line 2672 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_left(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 597:
#line 2673 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_right(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 598:
#line 2674 "sql_yacc.yy"
    { yyval.item= new Item_func_plus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 599:
#line 2675 "sql_yacc.yy"
    { yyval.item= new Item_func_minus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 600:
#line 2676 "sql_yacc.yy"
    { yyval.item= new Item_func_mul(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 601:
#line 2677 "sql_yacc.yy"
    { yyval.item= new Item_func_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 602:
#line 2678 "sql_yacc.yy"
    { yyval.item= new Item_func_int_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 603:
#line 2679 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 604:
#line 2680 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 605:
#line 2681 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 606:
#line 2682 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 607:
#line 2683 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 608:
#line 2685 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,0); }
    break;

  case 609:
#line 2687 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,1); }
    break;

  case 611:
#line 2693 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-4].item); yyval.item= new Item_func_in(*yyvsp[-1].item_list); }
    break;

  case 612:
#line 2695 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-5].item); yyval.item= new Item_func_not(new Item_func_in(*yyvsp[-1].item_list)); }
    break;

  case 613:
#line 2697 "sql_yacc.yy"
    { yyval.item= new Item_in_subselect(yyvsp[-2].item, yyvsp[0].select_lex); }
    break;

  case 614:
#line 2699 "sql_yacc.yy"
    {
            yyval.item= new Item_func_not(new Item_in_subselect(yyvsp[-3].item, yyvsp[0].select_lex));
          }
    break;

  case 615:
#line 2703 "sql_yacc.yy"
    { yyval.item= new Item_func_between(yyvsp[-4].item,yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 616:
#line 2705 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_between(yyvsp[-5].item,yyvsp[-2].item,yyvsp[0].item)); }
    break;

  case 617:
#line 2706 "sql_yacc.yy"
    { yyval.item= or_or_concat(YYTHD, yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 618:
#line 2707 "sql_yacc.yy"
    { yyval.item= new Item_cond_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 619:
#line 2708 "sql_yacc.yy"
    { yyval.item= new Item_cond_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 620:
#line 2710 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_eq(new Item_func_soundex(yyvsp[-3].item),
				 new Item_func_soundex(yyvsp[0].item));
	  }
    break;

  case 621:
#line 2715 "sql_yacc.yy"
    { yyval.item= new Item_func_like(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].item); }
    break;

  case 622:
#line 2717 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_like(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].item)); }
    break;

  case 623:
#line 2718 "sql_yacc.yy"
    { yyval.item= new Item_func_regex(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 624:
#line 2720 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_regex(yyvsp[-3].item,yyvsp[0].item)); }
    break;

  case 625:
#line 2721 "sql_yacc.yy"
    { yyval.item= new Item_func_isnull(yyvsp[-2].item); }
    break;

  case 626:
#line 2722 "sql_yacc.yy"
    { yyval.item= new Item_func_isnotnull(yyvsp[-3].item); }
    break;

  case 627:
#line 2723 "sql_yacc.yy"
    { yyval.item= new Item_func_equal(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 628:
#line 2724 "sql_yacc.yy"
    { yyval.item= (*yyvsp[-1].boolfunc2creator)(0)->create(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 629:
#line 2726 "sql_yacc.yy"
    {
	  all_any_subquery_creator(yyvsp[-3].item, yyvsp[-2].boolfunc2creator, yyvsp[-1].num, yyvsp[0].select_lex);
	}
    break;

  case 630:
#line 2729 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_left(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 631:
#line 2730 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_right(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 632:
#line 2731 "sql_yacc.yy"
    { yyval.item= new Item_func_plus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 633:
#line 2732 "sql_yacc.yy"
    { yyval.item= new Item_func_minus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 634:
#line 2733 "sql_yacc.yy"
    { yyval.item= new Item_func_mul(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 635:
#line 2734 "sql_yacc.yy"
    { yyval.item= new Item_func_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 636:
#line 2735 "sql_yacc.yy"
    { yyval.item= new Item_func_int_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 637:
#line 2736 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 638:
#line 2737 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 639:
#line 2738 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 640:
#line 2739 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 641:
#line 2740 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 642:
#line 2742 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,0); }
    break;

  case 643:
#line 2744 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].interval,1); }
    break;

  case 645:
#line 2748 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 647:
#line 2754 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_set_collation(yyvsp[-2].item,
					    new Item_string(yyvsp[0].lex_str.str,
							    yyvsp[0].lex_str.length,
                                                            YYTHD->charset()));
	  }
    break;

  case 650:
#line 2763 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_set_user_var(yyvsp[-2].lex_str,yyvsp[0].item);
	    Lex->uncacheable(UNCACHEABLE_RAND);
	  }
    break;

  case 651:
#line 2768 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_get_user_var(yyvsp[0].lex_str);
	    Lex->uncacheable(UNCACHEABLE_RAND);
	  }
    break;

  case 652:
#line 2773 "sql_yacc.yy"
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

  case 654:
#line 2784 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 655:
#line 2785 "sql_yacc.yy"
    { yyval.item= new Item_func_neg(yyvsp[0].item); }
    break;

  case 656:
#line 2786 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_neg(yyvsp[0].item); }
    break;

  case 657:
#line 2788 "sql_yacc.yy"
    {
            yyval.item= negate_expression(YYTHD, yyvsp[0].item);
          }
    break;

  case 658:
#line 2792 "sql_yacc.yy"
    {
            yyval.item= negate_expression(YYTHD, yyvsp[0].item);
          }
    break;

  case 659:
#line 2795 "sql_yacc.yy"
    { yyval.item= yyvsp[-1].item; }
    break;

  case 660:
#line 2797 "sql_yacc.yy"
    {
	    yyvsp[-1].item_list->push_front(yyvsp[-3].item);
	    yyval.item= new Item_row(*yyvsp[-1].item_list);
	  }
    break;

  case 661:
#line 2802 "sql_yacc.yy"
    {
	    yyvsp[-1].item_list->push_front(yyvsp[-3].item);
	    yyval.item= new Item_row(*yyvsp[-1].item_list);
	  }
    break;

  case 662:
#line 2806 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 663:
#line 2807 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 664:
#line 2808 "sql_yacc.yy"
    { yyval.item= yyvsp[-1].item; }
    break;

  case 665:
#line 2810 "sql_yacc.yy"
    { yyvsp[-5].item_list->push_front(yyvsp[-2].item);
            Select->add_ftfunc_to_list((Item_func_match*)
                                        (yyval.item=new Item_func_match(*yyvsp[-5].item_list,yyvsp[-1].num))); }
    break;

  case 666:
#line 2813 "sql_yacc.yy"
    { yyval.item= new Item_func_ascii(yyvsp[-1].item); }
    break;

  case 667:
#line 2815 "sql_yacc.yy"
    {
	    yyval.item= create_func_cast(yyvsp[0].item, ITEM_CAST_CHAR, -1, &my_charset_bin);
	  }
    break;

  case 668:
#line 2819 "sql_yacc.yy"
    {
	    yyval.item= create_func_cast(yyvsp[-3].item, yyvsp[-1].cast_type,
				 Lex->length ? atoi(Lex->length) : -1,
				 Lex->charset);
	  }
    break;

  case 669:
#line 2825 "sql_yacc.yy"
    { yyval.item= new Item_func_case(* yyvsp[-2].item_list, yyvsp[-4].item, yyvsp[-1].item ); }
    break;

  case 670:
#line 2827 "sql_yacc.yy"
    {
	    yyval.item= create_func_cast(yyvsp[-3].item, yyvsp[-1].cast_type,
				 Lex->length ? atoi(Lex->length) : -1,
				 Lex->charset);
	  }
    break;

  case 671:
#line 2833 "sql_yacc.yy"
    { yyval.item= new Item_func_conv_charset(yyvsp[-3].item,yyvsp[-1].charset); }
    break;

  case 672:
#line 2835 "sql_yacc.yy"
    { yyval.item= new Item_default_value(yyvsp[-1].item); }
    break;

  case 673:
#line 2837 "sql_yacc.yy"
    { yyval.item= new Item_insert_value(yyvsp[-1].item); }
    break;

  case 674:
#line 2839 "sql_yacc.yy"
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

  case 675:
#line 2850 "sql_yacc.yy"
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

  case 676:
#line 2861 "sql_yacc.yy"
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

  case 677:
#line 2872 "sql_yacc.yy"
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

  case 678:
#line 2883 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item, yyvsp[-1].item, INTERVAL_DAY, 0);}
    break;

  case 679:
#line 2885 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-5].item, yyvsp[-2].item, yyvsp[-1].interval, 0); }
    break;

  case 680:
#line 2887 "sql_yacc.yy"
    { yyval.item= new Item_func_atan(yyvsp[-1].item); }
    break;

  case 681:
#line 2889 "sql_yacc.yy"
    { yyval.item= new Item_func_atan(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 682:
#line 2891 "sql_yacc.yy"
    { yyval.item= new Item_func_char(*yyvsp[-1].item_list); }
    break;

  case 683:
#line 2893 "sql_yacc.yy"
    { yyval.item= new Item_func_charset(yyvsp[-1].item); }
    break;

  case 684:
#line 2895 "sql_yacc.yy"
    { yyval.item= new Item_func_coalesce(* yyvsp[-1].item_list); }
    break;

  case 685:
#line 2897 "sql_yacc.yy"
    { yyval.item= new Item_func_collation(yyvsp[-1].item); }
    break;

  case 686:
#line 2899 "sql_yacc.yy"
    { yyval.item= new Item_func_concat(* yyvsp[-1].item_list); }
    break;

  case 687:
#line 2901 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_concat_ws(*yyvsp[-1].item_list); }
    break;

  case 688:
#line 2903 "sql_yacc.yy"
    {
	    Lex->time_zone_tables_used= &fake_time_zone_tables_list;
	    yyval.item= new Item_func_convert_tz(yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item);
	  }
    break;

  case 689:
#line 2908 "sql_yacc.yy"
    { yyval.item= new Item_func_curdate_local(); Lex->safe_to_cache_query=0; }
    break;

  case 690:
#line 2910 "sql_yacc.yy"
    { yyval.item= new Item_func_curtime_local(); Lex->safe_to_cache_query=0; }
    break;

  case 691:
#line 2912 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_curtime_local(yyvsp[-1].item);
	    Lex->safe_to_cache_query=0;
	  }
    break;

  case 692:
#line 2917 "sql_yacc.yy"
    { yyval.item= create_func_current_user(); }
    break;

  case 693:
#line 2919 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-2].item,yyvsp[-1].interval,0); }
    break;

  case 694:
#line 2921 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-2].item,yyvsp[-1].interval,1); }
    break;

  case 695:
#line 2923 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_database();
            Lex->safe_to_cache_query=0;
	  }
    break;

  case 696:
#line 2928 "sql_yacc.yy"
    { yyval.item= new Item_date_typecast(yyvsp[-1].item); }
    break;

  case 697:
#line 2930 "sql_yacc.yy"
    { yyval.item= new Item_func_dayofmonth(yyvsp[-1].item); }
    break;

  case 698:
#line 2932 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_elt(*yyvsp[-1].item_list); }
    break;

  case 699:
#line 2934 "sql_yacc.yy"
    { yyval.item= new Item_func_make_set(yyvsp[-3].item, *yyvsp[-1].item_list); }
    break;

  case 700:
#line 2936 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_encrypt(yyvsp[-1].item);
	    Lex->uncacheable(UNCACHEABLE_RAND);
	  }
    break;

  case 701:
#line 2940 "sql_yacc.yy"
    { yyval.item= new Item_func_encrypt(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 702:
#line 2942 "sql_yacc.yy"
    { yyval.item= new Item_func_decode(yyvsp[-3].item,yyvsp[-1].lex_str.str); }
    break;

  case 703:
#line 2944 "sql_yacc.yy"
    { yyval.item= new Item_func_encode(yyvsp[-3].item,yyvsp[-1].lex_str.str); }
    break;

  case 704:
#line 2946 "sql_yacc.yy"
    { yyval.item= new Item_func_des_decrypt(yyvsp[-1].item); }
    break;

  case 705:
#line 2948 "sql_yacc.yy"
    { yyval.item= new Item_func_des_decrypt(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 706:
#line 2950 "sql_yacc.yy"
    { yyval.item= new Item_func_des_encrypt(yyvsp[-1].item); }
    break;

  case 707:
#line 2952 "sql_yacc.yy"
    { yyval.item= new Item_func_des_encrypt(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 708:
#line 2954 "sql_yacc.yy"
    { yyval.item= new Item_func_export_set(yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 709:
#line 2956 "sql_yacc.yy"
    { yyval.item= new Item_func_export_set(yyvsp[-7].item, yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 710:
#line 2958 "sql_yacc.yy"
    { yyval.item= new Item_func_export_set(yyvsp[-9].item, yyvsp[-7].item, yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 711:
#line 2960 "sql_yacc.yy"
    { yyval.item= new Item_int((char*) "FALSE",0,1); }
    break;

  case 712:
#line 2962 "sql_yacc.yy"
    { yyval.item= new Item_func_format(yyvsp[-3].item,atoi(yyvsp[-1].lex_str.str)); }
    break;

  case 713:
#line 2964 "sql_yacc.yy"
    { yyval.item= new Item_func_from_unixtime(yyvsp[-1].item); }
    break;

  case 714:
#line 2966 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_date_format (new Item_func_from_unixtime(yyvsp[-3].item),yyvsp[-1].item,0);
	  }
    break;

  case 715:
#line 2970 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_field(*yyvsp[-1].item_list); }
    break;

  case 716:
#line 2972 "sql_yacc.yy"
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
#line 2982 "sql_yacc.yy"
    { yyval.item= new Item_func_get_format(yyvsp[-3].date_time_type, yyvsp[-1].item); }
    break;

  case 718:
#line 2984 "sql_yacc.yy"
    { yyval.item= new Item_func_hour(yyvsp[-1].item); }
    break;

  case 719:
#line 2986 "sql_yacc.yy"
    { yyval.item= new Item_func_if(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 720:
#line 2988 "sql_yacc.yy"
    { yyval.item= new Item_func_insert(yyvsp[-7].item,yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 721:
#line 2991 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[0].item,yyvsp[-3].item,yyvsp[-2].interval,0); }
    break;

  case 722:
#line 2993 "sql_yacc.yy"
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
#line 3002 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_last_insert_id();
	    Lex->safe_to_cache_query= 0;
	  }
    break;

  case 724:
#line 3007 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_last_insert_id(yyvsp[-1].item);
	    Lex->safe_to_cache_query= 0;
	  }
    break;

  case 725:
#line 3012 "sql_yacc.yy"
    { yyval.item= new Item_func_left(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 726:
#line 3014 "sql_yacc.yy"
    { yyval.item= new Item_func_locate(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 727:
#line 3016 "sql_yacc.yy"
    { yyval.item= new Item_func_locate(yyvsp[-3].item,yyvsp[-5].item,yyvsp[-1].item); }
    break;

  case 728:
#line 3018 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_max(*yyvsp[-1].item_list); }
    break;

  case 729:
#line 3020 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_min(*yyvsp[-1].item_list); }
    break;

  case 730:
#line 3022 "sql_yacc.yy"
    { yyval.item= new Item_func_log(yyvsp[-1].item); }
    break;

  case 731:
#line 3024 "sql_yacc.yy"
    { yyval.item= new Item_func_log(yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 732:
#line 3026 "sql_yacc.yy"
    {
	    yyval.item= new Item_master_pos_wait(yyvsp[-3].item, yyvsp[-1].item);
	    Lex->safe_to_cache_query=0;
		  }
    break;

  case 733:
#line 3031 "sql_yacc.yy"
    {
	    yyval.item= new Item_master_pos_wait(yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item);
	    Lex->safe_to_cache_query=0;
	  }
    break;

  case 734:
#line 3036 "sql_yacc.yy"
    { yyval.item= new Item_func_microsecond(yyvsp[-1].item); }
    break;

  case 735:
#line 3038 "sql_yacc.yy"
    { yyval.item= new Item_func_minute(yyvsp[-1].item); }
    break;

  case 736:
#line 3040 "sql_yacc.yy"
    { yyval.item = new Item_func_mod( yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 737:
#line 3042 "sql_yacc.yy"
    { yyval.item= new Item_func_month(yyvsp[-1].item); }
    break;

  case 738:
#line 3044 "sql_yacc.yy"
    { yyval.item= new Item_func_now_local(); Lex->safe_to_cache_query=0;}
    break;

  case 739:
#line 3046 "sql_yacc.yy"
    { yyval.item= new Item_func_now_local(yyvsp[-1].item); Lex->safe_to_cache_query=0;}
    break;

  case 740:
#line 3048 "sql_yacc.yy"
    {
	    yyval.item= YYTHD->variables.old_passwords ?
              (Item *) new Item_func_old_password(yyvsp[-1].item) :
	      (Item *) new Item_func_password(yyvsp[-1].item);
	  }
    break;

  case 741:
#line 3054 "sql_yacc.yy"
    { yyval.item=  new Item_func_old_password(yyvsp[-1].item); }
    break;

  case 742:
#line 3056 "sql_yacc.yy"
    { yyval.item = new Item_func_locate(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 743:
#line 3058 "sql_yacc.yy"
    { yyval.item= new Item_func_rand(yyvsp[-1].item); Lex->uncacheable(UNCACHEABLE_RAND);}
    break;

  case 744:
#line 3060 "sql_yacc.yy"
    { yyval.item= new Item_func_rand(); Lex->uncacheable(UNCACHEABLE_RAND);}
    break;

  case 745:
#line 3062 "sql_yacc.yy"
    { yyval.item= new Item_func_replace(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 746:
#line 3064 "sql_yacc.yy"
    { yyval.item= new Item_func_right(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 747:
#line 3066 "sql_yacc.yy"
    { yyval.item= new Item_func_round(yyvsp[-1].item, new Item_int((char*)"0",0,1),0); }
    break;

  case 748:
#line 3067 "sql_yacc.yy"
    { yyval.item= new Item_func_round(yyvsp[-3].item,yyvsp[-1].item,0); }
    break;

  case 749:
#line 3069 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-3].item, yyvsp[-1].item, INTERVAL_DAY, 1);}
    break;

  case 750:
#line 3071 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-5].item, yyvsp[-2].item, yyvsp[-1].interval, 1); }
    break;

  case 751:
#line 3073 "sql_yacc.yy"
    { yyval.item= new Item_func_second(yyvsp[-1].item); }
    break;

  case 752:
#line 3075 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 753:
#line 3077 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 754:
#line 3079 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 755:
#line 3081 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 756:
#line 3083 "sql_yacc.yy"
    { yyval.item= new Item_func_substr_index(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 757:
#line 3085 "sql_yacc.yy"
    { yyval.item= new Item_time_typecast(yyvsp[-1].item); }
    break;

  case 758:
#line 3087 "sql_yacc.yy"
    { yyval.item= new Item_datetime_typecast(yyvsp[-1].item); }
    break;

  case 759:
#line 3089 "sql_yacc.yy"
    { yyval.item= new Item_func_add_time(yyvsp[-3].item, yyvsp[-1].item, 1, 0); }
    break;

  case 760:
#line 3091 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item); }
    break;

  case 761:
#line 3093 "sql_yacc.yy"
    { yyval.item= new Item_func_ltrim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 762:
#line 3095 "sql_yacc.yy"
    { yyval.item= new Item_func_rtrim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 763:
#line 3097 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 764:
#line 3099 "sql_yacc.yy"
    { yyval.item= new Item_func_ltrim(yyvsp[-1].item); }
    break;

  case 765:
#line 3101 "sql_yacc.yy"
    { yyval.item= new Item_func_rtrim(yyvsp[-1].item); }
    break;

  case 766:
#line 3103 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item); }
    break;

  case 767:
#line 3105 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 768:
#line 3107 "sql_yacc.yy"
    { yyval.item= new Item_func_round(yyvsp[-3].item,yyvsp[-1].item,1); }
    break;

  case 769:
#line 3109 "sql_yacc.yy"
    { yyval.item= new Item_int((char*) "TRUE",1,1); }
    break;

  case 770:
#line 3111 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_sum_udf_str(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_sum_udf_str(yyvsp[-3].udf);
	  }
    break;

  case 771:
#line 3118 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_sum_udf_float(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_sum_udf_float(yyvsp[-3].udf);
	  }
    break;

  case 772:
#line 3125 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_sum_udf_int(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_sum_udf_int(yyvsp[-3].udf);
	  }
    break;

  case 773:
#line 3132 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_func_udf_str(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_func_udf_str(yyvsp[-3].udf);
	  }
    break;

  case 774:
#line 3139 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_func_udf_float(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_func_udf_float(yyvsp[-3].udf);
	  }
    break;

  case 775:
#line 3146 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_func_udf_int(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_func_udf_int(yyvsp[-3].udf);
	  }
    break;

  case 776:
#line 3153 "sql_yacc.yy"
    {
            yyval.item= new Item_func_unique_users(yyvsp[-7].item,atoi(yyvsp[-5].lex_str.str),atoi(yyvsp[-3].lex_str.str), * yyvsp[-1].item_list);
	  }
    break;

  case 777:
#line 3157 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_unix_timestamp();
	    Lex->safe_to_cache_query=0;
	  }
    break;

  case 778:
#line 3162 "sql_yacc.yy"
    { yyval.item= new Item_func_unix_timestamp(yyvsp[-1].item); }
    break;

  case 779:
#line 3164 "sql_yacc.yy"
    { yyval.item= new Item_func_user(); Lex->safe_to_cache_query=0; }
    break;

  case 780:
#line 3166 "sql_yacc.yy"
    { yyval.item= new Item_func_curdate_utc(); Lex->safe_to_cache_query=0;}
    break;

  case 781:
#line 3168 "sql_yacc.yy"
    { yyval.item= new Item_func_curtime_utc(); Lex->safe_to_cache_query=0;}
    break;

  case 782:
#line 3170 "sql_yacc.yy"
    { yyval.item= new Item_func_now_utc(); Lex->safe_to_cache_query=0;}
    break;

  case 783:
#line 3172 "sql_yacc.yy"
    {
            yyval.item= new Item_func_week(yyvsp[-1].item,new Item_int((char*) "0",
				   YYTHD->variables.default_week_format,1));
          }
    break;

  case 784:
#line 3177 "sql_yacc.yy"
    { yyval.item= new Item_func_week(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 785:
#line 3179 "sql_yacc.yy"
    { yyval.item= new Item_func_year(yyvsp[-1].item); }
    break;

  case 786:
#line 3181 "sql_yacc.yy"
    { yyval.item= new Item_func_yearweek(yyvsp[-1].item,new Item_int((char*) "0",0,1)); }
    break;

  case 787:
#line 3183 "sql_yacc.yy"
    { yyval.item= new Item_func_yearweek(yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 788:
#line 3185 "sql_yacc.yy"
    {
	    yyval.item=new Item_func_benchmark(yyvsp[-3].ulong_num,yyvsp[-1].item);
	    Lex->uncacheable(UNCACHEABLE_SIDEEFFECT);
	  }
    break;

  case 789:
#line 3190 "sql_yacc.yy"
    { yyval.item=new Item_extract( yyvsp[-3].interval, yyvsp[-1].item); }
    break;

  case 790:
#line 3194 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 791:
#line 3196 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 792:
#line 3198 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_wkb(yyvsp[-1].item)); }
    break;

  case 793:
#line 3200 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_wkb(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 794:
#line 3202 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
                           Geometry::wkb_geometrycollection,
                           Geometry::wkb_point)); }
    break;

  case 795:
#line 3206 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
                  Geometry::wkb_linestring, Geometry::wkb_point)); }
    break;

  case 796:
#line 3209 "sql_yacc.yy"
    { yyval.item= GEOM_NEW( Item_func_spatial_collection(* yyvsp[-1].item_list,
                   Geometry::wkb_multilinestring, Geometry::wkb_linestring)); }
    break;

  case 797:
#line 3212 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 798:
#line 3214 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 799:
#line 3216 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 800:
#line 3218 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 801:
#line 3220 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 802:
#line 3222 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 803:
#line 3224 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
                  Geometry::wkb_multipoint, Geometry::wkb_point)); }
    break;

  case 804:
#line 3227 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
                  Geometry::wkb_multipolygon, Geometry::wkb_polygon)); }
    break;

  case 805:
#line 3230 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_point(yyvsp[-3].item,yyvsp[-1].item)); }
    break;

  case 806:
#line 3232 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 807:
#line 3234 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 808:
#line 3236 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 809:
#line 3238 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 810:
#line 3240 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_spatial_collection(* yyvsp[-1].item_list,
	          Geometry::wkb_polygon, Geometry::wkb_linestring)); }
    break;

  case 811:
#line 3243 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 812:
#line 3245 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 813:
#line 3247 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-1].item)); }
    break;

  case 814:
#line 3249 "sql_yacc.yy"
    { yyval.item= GEOM_NEW(Item_func_geometry_from_text(yyvsp[-3].item, yyvsp[-1].item)); }
    break;

  case 815:
#line 3253 "sql_yacc.yy"
    { yyval.num= FT_NL;  }
    break;

  case 816:
#line 3254 "sql_yacc.yy"
    { yyval.num= FT_NL | FT_EXPAND; }
    break;

  case 817:
#line 3255 "sql_yacc.yy"
    { yyval.num= FT_BOOL; }
    break;

  case 818:
#line 3259 "sql_yacc.yy"
    { yyval.item_list= NULL; }
    break;

  case 819:
#line 3260 "sql_yacc.yy"
    { yyval.item_list= yyvsp[0].item_list;}
    break;

  case 820:
#line 3264 "sql_yacc.yy"
    { yyval.item=new Item_sum_avg(yyvsp[-1].item); }
    break;

  case 821:
#line 3266 "sql_yacc.yy"
    { yyval.item=new Item_sum_and(yyvsp[-1].item); }
    break;

  case 822:
#line 3268 "sql_yacc.yy"
    { yyval.item=new Item_sum_or(yyvsp[-1].item); }
    break;

  case 823:
#line 3270 "sql_yacc.yy"
    { yyval.item=new Item_sum_xor(yyvsp[-1].item); }
    break;

  case 824:
#line 3272 "sql_yacc.yy"
    { yyval.item=new Item_sum_count(new Item_int((int32) 0L,1)); }
    break;

  case 825:
#line 3274 "sql_yacc.yy"
    { yyval.item=new Item_sum_count(yyvsp[-1].item); }
    break;

  case 826:
#line 3276 "sql_yacc.yy"
    { Select->in_sum_expr++; }
    break;

  case 827:
#line 3278 "sql_yacc.yy"
    { Select->in_sum_expr--; }
    break;

  case 828:
#line 3280 "sql_yacc.yy"
    { yyval.item=new Item_sum_count_distinct(* yyvsp[-2].item_list); }
    break;

  case 829:
#line 3282 "sql_yacc.yy"
    { yyval.item= new Item_sum_unique_users(yyvsp[-7].item,atoi(yyvsp[-5].lex_str.str),atoi(yyvsp[-3].lex_str.str),yyvsp[-1].item); }
    break;

  case 830:
#line 3284 "sql_yacc.yy"
    { yyval.item=new Item_sum_min(yyvsp[-1].item); }
    break;

  case 831:
#line 3286 "sql_yacc.yy"
    { yyval.item=new Item_sum_max(yyvsp[-1].item); }
    break;

  case 832:
#line 3288 "sql_yacc.yy"
    { yyval.item=new Item_sum_std(yyvsp[-1].item); }
    break;

  case 833:
#line 3290 "sql_yacc.yy"
    { yyval.item=new Item_sum_variance(yyvsp[-1].item); }
    break;

  case 834:
#line 3292 "sql_yacc.yy"
    { yyval.item=new Item_sum_sum(yyvsp[-1].item); }
    break;

  case 835:
#line 3294 "sql_yacc.yy"
    { Select->in_sum_expr++; }
    break;

  case 836:
#line 3298 "sql_yacc.yy"
    {
	    Select->in_sum_expr--;
	    yyval.item=new Item_func_group_concat(yyvsp[-5].num,yyvsp[-3].item_list,Select->gorder_list,yyvsp[-1].string);
	    yyvsp[-3].item_list->empty();
	  }
    break;

  case 837:
#line 3305 "sql_yacc.yy"
    { yyval.num = 0; }
    break;

  case 838:
#line 3306 "sql_yacc.yy"
    { yyval.num = 1; }
    break;

  case 839:
#line 3309 "sql_yacc.yy"
    { yyval.string = new (YYTHD->mem_root) String(",",1,default_charset_info); }
    break;

  case 840:
#line 3310 "sql_yacc.yy"
    { yyval.string = yyvsp[0].string; }
    break;

  case 841:
#line 3315 "sql_yacc.yy"
    {
            Select->gorder_list = NULL;
	  }
    break;

  case 842:
#line 3319 "sql_yacc.yy"
    {
            SELECT_LEX *select= Select;
            select->gorder_list=
	      (SQL_LIST*) sql_memdup((char*) &select->order_list,
				     sizeof(st_sql_list));
	    select->order_list.empty();
	  }
    break;

  case 843:
#line 3330 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  if (lex->current_select->inc_in_sum_expr())
	  {
	    yyerror(ER(ER_SYNTAX_ERROR));
	    YYABORT;
	  }
	}
    break;

  case 844:
#line 3339 "sql_yacc.yy"
    {
	  Select->in_sum_expr--;
	  yyval.item= yyvsp[0].item;
	}
    break;

  case 845:
#line 3345 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_CHAR; Lex->charset= &my_charset_bin; }
    break;

  case 846:
#line 3346 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_CHAR; }
    break;

  case 847:
#line 3347 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_CHAR; Lex->charset= national_charset_info; }
    break;

  case 848:
#line 3348 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_SIGNED_INT; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 849:
#line 3349 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_SIGNED_INT; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 850:
#line 3350 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_UNSIGNED_INT; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 851:
#line 3351 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_UNSIGNED_INT; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 852:
#line 3352 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_DATE; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 853:
#line 3353 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_TIME; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 854:
#line 3354 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_DATETIME; Lex->charset= NULL; Lex->length= (char*)0; }
    break;

  case 855:
#line 3358 "sql_yacc.yy"
    { Select->expr_list.push_front(new List<Item>); }
    break;

  case 856:
#line 3360 "sql_yacc.yy"
    { yyval.item_list= Select->expr_list.pop(); }
    break;

  case 857:
#line 3363 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 858:
#line 3364 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 859:
#line 3367 "sql_yacc.yy"
    { yyval.item_list= yyvsp[0].item_list; }
    break;

  case 860:
#line 3368 "sql_yacc.yy"
    { yyval.item_list= yyvsp[-1].item_list; }
    break;

  case 861:
#line 3371 "sql_yacc.yy"
    { Select->expr_list.push_front(new List<Item>); }
    break;

  case 862:
#line 3373 "sql_yacc.yy"
    { yyval.item_list= Select->expr_list.pop(); }
    break;

  case 863:
#line 3376 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 864:
#line 3377 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 865:
#line 3380 "sql_yacc.yy"
    { yyval.item= NULL; }
    break;

  case 866:
#line 3381 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 867:
#line 3384 "sql_yacc.yy"
    { yyval.item= NULL; }
    break;

  case 868:
#line 3385 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 869:
#line 3388 "sql_yacc.yy"
    { Select->when_list.push_front(new List<Item>); }
    break;

  case 870:
#line 3390 "sql_yacc.yy"
    { yyval.item_list= Select->when_list.pop(); }
    break;

  case 871:
#line 3394 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->when_list.head()->push_back(yyvsp[-2].item);
	    sel->when_list.head()->push_back(yyvsp[0].item);
	}
    break;

  case 872:
#line 3400 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->when_list.head()->push_back(yyvsp[-2].item);
	    sel->when_list.head()->push_back(yyvsp[0].item);
	  }
    break;

  case 873:
#line 3407 "sql_yacc.yy"
    { yyval.table_list=yyvsp[-1].table_list; }
    break;

  case 874:
#line 3408 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list; }
    break;

  case 875:
#line 3409 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list; }
    break;

  case 876:
#line 3410 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list; }
    break;

  case 877:
#line 3412 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list ; yyvsp[-2].table_list->next->straight=1; }
    break;

  case 878:
#line 3414 "sql_yacc.yy"
    { add_join_on(yyvsp[-2].table_list,yyvsp[0].item); yyval.table_list=yyvsp[-2].table_list; }
    break;

  case 879:
#line 3417 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->db1=yyvsp[-3].table_list->db; sel->table1=yyvsp[-3].table_list->alias;
	    sel->db2=yyvsp[-1].table_list->db; sel->table2=yyvsp[-1].table_list->alias;
	  }
    break;

  case 880:
#line 3423 "sql_yacc.yy"
    { add_join_on(yyvsp[-5].table_list,yyvsp[-1].item); yyval.table_list=yyvsp[-5].table_list; }
    break;

  case 881:
#line 3426 "sql_yacc.yy"
    { add_join_on(yyvsp[-2].table_list,yyvsp[0].item); yyvsp[-2].table_list->outer_join|=JOIN_TYPE_LEFT; yyval.table_list=yyvsp[-2].table_list; }
    break;

  case 882:
#line 3428 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->db1=yyvsp[-4].table_list->db; sel->table1=yyvsp[-4].table_list->alias;
	    sel->db2=yyvsp[0].table_list->db; sel->table2=yyvsp[0].table_list->alias;
	  }
    break;

  case 883:
#line 3434 "sql_yacc.yy"
    { add_join_on(yyvsp[-5].table_list,yyvsp[-1].item); yyvsp[-5].table_list->outer_join|=JOIN_TYPE_LEFT; yyval.table_list=yyvsp[-5].table_list; }
    break;

  case 884:
#line 3436 "sql_yacc.yy"
    {
	    add_join_natural(yyvsp[-5].table_list,yyvsp[-5].table_list->next);
	    yyvsp[-5].table_list->next->outer_join|=JOIN_TYPE_LEFT;
	    yyval.table_list=yyvsp[0].table_list;
	  }
    break;

  case 885:
#line 3442 "sql_yacc.yy"
    { add_join_on(yyvsp[-6].table_list,yyvsp[0].item); yyvsp[-6].table_list->outer_join|=JOIN_TYPE_RIGHT; yyval.table_list=yyvsp[-2].table_list; }
    break;

  case 886:
#line 3444 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->db1=yyvsp[-4].table_list->db; sel->table1=yyvsp[-4].table_list->alias;
	    sel->db2=yyvsp[0].table_list->db; sel->table2=yyvsp[0].table_list->alias;
	  }
    break;

  case 887:
#line 3450 "sql_yacc.yy"
    { add_join_on(yyvsp[-9].table_list,yyvsp[-1].item); yyvsp[-9].table_list->outer_join|=JOIN_TYPE_RIGHT; yyval.table_list=yyvsp[-5].table_list; }
    break;

  case 888:
#line 3452 "sql_yacc.yy"
    {
	    add_join_natural(yyvsp[-5].table_list->next,yyvsp[-5].table_list);
	    yyvsp[-5].table_list->outer_join|=JOIN_TYPE_RIGHT;
	    yyval.table_list=yyvsp[0].table_list;
	  }
    break;

  case 889:
#line 3458 "sql_yacc.yy"
    { add_join_natural(yyvsp[-3].table_list,yyvsp[-3].table_list->next); yyval.table_list=yyvsp[0].table_list; }
    break;

  case 890:
#line 3461 "sql_yacc.yy"
    {}
    break;

  case 891:
#line 3462 "sql_yacc.yy"
    {}
    break;

  case 892:
#line 3463 "sql_yacc.yy"
    {}
    break;

  case 893:
#line 3467 "sql_yacc.yy"
    {
	  SELECT_LEX *sel= Select;
	  sel->use_index_ptr=sel->ignore_index_ptr=0;
	  sel->table_join_options= 0;
	}
    break;

  case 894:
#line 3473 "sql_yacc.yy"
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

  case 895:
#line 3484 "sql_yacc.yy"
    { add_join_on(yyvsp[-3].table_list,yyvsp[-1].item); yyvsp[-3].table_list->outer_join|=JOIN_TYPE_LEFT; yyval.table_list=yyvsp[-3].table_list; }
    break;

  case 896:
#line 3486 "sql_yacc.yy"
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

  case 897:
#line 3499 "sql_yacc.yy"
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

  case 898:
#line 3517 "sql_yacc.yy"
    {
	  Select->parsing_place= NO_MATTER;
	}
    break;

  case 900:
#line 3524 "sql_yacc.yy"
    {}
    break;

  case 901:
#line 3525 "sql_yacc.yy"
    {}
    break;

  case 902:
#line 3528 "sql_yacc.yy"
    {}
    break;

  case 903:
#line 3530 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->use_index= *yyvsp[0].string_list;
	    sel->use_index_ptr= &sel->use_index;
	  }
    break;

  case 904:
#line 3536 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->use_index= *yyvsp[0].string_list;
	    sel->use_index_ptr= &sel->use_index;
	    sel->table_join_options|= TL_OPTION_FORCE_INDEX;
	  }
    break;

  case 905:
#line 3543 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->ignore_index= *yyvsp[0].string_list;
	    sel->ignore_index_ptr= &sel->ignore_index;
	  }
    break;

  case 906:
#line 3550 "sql_yacc.yy"
    { Select->interval_list.empty(); }
    break;

  case 907:
#line 3552 "sql_yacc.yy"
    { yyval.string_list= &Select->interval_list; }
    break;

  case 908:
#line 3556 "sql_yacc.yy"
    {}
    break;

  case 909:
#line 3557 "sql_yacc.yy"
    {}
    break;

  case 910:
#line 3562 "sql_yacc.yy"
    { Select->
	    interval_list.push_back(new (YYTHD->mem_root) String((const char*) yyvsp[0].lex_str.str, yyvsp[0].lex_str.length,
				    system_charset_info)); }
    break;

  case 911:
#line 3566 "sql_yacc.yy"
    { Select->
	    interval_list.push_back(new (YYTHD->mem_root) String((const char*) yyvsp[0].lex_str.str, yyvsp[0].lex_str.length,
				    system_charset_info)); }
    break;

  case 912:
#line 3570 "sql_yacc.yy"
    { Select->
	    interval_list.push_back(new (YYTHD->mem_root) String("PRIMARY", 7,
				    system_charset_info)); }
    break;

  case 913:
#line 3576 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    if (!(yyval.item= new Item_func_eq(new Item_field(sel->db1, sel->table1,
						      yyvsp[0].lex_str.str),
				       new Item_field(sel->db2, sel->table2,
						      yyvsp[0].lex_str.str))))
	      YYABORT;
	  }
    break;

  case 914:
#line 3585 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    if (!(yyval.item= new Item_cond_and(new Item_func_eq(new Item_field(sel->db1,sel->table1,yyvsp[0].lex_str.str), new Item_field(sel->db2,sel->table2,yyvsp[0].lex_str.str)), yyvsp[-2].item)))
	      YYABORT;
	  }
    break;

  case 915:
#line 3592 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_HOUR; }
    break;

  case 916:
#line 3593 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_MICROSECOND; }
    break;

  case 917:
#line 3594 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_MINUTE; }
    break;

  case 918:
#line 3595 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_SECOND; }
    break;

  case 919:
#line 3596 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY; }
    break;

  case 920:
#line 3597 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR_MICROSECOND; }
    break;

  case 921:
#line 3598 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR_MINUTE; }
    break;

  case 922:
#line 3599 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR_SECOND; }
    break;

  case 923:
#line 3600 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR; }
    break;

  case 924:
#line 3601 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MICROSECOND; }
    break;

  case 925:
#line 3602 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MINUTE_MICROSECOND; }
    break;

  case 926:
#line 3603 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MINUTE_SECOND; }
    break;

  case 927:
#line 3604 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MINUTE; }
    break;

  case 928:
#line 3605 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MONTH; }
    break;

  case 929:
#line 3606 "sql_yacc.yy"
    { yyval.interval=INTERVAL_SECOND_MICROSECOND; }
    break;

  case 930:
#line 3607 "sql_yacc.yy"
    { yyval.interval=INTERVAL_SECOND; }
    break;

  case 931:
#line 3608 "sql_yacc.yy"
    { yyval.interval=INTERVAL_YEAR_MONTH; }
    break;

  case 932:
#line 3609 "sql_yacc.yy"
    { yyval.interval=INTERVAL_YEAR; }
    break;

  case 933:
#line 3613 "sql_yacc.yy"
    {yyval.date_time_type=MYSQL_TIMESTAMP_DATE;}
    break;

  case 934:
#line 3614 "sql_yacc.yy"
    {yyval.date_time_type=MYSQL_TIMESTAMP_TIME;}
    break;

  case 935:
#line 3615 "sql_yacc.yy"
    {yyval.date_time_type=MYSQL_TIMESTAMP_DATETIME;}
    break;

  case 936:
#line 3616 "sql_yacc.yy"
    {yyval.date_time_type=MYSQL_TIMESTAMP_DATETIME;}
    break;

  case 940:
#line 3625 "sql_yacc.yy"
    { yyval.lex_str_ptr=0; }
    break;

  case 941:
#line 3627 "sql_yacc.yy"
    { yyval.lex_str_ptr= (LEX_STRING*) sql_memdup(&yyvsp[0].lex_str,sizeof(LEX_STRING)); }
    break;

  case 944:
#line 3635 "sql_yacc.yy"
    { Select->where= 0; }
    break;

  case 945:
#line 3637 "sql_yacc.yy"
    {
            Select->parsing_place= IN_WHERE;
          }
    break;

  case 946:
#line 3641 "sql_yacc.yy"
    {
            SELECT_LEX *select= Select;
	    select->where= yyvsp[0].item;
            select->parsing_place= NO_MATTER;
	    if (yyvsp[0].item)
	      yyvsp[0].item->top_level_item();
	  }
    break;

  case 948:
#line 3653 "sql_yacc.yy"
    {
	    Select->parsing_place= IN_HAVING;
          }
    break;

  case 949:
#line 3657 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->having= yyvsp[0].item;
	    sel->parsing_place= NO_MATTER;
	    if (yyvsp[0].item)
	      yyvsp[0].item->top_level_item();
	  }
    break;

  case 950:
#line 3667 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 951:
#line 3669 "sql_yacc.yy"
    { 
            yyval.item= new Item_string("\\", 1, &my_charset_latin1);
          }
    break;

  case 954:
#line 3685 "sql_yacc.yy"
    { if (add_group_to_list(YYTHD, yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 955:
#line 3687 "sql_yacc.yy"
    { if (add_group_to_list(YYTHD, yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 956:
#line 3690 "sql_yacc.yy"
    {}
    break;

  case 957:
#line 3692 "sql_yacc.yy"
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

  case 958:
#line 3705 "sql_yacc.yy"
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

  case 961:
#line 3727 "sql_yacc.yy"
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

  case 963:
#line 3742 "sql_yacc.yy"
    { if (add_order_to_list(YYTHD, yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 964:
#line 3744 "sql_yacc.yy"
    { if (add_order_to_list(YYTHD, yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 965:
#line 3747 "sql_yacc.yy"
    { yyval.num =  1; }
    break;

  case 966:
#line 3748 "sql_yacc.yy"
    { yyval.num =1; }
    break;

  case 967:
#line 3749 "sql_yacc.yy"
    { yyval.num =0; }
    break;

  case 968:
#line 3754 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  SELECT_LEX *sel= lex->current_select;
          sel->offset_limit= 0L;
          sel->select_limit= HA_POS_ERROR;
	}
    break;

  case 969:
#line 3760 "sql_yacc.yy"
    {}
    break;

  case 970:
#line 3764 "sql_yacc.yy"
    {}
    break;

  case 971:
#line 3765 "sql_yacc.yy"
    {}
    break;

  case 972:
#line 3769 "sql_yacc.yy"
    {}
    break;

  case 973:
#line 3774 "sql_yacc.yy"
    {
            SELECT_LEX *sel= Select;
            sel->select_limit= yyvsp[0].ulong_num;
            sel->offset_limit= 0L;
	    sel->explicit_limit= 1;
	  }
    break;

  case 974:
#line 3781 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->select_limit= yyvsp[0].ulong_num;
	    sel->offset_limit= yyvsp[-2].ulong_num;
	    sel->explicit_limit= 1;
	  }
    break;

  case 975:
#line 3788 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->select_limit= yyvsp[-2].ulong_num;
	    sel->offset_limit= yyvsp[0].ulong_num;
	    sel->explicit_limit= 1;
	  }
    break;

  case 976:
#line 3799 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->current_select->select_limit= HA_POS_ERROR;
	}
    break;

  case 977:
#line 3804 "sql_yacc.yy"
    {
	  SELECT_LEX *sel= Select;
	  sel->select_limit= (ha_rows) yyvsp[0].ulonglong_number;
	  sel->explicit_limit= 1;
	}
    break;

  case 978:
#line 3811 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 979:
#line 3812 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 980:
#line 3813 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 981:
#line 3814 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 982:
#line 3815 "sql_yacc.yy"
    { int error; yyval.ulong_num= (ulong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 983:
#line 3819 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 984:
#line 3820 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 985:
#line 3821 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 986:
#line 3822 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 987:
#line 3823 "sql_yacc.yy"
    { int error; yyval.ulonglong_number= (ulonglong) my_strtoll10(yyvsp[0].lex_str.str, (char**) 0, &error); }
    break;

  case 989:
#line 3829 "sql_yacc.yy"
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

  case 991:
#line 3849 "sql_yacc.yy"
    {}
    break;

  case 992:
#line 3850 "sql_yacc.yy"
    {}
    break;

  case 995:
#line 3858 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    if (add_proc_to_list(lex->thd, yyvsp[0].item))
	      YYABORT;
	    if (!yyvsp[0].item->name)
	      yyvsp[0].item->set_name(yyvsp[-1].simple_string,(uint) ((char*) lex->tok_end - yyvsp[-1].simple_string), YYTHD->charset());
	  }
    break;

  case 996:
#line 3869 "sql_yacc.yy"
    {
             LEX *lex=Lex;
	     if (!lex->describe && (!(lex->result= new select_dumpvar())))
	        YYABORT;
	   }
    break;

  case 997:
#line 3875 "sql_yacc.yy"
    {}
    break;

  case 999:
#line 3880 "sql_yacc.yy"
    {}
    break;

  case 1000:
#line 3884 "sql_yacc.yy"
    {
             LEX *lex=Lex;
	     if (lex->result && ((select_dumpvar *)lex->result)->var_list.push_back((LEX_STRING*) sql_memdup(&yyvsp[0].lex_str,sizeof(LEX_STRING))))
	       YYABORT;
	   }
    break;

  case 1001:
#line 3893 "sql_yacc.yy"
    {
          LEX *lex= Lex;
          lex->uncacheable(UNCACHEABLE_SIDEEFFECT);
          if (!(lex->exchange= new sql_exchange(yyvsp[0].lex_str.str, 0)) ||
              !(lex->result= new select_export(lex->exchange)))
            YYABORT;
	}
    break;

  case 1003:
#line 3902 "sql_yacc.yy"
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

  case 1004:
#line 3914 "sql_yacc.yy"
    {
	  Lex->uncacheable(UNCACHEABLE_SIDEEFFECT);
	}
    break;

  case 1005:
#line 3924 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_DO;
	  mysql_init_select(lex);
	}
    break;

  case 1006:
#line 3930 "sql_yacc.yy"
    {
	  Lex->insert_list= yyvsp[0].item_list;
	}
    break;

  case 1007:
#line 3941 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_DROP_TABLE;
	  lex->drop_temporary= yyvsp[-4].num;
	  lex->drop_if_exists= yyvsp[-2].num;
	}
    break;

  case 1008:
#line 3947 "sql_yacc.yy"
    {}
    break;

  case 1009:
#line 3948 "sql_yacc.yy"
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

  case 1010:
#line 3959 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_DROP_DB;
	    lex->drop_if_exists=yyvsp[-1].num;
	    lex->name=yyvsp[0].lex_str.str;
	 }
    break;

  case 1011:
#line 3966 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command = SQLCOM_DROP_FUNCTION;
	    lex->udf.name = yyvsp[0].lex_str;
	  }
    break;

  case 1012:
#line 3972 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command = SQLCOM_DROP_USER;
	    lex->users_list.empty();
	  }
    break;

  case 1013:
#line 3978 "sql_yacc.yy"
    {}
    break;

  case 1016:
#line 3988 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, yyvsp[0].table, NULL, TL_OPTION_UPDATING))
	    YYABORT;
	}
    break;

  case 1017:
#line 3995 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 1018:
#line 3996 "sql_yacc.yy"
    { yyval.num= 1; }
    break;

  case 1019:
#line 4000 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 1020:
#line 4001 "sql_yacc.yy"
    { yyval.num= 1; }
    break;

  case 1021:
#line 4009 "sql_yacc.yy"
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

  case 1022:
#line 4019 "sql_yacc.yy"
    {
	  Select->set_lock_for_tables(yyvsp[-2].lock_type);
	  Lex->current_select= &Lex->select_lex;
	}
    break;

  case 1023:
#line 4024 "sql_yacc.yy"
    {}
    break;

  case 1024:
#line 4029 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_REPLACE;
	  lex->duplicates= DUP_REPLACE;
          mysql_init_select(lex);
	  lex->select_lex.resolve_mode= SELECT_LEX::INSERT_MODE;
	}
    break;

  case 1025:
#line 4037 "sql_yacc.yy"
    {
	  Select->set_lock_for_tables(yyvsp[-1].lock_type);
	  Lex->current_select= &Lex->select_lex;
	}
    break;

  case 1026:
#line 4042 "sql_yacc.yy"
    {}
    break;

  case 1027:
#line 4043 "sql_yacc.yy"
    {}
    break;

  case 1028:
#line 4047 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_CONCURRENT_INSERT; }
    break;

  case 1029:
#line 4048 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_LOW_PRIORITY; }
    break;

  case 1030:
#line 4049 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_DELAYED; }
    break;

  case 1031:
#line 4050 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE; }
    break;

  case 1032:
#line 4054 "sql_yacc.yy"
    { yyval.lock_type= yyvsp[0].lock_type; }
    break;

  case 1033:
#line 4055 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_DELAYED; }
    break;

  case 1034:
#line 4058 "sql_yacc.yy"
    {}
    break;

  case 1035:
#line 4059 "sql_yacc.yy"
    {}
    break;

  case 1036:
#line 4063 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->field_list.empty();
	  lex->many_values.empty();
	  lex->insert_list=0;
	}
    break;

  case 1037:
#line 4071 "sql_yacc.yy"
    {}
    break;

  case 1038:
#line 4072 "sql_yacc.yy"
    {}
    break;

  case 1039:
#line 4073 "sql_yacc.yy"
    {}
    break;

  case 1040:
#line 4075 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (!(lex->insert_list = new List_item) ||
		lex->many_values.push_back(lex->insert_list))
	      YYABORT;
	   }
    break;

  case 1042:
#line 4084 "sql_yacc.yy"
    { }
    break;

  case 1043:
#line 4085 "sql_yacc.yy"
    { }
    break;

  case 1044:
#line 4086 "sql_yacc.yy"
    { }
    break;

  case 1045:
#line 4089 "sql_yacc.yy"
    { Lex->field_list.push_back(yyvsp[0].item); }
    break;

  case 1046:
#line 4090 "sql_yacc.yy"
    { Lex->field_list.push_back(yyvsp[0].item); }
    break;

  case 1047:
#line 4093 "sql_yacc.yy"
    {}
    break;

  case 1048:
#line 4094 "sql_yacc.yy"
    {}
    break;

  case 1049:
#line 4095 "sql_yacc.yy"
    { Select->set_braces(0);}
    break;

  case 1050:
#line 4095 "sql_yacc.yy"
    {}
    break;

  case 1051:
#line 4096 "sql_yacc.yy"
    { Select->set_braces(1);}
    break;

  case 1052:
#line 4096 "sql_yacc.yy"
    {}
    break;

  case 1057:
#line 4110 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->field_list.push_back(yyvsp[-2].item) ||
	      lex->insert_list->push_back(yyvsp[0].item))
	    YYABORT;
	 }
    break;

  case 1058:
#line 4117 "sql_yacc.yy"
    {}
    break;

  case 1059:
#line 4118 "sql_yacc.yy"
    {}
    break;

  case 1060:
#line 4122 "sql_yacc.yy"
    {}
    break;

  case 1061:
#line 4123 "sql_yacc.yy"
    {}
    break;

  case 1062:
#line 4128 "sql_yacc.yy"
    {
	    if (!(Lex->insert_list = new List_item))
	      YYABORT;
	 }
    break;

  case 1063:
#line 4133 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->many_values.push_back(lex->insert_list))
	    YYABORT;
	 }
    break;

  case 1064:
#line 4140 "sql_yacc.yy"
    {}
    break;

  case 1066:
#line 4145 "sql_yacc.yy"
    {
	  if (Lex->insert_list->push_back(yyvsp[0].item))
	    YYABORT;
	}
    break;

  case 1067:
#line 4150 "sql_yacc.yy"
    {
	    if (Lex->insert_list->push_back(yyvsp[0].item))
	      YYABORT;
	  }
    break;

  case 1068:
#line 4157 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item;}
    break;

  case 1069:
#line 4158 "sql_yacc.yy"
    {yyval.item= new Item_default_value(); }
    break;

  case 1071:
#line 4163 "sql_yacc.yy"
    { Lex->duplicates= DUP_UPDATE; }
    break;

  case 1073:
#line 4171 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  mysql_init_select(lex);
          lex->sql_command= SQLCOM_UPDATE;
	  lex->lock_option= TL_UNLOCK; 	/* Will be set later */
	  lex->duplicates= DUP_ERROR; 
        }
    break;

  case 1074:
#line 4180 "sql_yacc.yy"
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
#line 4197 "sql_yacc.yy"
    {}
    break;

  case 1078:
#line 4206 "sql_yacc.yy"
    {
	  if (add_item_to_list(YYTHD, yyvsp[-2].item) || add_value_to_list(YYTHD, yyvsp[0].item))
	    YYABORT;
	}
    break;

  case 1081:
#line 4217 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  if (lex->update_list.push_back(yyvsp[-2].item) || 
	      lex->value_list.push_back(yyvsp[0].item))
	    YYABORT;
	}
    break;

  case 1082:
#line 4225 "sql_yacc.yy"
    { yyval.lock_type= YYTHD->update_lock_default; }
    break;

  case 1083:
#line 4226 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_LOW_PRIORITY; }
    break;

  case 1084:
#line 4232 "sql_yacc.yy"
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
#line 4240 "sql_yacc.yy"
    {}
    break;

  case 1086:
#line 4245 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, yyvsp[0].table, NULL, TL_OPTION_UPDATING,
					 Lex->lock_option))
	    YYABORT;
	}
    break;

  case 1087:
#line 4251 "sql_yacc.yy"
    {}
    break;

  case 1088:
#line 4253 "sql_yacc.yy"
    { mysql_init_multi_delete(Lex); }
    break;

  case 1090:
#line 4256 "sql_yacc.yy"
    { mysql_init_multi_delete(Lex); }
    break;

  case 1091:
#line 4258 "sql_yacc.yy"
    {}
    break;

  case 1092:
#line 4262 "sql_yacc.yy"
    {}
    break;

  case 1093:
#line 4263 "sql_yacc.yy"
    {}
    break;

  case 1094:
#line 4267 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, new Table_ident(yyvsp[-2].lex_str), yyvsp[0].lex_str_ptr,
					 TL_OPTION_UPDATING, Lex->lock_option))
	    YYABORT;
        }
    break;

  case 1095:
#line 4273 "sql_yacc.yy"
    {
	    if (!Select->add_table_to_list(YYTHD,
					   new Table_ident(YYTHD, yyvsp[-4].lex_str, yyvsp[-2].lex_str, 0),
					   yyvsp[0].lex_str_ptr, TL_OPTION_UPDATING,
					   Lex->lock_option))
	      YYABORT;
	  }
    break;

  case 1096:
#line 4283 "sql_yacc.yy"
    {}
    break;

  case 1097:
#line 4284 "sql_yacc.yy"
    {}
    break;

  case 1098:
#line 4288 "sql_yacc.yy"
    {}
    break;

  case 1099:
#line 4289 "sql_yacc.yy"
    {}
    break;

  case 1100:
#line 4292 "sql_yacc.yy"
    { Select->options|= OPTION_QUICK; }
    break;

  case 1101:
#line 4293 "sql_yacc.yy"
    { Lex->lock_option= TL_WRITE_LOW_PRIORITY; }
    break;

  case 1102:
#line 4294 "sql_yacc.yy"
    { Lex->ignore= 1; }
    break;

  case 1103:
#line 4298 "sql_yacc.yy"
    {
	  LEX* lex= Lex;
	  lex->sql_command= SQLCOM_TRUNCATE;
	  lex->select_lex.options= 0;
	  lex->select_lex.init_order();
	}
    break;

  case 1106:
#line 4313 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->wild=0;
	  bzero((char*) &lex->create_info,sizeof(lex->create_info));
	}
    break;

  case 1107:
#line 4319 "sql_yacc.yy"
    {}
    break;

  case 1108:
#line 4324 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_DATABASES; }
    break;

  case 1109:
#line 4326 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->sql_command= SQLCOM_SHOW_TABLES;
	    lex->select_lex.db= yyvsp[-1].simple_string;
	   }
    break;

  case 1110:
#line 4332 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->sql_command= SQLCOM_SHOW_TABLES;
	    lex->describe= DESCRIBE_EXTENDED;
	    lex->select_lex.db= yyvsp[-1].simple_string;
	  }
    break;

  case 1111:
#line 4339 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->sql_command= SQLCOM_SHOW_OPEN_TABLES;
	    lex->select_lex.db= yyvsp[-1].simple_string;
	  }
    break;

  case 1112:
#line 4345 "sql_yacc.yy"
    { Lex->create_info.db_type= yyvsp[0].db_type; }
    break;

  case 1114:
#line 4348 "sql_yacc.yy"
    {
	    Lex->sql_command= SQLCOM_SHOW_FIELDS;
	    if (yyvsp[-1].simple_string)
	      yyvsp[-2].table->change_db(yyvsp[-1].simple_string);
	    if (!Select->add_table_to_list(YYTHD, yyvsp[-2].table, NULL, 0))
	      YYABORT;
	  }
    break;

  case 1115:
#line 4359 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_NEW_MASTER;
	    Lex->mi.log_file_name = yyvsp[-8].lex_str.str;
	    Lex->mi.pos = yyvsp[-4].ulonglong_number;
	    Lex->mi.server_id = yyvsp[0].ulong_num;
          }
    break;

  case 1116:
#line 4366 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_BINLOGS;
          }
    break;

  case 1117:
#line 4370 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_SLAVE_HOSTS;
          }
    break;

  case 1118:
#line 4374 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    lex->sql_command= SQLCOM_SHOW_BINLOG_EVENTS;
          }
    break;

  case 1120:
#line 4379 "sql_yacc.yy"
    {
	    Lex->sql_command= SQLCOM_SHOW_KEYS;
	    if (yyvsp[0].simple_string)
	      yyvsp[-1].table->change_db(yyvsp[0].simple_string);
	    if (!Select->add_table_to_list(YYTHD, yyvsp[-1].table, NULL, 0))
	      YYABORT;
	  }
    break;

  case 1121:
#line 4387 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_COLUMN_TYPES;
	  }
    break;

  case 1122:
#line 4392 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_STORAGE_ENGINES;
	    WARN_DEPRECATED("SHOW TABLE TYPES", "SHOW [STORAGE] ENGINES");
	  }
    break;

  case 1123:
#line 4398 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_STORAGE_ENGINES;
	  }
    break;

  case 1124:
#line 4403 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_PRIVILEGES;
	  }
    break;

  case 1125:
#line 4408 "sql_yacc.yy"
    { (void) create_select_for_variable("warning_count"); }
    break;

  case 1126:
#line 4410 "sql_yacc.yy"
    { (void) create_select_for_variable("error_count"); }
    break;

  case 1127:
#line 4412 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_SHOW_WARNS;}
    break;

  case 1128:
#line 4414 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_SHOW_ERRORS;}
    break;

  case 1129:
#line 4416 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_STATUS; }
    break;

  case 1130:
#line 4418 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_SHOW_INNODB_STATUS; WARN_DEPRECATED("SHOW INNODB STATUS", "SHOW ENGINE INNODB STATUS"); }
    break;

  case 1131:
#line 4420 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_PROCESSLIST;}
    break;

  case 1132:
#line 4422 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    thd->lex->sql_command= SQLCOM_SHOW_VARIABLES;
	    thd->lex->option_type= (enum_var_type) yyvsp[-2].num;
	  }
    break;

  case 1133:
#line 4428 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_CHARSETS; }
    break;

  case 1134:
#line 4430 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_COLLATIONS; }
    break;

  case 1135:
#line 4432 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_LOGS; WARN_DEPRECATED("SHOW BDB LOGS", "SHOW ENGINE BDB LOGS"); }
    break;

  case 1136:
#line 4434 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_LOGS; WARN_DEPRECATED("SHOW LOGS", "SHOW ENGINE BDB LOGS"); }
    break;

  case 1137:
#line 4436 "sql_yacc.yy"
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
#line 4459 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_GRANTS;
	    lex->grant_user=yyvsp[0].lex_user;
	    lex->grant_user->password.str=NullS;
	  }
    break;

  case 1139:
#line 4466 "sql_yacc.yy"
    {
	    Lex->sql_command=SQLCOM_SHOW_CREATE_DB;
	    Lex->create_info.options=yyvsp[-1].num;
	    Lex->name=yyvsp[0].lex_str.str;
	  }
    break;

  case 1140:
#line 4472 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_CREATE;
	    if (!Select->add_table_to_list(YYTHD, yyvsp[0].table, NULL,0))
	      YYABORT;
	  }
    break;

  case 1141:
#line 4478 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_MASTER_STAT;
          }
    break;

  case 1142:
#line 4482 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_SLAVE_STAT;
          }
    break;

  case 1143:
#line 4488 "sql_yacc.yy"
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
#line 4499 "sql_yacc.yy"
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
#line 4519 "sql_yacc.yy"
    { yyval.simple_string= 0; }
    break;

  case 1150:
#line 4520 "sql_yacc.yy"
    { yyval.simple_string= yyvsp[0].lex_str.str; }
    break;

  case 1152:
#line 4525 "sql_yacc.yy"
    { Lex->wild=  new (YYTHD->mem_root) String(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length,
                                                      system_charset_info); }
    break;

  case 1153:
#line 4529 "sql_yacc.yy"
    { Lex->verbose=0; }
    break;

  case 1154:
#line 4530 "sql_yacc.yy"
    { Lex->verbose=1; }
    break;

  case 1157:
#line 4537 "sql_yacc.yy"
    { Lex->mi.log_file_name = 0; }
    break;

  case 1158:
#line 4538 "sql_yacc.yy"
    { Lex->mi.log_file_name = yyvsp[0].lex_str.str; }
    break;

  case 1159:
#line 4541 "sql_yacc.yy"
    { Lex->mi.pos = 4; /* skip magic number */ }
    break;

  case 1160:
#line 4542 "sql_yacc.yy"
    { Lex->mi.pos = yyvsp[0].ulonglong_number; }
    break;

  case 1161:
#line 4548 "sql_yacc.yy"
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
#line 4556 "sql_yacc.yy"
    {}
    break;

  case 1163:
#line 4558 "sql_yacc.yy"
    { Lex->describe|= DESCRIBE_NORMAL; }
    break;

  case 1164:
#line 4560 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->select_lex.options|= SELECT_DESCRIBE;
	  }
    break;

  case 1167:
#line 4571 "sql_yacc.yy"
    {}
    break;

  case 1168:
#line 4572 "sql_yacc.yy"
    { Lex->describe|= DESCRIBE_EXTENDED; }
    break;

  case 1169:
#line 4576 "sql_yacc.yy"
    {}
    break;

  case 1170:
#line 4577 "sql_yacc.yy"
    { Lex->wild= yyvsp[0].string; }
    break;

  case 1171:
#line 4579 "sql_yacc.yy"
    { Lex->wild= new (YYTHD->mem_root) String((const char*) yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,system_charset_info); }
    break;

  case 1172:
#line 4586 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_FLUSH; lex->type=0;
          lex->no_write_to_binlog= yyvsp[0].num;
	}
    break;

  case 1173:
#line 4592 "sql_yacc.yy"
    {}
    break;

  case 1176:
#line 4600 "sql_yacc.yy"
    { Lex->type|= REFRESH_TABLES; }
    break;

  case 1177:
#line 4600 "sql_yacc.yy"
    {}
    break;

  case 1178:
#line 4601 "sql_yacc.yy"
    { Lex->type|= REFRESH_TABLES | REFRESH_READ_LOCK; }
    break;

  case 1179:
#line 4602 "sql_yacc.yy"
    { Lex->type|= REFRESH_QUERY_CACHE_FREE; }
    break;

  case 1180:
#line 4603 "sql_yacc.yy"
    { Lex->type|= REFRESH_HOSTS; }
    break;

  case 1181:
#line 4604 "sql_yacc.yy"
    { Lex->type|= REFRESH_GRANT; }
    break;

  case 1182:
#line 4605 "sql_yacc.yy"
    { Lex->type|= REFRESH_LOG; }
    break;

  case 1183:
#line 4606 "sql_yacc.yy"
    { Lex->type|= REFRESH_STATUS; }
    break;

  case 1184:
#line 4607 "sql_yacc.yy"
    { Lex->type|= REFRESH_SLAVE; }
    break;

  case 1185:
#line 4608 "sql_yacc.yy"
    { Lex->type|= REFRESH_MASTER; }
    break;

  case 1186:
#line 4609 "sql_yacc.yy"
    { Lex->type|= REFRESH_DES_KEY_FILE; }
    break;

  case 1187:
#line 4610 "sql_yacc.yy"
    { Lex->type|= REFRESH_USER_RESOURCES; }
    break;

  case 1188:
#line 4613 "sql_yacc.yy"
    {;}
    break;

  case 1189:
#line 4614 "sql_yacc.yy"
    {;}
    break;

  case 1190:
#line 4618 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_RESET; lex->type=0;
	}
    break;

  case 1191:
#line 4622 "sql_yacc.yy"
    {}
    break;

  case 1194:
#line 4630 "sql_yacc.yy"
    { Lex->type|= REFRESH_SLAVE; }
    break;

  case 1195:
#line 4631 "sql_yacc.yy"
    { Lex->type|= REFRESH_MASTER; }
    break;

  case 1196:
#line 4632 "sql_yacc.yy"
    { Lex->type|= REFRESH_QUERY_CACHE;}
    break;

  case 1197:
#line 4636 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->type=0;
	}
    break;

  case 1198:
#line 4640 "sql_yacc.yy"
    {}
    break;

  case 1200:
#line 4649 "sql_yacc.yy"
    {
	   Lex->sql_command = SQLCOM_PURGE;
	   Lex->to_log = yyvsp[0].lex_str.str;
        }
    break;

  case 1201:
#line 4654 "sql_yacc.yy"
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
#line 4675 "sql_yacc.yy"
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
#line 4689 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command=SQLCOM_CHANGE_DB;
	  lex->select_lex.db= yyvsp[0].lex_str.str;
	}
    break;

  case 1204:
#line 4698 "sql_yacc.yy"
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
#line 4711 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, yyvsp[-4].table, NULL, TL_OPTION_UPDATING))
	    YYABORT;
	}
    break;

  case 1206:
#line 4717 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_LOAD_MASTER_TABLE;
	  if (!Select->add_table_to_list(YYTHD, yyvsp[-2].table, NULL, TL_OPTION_UPDATING))
	    YYABORT;

        }
    break;

  case 1207:
#line 4725 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_LOAD_MASTER_DATA;
        }
    break;

  case 1208:
#line 4730 "sql_yacc.yy"
    { yyval.num=0;}
    break;

  case 1209:
#line 4731 "sql_yacc.yy"
    { yyval.num=1;}
    break;

  case 1210:
#line 4734 "sql_yacc.yy"
    { yyval.lock_type= YYTHD->update_lock_default; }
    break;

  case 1211:
#line 4735 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_CONCURRENT_INSERT ; }
    break;

  case 1212:
#line 4736 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_LOW_PRIORITY; }
    break;

  case 1213:
#line 4740 "sql_yacc.yy"
    { Lex->duplicates=DUP_ERROR; }
    break;

  case 1214:
#line 4741 "sql_yacc.yy"
    { Lex->duplicates=DUP_REPLACE; }
    break;

  case 1215:
#line 4742 "sql_yacc.yy"
    { Lex->ignore= 1; }
    break;

  case 1220:
#line 4754 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->field_term= yyvsp[0].string;
          }
    break;

  case 1221:
#line 4759 "sql_yacc.yy"
    {
            LEX *lex= Lex;
            DBUG_ASSERT(lex->exchange);
            lex->exchange->enclosed= yyvsp[0].string;
            lex->exchange->opt_enclosed= 1;
	  }
    break;

  case 1222:
#line 4766 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->enclosed= yyvsp[0].string;
          }
    break;

  case 1223:
#line 4771 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->escaped= yyvsp[0].string;
          }
    break;

  case 1228:
#line 4786 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->line_term= yyvsp[0].string;
          }
    break;

  case 1229:
#line 4791 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->line_start= yyvsp[0].string;
          }
    break;

  case 1231:
#line 4799 "sql_yacc.yy"
    {
            DBUG_ASSERT(Lex->exchange);
            Lex->exchange->skip_lines= atol(yyvsp[-1].lex_str.str);
          }
    break;

  case 1232:
#line 4808 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  yyval.item = new Item_string(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,thd->variables.collation_connection);
	}
    break;

  case 1233:
#line 4813 "sql_yacc.yy"
    { yyval.item=  new Item_string(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,national_charset_info); }
    break;

  case 1234:
#line 4815 "sql_yacc.yy"
    { yyval.item = new Item_string(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,Lex->charset); }
    break;

  case 1235:
#line 4817 "sql_yacc.yy"
    { ((Item_string*) yyvsp[-1].item)->append(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length); }
    break;

  case 1236:
#line 4822 "sql_yacc.yy"
    { yyval.string=  new (YYTHD->mem_root) String(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length,YYTHD->variables.collation_connection); }
    break;

  case 1237:
#line 4824 "sql_yacc.yy"
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
#line 4838 "sql_yacc.yy"
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
#line 4860 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 1240:
#line 4861 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item_num; }
    break;

  case 1241:
#line 4863 "sql_yacc.yy"
    {
	    yyvsp[0].item_num->max_length++;
	    yyval.item= yyvsp[0].item_num->neg();
	  }
    break;

  case 1242:
#line 4871 "sql_yacc.yy"
    { yyval.item =	yyvsp[0].item; }
    break;

  case 1243:
#line 4872 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item_num; }
    break;

  case 1244:
#line 4873 "sql_yacc.yy"
    { yyval.item =	new Item_null();
			  Lex->next_state=MY_LEX_OPERATOR_OR_IDENT;}
    break;

  case 1245:
#line 4875 "sql_yacc.yy"
    { yyval.item =	new Item_varbinary(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length);}
    break;

  case 1246:
#line 4877 "sql_yacc.yy"
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

  case 1247:
#line 4890 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 1248:
#line 4891 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 1249:
#line 4892 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 1250:
#line 4895 "sql_yacc.yy"
    { int error; yyval.item_num = new Item_int(yyvsp[0].lex_str.str, (longlong) my_strtoll10(yyvsp[0].lex_str.str, NULL, &error), yyvsp[0].lex_str.length); }
    break;

  case 1251:
#line 4896 "sql_yacc.yy"
    { int error; yyval.item_num = new Item_int(yyvsp[0].lex_str.str, (longlong) my_strtoll10(yyvsp[0].lex_str.str, NULL, &error), yyvsp[0].lex_str.length); }
    break;

  case 1252:
#line 4897 "sql_yacc.yy"
    { yyval.item_num =	new Item_uint(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length); }
    break;

  case 1253:
#line 4898 "sql_yacc.yy"
    { yyval.item_num =	new Item_real(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length); }
    break;

  case 1254:
#line 4899 "sql_yacc.yy"
    { yyval.item_num =	new Item_float(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length); }
    break;

  case 1255:
#line 4907 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 1256:
#line 4908 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 1257:
#line 4912 "sql_yacc.yy"
    {
	  yyval.item = new Item_field(NullS,yyvsp[-2].lex_str.str,"*");
	  Lex->current_select->with_wild++;
	}
    break;

  case 1258:
#line 4917 "sql_yacc.yy"
    {
	  yyval.item = new Item_field((YYTHD->client_capabilities &
   			     CLIENT_NO_SCHEMA ? NullS : yyvsp[-4].lex_str.str),
			     yyvsp[-2].lex_str.str,"*");
	  Lex->current_select->with_wild++;
	}
    break;

  case 1259:
#line 4926 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 1260:
#line 4930 "sql_yacc.yy"
    {
	  SELECT_LEX *sel=Select;
	  yyval.item= (sel->parsing_place != IN_HAVING ||
	       sel->get_in_sum_expr() > 0) ?
              (Item*) new Item_field(NullS,NullS,yyvsp[0].lex_str.str) :
	      (Item*) new Item_ref(NullS, NullS, yyvsp[0].lex_str.str);
	}
    break;

  case 1261:
#line 4938 "sql_yacc.yy"
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

  case 1262:
#line 4954 "sql_yacc.yy"
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

  case 1263:
#line 4970 "sql_yacc.yy"
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

  case 1264:
#line 4992 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1265:
#line 4993 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1266:
#line 4994 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1267:
#line 4997 "sql_yacc.yy"
    { yyval.table=new Table_ident(yyvsp[0].lex_str); }
    break;

  case 1268:
#line 4998 "sql_yacc.yy"
    { yyval.table=new Table_ident(YYTHD, yyvsp[-2].lex_str,yyvsp[0].lex_str,0);}
    break;

  case 1269:
#line 4999 "sql_yacc.yy"
    { yyval.table=new Table_ident(yyvsp[0].lex_str);}
    break;

  case 1270:
#line 5003 "sql_yacc.yy"
    { LEX_STRING db={(char*) any_db,3}; yyval.table=new Table_ident(YYTHD, db,yyvsp[0].lex_str,0); }
    break;

  case 1271:
#line 5007 "sql_yacc.yy"
    { yyval.lex_str= yyvsp[0].lex_str; }
    break;

  case 1272:
#line 5009 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    if (thd->charset_is_system_charset)
            {
              CHARSET_INFO *cs= system_charset_info;
              uint wlen= cs->cset->well_formed_len(cs, yyvsp[0].lex_str.str,
                                                   yyvsp[0].lex_str.str+yyvsp[0].lex_str.length,
                                                   yyvsp[0].lex_str.length);
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

  case 1273:
#line 5033 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  if (thd->charset_is_system_charset)
	    yyval.lex_str= yyvsp[0].lex_str;
	  else
	    thd->convert_string(&yyval.lex_str, system_charset_info,
				yyvsp[0].lex_str.str, yyvsp[0].lex_str.length, thd->charset());
	}
    break;

  case 1274:
#line 5045 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  if (thd->charset_is_collation_connection)
	    yyval.lex_str= yyvsp[0].lex_str;
	  else
	    thd->convert_string(&yyval.lex_str, thd->variables.collation_connection,
				yyvsp[0].lex_str.str, yyvsp[0].lex_str.length, thd->charset());
	}
    break;

  case 1275:
#line 5057 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 1276:
#line 5059 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  yyval.lex_str.str=    thd->strmake(yyvsp[0].symbol.str, yyvsp[0].symbol.length);
	  yyval.lex_str.length= yyvsp[0].symbol.length;
	}
    break;

  case 1277:
#line 5067 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1278:
#line 5068 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1279:
#line 5069 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 1280:
#line 5073 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  if (!(yyval.lex_user=(LEX_USER*) thd->alloc(sizeof(st_lex_user))))
	    YYABORT;
	  yyval.lex_user->user = yyvsp[0].lex_str;
	  yyval.lex_user->host.str= (char *) "%";
	  yyval.lex_user->host.length= 1;
	}
    break;

  case 1281:
#line 5082 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    if (!(yyval.lex_user=(LEX_USER*) thd->alloc(sizeof(st_lex_user))))
	      YYABORT;
	    yyval.lex_user->user = yyvsp[-2].lex_str; yyval.lex_user->host=yyvsp[0].lex_str;
	  }
    break;

  case 1282:
#line 5089 "sql_yacc.yy"
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

  case 1283:
#line 5110 "sql_yacc.yy"
    {}
    break;

  case 1284:
#line 5111 "sql_yacc.yy"
    {}
    break;

  case 1285:
#line 5112 "sql_yacc.yy"
    {}
    break;

  case 1286:
#line 5113 "sql_yacc.yy"
    {}
    break;

  case 1287:
#line 5114 "sql_yacc.yy"
    {}
    break;

  case 1288:
#line 5115 "sql_yacc.yy"
    {}
    break;

  case 1289:
#line 5116 "sql_yacc.yy"
    {}
    break;

  case 1290:
#line 5117 "sql_yacc.yy"
    {}
    break;

  case 1291:
#line 5118 "sql_yacc.yy"
    {}
    break;

  case 1292:
#line 5119 "sql_yacc.yy"
    {}
    break;

  case 1293:
#line 5120 "sql_yacc.yy"
    {}
    break;

  case 1294:
#line 5121 "sql_yacc.yy"
    {}
    break;

  case 1295:
#line 5122 "sql_yacc.yy"
    {}
    break;

  case 1296:
#line 5123 "sql_yacc.yy"
    {}
    break;

  case 1297:
#line 5124 "sql_yacc.yy"
    {}
    break;

  case 1298:
#line 5125 "sql_yacc.yy"
    {}
    break;

  case 1299:
#line 5126 "sql_yacc.yy"
    {}
    break;

  case 1300:
#line 5127 "sql_yacc.yy"
    {}
    break;

  case 1301:
#line 5128 "sql_yacc.yy"
    {}
    break;

  case 1302:
#line 5129 "sql_yacc.yy"
    {}
    break;

  case 1303:
#line 5130 "sql_yacc.yy"
    {}
    break;

  case 1304:
#line 5131 "sql_yacc.yy"
    {}
    break;

  case 1305:
#line 5132 "sql_yacc.yy"
    {}
    break;

  case 1306:
#line 5133 "sql_yacc.yy"
    {}
    break;

  case 1307:
#line 5134 "sql_yacc.yy"
    {}
    break;

  case 1308:
#line 5135 "sql_yacc.yy"
    {}
    break;

  case 1309:
#line 5136 "sql_yacc.yy"
    {}
    break;

  case 1310:
#line 5137 "sql_yacc.yy"
    {}
    break;

  case 1311:
#line 5138 "sql_yacc.yy"
    {}
    break;

  case 1312:
#line 5139 "sql_yacc.yy"
    {}
    break;

  case 1313:
#line 5140 "sql_yacc.yy"
    {}
    break;

  case 1314:
#line 5141 "sql_yacc.yy"
    {}
    break;

  case 1315:
#line 5142 "sql_yacc.yy"
    {}
    break;

  case 1316:
#line 5143 "sql_yacc.yy"
    {}
    break;

  case 1317:
#line 5144 "sql_yacc.yy"
    {}
    break;

  case 1318:
#line 5145 "sql_yacc.yy"
    {}
    break;

  case 1319:
#line 5146 "sql_yacc.yy"
    {}
    break;

  case 1320:
#line 5147 "sql_yacc.yy"
    {}
    break;

  case 1321:
#line 5148 "sql_yacc.yy"
    {}
    break;

  case 1322:
#line 5149 "sql_yacc.yy"
    {}
    break;

  case 1323:
#line 5150 "sql_yacc.yy"
    {}
    break;

  case 1324:
#line 5151 "sql_yacc.yy"
    {}
    break;

  case 1325:
#line 5152 "sql_yacc.yy"
    {}
    break;

  case 1326:
#line 5153 "sql_yacc.yy"
    {}
    break;

  case 1327:
#line 5154 "sql_yacc.yy"
    {}
    break;

  case 1328:
#line 5155 "sql_yacc.yy"
    {}
    break;

  case 1329:
#line 5156 "sql_yacc.yy"
    {}
    break;

  case 1330:
#line 5157 "sql_yacc.yy"
    {}
    break;

  case 1331:
#line 5158 "sql_yacc.yy"
    {}
    break;

  case 1332:
#line 5159 "sql_yacc.yy"
    {}
    break;

  case 1333:
#line 5160 "sql_yacc.yy"
    {}
    break;

  case 1334:
#line 5161 "sql_yacc.yy"
    {}
    break;

  case 1335:
#line 5162 "sql_yacc.yy"
    {}
    break;

  case 1336:
#line 5163 "sql_yacc.yy"
    {}
    break;

  case 1337:
#line 5164 "sql_yacc.yy"
    {}
    break;

  case 1338:
#line 5165 "sql_yacc.yy"
    {}
    break;

  case 1339:
#line 5166 "sql_yacc.yy"
    {}
    break;

  case 1340:
#line 5167 "sql_yacc.yy"
    {}
    break;

  case 1341:
#line 5168 "sql_yacc.yy"
    {}
    break;

  case 1342:
#line 5169 "sql_yacc.yy"
    {}
    break;

  case 1343:
#line 5170 "sql_yacc.yy"
    {}
    break;

  case 1344:
#line 5171 "sql_yacc.yy"
    {}
    break;

  case 1345:
#line 5172 "sql_yacc.yy"
    {}
    break;

  case 1346:
#line 5173 "sql_yacc.yy"
    {}
    break;

  case 1347:
#line 5174 "sql_yacc.yy"
    {}
    break;

  case 1348:
#line 5175 "sql_yacc.yy"
    {}
    break;

  case 1349:
#line 5176 "sql_yacc.yy"
    {}
    break;

  case 1350:
#line 5177 "sql_yacc.yy"
    {}
    break;

  case 1351:
#line 5178 "sql_yacc.yy"
    {}
    break;

  case 1352:
#line 5179 "sql_yacc.yy"
    {}
    break;

  case 1353:
#line 5180 "sql_yacc.yy"
    {}
    break;

  case 1354:
#line 5181 "sql_yacc.yy"
    {}
    break;

  case 1355:
#line 5182 "sql_yacc.yy"
    {}
    break;

  case 1356:
#line 5183 "sql_yacc.yy"
    {}
    break;

  case 1357:
#line 5184 "sql_yacc.yy"
    {}
    break;

  case 1358:
#line 5185 "sql_yacc.yy"
    {}
    break;

  case 1359:
#line 5186 "sql_yacc.yy"
    {}
    break;

  case 1360:
#line 5187 "sql_yacc.yy"
    {}
    break;

  case 1361:
#line 5188 "sql_yacc.yy"
    {}
    break;

  case 1362:
#line 5189 "sql_yacc.yy"
    {}
    break;

  case 1363:
#line 5190 "sql_yacc.yy"
    {}
    break;

  case 1364:
#line 5191 "sql_yacc.yy"
    {}
    break;

  case 1365:
#line 5192 "sql_yacc.yy"
    {}
    break;

  case 1366:
#line 5193 "sql_yacc.yy"
    {}
    break;

  case 1367:
#line 5194 "sql_yacc.yy"
    {}
    break;

  case 1368:
#line 5195 "sql_yacc.yy"
    {}
    break;

  case 1369:
#line 5196 "sql_yacc.yy"
    {}
    break;

  case 1370:
#line 5197 "sql_yacc.yy"
    {}
    break;

  case 1371:
#line 5198 "sql_yacc.yy"
    {}
    break;

  case 1372:
#line 5199 "sql_yacc.yy"
    {}
    break;

  case 1373:
#line 5200 "sql_yacc.yy"
    {}
    break;

  case 1374:
#line 5201 "sql_yacc.yy"
    {}
    break;

  case 1375:
#line 5202 "sql_yacc.yy"
    {}
    break;

  case 1376:
#line 5203 "sql_yacc.yy"
    {}
    break;

  case 1377:
#line 5204 "sql_yacc.yy"
    {}
    break;

  case 1378:
#line 5205 "sql_yacc.yy"
    {}
    break;

  case 1379:
#line 5206 "sql_yacc.yy"
    {}
    break;

  case 1380:
#line 5207 "sql_yacc.yy"
    {}
    break;

  case 1381:
#line 5208 "sql_yacc.yy"
    {}
    break;

  case 1382:
#line 5209 "sql_yacc.yy"
    {}
    break;

  case 1383:
#line 5210 "sql_yacc.yy"
    {}
    break;

  case 1384:
#line 5211 "sql_yacc.yy"
    {}
    break;

  case 1385:
#line 5212 "sql_yacc.yy"
    {}
    break;

  case 1386:
#line 5213 "sql_yacc.yy"
    {}
    break;

  case 1387:
#line 5214 "sql_yacc.yy"
    {}
    break;

  case 1388:
#line 5215 "sql_yacc.yy"
    {}
    break;

  case 1389:
#line 5216 "sql_yacc.yy"
    {}
    break;

  case 1390:
#line 5217 "sql_yacc.yy"
    {}
    break;

  case 1391:
#line 5218 "sql_yacc.yy"
    {}
    break;

  case 1392:
#line 5219 "sql_yacc.yy"
    {}
    break;

  case 1393:
#line 5220 "sql_yacc.yy"
    {}
    break;

  case 1394:
#line 5221 "sql_yacc.yy"
    {}
    break;

  case 1395:
#line 5222 "sql_yacc.yy"
    {}
    break;

  case 1396:
#line 5223 "sql_yacc.yy"
    {}
    break;

  case 1397:
#line 5224 "sql_yacc.yy"
    {}
    break;

  case 1398:
#line 5225 "sql_yacc.yy"
    {}
    break;

  case 1399:
#line 5226 "sql_yacc.yy"
    {}
    break;

  case 1400:
#line 5227 "sql_yacc.yy"
    {}
    break;

  case 1401:
#line 5228 "sql_yacc.yy"
    {}
    break;

  case 1402:
#line 5229 "sql_yacc.yy"
    {}
    break;

  case 1403:
#line 5230 "sql_yacc.yy"
    {}
    break;

  case 1404:
#line 5231 "sql_yacc.yy"
    {}
    break;

  case 1405:
#line 5232 "sql_yacc.yy"
    {}
    break;

  case 1406:
#line 5233 "sql_yacc.yy"
    {}
    break;

  case 1407:
#line 5234 "sql_yacc.yy"
    {}
    break;

  case 1408:
#line 5235 "sql_yacc.yy"
    {}
    break;

  case 1409:
#line 5236 "sql_yacc.yy"
    {}
    break;

  case 1410:
#line 5237 "sql_yacc.yy"
    {}
    break;

  case 1411:
#line 5238 "sql_yacc.yy"
    {}
    break;

  case 1412:
#line 5239 "sql_yacc.yy"
    {}
    break;

  case 1413:
#line 5240 "sql_yacc.yy"
    {}
    break;

  case 1414:
#line 5241 "sql_yacc.yy"
    {}
    break;

  case 1415:
#line 5242 "sql_yacc.yy"
    {}
    break;

  case 1416:
#line 5243 "sql_yacc.yy"
    {}
    break;

  case 1417:
#line 5244 "sql_yacc.yy"
    {}
    break;

  case 1418:
#line 5245 "sql_yacc.yy"
    {}
    break;

  case 1419:
#line 5246 "sql_yacc.yy"
    {}
    break;

  case 1420:
#line 5247 "sql_yacc.yy"
    {}
    break;

  case 1421:
#line 5248 "sql_yacc.yy"
    {}
    break;

  case 1422:
#line 5249 "sql_yacc.yy"
    {}
    break;

  case 1423:
#line 5250 "sql_yacc.yy"
    {}
    break;

  case 1424:
#line 5251 "sql_yacc.yy"
    {}
    break;

  case 1425:
#line 5252 "sql_yacc.yy"
    {}
    break;

  case 1426:
#line 5253 "sql_yacc.yy"
    {}
    break;

  case 1427:
#line 5254 "sql_yacc.yy"
    {}
    break;

  case 1428:
#line 5255 "sql_yacc.yy"
    {}
    break;

  case 1429:
#line 5256 "sql_yacc.yy"
    {}
    break;

  case 1430:
#line 5257 "sql_yacc.yy"
    {}
    break;

  case 1431:
#line 5258 "sql_yacc.yy"
    {}
    break;

  case 1432:
#line 5259 "sql_yacc.yy"
    {}
    break;

  case 1433:
#line 5260 "sql_yacc.yy"
    {}
    break;

  case 1434:
#line 5261 "sql_yacc.yy"
    {}
    break;

  case 1435:
#line 5262 "sql_yacc.yy"
    {}
    break;

  case 1436:
#line 5263 "sql_yacc.yy"
    {}
    break;

  case 1437:
#line 5264 "sql_yacc.yy"
    {}
    break;

  case 1438:
#line 5265 "sql_yacc.yy"
    {}
    break;

  case 1439:
#line 5266 "sql_yacc.yy"
    {}
    break;

  case 1440:
#line 5267 "sql_yacc.yy"
    {}
    break;

  case 1441:
#line 5268 "sql_yacc.yy"
    {}
    break;

  case 1442:
#line 5269 "sql_yacc.yy"
    {}
    break;

  case 1443:
#line 5270 "sql_yacc.yy"
    {}
    break;

  case 1444:
#line 5271 "sql_yacc.yy"
    {}
    break;

  case 1445:
#line 5272 "sql_yacc.yy"
    {}
    break;

  case 1446:
#line 5273 "sql_yacc.yy"
    {}
    break;

  case 1447:
#line 5274 "sql_yacc.yy"
    {}
    break;

  case 1448:
#line 5275 "sql_yacc.yy"
    {}
    break;

  case 1449:
#line 5276 "sql_yacc.yy"
    {}
    break;

  case 1450:
#line 5277 "sql_yacc.yy"
    {}
    break;

  case 1451:
#line 5278 "sql_yacc.yy"
    {}
    break;

  case 1452:
#line 5279 "sql_yacc.yy"
    {}
    break;

  case 1453:
#line 5280 "sql_yacc.yy"
    {}
    break;

  case 1454:
#line 5281 "sql_yacc.yy"
    {}
    break;

  case 1455:
#line 5282 "sql_yacc.yy"
    {}
    break;

  case 1456:
#line 5283 "sql_yacc.yy"
    {}
    break;

  case 1457:
#line 5284 "sql_yacc.yy"
    {}
    break;

  case 1458:
#line 5285 "sql_yacc.yy"
    {}
    break;

  case 1459:
#line 5286 "sql_yacc.yy"
    {}
    break;

  case 1460:
#line 5287 "sql_yacc.yy"
    {}
    break;

  case 1461:
#line 5288 "sql_yacc.yy"
    {}
    break;

  case 1462:
#line 5289 "sql_yacc.yy"
    {}
    break;

  case 1463:
#line 5290 "sql_yacc.yy"
    {}
    break;

  case 1464:
#line 5291 "sql_yacc.yy"
    {}
    break;

  case 1465:
#line 5292 "sql_yacc.yy"
    {}
    break;

  case 1466:
#line 5293 "sql_yacc.yy"
    {}
    break;

  case 1467:
#line 5294 "sql_yacc.yy"
    {}
    break;

  case 1468:
#line 5295 "sql_yacc.yy"
    {}
    break;

  case 1469:
#line 5296 "sql_yacc.yy"
    {}
    break;

  case 1470:
#line 5297 "sql_yacc.yy"
    {}
    break;

  case 1471:
#line 5298 "sql_yacc.yy"
    {}
    break;

  case 1472:
#line 5299 "sql_yacc.yy"
    {}
    break;

  case 1473:
#line 5300 "sql_yacc.yy"
    {}
    break;

  case 1474:
#line 5301 "sql_yacc.yy"
    {}
    break;

  case 1475:
#line 5302 "sql_yacc.yy"
    {}
    break;

  case 1476:
#line 5303 "sql_yacc.yy"
    {}
    break;

  case 1477:
#line 5304 "sql_yacc.yy"
    {}
    break;

  case 1478:
#line 5305 "sql_yacc.yy"
    {}
    break;

  case 1479:
#line 5306 "sql_yacc.yy"
    {}
    break;

  case 1480:
#line 5307 "sql_yacc.yy"
    {}
    break;

  case 1481:
#line 5308 "sql_yacc.yy"
    {}
    break;

  case 1482:
#line 5309 "sql_yacc.yy"
    {}
    break;

  case 1483:
#line 5310 "sql_yacc.yy"
    {}
    break;

  case 1484:
#line 5311 "sql_yacc.yy"
    {}
    break;

  case 1485:
#line 5312 "sql_yacc.yy"
    {}
    break;

  case 1486:
#line 5313 "sql_yacc.yy"
    {}
    break;

  case 1487:
#line 5314 "sql_yacc.yy"
    {}
    break;

  case 1488:
#line 5315 "sql_yacc.yy"
    {}
    break;

  case 1489:
#line 5316 "sql_yacc.yy"
    {}
    break;

  case 1490:
#line 5317 "sql_yacc.yy"
    {}
    break;

  case 1491:
#line 5318 "sql_yacc.yy"
    {}
    break;

  case 1492:
#line 5325 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_SET_OPTION;
          mysql_init_select(lex);
	  lex->option_type=OPT_SESSION;
	  lex->var_list.empty();
          lex->one_shot_set= 0;
	}
    break;

  case 1493:
#line 5334 "sql_yacc.yy"
    {}
    break;

  case 1494:
#line 5338 "sql_yacc.yy"
    {}
    break;

  case 1495:
#line 5339 "sql_yacc.yy"
    {}
    break;

  case 1498:
#line 5346 "sql_yacc.yy"
    {}
    break;

  case 1499:
#line 5347 "sql_yacc.yy"
    { Lex->option_type= OPT_GLOBAL; }
    break;

  case 1500:
#line 5348 "sql_yacc.yy"
    { Lex->option_type= OPT_SESSION; }
    break;

  case 1501:
#line 5349 "sql_yacc.yy"
    { Lex->option_type= OPT_SESSION; }
    break;

  case 1502:
#line 5350 "sql_yacc.yy"
    { Lex->option_type= OPT_SESSION; Lex->one_shot_set= 1; }
    break;

  case 1503:
#line 5354 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1504:
#line 5355 "sql_yacc.yy"
    { yyval.num=OPT_GLOBAL; }
    break;

  case 1505:
#line 5356 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1506:
#line 5357 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1507:
#line 5361 "sql_yacc.yy"
    { yyval.num=OPT_DEFAULT; }
    break;

  case 1508:
#line 5362 "sql_yacc.yy"
    { yyval.num=OPT_GLOBAL; }
    break;

  case 1509:
#line 5363 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1510:
#line 5364 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1511:
#line 5369 "sql_yacc.yy"
    {
	  Lex->var_list.push_back(new set_var_user(new Item_func_set_user_var(yyvsp[-2].lex_str,yyvsp[0].item)));
	}
    break;

  case 1512:
#line 5373 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->var_list.push_back(new set_var(lex->option_type, yyvsp[-2].variable.var,
						&yyvsp[-2].variable.base_name, yyvsp[0].item));
	  }
    break;

  case 1513:
#line 5379 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->var_list.push_back(new set_var((enum_var_type) yyvsp[-3].num, yyvsp[-2].variable.var,
						&yyvsp[-2].variable.base_name, yyvsp[0].item));
	  }
    break;

  case 1514:
#line 5385 "sql_yacc.yy"
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

  case 1515:
#line 5396 "sql_yacc.yy"
    {
	  THD *thd= YYTHD;
	  LEX *lex= Lex;
	  yyvsp[0].charset= yyvsp[0].charset ? yyvsp[0].charset: global_system_variables.character_set_client;
	  lex->var_list.push_back(new set_var_collation_client(yyvsp[0].charset,thd->variables.collation_database,yyvsp[0].charset));
	}
    break;

  case 1516:
#line 5403 "sql_yacc.yy"
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

  case 1517:
#line 5416 "sql_yacc.yy"
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

  case 1518:
#line 5426 "sql_yacc.yy"
    {
	    Lex->var_list.push_back(new set_var_password(yyvsp[-2].lex_user,yyvsp[0].simple_string));
	  }
    break;

  case 1519:
#line 5433 "sql_yacc.yy"
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

  case 1520:
#line 5448 "sql_yacc.yy"
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

  case 1521:
#line 5463 "sql_yacc.yy"
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

  case 1522:
#line 5476 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_READ_UNCOMMITTED; }
    break;

  case 1523:
#line 5477 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_READ_COMMITTED; }
    break;

  case 1524:
#line 5478 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_REPEATABLE_READ; }
    break;

  case 1525:
#line 5479 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_SERIALIZABLE; }
    break;

  case 1526:
#line 5483 "sql_yacc.yy"
    { yyval.simple_string=yyvsp[0].lex_str.str;}
    break;

  case 1527:
#line 5485 "sql_yacc.yy"
    {
	    yyval.simple_string= yyvsp[-1].lex_str.length ? YYTHD->variables.old_passwords ?
	        Item_func_old_password::alloc(YYTHD, yyvsp[-1].lex_str.str) :
	        Item_func_password::alloc(YYTHD, yyvsp[-1].lex_str.str) :
	      yyvsp[-1].lex_str.str;
	  }
    break;

  case 1528:
#line 5492 "sql_yacc.yy"
    {
	    yyval.simple_string= yyvsp[-1].lex_str.length ? Item_func_old_password::alloc(YYTHD, yyvsp[-1].lex_str.str) :
	      yyvsp[-1].lex_str.str;
	  }
    break;

  case 1529:
#line 5500 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 1530:
#line 5501 "sql_yacc.yy"
    { yyval.item=0; }
    break;

  case 1531:
#line 5502 "sql_yacc.yy"
    { yyval.item=new Item_string("ON",  2, system_charset_info); }
    break;

  case 1532:
#line 5503 "sql_yacc.yy"
    { yyval.item=new Item_string("ALL", 3, system_charset_info); }
    break;

  case 1533:
#line 5504 "sql_yacc.yy"
    { yyval.item=new Item_string("binary", 6, system_charset_info); }
    break;

  case 1534:
#line 5512 "sql_yacc.yy"
    {
	  Lex->sql_command=SQLCOM_LOCK_TABLES;
	}
    break;

  case 1535:
#line 5516 "sql_yacc.yy"
    {}
    break;

  case 1540:
#line 5529 "sql_yacc.yy"
    {
	  if (!Select->add_table_to_list(YYTHD, yyvsp[-2].table, yyvsp[-1].lex_str_ptr, 0, (thr_lock_type) yyvsp[0].num))
	   YYABORT;
	}
    break;

  case 1541:
#line 5536 "sql_yacc.yy"
    { yyval.num=TL_READ_NO_INSERT; }
    break;

  case 1542:
#line 5537 "sql_yacc.yy"
    { yyval.num=YYTHD->update_lock_default; }
    break;

  case 1543:
#line 5538 "sql_yacc.yy"
    { yyval.num=TL_WRITE_LOW_PRIORITY; }
    break;

  case 1544:
#line 5539 "sql_yacc.yy"
    { yyval.num= TL_READ; }
    break;

  case 1545:
#line 5543 "sql_yacc.yy"
    { Lex->sql_command=SQLCOM_UNLOCK_TABLES; }
    break;

  case 1546:
#line 5553 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  lex->sql_command = SQLCOM_HA_OPEN;
	  if (!lex->current_select->add_table_to_list(lex->thd, yyvsp[-2].table, yyvsp[0].lex_str_ptr, 0))
	    YYABORT;
	}
    break;

  case 1547:
#line 5560 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  lex->sql_command = SQLCOM_HA_CLOSE;
	  if (!lex->current_select->add_table_to_list(lex->thd, yyvsp[-1].table, 0, 0))
	    YYABORT;
	}
    break;

  case 1548:
#line 5567 "sql_yacc.yy"
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

  case 1549:
#line 5576 "sql_yacc.yy"
    {}
    break;

  case 1550:
#line 5580 "sql_yacc.yy"
    { Lex->backup_dir= 0; }
    break;

  case 1551:
#line 5581 "sql_yacc.yy"
    { Lex->backup_dir= yyvsp[-1].lex_str.str; }
    break;

  case 1552:
#line 5585 "sql_yacc.yy"
    { Lex->ha_read_mode = RFIRST; }
    break;

  case 1553:
#line 5586 "sql_yacc.yy"
    { Lex->ha_read_mode = RNEXT;  }
    break;

  case 1554:
#line 5590 "sql_yacc.yy"
    { Lex->ha_read_mode = RFIRST; }
    break;

  case 1555:
#line 5591 "sql_yacc.yy"
    { Lex->ha_read_mode = RNEXT;  }
    break;

  case 1556:
#line 5592 "sql_yacc.yy"
    { Lex->ha_read_mode = RPREV;  }
    break;

  case 1557:
#line 5593 "sql_yacc.yy"
    { Lex->ha_read_mode = RLAST;  }
    break;

  case 1558:
#line 5595 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->ha_read_mode = RKEY;
	  lex->ha_rkey_mode=yyvsp[0].ha_rkey_mode;
	  if (!(lex->insert_list = new List_item))
	    YYABORT;
	}
    break;

  case 1559:
#line 5601 "sql_yacc.yy"
    { }
    break;

  case 1560:
#line 5605 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_KEY_EXACT;   }
    break;

  case 1561:
#line 5606 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_KEY_OR_NEXT; }
    break;

  case 1562:
#line 5607 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_KEY_OR_PREV; }
    break;

  case 1563:
#line 5608 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_AFTER_KEY;   }
    break;

  case 1564:
#line 5609 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_BEFORE_KEY;  }
    break;

  case 1565:
#line 5616 "sql_yacc.yy"
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

  case 1566:
#line 5628 "sql_yacc.yy"
    {}
    break;

  case 1567:
#line 5633 "sql_yacc.yy"
    {}
    break;

  case 1568:
#line 5636 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_REVOKE_ALL;
	}
    break;

  case 1569:
#line 5643 "sql_yacc.yy"
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

  case 1570:
#line 5656 "sql_yacc.yy"
    {}
    break;

  case 1571:
#line 5660 "sql_yacc.yy"
    {}
    break;

  case 1572:
#line 5661 "sql_yacc.yy"
    { Lex->grant = GLOBAL_ACLS;}
    break;

  case 1577:
#line 5674 "sql_yacc.yy"
    { Lex->which_columns = SELECT_ACL;}
    break;

  case 1578:
#line 5674 "sql_yacc.yy"
    {}
    break;

  case 1579:
#line 5675 "sql_yacc.yy"
    { Lex->which_columns = INSERT_ACL;}
    break;

  case 1580:
#line 5675 "sql_yacc.yy"
    {}
    break;

  case 1581:
#line 5676 "sql_yacc.yy"
    { Lex->which_columns = UPDATE_ACL; }
    break;

  case 1582:
#line 5676 "sql_yacc.yy"
    {}
    break;

  case 1583:
#line 5677 "sql_yacc.yy"
    { Lex->which_columns = REFERENCES_ACL;}
    break;

  case 1584:
#line 5677 "sql_yacc.yy"
    {}
    break;

  case 1585:
#line 5678 "sql_yacc.yy"
    { Lex->grant |= DELETE_ACL;}
    break;

  case 1586:
#line 5679 "sql_yacc.yy"
    {}
    break;

  case 1587:
#line 5680 "sql_yacc.yy"
    { Lex->grant |= INDEX_ACL;}
    break;

  case 1588:
#line 5681 "sql_yacc.yy"
    { Lex->grant |= ALTER_ACL;}
    break;

  case 1589:
#line 5682 "sql_yacc.yy"
    { Lex->grant |= CREATE_ACL;}
    break;

  case 1590:
#line 5683 "sql_yacc.yy"
    { Lex->grant |= DROP_ACL;}
    break;

  case 1591:
#line 5684 "sql_yacc.yy"
    { Lex->grant |= EXECUTE_ACL;}
    break;

  case 1592:
#line 5685 "sql_yacc.yy"
    { Lex->grant |= RELOAD_ACL;}
    break;

  case 1593:
#line 5686 "sql_yacc.yy"
    { Lex->grant |= SHUTDOWN_ACL;}
    break;

  case 1594:
#line 5687 "sql_yacc.yy"
    { Lex->grant |= PROCESS_ACL;}
    break;

  case 1595:
#line 5688 "sql_yacc.yy"
    { Lex->grant |= FILE_ACL;}
    break;

  case 1596:
#line 5689 "sql_yacc.yy"
    { Lex->grant |= GRANT_ACL;}
    break;

  case 1597:
#line 5690 "sql_yacc.yy"
    { Lex->grant |= SHOW_DB_ACL;}
    break;

  case 1598:
#line 5691 "sql_yacc.yy"
    { Lex->grant |= SUPER_ACL;}
    break;

  case 1599:
#line 5692 "sql_yacc.yy"
    { Lex->grant |= CREATE_TMP_ACL;}
    break;

  case 1600:
#line 5693 "sql_yacc.yy"
    { Lex->grant |= LOCK_TABLES_ACL; }
    break;

  case 1601:
#line 5694 "sql_yacc.yy"
    { Lex->grant |= REPL_SLAVE_ACL;}
    break;

  case 1602:
#line 5695 "sql_yacc.yy"
    { Lex->grant |= REPL_CLIENT_ACL;}
    break;

  case 1603:
#line 5700 "sql_yacc.yy"
    {}
    break;

  case 1604:
#line 5701 "sql_yacc.yy"
    {}
    break;

  case 1607:
#line 5711 "sql_yacc.yy"
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

  case 1608:
#line 5721 "sql_yacc.yy"
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

  case 1609:
#line 5731 "sql_yacc.yy"
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

  case 1610:
#line 5744 "sql_yacc.yy"
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

  case 1611:
#line 5756 "sql_yacc.yy"
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

  case 1612:
#line 5768 "sql_yacc.yy"
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

  case 1613:
#line 5780 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (!lex->current_select->add_table_to_list(lex->thd, yyvsp[0].table,NULL,0))
	      YYABORT;
	    if (lex->grant == GLOBAL_ACLS)
	      lex->grant =  TABLE_ACLS & ~GRANT_ACL;
	  }
    break;

  case 1614:
#line 5791 "sql_yacc.yy"
    { if (Lex->users_list.push_back(yyvsp[0].lex_user)) YYABORT;}
    break;

  case 1615:
#line 5793 "sql_yacc.yy"
    {
	    if (Lex->users_list.push_back(yyvsp[0].lex_user))
	      YYABORT;
	  }
    break;

  case 1616:
#line 5802 "sql_yacc.yy"
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

  case 1617:
#line 5827 "sql_yacc.yy"
    { yyval.lex_user=yyvsp[-4].lex_user; yyvsp[-4].lex_user->password=yyvsp[0].lex_str ; }
    break;

  case 1618:
#line 5829 "sql_yacc.yy"
    { yyval.lex_user=yyvsp[0].lex_user; yyvsp[0].lex_user->password.str=NullS; }
    break;

  case 1619:
#line 5835 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->grant |= lex->which_columns;
	}
    break;

  case 1623:
#line 5847 "sql_yacc.yy"
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

  case 1625:
#line 5869 "sql_yacc.yy"
    {
            Lex->ssl_type=SSL_TYPE_SPECIFIED;
          }
    break;

  case 1626:
#line 5873 "sql_yacc.yy"
    {
            Lex->ssl_type=SSL_TYPE_ANY;
          }
    break;

  case 1627:
#line 5877 "sql_yacc.yy"
    {
            Lex->ssl_type=SSL_TYPE_X509;
          }
    break;

  case 1628:
#line 5881 "sql_yacc.yy"
    {
	    Lex->ssl_type=SSL_TYPE_NONE;
	  }
    break;

  case 1629:
#line 5887 "sql_yacc.yy"
    {}
    break;

  case 1631:
#line 5891 "sql_yacc.yy"
    {}
    break;

  case 1632:
#line 5892 "sql_yacc.yy"
    {}
    break;

  case 1633:
#line 5896 "sql_yacc.yy"
    { Lex->grant |= GRANT_ACL;}
    break;

  case 1634:
#line 5898 "sql_yacc.yy"
    {
	  Lex->mqh.questions=yyvsp[0].ulong_num;
	  Lex->mqh.bits |= 1;
	}
    break;

  case 1635:
#line 5903 "sql_yacc.yy"
    {
	  Lex->mqh.updates=yyvsp[0].ulong_num;
	  Lex->mqh.bits |= 2;
	}
    break;

  case 1636:
#line 5908 "sql_yacc.yy"
    {
	  Lex->mqh.connections=yyvsp[0].ulong_num;
	  Lex->mqh.bits |= 4;
	}
    break;

  case 1637:
#line 5915 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_BEGIN; Lex->start_transaction_opt= 0;}
    break;

  case 1638:
#line 5915 "sql_yacc.yy"
    {}
    break;

  case 1639:
#line 5919 "sql_yacc.yy"
    {}
    break;

  case 1640:
#line 5920 "sql_yacc.yy"
    {;}
    break;

  case 1641:
#line 5924 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_COMMIT;}
    break;

  case 1642:
#line 5928 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_ROLLBACK;
	}
    break;

  case 1643:
#line 5932 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_ROLLBACK_TO_SAVEPOINT;
	  Lex->savepoint_name = yyvsp[0].lex_str.str;
	}
    break;

  case 1644:
#line 5938 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_SAVEPOINT;
	  Lex->savepoint_name = yyvsp[0].lex_str.str;
	}
    break;

  case 1645:
#line 5949 "sql_yacc.yy"
    {}
    break;

  case 1647:
#line 5955 "sql_yacc.yy"
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

  case 1648:
#line 5976 "sql_yacc.yy"
    {}
    break;

  case 1649:
#line 5980 "sql_yacc.yy"
    {}
    break;

  case 1650:
#line 5981 "sql_yacc.yy"
    {}
    break;

  case 1651:
#line 5985 "sql_yacc.yy"
    {}
    break;

  case 1652:
#line 5987 "sql_yacc.yy"
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

  case 1653:
#line 6003 "sql_yacc.yy"
    {
	    THD *thd= YYTHD;
	    thd->lex->current_select->no_table_names_allowed= 0;
	    thd->where= "";
          }
    break;

  case 1656:
#line 6016 "sql_yacc.yy"
    { yyval.num=1; }
    break;

  case 1657:
#line 6017 "sql_yacc.yy"
    { yyval.num=1; }
    break;

  case 1658:
#line 6018 "sql_yacc.yy"
    { yyval.num=0; }
    break;

  case 1659:
#line 6024 "sql_yacc.yy"
    {
	  yyval.item= yyvsp[-1].item;
	}
    break;

  case 1660:
#line 6030 "sql_yacc.yy"
    {
	  yyval.item= new Item_singlerow_subselect(Lex->current_select->
					   master_unit()->first_select());
	}
    break;

  case 1661:
#line 6038 "sql_yacc.yy"
    {
	  yyval.item= yyvsp[-1].item;
	}
    break;

  case 1662:
#line 6044 "sql_yacc.yy"
    {
	  yyval.item= new Item_exists_subselect(Lex->current_select->master_unit()->
					first_select());
	}
    break;

  case 1663:
#line 6052 "sql_yacc.yy"
    {
    yyval.select_lex= yyvsp[-1].select_lex;
  }
    break;

  case 1664:
#line 6058 "sql_yacc.yy"
    {
    yyval.select_lex= Lex->current_select->master_unit()->first_select();
  }
    break;

  case 1665:
#line 6064 "sql_yacc.yy"
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

  case 1666:
#line 6079 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->current_select = lex->current_select->return_after_parsing();
	}
    break;


    }

/* Line 1000 of yacc.c.  */
#line 21681 "sql_yacc.cc"

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



