/* A Bison parser, made from sql_yacc.yy, by GNU bison 1.75.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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
#define YYBISON	1

/* Pure parsers.  */
#define YYPURE	1

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
     EQ = 264,
     EQUAL_SYM = 265,
     GE = 266,
     GT_SYM = 267,
     LE = 268,
     LT = 269,
     NE = 270,
     IS = 271,
     SHIFT_LEFT = 272,
     SHIFT_RIGHT = 273,
     SET_VAR = 274,
     ABORT_SYM = 275,
     ADD = 276,
     AFTER_SYM = 277,
     ALTER = 278,
     ANALYZE_SYM = 279,
     AVG_SYM = 280,
     BEGIN_SYM = 281,
     BINLOG_SYM = 282,
     CHANGE = 283,
     CLIENT_SYM = 284,
     COMMENT_SYM = 285,
     COMMIT_SYM = 286,
     COUNT_SYM = 287,
     CREATE = 288,
     CROSS = 289,
     CUBE_SYM = 290,
     DELETE_SYM = 291,
     DO_SYM = 292,
     DROP = 293,
     EVENTS_SYM = 294,
     EXECUTE_SYM = 295,
     FLUSH_SYM = 296,
     INSERT = 297,
     IO_THREAD = 298,
     KILL_SYM = 299,
     LOAD = 300,
     LOCKS_SYM = 301,
     LOCK_SYM = 302,
     MASTER_SYM = 303,
     MAX_SYM = 304,
     MIN_SYM = 305,
     NONE_SYM = 306,
     OPTIMIZE = 307,
     PURGE = 308,
     REPAIR = 309,
     REPLICATION = 310,
     RESET_SYM = 311,
     ROLLBACK_SYM = 312,
     ROLLUP_SYM = 313,
     SAVEPOINT_SYM = 314,
     SELECT_SYM = 315,
     SHOW = 316,
     SLAVE = 317,
     SQL_THREAD = 318,
     START_SYM = 319,
     STD_SYM = 320,
     STOP_SYM = 321,
     SUM_SYM = 322,
     SUPER_SYM = 323,
     TRUNCATE_SYM = 324,
     UNLOCK_SYM = 325,
     UPDATE_SYM = 326,
     ACTION = 327,
     AGGREGATE_SYM = 328,
     ALL = 329,
     AND = 330,
     AS = 331,
     ASC = 332,
     AUTO_INC = 333,
     AVG_ROW_LENGTH = 334,
     BACKUP_SYM = 335,
     BERKELEY_DB_SYM = 336,
     BINARY = 337,
     BIT_SYM = 338,
     BOOL_SYM = 339,
     BOOLEAN_SYM = 340,
     BOTH = 341,
     BY = 342,
     CACHE_SYM = 343,
     CASCADE = 344,
     CAST_SYM = 345,
     CHARSET = 346,
     CHECKSUM_SYM = 347,
     CHECK_SYM = 348,
     COMMITTED_SYM = 349,
     COLUMNS = 350,
     COLUMN_SYM = 351,
     CONCURRENT = 352,
     CONSTRAINT = 353,
     CONVERT_SYM = 354,
     DATABASES = 355,
     DATA_SYM = 356,
     DEFAULT = 357,
     DELAYED_SYM = 358,
     DELAY_KEY_WRITE_SYM = 359,
     DESC = 360,
     DESCRIBE = 361,
     DES_KEY_FILE = 362,
     DISABLE_SYM = 363,
     DISTINCT = 364,
     DYNAMIC_SYM = 365,
     ENABLE_SYM = 366,
     ENCLOSED = 367,
     ESCAPED = 368,
     DIRECTORY_SYM = 369,
     ESCAPE_SYM = 370,
     EXISTS = 371,
     EXTENDED_SYM = 372,
     FILE_SYM = 373,
     FIRST_SYM = 374,
     FIXED_SYM = 375,
     FLOAT_NUM = 376,
     FORCE_SYM = 377,
     FOREIGN = 378,
     FROM = 379,
     FULL = 380,
     FULLTEXT_SYM = 381,
     GLOBAL_SYM = 382,
     GRANT = 383,
     GRANTS = 384,
     GREATEST_SYM = 385,
     GROUP = 386,
     HAVING = 387,
     HEAP_SYM = 388,
     HEX_NUM = 389,
     HIGH_PRIORITY = 390,
     HOSTS_SYM = 391,
     IDENT = 392,
     IGNORE_SYM = 393,
     INDEX = 394,
     INDEXES = 395,
     INFILE = 396,
     INNER_SYM = 397,
     INNOBASE_SYM = 398,
     INTO = 399,
     IN_SYM = 400,
     ISOLATION = 401,
     ISAM_SYM = 402,
     JOIN_SYM = 403,
     KEYS = 404,
     KEY_SYM = 405,
     LEADING = 406,
     LEAST_SYM = 407,
     LEVEL_SYM = 408,
     LEX_HOSTNAME = 409,
     LIKE = 410,
     LINES = 411,
     LOCAL_SYM = 412,
     LOG_SYM = 413,
     LOGS_SYM = 414,
     LONG_NUM = 415,
     LONG_SYM = 416,
     LOW_PRIORITY = 417,
     MASTER_HOST_SYM = 418,
     MASTER_USER_SYM = 419,
     MASTER_LOG_FILE_SYM = 420,
     MASTER_LOG_POS_SYM = 421,
     MASTER_PASSWORD_SYM = 422,
     MASTER_PORT_SYM = 423,
     MASTER_CONNECT_RETRY_SYM = 424,
     MASTER_SERVER_ID_SYM = 425,
     RELAY_LOG_FILE_SYM = 426,
     RELAY_LOG_POS_SYM = 427,
     MATCH = 428,
     MAX_ROWS = 429,
     MAX_CONNECTIONS_PER_HOUR = 430,
     MAX_QUERIES_PER_HOUR = 431,
     MAX_UPDATES_PER_HOUR = 432,
     MEDIUM_SYM = 433,
     MERGE_SYM = 434,
     MEMORY_SYM = 435,
     MIN_ROWS = 436,
     MYISAM_SYM = 437,
     NATIONAL_SYM = 438,
     NATURAL = 439,
     NEW_SYM = 440,
     NCHAR_SYM = 441,
     NOT = 442,
     NO_SYM = 443,
     NULL_SYM = 444,
     NUM = 445,
     OFFSET_SYM = 446,
     ON = 447,
     OPEN_SYM = 448,
     OPTION = 449,
     OPTIONALLY = 450,
     OR = 451,
     OR_OR_CONCAT = 452,
     ORDER_SYM = 453,
     OUTER = 454,
     OUTFILE = 455,
     DUMPFILE = 456,
     PACK_KEYS_SYM = 457,
     PARTIAL = 458,
     PRIMARY_SYM = 459,
     PRIVILEGES = 460,
     PROCESS = 461,
     PROCESSLIST_SYM = 462,
     QUERY_SYM = 463,
     RAID_0_SYM = 464,
     RAID_STRIPED_SYM = 465,
     RAID_TYPE = 466,
     RAID_CHUNKS = 467,
     RAID_CHUNKSIZE = 468,
     READ_SYM = 469,
     REAL_NUM = 470,
     REFERENCES = 471,
     REGEXP = 472,
     RELOAD = 473,
     RENAME = 474,
     REPEATABLE_SYM = 475,
     REQUIRE_SYM = 476,
     RESOURCES = 477,
     RESTORE_SYM = 478,
     RESTRICT = 479,
     REVOKE = 480,
     ROWS_SYM = 481,
     ROW_FORMAT_SYM = 482,
     ROW_SYM = 483,
     SET = 484,
     SERIALIZABLE_SYM = 485,
     SESSION_SYM = 486,
     SHUTDOWN = 487,
     SSL_SYM = 488,
     STARTING = 489,
     STATUS_SYM = 490,
     STRAIGHT_JOIN = 491,
     SUBJECT_SYM = 492,
     TABLES = 493,
     TABLE_SYM = 494,
     TEMPORARY = 495,
     TERMINATED = 496,
     TEXT_STRING = 497,
     TO_SYM = 498,
     TRAILING = 499,
     TRANSACTION_SYM = 500,
     TYPE_SYM = 501,
     FUNC_ARG0 = 502,
     FUNC_ARG1 = 503,
     FUNC_ARG2 = 504,
     FUNC_ARG3 = 505,
     UDF_RETURNS_SYM = 506,
     UDF_SONAME_SYM = 507,
     UDF_SYM = 508,
     UNCOMMITTED_SYM = 509,
     UNION_SYM = 510,
     UNIQUE_SYM = 511,
     USAGE = 512,
     USE_FRM = 513,
     USE_SYM = 514,
     USING = 515,
     VALUES = 516,
     VARIABLES = 517,
     WHERE = 518,
     WITH = 519,
     WRITE_SYM = 520,
     X509_SYM = 521,
     XOR = 522,
     COMPRESSED_SYM = 523,
     BIGINT = 524,
     BLOB_SYM = 525,
     CHAR_SYM = 526,
     CHANGED = 527,
     COALESCE = 528,
     DATETIME = 529,
     DATE_SYM = 530,
     DECIMAL_SYM = 531,
     DOUBLE_SYM = 532,
     ENUM = 533,
     FAST_SYM = 534,
     FLOAT_SYM = 535,
     INT_SYM = 536,
     LIMIT = 537,
     LONGBLOB = 538,
     LONGTEXT = 539,
     MEDIUMBLOB = 540,
     MEDIUMINT = 541,
     MEDIUMTEXT = 542,
     NUMERIC_SYM = 543,
     PRECISION = 544,
     QUICK = 545,
     REAL = 546,
     SIGNED_SYM = 547,
     SMALLINT = 548,
     STRING_SYM = 549,
     TEXT_SYM = 550,
     TIMESTAMP = 551,
     TIME_SYM = 552,
     TINYBLOB = 553,
     TINYINT = 554,
     TINYTEXT = 555,
     ULONGLONG_NUM = 556,
     UNSIGNED = 557,
     VARBINARY = 558,
     VARCHAR = 559,
     VARYING = 560,
     ZEROFILL = 561,
     AGAINST = 562,
     ATAN = 563,
     BETWEEN_SYM = 564,
     BIT_AND = 565,
     BIT_OR = 566,
     CASE_SYM = 567,
     CONCAT = 568,
     CONCAT_WS = 569,
     CURDATE = 570,
     CURTIME = 571,
     DATABASE = 572,
     DATE_ADD_INTERVAL = 573,
     DATE_SUB_INTERVAL = 574,
     DAY_HOUR_SYM = 575,
     DAY_MINUTE_SYM = 576,
     DAY_SECOND_SYM = 577,
     DAY_SYM = 578,
     DECODE_SYM = 579,
     DES_ENCRYPT_SYM = 580,
     DES_DECRYPT_SYM = 581,
     ELSE = 582,
     ELT_FUNC = 583,
     ENCODE_SYM = 584,
     ENCRYPT = 585,
     EXPORT_SET = 586,
     EXTRACT_SYM = 587,
     FIELD_FUNC = 588,
     FORMAT_SYM = 589,
     FOR_SYM = 590,
     FROM_UNIXTIME = 591,
     GROUP_UNIQUE_USERS = 592,
     HOUR_MINUTE_SYM = 593,
     HOUR_SECOND_SYM = 594,
     HOUR_SYM = 595,
     IDENTIFIED_SYM = 596,
     IF = 597,
     INSERT_METHOD = 598,
     INTERVAL_SYM = 599,
     LAST_INSERT_ID = 600,
     LEFT = 601,
     LOCATE = 602,
     MAKE_SET_SYM = 603,
     MASTER_POS_WAIT = 604,
     MINUTE_SECOND_SYM = 605,
     MINUTE_SYM = 606,
     MODE_SYM = 607,
     MODIFY_SYM = 608,
     MONTH_SYM = 609,
     NOW_SYM = 610,
     PASSWORD = 611,
     POSITION_SYM = 612,
     PROCEDURE = 613,
     RAND = 614,
     REPLACE = 615,
     RIGHT = 616,
     ROUND = 617,
     SECOND_SYM = 618,
     SHARE_SYM = 619,
     SUBSTRING = 620,
     SUBSTRING_INDEX = 621,
     TRIM = 622,
     UDA_CHAR_SUM = 623,
     UDA_FLOAT_SUM = 624,
     UDA_INT_SUM = 625,
     UDF_CHAR_FUNC = 626,
     UDF_FLOAT_FUNC = 627,
     UDF_INT_FUNC = 628,
     UNIQUE_USERS = 629,
     UNIX_TIMESTAMP = 630,
     USER = 631,
     WEEK_SYM = 632,
     WHEN_SYM = 633,
     WORK_SYM = 634,
     YEAR_MONTH_SYM = 635,
     YEAR_SYM = 636,
     YEARWEEK = 637,
     BENCHMARK_SYM = 638,
     END = 639,
     THEN_SYM = 640,
     SQL_BIG_RESULT = 641,
     SQL_CACHE_SYM = 642,
     SQL_CALC_FOUND_ROWS = 643,
     SQL_NO_CACHE_SYM = 644,
     SQL_SMALL_RESULT = 645,
     SQL_BUFFER_RESULT = 646,
     ISSUER_SYM = 647,
     CIPHER_SYM = 648,
     NEG = 649
   };
#endif
#define END_OF_INPUT 258
#define CLOSE_SYM 259
#define HANDLER_SYM 260
#define LAST_SYM 261
#define NEXT_SYM 262
#define PREV_SYM 263
#define EQ 264
#define EQUAL_SYM 265
#define GE 266
#define GT_SYM 267
#define LE 268
#define LT 269
#define NE 270
#define IS 271
#define SHIFT_LEFT 272
#define SHIFT_RIGHT 273
#define SET_VAR 274
#define ABORT_SYM 275
#define ADD 276
#define AFTER_SYM 277
#define ALTER 278
#define ANALYZE_SYM 279
#define AVG_SYM 280
#define BEGIN_SYM 281
#define BINLOG_SYM 282
#define CHANGE 283
#define CLIENT_SYM 284
#define COMMENT_SYM 285
#define COMMIT_SYM 286
#define COUNT_SYM 287
#define CREATE 288
#define CROSS 289
#define CUBE_SYM 290
#define DELETE_SYM 291
#define DO_SYM 292
#define DROP 293
#define EVENTS_SYM 294
#define EXECUTE_SYM 295
#define FLUSH_SYM 296
#define INSERT 297
#define IO_THREAD 298
#define KILL_SYM 299
#define LOAD 300
#define LOCKS_SYM 301
#define LOCK_SYM 302
#define MASTER_SYM 303
#define MAX_SYM 304
#define MIN_SYM 305
#define NONE_SYM 306
#define OPTIMIZE 307
#define PURGE 308
#define REPAIR 309
#define REPLICATION 310
#define RESET_SYM 311
#define ROLLBACK_SYM 312
#define ROLLUP_SYM 313
#define SAVEPOINT_SYM 314
#define SELECT_SYM 315
#define SHOW 316
#define SLAVE 317
#define SQL_THREAD 318
#define START_SYM 319
#define STD_SYM 320
#define STOP_SYM 321
#define SUM_SYM 322
#define SUPER_SYM 323
#define TRUNCATE_SYM 324
#define UNLOCK_SYM 325
#define UPDATE_SYM 326
#define ACTION 327
#define AGGREGATE_SYM 328
#define ALL 329
#define AND 330
#define AS 331
#define ASC 332
#define AUTO_INC 333
#define AVG_ROW_LENGTH 334
#define BACKUP_SYM 335
#define BERKELEY_DB_SYM 336
#define BINARY 337
#define BIT_SYM 338
#define BOOL_SYM 339
#define BOOLEAN_SYM 340
#define BOTH 341
#define BY 342
#define CACHE_SYM 343
#define CASCADE 344
#define CAST_SYM 345
#define CHARSET 346
#define CHECKSUM_SYM 347
#define CHECK_SYM 348
#define COMMITTED_SYM 349
#define COLUMNS 350
#define COLUMN_SYM 351
#define CONCURRENT 352
#define CONSTRAINT 353
#define CONVERT_SYM 354
#define DATABASES 355
#define DATA_SYM 356
#define DEFAULT 357
#define DELAYED_SYM 358
#define DELAY_KEY_WRITE_SYM 359
#define DESC 360
#define DESCRIBE 361
#define DES_KEY_FILE 362
#define DISABLE_SYM 363
#define DISTINCT 364
#define DYNAMIC_SYM 365
#define ENABLE_SYM 366
#define ENCLOSED 367
#define ESCAPED 368
#define DIRECTORY_SYM 369
#define ESCAPE_SYM 370
#define EXISTS 371
#define EXTENDED_SYM 372
#define FILE_SYM 373
#define FIRST_SYM 374
#define FIXED_SYM 375
#define FLOAT_NUM 376
#define FORCE_SYM 377
#define FOREIGN 378
#define FROM 379
#define FULL 380
#define FULLTEXT_SYM 381
#define GLOBAL_SYM 382
#define GRANT 383
#define GRANTS 384
#define GREATEST_SYM 385
#define GROUP 386
#define HAVING 387
#define HEAP_SYM 388
#define HEX_NUM 389
#define HIGH_PRIORITY 390
#define HOSTS_SYM 391
#define IDENT 392
#define IGNORE_SYM 393
#define INDEX 394
#define INDEXES 395
#define INFILE 396
#define INNER_SYM 397
#define INNOBASE_SYM 398
#define INTO 399
#define IN_SYM 400
#define ISOLATION 401
#define ISAM_SYM 402
#define JOIN_SYM 403
#define KEYS 404
#define KEY_SYM 405
#define LEADING 406
#define LEAST_SYM 407
#define LEVEL_SYM 408
#define LEX_HOSTNAME 409
#define LIKE 410
#define LINES 411
#define LOCAL_SYM 412
#define LOG_SYM 413
#define LOGS_SYM 414
#define LONG_NUM 415
#define LONG_SYM 416
#define LOW_PRIORITY 417
#define MASTER_HOST_SYM 418
#define MASTER_USER_SYM 419
#define MASTER_LOG_FILE_SYM 420
#define MASTER_LOG_POS_SYM 421
#define MASTER_PASSWORD_SYM 422
#define MASTER_PORT_SYM 423
#define MASTER_CONNECT_RETRY_SYM 424
#define MASTER_SERVER_ID_SYM 425
#define RELAY_LOG_FILE_SYM 426
#define RELAY_LOG_POS_SYM 427
#define MATCH 428
#define MAX_ROWS 429
#define MAX_CONNECTIONS_PER_HOUR 430
#define MAX_QUERIES_PER_HOUR 431
#define MAX_UPDATES_PER_HOUR 432
#define MEDIUM_SYM 433
#define MERGE_SYM 434
#define MEMORY_SYM 435
#define MIN_ROWS 436
#define MYISAM_SYM 437
#define NATIONAL_SYM 438
#define NATURAL 439
#define NEW_SYM 440
#define NCHAR_SYM 441
#define NOT 442
#define NO_SYM 443
#define NULL_SYM 444
#define NUM 445
#define OFFSET_SYM 446
#define ON 447
#define OPEN_SYM 448
#define OPTION 449
#define OPTIONALLY 450
#define OR 451
#define OR_OR_CONCAT 452
#define ORDER_SYM 453
#define OUTER 454
#define OUTFILE 455
#define DUMPFILE 456
#define PACK_KEYS_SYM 457
#define PARTIAL 458
#define PRIMARY_SYM 459
#define PRIVILEGES 460
#define PROCESS 461
#define PROCESSLIST_SYM 462
#define QUERY_SYM 463
#define RAID_0_SYM 464
#define RAID_STRIPED_SYM 465
#define RAID_TYPE 466
#define RAID_CHUNKS 467
#define RAID_CHUNKSIZE 468
#define READ_SYM 469
#define REAL_NUM 470
#define REFERENCES 471
#define REGEXP 472
#define RELOAD 473
#define RENAME 474
#define REPEATABLE_SYM 475
#define REQUIRE_SYM 476
#define RESOURCES 477
#define RESTORE_SYM 478
#define RESTRICT 479
#define REVOKE 480
#define ROWS_SYM 481
#define ROW_FORMAT_SYM 482
#define ROW_SYM 483
#define SET 484
#define SERIALIZABLE_SYM 485
#define SESSION_SYM 486
#define SHUTDOWN 487
#define SSL_SYM 488
#define STARTING 489
#define STATUS_SYM 490
#define STRAIGHT_JOIN 491
#define SUBJECT_SYM 492
#define TABLES 493
#define TABLE_SYM 494
#define TEMPORARY 495
#define TERMINATED 496
#define TEXT_STRING 497
#define TO_SYM 498
#define TRAILING 499
#define TRANSACTION_SYM 500
#define TYPE_SYM 501
#define FUNC_ARG0 502
#define FUNC_ARG1 503
#define FUNC_ARG2 504
#define FUNC_ARG3 505
#define UDF_RETURNS_SYM 506
#define UDF_SONAME_SYM 507
#define UDF_SYM 508
#define UNCOMMITTED_SYM 509
#define UNION_SYM 510
#define UNIQUE_SYM 511
#define USAGE 512
#define USE_FRM 513
#define USE_SYM 514
#define USING 515
#define VALUES 516
#define VARIABLES 517
#define WHERE 518
#define WITH 519
#define WRITE_SYM 520
#define X509_SYM 521
#define XOR 522
#define COMPRESSED_SYM 523
#define BIGINT 524
#define BLOB_SYM 525
#define CHAR_SYM 526
#define CHANGED 527
#define COALESCE 528
#define DATETIME 529
#define DATE_SYM 530
#define DECIMAL_SYM 531
#define DOUBLE_SYM 532
#define ENUM 533
#define FAST_SYM 534
#define FLOAT_SYM 535
#define INT_SYM 536
#define LIMIT 537
#define LONGBLOB 538
#define LONGTEXT 539
#define MEDIUMBLOB 540
#define MEDIUMINT 541
#define MEDIUMTEXT 542
#define NUMERIC_SYM 543
#define PRECISION 544
#define QUICK 545
#define REAL 546
#define SIGNED_SYM 547
#define SMALLINT 548
#define STRING_SYM 549
#define TEXT_SYM 550
#define TIMESTAMP 551
#define TIME_SYM 552
#define TINYBLOB 553
#define TINYINT 554
#define TINYTEXT 555
#define ULONGLONG_NUM 556
#define UNSIGNED 557
#define VARBINARY 558
#define VARCHAR 559
#define VARYING 560
#define ZEROFILL 561
#define AGAINST 562
#define ATAN 563
#define BETWEEN_SYM 564
#define BIT_AND 565
#define BIT_OR 566
#define CASE_SYM 567
#define CONCAT 568
#define CONCAT_WS 569
#define CURDATE 570
#define CURTIME 571
#define DATABASE 572
#define DATE_ADD_INTERVAL 573
#define DATE_SUB_INTERVAL 574
#define DAY_HOUR_SYM 575
#define DAY_MINUTE_SYM 576
#define DAY_SECOND_SYM 577
#define DAY_SYM 578
#define DECODE_SYM 579
#define DES_ENCRYPT_SYM 580
#define DES_DECRYPT_SYM 581
#define ELSE 582
#define ELT_FUNC 583
#define ENCODE_SYM 584
#define ENCRYPT 585
#define EXPORT_SET 586
#define EXTRACT_SYM 587
#define FIELD_FUNC 588
#define FORMAT_SYM 589
#define FOR_SYM 590
#define FROM_UNIXTIME 591
#define GROUP_UNIQUE_USERS 592
#define HOUR_MINUTE_SYM 593
#define HOUR_SECOND_SYM 594
#define HOUR_SYM 595
#define IDENTIFIED_SYM 596
#define IF 597
#define INSERT_METHOD 598
#define INTERVAL_SYM 599
#define LAST_INSERT_ID 600
#define LEFT 601
#define LOCATE 602
#define MAKE_SET_SYM 603
#define MASTER_POS_WAIT 604
#define MINUTE_SECOND_SYM 605
#define MINUTE_SYM 606
#define MODE_SYM 607
#define MODIFY_SYM 608
#define MONTH_SYM 609
#define NOW_SYM 610
#define PASSWORD 611
#define POSITION_SYM 612
#define PROCEDURE 613
#define RAND 614
#define REPLACE 615
#define RIGHT 616
#define ROUND 617
#define SECOND_SYM 618
#define SHARE_SYM 619
#define SUBSTRING 620
#define SUBSTRING_INDEX 621
#define TRIM 622
#define UDA_CHAR_SUM 623
#define UDA_FLOAT_SUM 624
#define UDA_INT_SUM 625
#define UDF_CHAR_FUNC 626
#define UDF_FLOAT_FUNC 627
#define UDF_INT_FUNC 628
#define UNIQUE_USERS 629
#define UNIX_TIMESTAMP 630
#define USER 631
#define WEEK_SYM 632
#define WHEN_SYM 633
#define WORK_SYM 634
#define YEAR_MONTH_SYM 635
#define YEAR_SYM 636
#define YEARWEEK 637
#define BENCHMARK_SYM 638
#define END 639
#define THEN_SYM 640
#define SQL_BIG_RESULT 641
#define SQL_CACHE_SYM 642
#define SQL_CALC_FOUND_ROWS 643
#define SQL_NO_CACHE_SYM 644
#define SQL_SMALL_RESULT 645
#define SQL_BUFFER_RESULT 646
#define ISSUER_SYM 647
#define CIPHER_SYM 648
#define NEG 649




/* Copy the first part of user declarations.  */
#line 19 "sql_yacc.yy"

#define MYSQL_YACC
#define YYINITDEPTH 100
#define YYMAXDEPTH 3200				/* Because of 64K stack */
#define Lex current_lex
#define Select Lex->select
#include "mysql_priv.h"
#include "slave.h"
#include "sql_acl.h"
#include "lex_symbol.h"
#include <myisam.h>
#include <myisammrg.h>

extern void yyerror(const char*);
int yylex(void *yylval);

#define yyoverflow(A,B,C,D,E,F) if (my_yyoverflow((B),(D),(int*) (F))) { yyerror((char*) (A)); return 2; }

inline Item *or_or_concat(Item* A, Item* B)
{
  return (current_thd->sql_mode & MODE_PIPES_AS_CONCAT ?
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

#ifndef YYSTYPE
#line 44 "sql_yacc.yy"
typedef union {
  int  num;
  ulong ulong_num;
  ulonglong ulonglong_number;
  LEX_STRING lex_str;
  LEX_STRING *lex_str_ptr;
  LEX_SYMBOL symbol;
  Table_ident *table;
  char *simple_string;
  Item *item;
  List<Item> *item_list;
  List<String> *string_list;
  String *string;
  key_part_spec *key_part;
  TABLE_LIST *table_list;
  udf_func *udf;
  LEX_USER *lex_user;
  sys_var *variable;
  Key::Keytype key_type;
  enum db_type db_type;
  enum row_type row_type;
  enum ha_rkey_function ha_rkey_mode;
  enum enum_tx_isolation tx_isolation;
  enum Item_cast cast_type;
  enum Item_udftype udf_type;
  thr_lock_type lock_type;
  interval_type interval;
} yystype;
/* Line 193 of /usr/share/bison/yacc.c.  */
#line 916 "sql_yacc.cc"
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} yyltype;
# define YYLTYPE yyltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Copy the second part of user declarations.  */
#line 73 "sql_yacc.yy"

bool my_yyoverflow(short **a, YYSTYPE **b,int *yystacksize);


/* Line 213 of /usr/share/bison/yacc.c.  */
#line 940 "sql_yacc.cc"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
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
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];	\
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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
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
#define YYFINAL  411
#define YYLAST   25911

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  412
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  368
/* YYNRULES -- Number of rules. */
#define YYNRULES  1288
/* YYNRULES -- Number of states. */
#define YYNSTATES  2413

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   649

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[X] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   407,     2,     2,     2,   400,   395,     2,
     404,   405,   398,   397,   406,   396,   411,   399,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   410,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   403,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   408,   394,   409,   401,     2,     2,     2,
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   402
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
      80,    82,    84,    85,    91,    93,    97,   101,   105,   109,
     113,   117,   121,   125,   129,   133,   134,   142,   143,   154,
     159,   160,   170,   173,   176,   181,   182,   187,   188,   189,
     195,   196,   204,   205,   211,   212,   214,   215,   217,   219,
     222,   224,   225,   229,   230,   232,   234,   237,   241,   245,
     249,   253,   257,   261,   265,   269,   273,   277,   281,   285,
     289,   293,   297,   303,   307,   312,   316,   321,   326,   328,
     330,   332,   334,   336,   338,   340,   342,   344,   346,   348,
     350,   352,   354,   356,   358,   360,   361,   364,   365,   367,
     369,   371,   373,   375,   379,   381,   383,   386,   389,   395,
     404,   407,   408,   411,   412,   415,   416,   421,   425,   429,
     433,   436,   438,   444,   447,   452,   458,   463,   467,   469,
     471,   473,   478,   480,   482,   484,   486,   488,   491,   494,
     496,   498,   500,   502,   506,   510,   511,   517,   518,   524,
     526,   528,   531,   534,   536,   539,   542,   544,   546,   548,
     550,   552,   554,   556,   559,   560,   564,   566,   572,   573,
     575,   578,   580,   582,   584,   586,   587,   591,   592,   594,
     595,   597,   600,   602,   604,   607,   610,   612,   615,   617,
     620,   623,   624,   626,   631,   635,   642,   643,   645,   648,
     650,   654,   658,   661,   664,   666,   668,   671,   674,   677,
     681,   683,   685,   688,   691,   695,   697,   699,   701,   703,
     705,   706,   708,   710,   715,   718,   720,   725,   726,   728,
     730,   734,   735,   742,   743,   745,   749,   752,   756,   759,
     764,   765,   772,   773,   774,   783,   788,   792,   797,   801,
     804,   807,   814,   820,   824,   826,   828,   829,   831,   832,
     834,   835,   837,   839,   840,   843,   845,   846,   848,   850,
     852,   856,   860,   864,   868,   869,   873,   875,   879,   880,
     882,   884,   885,   892,   893,   900,   901,   907,   908,   910,
     912,   915,   917,   919,   921,   922,   928,   929,   935,   936,
     938,   940,   943,   945,   947,   949,   951,   953,   954,   960,
     961,   966,   968,   972,   976,   978,   979,   984,   985,   992,
     993,   999,  1001,  1003,  1006,  1009,  1018,  1019,  1021,  1024,
    1026,  1028,  1030,  1032,  1034,  1036,  1038,  1040,  1042,  1044,
    1046,  1047,  1050,  1055,  1059,  1061,  1063,  1068,  1069,  1070,
    1072,  1074,  1075,  1078,  1081,  1083,  1085,  1086,  1089,  1091,
    1093,  1099,  1106,  1112,  1119,  1123,  1127,  1131,  1135,  1140,
    1146,  1150,  1155,  1159,  1164,  1168,  1172,  1176,  1180,  1184,
    1188,  1192,  1196,  1200,  1204,  1208,  1212,  1216,  1220,  1224,
    1228,  1232,  1238,  1244,  1250,  1257,  1261,  1265,  1269,  1273,
    1278,  1284,  1288,  1293,  1297,  1302,  1306,  1310,  1314,  1318,
    1322,  1326,  1330,  1334,  1338,  1342,  1346,  1350,  1354,  1358,
    1362,  1366,  1370,  1376,  1382,  1384,  1390,  1397,  1403,  1410,
    1414,  1418,  1422,  1427,  1433,  1437,  1442,  1446,  1451,  1455,
    1459,  1463,  1467,  1471,  1475,  1479,  1483,  1487,  1491,  1495,
    1499,  1503,  1507,  1511,  1515,  1519,  1525,  1531,  1533,  1535,
    1537,  1542,  1545,  1550,  1552,  1555,  1558,  1561,  1564,  1568,
    1573,  1580,  1590,  1593,  1600,  1607,  1614,  1618,  1623,  1630,
    1639,  1644,  1651,  1656,  1661,  1666,  1673,  1676,  1679,  1684,
    1693,  1702,  1706,  1713,  1720,  1725,  1732,  1739,  1746,  1751,
    1758,  1763,  1770,  1779,  1790,  1803,  1810,  1815,  1822,  1829,
    1834,  1843,  1854,  1860,  1867,  1871,  1876,  1883,  1890,  1899,
    1906,  1913,  1918,  1925,  1932,  1941,  1946,  1951,  1954,  1959,
    1964,  1971,  1976,  1980,  1989,  1996,  2001,  2008,  2013,  2022,
    2029,  2038,  2045,  2054,  2059,  2067,  2075,  2083,  2090,  2097,
    2102,  2107,  2112,  2117,  2122,  2127,  2138,  2142,  2147,  2151,
    2156,  2163,  2168,  2173,  2180,  2187,  2194,  2195,  2197,  2202,
    2207,  2212,  2218,  2223,  2224,  2225,  2233,  2244,  2249,  2254,
    2259,  2264,  2265,  2269,  2271,  2273,  2275,  2278,  2280,  2283,
    2285,  2287,  2289,  2290,  2293,  2295,  2299,  2301,  2305,  2306,
    2309,  2311,  2315,  2316,  2318,  2319,  2322,  2323,  2326,  2330,
    2336,  2337,  2339,  2343,  2345,  2349,  2353,  2357,  2363,  2364,
    2373,  2381,  2382,  2393,  2400,  2408,  2409,  2420,  2427,  2432,
    2434,  2437,  2440,  2441,  2446,  2457,  2458,  2460,  2461,  2464,
    2467,  2470,  2471,  2477,  2481,  2483,  2485,  2487,  2491,  2493,
    2495,  2497,  2499,  2501,  2503,  2505,  2507,  2509,  2511,  2513,
    2515,  2517,  2518,  2520,  2522,  2523,  2526,  2527,  2529,  2530,
    2533,  2534,  2535,  2539,  2542,  2543,  2544,  2549,  2554,  2557,
    2558,  2561,  2564,  2565,  2567,  2568,  2573,  2578,  2581,  2582,
    2584,  2586,  2587,  2588,  2592,  2594,  2598,  2602,  2603,  2606,
    2608,  2610,  2612,  2614,  2616,  2618,  2620,  2622,  2624,  2626,
    2627,  2628,  2635,  2636,  2638,  2642,  2644,  2647,  2648,  2655,
    2659,  2660,  2664,  2671,  2672,  2679,  2684,  2688,  2690,  2694,
    2696,  2697,  2700,  2701,  2703,  2704,  2705,  2713,  2714,  2715,
    2722,  2723,  2725,  2727,  2729,  2731,  2733,  2736,  2738,  2740,
    2742,  2746,  2751,  2752,  2756,  2757,  2761,  2764,  2768,  2770,
    2773,  2774,  2778,  2779,  2785,  2789,  2791,  2795,  2797,  2801,
    2803,  2805,  2806,  2808,  2809,  2814,  2815,  2817,  2821,  2823,
    2825,  2827,  2828,  2839,  2845,  2849,  2850,  2852,  2853,  2858,
    2859,  2866,  2867,  2873,  2874,  2881,  2883,  2887,  2890,  2895,
    2896,  2899,  2900,  2903,  2905,  2907,  2911,  2912,  2914,  2915,
    2919,  2922,  2926,  2931,  2936,  2943,  2960,  2963,  2966,  2967,
    2974,  2979,  2982,  2985,  2988,  2992,  2994,  2998,  3002,  3005,
    3008,  3009,  3012,  3013,  3016,  3017,  3019,  3021,  3023,  3024,
    3027,  3028,  3031,  3032,  3037,  3040,  3042,  3044,  3045,  3047,
    3049,  3050,  3054,  3058,  3060,  3061,  3065,  3070,  3073,  3075,
    3077,  3079,  3081,  3083,  3085,  3087,  3089,  3090,  3092,  3093,
    3097,  3101,  3103,  3105,  3107,  3110,  3111,  3118,  3121,  3124,
    3125,  3141,  3147,  3152,  3153,  3155,  3156,  3158,  3160,  3161,
    3163,  3165,  3166,  3169,  3172,  3174,  3178,  3183,  3187,  3191,
    3192,  3195,  3198,  3200,  3204,  3208,  3209,  3213,  3215,  3218,
    3220,  3222,  3224,  3226,  3228,  3230,  3232,  3234,  3236,  3238,
    3241,  3244,  3247,  3249,  3251,  3255,  3261,  3263,  3265,  3269,
    3274,  3280,  3282,  3286,  3289,  3291,  3295,  3298,  3300,  3304,
    3306,  3308,  3310,  3312,  3314,  3316,  3320,  3322,  3324,  3326,
    3328,  3330,  3332,  3334,  3336,  3338,  3340,  3342,  3344,  3346,
    3348,  3350,  3352,  3354,  3356,  3358,  3360,  3362,  3364,  3366,
    3368,  3370,  3372,  3374,  3376,  3378,  3380,  3382,  3384,  3386,
    3388,  3390,  3392,  3394,  3396,  3398,  3400,  3402,  3404,  3406,
    3408,  3410,  3412,  3414,  3416,  3418,  3420,  3422,  3424,  3426,
    3428,  3430,  3432,  3434,  3436,  3438,  3440,  3442,  3444,  3446,
    3448,  3450,  3452,  3454,  3456,  3458,  3460,  3462,  3464,  3466,
    3468,  3470,  3472,  3474,  3476,  3478,  3480,  3482,  3484,  3486,
    3488,  3490,  3492,  3494,  3496,  3498,  3500,  3502,  3504,  3506,
    3508,  3510,  3512,  3514,  3516,  3518,  3520,  3522,  3524,  3526,
    3528,  3530,  3532,  3534,  3536,  3538,  3540,  3542,  3544,  3546,
    3548,  3550,  3552,  3554,  3556,  3558,  3560,  3562,  3564,  3566,
    3568,  3570,  3572,  3574,  3576,  3578,  3580,  3582,  3584,  3586,
    3588,  3590,  3592,  3594,  3596,  3598,  3600,  3602,  3604,  3606,
    3608,  3610,  3612,  3614,  3616,  3618,  3620,  3622,  3624,  3626,
    3628,  3630,  3632,  3634,  3635,  3640,  3641,  3643,  3646,  3651,
    3652,  3654,  3656,  3658,  3659,  3661,  3663,  3665,  3666,  3669,
    3672,  3675,  3680,  3684,  3691,  3696,  3701,  3705,  3711,  3713,
    3716,  3719,  3722,  3724,  3726,  3731,  3733,  3735,  3737,  3739,
    3740,  3745,  3747,  3749,  3751,  3755,  3759,  3761,  3763,  3766,
    3769,  3772,  3777,  3781,  3782,  3790,  3792,  3795,  3797,  3799,
    3801,  3803,  3805,  3807,  3808,  3814,  3816,  3818,  3820,  3822,
    3824,  3825,  3833,  3834,  3844,  3846,  3849,  3851,  3853,  3857,
    3858,  3862,  3863,  3867,  3868,  3872,  3873,  3877,  3879,  3881,
    3883,  3885,  3887,  3889,  3891,  3893,  3895,  3897,  3899,  3902,
    3905,  3907,  3911,  3914,  3917,  3920,  3921,  3923,  3927,  3929,
    3932,  3935,  3938,  3940,  3944,  3948,  3950,  3952,  3956,  3961,
    3967,  3969,  3970,  3974,  3978,  3980,  3982,  3983,  3986,  3989,
    3992,  3995,  3996,  3999,  4002,  4004,  4007,  4010,  4013,  4016,
    4017,  4021,  4022,  4024,  4026,  4028,  4033,  4036,  4037,  4039,
    4040,  4045,  4047,  4049,  4050,  4051,  4055,  4056,  4058
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     413,     0,    -1,     3,    -1,   414,     3,    -1,   485,    -1,
     512,    -1,   505,    -1,   767,    -1,   415,    -1,   514,    -1,
     770,    -1,   419,    -1,   650,    -1,   673,    -1,   611,    -1,
     613,    -1,   745,    -1,   619,    -1,   677,    -1,   691,    -1,
     729,    -1,   689,    -1,   519,    -1,   687,    -1,   521,    -1,
     507,    -1,   622,    -1,   683,    -1,   503,    -1,   743,    -1,
     771,    -1,   772,    -1,   525,    -1,   717,    -1,   498,    -1,
     499,    -1,   663,    -1,   661,    -1,   736,    -1,   735,    -1,
     646,    -1,   690,    -1,    -1,    28,    48,   243,   416,   417,
      -1,   418,    -1,   417,   406,   418,    -1,   163,     9,   242,
      -1,   164,     9,   242,    -1,   167,     9,   242,    -1,   165,
       9,   242,    -1,   168,     9,   602,    -1,   166,     9,   603,
      -1,   169,     9,   602,    -1,   171,     9,   242,    -1,   172,
       9,   602,    -1,    -1,    33,   432,   239,   435,   711,   420,
     423,    -1,    -1,    33,   480,   139,   713,   192,   711,   421,
     404,   481,   405,    -1,    33,   317,   435,   713,    -1,    -1,
      33,   444,   253,   713,   422,   251,   445,   252,   242,    -1,
     404,   424,    -1,   436,   426,    -1,   446,   405,   436,   426,
      -1,    -1,   429,   405,   425,   776,    -1,    -1,    -1,   695,
     431,   429,   427,   773,    -1,    -1,   695,   431,   404,   429,
     405,   428,   776,    -1,    -1,    60,   430,   533,   537,   443,
      -1,    -1,    76,    -1,    -1,   433,    -1,   434,    -1,   434,
     433,    -1,   240,    -1,    -1,   342,   187,   116,    -1,    -1,
     437,    -1,   438,    -1,   438,   437,    -1,   246,     9,   439,
      -1,   174,     9,   603,    -1,   181,     9,   603,    -1,    79,
       9,   602,    -1,   356,     9,   242,    -1,    30,     9,   242,
      -1,    78,     9,   603,    -1,   202,     9,   602,    -1,   202,
       9,   102,    -1,    92,     9,   602,    -1,   104,     9,   602,
      -1,   227,     9,   440,    -1,   211,     9,   441,    -1,   212,
       9,   602,    -1,   213,     9,   602,    -1,   255,     9,   404,
     615,   405,    -1,    91,   640,   713,    -1,   271,   229,   640,
     713,    -1,   343,     9,   442,    -1,   101,   114,     9,   242,
      -1,   139,   114,     9,   242,    -1,   147,    -1,   182,    -1,
     179,    -1,   133,    -1,   180,    -1,    81,    -1,   143,    -1,
     102,    -1,   120,    -1,   110,    -1,   268,    -1,   210,    -1,
     209,    -1,   602,    -1,   188,    -1,   119,    -1,     6,    -1,
      -1,   532,   536,    -1,    -1,    73,    -1,   294,    -1,   291,
      -1,   281,    -1,   447,    -1,   446,   406,   447,    -1,   448,
      -1,   449,    -1,   452,   450,    -1,   452,   472,    -1,   477,
     483,   404,   481,   405,    -1,   451,   123,   150,   483,   404,
     481,   405,   472,    -1,   451,   450,    -1,    -1,    93,   544,
      -1,    -1,    98,   483,    -1,    -1,   710,   453,   454,   468,
      -1,   459,   466,   463,    -1,   460,   467,   463,    -1,   280,
     461,   463,    -1,    83,   466,    -1,    84,    -1,   457,   404,
     190,   405,   471,    -1,   457,   471,    -1,    82,   404,   190,
     405,    -1,   458,   404,   190,   405,   471,    -1,   303,   404,
     190,   405,    -1,   381,   466,   463,    -1,   275,    -1,   297,
      -1,   296,    -1,   296,   404,   190,   405,    -1,   274,    -1,
     298,    -1,   270,    -1,   285,    -1,   283,    -1,   161,   303,
      -1,   161,   458,    -1,   300,    -1,   295,    -1,   287,    -1,
     284,    -1,   276,   461,   463,    -1,   288,   461,   463,    -1,
      -1,   278,   455,   404,   484,   405,    -1,    -1,   229,   456,
     404,   484,   405,    -1,   271,    -1,   186,    -1,   183,   271,
      -1,   457,   305,    -1,   304,    -1,   183,   304,    -1,   186,
     304,    -1,   281,    -1,   299,    -1,   293,    -1,   286,    -1,
     269,    -1,   291,    -1,   277,    -1,   277,   289,    -1,    -1,
     404,   190,   405,    -1,   462,    -1,   404,   190,   406,   190,
     405,    -1,    -1,   464,    -1,   464,   465,    -1,   465,    -1,
     292,    -1,   302,    -1,   306,    -1,    -1,   404,   190,   405,
      -1,    -1,   462,    -1,    -1,   469,    -1,   469,   470,    -1,
     470,    -1,   189,    -1,   187,   189,    -1,   102,   705,    -1,
      78,    -1,   204,   150,    -1,   256,    -1,   256,   150,    -1,
      30,   703,    -1,    -1,    82,    -1,   271,   229,   640,   713,
      -1,   216,   711,   473,    -1,   216,   711,   404,   481,   405,
     473,    -1,    -1,   474,    -1,   474,   475,    -1,   475,    -1,
     192,    36,   476,    -1,   192,    71,   476,    -1,   173,   125,
      -1,   173,   203,    -1,   224,    -1,    89,    -1,   229,   189,
      -1,   188,    72,    -1,   229,   102,    -1,   451,   204,   150,
      -1,   478,    -1,   126,    -1,   126,   478,    -1,   451,   256,
      -1,   451,   256,   478,    -1,   150,    -1,   139,    -1,   149,
      -1,   139,    -1,   140,    -1,    -1,   256,    -1,   126,    -1,
     481,   406,   482,   597,    -1,   482,   597,    -1,   713,    -1,
     713,   404,   190,   405,    -1,    -1,   710,    -1,   704,    -1,
     484,   406,   704,    -1,    -1,    23,   494,   239,   711,   486,
     487,    -1,    -1,   489,    -1,   487,   406,   489,    -1,    21,
     493,    -1,   488,   448,   496,    -1,    21,   449,    -1,   488,
     404,   446,   405,    -1,    -1,    28,   493,   710,   490,   452,
     496,    -1,    -1,    -1,   353,   493,   710,   491,   454,   468,
     492,   496,    -1,    38,   493,   710,   495,    -1,    38,   204,
     150,    -1,    38,   123,   150,   483,    -1,    38,   478,   710,
      -1,   108,   149,    -1,   111,   149,    -1,    23,   493,   710,
     229,   102,   705,    -1,    23,   493,   710,    38,   102,    -1,
     219,   497,   711,    -1,   437,    -1,   594,    -1,    -1,    96,
      -1,    -1,   138,    -1,    -1,   224,    -1,    89,    -1,    -1,
      22,   713,    -1,   119,    -1,    -1,   243,    -1,     9,    -1,
      76,    -1,    64,    62,   501,    -1,    66,    62,   501,    -1,
      62,    64,   501,    -1,    62,    66,   501,    -1,    -1,    64,
     245,   500,    -1,   502,    -1,   501,   406,   502,    -1,    -1,
      63,    -1,    43,    -1,    -1,   223,   731,   504,   615,   124,
     242,    -1,    -1,    80,   731,   506,   615,   243,   242,    -1,
      -1,    54,   731,   508,   615,   509,    -1,    -1,   510,    -1,
     511,    -1,   511,   510,    -1,   290,    -1,   117,    -1,   258,
      -1,    -1,    24,   731,   513,   615,   516,    -1,    -1,    93,
     731,   515,   615,   516,    -1,    -1,   517,    -1,   518,    -1,
     518,   517,    -1,   290,    -1,   279,    -1,   178,    -1,   117,
      -1,   272,    -1,    -1,    52,   731,   520,   615,   516,    -1,
      -1,   219,   731,   522,   523,    -1,   524,    -1,   523,   406,
     524,    -1,   711,   243,   711,    -1,   526,    -1,    -1,    60,
     529,   527,   773,    -1,    -1,   404,    60,   529,   405,   528,
     776,    -1,    -1,   530,   533,   537,   531,   536,    -1,   598,
      -1,   532,    -1,   609,   532,    -1,   532,   609,    -1,   124,
     569,   586,   590,   587,   593,   598,   604,    -1,    -1,   534,
      -1,   534,   535,    -1,   535,    -1,   236,    -1,   135,    -1,
     109,    -1,   390,    -1,   386,    -1,   391,    -1,   388,    -1,
     389,    -1,   387,    -1,    74,    -1,    -1,   335,    71,    -1,
      47,   145,   364,   352,    -1,   537,   406,   538,    -1,   538,
      -1,   398,    -1,   539,   541,   540,   542,    -1,    -1,    -1,
     707,    -1,   544,    -1,    -1,    76,   713,    -1,    76,   242,
      -1,   713,    -1,   242,    -1,    -1,   404,   405,    -1,   545,
      -1,   548,    -1,   544,   145,   404,   556,   405,    -1,   544,
     187,   145,   404,   556,   405,    -1,   544,   309,   547,    75,
     544,    -1,   544,   187,   309,   547,    75,   544,    -1,   544,
     197,   544,    -1,   544,   196,   544,    -1,   544,   267,   544,
      -1,   544,    75,   544,    -1,   544,   155,   548,   589,    -1,
     544,   187,   155,   548,   589,    -1,   544,   217,   544,    -1,
     544,   187,   217,   544,    -1,   544,    16,   189,    -1,   544,
      16,   187,   189,    -1,   544,     9,   544,    -1,   544,    10,
     544,    -1,   544,    11,   544,    -1,   544,    12,   544,    -1,
     544,    13,   544,    -1,   544,    14,   544,    -1,   544,    15,
     544,    -1,   544,    17,   544,    -1,   544,    18,   544,    -1,
     544,   397,   544,    -1,   544,   396,   544,    -1,   544,   398,
     544,    -1,   544,   399,   544,    -1,   544,   394,   544,    -1,
     544,   403,   544,    -1,   544,   395,   544,    -1,   544,   400,
     544,    -1,   544,   397,   344,   544,   582,    -1,   544,   396,
     344,   544,   582,    -1,   546,   309,   547,    75,   544,    -1,
     546,   187,   309,   547,    75,   544,    -1,   546,   197,   544,
      -1,   546,   196,   544,    -1,   546,   267,   544,    -1,   546,
      75,   544,    -1,   546,   155,   548,   589,    -1,   546,   187,
     155,   548,   589,    -1,   546,   217,   544,    -1,   546,   187,
     217,   544,    -1,   546,    16,   189,    -1,   546,    16,   187,
     189,    -1,   546,     9,   544,    -1,   546,    10,   544,    -1,
     546,    11,   544,    -1,   546,    12,   544,    -1,   546,    13,
     544,    -1,   546,    14,   544,    -1,   546,    15,   544,    -1,
     546,    17,   544,    -1,   546,    18,   544,    -1,   546,   397,
     544,    -1,   546,   396,   544,    -1,   546,   398,   544,    -1,
     546,   399,   544,    -1,   546,   394,   544,    -1,   546,   403,
     544,    -1,   546,   395,   544,    -1,   546,   400,   544,    -1,
     546,   397,   344,   544,   582,    -1,   546,   396,   344,   544,
     582,    -1,   548,    -1,   547,   145,   404,   556,   405,    -1,
     547,   187,   145,   404,   556,   405,    -1,   547,   309,   547,
      75,   544,    -1,   547,   187,   309,   547,    75,   544,    -1,
     547,   197,   544,    -1,   547,   196,   544,    -1,   547,   267,
     544,    -1,   547,   155,   548,   589,    -1,   547,   187,   155,
     548,   589,    -1,   547,   217,   544,    -1,   547,   187,   217,
     544,    -1,   547,    16,   189,    -1,   547,    16,   187,   189,
      -1,   547,     9,   544,    -1,   547,    10,   544,    -1,   547,
      11,   544,    -1,   547,    12,   544,    -1,   547,    13,   544,
      -1,   547,    14,   544,    -1,   547,    15,   544,    -1,   547,
      17,   544,    -1,   547,    18,   544,    -1,   547,   397,   544,
      -1,   547,   396,   544,    -1,   547,   398,   544,    -1,   547,
     399,   544,    -1,   547,   394,   544,    -1,   547,   403,   544,
      -1,   547,   395,   544,    -1,   547,   400,   544,    -1,   547,
     397,   344,   544,   582,    -1,   547,   396,   344,   544,   582,
      -1,   548,    -1,   709,    -1,   705,    -1,   410,   714,    19,
     544,    -1,   410,   714,    -1,   410,   410,   723,   714,    -1,
     550,    -1,   396,   544,    -1,   401,   544,    -1,   187,   544,
      -1,   407,   544,    -1,   404,   544,   405,    -1,   408,   713,
     544,   409,    -1,   173,   559,   307,   404,   544,   405,    -1,
     173,   559,   307,   404,   544,   145,    85,   352,   405,    -1,
      82,   544,    -1,    90,   404,   544,    76,   555,   405,    -1,
     312,   563,   378,   565,   564,   384,    -1,    99,   404,   544,
     406,   555,   405,    -1,   247,   404,   405,    -1,   248,   404,
     544,   405,    -1,   249,   404,   544,   406,   544,   405,    -1,
     250,   404,   544,   406,   544,   406,   544,   405,    -1,   308,
     404,   544,   405,    -1,   308,   404,   544,   406,   544,   405,
      -1,   271,   404,   556,   405,    -1,   273,   404,   556,   405,
      -1,   313,   404,   556,   405,    -1,   314,   404,   544,   406,
     556,   405,    -1,   315,   543,    -1,   316,   543,    -1,   316,
     404,   544,   405,    -1,   318,   404,   544,   406,   344,   544,
     582,   405,    -1,   319,   404,   544,   406,   344,   544,   582,
     405,    -1,   317,   404,   405,    -1,   328,   404,   544,   406,
     556,   405,    -1,   348,   404,   544,   406,   556,   405,    -1,
     330,   404,   544,   405,    -1,   330,   404,   544,   406,   544,
     405,    -1,   324,   404,   544,   406,   242,   405,    -1,   329,
     404,   544,   406,   242,   405,    -1,   326,   404,   544,   405,
      -1,   326,   404,   544,   406,   544,   405,    -1,   325,   404,
     544,   405,    -1,   325,   404,   544,   406,   544,   405,    -1,
     331,   404,   544,   406,   544,   406,   544,   405,    -1,   331,
     404,   544,   406,   544,   406,   544,   406,   544,   405,    -1,
     331,   404,   544,   406,   544,   406,   544,   406,   544,   406,
     544,   405,    -1,   334,   404,   544,   406,   190,   405,    -1,
     336,   404,   544,   405,    -1,   336,   404,   544,   406,   544,
     405,    -1,   333,   404,   544,   406,   556,   405,    -1,   340,
     404,   544,   405,    -1,   342,   404,   544,   406,   544,   406,
     544,   405,    -1,    42,   404,   544,   406,   544,   406,   544,
     406,   544,   405,    -1,   344,   544,   582,   397,   544,    -1,
     344,   404,   544,   406,   556,   405,    -1,   345,   404,   405,
      -1,   345,   404,   544,   405,    -1,   346,   404,   544,   406,
     544,   405,    -1,   347,   404,   544,   406,   544,   405,    -1,
     347,   404,   544,   406,   544,   406,   544,   405,    -1,   130,
     404,   544,   406,   556,   405,    -1,   152,   404,   544,   406,
     556,   405,    -1,   158,   404,   544,   405,    -1,   158,   404,
     544,   406,   544,   405,    -1,   349,   404,   544,   406,   544,
     405,    -1,   349,   404,   544,   406,   544,   406,   544,   405,
      -1,   351,   404,   544,   405,    -1,   354,   404,   544,   405,
      -1,   355,   543,    -1,   355,   404,   544,   405,    -1,   356,
     404,   544,   405,    -1,   357,   404,   546,   145,   544,   405,
      -1,   359,   404,   544,   405,    -1,   359,   404,   405,    -1,
     360,   404,   544,   406,   544,   406,   544,   405,    -1,   361,
     404,   544,   406,   544,   405,    -1,   362,   404,   544,   405,
      -1,   362,   404,   544,   406,   544,   405,    -1,   363,   404,
     544,   405,    -1,   365,   404,   544,   406,   544,   406,   544,
     405,    -1,   365,   404,   544,   406,   544,   405,    -1,   365,
     404,   544,   124,   544,   335,   544,   405,    -1,   365,   404,
     544,   124,   544,   405,    -1,   366,   404,   544,   406,   544,
     406,   544,   405,    -1,   367,   404,   544,   405,    -1,   367,
     404,   151,   568,   124,   544,   405,    -1,   367,   404,   244,
     568,   124,   544,   405,    -1,   367,   404,    86,   568,   124,
     544,   405,    -1,   367,   404,   544,   124,   544,   405,    -1,
      69,   404,   544,   406,   544,   405,    -1,   368,   404,   549,
     405,    -1,   369,   404,   549,   405,    -1,   370,   404,   549,
     405,    -1,   371,   404,   549,   405,    -1,   372,   404,   549,
     405,    -1,   373,   404,   549,   405,    -1,   374,   404,   703,
     406,   190,   406,   190,   406,   556,   405,    -1,   375,   404,
     405,    -1,   375,   404,   544,   405,    -1,   376,   404,   405,
      -1,   377,   404,   544,   405,    -1,   377,   404,   544,   406,
     544,   405,    -1,   381,   404,   544,   405,    -1,   382,   404,
     544,   405,    -1,   382,   404,   544,   406,   544,   405,    -1,
     383,   404,   602,   406,   544,   405,    -1,   332,   404,   582,
     124,   544,   405,    -1,    -1,   556,    -1,    25,   404,   553,
     405,    -1,   310,   404,   553,   405,    -1,   311,   404,   553,
     405,    -1,    32,   404,   585,   398,   405,    -1,    32,   404,
     553,   405,    -1,    -1,    -1,    32,   404,   109,   551,   556,
     552,   405,    -1,   337,   404,   703,   406,   190,   406,   190,
     406,   553,   405,    -1,    50,   404,   553,   405,    -1,    49,
     404,   553,   405,    -1,    65,   404,   553,   405,    -1,    67,
     404,   553,   405,    -1,    -1,   585,   554,   544,    -1,    82,
      -1,   271,    -1,   292,    -1,   292,   281,    -1,   302,    -1,
     302,   281,    -1,   275,    -1,   297,    -1,   274,    -1,    -1,
     557,   558,    -1,   544,    -1,   558,   406,   544,    -1,   560,
      -1,   404,   560,   405,    -1,    -1,   561,   562,    -1,   709,
      -1,   562,   406,   709,    -1,    -1,   544,    -1,    -1,   327,
     544,    -1,    -1,   566,   567,    -1,   544,   385,   544,    -1,
     567,   378,   544,   385,   544,    -1,    -1,   544,    -1,   404,
     569,   405,    -1,   574,    -1,   569,   406,   569,    -1,   569,
     573,   569,    -1,   569,   236,   569,    -1,   569,   573,   569,
     192,   544,    -1,    -1,   569,   573,   569,   260,   570,   404,
     581,   405,    -1,   569,   346,   576,   148,   569,   192,   544,
      -1,    -1,   569,   346,   576,   148,   569,   571,   260,   404,
     581,   405,    -1,   569,   184,   346,   576,   148,   569,    -1,
     569,   361,   576,   148,   569,   192,   544,    -1,    -1,   569,
     361,   576,   148,   569,   572,   260,   404,   581,   405,    -1,
     569,   184,   361,   576,   148,   569,    -1,   569,   184,   148,
     569,    -1,   148,    -1,   142,   148,    -1,    34,   148,    -1,
      -1,   575,   711,   584,   577,    -1,   408,   713,   574,   346,
     199,   148,   574,   192,   544,   409,    -1,    -1,   199,    -1,
      -1,   259,   578,    -1,   122,   578,    -1,   138,   578,    -1,
      -1,   478,   579,   404,   580,   405,    -1,   580,   406,   713,
      -1,   713,    -1,   204,    -1,   713,    -1,   581,   406,   713,
      -1,   320,    -1,   321,    -1,   322,    -1,   323,    -1,   338,
      -1,   339,    -1,   340,    -1,   350,    -1,   351,    -1,   354,
      -1,   363,    -1,   380,    -1,   381,    -1,    -1,    76,    -1,
       9,    -1,    -1,   583,   713,    -1,    -1,    74,    -1,    -1,
     263,   544,    -1,    -1,    -1,   132,   588,   544,    -1,   115,
     242,    -1,    -1,    -1,   131,    87,   591,   592,    -1,   591,
     406,   708,   597,    -1,   708,   597,    -1,    -1,   264,    35,
      -1,   264,    58,    -1,    -1,   594,    -1,    -1,   198,    87,
     595,   596,    -1,   596,   406,   708,   597,    -1,   708,   597,
      -1,    -1,    77,    -1,   105,    -1,    -1,    -1,   282,   599,
     600,    -1,   602,    -1,   602,   406,   602,    -1,   602,   191,
     602,    -1,    -1,   282,   603,    -1,   190,    -1,   160,    -1,
     301,    -1,   215,    -1,   121,    -1,   190,    -1,   301,    -1,
     160,    -1,   215,    -1,   121,    -1,    -1,    -1,   358,   713,
     605,   404,   606,   405,    -1,    -1,   607,    -1,   607,   406,
     608,    -1,   608,    -1,   539,   544,    -1,    -1,   144,   200,
     242,   610,   696,   699,    -1,   144,   201,   242,    -1,    -1,
      37,   612,   644,    -1,    38,   618,   239,   617,   615,   495,
      -1,    -1,    38,   139,   713,   192,   711,   614,    -1,    38,
     317,   617,   713,    -1,    38,   253,   713,    -1,   616,    -1,
     615,   406,   616,    -1,   711,    -1,    -1,   342,   116,    -1,
      -1,   240,    -1,    -1,    -1,    42,   620,   625,   494,   627,
     621,   629,    -1,    -1,    -1,   360,   623,   626,   627,   624,
     629,    -1,    -1,   162,    -1,   103,    -1,   135,    -1,   649,
      -1,   103,    -1,   144,   628,    -1,   628,    -1,   616,    -1,
     633,    -1,   404,   405,   633,    -1,   404,   632,   405,   633,
      -1,    -1,   229,   630,   637,    -1,    -1,   404,   632,   405,
      -1,   404,   405,    -1,   632,   406,   706,    -1,   706,    -1,
     261,   636,    -1,    -1,   429,   634,   773,    -1,    -1,   404,
     429,   405,   635,   776,    -1,   636,   406,   641,    -1,   641,
      -1,   637,   406,   638,    -1,   638,    -1,   709,   639,   645,
      -1,     9,    -1,    19,    -1,    -1,   639,    -1,    -1,   404,
     642,   643,   405,    -1,    -1,   644,    -1,   644,   406,   645,
      -1,   645,    -1,   544,    -1,   102,    -1,    -1,    71,   647,
     649,   494,   569,   229,   648,   586,   593,   601,    -1,   648,
     406,   709,   639,   544,    -1,   709,   639,   544,    -1,    -1,
     162,    -1,    -1,    36,   651,   659,   652,    -1,    -1,   124,
     711,   653,   586,   593,   601,    -1,    -1,   656,   654,   124,
     569,   586,    -1,    -1,   124,   656,   655,   260,   569,   586,
      -1,   657,    -1,   656,   406,   657,    -1,   713,   658,    -1,
     713,   411,   713,   658,    -1,    -1,   411,   398,    -1,    -1,
     660,   659,    -1,   290,    -1,   162,    -1,    69,   662,   616,
      -1,    -1,   239,    -1,    -1,    61,   664,   665,    -1,   100,
     668,    -1,   238,   667,   668,    -1,   239,   235,   667,   668,
      -1,   193,   238,   667,   668,    -1,   669,    95,   670,   711,
     667,   668,    -1,   185,    48,   335,    62,   264,   165,     9,
     242,    75,   166,     9,   603,    75,   170,     9,   602,    -1,
      48,   159,    -1,    62,   136,    -1,    -1,    27,    39,   671,
     672,   666,   598,    -1,   479,   124,   711,   667,    -1,   235,
     668,    -1,   143,   235,    -1,   669,   207,    -1,   722,   262,
     668,    -1,   159,    -1,   129,   335,   715,    -1,    33,   239,
     711,    -1,    48,   235,    -1,    62,   235,    -1,    -1,   670,
     713,    -1,    -1,   155,   704,    -1,    -1,   125,    -1,   124,
      -1,   145,    -1,    -1,   145,   242,    -1,    -1,   124,   603,
      -1,    -1,   675,   711,   674,   676,    -1,   675,   525,    -1,
     105,    -1,   106,    -1,    -1,   704,    -1,   713,    -1,    -1,
      41,   678,   679,    -1,   679,   406,   680,    -1,   680,    -1,
      -1,   731,   681,   682,    -1,   238,   264,   214,    47,    -1,
     208,    88,    -1,   136,    -1,   205,    -1,   159,    -1,   235,
      -1,    62,    -1,    48,    -1,   107,    -1,   222,    -1,    -1,
     615,    -1,    -1,    56,   684,   685,    -1,   685,   406,   686,
      -1,   686,    -1,    62,    -1,    48,    -1,   208,    88,    -1,
      -1,    53,   688,    48,   159,   243,   242,    -1,    44,   544,
      -1,   259,   713,    -1,    -1,    45,   101,   694,   693,   141,
     242,   692,   695,   144,   239,   711,   696,   699,   702,   631,
      -1,    45,   239,   711,   124,    48,    -1,    45,   101,   124,
      48,    -1,    -1,   157,    -1,    -1,    97,    -1,   162,    -1,
      -1,   360,    -1,   138,    -1,    -1,    95,   697,    -1,   697,
     698,    -1,   698,    -1,   241,    87,   704,    -1,   195,   112,
      87,   704,    -1,   112,    87,   704,    -1,   113,    87,   704,
      -1,    -1,   156,   700,    -1,   700,   701,    -1,   701,    -1,
     241,    87,   704,    -1,   234,    87,   704,    -1,    -1,   138,
     190,   156,    -1,   242,    -1,   703,   242,    -1,   242,    -1,
     134,    -1,   703,    -1,   190,    -1,   160,    -1,   301,    -1,
     215,    -1,   121,    -1,   189,    -1,   134,    -1,   275,   703,
      -1,   297,   703,    -1,   296,   703,    -1,   709,    -1,   707,
      -1,   713,   411,   398,    -1,   713,   411,   713,   411,   398,
      -1,   544,    -1,   713,    -1,   713,   411,   713,    -1,   411,
     713,   411,   713,    -1,   713,   411,   713,   411,   713,    -1,
     713,    -1,   713,   411,   713,    -1,   411,   713,    -1,   713,
      -1,   713,   411,   713,    -1,   411,   713,    -1,   713,    -1,
     713,   411,   713,    -1,   137,    -1,   716,    -1,   713,    -1,
     242,    -1,   154,    -1,   714,    -1,   714,   410,   714,    -1,
      72,    -1,    22,    -1,   307,    -1,    73,    -1,    78,    -1,
      79,    -1,    25,    -1,    80,    -1,    26,    -1,    81,    -1,
      27,    -1,    83,    -1,    84,    -1,    85,    -1,    88,    -1,
     272,    -1,    91,    -1,    92,    -1,   393,    -1,    29,    -1,
       4,    -1,    30,    -1,    94,    -1,    31,    -1,   268,    -1,
      97,    -1,    35,    -1,   101,    -1,   274,    -1,   275,    -1,
     323,    -1,   104,    -1,   107,    -1,   114,    -1,    37,    -1,
     201,    -1,   110,    -1,   384,    -1,   278,    -1,   115,    -1,
      39,    -1,    40,    -1,   117,    -1,   279,    -1,   108,    -1,
     111,    -1,   125,    -1,   118,    -1,   119,    -1,   120,    -1,
      41,    -1,   129,    -1,   127,    -1,   133,    -1,     5,    -1,
     136,    -1,   340,    -1,   341,    -1,   140,    -1,   146,    -1,
     147,    -1,   392,    -1,   143,    -1,   343,    -1,    43,    -1,
       6,    -1,   153,    -1,   157,    -1,    46,    -1,   159,    -1,
     174,    -1,    48,    -1,   163,    -1,   168,    -1,   165,    -1,
     166,    -1,   164,    -1,   167,    -1,   169,    -1,   175,    -1,
     176,    -1,   177,    -1,   178,    -1,   179,    -1,   180,    -1,
     351,    -1,   181,    -1,   353,    -1,   352,    -1,   354,    -1,
     182,    -1,   183,    -1,   186,    -1,     7,    -1,   185,    -1,
     188,    -1,    51,    -1,   191,    -1,   193,    -1,   202,    -1,
     356,    -1,     8,    -1,   206,    -1,   207,    -1,   208,    -1,
     290,    -1,   209,    -1,   212,    -1,   213,    -1,   210,    -1,
     211,    -1,   171,    -1,   172,    -1,   218,    -1,    54,    -1,
     220,    -1,    55,    -1,    56,    -1,   222,    -1,   223,    -1,
      57,    -1,    58,    -1,   226,    -1,   227,    -1,   228,    -1,
      59,    -1,   363,    -1,   230,    -1,   231,    -1,   292,    -1,
     364,    -1,   232,    -1,    62,    -1,   387,    -1,   391,    -1,
     389,    -1,    63,    -1,    64,    -1,   235,    -1,    66,    -1,
     294,    -1,   237,    -1,    68,    -1,   240,    -1,   295,    -1,
     245,    -1,    69,    -1,   296,    -1,   297,    -1,   246,    -1,
     253,    -1,   254,    -1,   258,    -1,   262,    -1,   379,    -1,
     266,    -1,   381,    -1,    -1,   229,   719,   718,   720,    -1,
      -1,   194,    -1,   721,   724,    -1,   720,   406,   721,   724,
      -1,    -1,   127,    -1,   157,    -1,   231,    -1,    -1,   157,
      -1,   231,    -1,   127,    -1,    -1,   157,   411,    -1,   231,
     411,    -1,   127,   411,    -1,   410,   714,   639,   544,    -1,
     725,   639,   728,    -1,   410,   410,   723,   725,   639,   728,
      -1,   245,   146,   153,   726,    -1,   271,   229,   640,   728,
      -1,   356,   639,   727,    -1,   356,   335,   715,   639,   727,
      -1,   713,    -1,   214,   254,    -1,   214,    94,    -1,   220,
     214,    -1,   230,    -1,   242,    -1,   356,   404,   242,   405,
      -1,   544,    -1,   102,    -1,   192,    -1,    74,    -1,    -1,
      47,   731,   730,   732,    -1,   239,    -1,   238,    -1,   733,
      -1,   732,   406,   733,    -1,   711,   584,   734,    -1,   214,
      -1,   265,    -1,   162,   265,    -1,   214,   157,    -1,    70,
     731,    -1,     5,   711,   193,   584,    -1,     5,   712,     4,
      -1,    -1,     5,   712,   214,   737,   738,   586,   598,    -1,
     739,    -1,   713,   740,    -1,   119,    -1,     7,    -1,   119,
      -1,     7,    -1,     8,    -1,     6,    -1,    -1,   742,   741,
     404,   644,   405,    -1,     9,    -1,    11,    -1,    13,    -1,
      12,    -1,    14,    -1,    -1,   225,   744,   747,   192,   757,
     124,   758,    -1,    -1,   128,   746,   747,   192,   757,   243,
     758,   763,   764,    -1,   748,    -1,    74,   205,    -1,    74,
      -1,   749,    -1,   748,   406,   749,    -1,    -1,    60,   750,
     760,    -1,    -1,    42,   751,   760,    -1,    -1,    71,   752,
     760,    -1,    -1,   216,   753,   760,    -1,    36,    -1,   257,
      -1,   139,    -1,    23,    -1,    33,    -1,    38,    -1,    40,
      -1,   218,    -1,   232,    -1,   206,    -1,   118,    -1,   128,
     194,    -1,    61,   100,    -1,    68,    -1,    33,   240,   238,
      -1,    47,   238,    -1,    55,    62,    -1,    55,    29,    -1,
      -1,    75,    -1,   756,   754,   755,    -1,   756,    -1,   237,
     242,    -1,   392,   242,    -1,   393,   242,    -1,   398,    -1,
     713,   411,   398,    -1,   398,   411,   398,    -1,   711,    -1,
     759,    -1,   758,   406,   759,    -1,   715,   341,    87,   242,
      -1,   715,   341,    87,   356,   242,    -1,   715,    -1,    -1,
     404,   761,   405,    -1,   761,   406,   762,    -1,   762,    -1,
     713,    -1,    -1,   221,   755,    -1,   221,   233,    -1,   221,
     266,    -1,   221,    51,    -1,    -1,   264,   765,    -1,   765,
     766,    -1,   766,    -1,   128,   194,    -1,   176,   602,    -1,
     177,   602,    -1,   175,   602,    -1,    -1,    26,   768,   769,
      -1,    -1,   379,    -1,    31,    -1,    57,    -1,    57,   243,
      59,   713,    -1,    59,   713,    -1,    -1,   774,    -1,    -1,
     255,   779,   775,   526,    -1,   774,    -1,   777,    -1,    -1,
      -1,   778,   593,   598,    -1,    -1,   109,    -1,    74,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   609,   609,   624,   626,   628,   629,   630,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,   664,   670,   668,   679,   681,   683,   688,   693,   698,
     703,   708,   725,   730,   735,   748,   746,   767,   766,   783,
     791,   790,   805,   807,   809,   811,   811,   814,   817,   816,
     819,   818,   824,   822,   838,   840,   842,   844,   846,   848,
     850,   853,   855,   857,   859,   861,   863,   865,   867,   868,
     869,   870,   871,   872,   873,   874,   875,   876,   877,   878,
     879,   880,   881,   894,   895,   896,   897,   898,   900,   902,
     903,   904,   905,   906,   907,   909,   911,   912,   913,   915,
     917,   918,   920,   922,   923,   925,   927,   929,   931,   933,
     935,   936,   938,   940,   943,   945,   948,   950,   956,   963,
     967,   973,   975,   978,   980,   984,   982,  1000,  1002,  1003,
    1004,  1006,  1008,  1010,  1012,  1015,  1017,  1020,  1021,  1022,
    1023,  1024,  1026,  1027,  1029,  1031,  1033,  1035,  1037,  1038,
    1039,  1040,  1041,  1042,  1044,  1046,  1046,  1052,  1052,  1059,
    1061,  1062,  1064,  1066,  1067,  1068,  1070,  1072,  1073,  1074,
    1075,  1077,  1080,  1081,  1084,  1086,  1087,  1089,  1096,  1098,
    1100,  1102,  1104,  1106,  1107,  1109,  1111,  1113,  1115,  1117,
    1119,  1121,  1123,  1125,  1127,  1128,  1129,  1130,  1131,  1132,
    1133,  1135,  1137,  1138,  1141,  1143,  1148,  1150,  1152,  1154,
    1157,  1159,  1160,  1161,  1163,  1165,  1166,  1167,  1168,  1170,
    1172,  1173,  1174,  1175,  1176,  1178,  1180,  1182,  1184,  1185,
    1187,  1189,  1190,  1192,  1194,  1196,  1198,  1200,  1202,  1204,
    1206,  1214,  1212,  1240,  1241,  1242,  1244,  1247,  1249,  1250,
    1252,  1251,  1258,  1265,  1257,  1275,  1281,  1286,  1287,  1294,
    1295,  1296,  1302,  1308,  1314,  1315,  1317,  1319,  1321,  1323,
    1325,  1327,  1328,  1330,  1332,  1333,  1335,  1337,  1338,  1339,
    1344,  1351,  1358,  1365,  1374,  1373,  1378,  1380,  1382,  1384,
    1385,  1390,  1388,  1400,  1398,  1410,  1408,  1419,  1421,  1423,
    1425,  1427,  1429,  1430,  1434,  1432,  1445,  1443,  1454,  1456,
    1458,  1460,  1462,  1464,  1465,  1466,  1467,  1471,  1469,  1482,
    1480,  1489,  1491,  1493,  1506,  1510,  1509,  1512,  1511,  1516,
    1515,  1523,  1525,  1526,  1527,  1529,  1533,  1535,  1537,  1539,
    1541,  1543,  1549,  1550,  1551,  1552,  1558,  1564,  1565,  1569,
    1572,  1574,  1582,  1592,  1594,  1595,  1602,  1613,  1616,  1619,
    1621,  1623,  1625,  1626,  1627,  1628,  1630,  1632,  1635,  1636,
    1639,  1642,  1644,  1646,  1648,  1649,  1650,  1651,  1652,  1653,
    1654,  1655,  1656,  1657,  1658,  1659,  1660,  1661,  1662,  1663,
    1664,  1665,  1666,  1667,  1668,  1669,  1670,  1671,  1672,  1673,
    1674,  1675,  1677,  1681,  1684,  1686,  1687,  1688,  1689,  1690,
    1691,  1692,  1693,  1694,  1695,  1696,  1697,  1698,  1699,  1700,
    1701,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,
    1711,  1712,  1713,  1715,  1717,  1720,  1723,  1725,  1727,  1729,
    1730,  1731,  1732,  1733,  1734,  1735,  1736,  1737,  1738,  1739,
    1740,  1741,  1742,  1743,  1744,  1745,  1746,  1747,  1748,  1749,
    1750,  1751,  1752,  1753,  1754,  1755,  1757,  1759,  1761,  1763,
    1764,  1769,  1774,  1779,  1780,  1781,  1782,  1783,  1784,  1785,
    1786,  1789,  1792,  1793,  1794,  1796,  1797,  1799,  1801,  1803,
    1805,  1807,  1809,  1811,  1813,  1815,  1817,  1819,  1821,  1826,
    1828,  1830,  1835,  1837,  1839,  1844,  1845,  1847,  1849,  1851,
    1853,  1855,  1857,  1859,  1861,  1863,  1865,  1867,  1871,  1873,
    1875,  1877,  1879,  1882,  1884,  1890,  1895,  1897,  1899,  1901,
    1903,  1905,  1907,  1909,  1914,  1919,  1921,  1923,  1925,  1927,
    1931,  1933,  1935,  1937,  1939,  1941,  1943,  1944,  1946,  1948,
    1950,  1952,  1954,  1956,  1958,  1960,  1962,  1964,  1966,  1968,
    1975,  1982,  1989,  1996,  2003,  2010,  2014,  2019,  2021,  2023,
    2029,  2031,  2033,  2035,  2037,  2042,  2045,  2047,  2049,  2052,
    2054,  2056,  2058,  2061,  2063,  2060,  2066,  2068,  2070,  2072,
    2074,  2079,  2077,  2086,  2088,  2089,  2090,  2091,  2092,  2093,
    2094,  2095,  2099,  2098,  2103,  2105,  2107,  2109,  2112,  2111,
    2116,  2118,  2120,  2122,  2124,  2126,  2129,  2128,  2133,  2140,
    2147,  2149,  2151,  2153,  2154,  2155,  2156,  2158,  2162,  2160,
    2170,  2173,  2172,  2180,  2186,  2189,  2188,  2196,  2202,  2205,
    2207,  2208,  2212,  2211,  2225,  2228,  2230,  2232,  2234,  2240,
    2247,  2256,  2255,  2259,  2262,  2264,  2267,  2274,  2281,  2283,
    2284,  2285,  2286,  2287,  2288,  2289,  2290,  2291,  2292,  2293,
    2294,  2296,  2298,  2299,  2301,  2303,  2306,  2308,  2311,  2313,
    2321,  2323,  2323,  2333,  2335,  2342,  2344,  2346,  2349,  2352,
    2354,  2360,  2372,  2374,  2378,  2376,  2390,  2393,  2396,  2398,
    2399,  2402,  2405,  2404,  2418,  2425,  2431,  2439,  2445,  2448,
    2450,  2451,  2452,  2453,  2455,  2457,  2458,  2459,  2460,  2462,
    2465,  2464,  2477,  2479,  2481,  2483,  2485,  2497,  2495,  2504,
    2517,  2516,  2531,  2539,  2539,  2549,  2556,  2564,  2566,  2568,
    2572,  2574,  2577,  2579,  2586,  2588,  2585,  2598,  2604,  2596,
    2612,  2614,  2615,  2616,  2619,  2621,  2623,  2625,  2627,  2636,
    2638,  2639,  2641,  2640,  2649,  2651,  2652,  2654,  2656,  2658,
    2660,  2660,  2661,  2661,  2664,  2666,  2668,  2670,  2673,  2682,
    2683,  2686,  2688,  2693,  2691,  2704,  2706,  2708,  2714,  2721,
    2723,  2730,  2728,  2744,  2750,  2756,  2758,  2764,  2762,  2777,
    2775,  2785,  2784,  2788,  2787,  2793,  2795,  2797,  2804,  2813,
    2815,  2818,  2820,  2822,  2824,  2826,  2838,  2840,  2844,  2844,
    2848,  2851,  2857,  2864,  2871,  2879,  2889,  2893,  2898,  2897,
    2904,  2912,  2914,  2916,  2918,  2924,  2926,  2933,  2939,  2943,
    2948,  2950,  2952,  2954,  2956,  2958,  2960,  2962,  2964,  2966,
    2968,  2970,  2976,  2974,  2985,  2989,  2991,  2993,  2995,  2996,
    3004,  3002,  3012,  3014,  3017,  3016,  3018,  3019,  3020,  3021,
    3022,  3023,  3024,  3025,  3026,  3027,  3029,  3031,  3035,  3033,
    3042,  3044,  3046,  3048,  3049,  3053,  3051,  3065,  3080,  3089,
    3088,  3104,  3112,  3118,  3120,  3122,  3124,  3125,  3128,  3130,
    3131,  3133,  3135,  3137,  3139,  3141,  3143,  3149,  3150,  3152,
    3154,  3156,  3158,  3160,  3162,  3164,  3166,  3171,  3173,  3176,
    3178,  3184,  3186,  3187,  3188,  3189,  3190,  3191,  3193,  3194,
    3195,  3196,  3202,  3204,  3206,  3208,  3212,  3215,  3221,  3226,
    3231,  3238,  3240,  3241,  3243,  3245,  3246,  3249,  3251,  3254,
    3256,  3266,  3268,  3269,  3271,  3278,  3287,  3289,  3290,  3291,
    3292,  3293,  3294,  3295,  3296,  3297,  3298,  3299,  3300,  3301,
    3302,  3303,  3304,  3305,  3306,  3307,  3308,  3309,  3310,  3311,
    3312,  3313,  3314,  3315,  3316,  3317,  3318,  3319,  3320,  3321,
    3322,  3323,  3324,  3325,  3326,  3327,  3328,  3329,  3330,  3331,
    3332,  3333,  3334,  3335,  3336,  3337,  3338,  3339,  3340,  3341,
    3342,  3343,  3344,  3345,  3346,  3347,  3348,  3349,  3350,  3351,
    3352,  3353,  3354,  3355,  3356,  3357,  3358,  3359,  3360,  3361,
    3362,  3363,  3364,  3365,  3366,  3367,  3368,  3369,  3370,  3371,
    3372,  3373,  3374,  3375,  3376,  3377,  3378,  3379,  3380,  3381,
    3382,  3383,  3384,  3385,  3386,  3387,  3388,  3389,  3390,  3391,
    3392,  3393,  3394,  3395,  3396,  3397,  3398,  3399,  3400,  3401,
    3402,  3403,  3404,  3405,  3406,  3407,  3408,  3409,  3410,  3411,
    3412,  3413,  3414,  3415,  3416,  3417,  3418,  3419,  3420,  3421,
    3422,  3423,  3424,  3425,  3426,  3427,  3428,  3429,  3430,  3431,
    3432,  3433,  3434,  3435,  3436,  3437,  3438,  3439,  3440,  3441,
    3442,  3443,  3444,  3450,  3448,  3460,  3462,  3464,  3466,  3468,
    3470,  3471,  3472,  3475,  3477,  3478,  3479,  3482,  3484,  3485,
    3486,  3489,  3494,  3499,  3504,  3511,  3518,  3528,  3534,  3544,
    3546,  3547,  3548,  3551,  3553,  3566,  3568,  3569,  3570,  3578,
    3576,  3585,  3587,  3589,  3591,  3593,  3597,  3599,  3600,  3601,
    3603,  3611,  3618,  3625,  3624,  3636,  3638,  3640,  3642,  3644,
    3646,  3647,  3648,  3650,  3649,  3658,  3660,  3661,  3662,  3663,
    3669,  3667,  3686,  3684,  3702,  3704,  3705,  3707,  3709,  3712,
    3711,  3713,  3713,  3714,  3714,  3715,  3715,  3716,  3717,  3718,
    3719,  3720,  3721,  3722,  3723,  3724,  3725,  3726,  3727,  3728,
    3729,  3730,  3731,  3732,  3733,  3737,  3739,  3742,  3744,  3747,
    3758,  3768,  3780,  3793,  3805,  3817,  3827,  3829,  3837,  3852,
    3854,  3858,  3864,  3866,  3868,  3870,  3890,  3891,  3895,  3899,
    3903,  3909,  3911,  3913,  3915,  3917,  3919,  3924,  3929,  3936,
    3935,  3939,  3941,  3943,  3946,  3951,  3956,  3968,  3970,  3974,
    3972,  3994,  3996,  3998,  4004,  4003,  4020,  4022,  4023
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "END_OF_INPUT", "CLOSE_SYM", "HANDLER_SYM", 
  "LAST_SYM", "NEXT_SYM", "PREV_SYM", "EQ", "EQUAL_SYM", "GE", "GT_SYM", 
  "LE", "LT", "NE", "IS", "SHIFT_LEFT", "SHIFT_RIGHT", "SET_VAR", 
  "ABORT_SYM", "ADD", "AFTER_SYM", "ALTER", "ANALYZE_SYM", "AVG_SYM", 
  "BEGIN_SYM", "BINLOG_SYM", "CHANGE", "CLIENT_SYM", "COMMENT_SYM", 
  "COMMIT_SYM", "COUNT_SYM", "CREATE", "CROSS", "CUBE_SYM", "DELETE_SYM", 
  "DO_SYM", "DROP", "EVENTS_SYM", "EXECUTE_SYM", "FLUSH_SYM", "INSERT", 
  "IO_THREAD", "KILL_SYM", "LOAD", "LOCKS_SYM", "LOCK_SYM", "MASTER_SYM", 
  "MAX_SYM", "MIN_SYM", "NONE_SYM", "OPTIMIZE", "PURGE", "REPAIR", 
  "REPLICATION", "RESET_SYM", "ROLLBACK_SYM", "ROLLUP_SYM", 
  "SAVEPOINT_SYM", "SELECT_SYM", "SHOW", "SLAVE", "SQL_THREAD", 
  "START_SYM", "STD_SYM", "STOP_SYM", "SUM_SYM", "SUPER_SYM", 
  "TRUNCATE_SYM", "UNLOCK_SYM", "UPDATE_SYM", "ACTION", "AGGREGATE_SYM", 
  "ALL", "AND", "AS", "ASC", "AUTO_INC", "AVG_ROW_LENGTH", "BACKUP_SYM", 
  "BERKELEY_DB_SYM", "BINARY", "BIT_SYM", "BOOL_SYM", "BOOLEAN_SYM", 
  "BOTH", "BY", "CACHE_SYM", "CASCADE", "CAST_SYM", "CHARSET", 
  "CHECKSUM_SYM", "CHECK_SYM", "COMMITTED_SYM", "COLUMNS", "COLUMN_SYM", 
  "CONCURRENT", "CONSTRAINT", "CONVERT_SYM", "DATABASES", "DATA_SYM", 
  "DEFAULT", "DELAYED_SYM", "DELAY_KEY_WRITE_SYM", "DESC", "DESCRIBE", 
  "DES_KEY_FILE", "DISABLE_SYM", "DISTINCT", "DYNAMIC_SYM", "ENABLE_SYM", 
  "ENCLOSED", "ESCAPED", "DIRECTORY_SYM", "ESCAPE_SYM", "EXISTS", 
  "EXTENDED_SYM", "FILE_SYM", "FIRST_SYM", "FIXED_SYM", "FLOAT_NUM", 
  "FORCE_SYM", "FOREIGN", "FROM", "FULL", "FULLTEXT_SYM", "GLOBAL_SYM", 
  "GRANT", "GRANTS", "GREATEST_SYM", "GROUP", "HAVING", "HEAP_SYM", 
  "HEX_NUM", "HIGH_PRIORITY", "HOSTS_SYM", "IDENT", "IGNORE_SYM", "INDEX", 
  "INDEXES", "INFILE", "INNER_SYM", "INNOBASE_SYM", "INTO", "IN_SYM", 
  "ISOLATION", "ISAM_SYM", "JOIN_SYM", "KEYS", "KEY_SYM", "LEADING", 
  "LEAST_SYM", "LEVEL_SYM", "LEX_HOSTNAME", "LIKE", "LINES", "LOCAL_SYM", 
  "LOG_SYM", "LOGS_SYM", "LONG_NUM", "LONG_SYM", "LOW_PRIORITY", 
  "MASTER_HOST_SYM", "MASTER_USER_SYM", "MASTER_LOG_FILE_SYM", 
  "MASTER_LOG_POS_SYM", "MASTER_PASSWORD_SYM", "MASTER_PORT_SYM", 
  "MASTER_CONNECT_RETRY_SYM", "MASTER_SERVER_ID_SYM", 
  "RELAY_LOG_FILE_SYM", "RELAY_LOG_POS_SYM", "MATCH", "MAX_ROWS", 
  "MAX_CONNECTIONS_PER_HOUR", "MAX_QUERIES_PER_HOUR", 
  "MAX_UPDATES_PER_HOUR", "MEDIUM_SYM", "MERGE_SYM", "MEMORY_SYM", 
  "MIN_ROWS", "MYISAM_SYM", "NATIONAL_SYM", "NATURAL", "NEW_SYM", 
  "NCHAR_SYM", "NOT", "NO_SYM", "NULL_SYM", "NUM", "OFFSET_SYM", "ON", 
  "OPEN_SYM", "OPTION", "OPTIONALLY", "OR", "OR_OR_CONCAT", "ORDER_SYM", 
  "OUTER", "OUTFILE", "DUMPFILE", "PACK_KEYS_SYM", "PARTIAL", 
  "PRIMARY_SYM", "PRIVILEGES", "PROCESS", "PROCESSLIST_SYM", "QUERY_SYM", 
  "RAID_0_SYM", "RAID_STRIPED_SYM", "RAID_TYPE", "RAID_CHUNKS", 
  "RAID_CHUNKSIZE", "READ_SYM", "REAL_NUM", "REFERENCES", "REGEXP", 
  "RELOAD", "RENAME", "REPEATABLE_SYM", "REQUIRE_SYM", "RESOURCES", 
  "RESTORE_SYM", "RESTRICT", "REVOKE", "ROWS_SYM", "ROW_FORMAT_SYM", 
  "ROW_SYM", "SET", "SERIALIZABLE_SYM", "SESSION_SYM", "SHUTDOWN", 
  "SSL_SYM", "STARTING", "STATUS_SYM", "STRAIGHT_JOIN", "SUBJECT_SYM", 
  "TABLES", "TABLE_SYM", "TEMPORARY", "TERMINATED", "TEXT_STRING", 
  "TO_SYM", "TRAILING", "TRANSACTION_SYM", "TYPE_SYM", "FUNC_ARG0", 
  "FUNC_ARG1", "FUNC_ARG2", "FUNC_ARG3", "UDF_RETURNS_SYM", 
  "UDF_SONAME_SYM", "UDF_SYM", "UNCOMMITTED_SYM", "UNION_SYM", 
  "UNIQUE_SYM", "USAGE", "USE_FRM", "USE_SYM", "USING", "VALUES", 
  "VARIABLES", "WHERE", "WITH", "WRITE_SYM", "X509_SYM", "XOR", 
  "COMPRESSED_SYM", "BIGINT", "BLOB_SYM", "CHAR_SYM", "CHANGED", 
  "COALESCE", "DATETIME", "DATE_SYM", "DECIMAL_SYM", "DOUBLE_SYM", "ENUM", 
  "FAST_SYM", "FLOAT_SYM", "INT_SYM", "LIMIT", "LONGBLOB", "LONGTEXT", 
  "MEDIUMBLOB", "MEDIUMINT", "MEDIUMTEXT", "NUMERIC_SYM", "PRECISION", 
  "QUICK", "REAL", "SIGNED_SYM", "SMALLINT", "STRING_SYM", "TEXT_SYM", 
  "TIMESTAMP", "TIME_SYM", "TINYBLOB", "TINYINT", "TINYTEXT", 
  "ULONGLONG_NUM", "UNSIGNED", "VARBINARY", "VARCHAR", "VARYING", 
  "ZEROFILL", "AGAINST", "ATAN", "BETWEEN_SYM", "BIT_AND", "BIT_OR", 
  "CASE_SYM", "CONCAT", "CONCAT_WS", "CURDATE", "CURTIME", "DATABASE", 
  "DATE_ADD_INTERVAL", "DATE_SUB_INTERVAL", "DAY_HOUR_SYM", 
  "DAY_MINUTE_SYM", "DAY_SECOND_SYM", "DAY_SYM", "DECODE_SYM", 
  "DES_ENCRYPT_SYM", "DES_DECRYPT_SYM", "ELSE", "ELT_FUNC", "ENCODE_SYM", 
  "ENCRYPT", "EXPORT_SET", "EXTRACT_SYM", "FIELD_FUNC", "FORMAT_SYM", 
  "FOR_SYM", "FROM_UNIXTIME", "GROUP_UNIQUE_USERS", "HOUR_MINUTE_SYM", 
  "HOUR_SECOND_SYM", "HOUR_SYM", "IDENTIFIED_SYM", "IF", "INSERT_METHOD", 
  "INTERVAL_SYM", "LAST_INSERT_ID", "LEFT", "LOCATE", "MAKE_SET_SYM", 
  "MASTER_POS_WAIT", "MINUTE_SECOND_SYM", "MINUTE_SYM", "MODE_SYM", 
  "MODIFY_SYM", "MONTH_SYM", "NOW_SYM", "PASSWORD", "POSITION_SYM", 
  "PROCEDURE", "RAND", "REPLACE", "RIGHT", "ROUND", "SECOND_SYM", 
  "SHARE_SYM", "SUBSTRING", "SUBSTRING_INDEX", "TRIM", "UDA_CHAR_SUM", 
  "UDA_FLOAT_SUM", "UDA_INT_SUM", "UDF_CHAR_FUNC", "UDF_FLOAT_FUNC", 
  "UDF_INT_FUNC", "UNIQUE_USERS", "UNIX_TIMESTAMP", "USER", "WEEK_SYM", 
  "WHEN_SYM", "WORK_SYM", "YEAR_MONTH_SYM", "YEAR_SYM", "YEARWEEK", 
  "BENCHMARK_SYM", "END", "THEN_SYM", "SQL_BIG_RESULT", "SQL_CACHE_SYM", 
  "SQL_CALC_FOUND_ROWS", "SQL_NO_CACHE_SYM", "SQL_SMALL_RESULT", 
  "SQL_BUFFER_RESULT", "ISSUER_SYM", "CIPHER_SYM", "'|'", "'&'", "'-'", 
  "'+'", "'*'", "'/'", "'%'", "'~'", "NEG", "'^'", "'('", "')'", "','", 
  "'!'", "'{'", "'}'", "'@'", "'.'", "$accept", "query", "verb_clause", 
  "change", "@1", "master_defs", "master_def", "create", "@2", "@3", "@4", 
  "create2", "create2a", "@5", "create3", "@6", "@7", "create_select", 
  "@8", "opt_as", "opt_table_options", "table_options", "table_option", 
  "opt_if_not_exists", "opt_create_table_options", "create_table_options", 
  "create_table_option", "table_types", "row_types", "raid_types", 
  "merge_insert_types", "opt_select_from", "udf_func_type", "udf_type", 
  "field_list", "field_list_item", "column_def", "key_def", 
  "check_constraint", "opt_constraint", "field_spec", "@9", "type", "@10", 
  "@11", "char", "varchar", "int_type", "real_type", "float_options", 
  "precision", "field_options", "field_opt_list", "field_option", 
  "opt_len", "opt_precision", "opt_attribute", "opt_attribute_list", 
  "attribute", "opt_binary", "references", "opt_on_delete", 
  "opt_on_delete_list", "opt_on_delete_item", "delete_option", "key_type", 
  "key_or_index", "keys_or_index", "opt_unique_or_fulltext", "key_list", 
  "key_part", "opt_ident", "string_list", "alter", "@12", "alter_list", 
  "add_column", "alter_list_item", "@13", "@14", "@15", "opt_column", 
  "opt_ignore", "opt_restrict", "opt_place", "opt_to", "slave", "start", 
  "@16", "slave_thread_opts", "slave_thread_opt", "restore", "@17", 
  "backup", "@18", "repair", "@19", "opt_mi_repair_type", 
  "mi_repair_types", "mi_repair_type", "analyze", "@20", "check", "@21", 
  "opt_mi_check_type", "mi_check_types", "mi_check_type", "optimize", 
  "@22", "rename", "@23", "table_to_table_list", "table_to_table", 
  "select", "select_init", "@24", "@25", "select_part2", "@26", 
  "select_into", "select_from", "select_options", "select_option_list", 
  "select_option", "select_lock_type", "select_item_list", "select_item", 
  "remember_name", "remember_end", "select_item2", "select_alias", 
  "optional_braces", "expr", "expr_expr", "no_in_expr", "no_and_expr", 
  "simple_expr", "udf_expr_list", "sum_expr", "@27", "@28", "in_sum_expr", 
  "@29", "cast_type", "expr_list", "@30", "expr_list2", "ident_list_arg", 
  "ident_list", "@31", "ident_list2", "opt_expr", "opt_else", "when_list", 
  "@32", "when_list2", "opt_pad", "join_table_list", "@33", "@34", "@35", 
  "normal_join", "join_table", "@36", "opt_outer", "opt_key_definition", 
  "key_usage_list", "@37", "key_usage_list2", "using_list", "interval", 
  "table_alias", "opt_table_alias", "opt_all", "where_clause", 
  "having_clause", "@38", "opt_escape", "group_clause", "group_list", 
  "olap_opt", "opt_order_clause", "order_clause", "@39", "order_list", 
  "order_dir", "limit_clause", "@40", "limit_options", 
  "delete_limit_clause", "ULONG_NUM", "ulonglong_num", "procedure_clause", 
  "@41", "procedure_list", "procedure_list2", "procedure_item", 
  "opt_into", "@42", "do", "@43", "drop", "@44", "table_list", 
  "table_name", "if_exists", "opt_temporary", "insert", "@45", "@46", 
  "replace", "@47", "@48", "insert_lock_option", "replace_lock_option", 
  "insert2", "insert_table", "insert_field_spec", "@49", "opt_field_spec", 
  "fields", "insert_values", "@50", "@51", "values_list", "ident_eq_list", 
  "ident_eq_value", "equal", "opt_equal", "no_braces", "@52", 
  "opt_values", "values", "expr_or_default", "update", "@53", 
  "update_list", "opt_low_priority", "delete", "@54", "single_multi", 
  "@55", "@56", "@57", "table_wild_list", "table_wild_one", "opt_wild", 
  "opt_delete_options", "opt_delete_option", "truncate", "opt_table_sym", 
  "show", "@58", "show_param", "@59", "opt_db", "wild", "opt_full", 
  "from_or_in", "binlog_in", "binlog_from", "describe", "@60", 
  "describe_command", "opt_describe_column", "flush", "@61", 
  "flush_options", "flush_option", "@62", "opt_table_list", "reset", 
  "@63", "reset_options", "reset_option", "purge", "@64", "kill", "use", 
  "load", "@65", "opt_local", "load_data_lock", "opt_duplicate", 
  "opt_field_term", "field_term_list", "field_term", "opt_line_term", 
  "line_term_list", "line_term", "opt_ignore_lines", "text_literal", 
  "text_string", "literal", "insert_ident", "table_wild", "order_ident", 
  "simple_ident", "field_ident", "table_ident", "table_ident_ref", 
  "ident", "ident_or_text", "user", "keyword", "set", "@66", "opt_option", 
  "option_value_list", "option_type", "opt_var_type", 
  "opt_var_ident_type", "option_value", "internal_variable_name", 
  "isolation_types", "text_or_password", "set_expr_or_default", "lock", 
  "@67", "table_or_tables", "table_lock_list", "table_lock", 
  "lock_option", "unlock", "handler", "@68", "handler_read_or_scan", 
  "handler_scan_function", "handler_rkey_function", "@69", 
  "handler_rkey_mode", "revoke", "@70", "grant", "@71", 
  "grant_privileges", "grant_privilege_list", "grant_privilege", "@72", 
  "@73", "@74", "@75", "opt_and", "require_list", "require_list_element", 
  "opt_table", "user_list", "grant_user", "opt_column_list", 
  "column_list", "column_list_id", "require_clause", "grant_options", 
  "grant_option_list", "grant_option", "begin", "@76", "opt_work", 
  "commit", "rollback", "savepoint", "opt_union", "union_list", "@77", 
  "union_opt", "optional_order_or_limit", "@78", "union_option", 0
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
     645,   646,   647,   648,   124,    38,    45,    43,    42,    47,
      37,   126,   649,    94,    40,    41,    44,    33,   123,   125,
      64,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   412,   413,   413,   414,   414,   414,   414,   414,   414,
     414,   414,   414,   414,   414,   414,   414,   414,   414,   414,
     414,   414,   414,   414,   414,   414,   414,   414,   414,   414,
     414,   414,   414,   414,   414,   414,   414,   414,   414,   414,
     414,   414,   416,   415,   417,   417,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   420,   419,   421,   419,   419,
     422,   419,   423,   423,   424,   425,   424,   426,   427,   426,
     428,   426,   430,   429,   431,   431,   432,   432,   433,   433,
     434,   435,   435,   436,   436,   437,   437,   438,   438,   438,
     438,   438,   438,   438,   438,   438,   438,   438,   438,   438,
     438,   438,   438,   438,   438,   438,   438,   438,   439,   439,
     439,   439,   439,   439,   439,   440,   440,   440,   440,   441,
     441,   441,   442,   442,   442,   443,   443,   444,   444,   445,
     445,   445,   446,   446,   447,   447,   448,   448,   449,   449,
     449,   450,   450,   451,   451,   453,   452,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   455,   454,   456,   454,   457,
     457,   457,   458,   458,   458,   458,   459,   459,   459,   459,
     459,   460,   460,   460,   461,   461,   461,   462,   463,   463,
     464,   464,   465,   465,   465,   466,   466,   467,   467,   468,
     468,   469,   469,   470,   470,   470,   470,   470,   470,   470,
     470,   471,   471,   471,   472,   472,   473,   473,   474,   474,
     475,   475,   475,   475,   476,   476,   476,   476,   476,   477,
     477,   477,   477,   477,   477,   478,   478,   479,   479,   479,
     480,   480,   480,   481,   481,   482,   482,   483,   483,   484,
     484,   486,   485,   487,   487,   487,   488,   489,   489,   489,
     490,   489,   491,   492,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   489,   489,   489,   493,   493,   494,   494,
     495,   495,   495,   496,   496,   496,   497,   497,   497,   497,
     498,   498,   498,   498,   500,   499,   501,   501,   502,   502,
     502,   504,   503,   506,   505,   508,   507,   509,   509,   510,
     510,   511,   511,   511,   513,   512,   515,   514,   516,   516,
     517,   517,   518,   518,   518,   518,   518,   520,   519,   522,
     521,   523,   523,   524,   525,   527,   526,   528,   526,   530,
     529,   531,   531,   531,   531,   532,   533,   533,   534,   534,
     535,   535,   535,   535,   535,   535,   535,   535,   535,   535,
     536,   536,   536,   537,   537,   537,   538,   539,   540,   541,
     541,   542,   542,   542,   542,   542,   543,   543,   544,   544,
     545,   545,   545,   545,   545,   545,   545,   545,   545,   545,
     545,   545,   545,   545,   545,   545,   545,   545,   545,   545,
     545,   545,   545,   545,   545,   545,   545,   545,   545,   545,
     545,   545,   545,   546,   546,   546,   546,   546,   546,   546,
     546,   546,   546,   546,   546,   546,   546,   546,   546,   546,
     546,   546,   546,   546,   546,   546,   546,   546,   546,   546,
     546,   546,   546,   546,   546,   547,   547,   547,   547,   547,
     547,   547,   547,   547,   547,   547,   547,   547,   547,   547,
     547,   547,   547,   547,   547,   547,   547,   547,   547,   547,
     547,   547,   547,   547,   547,   547,   547,   547,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   549,   549,   550,   550,
     550,   550,   550,   551,   552,   550,   550,   550,   550,   550,
     550,   554,   553,   555,   555,   555,   555,   555,   555,   555,
     555,   555,   557,   556,   558,   558,   559,   559,   561,   560,
     562,   562,   563,   563,   564,   564,   566,   565,   567,   567,
     568,   568,   569,   569,   569,   569,   569,   569,   570,   569,
     569,   571,   569,   569,   569,   572,   569,   569,   569,   573,
     573,   573,   575,   574,   574,   576,   576,   577,   577,   577,
     577,   579,   578,   580,   580,   580,   581,   581,   582,   582,
     582,   582,   582,   582,   582,   582,   582,   582,   582,   582,
     582,   583,   583,   583,   584,   584,   585,   585,   586,   586,
     587,   588,   587,   589,   589,   590,   590,   591,   591,   592,
     592,   592,   593,   593,   595,   594,   596,   596,   597,   597,
     597,   598,   599,   598,   600,   600,   600,   601,   601,   602,
     602,   602,   602,   602,   603,   603,   603,   603,   603,   604,
     605,   604,   606,   606,   607,   607,   608,   610,   609,   609,
     612,   611,   613,   614,   613,   613,   613,   615,   615,   616,
     617,   617,   618,   618,   620,   621,   619,   623,   624,   622,
     625,   625,   625,   625,   626,   626,   627,   627,   628,   629,
     629,   629,   630,   629,   631,   631,   631,   632,   632,   633,
     634,   633,   635,   633,   636,   636,   637,   637,   638,   639,
     639,   640,   640,   642,   641,   643,   643,   644,   644,   645,
     645,   647,   646,   648,   648,   649,   649,   651,   650,   653,
     652,   654,   652,   655,   652,   656,   656,   657,   657,   658,
     658,   659,   659,   660,   660,   661,   662,   662,   664,   663,
     665,   665,   665,   665,   665,   665,   665,   665,   666,   665,
     665,   665,   665,   665,   665,   665,   665,   665,   665,   665,
     667,   667,   668,   668,   669,   669,   670,   670,   671,   671,
     672,   672,   674,   673,   673,   675,   675,   676,   676,   676,
     678,   677,   679,   679,   681,   680,   680,   680,   680,   680,
     680,   680,   680,   680,   680,   680,   682,   682,   684,   683,
     685,   685,   686,   686,   686,   688,   687,   689,   690,   692,
     691,   691,   691,   693,   693,   694,   694,   694,   695,   695,
     695,   696,   696,   697,   697,   698,   698,   698,   698,   699,
     699,   700,   700,   701,   701,   702,   702,   703,   703,   704,
     704,   705,   705,   705,   705,   705,   705,   705,   705,   705,
     705,   705,   706,   706,   707,   707,   708,   709,   709,   709,
     709,   710,   710,   710,   711,   711,   711,   712,   712,   713,
     713,   714,   714,   714,   715,   715,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   716,   716,   716,   716,   716,   716,   716,
     716,   716,   716,   718,   717,   719,   719,   720,   720,   721,
     721,   721,   721,   722,   722,   722,   722,   723,   723,   723,
     723,   724,   724,   724,   724,   724,   724,   724,   725,   726,
     726,   726,   726,   727,   727,   728,   728,   728,   728,   730,
     729,   731,   731,   732,   732,   733,   734,   734,   734,   734,
     735,   736,   736,   737,   736,   738,   738,   739,   739,   740,
     740,   740,   740,   741,   740,   742,   742,   742,   742,   742,
     744,   743,   746,   745,   747,   747,   747,   748,   748,   750,
     749,   751,   749,   752,   749,   753,   749,   749,   749,   749,
     749,   749,   749,   749,   749,   749,   749,   749,   749,   749,
     749,   749,   749,   749,   749,   754,   754,   755,   755,   756,
     756,   756,   757,   757,   757,   757,   758,   758,   759,   759,
     759,   760,   760,   761,   761,   762,   763,   763,   763,   763,
     763,   764,   764,   765,   765,   766,   766,   766,   766,   768,
     767,   769,   769,   770,   771,   771,   772,   773,   773,   775,
     774,   776,   776,   777,   778,   777,   779,   779,   779
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     5,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     0,     7,     0,    10,     4,
       0,     9,     2,     2,     4,     0,     4,     0,     0,     5,
       0,     7,     0,     5,     0,     1,     0,     1,     1,     2,
       1,     0,     3,     0,     1,     1,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     5,     3,     4,     3,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     2,     0,     1,     1,
       1,     1,     1,     3,     1,     1,     2,     2,     5,     8,
       2,     0,     2,     0,     2,     0,     4,     3,     3,     3,
       2,     1,     5,     2,     4,     5,     4,     3,     1,     1,
       1,     4,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     3,     3,     0,     5,     0,     5,     1,
       1,     2,     2,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     0,     3,     1,     5,     0,     1,
       2,     1,     1,     1,     1,     0,     3,     0,     1,     0,
       1,     2,     1,     1,     2,     2,     1,     2,     1,     2,
       2,     0,     1,     4,     3,     6,     0,     1,     2,     1,
       3,     3,     2,     2,     1,     1,     2,     2,     2,     3,
       1,     1,     2,     2,     3,     1,     1,     1,     1,     1,
       0,     1,     1,     4,     2,     1,     4,     0,     1,     1,
       3,     0,     6,     0,     1,     3,     2,     3,     2,     4,
       0,     6,     0,     0,     8,     4,     3,     4,     3,     2,
       2,     6,     5,     3,     1,     1,     0,     1,     0,     1,
       0,     1,     1,     0,     2,     1,     0,     1,     1,     1,
       3,     3,     3,     3,     0,     3,     1,     3,     0,     1,
       1,     0,     6,     0,     6,     0,     5,     0,     1,     1,
       2,     1,     1,     1,     0,     5,     0,     5,     0,     1,
       1,     2,     1,     1,     1,     1,     1,     0,     5,     0,
       4,     1,     3,     3,     1,     0,     4,     0,     6,     0,
       5,     1,     1,     2,     2,     8,     0,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     2,     4,     3,     1,     1,     4,     0,     0,     1,
       1,     0,     2,     2,     1,     1,     0,     2,     1,     1,
       5,     6,     5,     6,     3,     3,     3,     3,     4,     5,
       3,     4,     3,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     5,     5,     6,     3,     3,     3,     3,     4,
       5,     3,     4,     3,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     5,     5,     1,     5,     6,     5,     6,     3,
       3,     3,     4,     5,     3,     4,     3,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     5,     5,     1,     1,     1,
       4,     2,     4,     1,     2,     2,     2,     2,     3,     4,
       6,     9,     2,     6,     6,     6,     3,     4,     6,     8,
       4,     6,     4,     4,     4,     6,     2,     2,     4,     8,
       8,     3,     6,     6,     4,     6,     6,     6,     4,     6,
       4,     6,     8,    10,    12,     6,     4,     6,     6,     4,
       8,    10,     5,     6,     3,     4,     6,     6,     8,     6,
       6,     4,     6,     6,     8,     4,     4,     2,     4,     4,
       6,     4,     3,     8,     6,     4,     6,     4,     8,     6,
       8,     6,     8,     4,     7,     7,     7,     6,     6,     4,
       4,     4,     4,     4,     4,    10,     3,     4,     3,     4,
       6,     4,     4,     6,     6,     6,     0,     1,     4,     4,
       4,     5,     4,     0,     0,     7,    10,     4,     4,     4,
       4,     0,     3,     1,     1,     1,     2,     1,     2,     1,
       1,     1,     0,     2,     1,     3,     1,     3,     0,     2,
       1,     3,     0,     1,     0,     2,     0,     2,     3,     5,
       0,     1,     3,     1,     3,     3,     3,     5,     0,     8,
       7,     0,    10,     6,     7,     0,    10,     6,     4,     1,
       2,     2,     0,     4,    10,     0,     1,     0,     2,     2,
       2,     0,     5,     3,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     1,     0,     2,     0,     1,     0,     2,
       0,     0,     3,     2,     0,     0,     4,     4,     2,     0,
       2,     2,     0,     1,     0,     4,     4,     2,     0,     1,
       1,     0,     0,     3,     1,     3,     3,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     6,     0,     1,     3,     1,     2,     0,     6,     3,
       0,     3,     6,     0,     6,     4,     3,     1,     3,     1,
       0,     2,     0,     1,     0,     0,     7,     0,     0,     6,
       0,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       3,     4,     0,     3,     0,     3,     2,     3,     1,     2,
       0,     3,     0,     5,     3,     1,     3,     1,     3,     1,
       1,     0,     1,     0,     4,     0,     1,     3,     1,     1,
       1,     0,    10,     5,     3,     0,     1,     0,     4,     0,
       6,     0,     5,     0,     6,     1,     3,     2,     4,     0,
       2,     0,     2,     1,     1,     3,     0,     1,     0,     3,
       2,     3,     4,     4,     6,    16,     2,     2,     0,     6,
       4,     2,     2,     2,     3,     1,     3,     3,     2,     2,
       0,     2,     0,     2,     0,     1,     1,     1,     0,     2,
       0,     2,     0,     4,     2,     1,     1,     0,     1,     1,
       0,     3,     3,     1,     0,     3,     4,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     0,     3,
       3,     1,     1,     1,     2,     0,     6,     2,     2,     0,
      15,     5,     4,     0,     1,     0,     1,     1,     0,     1,
       1,     0,     2,     2,     1,     3,     4,     3,     3,     0,
       2,     2,     1,     3,     3,     0,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     3,     5,     1,     1,     3,     4,
       5,     1,     3,     2,     1,     3,     2,     1,     3,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     1,
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
       1,     1,     1,     0,     4,     0,     1,     2,     4,     0,
       1,     1,     1,     0,     1,     1,     1,     0,     2,     2,
       2,     4,     3,     6,     4,     4,     3,     5,     1,     2,
       2,     2,     1,     1,     4,     1,     1,     1,     1,     0,
       4,     1,     1,     1,     3,     3,     1,     1,     2,     2,
       2,     4,     3,     0,     7,     1,     2,     1,     1,     1,
       1,     1,     1,     0,     5,     1,     1,     1,     1,     1,
       0,     7,     0,     9,     1,     2,     1,     1,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     3,     2,     2,     2,     0,     1,     3,     1,     2,
       2,     2,     1,     3,     3,     1,     1,     3,     4,     5,
       1,     0,     3,     3,     1,     1,     0,     2,     2,     2,
       2,     0,     2,     2,     1,     2,     2,     2,     2,     0,
       3,     0,     1,     1,     1,     4,     2,     0,     1,     0,
       4,     1,     1,     0,     0,     3,     0,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     2,     0,   288,     0,  1269,     0,  1273,    76,   817,
     750,   762,   880,   764,     0,     0,     0,     0,   905,     0,
     898,  1274,     0,   349,   838,     0,     0,     0,   836,     0,
     811,     0,     0,   875,   876,  1202,     0,     0,  1200,  1135,
       0,   767,     0,     0,     0,     8,    11,     4,    34,    35,
      28,     6,    25,     5,     9,    22,    24,    32,   344,    14,
      15,    17,    26,    40,    12,    37,    36,    13,     0,    18,
      27,    23,    21,    41,    19,    33,    20,    39,    38,    29,
      16,     7,    10,    30,    31,   996,  1030,  1041,  1069,  1077,
     977,   982,   984,   986,   995,   997,   999,  1002,  1010,  1016,
    1017,  1026,  1040,  1044,  1047,  1072,  1090,  1092,  1093,  1096,
    1097,  1101,  1108,  1112,  1113,  1115,  1118,  1122,   976,   979,
     980,   981,   983,   985,   987,   988,   989,   990,   992,   993,
     998,  1001,  1003,  1007,  1008,  1020,  1012,  1021,  1009,  1015,
    1018,  1023,  1024,  1025,  1022,  1028,  1027,  1029,  1031,   969,
    1034,  1038,  1035,  1036,  1042,  1043,  1045,  1048,  1052,  1050,
    1051,  1053,  1049,  1054,  1087,  1088,  1046,  1055,  1056,  1057,
    1058,  1059,  1060,  1062,  1066,  1067,  1070,  1068,  1071,  1073,
    1074,  1011,  1075,  1078,  1079,  1080,  1082,  1085,  1086,  1083,
    1084,  1089,  1091,  1094,  1095,  1098,  1099,  1100,  1103,  1104,
    1107,  1114,  1117,  1119,  1121,  1125,  1126,  1127,  1128,  1129,
    1131,  1000,   991,  1004,  1005,  1014,  1019,  1081,  1105,  1116,
    1120,  1123,  1124,   978,  1006,  1032,  1033,  1039,  1061,  1064,
    1063,  1065,  1076,  1102,  1106,  1130,  1132,  1013,  1109,  1111,
    1110,  1037,   994,     0,     0,     0,   967,   970,   289,     0,
    1172,  1171,   324,  1271,     0,   128,   252,    80,   251,    81,
       0,    77,    78,     0,     0,   831,     0,     0,   763,     0,
     760,     0,     0,   770,   982,     0,     0,     0,     0,     0,
       0,  1122,     0,     0,     0,   946,     0,   948,     0,     0,
     943,   628,     0,   947,   942,   945,   937,     0,     0,     0,
       0,     0,     0,  1005,  1123,  1124,   944,     0,     0,     0,
     632,     0,     0,   386,   386,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1032,     0,     0,     0,     0,     0,     0,     0,  1061,  1065,
     386,  1076,     0,     0,     0,     0,     0,  1102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   907,   388,   389,   493,   941,   489,   488,   957,   915,
       0,  1169,   337,     0,   315,     0,     0,  1276,   345,   356,
     864,   308,   308,   308,   304,   308,   837,     0,  1180,   815,
     313,   326,     0,   339,   311,     0,  1136,  1133,   908,   815,
     349,     1,     3,   874,   872,   964,   966,   691,  1182,  1183,
       0,     0,     0,  1272,  1270,    42,     0,     0,    81,    79,
       0,     0,   834,   833,     0,   831,   810,   809,   751,   808,
       0,   756,     0,     0,   760,   893,   892,   894,   888,   890,
     889,     0,   895,   891,  1172,   881,   883,   884,   772,   773,
     771,   288,   696,   696,     0,   696,   696,   696,   696,     0,
     502,     0,     0,     0,     0,     0,   628,     0,   626,     0,
     496,     0,     0,     0,     0,   622,   622,   949,   951,   950,
       0,   696,   696,   633,     0,   622,     0,     0,   516,     0,
     517,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   557,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   622,
     622,   622,   622,   622,   622,     0,     0,     0,     0,     0,
       0,     0,   494,   495,     0,   497,     0,   973,   972,  1147,
     971,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     938,     0,   916,     0,   917,   913,     0,     0,     0,     0,
       0,   903,   902,     0,   899,   901,     0,  1277,   369,   362,
     361,   360,   364,   368,   366,   367,   363,   365,   377,   357,
     359,     0,     0,     0,     0,   862,   865,  1146,     0,   248,
     249,     0,   247,  1144,   855,     0,     0,  1145,   862,   860,
       0,     0,   839,     0,     0,   310,   309,   302,   306,   303,
     300,   305,   301,   835,   759,   816,   288,     0,     0,  1220,
    1221,  1217,  1222,  1223,  1211,     0,     0,  1209,     0,  1230,
    1213,  1206,  1227,     0,  1219,  1226,  1215,  1224,  1225,  1218,
       0,  1204,  1207,     0,     0,     0,  1139,   775,     0,   774,
       0,   877,     0,   693,   692,     0,  1181,     0,   968,   261,
     328,   757,     0,     0,    59,     0,    60,     0,     0,   818,
     821,   825,   829,   832,     0,     0,   761,   755,     0,   887,
       0,     0,   896,     0,   697,     0,   611,   603,     0,   611,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   629,   630,   506,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   636,     0,     0,   387,     0,
     521,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     678,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   544,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   454,   562,     0,     0,     0,     0,     0,
       0,     0,   640,   640,   640,     0,     0,   597,     0,     0,
       0,     0,     0,     0,   586,     0,   588,     0,     0,     0,
     733,   730,   729,   732,   731,     0,   498,     0,     0,     0,
       0,     0,     0,     0,   404,   405,   406,   407,   408,   409,
     410,     0,   402,   411,   412,   397,   622,   704,     0,     0,
       0,     0,   395,   394,   400,   396,     0,   487,   417,   419,
       0,   414,     0,   413,   415,   416,   420,   418,   958,   912,
     914,     0,     0,   691,  1170,  1173,   328,     0,   317,   904,
       0,  1275,  1286,   346,  1278,   375,   721,   374,     0,   358,
     868,     0,   846,   858,   847,   859,     0,   840,     0,   852,
       0,   860,   851,   866,   867,   862,     0,   860,     0,     0,
     853,   862,   308,   662,     0,   328,     0,  1251,  1232,  1234,
    1233,  1251,  1229,  1251,  1205,  1228,  1251,     0,     0,   340,
     341,     0,     0,     0,  1140,  1141,  1142,  1134,     0,     0,
     778,   768,   777,   347,   940,   939,   873,   878,   879,   965,
     695,  1069,  1024,     0,   698,  1185,   263,   335,   334,   336,
     333,   332,     0,   325,   329,   330,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,    82,    55,     0,
       0,   823,   819,   964,     0,     0,     0,   827,   807,   753,
     290,     0,   882,   897,   885,   765,   598,     0,   622,   602,
       0,     0,   608,   607,   609,   610,     0,     0,     0,   622,
     622,   551,     0,   627,     0,     0,   507,     0,     0,   512,
     624,   623,   513,   510,     0,   599,   600,   634,     0,   514,
     622,   518,     0,     0,     0,   530,     0,   528,     0,   622,
       0,   524,     0,     0,     0,   622,     0,   536,     0,     0,
     539,     0,   622,     0,   545,     0,     0,   622,     0,   555,
     556,   558,   559,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     561,     0,     0,   565,     0,   567,     0,     0,     0,   641,
       0,     0,     0,     0,   573,   579,   580,   581,   582,   583,
     584,     0,   587,   589,     0,   591,   592,     0,     0,   499,
    1150,  1148,  1149,   492,   490,   959,   403,     0,     0,   398,
     622,   704,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   911,     0,     0,   338,     0,
     322,   323,   321,   316,   318,   319,   900,  1288,  1287,  1279,
     662,     0,   722,   377,   370,   352,   351,     0,   378,   380,
     379,   957,     0,   870,   857,   863,   974,   856,     0,   862,
     841,   861,   862,   860,     0,   854,   307,   662,     0,     0,
     643,     0,     0,   327,  1231,     0,  1212,  1210,  1214,  1216,
    1242,  1245,   964,     0,  1208,     0,     0,     0,     0,  1139,
    1121,     0,     0,     0,  1158,  1137,     0,   776,     0,  1284,
    1192,  1190,  1191,  1195,  1196,  1198,  1197,  1199,  1189,  1186,
    1193,     0,   721,   286,   286,   286,     0,   286,     0,     0,
     801,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,     0,     0,     0,     0,     0,
     286,     0,   284,    85,   262,     0,   264,   285,   758,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,     0,    57,     0,   698,     0,   826,   662,   830,   829,
     754,   292,   291,   752,   886,     0,   612,   604,   601,     0,
       0,   613,   614,   621,   619,   615,   620,   617,     0,     0,
       0,     0,     0,     0,   631,     0,     0,     0,     0,     0,
       0,     0,   637,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,     0,     0,     0,     0,   435,   436,   437,   438,   439,
     440,   441,     0,   433,   442,   443,   428,     0,   704,     0,
       0,     0,   426,   425,   431,   427,     0,   448,   450,     0,
     445,     0,   444,   446,   447,   451,   449,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   390,   703,     0,   399,     0,   468,   469,   470,   471,
     472,   473,   474,     0,   466,   475,   476,   392,   622,   704,
       0,     0,     0,     0,   460,   459,   464,   461,     0,   481,
     483,     0,   478,     0,   477,   479,   480,   484,   482,   422,
     421,   960,   909,     0,  1176,  1177,  1175,  1174,   906,   320,
       0,   698,     0,     0,     0,   373,     0,     0,   350,   354,
     353,   381,     0,   869,     0,   848,     0,     0,   843,   842,
     850,   860,     0,   662,     0,     0,   659,     0,     0,   662,
     665,   665,   662,   662,   691,   314,  1255,     0,  1254,     0,
       0,     0,   342,   343,   312,     0,     0,     0,   801,   799,
     800,     0,     0,  1147,     0,     0,    72,   782,     0,     0,
     790,   769,   779,  1281,   348,  1282,   712,     0,   699,  1184,
     287,   257,   241,   246,   245,   268,   141,   257,   240,   266,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   802,
       0,     0,     0,     0,   279,   280,     0,     0,     0,   714,
       0,     0,     0,     0,   298,   299,   297,     0,     0,     0,
       0,   801,     0,     0,     0,    86,     0,   143,     0,   293,
     141,   145,   961,    46,    47,    49,   738,   736,   734,   737,
     735,    51,    48,    50,    52,    53,    54,    45,   143,    56,
     918,    84,   131,   130,   129,     0,     0,   662,   712,   965,
     698,     0,   828,   766,     0,     0,   578,   616,   618,   503,
     505,   549,   550,   552,     0,   500,   508,     0,   625,   511,
     635,   504,     0,     0,   515,     0,     0,   526,   531,   529,
     522,   527,   525,     0,   595,   538,   535,   537,     0,     0,
     543,   546,   547,     0,   523,   553,     0,   434,   560,   429,
     704,   432,     0,     0,     0,     0,     0,   564,   566,     0,
     571,   569,     0,     0,     0,     0,     0,   577,     0,   590,
     593,   594,   391,   393,   467,     0,   462,   622,   704,   465,
       0,     0,     0,     0,   918,  1178,  1179,  1280,   705,   747,
     749,   723,   724,     0,   371,     0,   385,   376,   384,   954,
     958,   871,   721,   975,     0,   862,   642,     0,   661,   660,
     662,   665,   665,   698,     0,   646,   666,     0,     0,   644,
     645,   667,  1252,     0,  1244,  1243,  1250,  1256,  1246,  1201,
    1138,     0,     0,     0,  1163,     0,  1156,     0,     0,  1168,
    1166,  1167,  1165,  1152,   356,     0,   803,   789,   795,     0,
       0,     0,   788,   953,   952,  1277,   721,   713,     0,   144,
     258,   242,     0,     0,     0,   243,   140,     0,     0,   270,
      92,   257,   276,   278,   290,    93,    90,   103,    96,     0,
      97,     0,    88,    89,     0,    95,    94,   120,   119,    99,
     121,   100,   101,   283,   115,   117,   116,   118,    98,   113,
     111,   114,   108,   110,   112,   109,    87,     0,     0,   124,
     123,   122,   105,   272,    91,   265,     0,   132,   134,   135,
     963,     0,   295,   267,     0,   136,   137,     0,     0,    62,
       0,     0,   920,   919,    63,    74,     0,     0,   698,   727,
     822,   605,     0,     0,     0,   638,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,   423,   453,   452,     0,
       0,     0,     0,   576,   574,   575,     0,   455,     0,   463,
       0,   457,   486,   485,     0,     0,   700,   921,     0,     0,
       0,   383,   382,     0,   849,     0,   844,     0,   658,     0,
       0,     0,   712,     0,   662,   662,     0,   648,     0,     0,
       0,   663,  1253,     0,     0,     0,  1261,     0,     0,  1162,
    1154,  1155,     0,     0,     0,  1151,   377,   783,   797,     0,
     805,     0,     0,   780,   792,     0,     0,   791,  1285,     0,
     142,   257,   239,   244,     0,     0,     0,     0,   277,   275,
     106,   107,   956,   715,   718,     0,   104,     0,   269,   143,
     294,   226,     0,   205,   151,     0,     0,   180,   177,   190,
     164,   179,   162,   158,   194,   192,   175,   194,   186,   166,
     172,   165,   189,   171,   194,   191,   188,   170,   160,   159,
     163,   187,   169,     0,   183,   205,   209,   221,     0,   205,
     207,   962,    65,    83,    75,     0,    61,     0,   718,   255,
     824,     0,   820,     0,     0,   509,     0,   519,   520,   532,
       0,   696,   540,   548,   554,   424,   563,   570,   568,   572,
     622,   456,   458,     0,     0,   701,   712,     0,   929,   726,
     725,   372,   955,     0,     0,   662,   662,     0,   727,   814,
     651,   655,   647,     0,   671,   669,   670,   668,     0,  1260,
    1258,     0,  1259,     0,     0,  1257,  1235,  1247,     0,  1203,
    1160,  1159,  1161,  1157,     0,     0,   125,     0,     0,     0,
     806,   794,  1284,   781,   787,  1194,     0,     0,   282,     0,
     293,     0,   719,   720,   717,   102,   209,   133,     0,     0,
       0,   224,   227,   229,     0,     0,   150,   167,     0,   168,
     181,   184,   185,     0,     0,   198,   196,   193,     0,   198,
     198,     0,     0,   198,     0,   216,     0,     0,   213,     0,
     218,   146,   210,   212,   222,     0,   182,     0,   153,     0,
     198,     0,   208,   198,  1284,   918,     0,    68,    58,     0,
     254,     0,   728,     0,   501,   639,     0,     0,     0,     0,
     709,   718,     0,   721,     0,     0,     0,     0,   922,   924,
       0,   748,     0,   662,   653,   657,     0,   812,     0,     0,
       0,     0,     0,     0,  1248,     0,  1239,  1240,  1241,  1236,
       0,     0,     0,     0,     0,  1262,  1264,  1164,  1153,    73,
     370,   796,   798,   804,   793,     0,   138,     0,     0,     0,
     281,   271,   718,   273,   232,   233,     0,     0,     0,   228,
       0,     0,     0,     0,   202,   203,   204,   173,   199,   201,
       0,   149,   174,     0,     0,   157,   220,   215,   214,   217,
     219,   211,   801,     0,     0,   147,     0,   148,    66,    64,
       0,  1277,   718,     0,   541,   533,     0,   606,   585,   921,
       0,     0,   706,   708,   702,   739,     0,     0,     0,     0,
     923,     0,     0,   930,   932,     0,     0,   813,   650,     0,
     654,     0,     0,   676,     0,  1249,  1237,  1265,  1268,  1266,
    1267,  1263,   126,     0,   716,   293,   235,     0,   234,     0,
     230,   231,   226,   154,   206,     0,   259,   195,     0,   200,
       0,   161,   156,     0,   221,   221,    70,    69,   253,   256,
       0,   929,   710,   711,   718,     0,   355,   927,   928,     0,
     925,     0,     0,   931,     0,     0,     0,     0,   649,     0,
     675,     0,   674,     0,   274,   237,   238,   236,   225,   178,
       0,     0,   176,   223,   152,   155,  1284,   534,   935,   707,
     740,   926,   934,   933,     0,     0,     0,     0,   677,   672,
       0,   139,   260,   197,    71,     0,   784,     0,     0,   664,
     652,   656,   673,     0,     0,   910,   377,     0,   936,   786,
       0,     0,     0,   743,   745,     0,   785,   746,   741,   377,
       0,   744,   845
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    43,    44,    45,   702,   985,   986,    46,  1310,  1616,
     989,  1609,  1849,  2154,  1854,  2261,  2366,  1530,  1764,  2015,
     260,   261,   262,   427,  1610,  1292,  1293,  1826,  1818,  1809,
    1832,  2209,   263,  1615,  1836,  1837,  1838,  1839,  1786,  1546,
    1590,  1847,  2006,  2128,  2123,  2007,  2008,  2009,  2010,  2125,
    2126,  2237,  2238,  2239,  2116,  2153,  2141,  2142,  2143,  2148,
    1846,  2111,  2112,  2113,  2310,  1547,  1548,   641,   264,  2017,
    2018,  1779,  2315,    47,   966,  1294,  1295,  1296,  1957,  1967,
    2305,  1549,   249,  1323,  1843,  1577,    48,    49,   651,   647,
     648,    50,   684,    51,   657,    52,   600,  1183,  1184,  1185,
      53,   422,    54,   658,   973,   974,   975,    55,   598,    56,
     683,   939,   940,    57,    58,   607,  1249,   388,   389,  1194,
    1195,   618,   619,   620,  1478,   896,   897,   898,  1481,  1198,
    1717,   498,   437,   372,   802,   866,   373,   816,   374,  1008,
    1624,   725,  1007,  1338,   817,   750,  1031,   477,   478,   479,
     743,   494,  1350,  1037,  1038,  1352,  1110,  1219,  2063,  2189,
    2191,  1503,  1220,  1221,  1737,  1921,  2065,  2193,  2351,  2292,
     783,   695,   696,   726,  1262,  2046,  2172,  1139,  1896,  2170,
    2272,  1776,  1777,  1804,  1963,  2104,  1196,  1474,  1711,  2022,
     835,  1601,  2336,  2387,  2402,  2403,  2404,  1197,  1897,    59,
     266,    60,  1320,   700,   701,   443,   271,    61,   273,  1325,
      62,   409,  1248,   461,   688,   951,   952,  1531,  1765,  2395,
    1771,  1532,  1775,  2092,  1767,  1937,  1938,  1559,  1560,  1768,
    1940,  2089,   438,   439,    63,   399,  1733,   656,    64,   265,
     709,  1314,   995,  1313,   710,   711,   997,   434,   435,    65,
     397,    66,   390,   642,  1722,   915,   907,   643,   916,  1203,
    1485,    67,   691,    68,   956,    69,   272,   455,   456,   722,
    1004,    70,   385,   604,   605,    71,   383,    72,    73,    74,
    1704,   881,   595,  1855,  2048,  2178,  2179,  2181,  2283,  2284,
    2386,   375,  2316,   376,  1772,  1773,  1964,   377,  1591,   654,
     245,   378,  1206,  1746,   247,    75,   686,   407,   947,   948,
     644,   841,  1245,  1246,  1930,  1756,  1763,    76,   597,   457,
     884,   885,  1466,    77,    78,   697,   964,   965,  1259,  1537,
    1260,    79,   405,    80,   402,   680,   681,   682,   931,   927,
     933,   936,  2200,  2075,  2076,  1233,  1747,  1748,  1226,  1507,
    1508,  1926,  2079,  2205,  2206,    81,   253,   424,    82,    83,
      84,   893,  1533,  1470,  1534,  1535,  1536,  1189
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1998
static const short yypact[] =
{
    1825, -1998, 11603,    46,   441, -1998,   139, -1998,   509, -1998,
   -1998,   231, -1998, -1998,  5214,   124,   441,   441, -1998,   441,
   -1998,   -51, 25020, -1998, -1998,   162,    60,   167,    -1,   441,
   -1998,   441,   441, -1998, -1998, -1998,   441,   441, -1998,   152,
   25020, -1998,   300,   424,   423, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,  9258, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, 25020,   245,   105,  -103, -1998, -1998,   228,
   -1998, -1998, -1998,   112,   262, -1998, -1998, -1998, -1998,   190,
     312, -1998,   351,   375,   508,    53,  3174, 25020, -1998, 25020,
     315,   433,   639,   314,   250,   265,   284,   298,   306,   310,
     319,   323,  5214,   328,   348, -1998,   364, -1998,   370,   383,
   -1998,   395,  5214, -1998, -1998, -1998, -1998,   397,   403,   421,
     427,   439,   445,   477,   477,   477, -1998,   455,   468,   472,
    5214,   475,   479,   482,   500,   511,   519,   523,   527,   532,
     536,   544,   549,   558,   560,   581,   583,   585,   589,   607,
     609,   615,  5622,   616,   617,   618,   619,   621,   622,   623,
     626,   627,   628,   629,   631,   633,   635,   636,   637,   641,
     642,   643,   645,   646,   648,   650,   653,   655,   656,   657,
     658,   666,   677,   686,  5214,  5214,  5214,  5214, 25020, 12773,
   25020, 21889, -1998, -1998, -1998,   570, -1998, -1998,   416,   490,
   11603, -1998, -1998,   833, -1998,   195,   849, -1998, -1998,   587,
    1268,   576,   576,   576, -1998,   576, -1998, 11603, -1998,   821,
   -1998, -1998, 25654, -1998, -1998, 25654, -1998, -1998, -1998,    78,
   -1998, -1998, -1998, -1998, -1998,   580, -1998,   151, -1998, -1998,
   25020, 11603, 11603, -1998, -1998, -1998,   813, 25020,   190, -1998,
   25020, 25020, -1998, -1998, 23850,    53, -1998, 21889,   687, -1998,
     903, -1998,   983, 25020,   315, -1998, -1998, -1998, -1998, -1998,
   -1998,  1012, -1998, -1998,   837,   696, -1998, -1998, -1998, -1998,
   -1998,    46,  1030,   136,  5214,  1030,  1030,  1030,  1030,  5214,
     -62,  5214,  5214,  5214,  5214,  5214, -1998,   798, -1998, 11993,
     -62,   702,  5214,  5214,  5214, -1998, -1998,   570,   570,   570,
    5214,  1030,  1030, 21889,   730, -1998,  5214,   704, -1998,  3582,
   -1998,   706,  5214,  5214,  5214,  5214,  5214,  5214,  5214,  5214,
    5214,  1652,  5214,  5214,  5214,   477,  5214,  5214,  5214, 21698,
    3990,  5214,  5214,  5214,  5214,  5214,  5214,  3582, -1998,  5214,
    5214,  4398,  5214,  5214,  5214,  5214,  5214,  5214,  2358,   707,
     707,   707,   707,   707,   707,   477,  4806,   708,  5214,  5214,
    5214,   552,   -62,   -62, 16898,   -62,  5214, -1998, -1998,     2,
   -1998,  1091,   719,  5214,  5214,  5214,  5214,  5214,  5214,  5214,
     539,  5214,  5214,  5214,   715,  5214,   -15,  5214,  5214,  5214,
    5214,  5214,  5214,  5214,  6030,  6438,  5214,  5214,  5214,  5214,
   -1998, 25020, -1998,  1083, -1998,   976,  1017, 11603, 11603,   986,
   11603, -1998, -1998,  1059,   746, -1998, 25020,   919, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,   770,   587,
   -1998,  1137,   949,    -4,     9,  1034, -1998, -1998,   855, -1998,
   -1998,   956, -1998, -1998, -1998,  1144,   955, -1998,  1034,   261,
     961,  1073, -1998,   353,   937, -1998, -1998,   794, -1998,   794,
     794, -1998,   794, -1998, -1998, -1998,    46, 11603, 11603, -1998,
     963, -1998, -1998, -1998, -1998,   966,   408, -1998,  1105, -1998,
   -1998,  1001, -1998,  1015, -1998, -1998, -1998, -1998, -1998, -1998,
    1018,   807, -1998, 11603, 11603,  1023,   494, -1998, 10038, -1998,
     817, 22680, 25020, -1998, -1998, 25020, -1998, 25410,  1025, -1998,
     -42, -1998,   957,  1108, -1998, 11603, -1998,  1033, 11603, -1998,
     820, -1998,   816, -1998,  3174, 11603, -1998, -1998, 11603, -1998,
    1014,   639, 11603, 10038, -1998,   826, -1998, -1998,   827,   840,
   14805,   834,   835,   841,   842, 14881, 21726, 14965, 15032, 15113,
   13824,   843,   838,   839, -1998, -1998, 16993, 15156, 15279,   844,
    5214,   845, 13852,   847,   848, -1998,   851, 15303, -1998, 17015,
   -1998, 15377, 15402, 15470, 13868, 13971, 15525, 15568, 14015, 15716,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998,  1130, 15800, 15814, 14088,   -70, 17208, 15827,
   14131,   860, -1998, 17230, 15840, 15883, 16073, 16116, 17252, 17305,
   17327, 17520, 21964, -1998, -1998, 17542, 16129, 16154, 14278, 17564,
   14321, 16169,  5214,  5214,  5214, 16800,   853, -1998,   854,   856,
     857,   858,   859,   -64, -1998, 17667, -1998, 14377, 17823, 14444,
   -1998, -1998, -1998, -1998, -1998,   862, -1998,  1169,   861,   863,
     864, 23070,  5214, 25020,   268,   268,   268,   268,   268,   268,
     268,  1071, -1998,   418,   418,  1752, -1998,  1150,   865,  5214,
    5214,  5214,   767,   767,   268,   -85, 22153, -1998,   393,   358,
    5622,   430,  5622,   430,   -62,   -62,   -62,  1084,   866, -1998,
   -1998,  1129,  1225,   107,   870, -1998,   -42,  1035,   -19, -1998,
     195, -1998,   376, -1998, -1998, -1998,   -12, -1998,  5214, -1998,
    1134, 11603, -1998, -1998, -1998, -1998,    -7, -1998, 23070, -1998,
     945,   261, -1998, -1998, -1998,  1034, 25020,   261, 11603,   261,
   -1998,  1034,   576,   237,  -129,   -42,  1043,   878, -1998, -1998,
   -1998,   878, -1998,   878, -1998, -1998,   878, 10428,  1422,   877,
   -1998,  1041,   -57, 10428, -1998, -1998, -1998,   879, 13553, 11603,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998,   134,   160,   996,  1024, -1998, 19960, -1998, -1998, -1998,
   -1998, -1998, 11603, -1998, -1998,   592,  1277,  1280,  1282,  1285,
    1287,  1289,  1290,  1291,  1293,   897, -1998, -1998, -1998,  1053,
   11603,   820, -1998,   218, 25020,  1181, 20242, -1998, -1998, -1998,
      -8,  1259, -1998,   901, -1998, -1998, -1998,  5214, -1998, -1998,
     904,  5214, -1998, -1998, -1998, -1998,  5214,   138,   138, -1998,
   -1998, -1998,  5214, -1998,  5214, 11993, -1998,  5214,  5214, -1998,
   21889,   902, -1998, -1998,  5214, -1998, -1998,   984,  5214, -1998,
   -1998, -1998,   968,   971,  1068, -1998,  5214, -1998,  5214, -1998,
    1075, -1998,  5214,  5214,  5214, -1998,  1131, -1998,  5214,  1132,
   -1998,  5214, -1998,  5214, -1998,  5214,  5214, -1998,  5214, -1998,
   -1998, -1998, -1998,  5214,  5214,  5214,  5214,  5214,  5214,  5214,
     554,  5214,  5214,  5214,  5214,  5214,     6,  5214,  5214,  5214,
    5214,  5214,  5214,  5214,  6846,  7254,  5214,  5214,  5214,  5214,
   -1998,  5214,  5214, -1998,  5214, -1998,  5214,  5214,  5214, 21889,
    1196,  1199,  1201,  5214, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998,  1136, -1998, -1998,  5214, -1998, -1998,  5214,  5214, -1998,
   -1998, -1998, -1998, -1998, 21889, -1998, -1998,   926,  1090, -1998,
   -1998,  1150,   268, 22178,  5214,  5214,  5214,  5214,  5214,  5214,
    5214,   577,  5214,  5214,  5214,   929,  5214,    49,  5214,  5214,
    5214,  5214,  5214,  5214,  5214,  7662,  8070,  5214,  5214,  5214,
    5214, 21698, 21698, 25020,  1092, -1998,   -29, 11603, -1998,  1093,
   -1998, -1998, -1998, -1998, -1998,    89, -1998, -1998, -1998, -1998,
     237,   491, -1998, -1998,    57,  1192, -1998,  1214, -1998, 21889,
   -1998,   930,  1098,  1222, -1998, -1998,   938, -1998,  1288,  1034,
   -1998, -1998,  1034,   261, 11603, -1998, -1998,   237, 25020,    25,
   -1998, 11603,  1107, -1998, -1998, 25020, -1998, -1998, -1998, -1998,
     936, -1998,   940,  1110, -1998, 11603, 11603,  1112,  1231,   494,
    1212,  1135,    61, 13163, -1998, -1998,   180, -1998,   -11,    82,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998,  5214,  1078,    51,  1263,  1263,  1352,   701,  1358,  1360,
     180,  1361,  1258,  1366,  1227,  1230,  1267,  1373,  1374,  1297,
    1376,  1381,  1383,  1385,    74,  1387,  1389,  1390,  1171,  1393,
    1263,  1397, -1998,  1885,  1003, 10823, -1998, -1998, -1998, -1998,
    1168,  1170,  1172,   579,  1174,   552,   552,  1176,   552,   957,
    1116,   413, -1998,  1153,  1024, 20242, -1998,   237, -1998,  1008,
   -1998, -1998, -1998, -1998, -1998,   -11, 21889, -1998, -1998, 16389,
   17890, -1998, -1998, -1998, -1998,  1139, -1998,  1140,  1019,  1021,
    1026,  1028, 17939, 17972, -1998, 17985, 16432,  5214, 18016,  5214,
    1038, 21742,  1045,  1029,  5214,  5214,  1032, 18040, 18254,  1037,
    1039, 18287, 16471, 18299,  1042,  1044, 18413,  1022, 16487,  1046,
     430, 18447, 14542,  1047, 14567,   268,   268,   268,   268,   268,
     268,   268,  1240, -1998,   418,   418,  1752, 18516,  1150,  5214,
    5214,  5214,   767,   767,   268,   -85, 22190,   393,   358,  5622,
     430,  5622,   430,   -62,   -62,   -62,  1084, 16580, 18561, 18751,
   16747, 14591, 16704,  5214,  5214,  5214, 18778,  1040, 18851, 18875,
   18968, -1998, -1998,  1049, -1998,  5214,   268,   268,   268,   268,
     268,   268,   268,  1246, -1998,   418,   418,  1752, -1998,  1150,
    1036,  5214,  5214,  5214,   767,   767,   268,   -85, 22201,   393,
     358,  5622,   430,  5622,   430,   -62,   -62,   -62,  1084,   860,
     860, -1998, -1998,  1173,  1299, -1998, -1998, -1998, -1998, -1998,
       3,    28,  1197,  1206,   552, -1998,  1305,  1386, -1998, -1998,
   -1998, 23460, 20637, -1998,   579, -1998, 23070,  1202, -1998, -1998,
   -1998,   261,    38,  1055,  1317,  1319, -1998,   -35, 11993,   237,
    1269,  1269,   237,   237,  1354, -1998, -1998,   330, -1998,  1072,
   21032, 23070, -1998, -1998, -1998, 23070, 13553,  1318,   180, -1998,
   -1998, 23070,   -96,     2,   180,  2766, -1998, -1998,  1069,  9648,
   -1998, -1998, -1998, -1998, -1998, -1998,  1276,  1085, 21889, -1998,
   -1998, 12383,   484, -1998, -1998, -1998,   386, 12383, -1998, -1998,
   12383, 12383,  1233,  1331,  1337, 12383, 12383,   579,   552, -1998,
   25020,   552,  1482,   552, -1998, -1998,  1485,   579,   579, -1998,
     428,   685,   552,   552, -1998, -1998, -1998, 11603,    93,   705,
    1096,   180,   115, 12383,  1253, -1998, 19960,  8868, 25020,   113,
      30, -1998,  1086, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,  8478, -1998,
      44, -1998, -1998, -1998, -1998,  1249,  1100,   237,  1276,   221,
      28,  1113, -1998, -1998,  1111,  5214, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998,   -14, -1998, -1998,  5214, 21889, -1998,
   21889, -1998,  5214,  5214, -1998, 21698, 21698, -1998, -1998, -1998,
   -1998, -1998, -1998,  5214, -1998, -1998, -1998, -1998,  1315,  5214,
   -1998, -1998, -1998,  5214, -1998, -1998,  5214, -1998, -1998, -1998,
    1150,   268, 22246,  5214, 21698, 21698,  5214, -1998, -1998,  5214,
   -1998, -1998,  5214,  5214, 19041, 19065, 19188, -1998,  1320, -1998,
   -1998, -1998, -1998,  1752, -1998,  1117, -1998, -1998,  1150,   268,
   22301,  5214, 21698, 21698,   -47, -1998, -1998, -1998,  1394, -1998,
   -1998, -1998,   -90,  1148, -1998, 24240, -1998, -1998, -1998, -1998,
    1106, -1998,  1078, -1998,  1353,  1034, -1998,  1175, -1998, -1998,
     237,  1269,  1269,  -122,   180,   114, -1998,  1378,  1380,   114,
      16,   471, -1998, 25020, -1998, -1998,  1183,   -97, -1998,  1123,
   -1998,   406,  2766,   180, -1998,  1127, -1998, 25020,  5214, -1998,
   -1998, -1998, 21889, -1998,   587, 11993, -1998,  1126, -1998,    -3,
    1128,   339, -1998, -1998, -1998,   919,  1078, -1998,  3174, -1998,
   -1998, -1998,  5214,  1384,  1391,   484, -1998,  1133,    81, -1998,
   -1998, 12383, -1998, -1998,   313, -1998, -1998, -1998, -1998,  1294,
   -1998,  1300, -1998, -1998,  5214, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, 11603, 25020, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998,   414, -1998, -1998, -1998,
   -1998, 25020, -1998, -1998, 11603, -1998, -1998,  1998, 25020, -1998,
    1138,   452, -1998, -1998, -1998,  1459,  1302, 25020,    28,  1265,
   -1998, -1998, 16733,  1203, 19302, 21889, 21845,  1146,  1151, 14610,
    1154, 19377, 19402, 19416, -1998,  5214,  1752,   860,   860, 19470,
   19495, 19526, 19717, -1998, -1998, -1998,  1156, -1998,  1152, -1998,
    5214,  1752,   860,   860,  1415,  1483,  1439,  1478,   552,   552,
    1228, -1998, -1998, 21427, -1998,  1565, -1998,  1388,   114,  1431,
    1434, 11993,  1276,  5214,   237,   237,  5214, -1998,   484,   484,
     484, -1998, -1998,  1497,    59, 23070,  1324,    94,  1375, -1998,
   -1998, -1998,   -96,  1349,   180, 21889,   770,  1186, -1998,   180,
    3174,  1069,  1533, -1998, -1998,    -3, 11993, -1998, -1998,   461,
   21889, 12383, -1998, -1998, 25020,  1494,  1496, 12383, -1998, -1998,
   -1998, -1998, 21889,  1193,   487,   492, -1998,  1998, -1998,  8868,
   -1998,   -77,  1198,  1200, -1998,   551,  -106,  1296, -1998, -1998,
   -1998, -1998, -1998, -1998,  1204,  1312, -1998,  1204, -1998, -1998,
   -1998, -1998, -1998, -1998,  1204, -1998, -1998, -1998,  1207, -1998,
   -1998, -1998, -1998,  1208, -1998,  1200,    64,   -22,  1215,  1200,
    1216, -1998, -1998,  1885, -1998,     4, -1998,   497,   487,  1217,
   -1998,   579, -1998,  5214,  1205, -1998,  5214, -1998, -1998, -1998,
    5214,  1030, -1998, -1998, -1998,  1752, -1998, -1998, -1998, -1998,
   -1998, -1998,  1752,  1364,  5214, -1998,  1276,   605,  1449, -1998,
   -1998, -1998, -1998,  1365,  1468,   237,   237,   180,  1265, 21889,
      27,   402, 21889,  1218, -1998, -1998, -1998, -1998,   185, -1998,
   -1998,  1382, -1998,  1392,  1395, -1998,    76, -1998,   596, -1998,
   -1998, -1998, -1998, -1998,  1213,  2766,   -55, 11993,  3174,  1220,
     687, -1998,    82, -1998, -1998, -1998,  1219,   504, -1998,  1077,
     113,  5214, -1998, -1998, -1998, -1998,    64, -1998,    18,   143,
   25020, -1998,   309, -1998,  1436,  1440, -1998, -1998,  1330, -1998,
   -1998, -1998, -1998,  1235,  1451,   503, -1998, -1998,  1238,   503,
     503,  1454,  1455,   503,   477, -1998,  1077,  1457, -1998,  1479,
    1498, -1998,    64, -1998, -1998,  1418, -1998,  1460, -1998,  1461,
     503,  1462, -1998,   503,    82,    44,  1533, -1998, -1998, 25020,
   -1998,  1471, -1998, 19827, -1998, 21889, 14691,  1257,  1260, 11603,
    -133,   487,  5214,  1078,  1571,  1580,  1556,  1583,   605, -1998,
     249, -1998,  1596,  1055,   114,   114,  5214, -1998,  5214,  1414,
    5214,  1416, 25020,  1271, -1998,  1435, -1998, -1998, -1998, -1998,
      22,  1486,   552,   552,   552,   596, -1998, -1998, -1998, -1998,
      57, -1998, -1998, -1998, -1998, 25020, -1998,   477,   477,   477,
   -1998, -1998,   487, -1998, -1998, -1998,   377,   377,   520, -1998,
    1278,  1279,    -7,   528, -1998, -1998, -1998, -1998,   503, -1998,
      -7, -1998, -1998,  1281,  1283, -1998,   570, -1998, -1998, -1998,
   -1998, -1998,   180,  1292,  1298, -1998,  1275, -1998, -1998, -1998,
    1301,   919,   487,  1303, -1998, -1998,  5214, -1998, -1998,  1478,
     411,  5214, -1998, -1998, 21889,  1327,    -7,    -7,  1595,    -7,
   -1998,  1600,  1603,   249, -1998,  1526,  1501, 21889, 21889,  1295,
   21889,  1306,   533, -1998, 24630, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998,   546, -1998,   113, -1998,  1622, -1998,   354,
   -1998, -1998,   309, -1998, -1998,   555, -1998, -1998,  1505, -1998,
     561, -1998, -1998, 25020,    26,    26, -1998, -1998, -1998, -1998,
   19840,  1449, -1998, -1998,   487, 25020, -1998, -1998, -1998,    -7,
   -1998,    -7,    -7, -1998,  1687,  5214, 25020, 25020, -1998, 25020,
   -1998,   563, -1998,  1488, -1998, -1998, -1998, -1998, -1998, -1998,
      -7,  1304, -1998, -1998, -1998, -1998,    82, -1998,  1560, -1998,
   -1998, -1998, -1998, -1998,   579,  1801,   566,   574, -1998, -1998,
   25020, -1998, -1998, -1998, -1998,  1511,  1307,  1310,  1627, -1998,
   -1998, -1998, -1998,  1549, 11213, -1998,  1311,  1537, -1998, -1998,
     590,  5214,  1313,  1314, -1998,  1708, -1998, 21889, -1998, -1998,
     552, -1998, -1998
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -1998, -1998, -1998, -1998, -1998, -1998,   410, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998,  -434, -1998, -1998, -1497, -1998, -1998,
   -1998,  1463, -1998,  1308,  -291, -1266, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998,   116,  -246,   431,   464,   140, -1998,
    -229, -1998,  -238, -1998, -1998,  -244,  -243, -1998, -1998, -1357,
    -277, -1260, -1998,  -504, -1335, -1998,  -371, -1998,  -405, -1565,
    -615,  -573, -1998,  -372,  -484, -1998, -1243, -1998, -1998, -1815,
    -415, -1512,  -495, -1998, -1998, -1998, -1998,   161, -1998, -1998,
   -1998,  -454,  -355,   -48, -1974, -1998, -1998, -1998, -1998,   442,
     828, -1998, -1998, -1998, -1998, -1998, -1998, -1998,   564, -1998,
   -1998, -1998, -1998, -1998,  -718,   776, -1998, -1998, -1998, -1998,
   -1998, -1998,   517,  1685,   286, -1998, -1998,  1344, -1998, -1998,
   -1179,    -6, -1998,  1155,  -453,  -165,   582, -1862, -1998, -1998,
   -1998,  -143,   -10, -1998, -1998,  -810,  -478,   415, -1998, -1998,
   -1998,  -455, -1998,   755,  -409, -1998, -1998, -1998,  1309, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998,   168, -1164, -1998, -1998,
   -1998, -1998, -1472, -1998, -1402, -1998,  -921, -1998, -1998, -1331,
    -473, -1998,  -855,  1316, -1275, -1998, -1998, -1086, -1998, -1998,
   -1998, -1578,  -936, -1998, -1998, -1929, -1245, -1998, -1998,  -282,
   -1283, -1450, -1998, -1998, -1998, -1998,  -632,   586, -1998, -1998,
   -1998, -1998, -1998,  -584,  -364,  1334, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998,  1057,   846,   457, -1998, -1998,
    -611, -1641, -1998, -1998, -1998, -1998,  -303, -1201, -1499,  -155,
   -1998, -1998, -1626,  -711, -1998, -1998, -1998,  1379, -1998, -1998,
   -1998, -1998, -1998, -1998,  1079,   796, -1254,  1356, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998,  -869,  -609, -1998,   873, -1998,
   -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,  1076, -1998,
   -1998, -1998, -1998, -1998,   906, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998,    95,  -476, -1998,  -380,  -531, -1998,  -482,
   -1998,  -298,  -682, -1943,  -144,   905, -1939,  -423, -1463,     0,
   -1998,    -2,  -354,  -877, -1998, -1998, -1998, -1998, -1998,   565,
   -1998,   282,   290,    50, -1998,  -124, -1694, -1998, -1998,   913,
   -1998,   632, -1998, -1998, -1998, -1998, -1998, -1998, -1998, -1998,
   -1998, -1998, -1998, -1998, -1998,  1417, -1998,   882, -1998, -1998,
   -1998, -1998, -1998,  -379, -1998,   880,   316,  -101,   -68, -1998,
      84, -1998, -1998, -1998,  -376, -1998, -1998, -1998, -1998, -1998,
   -1998, -1732,  -606, -1998, -1997, -1998, -1998, -1998
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -1284
static const short yytable[] =
{
     246,   894,   244,   998,   371,   487,   488,   489,   728,   957,
     731,   732,   733,   734,   886,   561,   888,  1539,  1480,  1752,
     387,  1727,  1603,  1604,  1555,  1606,  1471,  1585,  1176,   912,
    1297,  1207,  1770,   653,  1721,  1787,   753,   754,   408,  1618,
    1859,  1522,  1209,  1947,  1611,  1525,   791,   -67,  1212,  1526,
    1494,  1143,   803,  1492,  -143,  1424,   744,  1526,  1931,  1494,
    2144,  1494,  1494,    23,  1526,  1622,   415,  1237,   414,  1190,
    1519,  1863,  1494,   924,   925,   967,   749,   751,  1780, -1238,
    1520,  1321,  1828,  1574,  1780, -1283,   756,  1788,  1789,  2160,
    -964,  1852,  1793,  1794,  2134,  2214,  2108,   857,  1180,  1738,
     942,  1898,   576,   867,  1476,  2171,   723,  1795,  2144,   418,
    2069,  1850,  1190,  1730,  1222,  2109,   693,  1802,  1803,  1955,
    1833,  1829,   393,  1782,  1924,   576,  2221,   954,  1943,   838,
     858,  2270,  1191,  1463,  1000,  1841,   968, -1188,  1003,  2097,
     859,  1261,  2135,  2224,  -143,   904,  1754,  1540,  1494,  1541,
    1575,  2199,  1949,  1620,  -694,   902,  2220,  2258,  1495,   839,
     693,  1389,  2222, -1187,  1496,  2120,  2136,  1495,  1178,  1495,
    1495,   500,   590,  1496,  -143,  1496,  1496,  1542,   590,  2226,
    1495,   687,  1852,   694,   248,  1464,  1496,   254,  2080,  1519,
    1543,  1712,   386,  2247,  1440,  1814,  1708,   528,  2121,  1520,
    1497,  1544,   860,  1815,  1441,   580,  1180,  1223,  1916,  1497,
     724,  1497,  1497,  1816,  2227,   432,  1322,   787,  1527,  2188,
    1331,  2225,  1497,  1390,  1205,   379,   391,   694,   392,   395,
     969,   903,  1842,   840,  1830,   955,  1465,   970,   396,  1181,
     655,   416,  2273,   601,   905,   727,  1844,   823,   971,  2145,
    1528,  2137,  1499,  2138,  1498,  -143,  1495,   602,  1528,  2071,
    1755,  1499,  1496,  1499,  1499,   440,  1442,   441,  2139,  -694,
    1192,  1182,   470,  2271,  1499,  1796,  1917,   972,  1798,  1958,
    1800,  1396,   480,  2146,  1911,   571,   572,  1806,  1810,  1811,
    1812,  1261,  2070,  2304,   861,  2228,  2071,  2145,  1497,  1781,
     493,   923,  1669,  1831,  2093,   394,  1210,  -143,   420,  1925,
    1956,  1731,  1215,  1853,  2090,  1391,  1899,  1576,   589,   419,
    2140,  -694,   519,  1758,   950,  2072,  1732,  2110,  1780,  1909,
    1910,  2354,  2334,  2328,  2058,  1735,  1059,   892,  1739,  1740,
   -1238,   589,  1121,   433,  1490,  1860,   406,  1181,  2081,   972,
    1499,  1193,  1448,  1696,   552,   553,   554,   555,  1443,   950,
     410,  1817,  1500,   380,   972,  1622,   556,   560,   562,  2384,
     267,  1500,  -694,  1500,  1500,   571,   572,  1501,   415,  1182,
     596,  1141,  2147,   867,  1500,   913,  1501,   972,  1501,  1501,
     856,  2208,  1477,  1529,  1193,   415,  1521, -1188,   972,  1501,
    2303,  1942,  1321,   603,  1853,  2369,   914,    42,  2156,  1332,
     571,   572,  1333,  1334,  2073,  2074, -1188,   458,   698,   415,
     415,   699,  1502, -1187,   411,   704,   412,  2194,   706,   707,
    1335,  1502,   712,  1502,  1502,  1336,  1494,   929,   417,  2096,
    1337,   717, -1187,  1726,  1502,  1770,  2332,  1137,   919,   459,
    1187,  2073,  2074,  1858,   730,   576,  2356,  -143,  1912,   735,
    1500,   736,   737,   738,   739,   740,  2306,   421,  2173,  2333,
     930,   268,   746,   747,   748,  1501,   460,  1904,  -829,  1782,
     752,  -829,  2108,  2281,   269,  1188,   757,  1133,  1780,   759,
    2282,   423,   761,   762,   763,   764,   765,   766,   767,   768,
     769,  2109,   784,   785,   786,   425,   788,   789,   790,  1783,
     793,   794,   795,   796,   797,   798,   799,   800,  2157,   801,
    1502,   805,   806,   807,   808,   809,   810,   811,   815,  2327,
    1805,  1948,   426,  1913,  2401,   580,   825,  1322,   827,   828,
     829,  2195,  1953,  2357,  1495,   576,   837,  2401,   270,   830,
    1496,   428,  1932,   844,   845,   846,   847,   848,   849,   850,
     920,   853,   854,   855,  2102,  2307,  1908,   862,   863,   864,
     865,  2162,   868,   869,   871,   873,   874,   875,   876,   877,
     576,  1672,   255,  2020,  1874,   950,  1497,   592,   831,   878,
    1784,   257,  2103,  1918,  2190,   415,   415,   883,   415,  1327,
    1488,  2308,  1344,  1489,   891,   576,  2309,  1388,  1298,  1919,
    1340,  1341,  1889,   867,   593,  2049,  2050,   576,   832,   645,
    1927,   944,  1725,  1543,  -829,   580,  1928,  -829,   430,  1315,
    2129,  1353,  1621,  1700,  1544,   256,  1929,  2130,  1499,   646,
    1359,  1217,  1785,   833,  1753,  1218,  1364,   431,  -250,  1741,
    1297,   945,   594,  1369,   462,   415,   415,   442,  1373,  2260,
     580,   608,   582,   583,   584,   585,   586,   587,   588,   463,
    2133,   589,   444,   830,  2150,  2064,  2064,  2064,  1439,   250,
     251,   415,   415,   941,   867,   580,   415,   445,   464,   958,
     959,  1472,  1473,   960,  1612,   963,   609,   580,  1459,  1460,
    1596,   446,   465,   415,  1613,   988,   993,  1614,   992,   967,
     466,  2286,   831,   415,   467,   999,   415,  2174,  2175,   296,
     415,   415,   610,   468,  2201,   946,   851,   469,   852,   834,
    1920,  1423,   471,  2085,  1976,  1742,  1743,  1977,  2088,  1597,
    1030,  1382,   832,  1383,  1945,  1946,   447,  1611,  1500,   257,
    2060,  2061,   472,  2323,   584,   585,   586,   587,   588,  2364,
    2365,   589,  -127,  1501,  1433,   258,  1434,   833,   473,  1598,
     968,  2202,  2203,  2204,   474,   448,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,  1819,   475,   583,   584,
     585,   586,   587,   588,  1599,  2234,   589,  1540,   449,   476,
    2176,   481,  1109,  1109,  1109,  2235,   830,   482,  1502,  2236,
    1550,  1551,   590,  1556,   584,   585,   586,   587,   588,  1968,
    1969,   589,  1981,   611,  1553,   483,   259,   591,   586,   587,
     588,   484,  1134,   589,   649,   650,  1583,   652,  1820,   560,
    1543,  1135,   573,   485,   450,   831,  2177,   451,  1821,   486,
    1142,  1544,  1822,   834,  2117,  2004,  2186,  2013,  1969,   490,
    1171,   452,  1172,  1227,   969,  1228,  2095,   714,  1229,  2241,
    2242,   970,   491,  2245,   453,   832,   492,   454,   251,   495,
    1600,   599,   971,   496,  1823,  1824,   497,  1825,  1199,  1524,
    2255,  2184,  2185,  2257,  1807,  1808,  1201,  2105,   972,   415,
     833,  1204,  2158,  2159,   499,  1554,   560,  2210,   606,  2216,
    2159,  1670,   574,   867,  1211,   501,   415,   252,  1213,  2298,
    2299,  2300,   575,   502,  2388,  2312,  2159,   503,  2275,   381,
     382,   504,   384,  2317,  2318,  1232,   505,  1231,  2348,  2349,
     506,  1232,   398,  1231,   400,   401,  1244,   415,   507,   403,
     404,  2353,  2159,   508,   576,   818,   819,   820,   821,   822,
    2359,  2360,   509,  1698,   510,   867,  2362,  2360,  2379,  2380,
     415,  2390,  2349,   612,   613,   614,   615,   616,   617,  2391,
    2349,  1111,  1112,   655,   579,   511,   834,   512,   415,   513,
    1312,   692,   712,   514,  1319,  2406,  1946,  1326,  2066,  2067,
     703,  1329,  1250,  1251,  1252,  1253,  1330,  1254,  1255,  1256,
    1257,   515,  1342,   516,  1343,  2376,  2377,  1345,  1346,   517,
     520,   521,   522,   523,  1348,   524,   525,   526,  1351,  1695,
     527,   529,   530,   531,   580,   532,  1357,   533,  1358,   534,
     535,   536,  1361,  1362,  1363,   537,   538,   539,  1366,   540,
     541,  1368,   542,  1370,   543,  1371,  1372,   544,  1374,   545,
     546,   547,   548,  1375,  1376,  1377,  1378,  1379,  1380,  1381,
     549,  1384,  1385,  1386,  1387,  1734,   581,  1392,  1393,  1394,
    1395,   550,  1397,  1398,  1400,  1402,  1403,  1404,  1405,  1406,
     551,  1407,  1408,   714,  1409,   715,  1410,  1411,  1412,   716,
     719,   720,   721,  1416,   724,   742,  1774,   745,   755,   758,
     842,   760,  -596,   826,  1418,  1258,  1906,  1419,  1420,   856,
     976,   977,   978,   979,   980,   981,   982,  2412,   983,   984,
     843,   879,  1723,   880,  1426,  1427,  1428,  1429,  1430,  1431,
    1432,   882,  1435,  1436,  1437,   887,  1266,   889,  1444,  1445,
    1446,  1447,   890,  1449,  1450,  1452,  1454,  1455,  1456,  1457,
    1458,   582,   583,   584,   585,   586,   587,   588,   895,   894,
     589,  1461,  1867,  1868,   892,   415,   900,   883,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   901,   906,
     908,   909,   910,   911,  1268,  1269,   917,   918,   285,   921,
     922,  1877,  1878,   926,   928,   932,   934,  1270,  1271,   935,
     937,   287,   415,   938,  1491,   943,  1493,  1272,  -965,   415,
    1273,  1504,   953,  1506,   987,   990,   994,   996,  1001,  1892,
    1893,  1006,  1009,   415,   415,   941,  1513,   290,  1010,  1012,
    1013,   560,  1024,  1965,   573,  1025,  1014,  1015,  1023,  1029,
    1032,  1538,  1035,  1036,  1054,  1276,  1039,  1063,  1115,  1116,
    1136,  1117,  1118,  1119,  1120,  1138,   293,   294,  1128,  1140,
    1174,   576,  1130,  1175,  1131,  1132,  1177,  1173,  1179,  1202,
    1208,  1224,  1225,  1235,  1236,  1239,  1300,  1261,  1888,  1301,
    1277,  1302,   295,  1592,  1303,   621,  1304,  1278,  1305,  1306,
    1307,   622,  1308,  1309,  1311,  1317,  1324,   972,  1347,  1328,
    1356,  1349,  1354,  1619,   574,  1355,   623,  1360,  1280,   296,
    1413,  1365,  1367,  1414,   575,  1415,  1417,  1281,  1282,  1283,
     624,  1421,  1422,  1438,  1462,  1468,  1191,  1638,  1190,  1640,
    1483,  1482,  1939,  1285,  1645,  1646,  1484,  1509,  1486,  1505,
    1487,  1510,  2217,  1511,  1514,  1515,   576,  -694,  1517,  1540,
    1192,  1552,  1286,   693,  1518,   577,   578,  1557,   625,  1558,
    1561,  1287,  1562,  2218,  2219,  1563,  1564,  2212,   306,  1565,
    1671,  1566,  1567,  1568,  1569,  1570,   579,  1288,  -694,  1674,
    1571,  1675,  1572,   626,  1573,   627,  1578,   628,  1579,  1580,
    1581,  -694,  1582,  1684,  1685,  1686,  1584,   629,   630,  1586,
    1593,   631,  1594,  1617,  1595,  1693,  1602,   632,  1605,  1621,
    1627,  1628,  1641,  1643,  1629,   633,  1630,   634,  1658,  1667,
     694,  1631,  1699,  1632,  1644,  1694,   580,  1647,  1705,  1709,
    1697,  1702,  1650,  1703,  1651,   659,  1688,  1655,  1710,  1656,
    1713,  1660,  1664,   635,  1692,   660,  1706,  1714,   661,  1289,
     662,   636,   663,  1218,   664,  1728,  1724,  1729,  1736,   665,
    1744,  1751,  1291,  1766,  1279,  1790,  -694,   666,   581,  1718,
    1720,  1791,   667,   668,   560,  -694,  -694,  1792,  2057,  1778,
     669,  1799,  -694,   670,  1801,  1834,  -694,  1848,  -694,   637,
    1827,  1856,  -694,   638,  1857,  1870,   639,   640,   959,   560,
    1886,  1318,  1900,   560,  1244,  1762,  1861,  1903,  1905,   560,
    1608,  1907,  1887,  1774,  1923,  1895,  1914,  1201,  1915,  1925,
   -1143,  1933,  1941,  1944,  1951,  2014,  1960,  1954,  -694,  1592,
     672,  1952,  1961,  2012,  2016,  1592,  -694,  2021,  1592,  1592,
     673,  2027,  -694,  1592,  1592,  2024,  2028,  2041,  1797,  2043,
    2031,   674,  2040,   582,   583,   584,   585,   586,   587,   588,
    2044,  2045,   589,  2047,  2053,   415,  2167,  1813,  1129,  2055,
    2051,  1592,  2056,  -694,  2068,  1592,  1840,  2054,  2078,  2082,
    -694,  2084,  2087,  1526,  2337,  2338,  2098,  2340,  2099,  2101,
    2122,  2127,  2114,  2169,  2115,  2180,  1592,  2182,  2124,  -694,
    2164,  2131,  2132,  -694,  -694,  1862,  2183,  -694,  2207,  2149,
    2151,  2161,  2192,  2215,  2196,  2213,  2230,  1864,   675,  2249,
    2231,  2168,  1865,  1866,  2197,  2146,  -694,  2198,   676,  2232,
     677,  2233,  2240,  1869,  2243,  2244,  2248,  2252,  2250,  1871,
    2253,  2254,  2256,  1872,   678,   894,  1873,  2371,  2276,  2372,
    2373,  2263,  2267,  1876,  1939,  2268,  1879,  2277,  2278,  1880,
    2279,  2285,  1881,  1882,  2289,  2294,  2291,  2295,  2382,   679,
    2297,  2318,  2339,  2313,  2314,  2335,  2321,  2341,  2322,  -694,
    2342,  1891,  2344,  2345,  2355,  2361,  2374,  2324,  2385,  2346,
    -694,  2393,  2397,  2325,  1844,  2398,  2326,  2405,  2329,  2383,
    2347,  2394,  -694,  1902,  2396,  -694,  -742,  2410,  2408,  1607,
    2409,  2259,  2155,  2107,  1851,   429,  1589,  1545,  2100,  2106,
    1845,  2118,  2119,  2152,  2319,  2223,   705,  2251,  2381,  2358,
    2229,  1506,  1762,  2311,  2262,  2320,  1959,  1835,  1935,  1469,
    1216,  1299,  1512,   413,   690,  1244,  1707,  2302,  1936,  -694,
    -694,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,  2086,  1950,  1339,   899,  1475,  2187,  2411,   718,   729,
    1005,  1479,  1623,  2400,  2211,   741,  2091,   991,   689,  1592,
    1316,   713,  1214,  2331,  1962,  1247,  1186,  1002,  2280,  1894,
    2368,  2343,  2094,  1200,  1516,  1757,  1750,  1934,  2083,  1467,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
    1234,  2296,   685,  1238,  2077,   415,  1966,  1922,     1,  2301,
       2,  1749,     0,     0,     0,     0,  2246,     0,     0,  1970,
       0,     0,   415,     0,  1971,     0,  2011,     0,     3,     4,
       0,     5,     0,     6,     0,  2019,     7,     0,     8,     0,
       0,     9,    10,    11,     0,  2035,    12,    13,     0,    14,
      15,     0,    16,     0,     0,     0,   573,    17,    18,    19,
    2042,    20,    21,     0,    22,    23,    24,    25,     0,    26,
       0,    27,     0,     0,    28,    29,    30,   574,     0,     0,
       0,  1461,     0,  2059,     0,    31,  2062,   575,     0,     0,
       0,     0,     0,     0,     0,  1266,     0,     0,    32,   487,
     488,   489,     0,   560,     0,     0,     0,     0,     0,     0,
      33,    34,     0,     0,     0,     0,     0,     0,     0,   576,
       0,     0,     0,     0,  1201,     0,   574,     0,     0,  1592,
       0,     0,  2019,    35,     0,  1592,   575,     0,     0,     0,
       0,     0,     0,  1268,  1269,     0,     0,  1592,     0,   579,
       0,  1774,   770,   771,   772,   773,  1270,  1271,     0,     0,
       0,     0,     0,     0,     0,     0,  1272,     0,   576,  1273,
     774,   775,   776,     0,     0,     0,     0,   577,   578,     0,
       0,     0,   777,   778,     0,     0,   779,     0,     0,     0,
       0,     0,     0,  2163,     0,   780,  2165,     0,   579,   580,
    2166,     0,     0,     0,  1276,     0,     0,     0,     0,     0,
       0,     0,   781,   782,  1962,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    36,     0,     0,     0,    37,     0,
      38,     0,     0,     0,    39,     0,     0,     0,     0,  1277,
       0,   581,     0,     0,     0,     0,  1278,     0,   580,     0,
       0,     0,     0,     0,     0,  1762,     0,     0,     0,     0,
    1972,  1973,  1974,     0,    40,     0,     0,  1280,     0,     0,
       0,  1962,     0,     0,     0,     0,  1281,  1282,  1283,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  2019,     0,
     581,     0,  1285,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1286,     0,     0,     0,     0,     0,     0,     0,     0,
    1287,     0,     0,     0,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,     0,   589,  1288,  2019,     0,  1975,
       0,     0,  2274,     0,     0,     0,     0,   415,     0,  2269,
       0,     0,     0,     0,     0,     0,  2287,     0,  2288,     0,
    2290,  1976,     0,     0,  1977,    41,     0,     0,     0,     0,
    2293,     0,     0,     0,     0,   582,   583,   584,   585,   586,
     587,   588,     0,     0,   589,     0,     0,     0,     0,     0,
    2389,     0,     0,  2019,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1978,  1289,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  2330,     0,     0,     0,
       0,  1962,     0,     0,     0,     0,     0,  1979,  1980,  1981,
       0,     0,  1982,  1983,  1984,  1985,  1986,     0,  1987,  1988,
       0,  1989,  1990,  1991,  1992,  1993,  1994,     0,     0,  1995,
       0,  1996,  2352,  1997,  1998,  1999,  2000,  2001,  2002,     0,
       0,  2003,  2004,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  2363,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  2370,     0,  2375,     0,     0,     0,     0,
       0,     0,     0,     0,  2293,  2293,     0,  2378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  2392,  2005,
      90,     0,     0,   274,    92,    93,     0,    94,    95,    96,
     275,  2407,  1201,    97,     0,    98,     0,    99,   100,   101,
     276,   102,     0,     0,   103,     0,   104,   277,   278,   105,
       0,     0,   106,   107,   108,   109,   110,   111,     0,     0,
     112,   113,   114,   279,   115,   280,   116,   281,     0,     0,
     118,   119,     0,     0,     0,     0,   120,   121,   122,   123,
     282,   124,   125,   126,   812,     0,   127,     0,   283,   128,
     129,     0,   130,     0,     0,   131,     0,   284,     0,   132,
       0,     0,   133,     0,     0,   134,   135,     0,   136,   137,
       0,     0,   138,   139,     0,   140,   141,   142,   143,   285,
       0,     0,     0,   144,     0,   145,     0,   146,   286,     0,
       0,   147,   287,     0,   148,   149,     0,     0,   150,     0,
       0,   151,     0,     0,   152,   153,     0,     0,     0,   813,
     288,   154,     0,     0,     0,   155,   289,   156,   290,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,   164,
     165,   291,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   176,   177,   292,   178,   293,   294,   179,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   183,   184,   185,   186,   187,   188,
     189,   190,     0,   295,     0,     0,   191,     0,   192,     0,
     193,   194,     0,     0,   195,   196,   197,     0,   198,   199,
     200,     0,     0,   201,     0,   202,     0,     0,   203,     0,
     296,     0,   814,   204,   205,   297,   298,   299,   300,     0,
       0,   206,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,   211,     0,     0,   301,
     212,   302,   213,   303,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
     218,     0,   219,   220,   304,   305,     0,     0,     0,   306,
       0,     0,     0,     0,     0,   223,   307,     0,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,     0,
       0,   224,   318,   319,   320,     0,   321,   322,   323,   324,
     325,   326,   327,     0,   328,   329,     0,     0,   330,   226,
     331,   227,   332,   333,   334,   335,   336,   337,     0,   338,
     229,   230,   339,   340,   341,   342,     0,   343,   344,   345,
     346,   347,   234,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,     0,   235,     0,   361,
     362,   363,   237,     0,     0,   238,     0,   239,     0,   240,
     241,   242,     0,     0,   364,     0,     0,     0,     0,   365,
       0,     0,   366,     0,     0,   367,   368,     0,   369,   370,
      85,    86,    87,    88,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,   274,    92,    93,     0,    94,    95,    96,   275,     0,
       0,    97,     0,    98,     0,    99,   100,   101,   276,   102,
       0,     0,   103,     0,   104,   277,   278,   105,     0,     0,
     106,   107,   108,   109,   110,   111,     0,     0,   112,   113,
     114,   279,   115,   280,   116,   281,     0,     0,   118,   119,
    1759,     0,     0,     0,   120,   121,   122,   123,   282,   124,
     125,   126,     0,     0,   127,     0,   283,   128,   129,     0,
     130,     0,     0,   131,     0,   284,     0,   132,  1760,     0,
     133,     0,     0,   134,   135,     0,   136,   137,     0,     0,
     138,   139,     0,   140,   141,   142,   143,   285,     0,     0,
       0,   144,     0,   145,     0,   146,   286,     0,     0,   147,
     287,     0,   148,   149,     0,     0,   150,     0,     0,   151,
       0,     0,   152,   153,     0,     0,     0,     0,   288,   154,
       0,     0,     0,   155,   289,   156,   290,     0,     0,   157,
     158,   159,   160,   161,   162,   163,     0,   164,   165,   291,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,   176,   177,   292,   178,   293,   294,   179,  1761,   180,
       0,     0,     0,     0,     0,     0,     0,   181,   182,     0,
       0,     0,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   295,     0,     0,   191,     0,   192,     0,   193,   194,
       0,     0,   195,   196,   197,     0,   198,   199,   200,     0,
       0,   201,     0,   202,     0,     0,   203,     0,   296,     0,
       0,   204,   205,   297,   298,   299,   300,     0,     0,   206,
     207,     0,     0,     0,   208,     0,     0,     0,   209,     0,
       0,     0,   210,     0,   211,     0,     0,   301,   212,   302,
     213,   303,     0,     0,   215,   216,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,     0,   218,     0,
     219,   220,   304,   305,     0,     0,     0,   306,     0,     0,
       0,     0,     0,   223,   307,     0,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,     0,     0,   224,
     318,   319,   320,     0,   321,   322,   323,   324,   325,   326,
     327,     0,   328,   329,     0,     0,   330,   226,   331,   227,
     332,   333,   334,   335,   336,   337,     0,   338,   229,   230,
     339,   340,   341,   342,     0,   343,   344,   345,   346,   347,
     234,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,     0,   235,     0,   361,   362,   363,
     237,     0,     0,   238,     0,   239,     0,   240,   241,   242,
       0,     0,   364,     0,     0,     0,     0,   365,     0,     0,
     366,     0,     0,   367,   368,     0,   369,   370,    85,    86,
      87,    88,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,   274,
      92,    93,     0,    94,    95,    96,   275,     0,     0,    97,
       0,    98,     0,    99,   100,   101,   276,   102,     0,     0,
     103,     0,   104,   277,   278,   105,     0,     0,   106,   107,
     108,   109,   110,   111,     0,     0,   112,   113,   114,   279,
     115,   280,   116,   281,     0,     0,   118,   119,     0,     0,
       0,     0,   120,   121,   122,   123,   282,   124,   125,   126,
       0,     0,   127,     0,   283,   128,   129,     0,   130,     0,
       0,   131,     0,   284,     0,   132,   436,     0,   133,     0,
       0,   134,   135,     0,   136,   137,     0,     0,   138,   139,
       0,   140,   141,   142,   143,   285,     0,     0,     0,   144,
       0,   145,     0,   146,   286,     0,     0,   147,   287,     0,
     148,   149,     0,     0,   150,     0,     0,   151,     0,     0,
     152,   153,     0,     0,     0,     0,   288,   154,     0,     0,
       0,   155,   289,   156,   290,     0,     0,   157,   158,   159,
     160,   161,   162,   163,     0,   164,   165,   291,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,     0,   176,
     177,   292,   178,   293,   294,   179,     0,   180,     0,     0,
       0,     0,     0,     0,     0,   181,   182,     0,     0,     0,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   295,
       0,     0,   191,     0,   192,     0,   193,   194,     0,     0,
     195,   196,   197,     0,   198,   199,   200,     0,     0,   201,
       0,   202,     0,     0,   203,     0,   296,     0,     0,   204,
     205,   297,   298,   299,   300,     0,     0,   206,   207,     0,
       0,     0,   208,     0,     0,     0,   209,     0,     0,     0,
     210,     0,   211,     0,     0,   301,   212,   302,   213,   303,
       0,     0,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   217,     0,   218,     0,   219,   220,
     304,   305,     0,     0,     0,   306,     0,     0,     0,     0,
       0,   223,   307,     0,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,     0,     0,   224,   318,   319,
     320,     0,   321,   322,   323,   324,   325,   326,   327,     0,
     328,   329,     0,     0,   330,   226,   331,   227,   332,   333,
     334,   335,   336,   337,     0,   338,   229,   230,   339,   340,
     341,   342,     0,   343,   344,   345,   346,   347,   234,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,     0,   235,     0,   361,   362,   363,   237,     0,
       0,   238,     0,   239,     0,   240,   241,   242,     0,     0,
     364,     0,     0,     0,     0,   365,     0,     0,   366,     0,
       0,   367,   368,     0,   369,   370,    85,    86,    87,    88,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,   274,    92,    93,
       0,    94,    95,    96,   275,     0,     0,    97,     0,    98,
       0,    99,   100,   101,   276,   102,     0,     0,   103,     0,
     104,   277,   278,   105,     0,     0,   106,   107,   108,   109,
     110,   111,     0,     0,   112,   113,   114,   279,   115,   280,
     116,   281,     0,     0,   118,   119,     0,     0,     0,     0,
     120,   121,   122,   123,   282,   124,   125,   126,     0,     0,
     127,     0,   283,   128,   129,     0,   130,     0,     0,   131,
       0,   284,     0,   132,     0,     0,   133,     0,     0,   134,
     135,     0,   136,   137,     0,     0,   138,   139,     0,   140,
     141,   142,   143,   285,     0,     0,     0,   144,     0,   145,
       0,   146,   286,     0,     0,   147,   287,     0,   148,   149,
       0,     0,   150,     0,     0,   151,     0,     0,   152,   153,
       0,     0,     0,     0,   288,   154,     0,     0,     0,   155,
     289,   156,   290,     0,     0,   157,   158,   159,   160,   161,
     162,   163,     0,   164,   165,   291,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   176,   177,   292,
     178,   293,   294,   179,     0,   180,     0,     0,     0,     0,
       0,     0,     0,   181,   182,     0,     0,     0,   183,   184,
     185,   186,   187,   188,   189,   190,     0,   295,     0,     0,
     191,     0,   192,     0,   193,   194,     0,     0,   195,   196,
     197,     0,   198,   199,   200,     0,     0,   201,     0,   202,
       0,     0,   203,     0,   296,     0,     0,   204,   205,   297,
     298,   299,   300,     0,     0,   206,   207,     0,     0,     0,
     208,     0,     0,     0,   209,     0,     0,     0,   210,     0,
     211,     0,     0,   301,   212,   302,   213,   303,     0,     0,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,     0,   218,     0,   219,   220,   304,   305,
       0,     0,     0,   306,     0,     0,     0,     0,     0,   223,
     307,     0,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,     0,     0,   224,   318,   319,   320,     0,
     321,   322,   323,   324,   325,   326,   327,     0,   328,   329,
       0,     0,   330,   226,   331,   227,   332,   333,   334,   335,
     336,   337,     0,   338,   229,   230,   339,   340,   341,   342,
       0,   343,   344,   345,   346,   347,   234,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   235,     0,   361,   362,   363,   237,     0,     0,   238,
       0,   239,     0,   240,   241,   242,     0,     0,   364,     0,
       0,     0,     0,   365,     0,     0,   366,   758,     0,   367,
     368,     0,   369,   370,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,   274,    92,    93,     0,    94,
      95,    96,   275,     0,     0,    97,     0,    98,     0,    99,
     100,   101,   276,   102,     0,     0,   103,     0,   104,   277,
     278,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,   279,   115,   280,   116,   281,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,   282,   124,   125,   126,     0,     0,   127,     0,
     283,   128,   129,     0,   130,     0,     0,   131,     0,   284,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,   285,     0,     0,     0,   144,     0,   145,     0,   146,
     286,     0,     0,   147,   287,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,   288,   154,     0,     0,     0,   155,   289,   156,
     290,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,   291,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,   292,   178,   293,
     294,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,   295,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,   296,     0,     0,   204,   205,   297,   298,   299,
     300,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,   301,   212,   302,   213,   303,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   304,   305,     0,     0,
       0,   306,     0,     0,     0,     0,     0,   223,   307,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,     0,     0,   224,   318,   319,   320,     0,   321,   322,
     323,   324,   325,   326,   327,     0,   328,   329,     0,     0,
     330,   226,   331,   227,   332,   333,   334,   335,   336,   337,
       0,   338,   229,   230,   339,   340,   341,   342,     0,   343,
     344,   345,   346,   347,   234,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,     0,   235,
       0,   361,   362,   363,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,     0,     0,   364,     0,     0,     0,
       0,   365,     0,     0,   366,   792,     0,   367,   368,     0,
     369,   370,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,   274,    92,    93,     0,    94,    95,    96,
     275,     0,     0,    97,     0,    98,     0,    99,   100,   101,
     276,   102,     0,     0,   103,     0,   104,   277,   278,   105,
       0,     0,   106,   107,   108,   109,   110,   111,     0,     0,
     112,   113,   114,   279,   115,   280,   116,   281,     0,     0,
     118,   119,     0,     0,     0,     0,   120,   121,   122,   123,
     282,   124,   125,   126,     0,     0,   127,     0,   283,   128,
     129,     0,   130,     0,     0,   131,     0,   284,     0,   132,
       0,     0,   133,     0,     0,   134,   135,     0,   136,   137,
       0,     0,   138,   139,     0,   140,   141,   142,   143,   285,
       0,     0,     0,   144,     0,   145,     0,   146,   286,     0,
       0,   147,   287,     0,   148,   149,     0,     0,   150,     0,
       0,   151,     0,     0,   152,   153,     0,     0,     0,     0,
     288,   154,     0,     0,     0,   155,   289,   156,   290,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,   164,
     165,   291,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   176,   177,   292,   178,   293,   294,   179,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   183,   184,   185,   186,   187,   188,
     189,   190,     0,   295,     0,     0,   191,     0,   192,     0,
     193,   194,     0,     0,   195,   196,   197,     0,   198,   199,
     200,     0,     0,   201,     0,   202,     0,     0,   203,     0,
     296,     0,     0,   204,   205,   297,   298,   299,   300,     0,
       0,   206,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,   211,     0,     0,   301,
     212,   302,   213,   303,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
     218,     0,   219,   220,   304,   305,     0,     0,     0,   306,
       0,     0,     0,     0,     0,   223,   307,     0,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,     0,
       0,   224,   318,   319,   320,     0,   321,   322,   323,   324,
     325,   326,   327,     0,   328,   329,     0,     0,   330,   226,
     331,   227,   332,   333,   334,   335,   336,   337,     0,   338,
     229,   230,   339,   340,   341,   342,     0,   343,   344,   345,
     346,   347,   234,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,     0,   235,     0,   361,
     362,   363,   237,     0,     0,   238,     0,   239,     0,   240,
     241,   242,     0,     0,   364,     0,     0,     0,     0,   365,
       0,     0,   366,   804,     0,   367,   368,     0,   369,   370,
      85,    86,    87,    88,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,   274,    92,    93,     0,    94,    95,    96,   275,     0,
       0,    97,     0,    98,     0,    99,   100,   101,   276,   102,
       0,     0,   103,     0,   104,   277,   278,   105,     0,     0,
     106,   107,   108,   109,   110,   111,     0,     0,   112,   113,
     114,   279,   115,   280,   116,   281,     0,     0,   118,   119,
       0,     0,     0,     0,   120,   121,   122,   123,   282,   124,
     125,   126,     0,     0,   127,     0,   283,   128,   129,     0,
     130,     0,     0,   131,     0,   284,     0,   132,     0,     0,
     133,     0,     0,   134,   135,     0,   136,   137,     0,     0,
     138,   139,     0,   140,   141,   142,   143,   285,     0,     0,
       0,   144,     0,   145,     0,   146,   286,     0,     0,   147,
     287,     0,   148,   149,     0,     0,   150,     0,     0,   151,
       0,     0,   152,   153,     0,     0,     0,     0,   288,   154,
       0,     0,     0,   155,   289,   156,   290,     0,     0,   157,
     158,   159,   160,   161,   162,   163,     0,   164,   165,   291,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,   176,   177,   292,   178,   293,   294,   179,     0,   180,
       0,     0,     0,     0,     0,     0,     0,   181,   182,     0,
       0,     0,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   295,     0,     0,   191,     0,   192,     0,   193,   194,
       0,     0,   195,   196,   197,     0,   198,   199,   200,     0,
       0,   201,     0,   202,     0,     0,   203,     0,   296,     0,
       0,   204,   205,   297,   298,   299,   300,     0,     0,   206,
     207,     0,     0,     0,   208,     0,     0,     0,   209,     0,
       0,     0,   210,     0,   211,     0,     0,   301,   212,   302,
     213,   303,     0,     0,   215,   216,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,     0,   218,     0,
     219,   220,   304,   305,     0,     0,     0,   306,     0,     0,
       0,     0,     0,   223,   307,     0,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,     0,     0,   224,
     318,   319,   320,     0,   321,   322,   323,   324,   325,   326,
     327,     0,   328,   329,     0,     0,   330,   226,   331,   227,
     332,   333,   334,   335,   336,   337,     0,   338,   229,   230,
     339,   340,   341,   342,     0,   343,   344,   345,   346,   347,
     234,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,     0,   235,     0,   361,   362,   363,
     237,     0,     0,   238,     0,   239,     0,   240,   241,   242,
       0,     0,   364,     0,     0,     0,     0,   365,     0,     0,
     366,   824,     0,   367,   368,     0,   369,   370,    85,    86,
      87,    88,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,   274,
      92,    93,     0,    94,    95,    96,   275,     0,     0,    97,
       0,    98,     0,    99,   100,   101,   276,   102,     0,     0,
     103,     0,   104,   277,   278,   105,     0,     0,   106,   107,
     108,   109,   110,   111,     0,     0,   112,   113,   114,   279,
     115,   280,   116,   281,     0,     0,   118,   119,     0,     0,
       0,     0,   120,   121,   122,   123,   282,   124,   125,   126,
       0,     0,   127,     0,   283,   128,   129,     0,   130,     0,
       0,   131,     0,   284,     0,   132,     0,     0,   133,     0,
       0,   134,   135,     0,   136,   137,     0,     0,   138,   139,
       0,   140,   141,   142,   143,   285,     0,     0,     0,   144,
       0,   145,     0,   146,   286,     0,     0,   147,   287,     0,
     148,   149,     0,     0,   150,     0,     0,   151,     0,     0,
     152,   153,     0,     0,     0,     0,   288,   154,     0,     0,
       0,   155,   289,   156,   290,     0,     0,   157,   158,   159,
     160,   161,   162,   163,     0,   164,   165,   291,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,     0,   176,
     177,   292,   178,   293,   294,   179,     0,   180,     0,     0,
       0,     0,     0,     0,     0,   181,   182,     0,     0,     0,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   295,
       0,     0,   191,     0,   192,     0,   193,   194,     0,     0,
     195,   196,   197,     0,   198,   199,   200,     0,     0,   201,
       0,   202,     0,     0,   203,     0,   296,     0,     0,   204,
     205,   297,   298,   299,   300,     0,     0,   206,   207,     0,
       0,     0,   208,     0,     0,     0,   209,     0,     0,     0,
     210,     0,   211,     0,     0,   301,   212,   302,   213,   303,
       0,     0,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   217,     0,   218,     0,   219,   220,
     304,   305,     0,     0,     0,   306,     0,     0,     0,     0,
       0,   223,   307,     0,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,     0,     0,   224,   318,   319,
     320,     0,   321,   322,   323,   324,   325,   326,   327,     0,
     328,   329,     0,     0,   330,   226,   331,   227,   332,   333,
     334,   335,   336,   337,     0,   338,   229,   230,   339,   340,
     341,   342,     0,   343,   344,   345,   346,   347,   234,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,     0,   235,     0,   361,   362,   363,   237,     0,
       0,   238,     0,   239,     0,   240,   241,   242,     0,     0,
     364,     0,     0,     0,     0,   365,     0,     0,   366,     0,
       0,   367,   368,     0,   369,   370,    85,    86,    87,    88,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,   274,    92,    93,
       0,    94,    95,    96,   275,     0,     0,    97,     0,    98,
       0,    99,   100,   101,   276,   102,     0,     0,   103,     0,
     104,   277,   278,   105,     0,     0,   106,   107,   108,   109,
     110,   111,     0,     0,   112,   113,   114,   279,   115,   280,
     116,   281,     0,     0,   118,   119,     0,     0,     0,     0,
     120,   121,   122,   123,   282,   124,   125,   126,     0,     0,
     127,     0,   283,   128,   129,     0,   130,     0,     0,   131,
       0,   284,     0,   132,     0,     0,   133,     0,     0,   134,
     135,     0,   136,   137,     0,     0,   138,   139,     0,   140,
     141,   142,   143,   285,     0,     0,     0,   144,     0,   145,
       0,   146,   286,     0,     0,   147,   287,     0,   148,   149,
       0,     0,   150,     0,     0,   151,     0,     0,   152,   153,
       0,     0,     0,     0,   288,   154,     0,     0,     0,   155,
     289,   156,   290,     0,     0,   157,   158,   159,   160,   161,
     162,   163,     0,   164,   165,   291,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   176,   177,   292,
     178,   293,   294,   179,     0,   180,     0,     0,     0,     0,
       0,     0,     0,   181,   182,     0,     0,     0,   183,   184,
     185,   186,   187,   188,   189,   190,     0,   295,     0,     0,
     191,     0,   192,     0,   193,   194,     0,     0,   195,   196,
     197,     0,   198,   199,   200,     0,     0,   201,     0,   202,
       0,     0,   203,     0,   296,     0,     0,   204,   205,   297,
     298,   299,   300,     0,     0,   206,   207,     0,     0,     0,
     208,     0,     0,     0,   209,     0,     0,     0,   210,     0,
     211,     0,     0,   301,   212,   302,   213,   303,     0,     0,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,     0,   218,     0,   219,   220,   304,   305,
       0,     0,     0,   306,     0,     0,     0,     0,     0,   223,
     307,     0,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,     0,     0,   224,   318,   319,   320,     0,
     321,   322,   323,   324,   325,   326,   327,     0,   328,   329,
       0,     0,   330,   226,   331,   227,   332,   333,   334,   335,
     336,   337,     0,   338,   229,   230,   339,   340,   341,   342,
       0,   343,   344,   345,   346,   347,   234,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   235,     0,   361,   362,   363,   237,     0,     0,   238,
       0,   239,     0,   240,   241,   242,     0,     0,   364,     0,
       0,     0,     0,   365,     0,     0,   518,     0,     0,   367,
     368,     0,   369,   370,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,   274,    92,    93,     0,    94,
      95,    96,   275,     0,     0,    97,     0,    98,     0,    99,
     100,   101,   276,   102,     0,     0,   103,     0,   104,   277,
     278,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,   279,   115,   280,   116,   281,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,   282,   124,   125,   126,     0,     0,   127,     0,
     283,   128,   129,     0,   130,     0,     0,   131,     0,   284,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,   285,     0,     0,     0,   144,     0,   145,     0,   146,
     286,     0,     0,   147,   287,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,   288,   154,     0,     0,     0,   155,   289,   156,
     290,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,   291,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,   292,   178,   293,
     294,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,   295,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,   296,     0,     0,   204,   205,   297,   298,   299,
     300,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,   301,   212,   302,   213,   303,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   304,   305,     0,     0,
       0,   306,     0,     0,     0,     0,     0,   223,   307,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,     0,     0,   224,   318,   319,   320,     0,   321,   322,
     323,   324,   325,   326,   327,     0,   328,   329,     0,     0,
     330,   226,   331,   227,   870,   333,   334,   335,   336,   337,
       0,   338,   229,   230,   339,   340,   341,   342,     0,   343,
     344,   345,   346,   347,   234,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,     0,   235,
       0,   361,   362,   363,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,     0,     0,   364,     0,     0,     0,
       0,   365,     0,     0,   366,     0,     0,   367,   368,     0,
     369,   370,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,   274,    92,    93,     0,    94,    95,    96,
     275,     0,     0,    97,     0,    98,     0,    99,   100,   101,
     276,   102,     0,     0,   103,     0,   104,   277,   278,   105,
       0,     0,   106,   107,   108,   109,   110,   111,     0,     0,
     112,   113,   114,   279,   115,   280,   116,   281,     0,     0,
     118,   119,     0,     0,     0,     0,   120,   121,   122,   123,
     282,   124,   125,   126,     0,     0,   127,     0,   283,   128,
     129,     0,   130,     0,     0,   131,     0,   284,     0,   132,
       0,     0,   133,     0,     0,   134,   135,     0,   136,   137,
       0,     0,   138,   139,     0,   140,   141,   142,   143,   285,
       0,     0,     0,   144,     0,   145,     0,   146,   286,     0,
       0,   147,   287,     0,   148,   149,     0,     0,   150,     0,
       0,   151,     0,     0,   152,   153,     0,     0,     0,     0,
     288,   154,     0,     0,     0,   155,   289,   156,   290,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,   164,
     165,   291,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   176,   177,   292,   178,   293,   294,   179,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   183,   184,   185,   186,   187,   188,
     189,   190,     0,   295,     0,     0,   191,     0,   192,     0,
     193,   194,     0,     0,   195,   196,   197,     0,   198,   199,
     200,     0,     0,   201,     0,   202,     0,     0,   203,     0,
     296,     0,     0,   204,   205,   297,   298,   299,   300,     0,
       0,   206,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,   211,     0,     0,   301,
     212,   302,   213,   303,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
     218,     0,   219,   220,   304,   305,     0,     0,     0,   306,
       0,     0,     0,     0,     0,   223,   307,     0,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,     0,
       0,   224,   318,   319,   320,     0,   321,   322,   323,   324,
     325,   326,   327,     0,   328,   329,     0,     0,   330,   226,
     331,   227,   872,   333,   334,   335,   336,   337,     0,   338,
     229,   230,   339,   340,   341,   342,     0,   343,   344,   345,
     346,   347,   234,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,     0,   235,     0,   361,
     362,   363,   237,     0,     0,   238,     0,   239,     0,   240,
     241,   242,     0,     0,   364,     0,     0,     0,     0,   365,
       0,     0,   366,     0,     0,   367,   368,     0,   369,   370,
      85,    86,    87,    88,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,     0,
       0,   274,    92,    93,     0,    94,    95,    96,   275,     0,
       0,    97,     0,    98,     0,    99,   100,   101,   276,   102,
       0,     0,   103,     0,   104,   277,   278,   105,     0,     0,
     106,   107,   108,   109,   110,   111,     0,     0,   112,   113,
     114,   279,   115,   280,   116,   281,     0,     0,   118,   119,
       0,     0,     0,     0,   120,   121,   122,   123,   282,   124,
     125,   126,     0,     0,   127,     0,   283,   128,   129,     0,
     130,     0,     0,   131,     0,   284,     0,   132,     0,     0,
     133,     0,     0,   134,   135,     0,   136,   137,     0,     0,
     138,   139,     0,   140,   141,   142,   143,   285,     0,     0,
       0,   144,     0,   145,     0,   146,   286,     0,     0,   147,
     287,     0,   148,   149,     0,     0,   150,     0,     0,   151,
       0,     0,   152,   153,     0,     0,     0,     0,   288,   154,
       0,     0,     0,   155,   289,   156,   290,     0,     0,   157,
     158,   159,   160,   161,   162,   163,     0,   164,   165,   291,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,   176,   177,   292,   178,   293,   294,   179,     0,   180,
       0,     0,     0,     0,     0,     0,     0,   181,   182,     0,
       0,     0,   183,   184,   185,   186,   187,   188,   189,   190,
       0,   295,     0,     0,   191,     0,   192,     0,   193,   194,
       0,     0,   195,   196,   197,     0,   198,   199,   200,     0,
       0,   201,     0,   202,     0,     0,   203,     0,   296,     0,
       0,   204,   205,   297,   298,   299,   300,     0,     0,   206,
     207,     0,     0,     0,   208,     0,     0,     0,   209,     0,
       0,     0,   210,     0,   211,     0,     0,   301,   212,   302,
     213,   303,     0,     0,   215,   216,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,     0,   218,     0,
     219,   220,   304,   305,     0,     0,     0,   306,     0,     0,
       0,     0,     0,   223,   307,     0,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,     0,     0,   224,
     318,   319,   320,     0,   321,   322,   323,   324,   325,   326,
     327,     0,   328,   329,     0,     0,   330,   226,   331,   227,
    1399,   333,   334,   335,   336,   337,     0,   338,   229,   230,
     339,   340,   341,   342,     0,   343,   344,   345,   346,   347,
     234,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,     0,   235,     0,   361,   362,   363,
     237,     0,     0,   238,     0,   239,     0,   240,   241,   242,
       0,     0,   364,     0,     0,     0,     0,   365,     0,     0,
     366,     0,     0,   367,   368,     0,   369,   370,    85,    86,
      87,    88,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,   274,
      92,    93,     0,    94,    95,    96,   275,     0,     0,    97,
       0,    98,     0,    99,   100,   101,   276,   102,     0,     0,
     103,     0,   104,   277,   278,   105,     0,     0,   106,   107,
     108,   109,   110,   111,     0,     0,   112,   113,   114,   279,
     115,   280,   116,   281,     0,     0,   118,   119,     0,     0,
       0,     0,   120,   121,   122,   123,   282,   124,   125,   126,
       0,     0,   127,     0,   283,   128,   129,     0,   130,     0,
       0,   131,     0,   284,     0,   132,     0,     0,   133,     0,
       0,   134,   135,     0,   136,   137,     0,     0,   138,   139,
       0,   140,   141,   142,   143,   285,     0,     0,     0,   144,
       0,   145,     0,   146,   286,     0,     0,   147,   287,     0,
     148,   149,     0,     0,   150,     0,     0,   151,     0,     0,
     152,   153,     0,     0,     0,     0,   288,   154,     0,     0,
       0,   155,   289,   156,   290,     0,     0,   157,   158,   159,
     160,   161,   162,   163,     0,   164,   165,   291,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,     0,   176,
     177,   292,   178,   293,   294,   179,     0,   180,     0,     0,
       0,     0,     0,     0,     0,   181,   182,     0,     0,     0,
     183,   184,   185,   186,   187,   188,   189,   190,     0,   295,
       0,     0,   191,     0,   192,     0,   193,   194,     0,     0,
     195,   196,   197,     0,   198,   199,   200,     0,     0,   201,
       0,   202,     0,     0,   203,     0,   296,     0,     0,   204,
     205,   297,   298,   299,   300,     0,     0,   206,   207,     0,
       0,     0,   208,     0,     0,     0,   209,     0,     0,     0,
     210,     0,   211,     0,     0,   301,   212,   302,   213,   303,
       0,     0,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   217,     0,   218,     0,   219,   220,
     304,   305,     0,     0,     0,   306,     0,     0,     0,     0,
       0,   223,   307,     0,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,     0,     0,   224,   318,   319,
     320,     0,   321,   322,   323,   324,   325,   326,   327,     0,
     328,   329,     0,     0,   330,   226,   331,   227,  1401,   333,
     334,   335,   336,   337,     0,   338,   229,   230,   339,   340,
     341,   342,     0,   343,   344,   345,   346,   347,   234,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,     0,   235,     0,   361,   362,   363,   237,     0,
       0,   238,     0,   239,     0,   240,   241,   242,     0,     0,
     364,     0,     0,     0,     0,   365,     0,     0,   366,     0,
       0,   367,   368,     0,   369,   370,    85,    86,    87,    88,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,   274,    92,    93,
       0,    94,    95,    96,   275,     0,     0,    97,     0,    98,
       0,    99,   100,   101,   276,   102,     0,     0,   103,     0,
     104,   277,   278,   105,     0,     0,   106,   107,   108,   109,
     110,   111,     0,     0,   112,   113,   114,   279,   115,   280,
     116,   281,     0,     0,   118,   119,     0,     0,     0,     0,
     120,   121,   122,   123,   282,   124,   125,   126,     0,     0,
     127,     0,   283,   128,   129,     0,   130,     0,     0,   131,
       0,   284,     0,   132,     0,     0,   133,     0,     0,   134,
     135,     0,   136,   137,     0,     0,   138,   139,     0,   140,
     141,   142,   143,   285,     0,     0,     0,   144,     0,   145,
       0,   146,   286,     0,     0,   147,   287,     0,   148,   149,
       0,     0,   150,     0,     0,   151,     0,     0,   152,   153,
       0,     0,     0,     0,   288,   154,     0,     0,     0,   155,
     289,   156,   290,     0,     0,   157,   158,   159,   160,   161,
     162,   163,     0,   164,   165,   291,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   176,   177,   292,
     178,   293,   294,   179,     0,   180,     0,     0,     0,     0,
       0,     0,     0,   181,   182,     0,     0,     0,   183,   184,
     185,   186,   187,   188,   189,   190,     0,   295,     0,     0,
     191,     0,   192,     0,   193,   194,     0,     0,   195,   196,
     197,     0,   198,   199,   200,     0,     0,   201,     0,   202,
       0,     0,   203,     0,   296,     0,     0,   204,   205,   297,
     298,   299,   300,     0,     0,   206,   207,     0,     0,     0,
     208,     0,     0,     0,   209,     0,     0,     0,   210,     0,
     211,     0,     0,   301,   212,   302,   213,   303,     0,     0,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,     0,   218,     0,   219,   220,   304,   305,
       0,     0,     0,   306,     0,     0,     0,     0,     0,   223,
     307,     0,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,     0,     0,   224,   318,   319,   320,     0,
     321,   322,   323,   324,   325,   326,   327,     0,   328,   329,
       0,     0,   330,   226,   331,   227,  1451,   333,   334,   335,
     336,   337,     0,   338,   229,   230,   339,   340,   341,   342,
       0,   343,   344,   345,   346,   347,   234,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,   235,     0,   361,   362,   363,   237,     0,     0,   238,
       0,   239,     0,   240,   241,   242,     0,     0,   364,     0,
       0,     0,     0,   365,     0,     0,   366,     0,     0,   367,
     368,     0,   369,   370,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,   274,    92,    93,     0,    94,
      95,    96,   275,     0,     0,    97,     0,    98,     0,    99,
     100,   101,   276,   102,     0,     0,   103,     0,   104,   277,
     278,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,   279,   115,   280,   116,   281,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,   282,   124,   125,   126,     0,     0,   127,     0,
     283,   128,   129,     0,   130,     0,     0,   131,     0,   284,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,   285,     0,     0,     0,   144,     0,   145,     0,   146,
     286,     0,     0,   147,   287,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,   288,   154,     0,     0,     0,   155,   289,   156,
     290,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,   291,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,   292,   178,   293,
     294,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,   295,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,   296,     0,     0,   204,   205,   297,   298,   299,
     300,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,   301,   212,   302,   213,   303,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   304,   305,     0,     0,
       0,   306,     0,     0,     0,     0,     0,   223,   307,     0,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,     0,     0,   224,   318,   319,   320,     0,   321,   322,
     323,   324,   325,   326,   327,     0,   328,   329,     0,     0,
     330,   226,   331,   227,  1453,   333,   334,   335,   336,   337,
       0,   338,   229,   230,   339,   340,   341,   342,     0,   343,
     344,   345,   346,   347,   234,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,     0,   235,
       0,   361,   362,   363,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,     0,     0,   364,     0,     0,     0,
       0,   365,     0,     0,   366,     0,     0,   367,   368,     0,
     369,   370,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,    91,    92,    93,     0,    94,    95,    96,
       0,     0,     0,    97,     0,    98,     0,    99,   100,   101,
       0,   102,     0,     0,   103,     0,   104,     0,     0,   105,
       0,     0,   106,   107,   108,   109,   110,   111,  1526,     0,
     112,   113,   114,     0,   115,     0,   116,   117,     0,     0,
     118,   119,     0,     0,     0,     0,   120,   121,   122,   123,
       0,   124,   125,   126,     0,     0,   127,     0,     0,   128,
     129,     0,   130,     0,     0,   131,  1541,     0,     0,   132,
       0,     0,   133,     0,     0,   134,   135,     0,   136,   137,
       0,     0,   138,   139,     0,   140,   141,   142,   143,     0,
       0,     0,     0,   144,  1542,   145,     0,   146,     0,     0,
       0,   147,     0,     0,   148,   149,     0,  1543,   150,     0,
       0,   151,     0,     0,   152,   153,     0,     0,  1544,     0,
       0,   154,     0,     0,     0,   155,     0,   156,     0,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,   164,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   176,   177,     0,   178,     0,     0,   179,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   183,   184,   185,   186,   187,   188,
     189,   190,     0,     0,     0,     0,   191,     0,   192,     0,
     193,   194,     0,     0,   195,   196,   197,     0,   198,   199,
     200,     0,     0,   201,     0,   202,     0,     0,   203,     0,
       0,     0,     0,   204,   205,     0,     0,     0,     0,     0,
       0,   206,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,   211,     0,     0,     0,
     212,     0,   213,   214,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
     218,     0,   219,   220,   221,   222,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   226,
       0,   227,     0,     0,     0,     0,     0,     0,     0,   228,
     229,   230,   231,     0,   232,     0,     0,     0,     0,     0,
       0,   233,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,     0,   236,
       0,     0,   237,     0,     0,   238,     0,   239,     0,   240,
     241,   242,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1588,
      90,     0,     0,    91,    92,    93,     0,    94,    95,    96,
       0,     0,     0,    97,     0,    98,     0,    99,   100,   101,
       0,   102,     0,     0,   103,     0,   104,     0,     0,   105,
       0,     0,   106,   107,   108,   109,   110,   111,     0,     0,
     112,   113,   114,     0,   115,     0,   116,   117,     0,     0,
     118,   119,     0,     0,     0,     0,   120,   121,   122,   123,
       0,   124,   125,   126,     0,     0,   127,     0,     0,   128,
     129,     0,   130,     0,     0,   131,  1541,     0,     0,   132,
       0,     0,   133,     0,     0,   134,   135,     0,   136,   137,
       0,     0,   138,   139,     0,   140,   141,   142,   143,     0,
       0,     0,     0,   144,  1542,   145,     0,   146,     0,     0,
       0,   147,     0,     0,   148,   149,     0,  1543,   150,     0,
       0,   151,     0,     0,   152,   153,     0,     0,  1544,     0,
       0,   154,     0,     0,     0,   155,     0,   156,     0,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,   164,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   176,   177,     0,   178,     0,     0,   179,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   183,   184,   185,   186,   187,   188,
     189,   190,     0,     0,     0,     0,   191,     0,   192,     0,
     193,   194,     0,     0,   195,   196,   197,     0,   198,   199,
     200,     0,     0,   201,     0,   202,     0,     0,   203,     0,
       0,     0,     0,   204,   205,     0,     0,     0,     0,     0,
       0,   206,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,   211,     0,     0,     0,
     212,     0,   213,   214,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
     218,     0,   219,   220,   221,   222,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   226,
       0,   227,     0,     0,     0,     0,     0,     0,     0,   228,
     229,   230,   231,     0,   232,     0,     0,     0,     0,     0,
       0,   233,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,     0,   236,
       0,     0,   237,     0,     0,   238,     0,   239,     0,   240,
     241,   242,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1588,
      90,     0,     0,    91,    92,    93,     0,    94,    95,    96,
       0,     0,     0,    97,     0,    98,     0,    99,   100,   101,
       0,   102,     0,     0,   103,     0,   104,     0,     0,   105,
       0,     0,   106,   107,   108,   109,   110,   111,    23,     0,
     112,   113,   114,     0,   115,     0,   116,   117,     0,     0,
     118,   119,     0,     0,     0,     0,   120,   121,   122,   123,
       0,   124,   125,   126,     0,     0,   127,     0,     0,   128,
     129,     0,   130,     0,     0,   131,     0,     0,     0,   132,
       0,     0,   133,     0,     0,   134,   135,     0,   136,   137,
       0,     0,   138,   139,     0,   140,   141,   142,   143,     0,
       0,     0,     0,   144,     0,   145,     0,   146,     0,     0,
       0,   147,     0,     0,   148,   149,     0,     0,   150,     0,
       0,   151,     0,     0,   152,   153,     0,     0,     0,     0,
       0,   154,     0,     0,     0,   155,     0,   156,     0,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,   164,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   176,   177,     0,   178,     0,     0,   179,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   183,   184,   185,   186,   187,   188,
     189,   190,     0,     0,     0,     0,   191,     0,   192,     0,
     193,   194,     0,     0,   195,   196,   197,     0,   198,   199,
     200,     0,     0,   201,     0,   202,     0,     0,   203,     0,
       0,     0,     0,   204,   205,     0,     0,     0,     0,     0,
       0,   206,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,   211,     0,     0,     0,
     212,     0,   213,   214,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
     218,     0,   219,   220,   221,   222,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   226,
       0,   227,     0,     0,     0,     0,     0,     0,     0,   228,
     229,   230,   231,     0,   232,     0,     0,     0,     0,     0,
       0,   233,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,     0,   236,
       0,     0,   237,     0,     0,   238,     0,   239,     0,   240,
     241,   242,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,    42,     0,     0,     0,     0,     0,     0,   243,
      90,     0,     0,    91,    92,    93,     0,    94,    95,    96,
       0,     0,     0,    97,     0,    98,     0,    99,   100,   101,
       0,   102,     0,     0,   103,     0,   104,     0,     0,   105,
       0,     0,   106,   107,   108,   109,   110,   111,  1526,     0,
     112,   113,   114,     0,   115,     0,   116,   117,     0,     0,
     118,   119,     0,     0,     0,     0,   120,   121,   122,   123,
       0,   124,   125,   126,     0,     0,   127,     0,     0,   128,
     129,     0,   130,     0,     0,   131,     0,     0,     0,   132,
       0,     0,   133,     0,     0,   134,   135,     0,   136,   137,
       0,     0,   138,   139,     0,   140,   141,   142,   143,     0,
       0,     0,     0,   144,     0,   145,     0,   146,     0,     0,
       0,   147,     0,     0,   148,   149,     0,     0,   150,     0,
       0,   151,     0,     0,   152,   153,     0,     0,     0,     0,
       0,   154,     0,     0,     0,   155,     0,   156,     0,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,   164,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   176,   177,     0,   178,     0,     0,   179,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   183,   184,   185,   186,   187,   188,
     189,   190,     0,     0,     0,     0,   191,     0,   192,     0,
     193,   194,     0,     0,   195,   196,   197,     0,   198,   199,
     200,     0,     0,   201,     0,   202,     0,     0,   203,     0,
       0,     0,     0,   204,   205,     0,     0,     0,     0,     0,
       0,   206,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,   211,     0,     0,     0,
     212,     0,   213,   214,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
     218,     0,   219,   220,   221,   222,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   226,
       0,   227,     0,     0,     0,     0,     0,     0,     0,   228,
     229,   230,   231,     0,   232,     0,     0,     0,     0,     0,
       0,   233,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,     0,   236,
       0,     0,   237,     0,     0,   238,     0,   239,     0,   240,
     241,   242,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,     0,  1769,     0,     0,     0,     0,     0,   370,
      90,     0,     0,    91,    92,    93,     0,    94,    95,    96,
       0,     0,     0,    97,     0,    98,     0,    99,   100,   101,
       0,   102,     0,     0,   103,     0,   104,     0,     0,   105,
       0,     0,   106,   107,   108,   109,   110,   111,     0,     0,
     112,   113,   114,     0,   115,     0,   116,   117,     0,     0,
     118,   119,     0,     0,     0,     0,   120,   121,   122,   123,
       0,   124,   125,   126,     0,     0,   127,     0,     0,   128,
     129,     0,   130,     0,     0,   131,     0,     0,     0,   132,
       0,     0,   133,     0,     0,   134,   135,     0,   136,   137,
       0,     0,   138,   139,     0,   140,   141,   142,   143,     0,
       0,     0,     0,   144,     0,   145,     0,   146,     0,     0,
       0,   147,     0,     0,   148,   149,     0,     0,   150,     0,
       0,   151,   949,     0,   152,   153,     0,     0,     0,     0,
       0,   154,     0,     0,     0,   155,     0,   156,     0,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,   164,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   176,   177,     0,   178,     0,     0,   179,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   183,   184,   185,   186,   187,   188,
     189,   190,     0,     0,     0,     0,   191,     0,   192,     0,
     193,   194,     0,     0,   195,   196,   197,     0,   198,   199,
     200,     0,     0,   201,     0,   202,     0,     0,   203,     0,
       0,     0,     0,   204,   205,     0,     0,     0,     0,     0,
       0,   206,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,   211,     0,     0,     0,
     212,     0,   213,   214,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
     218,     0,   219,   220,   221,   222,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   226,
       0,   227,     0,     0,     0,     0,     0,     0,     0,   228,
     229,   230,   231,     0,   232,     0,     0,     0,     0,     0,
       0,   233,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,     0,   236,
       0,     0,   237,     0,     0,   238,     0,   239,     0,   240,
     241,   242,    85,    86,    87,    88,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
      90,     0,     0,    91,    92,    93,     0,    94,    95,    96,
       0,     0,     0,    97,     0,    98,     0,    99,   100,   101,
       0,   102,     0,     0,   103,     0,   104,     0,     0,   105,
       0,     0,   106,   107,   108,   109,   110,   111,     0,     0,
     112,   113,   114,     0,   115,     0,   116,   117,     0,     0,
     118,   119,     0,     0,     0,     0,   120,   121,   122,   123,
       0,   124,   125,   126,     0,     0,   127,     0,     0,   128,
     129,     0,   130,     0,     0,   131,     0,     0,     0,   132,
       0,     0,   133,     0,     0,   134,   135,     0,   136,   137,
       0,     0,   138,   139,     0,   140,   141,   142,   143,     0,
       0,     0,     0,   144,     0,   145,     0,   146,     0,     0,
       0,   147,     0,     0,   148,   149,     0,     0,   150,     0,
       0,   151,     0,     0,   152,   153,     0,     0,     0,     0,
       0,   154,     0,     0,     0,   155,     0,   156,     0,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,   164,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   176,   177,     0,   178,     0,     0,   179,
       0,   180,     0,     0,     0,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   183,   184,   185,   186,   187,   188,
     189,   190,     0,     0,     0,     0,   191,     0,   192,     0,
     193,   194,     0,     0,   195,   196,   197,     0,   198,   199,
     200,     0,     0,   201,     0,   202,     0,     0,   203,     0,
       0,     0,     0,   204,   205,     0,     0,     0,     0,     0,
       0,   206,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,   211,     0,     0,     0,
     212,     0,   213,   214,     0,     0,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
     218,     0,   219,   220,   221,   222,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   226,
       0,   227,     0,     0,     0,     0,     0,     0,     0,   228,
     229,   230,   231,     0,   232,     0,     0,     0,     0,     0,
       0,   233,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,     0,   236,
       0,     0,   237,     0,     0,   238,     0,   239,     0,   240,
     241,   242,     0,     0,     0,     0,  1230,    85,    86,    87,
      88,    89,     0,     0,     0,     0,     0,     0,     0,   243,
       0,     0,     0,     0,     0,    90,     0,     0,    91,    92,
      93,     0,    94,    95,    96,     0,     0,     0,    97,     0,
      98,     0,    99,   100,   101,     0,   102,     0,     0,   103,
       0,   104,     0,     0,   105,     0,     0,   106,   107,   108,
     109,   110,   111,     0,     0,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   118,   119,     0,     0,     0,
       0,   120,   121,   122,   123,     0,   124,   125,   126,     0,
       0,   127,     0,     0,   128,   129,     0,   130,     0,     0,
     131,     0,     0,     0,   132,     0,     0,   133,     0,     0,
     134,   135,     0,   136,   137,     0,     0,   138,   139,     0,
     140,   141,   142,   143,     0,     0,     0,     0,   144,     0,
     145,     0,   146,     0,     0,     0,   147,     0,     0,   148,
     149,     0,     0,   150,     0,     0,   151,     0,     0,   152,
     153,     0,     0,     0,     0,     0,   154,     0,     0,     0,
     155,     0,   156,     0,     0,     0,   157,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   176,   177,
       0,   178,     0,     0,   179,     0,   180,     0,     0,     0,
       0,     0,     0,     0,   181,   182,     0,     0,     0,   183,
     184,   185,   186,   187,   188,   189,   190,     0,     0,     0,
       0,   191,     0,   192,     0,   193,   194,     0,     0,   195,
     196,   197,     0,   198,   199,   200,     0,     0,   201,     0,
     202,     0,     0,   203,     0,     0,     0,     0,   204,   205,
       0,     0,     0,     0,     0,     0,   206,   207,     0,     0,
       0,   208,     0,     0,     0,   209,     0,     0,     0,   210,
       0,   211,     0,     0,     0,   212,     0,   213,   214,     0,
       0,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   226,     0,   227,     0,     0,     0,
       0,     0,     0,     0,   228,   229,   230,   231,     0,   232,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   235,     0,   236,     0,     0,   237,     0,     0,
     238,     0,   239,     0,   240,   241,   242,    85,    86,    87,
      88,    89,     0,     0,     0,     0,     0,  1587,     0,     0,
       0,     0,     0,     0,  1588,    90,     0,     0,    91,    92,
      93,     0,    94,    95,    96,     0,     0,     0,    97,     0,
      98,     0,    99,   100,   101,     0,   102,     0,     0,   103,
       0,   104,     0,     0,   105,     0,     0,   106,   107,   108,
     109,   110,   111,     0,     0,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   118,   119,     0,     0,     0,
       0,   120,   121,   122,   123,     0,   124,   125,   126,     0,
       0,   127,     0,     0,   128,   129,     0,   130,     0,     0,
     131,     0,     0,     0,   132,     0,     0,   133,     0,     0,
     134,   135,     0,   136,   137,     0,     0,   138,   139,     0,
     140,   141,   142,   143,     0,     0,     0,     0,   144,     0,
     145,     0,   146,     0,     0,     0,   147,     0,     0,   148,
     149,     0,     0,   150,     0,     0,   151,     0,     0,   152,
     153,     0,     0,     0,     0,     0,   154,     0,     0,     0,
     155,     0,   156,     0,     0,     0,   157,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   176,   177,
       0,   178,     0,     0,   179,     0,   180,     0,     0,     0,
       0,     0,     0,     0,   181,   182,     0,     0,     0,   183,
     184,   185,   186,   187,   188,   189,   190,     0,     0,     0,
       0,   191,     0,   192,     0,   193,   194,     0,     0,   195,
     196,   197,     0,   198,   199,   200,     0,     0,   201,     0,
     202,     0,     0,   203,     0,     0,     0,     0,   204,   205,
       0,     0,     0,     0,     0,     0,   206,   207,     0,     0,
       0,   208,     0,     0,     0,   209,     0,     0,     0,   210,
       0,   211,     0,     0,     0,   212,     0,   213,   214,     0,
       0,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   226,     0,   227,     0,     0,     0,
       0,     0,     0,     0,   228,   229,   230,   231,     0,   232,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   235,     0,   236,     0,     0,   237,     0,     0,
     238,     0,   239,     0,   240,   241,   242,    85,    86,    87,
      88,    89,     0,     0,     0,     0,     0,     0,  2399,     0,
       0,     0,     0,     0,   370,    90,     0,     0,    91,    92,
      93,     0,    94,    95,    96,     0,     0,     0,    97,     0,
      98,     0,    99,   100,   101,     0,   102,     0,     0,   103,
       0,   104,     0,     0,   105,     0,     0,   106,   107,   108,
     109,   110,   111,     0,     0,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   118,   119,     0,     0,     0,
       0,   120,   121,   122,   123,     0,   124,   125,   126,     0,
       0,   127,     0,     0,   128,   129,     0,   130,     0,     0,
     131,     0,     0,     0,   132,     0,     0,   133,     0,     0,
     134,   135,     0,   136,   137,     0,     0,   138,   139,     0,
     140,   141,   142,   143,     0,     0,     0,     0,   144,     0,
     145,     0,   146,     0,     0,     0,   147,     0,     0,   148,
     149,     0,     0,   150,     0,     0,   151,     0,     0,   152,
     153,     0,     0,     0,     0,     0,   154,     0,     0,     0,
     155,     0,   156,     0,     0,     0,   157,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   176,   177,
       0,   178,     0,     0,   179,     0,   180,     0,     0,     0,
       0,     0,     0,     0,   181,   182,     0,     0,     0,   183,
     184,   185,   186,   187,   188,   189,   190,     0,     0,     0,
       0,   191,     0,   192,     0,   193,   194,     0,     0,   195,
     196,   197,     0,   198,   199,   200,     0,     0,   201,     0,
     202,     0,     0,   203,     0,     0,     0,     0,   204,   205,
       0,     0,     0,     0,     0,     0,   206,   207,     0,     0,
       0,   208,     0,     0,     0,   209,     0,     0,     0,   210,
       0,   211,     0,     0,     0,   212,     0,   213,   214,     0,
       0,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   226,     0,   227,     0,     0,     0,
       0,     0,     0,     0,   228,   229,   230,   231,     0,   232,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   235,     0,   236,     0,     0,   237,     0,     0,
     238,     0,   239,     0,   240,   241,   242,    85,    86,    87,
      88,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,    90,     0,     0,    91,    92,
      93,     0,    94,    95,    96,     0,     0,     0,    97,     0,
      98,     0,    99,   100,   101,     0,   102,     0,     0,   103,
       0,   104,     0,     0,   105,     0,     0,   106,   107,   108,
     109,   110,   111,     0,     0,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   118,   119,     0,     0,     0,
       0,   120,   121,   122,   123,     0,   124,   125,   126,     0,
       0,   127,     0,     0,   128,   129,     0,   130,     0,     0,
     131,     0,     0,     0,   132,     0,     0,   133,     0,     0,
     134,   135,     0,   136,   137,     0,     0,   138,   139,     0,
     140,   141,   142,   143,     0,     0,     0,     0,   144,     0,
     145,     0,   146,     0,     0,     0,   147,     0,     0,   148,
     149,     0,     0,   150,     0,     0,   151,     0,     0,   152,
     153,     0,     0,     0,     0,     0,   154,     0,     0,     0,
     155,     0,   156,     0,     0,     0,   157,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   176,   177,
       0,   178,     0,     0,   179,     0,   180,     0,     0,     0,
       0,     0,     0,     0,   181,   182,     0,     0,     0,   183,
     184,   185,   186,   187,   188,   189,   190,     0,     0,     0,
       0,   191,     0,   192,     0,   193,   194,     0,     0,   195,
     196,   197,     0,   198,   199,   200,     0,     0,   201,     0,
     202,     0,     0,   203,     0,     0,     0,     0,   204,   205,
       0,     0,     0,     0,     0,     0,   206,   207,     0,     0,
       0,   208,     0,     0,     0,   209,     0,     0,     0,   210,
       0,   211,     0,     0,     0,   212,     0,   213,   214,     0,
       0,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   226,     0,   227,     0,     0,     0,
       0,     0,     0,     0,   228,   229,   230,   231,     0,   232,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   235,     0,   236,     0,     0,   237,     0,     0,
     238,     0,   239,     0,   240,   241,   242,    85,    86,    87,
      88,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,    90,     0,     0,    91,    92,
      93,     0,    94,    95,    96,     0,     0,     0,    97,     0,
      98,     0,    99,   100,   101,     0,   102,     0,     0,   103,
       0,   104,     0,     0,   105,     0,     0,   106,   107,   108,
     109,   110,   111,     0,     0,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   118,   119,     0,     0,     0,
       0,   120,   121,   122,   123,     0,   124,   125,   126,     0,
       0,   127,     0,     0,   128,   129,     0,   130,     0,     0,
     131,     0,     0,     0,   132,     0,     0,   133,     0,     0,
     134,   135,     0,   136,   137,     0,     0,   138,   139,     0,
     140,   141,   142,   143,     0,     0,     0,     0,   144,     0,
     145,     0,   146,     0,     0,     0,   147,     0,     0,   148,
     149,     0,     0,   150,     0,     0,   151,     0,     0,   152,
     153,     0,     0,     0,     0,     0,   154,     0,     0,     0,
     155,     0,   156,     0,     0,     0,   157,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   176,   177,
       0,   178,     0,     0,   179,     0,   180,     0,     0,     0,
       0,     0,     0,     0,   181,   182,     0,     0,     0,   183,
     184,   185,   186,   187,   188,   189,   190,     0,     0,     0,
       0,   191,     0,   192,     0,   193,   194,     0,     0,   195,
     196,   197,     0,   198,   199,   200,     0,     0,   201,     0,
     202,     0,     0,   203,     0,     0,     0,     0,   204,   205,
       0,     0,     0,     0,     0,     0,   206,   207,     0,     0,
       0,   208,     0,     0,     0,   209,     0,     0,     0,   210,
       0,   211,     0,     0,     0,   212,     0,   213,   214,     0,
       0,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   226,     0,   227,     0,     0,     0,
       0,     0,     0,     0,   228,   229,   230,   231,     0,   232,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   235,     0,   236,     0,     0,   237,     0,     0,
     238,     0,   239,     0,   240,   241,   242,    85,    86,    87,
      88,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1588,    90,     0,     0,    91,    92,
      93,     0,    94,    95,    96,     0,     0,     0,    97,     0,
      98,     0,    99,   100,   101,     0,   102,     0,     0,   103,
       0,   104,     0,     0,   105,     0,     0,   106,   107,   108,
     109,   110,   111,     0,     0,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   118,   119,     0,     0,     0,
       0,   120,   121,   122,   123,     0,   124,   125,   126,     0,
       0,   127,     0,     0,   128,   129,     0,   130,     0,     0,
     131,     0,     0,     0,   132,     0,     0,   133,     0,     0,
     134,   135,     0,   136,   137,     0,     0,   138,   139,     0,
     140,   141,   142,   143,     0,     0,     0,     0,   144,     0,
     145,     0,   146,     0,     0,     0,   147,     0,     0,   148,
     149,     0,     0,   150,     0,     0,   151,     0,     0,   152,
     153,     0,     0,     0,     0,     0,   154,   557,     0,     0,
     155,     0,   156,     0,     0,     0,   157,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   176,   177,
       0,   178,     0,     0,   179,     0,   180,     0,     0,     0,
       0,     0,     0,     0,   181,   182,     0,     0,     0,   183,
     184,   185,   186,   187,   188,   189,   190,     0,     0,     0,
       0,   191,     0,   192,     0,   193,   194,     0,     0,   195,
     196,   197,     0,   198,   199,   200,     0,     0,   201,     0,
     202,     0,     0,   203,     0,   558,     0,     0,   204,   205,
       0,     0,     0,     0,     0,     0,   206,   207,     0,     0,
       0,   208,     0,     0,     0,   209,     0,     0,     0,   210,
       0,   211,     0,     0,     0,   212,     0,   213,   214,     0,
       0,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   226,     0,   227,     0,     0,     0,
       0,     0,     0,     0,   228,   229,   230,   231,     0,   232,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   235,     0,   236,     0,     0,   237,     0,     0,
     238,     0,   239,     0,   240,   241,   242,    85,    86,    87,
      88,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   559,     0,    90,     0,     0,    91,    92,
      93,     0,    94,    95,    96,     0,     0,     0,    97,     0,
      98,     0,    99,   100,   101,     0,   102,     0,     0,   103,
       0,   104,     0,     0,   105,     0,     0,   106,   107,   108,
     109,   110,   111,     0,     0,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   118,   119,     0,     0,     0,
       0,   120,   121,   122,   123,     0,   124,   125,   126,     0,
       0,   127,     0,     0,   128,   129,     0,   130,     0,     0,
     131,     0,     0,     0,   132,     0,     0,   133,     0,     0,
     134,   135,     0,   136,   137,     0,     0,   138,   139,     0,
     140,   141,   142,   143,     0,     0,     0,     0,   144,     0,
     145,     0,   146,     0,     0,     0,   147,     0,     0,   148,
     149,     0,     0,   150,     0,     0,   151,     0,     0,   152,
     153,     0,     0,     0,     0,     0,   154,   557,     0,     0,
     155,     0,   156,     0,     0,     0,   157,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   176,   177,
       0,   178,     0,     0,   179,     0,   180,     0,     0,     0,
       0,     0,     0,     0,   181,   182,     0,     0,     0,   183,
     184,   185,   186,   187,   188,   189,   190,     0,     0,     0,
       0,   191,     0,   192,     0,   193,   194,     0,     0,   195,
     196,   197,     0,   198,   199,   200,     0,     0,   201,     0,
     202,     0,     0,   203,     0,   558,     0,     0,   204,   205,
       0,     0,     0,     0,     0,     0,   206,   207,     0,     0,
       0,   208,     0,     0,     0,   209,     0,     0,     0,   210,
       0,   211,     0,     0,     0,   212,     0,   213,   214,     0,
       0,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   226,     0,   227,     0,     0,     0,
       0,     0,     0,     0,   228,   229,   230,   231,     0,   232,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   235,     0,   236,     0,     0,   237,     0,     0,
     238,     0,   239,     0,   240,   241,   242,    85,    86,    87,
      88,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1523,     0,    90,     0,     0,    91,    92,
      93,     0,    94,    95,    96,     0,     0,     0,    97,     0,
      98,     0,    99,   100,   101,     0,   102,     0,     0,   103,
       0,   104,     0,     0,   105,     0,     0,   106,   107,   108,
     109,   110,   111,     0,     0,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   118,   119,     0,     0,     0,
       0,   120,   121,   122,   123,     0,   124,   125,   126,     0,
       0,   127,     0,     0,   128,   129,     0,   130,     0,     0,
     131,     0,     0,     0,   132,     0,     0,   133,     0,     0,
     134,   135,     0,   136,   137,     0,     0,   138,   139,     0,
     140,   141,   142,   143,     0,     0,     0,     0,   144,     0,
     145,     0,   146,     0,     0,     0,   147,     0,     0,   148,
     149,     0,     0,   150,     0,     0,   151,     0,     0,   152,
     153,     0,     0,     0,     0,     0,   154,     0,     0,     0,
     155,     0,   156,     0,     0,     0,   157,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   176,   177,
       0,   178,     0,     0,   179,     0,   180,     0,     0,     0,
       0,     0,     0,     0,   181,   182,     0,     0,     0,   183,
     184,   185,   186,   187,   188,   189,   190,     0,     0,     0,
       0,   191,     0,   192,     0,   193,   194,     0,     0,   195,
     196,   197,     0,   198,   199,   200,     0,     0,   201,     0,
     202,     0,     0,   203,     0,     0,     0,     0,  1240,   205,
       0,     0,     0,     0,     0,     0,   206,   207,     0,     0,
       0,   208,     0,     0,     0,   209,     0,     0,     0,   210,
       0,   211,     0,     0,  1241,   212,     0,   213,   214,     0,
       0,   215,   216,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   217,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     223,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,     0,     0,     0,     0,   224,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,     0,     0,     0,
       0,     0,     0,   225,   226,     0,   227,     0,     0,   573,
       0,     0,     0,     0,   228,   229,   230,   231,     0,  1242,
       0,     0,     0,     0,     0,     0,   233,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   573,     0,     0,
       0,     0,   235,     0,   236,     0,     0,   237,     0,     0,
     238,     0,   239,   573,   240,   241,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1243,     0,     0,     0,     0,     0,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   575,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
       0,     0,     0,     0,     0,     0,     0,   574,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,     0,     0,
       0,   576,     0,   574,     0,     0,     0,     0,     0,     0,
     577,   578,     0,   575,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,     0,     0,     0,     0,   576,
       0,   579,     0,     0,     0,     0,   573,     0,   577,   578,
       0,     0,     0,     0,     0,   576,     0,     0,     0,     0,
       0,     0,     0,     0,   577,   578,     0,     0,     0,   579,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   579,     0,     0,     0,     0,
     573,   580,     0,     0,     0,     0,     0,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   574,     0,     0,   580,
       0,     0,     0,     0,     0,     0,   575,     0,     0,     0,
       0,     0,     0,   581,     0,   580,     0,     0,     0,     0,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
       0,     0,     0,     0,     0,     0,     0,     0,   576,     0,
     574,   581,     0,   573,     0,     0,     0,   577,   578,     0,
     575,     0,     0,     0,     0,     0,     0,   581,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   579,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   576,     0,     0,     0,   573,     0,     0,     0,
       0,   577,   578,     0,     0,     0,     0,     0,   582,   583,
     584,   585,   586,   587,   588,     0,     0,   589,     0,  1021,
    1022,     0,   579,   574,     0,     0,     0,     0,   580,     0,
       0,     0,     0,   575,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,     0,   589,     0,  1033,  1034,     0,
       0,     0,   582,   583,   584,   585,   586,   587,   588,     0,
       0,   589,     0,  1045,  1046,   576,   574,     0,     0,     0,
     581,     0,   580,     0,   577,   578,   575,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,     0,     0,     0,
       0,     0,     0,     0,     0,   579,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,   581,     0,     0,   577,   578,     0,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
       0,     0,     0,     0,     0,     0,     0,     0,   579,     0,
       0,     0,     0,   573,     0,   580,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   582,   583,   584,   585,   586,
     587,   588,     0,     0,   589,     0,  1047,  1048,     0,     0,
       0,     0,     0,     0,     0,     0,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,   573,   581,   580,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   582,
     583,   584,   585,   586,   587,   588,     0,     0,   589,     0,
    1051,  1052,     0,   574,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   575,     0,     0,     0,     0,     0,     0,
     581,     0,     0,     0,     0,  1106,     0,     0,     0,     0,
       0,     0,   573,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,     0,     0,   576,   574,     0,     0,     0,
       0,     0,     0,     0,   577,   578,   575,     0,     0,     0,
       0,     0,   582,   583,   584,   585,   586,   587,   588,     0,
       0,   589,     0,  1057,  1058,   579,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,   577,   578,   573,
       0,     0,   574,     0,     0,   582,   583,   584,   585,   586,
     587,   588,   575,     0,   589,     0,   836,  1062,   579,     0,
       0,     0,     0,     0,     0,   580,     0,     0,     0,     0,
       0,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,     0,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   578,     0,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,   581,   580,   574,
       0,     0,     0,     0,   579,     0,     0,     0,     0,   575,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
       0,     0,     0,     0,     0,     0,     0,   573,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
     581,   576,     0,     0,     0,     0,     0,     0,     0,     0,
     577,   578,   573,     0,   580,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   579,     0,     0,     0,     0,   573,     0,     0,     0,
       0,     0,   582,   583,   584,   585,   586,   587,   588,     0,
       0,   589,     0,  1103,  1104,   573,   581,   574,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,     0,     0,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
       0,   580,   574,     0,     0,   582,   583,   584,   585,   586,
     587,   588,   575,     0,   589,     0,     0,  1107,     0,   576,
       0,     0,     0,     0,     0,     0,   574,     0,   577,   578,
       0,     0,     0,     0,     0,     0,   575,     0,     0,     0,
       0,     0,     0,   581,   576,   574,     0,     0,     0,   579,
       0,     0,     0,   577,   578,   575,   573,     0,     0,     0,
       0,   582,   583,   584,   585,   586,   587,   588,   576,     0,
     589,     0,  1123,  1124,   579,     0,     0,   577,   578,     0,
       0,     0,     0,     0,     0,     0,     0,   576,     0,     0,
       0,     0,     0,     0,     0,     0,   577,   578,   579,   580,
       0,     0,     0,     0,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,     0,     0,   579,     0,     0,
       0,     0,     0,     0,   580,     0,   574,     0,   582,   583,
     584,   585,   586,   587,   588,     0,   575,   589,     0,  1126,
    1127,   581,     0,     0,     0,     0,     0,     0,   580,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   581,   580,   576,     0,
     573,     0,     0,     0,     0,     0,     0,   577,   578,     0,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     581,     0,     0,     0,     0,     0,     0,     0,   579,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   581,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,     0,   589,     0,  1662,  1663,     0,
     574,     0,     0,     0,     0,     0,   573,     0,   580,     0,
     575,   582,   583,   584,   585,   586,   587,   588,     0,     0,
     589,     0,  1665,  1666,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,   582,   583,   584,   585,   586,
     587,   588,   576,     0,   589,     0,  1681,  1682,     0,     0,
     581,   577,   578,     0,   582,   583,   584,   585,   586,   587,
     588,     0,     0,   589,     0,  2029,  2030,     0,     0,     0,
       0,     0,   579,     0,     0,     0,   574,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   575,     0,     0,     0,
     573,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,   580,     0,     0,     0,     0,   577,   578,     0,
       0,     0,     0,     0,     0,   582,   583,   584,   585,   586,
     587,   588,     0,     0,   589,     0,  2265,  2266,   579,     0,
       0,     0,     0,     0,     0,     0,     0,   573,     0,     0,
     574,     0,     0,     0,   581,     0,     0,     0,     0,     0,
     575,     0,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   580,     0,
       0,     0,   576,     0,     0,     0,     0,     0,     0,     0,
       0,   577,   578,     0,     0,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,     0,     0,   574,     0,     0,
       0,     0,   579,     0,     0,     0,     0,   575,   573,     0,
     581,     0,     0,     0,     0,     0,     0,     0,     0,   582,
     583,   584,   585,   586,   587,   588,     0,     0,   589,     0,
       0,  1011,     0,     0,     0,     0,     0,     0,     0,   576,
       0,     0,     0,     0,     0,     0,     0,     0,   577,   578,
       0,   573,   580,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   579,
       0,     0,     0,     0,     0,     0,     0,     0,   574,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   575,     0,
       0,     0,     0,     0,   581,   582,   583,   584,   585,   586,
     587,   588,     0,     0,   589,     0,     0,  1016,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,     0,   580,
     576,   574,     0,     0,     0,     0,     0,     0,     0,   577,
     578,   575,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,     0,     0,     0,     0,     0,     0,     0,     0,
     579,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   581,     0,   576,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,   573,     0,     0,     0,     0,   582,
     583,   584,   585,   586,   587,   588,     0,     0,   589,     0,
       0,  1018,     0,   579,     0,     0,     0,     0,   573,     0,
     580,     0,     0,     0,     0,     0,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,   581,   580,   574,     0,   582,   583,   584,   585,
     586,   587,   588,     0,   575,   589,     0,     0,  1019,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   574,     0,
       0,     0,   573,     0,     0,     0,     0,     0,   575,     0,
       0,     0,     0,     0,     0,   581,   576,     0,     0,     0,
       0,     0,     0,     0,     0,   577,   578,   573,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
     576,     0,     0,     0,     0,     0,   579,     0,     0,   577,
     578,     0,     0,     0,     0,     0,     0,   582,   583,   584,
     585,   586,   587,   588,     0,     0,   589,     0,     0,  1020,
     579,     0,   574,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   575,     0,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,   573,   580,   574,     0,     0,
     582,   583,   584,   585,   586,   587,   588,   575,     0,   589,
       0,     0,  1027,     0,   576,     0,     0,     0,     0,     0,
     580,     0,     0,   577,   578,     0,     0,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,     0,   581,   576,
       0,     0,     0,     0,   579,     0,     0,     0,   577,   578,
     573,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   581,     0,     0,   574,     0,     0,     0,   579,
       0,     0,     0,     0,     0,   575,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   573,   580,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   576,     0,     0,
       0,     0,     0,     0,     0,     0,   577,   578,     0,   580,
     574,     0,     0,   582,   583,   584,   585,   586,   587,   588,
     575,     0,   589,     0,     0,  1028,   581,   579,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   582,   583,   584,
     585,   586,   587,   588,     0,     0,   589,     0,     0,  1040,
       0,   581,   576,   574,     0,     0,     0,     0,     0,     0,
       0,   577,   578,   575,     0,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,     0,     0,   580,     0,     0,
       0,     0,   579,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   576,     0,     0,     0,     0,
       0,     0,     0,     0,   577,   578,     0,     0,     0,     0,
       0,   582,   583,   584,   585,   586,   587,   588,     0,   581,
     589,     0,     0,  1042,     0,   579,     0,     0,     0,     0,
       0,   573,   580,     0,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,     0,   589,     0,     0,  1043,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
       0,     0,     0,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,     0,   581,   580,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,     0,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
       0,   574,     0,     0,   582,   583,   584,   585,   586,   587,
     588,   575,     0,   589,     0,   573,  1044,   581,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   573,
       0,     0,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   576,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,     0,   573,     0,     0,     0,   582,
     583,   584,   585,   586,   587,   588,     0,     0,   589,     0,
       0,  1049,     0,   579,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   574,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   575,     0,     0,   573,   574,
       0,     0,   582,   583,   584,   585,   586,   587,   588,   575,
       0,   589,   574,     0,  1050,     0,     0,     0,     0,     0,
       0,     0,   575,   580,     0,   574,     0,   576,     0,     0,
       0,     0,     0,     0,     0,   575,   577,   578,     0,     0,
       0,   576,     0,     0,     0,     0,     0,     0,     0,     0,
     577,   578,     0,     0,   576,     0,     0,   579,     0,     0,
       0,     0,     0,   577,   578,   581,     0,   576,   574,     0,
       0,   579,     0,     0,     0,     0,   577,   578,   575,     0,
       0,     0,     0,     0,   579,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   579,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   580,     0,     0,
     576,     0,     0,     0,     0,     0,     0,     0,     0,   577,
     578,   580,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,     0,     0,   580,     0,     0,     0,     0,     0,
     579,     0,     0,     0,     0,     0,     0,   580,     0,   581,
     582,   583,   584,   585,   586,   587,   588,     0,     0,   589,
       0,     0,  1053,   581,     0,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,     0,   581,     0,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   581,
     580,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,     0,     0,     0,     0,     0,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,     0,     0,
       0,   573,   581,     0,   582,   583,   584,   585,   586,   587,
     588,     0,     0,   589,   573,     0,  1055,     0,   582,   583,
     584,   585,   586,   587,   588,     0,     0,   589,   574,     0,
    1056,   582,   583,   584,   585,   586,   587,   588,   575,   573,
     589,     0,     0,  1061,   582,   583,   584,   585,   586,   587,
     588,     0,     0,   589,   573,     0,  1065,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     576,   574,     0,     0,     0,     0,     0,     0,     0,   577,
     578,   575,     0,     0,   574,     0,     0,   582,   583,   584,
     585,   586,   587,   588,   575,     0,   589,     0,     0,  1066,
     579,     0,     0,     0,     0,     0,     0,     0,     0,   574,
       0,     0,     0,   576,     0,     0,     0,     0,     0,   575,
       0,     0,   577,   578,   574,     0,   576,     0,     0,     0,
       0,     0,     0,     0,   575,   577,   578,     0,     0,     0,
       0,     0,     0,   579,     0,     0,     0,     0,     0,     0,
     580,   576,     0,     0,     0,     0,   579,     0,     0,     0,
     577,   578,     0,     0,     0,     0,   576,     0,     0,     0,
       0,     0,     0,     0,     0,   577,   578,     0,     0,     0,
       0,   579,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   581,   580,     0,     0,   579,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   580,     0,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   580,     0,     0,     0,   581,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   580,     0,   581,     0,
       0,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   581,   573,     0,     0,   582,   583,   584,
     585,   586,   587,   588,     0,     0,   589,     0,   581,  1067,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
       0,     0,     0,     0,     0,     0,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,   573,     0,     0,
     582,   583,   584,   585,   586,   587,   588,     0,     0,   589,
       0,     0,  1068,   582,   583,   584,   585,   586,   587,   588,
       0,     0,   589,     0,   574,  1101,     0,     0,     0,     0,
       0,     0,     0,     0,   575,     0,   573,     0,   582,   583,
     584,   585,   586,   587,   588,     0,     0,   589,     0,     0,
    1102,     0,   573,   582,   583,   584,   585,   586,   587,   588,
       0,     0,   589,     0,     0,  1108,   576,   574,     0,     0,
       0,     0,     0,     0,     0,   577,   578,   575,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
       0,     0,     0,     0,     0,     0,   579,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   574,     0,     0,   576,
       0,     0,     0,     0,     0,     0,   575,     0,   577,   578,
       0,     0,   574,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   575,     0,     0,     0,     0,     0,     0,   579,
       0,     0,     0,     0,     0,   573,   580,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,   577,   578,     0,
       0,     0,     0,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   578,     0,     0,     0,   579,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   581,   580,
       0,     0,     0,     0,   579,     0,     0,     0,     0,     0,
       0,     0,     0,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,     0,     0,   574,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   575,     0,     0,   580,     0,
       0,   581,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,     0,     0,   580,     0,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,   576,     0,     0,
       0,     0,     0,     0,     0,     0,   577,   578,     0,   573,
     581,     0,     0,   582,   583,   584,   585,   586,   587,   588,
       0,     0,   589,     0,     0,  1625,   581,   579,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   573,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
       0,     0,   573,     0,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,     0,   589,     0,     0,  1637,     0,
       0,     0,     0,     0,     0,     0,     0,   580,     0,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   575,
       0,     0,     0,     0,     0,   582,   583,   584,   585,   586,
     587,   588,     0,     0,   589,   573,     0,  1653,   574,     0,
       0,   582,   583,   584,   585,   586,   587,   588,   575,   581,
     589,   576,   574,  1659,     0,     0,     0,     0,     0,     0,
     577,   578,   575,     0,     0,     0,     0,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,     0,     0,     0,
     576,   579,     0,     0,  1113,     0,     0,     0,     0,   577,
     578,     0,     0,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   578,   574,     0,     0,     0,     0,
     579,     0,     0,     0,     0,   575,     0,     0,     0,     0,
       0,     0,     0,     0,   579,     0,     0,     0,     0,     0,
       0,   580,     0,   573,   582,   583,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,  1676,   576,     0,     0,
       0,     0,     0,     0,     0,     0,   577,   578,     0,     0,
     580,     0,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,     0,   581,   580,     0,     0,   579,     0,     0,
       0,     0,     0,     0,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,     0,     0,     0,     0,     0,
       0,     0,   581,   574,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   575,     0,     0,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   580,   573,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1679,     0,     0,   576,     0,     0,     0,     0,
     573,     0,     0,     0,   577,   578,     0,     0,   582,   583,
     584,   585,   586,   587,   588,     0,     0,   589,     0,   581,
    1683,     0,     0,     0,     0,   579,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   582,   583,   584,
     585,   586,   587,   588,     0,     0,   589,     0,   574,  2023,
       0,   582,   583,   584,   585,   586,   587,   588,   575,     0,
     589,     0,  1680,     0,     0,     0,     0,     0,     0,     0,
     574,     0,     0,     0,     0,   580,     0,     0,     0,     0,
     575,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     576,     0,     0,     0,     0,     0,     0,     0,     0,   577,
     578,     0,     0,     0,   582,   583,   584,   585,   586,   587,
     588,     0,   576,   589,     0,  1114,     0,   581,     0,     0,
     579,   577,   578,     0,     0,     0,     0,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,     0,     0,     0,
       0,     0,   579,     0,     0,     0,     0,     0,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     580,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   580,   573,     0,     0,     0,     0,     0,     0,
       0,     0,   582,   583,   584,   585,   586,   587,   588,     0,
       0,   589,   581,   836,     0,   573,     0,     0,     0,     0,
       0,     0,     0,     0,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,   581,     0,     0,   573,     0,     0,
       0,     0,     0,     0,     0,     0,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,     0,     0,     0,
       0,     0,     0,   574,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   575,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   574,     0,     0,     0,     0,
     573,     0,     0,     0,     0,   575,     0,   582,   583,   584,
     585,   586,   587,   588,     0,   576,   589,   574,  1026,     0,
       0,     0,   573,     0,   577,   578,     0,   575,     0,   582,
     583,   584,   585,   586,   587,   588,     0,   576,   589,     0,
    1041,     0,     0,     0,     0,   579,   577,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   576,
       0,     0,     0,     0,     0,     0,     0,   579,   577,   578,
     574,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     575,     0,     0,     0,     0,     0,     0,     0,     0,   579,
       0,     0,   574,     0,     0,   580,     0,     0,     0,     0,
       0,     0,   575,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   576,     0,     0,     0,     0,   580,     0,     0,
       0,   577,   578,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   576,     0,     0,   581,     0,   580,
       0,     0,   579,   577,   578,     0,     0,     0,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   581,
       0,     0,     0,     0,   579,     0,     0,     0,     0,     0,
       0,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   581,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   580,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   580,   573,     0,     0,     0,     0,
       0,     0,   582,   583,   584,   585,   586,   587,   588,     0,
       0,   589,     0,  1060,   581,     0,     0,   573,     0,     0,
       0,     0,     0,     0,   582,   583,   584,   585,   586,   587,
     588,     0,     0,   589,     0,  1064,   581,     0,     0,   573,
       0,     0,     0,     0,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,     0,   589,     0,  1069,     0,     0,
       0,     0,     0,     0,     0,   574,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   575,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,   574,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,     0,   582,
     583,   584,   585,   586,   587,   588,     0,   576,   589,   574,
    1070,     0,     0,     0,     0,     0,   577,   578,     0,   575,
       0,   582,   583,   584,   585,   586,   587,   588,     0,   576,
     589,     0,  1071,     0,     0,     0,     0,   579,   577,   578,
       0,     0,   573,     0,     0,     0,     0,     0,     0,     0,
       0,   576,     0,     0,     0,     0,     0,     0,     0,   579,
     577,   578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   579,     0,     0,     0,     0,     0,   580,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   580,
       0,     0,   574,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   575,     0,     0,     0,     0,     0,     0,   581,
       0,   580,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   581,     0,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   578,     0,     0,     0,     0,     0,
       0,     0,     0,   581,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   579,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   573,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
       0,     0,     0,     0,   582,   583,   584,   585,   586,   587,
     588,     0,     0,   589,     0,  1072,     0,     0,     0,     0,
       0,     0,     0,     0,   580,     0,   582,   583,   584,   585,
     586,   587,   588,     0,     0,   589,     0,  1100,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   582,   583,
     584,   585,   586,   587,   588,   573,     0,   589,   574,  1105,
       0,     0,     0,     0,     0,     0,   581,     0,   575,     0,
       0,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,     0,     0,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,     0,     0,     0,     0,     0,
     576,     0,     0,     0,   573,     0,     0,     0,     0,   577,
     578,     0,     0,     0,     0,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   574,     0,     0,     0,     0,
     579,     0,     0,     0,     0,   575,     0,   573,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
     573,   582,   583,   584,   585,   586,   587,   588,     0,     0,
     589,     0,  1122,     0,     0,     0,     0,   576,     0,     0,
       0,     0,     0,     0,   574,     0,   577,   578,     0,     0,
     580,   573,     0,     0,   575,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   579,     0,     0,
       0,     0,     0,     0,     0,   573,     0,  1634,     0,     0,
       0,     0,     0,     0,     0,     0,   576,   575,     0,     0,
     574,     0,   581,     0,     0,   577,   578,     0,     0,     0,
     575,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   579,   580,     0,   576,
       0,   574,     0,     0,     0,     0,     0,     0,   577,   578,
       0,   575,   576,     0,     0,     0,     0,     0,     0,     0,
       0,   577,   578,     0,     0,   574,     0,     0,     0,   579,
       0,     0,     0,     0,     0,   575,     0,     0,     0,   581,
       0,     0,   579,   576,     0,     0,   580,     0,     0,     0,
       0,     0,   577,   578,     0,     0,     0,   582,   583,   584,
     585,   586,   587,   588,     0,     0,   589,   576,  1125,     0,
       0,     0,     0,   579,     0,     0,   577,   578,     0,   580,
       0,     0,     0,     0,     0,     0,     0,     0,   581,     0,
       0,     0,   580,     0,     0,     0,     0,   579,     0,     0,
       0,     0,     0,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,     0,     0,     0,     0,     0,     0,     0,
       0,   581,     0,   580,   582,   583,   584,   585,   586,   587,
     588,     0,     0,   589,   581,  1626,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,   580,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,     0,     0,
       0,     0,     0,     0,     0,   581,     0,     0,     0,   573,
       0,     0,     0,   582,   583,   584,   585,   586,   587,   588,
       0,     0,   589,     0,  1633,     0,     0,     0,     0,   581,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   573,     0,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,   573,   589,     0,  1635,     0,   582,
     583,   584,   585,   586,   587,   588,     0,     0,   589,     0,
    1636,     0,     0,     0,     0,     0,     0,     0,     0,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   575,
     582,   583,   584,   585,   586,   587,   588,     0,     0,   589,
       0,  1639,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,   574,     0,   582,   583,   584,   585,   586,   587,
     588,   576,   575,   589,   574,  1648,     0,     0,     0,     0,
     577,   578,     0,     0,   575,     0,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,     0,     0,     0,
       0,   579,     0,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   578,     0,   576,     0,   573,     0,
       0,     0,     0,     0,     0,   577,   578,     0,     0,     0,
       0,     0,     0,     0,   579,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   579,     0,     0,     0,
       0,   580,   573,     0,     0,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   580,     0,     0,     0,   574,     0,
       0,     0,     0,   581,     0,     0,   580,     0,   575,     0,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   573,   574,     0,     0,     0,   581,     0,     0,     0,
     576,     0,   575,     0,     0,     0,     0,     0,   581,   577,
     578,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     579,     0,     0,     0,   576,     0,   573,     0,     0,     0,
       0,     0,     0,   577,   578,     0,     0,     0,   582,   583,
     584,   585,   586,   587,   588,     0,     0,   589,     0,  1649,
       0,   574,     0,     0,   579,     0,     0,     0,     0,     0,
       0,   575,     0,     0,     0,     0,     0,     0,     0,     0,
     580,   582,   583,   584,   585,   586,   587,   588,     0,     0,
     589,     0,  1652,   582,   583,   584,   585,   586,   587,   588,
       0,     0,   589,   576,  1654,     0,   574,     0,     0,     0,
       0,     0,   577,   578,   580,     0,   575,     0,     0,     0,
       0,     0,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   579,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,   581,   577,   578,     0,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
       0,     0,     0,     0,     0,     0,     0,     0,   579,     0,
       0,     0,     0,   580,     0,     0,     0,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   582,   583,   584,
     585,   586,   587,   588,     0,     0,   589,     0,  1657,     0,
       0,     0,     0,     0,     0,   581,   573,     0,   580,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   582,   583,   584,   585,   586,   587,   588,     0,     0,
     589,     0,  1661,   573,     0,     0,     0,     0,     0,     0,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,     0,   574,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   575,     0,     0,     0,
     582,   583,   584,   585,   586,   587,   588,     0,     0,   589,
       0,  1668,     0,   574,     0,     0,   573,     0,     0,     0,
       0,     0,     0,   575,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,     0,     0,   577,   578,     0,
     573,     0,     0,     0,     0,   582,   583,   584,   585,   586,
     587,   588,     0,     0,   589,   576,  1677,     0,   579,     0,
       0,     0,     0,     0,   577,   578,     0,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,     0,     0,     0,
       0,     0,     0,     0,     0,   579,   574,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   575,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   580,     0,
     574,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     575,     0,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,   573,     0,   580,     0,   577,   578,     0,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     581,     0,   576,     0,     0,     0,     0,     0,   579,     0,
       0,   577,   578,     0,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,     0,     0,   581,     0,     0,
       0,     0,   579,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   574,     0,     0,   573,     0,   580,     0,
       0,     0,     0,   575,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     573,     0,   580,     0,     0,   582,   583,   584,   585,   586,
     587,   588,     0,     0,   589,   576,  1678,     0,     0,     0,
     581,     0,     0,     0,   577,   578,     0,     0,     0,     0,
       0,     0,   582,   583,   584,   585,   586,   587,   588,     0,
       0,   589,     0,  1687,   581,   579,   574,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   575,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,     0,     0,     0,
     574,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     575,     0,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,   580,     0,   577,   578,     0,
       0,     0,     0,     0,     0,   582,   583,   584,   585,   586,
     587,   588,   576,     0,   589,     0,  1689,     0,   579,     0,
       0,   577,   578,   573,     0,     0,     0,     0,     0,   582,
     583,   584,   585,   586,   587,   588,     0,   581,   589,     0,
    1690,     0,   579,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   580,     0,
       0,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   580,   574,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   575,     0,     0,     0,     0,     0,     0,
     581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   582,   583,   584,   585,   586,   587,   588,     0,
       0,   589,     0,  1691,   581,   576,     0,   573,     0,     0,
       0,     0,     0,     0,   577,   578,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   579,     0,     0,     0,     0,
       0,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,     0,     0,     0,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   582,   583,   584,   585,   586,
     587,   588,     0,     0,   589,     0,  1883,   574,     0,     0,
       0,     0,   573,     0,     0,   580,     0,   575,     0,   582,
     583,   584,   585,   586,   587,   588,     0,     0,   589,     0,
    1884,     0,     0,     0,     0,     0,     0,   573,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   576,
       0,   573,     0,     0,     0,     0,     0,   581,   577,   578,
       0,     0,     0,     0,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,     0,     0,     0,     0,   579,
       0,     0,   574,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   575,     0,     0,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   573,     0,   574,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,     0,     0,
       0,   574,     0,     0,   576,     0,     0,     0,     0,   580,
     573,   575,     0,   577,   578,     0,     0,     0,     0,     0,
       0,     0,   582,   583,   584,   585,   586,   587,   588,   576,
       0,   589,     0,  1885,   579,     0,     0,     0,   577,   578,
       0,   573,     0,   576,     0,     0,     0,     0,     0,     0,
       0,   581,   577,   578,     0,   574,     0,     0,     0,   579,
       0,     0,     0,     0,     0,   575,     0,     0,     0,     0,
       0,     0,     0,   579,     0,     0,     0,     0,     0,     0,
     574,     0,     0,     0,   580,     0,     0,     0,     0,     0,
     575,     0,     0,     0,     0,     0,     0,   576,     0,     0,
       0,     0,     0,     0,     0,     0,   577,   578,     0,   580,
       0,   574,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   575,   576,   580,     0,     0,   581,   579,     0,     0,
       0,   577,   578,     0,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,     0,   589,     0,  2025,     0,     0,
       0,   581,   579,   576,     0,     0,     0,     0,     0,     0,
       0,     0,   577,   578,     0,   581,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,   580,     0,     0,
       0,     0,     0,   579,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   580,     0,     0,     0,     0,     0,     0,     0,
       0,   582,   583,   584,   585,   586,   587,   588,     0,   581,
     589,     0,  2032,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   573,   580,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,   581,   589,     0,  2033,     0,     0,
     582,   583,   584,   585,   586,   587,   588,     0,     0,   589,
       0,  2034,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   581,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,     0,     0,     0,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,     0,
       0,     0,   574,     0,   582,   583,   584,   585,   586,   587,
     588,     0,   575,   589,     0,  2036,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   582,
     583,   584,   585,   586,   587,   588,     0,     0,   589,     0,
    2037,     0,   573,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   578,   573,     0,     0,     0,     0,
     582,   583,   584,   585,   586,   587,   588,     0,     0,   589,
       0,  2038,     0,     0,   579,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   574,     0,     0,     0,     0,     0,     0,     0,
       0,  1263,   575,  1264,   580,   574,     0,     0,  1265,     0,
    1266,     0,     0,     0,     0,   575,     0,     0,  1267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   576,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   578,     0,   581,   576,     0,     0,
       0,     0,     0,     0,     0,     0,   577,   578,  1268,  1269,
       0,     0,     0,     0,   579,     0,     0,     0,     0,     0,
       0,  1270,  1271,     0,     0,     0,     0,   579,     0,     0,
       0,  1272,     0,     0,  1273,     0,     0,     0,  1274,     0,
       0,  1275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   580,     0,     0,     0,     0,  1276,
       0,     0,     0,     0,     0,     0,     0,   580,     0,     0,
       0,   582,   583,   584,   585,   586,   587,   588,     0,     0,
     589,     0,  2039,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1277,     0,   581,     0,     0,     0,
       0,  1278,     0,     0,     0,     0,     0,     0,     0,   581,
       0,     0,     0,     0,     0,     0,     0,     0,  1279,     0,
       0,     0,  1280,     0,     0,     0,     0,     0,     0,     0,
       0,  1281,  1282,  1283,     0,     0,     0,     0,     0,  1284,
       0,     0,     0,     0,     0,     0,     0,  1285,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1286,     0,     0,     0,
       0,     0,     0,     0,     0,  1287,     0,     0,     0,     0,
       0,   582,   583,   584,   585,   586,   587,   588,     0,     0,
     589,  1288,  2264,     0,   582,   583,   584,   585,   586,   587,
     588,     0,     0,   589,     0,  2367,    85,    86,    87,    88,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,    91,    92,    93,
       0,    94,    95,    96,     0,     0,     0,    97,     0,    98,
       0,    99,   100,   101,     0,   102,     0,     0,   103,     0,
     104,     0,     0,   105,     0,     0,   106,   107,   108,   109,
     110,   111,     0,  1289,   112,   113,   114,     0,   115,     0,
     116,   117,     0,  1290,   118,   119,  1291,     0,     0,     0,
     120,   121,   122,   123,     0,   124,   125,   126,     0,     0,
     127,     0,     0,   128,   129,     0,   130,     0,     0,   131,
       0,     0,     0,   132,     0,     0,   133,     0,     0,   134,
     135,     0,   136,   137,     0,     0,   138,   139,     0,   140,
     141,   142,   143,     0,     0,     0,     0,   144,     0,   145,
       0,   146,     0,     0,     0,   147,     0,     0,   148,   149,
       0,     0,   150,     0,     0,   151,     0,     0,   152,   153,
       0,     0,     0,     0,     0,   154,     0,     0,     0,   155,
       0,   156,     0,     0,     0,   157,   158,   159,   160,   161,
     162,   163,     0,   164,   165,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   176,   177,     0,
     178,     0,     0,   179,     0,   180,     0,     0,     0,     0,
       0,     0,     0,   181,   182,     0,     0,     0,   183,   184,
     185,   186,   187,   188,   189,   190,     0,     0,     0,     0,
     191,     0,   192,     0,   193,   194,     0,     0,   195,   196,
     197,     0,   198,   199,   200,     0,     0,   201,     0,   202,
       0,     0,   203,     0,     0,     0,     0,   204,   205,     0,
       0,     0,     0,     0,     0,   206,   207,     0,     0,     0,
     208,     0,     0,     0,   209,     0,     0,     0,   210,     0,
     211,     0,     0,     0,   212,     0,   213,   214,     0,     0,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,     0,   218,     0,   219,   220,   221,   222,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,   226,     0,   227,     0,     0,     0,     0,
       0,     0,     0,   228,   229,   230,   231,     0,   232,     0,
       0,     0,     0,     0,     0,   233,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   235,     0,   236,     0,     0,   237,     0,     0,   238,
       0,   239,     0,   240,   241,   242,     0,     0,     0,     0,
    1318,    85,    86,    87,    88,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,    91,    92,    93,     0,    94,    95,    96,     0,
       0,     0,    97,     0,    98,     0,    99,   100,   101,     0,
     102,     0,     0,   103,     0,   104,     0,     0,   105,     0,
       0,   106,   107,   108,   109,   110,   111,     0,     0,   112,
     113,   114,     0,   115,     0,   116,   117,     0,     0,   118,
     119,     0,     0,     0,     0,   120,   121,   122,   123,     0,
     124,   125,   126,     0,     0,   127,     0,     0,   128,   129,
       0,   130,     0,     0,   131,     0,     0,     0,   132,     0,
       0,   133,     0,     0,   134,   135,     0,   136,   137,     0,
       0,   138,   139,     0,   140,   141,   142,   143,     0,     0,
       0,     0,   144,     0,   145,     0,   146,     0,     0,     0,
     147,     0,     0,   148,   149,     0,     0,   150,     0,     0,
     151,     0,     0,   152,   153,     0,     0,     0,     0,     0,
     154,     0,     0,     0,   155,     0,   156,     0,     0,     0,
     157,   158,   159,   160,   161,   162,   163,     0,   164,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,     0,   176,   177,     0,   178,     0,     0,   179,     0,
     180,     0,     0,     0,     0,     0,     0,     0,   181,   182,
       0,     0,     0,   183,   184,   185,   186,   187,   188,   189,
     190,     0,     0,     0,     0,   191,     0,   192,     0,   193,
     194,     0,     0,   195,   196,   197,     0,   198,   199,   200,
       0,     0,   201,     0,   202,     0,     0,   203,     0,     0,
       0,     0,   204,   205,     0,     0,     0,     0,     0,     0,
     206,   207,     0,     0,     0,   208,     0,     0,     0,   209,
       0,     0,     0,   210,     0,   211,     0,     0,     0,   212,
       0,   213,   214,     0,     0,   215,   216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   217,     0,   218,
       0,   219,   220,   221,   222,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   225,   226,     0,
     227,     0,     0,     0,     0,     0,     0,     0,   228,   229,
     230,   231,     0,   232,     0,     0,     0,     0,     0,     0,
     233,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   235,     0,   236,     0,
       0,   237,     0,     0,   238,     0,   239,     0,   240,   241,
     242,     0,     0,     0,     0,  1719,    85,    86,    87,    88,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,    91,    92,    93,
       0,    94,    95,    96,     0,     0,     0,    97,     0,    98,
       0,    99,   100,   101,     0,   102,     0,     0,   103,     0,
     104,     0,     0,   105,     0,     0,   106,   107,   108,   109,
     110,   111,     0,     0,   112,   113,   114,     0,   115,     0,
     116,   117,     0,     0,   118,   119,     0,     0,     0,     0,
     120,   121,   122,   123,     0,   124,   125,   126,     0,     0,
     127,     0,     0,   128,   129,     0,   130,     0,     0,   131,
       0,     0,     0,   132,     0,     0,   133,     0,     0,   134,
     135,     0,   136,   137,     0,     0,   138,   139,     0,   140,
     141,   142,   143,     0,     0,     0,     0,   144,     0,   145,
       0,   146,     0,     0,     0,   147,     0,     0,   148,   149,
       0,     0,   150,     0,     0,   151,     0,     0,   152,   153,
       0,     0,     0,     0,     0,   154,     0,     0,     0,   155,
       0,   156,     0,     0,     0,   157,   158,   159,   160,   161,
     162,   163,     0,   164,   165,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   176,   177,     0,
     178,     0,     0,   179,     0,   180,     0,     0,     0,     0,
       0,     0,     0,   181,   182,     0,     0,     0,   183,   184,
     185,   186,   187,   188,   189,   190,     0,     0,     0,     0,
     191,     0,   192,     0,   193,   194,     0,     0,   195,   196,
     197,     0,   198,   199,   200,     0,     0,   201,     0,   202,
       0,     0,   203,     0,     0,     0,     0,   204,   205,     0,
       0,     0,     0,     0,     0,   206,   207,     0,     0,     0,
     208,     0,     0,     0,   209,     0,     0,     0,   210,     0,
     211,     0,     0,     0,   212,     0,   213,   214,     0,     0,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   217,     0,   218,     0,   219,   220,   221,   222,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,   226,     0,   227,     0,     0,     0,     0,
       0,     0,     0,   228,   229,   230,   231,     0,   232,     0,
       0,     0,     0,     0,     0,   233,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   235,     0,   236,     0,     0,   237,     0,     0,   238,
       0,   239,     0,   240,   241,   242,     0,     0,     0,     0,
    1745,    85,    86,    87,    88,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,    91,    92,    93,     0,    94,    95,    96,     0,
       0,     0,    97,     0,    98,     0,    99,   100,   101,     0,
     102,     0,     0,   103,     0,   104,     0,     0,   105,     0,
       0,   106,   107,   108,   109,   110,   111,     0,     0,   112,
     113,   114,     0,   115,     0,   116,   117,     0,     0,   118,
     119,     0,     0,     0,     0,   120,   121,   122,   123,     0,
     124,   125,   126,     0,     0,   127,     0,     0,   128,   129,
       0,   130,     0,     0,   131,     0,     0,     0,   132,     0,
       0,   133,     0,     0,   134,   135,     0,   136,   137,     0,
       0,   138,   139,     0,   140,   141,   142,   143,     0,     0,
       0,     0,   144,     0,   145,     0,   146,     0,     0,     0,
     147,     0,     0,   148,   149,     0,     0,   150,     0,     0,
     151,     0,     0,   152,   153,     0,     0,     0,     0,     0,
     154,     0,     0,     0,   155,     0,   156,     0,     0,     0,
     157,   158,   159,   160,   161,   162,   163,     0,   164,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,     0,   176,   177,     0,   178,     0,     0,   179,     0,
     180,     0,     0,     0,     0,     0,     0,     0,   181,   182,
       0,     0,     0,   183,   184,   185,   186,   187,   188,   189,
     190,     0,     0,     0,     0,   191,     0,   192,     0,   193,
     194,     0,     0,   195,   196,   197,     0,   198,   199,   200,
       0,     0,   201,     0,   202,     0,     0,   203,     0,     0,
       0,     0,   204,   205,     0,     0,     0,     0,     0,     0,
     206,   207,     0,     0,     0,   208,     0,     0,     0,   209,
       0,     0,     0,   210,     0,   211,     0,     0,     0,   212,
       0,   213,   214,     0,     0,   215,   216,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   217,     0,   218,
       0,   219,   220,   221,   222,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   223,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,     0,     0,     0,     0,     0,
     224,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,     0,     0,     0,     0,     0,     0,   225,   226,     0,
     227,     0,     0,   573,     0,     0,     0,     0,   228,   229,
     230,   231,     0,   232,     0,     0,     0,     0,     0,     0,
     233,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   573,  1017,     0,     0,     0,   235,     0,   236,     0,
       0,   237,     0,     0,   238,     0,   239,   573,   240,   241,
     242,     0,     0,     0,     0,  2052,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   574,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   575,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,     0,     0,     0,     0,     0,     0,
       0,   574,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   575,     0,     0,     0,   576,     0,   574,     0,     0,
       0,     0,     0,     0,   577,   578,     0,   575,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,     0,     0,
       0,     0,     0,   576,     0,   579,     0,     0,     0,     0,
     573,     0,   577,   578,     0,     0,     0,     0,     0,   576,
       0,     0,     0,     0,     0,     0,     0,     0,   577,   578,
       0,     0,     0,   579,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   579,
       0,     0,     0,     0,   573,   580,     0,     0,     0,     0,
       0,     0,     0,  1073,  1074,  1075,  1076,  1077,  1078,  1079,
    1080,  1081,  1082,     0,     0,     0,     0,     0,     0,     0,
     574,     0,     0,   580,     0,     0,     0,     0,     0,     0,
     575,     0,     0,     0,     0,     0,     0,   581,     0,   580,
       0,     0,     0,     0,     0,     0,     0,     0,   770,   771,
     772,   773,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   576,     0,   574,   581,   774,   775,   776,  1083,
       0,   577,   578,     0,   575,     0,     0,     0,   777,   778,
       0,   581,   779,     0,     0,     0,     0,     0,     0,     0,
       0,   780,   579,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   576,     0,   781,   782,
       0,     0,     0,     0,     0,   577,   578,     0,     0,     0,
       0,     0,   582,   583,   584,   585,   586,   587,   588,     0,
       0,   589,     0,     0,     0,     0,   579,     0,     0,  1084,
       0,     0,   580,     0,     0,     0,     0,     0,     0,  1085,
     582,   583,   584,   585,   586,   587,   588,  1642,     0,   589,
       0,     0,     0,     0,     0,     0,   582,   583,   584,   585,
     586,   587,   588,     0,     0,   589,     0,     0,     0,     0,
       0,  1086,     0,     0,   581,     0,   580,     0,     0,     0,
    1087,  1088,  1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,
    1152,  1153,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1089,     0,     0,     0,     0,     0,  1144,  1145,  1146,
    1147,  1148,  1149,  1150,  1151,  1152,  1153,     0,   581,  1144,
    1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,     0,
    1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,
       0,     0,     0,     0,     0,     0,     0,     0,  1154,     0,
    2026,  1090,     0,     0,     0,     0,     0,     0,     0,   582,
     583,   584,   585,   586,   587,   588,     0,     0,   589,     0,
       0,     0,     0,  1425,     0,  1144,  1145,  1146,  1147,  1148,
    1149,  1150,  1151,  1152,  1153,  1673,     0,     0,     0,     0,
       0,     0,     0,  1091,     0,     0,  1701,     0,     0,     0,
       0,     0,     0,   582,   583,   584,   585,   586,   587,   588,
       0,     0,   589,     0,     0,     0,     0,     0,  1155,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1156,     0,
    1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,
       0,  1875,     0,  1155,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1156,     0,  1155,     0,     0,     0,     0,
    1157,     0,     0,     0,     0,  1156,  1155,     0,     0,  1158,
    1159,     0,     0,     0,     0,     0,  1156,     0,  1092,  1093,
    1094,  1095,  1096,  1097,  1098,  1157,     0,  1099,     0,     0,
    1160,     0,     0,     0,  1158,  1159,  1890,  1157,     0,     0,
       0,     0,     0,     0,     0,     0,  1158,  1159,  1157,     0,
       0,  1155,     0,     0,     0,  1160,     0,  1158,  1159,     0,
       0,  1156,     0,     0,     0,     0,     0,  1160,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1160,     0,
    1161,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1157,     0,     0,     0,     0,     0,     0,
       0,     0,  1158,  1159,     0,  1161,  1155,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1156,  1161,     0,     0,
       0,     0,  1162,  1160,     0,     0,     0,     0,  1161,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1162,  1157,     0,
       0,     0,     0,     0,     0,     0,     0,  1158,  1159,  1162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1162,     0,     0,  1161,     0,     0,     0,     0,  1160,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1163,  1164,  1165,
    1166,  1167,  1168,  1169,     0,  1162,  1170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1161,     0,
       0,     0,  1163,  1164,  1165,  1166,  1167,  1168,  1169,     0,
       0,  1170,     0,     0,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,     0,     0,  1170,     0,  1163,  1164,  1165,  1166,  1167,
    1168,  1169,     0,     0,  1170,     0,     0,     0,     0,     0,
    1162,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1163,  1164,  1165,  1166,  1167,  1168,  1169,     0,     0,  1170,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,  1163,  1164,  1165,  1166,  1167,
    1168,  1169,    90,     0,  1170,    91,    92,    93,     0,    94,
      95,    96,     0,     0,     0,    97,     0,    98,     0,    99,
     100,   101,     0,   102,     0,     0,   103,     0,   104,     0,
       0,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,     0,   124,   125,   126,     0,     0,   127,     0,
       0,   128,   129,     0,   130,     0,     0,   131,     0,     0,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,     0,     0,     0,     0,   144,     0,   145,     0,   146,
       0,     0,     0,   147,   954,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,     0,   154,     0,     0,     0,   155,     0,   156,
       0,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,     0,   178,     0,
       0,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,     0,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,   955,     0,     0,   204,   205,     0,     0,     0,
       0,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,     0,   212,     0,   213,   214,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   221,   222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,   229,   230,   231,     0,   232,     0,     0,     0,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,   236,     0,     0,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,    91,    92,    93,     0,    94,
      95,    96,     0,     0,     0,    97,     0,    98,     0,    99,
     100,   101,     0,   102,     0,     0,   103,     0,   104,     0,
       0,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,     0,   124,   125,   126,     0,     0,   127,     0,
       0,   128,   129,     0,   130,     0,     0,   131,     0,     0,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,     0,     0,     0,     0,   144,     0,   145,     0,   146,
       0,     0,     0,   147,     0,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,     0,   154,   557,     0,     0,   155,     0,   156,
       0,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,     0,   178,     0,
       0,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,     0,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,   558,     0,     0,   204,   205,     0,     0,     0,
       0,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,     0,   212,     0,   213,   214,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   221,   222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,   229,   230,   231,     0,   232,     0,     0,     0,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,   236,     0,     0,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,    91,    92,    93,     0,    94,
      95,    96,     0,     0,     0,    97,     0,    98,     0,    99,
     100,   101,     0,   102,     0,     0,   103,     0,   104,     0,
       0,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   118,   119,     0,     0,  1715,     0,   120,   121,
     122,   123,     0,   124,   125,   126,     0,     0,   127,     0,
       0,   128,   129,     0,   130,     0,     0,   131,     0,     0,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,     0,     0,     0,     0,   144,     0,   145,     0,   146,
       0,     0,     0,   147,     0,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,     0,   154,     0,     0,     0,   155,     0,   156,
       0,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,     0,   178,     0,
       0,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,     0,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,  1716,     0,     0,   204,   205,     0,     0,     0,
       0,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,     0,   212,     0,   213,   214,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   221,   222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,   229,   230,   231,     0,   232,     0,     0,     0,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,   236,     0,     0,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,    91,    92,    93,     0,    94,
      95,    96,     0,     0,     0,    97,     0,    98,     0,    99,
     100,   101,     0,   102,     0,     0,   103,     0,   104,     0,
       0,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,     0,   124,   125,   126,     0,     0,   127,     0,
       0,   128,   129,     0,   130,     0,     0,   131,     0,     0,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,     0,     0,     0,   708,   144,     0,   145,     0,   146,
       0,     0,     0,   147,     0,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,     0,   154,     0,     0,     0,   155,     0,   156,
       0,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,     0,   178,     0,
       0,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,     0,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,     0,     0,     0,   204,   205,     0,     0,     0,
       0,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,     0,   212,     0,   213,   214,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   221,   222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,   229,   230,   231,     0,   232,     0,     0,     0,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,   236,     0,     0,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,    91,    92,    93,     0,    94,
      95,    96,     0,     0,     0,    97,     0,    98,     0,    99,
     100,   101,     0,   102,     0,     0,   103,     0,   104,     0,
       0,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,     0,   124,   125,   126,     0,     0,   127,     0,
       0,   128,   129,     0,   130,     0,     0,   131,     0,     0,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,     0,     0,     0,     0,   144,     0,   145,     0,   146,
       0,     0,     0,   147,     0,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,     0,   154,     0,     0,     0,   155,     0,   156,
       0,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,     0,   178,     0,
       0,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,     0,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,  1901,     0,     0,   204,   205,     0,     0,     0,
       0,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,     0,   212,     0,   213,   214,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   221,   222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,   229,   230,   231,     0,   232,     0,     0,     0,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,   236,     0,     0,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,    91,    92,    93,     0,    94,
      95,    96,     0,     0,     0,    97,     0,    98,     0,    99,
     100,   101,     0,   102,     0,     0,   103,     0,   104,     0,
       0,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,     0,   124,   125,   126,     0,     0,   127,     0,
       0,   128,   129,     0,   130,     0,     0,   131,     0,     0,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,     0,     0,     0,     0,   144,     0,   145,     0,   146,
       0,     0,     0,   147,     0,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,     0,   154,     0,     0,     0,   155,     0,   156,
       0,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,     0,   178,     0,
       0,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,  2350,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,     0,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,     0,     0,     0,   204,   205,     0,     0,     0,
       0,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,     0,   212,     0,   213,   214,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   221,   222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,   229,   230,   231,     0,   232,     0,     0,     0,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,   236,     0,     0,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,    85,    86,    87,    88,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,    91,    92,    93,     0,    94,
      95,    96,     0,     0,     0,    97,     0,    98,     0,    99,
     100,   101,     0,   102,     0,     0,   103,     0,   104,     0,
       0,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,     0,   124,   125,   126,     0,     0,   127,     0,
       0,   128,   129,     0,   130,     0,     0,   131,     0,     0,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   142,
     143,     0,     0,     0,     0,   144,     0,   145,     0,   146,
       0,     0,     0,   147,     0,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,     0,   154,     0,     0,     0,   155,     0,   156,
       0,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,     0,   178,     0,
       0,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,     0,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,     0,     0,     0,   204,   205,     0,     0,     0,
       0,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,     0,   211,     0,
       0,     0,   212,     0,   213,   214,     0,     0,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,   218,     0,   219,   220,   221,   222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,   229,   230,   231,     0,   232,     0,     0,     0,
       0,     0,     0,   233,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,   236,     0,     0,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,    85,    86,    87,   961,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,    91,    92,    93,     0,    94,
      95,    96,     0,     0,     0,    97,     0,    98,     0,    99,
     100,   101,     0,   102,     0,     0,   103,     0,   104,     0,
       0,   105,     0,     0,   106,   107,   108,   109,   110,   111,
       0,     0,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   118,   119,     0,     0,     0,     0,   120,   121,
     122,   123,     0,   124,   125,   126,     0,     0,   127,     0,
       0,   128,   129,     0,   130,     0,     0,   131,     0,     0,
       0,   132,     0,     0,   133,     0,     0,   134,   135,     0,
     136,   137,     0,     0,   138,   139,     0,   140,   141,   962,
     143,     0,     0,     0,     0,   144,     0,   145,     0,   146,
       0,     0,     0,   147,     0,     0,   148,   149,     0,     0,
     150,     0,     0,   151,     0,     0,   152,   153,     0,     0,
       0,     0,     0,   154,     0,     0,     0,   155,     0,   156,
       0,     0,     0,   157,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   176,   177,     0,   178,     0,
       0,   179,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   183,   184,   185,   186,
     187,   188,   189,   190,     0,     0,     0,     0,   191,     0,
     192,     0,   193,   194,     0,     0,   195,   196,   197,     0,
     198,   199,   200,     0,     0,   201,     0,   202,     0,     0,
     203,     0,     0,     0,     0,   204,   205,     0,     0,     0,
       0,     0,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,   209,     0,     0,     0,   210,   659,   211,     0,
       0,     0,   212,     0,   213,   214,     0,   660,   215,   216,
     661,     0,   662,     0,   663,     0,   664,     0,     0,     0,
     217,   665,   218,     0,   219,   220,   221,   222,     0,   666,
       0,     0,     0,     0,   667,   668,     0,   223,     0,     0,
       0,     0,   669,     0,     0,   670,     0,     0,   671,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,   226,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   228,   229,   230,   231,     0,   232,     0,     0,     0,
       0,     0,   672,   233,   234,     0,     0,     0,     0,     0,
       0,     0,   673,     0,     0,     0,     0,     0,     0,   235,
       0,   236,     0,   674,   237,     0,     0,   238,     0,   239,
       0,   240,   241,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     675,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     676,     0,   677,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   678,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   679
};

static const short yycheck[] =
{
       2,   607,     2,   714,    14,   303,   304,   305,   463,   691,
     465,   466,   467,   468,   598,   369,   600,  1262,  1197,  1518,
      22,  1493,  1305,  1306,  1267,  1308,  1190,  1293,   883,   638,
     966,   908,  1529,   397,  1484,  1547,   491,   492,    40,  1314,
    1618,  1242,   911,  1775,  1310,  1246,   519,     3,   917,    60,
      34,   861,   530,  1217,     3,  1141,   479,    60,  1752,    34,
      82,    34,    34,    60,    60,  1319,    68,   124,    68,   124,
       9,    85,    34,   657,   658,   117,   485,   486,  1541,     3,
      19,    89,  1581,     9,  1547,     3,   495,  1550,  1551,  2018,
     193,   138,  1555,  1556,    30,  2092,   173,   575,   117,  1501,
     684,   191,   187,   581,    47,  2044,   461,  1557,    82,     4,
      51,  1608,   124,   148,   243,   192,     9,  1567,  1568,    38,
    1583,     6,    62,    93,   221,   187,  2100,   134,  1769,   127,
     145,   264,   144,   162,   718,    22,   178,     3,   722,  1954,
     155,   263,    78,   125,    93,   136,   242,    96,    34,    98,
      76,    75,  1778,  1317,     3,   159,  2099,  2154,   142,   157,
       9,   155,  2101,     3,   148,   271,   102,   142,   886,   142,
     142,   314,   242,   148,   123,   148,   148,   126,   242,    36,
     142,   103,   138,    76,   138,   214,   148,    48,    94,     9,
     139,  1474,   243,  2136,   145,   102,  1471,   340,   304,    19,
     184,   150,   217,   110,   155,   267,   117,   925,   192,   184,
      74,   184,   184,   120,    71,   162,   224,   515,   229,   192,
      82,   203,   184,   217,   906,   101,    64,    76,    66,    62,
     272,   235,   119,   231,   119,   242,   265,   279,   239,   258,
     162,   243,  2171,    48,   235,   109,   216,   545,   290,   271,
     261,   187,   236,   189,   229,   204,   142,    62,   261,   237,
     356,   236,   148,   236,   236,   267,   217,   269,   204,   162,
     282,   290,   282,   406,   236,  1558,   260,   406,  1561,  1791,
    1563,  1091,   292,   305,   406,    17,    18,  1570,  1571,  1572,
    1573,   263,   233,  2222,   309,  2110,   237,   271,   184,  1542,
     310,   656,  1388,   188,  1945,   245,   915,   256,   411,   406,
     229,   346,   921,   360,  1940,   309,   406,   243,   403,   214,
     256,   214,   332,  1524,   688,   266,   361,   404,  1791,  1731,
    1732,  2305,  2271,  2262,  1912,  1499,   406,   255,  1502,  1503,
     264,   403,   406,   290,  1213,  1620,   194,   258,   254,   406,
     236,   406,  1162,  1439,   364,   365,   366,   367,   309,   723,
      60,   268,   346,   239,   406,  1619,   368,   369,   370,  2366,
     139,   346,   265,   346,   346,    17,    18,   361,   380,   290,
     380,   859,   404,   861,   346,   124,   361,   406,   361,   361,
     404,  2085,   335,   404,   406,   397,   335,   263,   406,   361,
    2215,   404,    89,   208,   360,  2334,   145,   404,   404,   271,
      17,    18,   274,   275,   392,   393,   282,   103,   420,   421,
     422,   421,   406,   263,     0,   427,     3,   242,   430,   431,
     292,   406,   434,   406,   406,   297,    34,    29,   193,  1951,
     302,   443,   282,   405,   406,  1942,    35,   856,    95,   135,
      74,   392,   393,  1617,   464,   187,   102,   406,  1733,   469,
     346,   471,   472,   473,   474,   475,    89,   239,  2046,    58,
      62,   240,   482,   483,   484,   361,   162,  1722,   260,    93,
     490,   260,   173,   234,   253,   109,   496,   841,  1951,   499,
     241,   379,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   192,   512,   513,   514,   243,   516,   517,   518,   123,
     520,   521,   522,   523,   524,   525,   526,   527,  2015,   529,
     406,   531,   532,   533,   534,   535,   536,   537,   538,  2261,
     102,  1776,   342,  1734,  2396,   267,   546,   224,   548,   549,
     550,   356,  1785,   189,   142,   187,   556,  2409,   317,   121,
     148,   239,  1753,   563,   564,   565,   566,   567,   568,   569,
     207,   571,   572,   573,    77,   188,  1730,   577,   578,   579,
     580,  2021,   582,   583,   584,   585,   586,   587,   588,   589,
     187,  1391,    73,  1858,  1670,   949,   184,    97,   160,   591,
     204,   240,   105,   122,   192,   597,   598,   597,   600,  1008,
    1209,   224,  1025,  1212,   606,   187,   229,  1085,   972,   138,
    1019,  1020,  1698,  1091,   124,  1898,  1899,   187,   190,    43,
     214,   127,  1491,   139,   406,   267,   220,   406,   253,   411,
    1987,  1040,   411,  1443,   150,   126,   230,  1994,   236,    63,
    1049,   404,   256,   215,  1521,   408,  1055,   139,   139,  1504,
    1586,   157,   162,  1062,   404,   657,   658,   342,  1067,  2156,
     267,    74,   394,   395,   396,   397,   398,   399,   400,   404,
    2005,   403,   239,   121,  2009,  1918,  1919,  1920,  1156,   238,
     239,   683,   684,   683,  1162,   267,   688,    48,   404,   691,
     692,   200,   201,   695,   281,   697,   109,   267,  1171,  1172,
     121,    62,   404,   705,   291,   705,   708,   294,   708,   117,
     404,  2183,   160,   715,   404,   715,   718,   112,   113,   242,
     722,   723,   135,   404,   128,   231,   187,   404,   189,   301,
     259,  1140,   404,  1934,   183,   405,   406,   186,  1939,   160,
     750,   187,   190,   189,   405,   406,   107,  2013,   346,   240,
    1914,  1915,   404,  2252,   396,   397,   398,   399,   400,  2324,
    2325,   403,   253,   361,   187,   256,   189,   215,   404,   190,
     178,   175,   176,   177,   404,   136,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    81,   404,   395,   396,
     397,   398,   399,   400,   215,   292,   403,    96,   159,   404,
     195,   404,   812,   813,   814,   302,   121,   404,   406,   306,
    1264,  1265,   242,  1267,   396,   397,   398,   399,   400,   405,
     406,   403,   271,   236,   123,   404,   317,   411,   398,   399,
     400,   404,   842,   403,   392,   393,  1290,   395,   133,   841,
     139,   843,    75,   404,   205,   160,   241,   208,   143,   404,
     860,   150,   147,   301,   303,   304,  2057,   405,   406,   404,
     870,   222,   872,   931,   272,   933,   405,   406,   936,  2129,
    2130,   279,   404,  2133,   235,   190,   404,   238,   239,   404,
     301,    48,   290,   404,   179,   180,   404,   182,   898,  1243,
    2150,  2055,  2056,  2153,   209,   210,   898,   405,   406,   901,
     215,   901,   405,   406,   404,   204,   908,  2086,    59,   405,
     406,  1389,   145,  1391,   916,   404,   918,     4,   918,  2202,
    2203,  2204,   155,   404,  2374,   405,   406,   404,  2173,    16,
      17,   404,    19,   405,   406,   937,   404,   937,   405,   406,
     404,   943,    29,   943,    31,    32,   948,   949,   404,    36,
      37,   405,   406,   404,   187,   540,   541,   542,   543,   544,
     405,   406,   404,  1441,   404,  1443,   405,   406,   405,   406,
     972,   405,   406,   386,   387,   388,   389,   390,   391,   405,
     406,   813,   814,   162,   217,   404,   301,   404,   990,   404,
     990,   411,   994,   404,   996,   405,   406,  1007,  1919,  1920,
     187,  1011,     6,     7,     8,     9,  1016,    11,    12,    13,
      14,   404,  1022,   404,  1024,  2346,  2347,  1027,  1028,   404,
     404,   404,   404,   404,  1034,   404,   404,   404,  1038,  1438,
     404,   404,   404,   404,   267,   404,  1046,   404,  1048,   404,
     404,   404,  1052,  1053,  1054,   404,   404,   404,  1058,   404,
     404,  1061,   404,  1063,   404,  1065,  1066,   404,  1068,   404,
     404,   404,   404,  1073,  1074,  1075,  1076,  1077,  1078,  1079,
     404,  1081,  1082,  1083,  1084,  1498,   309,  1087,  1088,  1089,
    1090,   404,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,
     404,  1101,  1102,   406,  1104,   192,  1106,  1107,  1108,   116,
      88,   264,   406,  1113,    74,   307,  1529,   405,   378,   405,
      19,   405,   405,   405,  1124,   119,  1725,  1127,  1128,   404,
     163,   164,   165,   166,   167,   168,   169,  2410,   171,   172,
     411,    48,  1486,   157,  1144,  1145,  1146,  1147,  1148,  1149,
    1150,   124,  1152,  1153,  1154,   159,    30,    88,  1158,  1159,
    1160,  1161,   406,  1163,  1164,  1165,  1166,  1167,  1168,  1169,
    1170,   394,   395,   396,   397,   398,   399,   400,   398,  1775,
     403,  1173,  1645,  1646,   255,  1177,    39,  1177,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,   239,   155,
     335,   235,    48,   238,    78,    79,   235,   124,   121,   262,
     406,  1674,  1675,   240,   238,   100,   205,    91,    92,   194,
     192,   134,  1214,   406,  1214,   192,  1218,   101,   193,  1221,
     104,  1221,   405,  1225,   116,   192,   406,   411,   214,  1702,
    1703,   405,   405,  1235,  1236,  1235,  1236,   160,   398,   405,
     405,  1243,   404,  1827,    75,   406,   405,   405,   405,   405,
     405,  1261,   405,   405,   124,   139,   405,   397,   405,   405,
     189,   405,   405,   405,   405,   115,   189,   190,   406,   404,
     141,   187,   411,    48,   411,   411,   406,   411,   243,   145,
     335,   238,   404,   406,   243,   406,     9,   263,  1697,     9,
     174,     9,   215,  1295,     9,    27,     9,   181,     9,     9,
       9,    33,     9,   406,   251,   124,    47,   406,   406,   405,
     242,   327,   344,  1315,   145,   344,    48,   242,   202,   242,
     124,   190,   190,   124,   155,   124,   190,   211,   212,   213,
      62,   405,   242,   404,   242,   242,   144,  1347,   124,  1349,
     242,   411,  1765,   227,  1354,  1355,   124,   411,   410,   242,
      62,   411,   275,   243,   242,   124,   187,     3,   146,    96,
     282,     9,   246,     9,   229,   196,   197,     9,   100,     9,
       9,   255,   114,   296,   297,     9,   149,  2088,   301,   149,
    1390,   114,     9,     9,    87,     9,   217,   271,    34,  1399,
       9,  1401,     9,   125,     9,   127,     9,   129,     9,     9,
     229,    47,     9,  1413,  1414,  1415,     9,   139,   140,   406,
     242,   143,   242,   260,   242,  1425,   242,   149,   242,   411,
     281,   281,   384,   378,   405,   157,   405,   159,   406,   189,
      76,   405,  1442,   405,   405,   189,   267,   405,   265,   242,
     404,  1451,   405,  1453,   405,    23,   406,   405,   242,   405,
     145,   405,   405,   185,   405,    33,   157,    71,    36,   343,
      38,   193,    40,   408,    42,   148,   264,   148,   199,    47,
     398,   153,   356,   404,   198,   242,   122,    55,   309,  1481,
    1482,   150,    60,    61,  1486,   131,   132,   150,  1911,   404,
      68,     9,   138,    71,     9,   242,   142,   411,   144,   231,
     404,   252,   148,   235,   404,   190,   238,   239,  1510,  1511,
     190,   398,   364,  1515,  1516,  1525,   405,   411,   165,  1521,
     404,   346,   405,  1946,   341,   131,   148,  1529,   148,   406,
     262,   404,   406,   405,   150,    76,   242,   404,   184,  1541,
     118,   150,   242,   405,   242,  1547,   192,   282,  1550,  1551,
     128,   405,   198,  1555,  1556,   352,   405,   405,  1560,   144,
     406,   139,   406,   394,   395,   396,   397,   398,   399,   400,
      87,   132,   403,    95,     9,  1577,  2031,  1577,   409,   148,
     352,  1583,   148,   229,    87,  1587,  1588,   199,   264,   214,
     236,   242,   406,    60,  2276,  2277,   102,  2279,   102,   406,
     304,   289,   404,   239,   404,   156,  1608,   242,   404,   255,
     405,   404,   404,   259,   260,  1625,   148,   263,   405,   404,
     404,   404,   404,   404,   242,   405,   190,  1637,   206,   150,
     190,  2040,  1642,  1643,   242,   305,   282,   242,   216,   404,
     218,   190,   404,  1653,   190,   190,   189,   229,   150,  1659,
     190,   190,   190,  1663,   232,  2261,  1666,  2339,    87,  2341,
    2342,   190,   405,  1673,  2087,   405,  1676,    87,   112,  1679,
      87,    75,  1682,  1683,   260,   404,   260,   242,  2360,   257,
     194,   406,    87,   405,   405,   358,   405,    87,   405,   335,
      87,  1701,   166,   192,    72,   190,     9,   405,   138,   404,
     346,   190,    75,   405,   216,   156,   405,   170,   405,   405,
     404,   404,   358,  1715,   404,   361,   405,     9,   405,  1309,
     406,  2155,  2013,  1969,  1608,   262,  1295,  1263,  1957,  1967,
    1590,  1975,  1975,  2010,  2238,  2106,   428,  2142,  2353,  2312,
    2112,  1743,  1752,  2227,  2159,  2240,  1794,  1586,  1758,  1185,
     922,   975,  1235,    68,   410,  1757,  1470,  2210,  1764,   405,
     406,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,  1936,  1782,  1018,   619,  1193,  2058,  2409,   444,   463,
     723,  1195,  1325,  2394,  2087,   476,  1941,   708,   409,  1791,
     994,   435,   919,  2269,  1804,   949,   890,   721,  2178,  1704,
    2331,  2283,  1946,   898,  1239,  1523,  1516,  1757,  1932,  1177,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
     938,  2200,   405,   943,  1925,  1827,  1828,  1743,     3,  2205,
       5,  1515,    -1,    -1,    -1,    -1,  2134,    -1,    -1,  1841,
      -1,    -1,  1844,    -1,  1844,    -1,  1848,    -1,    23,    24,
      -1,    26,    -1,    28,    -1,  1857,    31,    -1,    33,    -1,
      -1,    36,    37,    38,    -1,  1875,    41,    42,    -1,    44,
      45,    -1,    47,    -1,    -1,    -1,    75,    52,    53,    54,
    1890,    56,    57,    -1,    59,    60,    61,    62,    -1,    64,
      -1,    66,    -1,    -1,    69,    70,    71,   145,    -1,    -1,
      -1,  1903,    -1,  1913,    -1,    80,  1916,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    93,  2217,
    2218,  2219,    -1,  1925,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,  1946,    -1,   145,    -1,    -1,  1951,
      -1,    -1,  1954,   128,    -1,  1957,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,  1969,    -1,   217,
      -1,  2394,   320,   321,   322,   323,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,   187,   104,
     338,   339,   340,    -1,    -1,    -1,    -1,   196,   197,    -1,
      -1,    -1,   350,   351,    -1,    -1,   354,    -1,    -1,    -1,
      -1,    -1,    -1,  2023,    -1,   363,  2026,    -1,   217,   267,
    2030,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   380,   381,  2044,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,   223,    -1,
     225,    -1,    -1,    -1,   229,    -1,    -1,    -1,    -1,   174,
      -1,   309,    -1,    -1,    -1,    -1,   181,    -1,   267,    -1,
      -1,    -1,    -1,    -1,    -1,  2085,    -1,    -1,    -1,    -1,
      82,    83,    84,    -1,   259,    -1,    -1,   202,    -1,    -1,
      -1,  2101,    -1,    -1,    -1,    -1,   211,   212,   213,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2110,    -1,
     309,    -1,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   246,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     255,    -1,    -1,    -1,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    -1,   403,   271,  2159,    -1,   161,
      -1,    -1,  2172,    -1,    -1,    -1,    -1,  2169,    -1,  2169,
      -1,    -1,    -1,    -1,    -1,    -1,  2186,    -1,  2188,    -1,
    2190,   183,    -1,    -1,   186,   360,    -1,    -1,    -1,    -1,
    2192,    -1,    -1,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,    -1,    -1,   403,    -1,    -1,    -1,    -1,    -1,
     409,    -1,    -1,  2215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   229,   343,   404,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2266,    -1,    -1,    -1,
      -1,  2271,    -1,    -1,    -1,    -1,    -1,   269,   270,   271,
      -1,    -1,   274,   275,   276,   277,   278,    -1,   280,   281,
      -1,   283,   284,   285,   286,   287,   288,    -1,    -1,   291,
      -1,   293,  2294,   295,   296,   297,   298,   299,   300,    -1,
      -1,   303,   304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  2335,    -1,  2345,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2346,  2347,    -1,  2349,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2380,   381,
      22,    -1,    -1,    25,    26,    27,    -1,    29,    30,    31,
      32,  2401,  2394,    35,    -1,    37,    -1,    39,    40,    41,
      42,    43,    -1,    -1,    46,    -1,    48,    49,    50,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    90,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,    -1,   114,   115,    -1,   117,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,    -1,   129,   130,    -1,
      -1,   133,   134,    -1,   136,   137,    -1,    -1,   140,    -1,
      -1,   143,    -1,    -1,   146,   147,    -1,    -1,    -1,   151,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
      -1,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,   218,    -1,   220,    -1,
     222,   223,    -1,    -1,   226,   227,   228,    -1,   230,   231,
     232,    -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,    -1,
      -1,   253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,
     262,    -1,    -1,    -1,   266,    -1,   268,    -1,    -1,   271,
     272,   273,   274,   275,    -1,    -1,   278,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
     292,    -1,   294,   295,   296,   297,    -1,    -1,    -1,   301,
      -1,    -1,    -1,    -1,    -1,   307,   308,    -1,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,    -1,    -1,
      -1,   323,   324,   325,   326,    -1,   328,   329,   330,   331,
     332,   333,   334,    -1,   336,   337,    -1,    -1,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,    -1,   351,
     352,   353,   354,   355,   356,   357,    -1,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,    -1,   379,    -1,   381,
     382,   383,   384,    -1,    -1,   387,    -1,   389,    -1,   391,
     392,   393,    -1,    -1,   396,    -1,    -1,    -1,    -1,   401,
      -1,    -1,   404,    -1,    -1,   407,   408,    -1,   410,   411,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    29,    30,    31,    32,    -1,
      -1,    35,    -1,    37,    -1,    39,    40,    41,    42,    43,
      -1,    -1,    46,    -1,    48,    49,    50,    51,    -1,    -1,
      54,    55,    56,    57,    58,    59,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    -1,    88,    -1,    90,    91,    92,    -1,
      94,    -1,    -1,    97,    -1,    99,    -1,   101,   102,    -1,
     104,    -1,    -1,   107,   108,    -1,   110,   111,    -1,    -1,
     114,   115,    -1,   117,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,    -1,   129,   130,    -1,    -1,   133,
     134,    -1,   136,   137,    -1,    -1,   140,    -1,    -1,   143,
      -1,    -1,   146,   147,    -1,    -1,    -1,    -1,   152,   153,
      -1,    -1,    -1,   157,   158,   159,   160,    -1,    -1,   163,
     164,   165,   166,   167,   168,   169,    -1,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,    -1,    -1,   218,    -1,   220,    -1,   222,   223,
      -1,    -1,   226,   227,   228,    -1,   230,   231,   232,    -1,
      -1,   235,    -1,   237,    -1,    -1,   240,    -1,   242,    -1,
      -1,   245,   246,   247,   248,   249,   250,    -1,    -1,   253,
     254,    -1,    -1,    -1,   258,    -1,    -1,    -1,   262,    -1,
      -1,    -1,   266,    -1,   268,    -1,    -1,   271,   272,   273,
     274,   275,    -1,    -1,   278,   279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,   292,    -1,
     294,   295,   296,   297,    -1,    -1,    -1,   301,    -1,    -1,
      -1,    -1,    -1,   307,   308,    -1,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,    -1,    -1,    -1,   323,
     324,   325,   326,    -1,   328,   329,   330,   331,   332,   333,
     334,    -1,   336,   337,    -1,    -1,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,    -1,   351,   352,   353,
     354,   355,   356,   357,    -1,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,    -1,   379,    -1,   381,   382,   383,
     384,    -1,    -1,   387,    -1,   389,    -1,   391,   392,   393,
      -1,    -1,   396,    -1,    -1,    -1,    -1,   401,    -1,    -1,
     404,    -1,    -1,   407,   408,    -1,   410,   411,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    25,
      26,    27,    -1,    29,    30,    31,    32,    -1,    -1,    35,
      -1,    37,    -1,    39,    40,    41,    42,    43,    -1,    -1,
      46,    -1,    48,    49,    50,    51,    -1,    -1,    54,    55,
      56,    57,    58,    59,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    -1,    -1,    72,    73,    -1,    -1,
      -1,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    -1,    88,    -1,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    -1,    99,    -1,   101,   102,    -1,   104,    -1,
      -1,   107,   108,    -1,   110,   111,    -1,    -1,   114,   115,
      -1,   117,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,    -1,   129,   130,    -1,    -1,   133,   134,    -1,
     136,   137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,
     146,   147,    -1,    -1,    -1,    -1,   152,   153,    -1,    -1,
      -1,   157,   158,   159,   160,    -1,    -1,   163,   164,   165,
     166,   167,   168,   169,    -1,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,    -1,   193,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,
     226,   227,   228,    -1,   230,   231,   232,    -1,    -1,   235,
      -1,   237,    -1,    -1,   240,    -1,   242,    -1,    -1,   245,
     246,   247,   248,   249,   250,    -1,    -1,   253,   254,    -1,
      -1,    -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,
     266,    -1,   268,    -1,    -1,   271,   272,   273,   274,   275,
      -1,    -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,
     296,   297,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,
      -1,   307,   308,    -1,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,    -1,    -1,    -1,   323,   324,   325,
     326,    -1,   328,   329,   330,   331,   332,   333,   334,    -1,
     336,   337,    -1,    -1,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,    -1,   351,   352,   353,   354,   355,
     356,   357,    -1,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,    -1,   379,    -1,   381,   382,   383,   384,    -1,
      -1,   387,    -1,   389,    -1,   391,   392,   393,    -1,    -1,
     396,    -1,    -1,    -1,    -1,   401,    -1,    -1,   404,    -1,
      -1,   407,   408,    -1,   410,   411,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    29,    30,    31,    32,    -1,    -1,    35,    -1,    37,
      -1,    39,    40,    41,    42,    43,    -1,    -1,    46,    -1,
      48,    49,    50,    51,    -1,    -1,    54,    55,    56,    57,
      58,    59,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    -1,
      88,    -1,    90,    91,    92,    -1,    94,    -1,    -1,    97,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
     108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,   117,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
      -1,   129,   130,    -1,    -1,   133,   134,    -1,   136,   137,
      -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,   147,
      -1,    -1,    -1,    -1,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,    -1,   163,   164,   165,   166,   167,
     168,   169,    -1,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,    -1,   193,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,    -1,    -1,
     218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,   227,
     228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,   237,
      -1,    -1,   240,    -1,   242,    -1,    -1,   245,   246,   247,
     248,   249,   250,    -1,    -1,   253,   254,    -1,    -1,    -1,
     258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,    -1,
     268,    -1,    -1,   271,   272,   273,   274,   275,    -1,    -1,
     278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,   297,
      -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,   307,
     308,    -1,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,    -1,    -1,    -1,   323,   324,   325,   326,    -1,
     328,   329,   330,   331,   332,   333,   334,    -1,   336,   337,
      -1,    -1,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,    -1,   351,   352,   353,   354,   355,   356,   357,
      -1,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
      -1,   379,    -1,   381,   382,   383,   384,    -1,    -1,   387,
      -1,   389,    -1,   391,   392,   393,    -1,    -1,   396,    -1,
      -1,    -1,    -1,   401,    -1,    -1,   404,   405,    -1,   407,
     408,    -1,   410,   411,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    32,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    42,    43,    -1,    -1,    46,    -1,    48,    49,
      50,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    -1,    88,    -1,
      90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
     130,    -1,    -1,   133,   134,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,   152,   153,    -1,    -1,    -1,   157,   158,   159,
     160,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,   242,    -1,    -1,   245,   246,   247,   248,   249,
     250,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,   271,   272,   273,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,   301,    -1,    -1,    -1,    -1,    -1,   307,   308,    -1,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,   323,   324,   325,   326,    -1,   328,   329,
     330,   331,   332,   333,   334,    -1,   336,   337,    -1,    -1,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
      -1,   351,   352,   353,   354,   355,   356,   357,    -1,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,    -1,   379,
      -1,   381,   382,   383,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,    -1,    -1,   396,    -1,    -1,    -1,
      -1,   401,    -1,    -1,   404,   405,    -1,   407,   408,    -1,
     410,   411,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    29,    30,    31,
      32,    -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,
      42,    43,    -1,    -1,    46,    -1,    48,    49,    50,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    -1,    88,    -1,    90,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,    -1,   114,   115,    -1,   117,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,    -1,   129,   130,    -1,
      -1,   133,   134,    -1,   136,   137,    -1,    -1,   140,    -1,
      -1,   143,    -1,    -1,   146,   147,    -1,    -1,    -1,    -1,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
      -1,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,   218,    -1,   220,    -1,
     222,   223,    -1,    -1,   226,   227,   228,    -1,   230,   231,
     232,    -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,
     242,    -1,    -1,   245,   246,   247,   248,   249,   250,    -1,
      -1,   253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,
     262,    -1,    -1,    -1,   266,    -1,   268,    -1,    -1,   271,
     272,   273,   274,   275,    -1,    -1,   278,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
     292,    -1,   294,   295,   296,   297,    -1,    -1,    -1,   301,
      -1,    -1,    -1,    -1,    -1,   307,   308,    -1,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,    -1,    -1,
      -1,   323,   324,   325,   326,    -1,   328,   329,   330,   331,
     332,   333,   334,    -1,   336,   337,    -1,    -1,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,    -1,   351,
     352,   353,   354,   355,   356,   357,    -1,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,    -1,   379,    -1,   381,
     382,   383,   384,    -1,    -1,   387,    -1,   389,    -1,   391,
     392,   393,    -1,    -1,   396,    -1,    -1,    -1,    -1,   401,
      -1,    -1,   404,   405,    -1,   407,   408,    -1,   410,   411,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    29,    30,    31,    32,    -1,
      -1,    35,    -1,    37,    -1,    39,    40,    41,    42,    43,
      -1,    -1,    46,    -1,    48,    49,    50,    51,    -1,    -1,
      54,    55,    56,    57,    58,    59,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    -1,    -1,    72,    73,
      -1,    -1,    -1,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    -1,    88,    -1,    90,    91,    92,    -1,
      94,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107,   108,    -1,   110,   111,    -1,    -1,
     114,   115,    -1,   117,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,    -1,   129,   130,    -1,    -1,   133,
     134,    -1,   136,   137,    -1,    -1,   140,    -1,    -1,   143,
      -1,    -1,   146,   147,    -1,    -1,    -1,    -1,   152,   153,
      -1,    -1,    -1,   157,   158,   159,   160,    -1,    -1,   163,
     164,   165,   166,   167,   168,   169,    -1,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,    -1,   193,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,    -1,    -1,   218,    -1,   220,    -1,   222,   223,
      -1,    -1,   226,   227,   228,    -1,   230,   231,   232,    -1,
      -1,   235,    -1,   237,    -1,    -1,   240,    -1,   242,    -1,
      -1,   245,   246,   247,   248,   249,   250,    -1,    -1,   253,
     254,    -1,    -1,    -1,   258,    -1,    -1,    -1,   262,    -1,
      -1,    -1,   266,    -1,   268,    -1,    -1,   271,   272,   273,
     274,   275,    -1,    -1,   278,   279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,   292,    -1,
     294,   295,   296,   297,    -1,    -1,    -1,   301,    -1,    -1,
      -1,    -1,    -1,   307,   308,    -1,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,    -1,    -1,    -1,   323,
     324,   325,   326,    -1,   328,   329,   330,   331,   332,   333,
     334,    -1,   336,   337,    -1,    -1,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,    -1,   351,   352,   353,
     354,   355,   356,   357,    -1,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,    -1,   379,    -1,   381,   382,   383,
     384,    -1,    -1,   387,    -1,   389,    -1,   391,   392,   393,
      -1,    -1,   396,    -1,    -1,    -1,    -1,   401,    -1,    -1,
     404,   405,    -1,   407,   408,    -1,   410,   411,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    25,
      26,    27,    -1,    29,    30,    31,    32,    -1,    -1,    35,
      -1,    37,    -1,    39,    40,    41,    42,    43,    -1,    -1,
      46,    -1,    48,    49,    50,    51,    -1,    -1,    54,    55,
      56,    57,    58,    59,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    -1,    -1,    72,    73,    -1,    -1,
      -1,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    -1,    88,    -1,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,   108,    -1,   110,   111,    -1,    -1,   114,   115,
      -1,   117,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,    -1,   129,   130,    -1,    -1,   133,   134,    -1,
     136,   137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,
     146,   147,    -1,    -1,    -1,    -1,   152,   153,    -1,    -1,
      -1,   157,   158,   159,   160,    -1,    -1,   163,   164,   165,
     166,   167,   168,   169,    -1,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,    -1,   193,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,
     226,   227,   228,    -1,   230,   231,   232,    -1,    -1,   235,
      -1,   237,    -1,    -1,   240,    -1,   242,    -1,    -1,   245,
     246,   247,   248,   249,   250,    -1,    -1,   253,   254,    -1,
      -1,    -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,
     266,    -1,   268,    -1,    -1,   271,   272,   273,   274,   275,
      -1,    -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,
     296,   297,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,
      -1,   307,   308,    -1,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,    -1,    -1,    -1,   323,   324,   325,
     326,    -1,   328,   329,   330,   331,   332,   333,   334,    -1,
     336,   337,    -1,    -1,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,    -1,   351,   352,   353,   354,   355,
     356,   357,    -1,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,    -1,   379,    -1,   381,   382,   383,   384,    -1,
      -1,   387,    -1,   389,    -1,   391,   392,   393,    -1,    -1,
     396,    -1,    -1,    -1,    -1,   401,    -1,    -1,   404,    -1,
      -1,   407,   408,    -1,   410,   411,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    29,    30,    31,    32,    -1,    -1,    35,    -1,    37,
      -1,    39,    40,    41,    42,    43,    -1,    -1,    46,    -1,
      48,    49,    50,    51,    -1,    -1,    54,    55,    56,    57,
      58,    59,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    -1,
      88,    -1,    90,    91,    92,    -1,    94,    -1,    -1,    97,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
     108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,   117,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
      -1,   129,   130,    -1,    -1,   133,   134,    -1,   136,   137,
      -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,   147,
      -1,    -1,    -1,    -1,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,    -1,   163,   164,   165,   166,   167,
     168,   169,    -1,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,    -1,   193,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,    -1,    -1,
     218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,   227,
     228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,   237,
      -1,    -1,   240,    -1,   242,    -1,    -1,   245,   246,   247,
     248,   249,   250,    -1,    -1,   253,   254,    -1,    -1,    -1,
     258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,    -1,
     268,    -1,    -1,   271,   272,   273,   274,   275,    -1,    -1,
     278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,   297,
      -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,   307,
     308,    -1,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,    -1,    -1,    -1,   323,   324,   325,   326,    -1,
     328,   329,   330,   331,   332,   333,   334,    -1,   336,   337,
      -1,    -1,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,    -1,   351,   352,   353,   354,   355,   356,   357,
      -1,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
      -1,   379,    -1,   381,   382,   383,   384,    -1,    -1,   387,
      -1,   389,    -1,   391,   392,   393,    -1,    -1,   396,    -1,
      -1,    -1,    -1,   401,    -1,    -1,   404,    -1,    -1,   407,
     408,    -1,   410,   411,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    32,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    42,    43,    -1,    -1,    46,    -1,    48,    49,
      50,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    -1,    88,    -1,
      90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
     130,    -1,    -1,   133,   134,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,   152,   153,    -1,    -1,    -1,   157,   158,   159,
     160,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,   242,    -1,    -1,   245,   246,   247,   248,   249,
     250,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,   271,   272,   273,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,   301,    -1,    -1,    -1,    -1,    -1,   307,   308,    -1,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,   323,   324,   325,   326,    -1,   328,   329,
     330,   331,   332,   333,   334,    -1,   336,   337,    -1,    -1,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
      -1,   351,   352,   353,   354,   355,   356,   357,    -1,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,    -1,   379,
      -1,   381,   382,   383,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,    -1,    -1,   396,    -1,    -1,    -1,
      -1,   401,    -1,    -1,   404,    -1,    -1,   407,   408,    -1,
     410,   411,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    29,    30,    31,
      32,    -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,
      42,    43,    -1,    -1,    46,    -1,    48,    49,    50,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    -1,    88,    -1,    90,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,    -1,   114,   115,    -1,   117,   118,   119,   120,   121,
      -1,    -1,    -1,   125,    -1,   127,    -1,   129,   130,    -1,
      -1,   133,   134,    -1,   136,   137,    -1,    -1,   140,    -1,
      -1,   143,    -1,    -1,   146,   147,    -1,    -1,    -1,    -1,
     152,   153,    -1,    -1,    -1,   157,   158,   159,   160,    -1,
      -1,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,   218,    -1,   220,    -1,
     222,   223,    -1,    -1,   226,   227,   228,    -1,   230,   231,
     232,    -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,
     242,    -1,    -1,   245,   246,   247,   248,   249,   250,    -1,
      -1,   253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,
     262,    -1,    -1,    -1,   266,    -1,   268,    -1,    -1,   271,
     272,   273,   274,   275,    -1,    -1,   278,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
     292,    -1,   294,   295,   296,   297,    -1,    -1,    -1,   301,
      -1,    -1,    -1,    -1,    -1,   307,   308,    -1,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,    -1,    -1,
      -1,   323,   324,   325,   326,    -1,   328,   329,   330,   331,
     332,   333,   334,    -1,   336,   337,    -1,    -1,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,    -1,   351,
     352,   353,   354,   355,   356,   357,    -1,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,    -1,   379,    -1,   381,
     382,   383,   384,    -1,    -1,   387,    -1,   389,    -1,   391,
     392,   393,    -1,    -1,   396,    -1,    -1,    -1,    -1,   401,
      -1,    -1,   404,    -1,    -1,   407,   408,    -1,   410,   411,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    29,    30,    31,    32,    -1,
      -1,    35,    -1,    37,    -1,    39,    40,    41,    42,    43,
      -1,    -1,    46,    -1,    48,    49,    50,    51,    -1,    -1,
      54,    55,    56,    57,    58,    59,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    -1,    -1,    72,    73,
      -1,    -1,    -1,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    -1,    88,    -1,    90,    91,    92,    -1,
      94,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,   107,   108,    -1,   110,   111,    -1,    -1,
     114,   115,    -1,   117,   118,   119,   120,   121,    -1,    -1,
      -1,   125,    -1,   127,    -1,   129,   130,    -1,    -1,   133,
     134,    -1,   136,   137,    -1,    -1,   140,    -1,    -1,   143,
      -1,    -1,   146,   147,    -1,    -1,    -1,    -1,   152,   153,
      -1,    -1,    -1,   157,   158,   159,   160,    -1,    -1,   163,
     164,   165,   166,   167,   168,   169,    -1,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,    -1,   193,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,    -1,    -1,   218,    -1,   220,    -1,   222,   223,
      -1,    -1,   226,   227,   228,    -1,   230,   231,   232,    -1,
      -1,   235,    -1,   237,    -1,    -1,   240,    -1,   242,    -1,
      -1,   245,   246,   247,   248,   249,   250,    -1,    -1,   253,
     254,    -1,    -1,    -1,   258,    -1,    -1,    -1,   262,    -1,
      -1,    -1,   266,    -1,   268,    -1,    -1,   271,   272,   273,
     274,   275,    -1,    -1,   278,   279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,   292,    -1,
     294,   295,   296,   297,    -1,    -1,    -1,   301,    -1,    -1,
      -1,    -1,    -1,   307,   308,    -1,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,    -1,    -1,    -1,   323,
     324,   325,   326,    -1,   328,   329,   330,   331,   332,   333,
     334,    -1,   336,   337,    -1,    -1,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,    -1,   351,   352,   353,
     354,   355,   356,   357,    -1,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,    -1,   379,    -1,   381,   382,   383,
     384,    -1,    -1,   387,    -1,   389,    -1,   391,   392,   393,
      -1,    -1,   396,    -1,    -1,    -1,    -1,   401,    -1,    -1,
     404,    -1,    -1,   407,   408,    -1,   410,   411,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    25,
      26,    27,    -1,    29,    30,    31,    32,    -1,    -1,    35,
      -1,    37,    -1,    39,    40,    41,    42,    43,    -1,    -1,
      46,    -1,    48,    49,    50,    51,    -1,    -1,    54,    55,
      56,    57,    58,    59,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    -1,    -1,    72,    73,    -1,    -1,
      -1,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    -1,    88,    -1,    90,    91,    92,    -1,    94,    -1,
      -1,    97,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,   107,   108,    -1,   110,   111,    -1,    -1,   114,   115,
      -1,   117,   118,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,   127,    -1,   129,   130,    -1,    -1,   133,   134,    -1,
     136,   137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,
     146,   147,    -1,    -1,    -1,    -1,   152,   153,    -1,    -1,
      -1,   157,   158,   159,   160,    -1,    -1,   163,   164,   165,
     166,   167,   168,   169,    -1,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,    -1,   193,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,
     226,   227,   228,    -1,   230,   231,   232,    -1,    -1,   235,
      -1,   237,    -1,    -1,   240,    -1,   242,    -1,    -1,   245,
     246,   247,   248,   249,   250,    -1,    -1,   253,   254,    -1,
      -1,    -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,
     266,    -1,   268,    -1,    -1,   271,   272,   273,   274,   275,
      -1,    -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,
     296,   297,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,
      -1,   307,   308,    -1,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,    -1,    -1,    -1,   323,   324,   325,
     326,    -1,   328,   329,   330,   331,   332,   333,   334,    -1,
     336,   337,    -1,    -1,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,    -1,   351,   352,   353,   354,   355,
     356,   357,    -1,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,    -1,   379,    -1,   381,   382,   383,   384,    -1,
      -1,   387,    -1,   389,    -1,   391,   392,   393,    -1,    -1,
     396,    -1,    -1,    -1,    -1,   401,    -1,    -1,   404,    -1,
      -1,   407,   408,    -1,   410,   411,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    29,    30,    31,    32,    -1,    -1,    35,    -1,    37,
      -1,    39,    40,    41,    42,    43,    -1,    -1,    46,    -1,
      48,    49,    50,    51,    -1,    -1,    54,    55,    56,    57,
      58,    59,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    -1,
      88,    -1,    90,    91,    92,    -1,    94,    -1,    -1,    97,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
     108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,   117,
     118,   119,   120,   121,    -1,    -1,    -1,   125,    -1,   127,
      -1,   129,   130,    -1,    -1,   133,   134,    -1,   136,   137,
      -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,   147,
      -1,    -1,    -1,    -1,   152,   153,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,    -1,   163,   164,   165,   166,   167,
     168,   169,    -1,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,    -1,   193,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,    -1,    -1,
     218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,   227,
     228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,   237,
      -1,    -1,   240,    -1,   242,    -1,    -1,   245,   246,   247,
     248,   249,   250,    -1,    -1,   253,   254,    -1,    -1,    -1,
     258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,    -1,
     268,    -1,    -1,   271,   272,   273,   274,   275,    -1,    -1,
     278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,   297,
      -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,   307,
     308,    -1,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,    -1,    -1,    -1,   323,   324,   325,   326,    -1,
     328,   329,   330,   331,   332,   333,   334,    -1,   336,   337,
      -1,    -1,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,    -1,   351,   352,   353,   354,   355,   356,   357,
      -1,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
      -1,   379,    -1,   381,   382,   383,   384,    -1,    -1,   387,
      -1,   389,    -1,   391,   392,   393,    -1,    -1,   396,    -1,
      -1,    -1,    -1,   401,    -1,    -1,   404,    -1,    -1,   407,
     408,    -1,   410,   411,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    32,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    42,    43,    -1,    -1,    46,    -1,    48,    49,
      50,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    -1,    88,    -1,
      90,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,   121,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
     130,    -1,    -1,   133,   134,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,   152,   153,    -1,    -1,    -1,   157,   158,   159,
     160,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,   242,    -1,    -1,   245,   246,   247,   248,   249,
     250,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,   271,   272,   273,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,   301,    -1,    -1,    -1,    -1,    -1,   307,   308,    -1,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,   323,   324,   325,   326,    -1,   328,   329,
     330,   331,   332,   333,   334,    -1,   336,   337,    -1,    -1,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
      -1,   351,   352,   353,   354,   355,   356,   357,    -1,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,    -1,   379,
      -1,   381,   382,   383,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,    -1,    -1,   396,    -1,    -1,    -1,
      -1,   401,    -1,    -1,   404,    -1,    -1,   407,   408,    -1,
     410,   411,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    29,    30,    31,
      -1,    -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    -1,    66,    -1,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    91,
      92,    -1,    94,    -1,    -1,    97,    98,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,    -1,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,    -1,   125,   126,   127,    -1,   129,    -1,    -1,
      -1,   133,    -1,    -1,   136,   137,    -1,   139,   140,    -1,
      -1,   143,    -1,    -1,   146,   147,    -1,    -1,   150,    -1,
      -1,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,    -1,
      -1,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,   186,    -1,   188,    -1,    -1,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,    -1,    -1,    -1,    -1,   218,    -1,   220,    -1,
     222,   223,    -1,    -1,   226,   227,   228,    -1,   230,   231,
     232,    -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,
      -1,    -1,    -1,   245,   246,    -1,    -1,    -1,    -1,    -1,
      -1,   253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,
     262,    -1,    -1,    -1,   266,    -1,   268,    -1,    -1,    -1,
     272,    -1,   274,   275,    -1,    -1,   278,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
     292,    -1,   294,   295,   296,   297,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,
      -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
     352,   353,   354,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,   363,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,    -1,   381,
      -1,    -1,   384,    -1,    -1,   387,    -1,   389,    -1,   391,
     392,   393,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   411,
      22,    -1,    -1,    25,    26,    27,    -1,    29,    30,    31,
      -1,    -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    -1,    -1,
      62,    63,    64,    -1,    66,    -1,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    91,
      92,    -1,    94,    -1,    -1,    97,    98,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,    -1,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,    -1,   125,   126,   127,    -1,   129,    -1,    -1,
      -1,   133,    -1,    -1,   136,   137,    -1,   139,   140,    -1,
      -1,   143,    -1,    -1,   146,   147,    -1,    -1,   150,    -1,
      -1,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,    -1,
      -1,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,   186,    -1,   188,    -1,    -1,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,    -1,    -1,    -1,    -1,   218,    -1,   220,    -1,
     222,   223,    -1,    -1,   226,   227,   228,    -1,   230,   231,
     232,    -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,
      -1,    -1,    -1,   245,   246,    -1,    -1,    -1,    -1,    -1,
      -1,   253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,
     262,    -1,    -1,    -1,   266,    -1,   268,    -1,    -1,    -1,
     272,    -1,   274,   275,    -1,    -1,   278,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
     292,    -1,   294,   295,   296,   297,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,
      -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
     352,   353,   354,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,   363,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,    -1,   381,
      -1,    -1,   384,    -1,    -1,   387,    -1,   389,    -1,   391,
     392,   393,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   411,
      22,    -1,    -1,    25,    26,    27,    -1,    29,    30,    31,
      -1,    -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    -1,    66,    -1,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,    -1,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,    -1,   125,    -1,   127,    -1,   129,    -1,    -1,
      -1,   133,    -1,    -1,   136,   137,    -1,    -1,   140,    -1,
      -1,   143,    -1,    -1,   146,   147,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,    -1,
      -1,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,   186,    -1,   188,    -1,    -1,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,    -1,    -1,    -1,    -1,   218,    -1,   220,    -1,
     222,   223,    -1,    -1,   226,   227,   228,    -1,   230,   231,
     232,    -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,
      -1,    -1,    -1,   245,   246,    -1,    -1,    -1,    -1,    -1,
      -1,   253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,
     262,    -1,    -1,    -1,   266,    -1,   268,    -1,    -1,    -1,
     272,    -1,   274,   275,    -1,    -1,   278,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
     292,    -1,   294,   295,   296,   297,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,
      -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
     352,   353,   354,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,   363,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,    -1,   381,
      -1,    -1,   384,    -1,    -1,   387,    -1,   389,    -1,   391,
     392,   393,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,   404,    -1,    -1,    -1,    -1,    -1,    -1,   411,
      22,    -1,    -1,    25,    26,    27,    -1,    29,    30,    31,
      -1,    -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    -1,    66,    -1,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,    -1,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,    -1,   125,    -1,   127,    -1,   129,    -1,    -1,
      -1,   133,    -1,    -1,   136,   137,    -1,    -1,   140,    -1,
      -1,   143,    -1,    -1,   146,   147,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,    -1,
      -1,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,   186,    -1,   188,    -1,    -1,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,    -1,    -1,    -1,    -1,   218,    -1,   220,    -1,
     222,   223,    -1,    -1,   226,   227,   228,    -1,   230,   231,
     232,    -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,
      -1,    -1,    -1,   245,   246,    -1,    -1,    -1,    -1,    -1,
      -1,   253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,
     262,    -1,    -1,    -1,   266,    -1,   268,    -1,    -1,    -1,
     272,    -1,   274,   275,    -1,    -1,   278,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
     292,    -1,   294,   295,   296,   297,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,
      -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
     352,   353,   354,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,   363,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,    -1,   381,
      -1,    -1,   384,    -1,    -1,   387,    -1,   389,    -1,   391,
     392,   393,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,   405,    -1,    -1,    -1,    -1,    -1,   411,
      22,    -1,    -1,    25,    26,    27,    -1,    29,    30,    31,
      -1,    -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    -1,    -1,
      62,    63,    64,    -1,    66,    -1,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,    -1,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,    -1,   125,    -1,   127,    -1,   129,    -1,    -1,
      -1,   133,    -1,    -1,   136,   137,    -1,    -1,   140,    -1,
      -1,   143,   144,    -1,   146,   147,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,    -1,
      -1,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,   186,    -1,   188,    -1,    -1,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,    -1,    -1,    -1,    -1,   218,    -1,   220,    -1,
     222,   223,    -1,    -1,   226,   227,   228,    -1,   230,   231,
     232,    -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,
      -1,    -1,    -1,   245,   246,    -1,    -1,    -1,    -1,    -1,
      -1,   253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,
     262,    -1,    -1,    -1,   266,    -1,   268,    -1,    -1,    -1,
     272,    -1,   274,   275,    -1,    -1,   278,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
     292,    -1,   294,   295,   296,   297,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,
      -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
     352,   353,   354,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,   363,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,    -1,   381,
      -1,    -1,   384,    -1,    -1,   387,    -1,   389,    -1,   391,
     392,   393,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   411,
      22,    -1,    -1,    25,    26,    27,    -1,    29,    30,    31,
      -1,    -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    -1,    -1,
      62,    63,    64,    -1,    66,    -1,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    91,
      92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,
      -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,
      -1,    -1,   114,   115,    -1,   117,   118,   119,   120,    -1,
      -1,    -1,    -1,   125,    -1,   127,    -1,   129,    -1,    -1,
      -1,   133,    -1,    -1,   136,   137,    -1,    -1,   140,    -1,
      -1,   143,    -1,    -1,   146,   147,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,    -1,   157,    -1,   159,    -1,    -1,
      -1,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,    -1,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,   186,    -1,   188,    -1,    -1,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,    -1,    -1,    -1,    -1,   218,    -1,   220,    -1,
     222,   223,    -1,    -1,   226,   227,   228,    -1,   230,   231,
     232,    -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,
      -1,    -1,    -1,   245,   246,    -1,    -1,    -1,    -1,    -1,
      -1,   253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,
     262,    -1,    -1,    -1,   266,    -1,   268,    -1,    -1,    -1,
     272,    -1,   274,   275,    -1,    -1,   278,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
     292,    -1,   294,   295,   296,   297,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,
      -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
     352,   353,   354,    -1,   356,    -1,    -1,    -1,    -1,    -1,
      -1,   363,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,    -1,   381,
      -1,    -1,   384,    -1,    -1,   387,    -1,   389,    -1,   391,
     392,   393,    -1,    -1,    -1,    -1,   398,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   411,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    25,    26,
      27,    -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,
      37,    -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    -1,    66,
      -1,    68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,   108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,
     117,   118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,
     147,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
     157,    -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,    -1,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   186,
      -1,   188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,
     227,   228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,
     237,    -1,    -1,   240,    -1,    -1,    -1,    -1,   245,   246,
      -1,    -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,
      -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,
      -1,   268,    -1,    -1,    -1,   272,    -1,   274,   275,    -1,
      -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,
     297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     307,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   340,   341,    -1,   343,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,
     387,    -1,   389,    -1,   391,   392,   393,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,   404,    -1,    -1,
      -1,    -1,    -1,    -1,   411,    22,    -1,    -1,    25,    26,
      27,    -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,
      37,    -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    -1,    66,
      -1,    68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,   108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,
     117,   118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,
     147,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
     157,    -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,    -1,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   186,
      -1,   188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,
     227,   228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,
     237,    -1,    -1,   240,    -1,    -1,    -1,    -1,   245,   246,
      -1,    -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,
      -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,
      -1,   268,    -1,    -1,    -1,   272,    -1,   274,   275,    -1,
      -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,
     297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     307,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   340,   341,    -1,   343,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,
     387,    -1,   389,    -1,   391,   392,   393,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,   405,    -1,
      -1,    -1,    -1,    -1,   411,    22,    -1,    -1,    25,    26,
      27,    -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,
      37,    -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    -1,    66,
      -1,    68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,   108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,
     117,   118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,
     147,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
     157,    -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,    -1,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   186,
      -1,   188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,
     227,   228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,
     237,    -1,    -1,   240,    -1,    -1,    -1,    -1,   245,   246,
      -1,    -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,
      -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,
      -1,   268,    -1,    -1,    -1,   272,    -1,   274,   275,    -1,
      -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,
     297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     307,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   340,   341,    -1,   343,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,
     387,    -1,   389,    -1,   391,   392,   393,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   411,    22,    -1,    -1,    25,    26,
      27,    -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,
      37,    -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    -1,    66,
      -1,    68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,   108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,
     117,   118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,
     147,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
     157,    -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,    -1,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   186,
      -1,   188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,
     227,   228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,
     237,    -1,    -1,   240,    -1,    -1,    -1,    -1,   245,   246,
      -1,    -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,
      -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,
      -1,   268,    -1,    -1,    -1,   272,    -1,   274,   275,    -1,
      -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,
     297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     307,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   340,   341,    -1,   343,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,
     387,    -1,   389,    -1,   391,   392,   393,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   411,    22,    -1,    -1,    25,    26,
      27,    -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,
      37,    -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    -1,    66,
      -1,    68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,   108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,
     117,   118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,
     147,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
     157,    -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,    -1,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   186,
      -1,   188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,
     227,   228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,
     237,    -1,    -1,   240,    -1,    -1,    -1,    -1,   245,   246,
      -1,    -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,
      -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,
      -1,   268,    -1,    -1,    -1,   272,    -1,   274,   275,    -1,
      -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,
     297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     307,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   340,   341,    -1,   343,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,
     387,    -1,   389,    -1,   391,   392,   393,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   411,    22,    -1,    -1,    25,    26,
      27,    -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,
      37,    -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    -1,    66,
      -1,    68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,   108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,
     117,   118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,
     147,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,    -1,
     157,    -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,    -1,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   186,
      -1,   188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,
     227,   228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,
     237,    -1,    -1,   240,    -1,   242,    -1,    -1,   245,   246,
      -1,    -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,
      -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,
      -1,   268,    -1,    -1,    -1,   272,    -1,   274,   275,    -1,
      -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,
     297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     307,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   340,   341,    -1,   343,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,
     387,    -1,   389,    -1,   391,   392,   393,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   410,    -1,    22,    -1,    -1,    25,    26,
      27,    -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,
      37,    -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    -1,    66,
      -1,    68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,   108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,
     117,   118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,
     147,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,    -1,
     157,    -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,    -1,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   186,
      -1,   188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,
     227,   228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,
     237,    -1,    -1,   240,    -1,   242,    -1,    -1,   245,   246,
      -1,    -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,
      -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,
      -1,   268,    -1,    -1,    -1,   272,    -1,   274,   275,    -1,
      -1,   278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,
     297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     307,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   340,   341,    -1,   343,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,
     387,    -1,   389,    -1,   391,   392,   393,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   410,    -1,    22,    -1,    -1,    25,    26,
      27,    -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,
      37,    -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    -1,    -1,    62,    63,    64,    -1,    66,
      -1,    68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    84,    85,    -1,
      -1,    88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,
     107,   108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,
     117,   118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,
     137,    -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,
     147,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
     157,    -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,    -1,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   186,
      -1,   188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,   218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,
     227,   228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,
     237,    -1,    -1,   240,    -1,    -1,    -1,    -1,   245,   246,
      -1,    -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,
      -1,   258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,
      -1,   268,    -1,    -1,   271,   272,    -1,   274,   275,    -1,
      -1,   278,   279,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,   290,    -1,   292,    -1,   294,   295,   296,
     297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     307,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,   323,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,   340,   341,    -1,   343,    -1,    -1,    75,
      -1,    -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,
      -1,    -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,
     387,    -1,   389,    75,   391,   392,   393,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   410,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,   187,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,    -1,   155,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,   187,
      -1,   217,    -1,    -1,    -1,    -1,    75,    -1,   196,   197,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      75,   267,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,   267,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   309,    -1,   267,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
     145,   309,    -1,    75,    -1,    -1,    -1,   196,   197,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   309,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,   196,   197,    -1,    -1,    -1,    -1,    -1,   394,   395,
     396,   397,   398,   399,   400,    -1,    -1,   403,    -1,   405,
     406,    -1,   217,   145,    -1,    -1,    -1,    -1,   267,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    -1,   403,    -1,   405,   406,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,    -1,
      -1,   403,    -1,   405,   406,   187,   145,    -1,    -1,    -1,
     309,    -1,   267,    -1,   196,   197,   155,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,   309,    -1,    -1,   196,   197,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    75,    -1,   267,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,    -1,    -1,   403,    -1,   405,   406,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    75,   309,   267,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,    -1,   403,    -1,
     405,   406,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     309,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    75,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,   187,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,   155,    -1,    -1,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,    -1,
      -1,   403,    -1,   405,   406,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    75,
      -1,    -1,   145,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,   155,    -1,   403,    -1,   405,   406,   217,    -1,
      -1,    -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,   309,   267,   145,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,   155,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
     309,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,    75,    -1,   267,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,    -1,
      -1,   403,    -1,   405,   406,    75,   309,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,   267,   145,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,   155,    -1,   403,    -1,    -1,   406,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,   196,   197,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   309,   187,   145,    -1,    -1,    -1,   217,
      -1,    -1,    -1,   196,   197,   155,    75,    -1,    -1,    -1,
      -1,   394,   395,   396,   397,   398,   399,   400,   187,    -1,
     403,    -1,   405,   406,   217,    -1,    -1,   196,   197,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   217,   267,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    -1,   145,    -1,   394,   395,
     396,   397,   398,   399,   400,    -1,   155,   403,    -1,   405,
     406,   309,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   309,   267,   187,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
     309,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   309,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    -1,   403,    -1,   405,   406,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    75,    -1,   267,    -1,
     155,   394,   395,   396,   397,   398,   399,   400,    -1,    -1,
     403,    -1,   405,   406,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,   394,   395,   396,   397,   398,
     399,   400,   187,    -1,   403,    -1,   405,   406,    -1,    -1,
     309,   196,   197,    -1,   394,   395,   396,   397,   398,   399,
     400,    -1,    -1,   403,    -1,   405,   406,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      75,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,   267,    -1,    -1,    -1,    -1,   196,   197,    -1,
      -1,    -1,    -1,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,    -1,    -1,   403,    -1,   405,   406,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
     145,    -1,    -1,    -1,   309,    -1,    -1,    -1,    -1,    -1,
     155,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,
      -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   196,   197,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,   145,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,   155,    75,    -1,
     309,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,    -1,   403,    -1,
      -1,   406,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
      -1,    75,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,   309,   394,   395,   396,   397,   398,
     399,   400,    -1,    -1,   403,    -1,    -1,   406,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,   267,
     187,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,
     197,   155,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   309,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,    75,    -1,    -1,    -1,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,    -1,   403,    -1,
      -1,   406,    -1,   217,    -1,    -1,    -1,    -1,    75,    -1,
     267,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,   309,   267,   145,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,   155,   403,    -1,    -1,   406,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   309,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   196,   197,    75,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
     187,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,   196,
     197,    -1,    -1,    -1,    -1,    -1,    -1,   394,   395,   396,
     397,   398,   399,   400,    -1,    -1,   403,    -1,    -1,   406,
     217,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    75,   267,   145,    -1,    -1,
     394,   395,   396,   397,   398,   399,   400,   155,    -1,   403,
      -1,    -1,   406,    -1,   187,    -1,    -1,    -1,    -1,    -1,
     267,    -1,    -1,   196,   197,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,   309,   187,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,   196,   197,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   309,    -1,    -1,   145,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,   267,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,   267,
     145,    -1,    -1,   394,   395,   396,   397,   398,   399,   400,
     155,    -1,   403,    -1,    -1,   406,   309,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,   395,   396,
     397,   398,   399,   400,    -1,    -1,   403,    -1,    -1,   406,
      -1,   309,   187,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   196,   197,   155,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,   267,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,    -1,    -1,    -1,    -1,
      -1,   394,   395,   396,   397,   398,   399,   400,    -1,   309,
     403,    -1,    -1,   406,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    75,   267,    -1,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    -1,   403,    -1,    -1,   406,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,   309,   267,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,   145,    -1,    -1,   394,   395,   396,   397,   398,   399,
     400,   155,    -1,   403,    -1,    75,   406,   309,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    75,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,    -1,    75,    -1,    -1,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,    -1,   403,    -1,
      -1,   406,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    75,   145,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,   155,
      -1,   403,   145,    -1,   406,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,   267,    -1,   145,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   196,   197,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,    -1,    -1,   187,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,   196,   197,   309,    -1,   187,   145,    -1,
      -1,   217,    -1,    -1,    -1,    -1,   196,   197,   155,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,    -1,
     187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,
     197,   267,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,   309,
     394,   395,   396,   397,   398,   399,   400,    -1,    -1,   403,
      -1,    -1,   406,   309,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,   309,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    75,   309,
     267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    75,   309,    -1,   394,   395,   396,   397,   398,   399,
     400,    -1,    -1,   403,    75,    -1,   406,    -1,   394,   395,
     396,   397,   398,   399,   400,    -1,    -1,   403,   145,    -1,
     406,   394,   395,   396,   397,   398,   399,   400,   155,    75,
     403,    -1,    -1,   406,   394,   395,   396,   397,   398,   399,
     400,    -1,    -1,   403,    75,    -1,   406,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,
     197,   155,    -1,    -1,   145,    -1,    -1,   394,   395,   396,
     397,   398,   399,   400,   155,    -1,   403,    -1,    -1,   406,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,   196,   197,   145,    -1,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
     267,   187,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
     196,   197,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   196,   197,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   309,   267,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   267,    -1,    -1,    -1,   309,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,   309,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   309,    75,    -1,    -1,   394,   395,   396,
     397,   398,   399,   400,    -1,    -1,   403,    -1,   309,   406,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    75,    -1,    -1,
     394,   395,   396,   397,   398,   399,   400,    -1,    -1,   403,
      -1,    -1,   406,   394,   395,   396,   397,   398,   399,   400,
      -1,    -1,   403,    -1,   145,   406,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    75,    -1,   394,   395,
     396,   397,   398,   399,   400,    -1,    -1,   403,    -1,    -1,
     406,    -1,    75,   394,   395,   396,   397,   398,   399,   400,
      -1,    -1,   403,    -1,    -1,   406,   187,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   196,   197,   155,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   196,   197,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    75,   267,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   309,   267,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,   267,    -1,
      -1,   309,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,   267,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,    75,
     309,    -1,    -1,   394,   395,   396,   397,   398,   399,   400,
      -1,    -1,   403,    -1,    -1,   406,   309,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    75,    -1,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    -1,   403,    -1,    -1,   406,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,    -1,    -1,   403,    75,    -1,   406,   145,    -1,
      -1,   394,   395,   396,   397,   398,   399,   400,   155,   309,
     403,   187,   145,   406,    -1,    -1,    -1,    -1,    -1,    -1,
     196,   197,   155,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     187,   217,    -1,    -1,   124,    -1,    -1,    -1,    -1,   196,
     197,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,   145,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,   267,    -1,    75,   394,   395,   396,   397,   398,   399,
     400,    -1,    -1,   403,    -1,    -1,   406,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,    -1,
     267,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,   309,   267,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   309,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,   309,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   335,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,   196,   197,    -1,    -1,   394,   395,
     396,   397,   398,   399,   400,    -1,    -1,   403,    -1,   309,
     406,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,   395,   396,
     397,   398,   399,   400,    -1,    -1,   403,    -1,   145,   406,
      -1,   394,   395,   396,   397,   398,   399,   400,   155,    -1,
     403,    -1,   405,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,
     197,    -1,    -1,    -1,   394,   395,   396,   397,   398,   399,
     400,    -1,   187,   403,    -1,   405,    -1,   309,    -1,    -1,
     217,   196,   197,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     267,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   267,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,    -1,
      -1,   403,   309,   405,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,   309,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,   155,    -1,   394,   395,   396,
     397,   398,   399,   400,    -1,   187,   403,   145,   405,    -1,
      -1,    -1,    75,    -1,   196,   197,    -1,   155,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,   187,   403,    -1,
     405,    -1,    -1,    -1,    -1,   217,   196,   197,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,   196,   197,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,   145,    -1,    -1,   267,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,    -1,    -1,    -1,    -1,   267,    -1,    -1,
      -1,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,    -1,   309,    -1,   267,
      -1,    -1,   217,   196,   197,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,   309,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,   309,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   267,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    75,    -1,    -1,    -1,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,    -1,
      -1,   403,    -1,   405,   309,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,   394,   395,   396,   397,   398,   399,
     400,    -1,    -1,   403,    -1,   405,   309,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    -1,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,   187,   403,   145,
     405,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,   155,
      -1,   394,   395,   396,   397,   398,   399,   400,    -1,   187,
     403,    -1,   405,    -1,    -1,    -1,    -1,   217,   196,   197,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
     196,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,   267,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,   309,
      -1,   267,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   309,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   309,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,   394,   395,   396,   397,   398,   399,
     400,    -1,    -1,   403,    -1,   405,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    -1,   403,    -1,   405,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,   394,   395,
     396,   397,   398,   399,   400,    75,    -1,   403,   145,   405,
      -1,    -1,    -1,    -1,    -1,    -1,   309,    -1,   155,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,   196,
     197,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,   145,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,   155,    -1,    75,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      75,   394,   395,   396,   397,   398,   399,   400,    -1,    -1,
     403,    -1,   405,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,   145,    -1,   196,   197,    -1,    -1,
     267,    75,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   155,    -1,    -1,
     145,    -1,   309,    -1,    -1,   196,   197,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,   267,    -1,   187,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
      -1,   155,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   196,   197,    -1,    -1,   145,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,   309,
      -1,    -1,   217,   187,    -1,    -1,   267,    -1,    -1,    -1,
      -1,    -1,   196,   197,    -1,    -1,    -1,   394,   395,   396,
     397,   398,   399,   400,    -1,    -1,   403,   187,   405,    -1,
      -1,    -1,    -1,   217,    -1,    -1,   196,   197,    -1,   267,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   309,    -1,
      -1,    -1,   267,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   309,    -1,   267,   394,   395,   396,   397,   398,   399,
     400,    -1,    -1,   403,   309,   405,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,   267,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   309,    -1,    -1,    -1,    75,
      -1,    -1,    -1,   394,   395,   396,   397,   398,   399,   400,
      -1,    -1,   403,    -1,   405,    -1,    -1,    -1,    -1,   309,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    75,   403,    -1,   405,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,    -1,   403,    -1,
     405,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     394,   395,   396,   397,   398,   399,   400,    -1,    -1,   403,
      -1,   405,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,   145,    -1,   394,   395,   396,   397,   398,   399,
     400,   187,   155,   403,   145,   405,    -1,    -1,    -1,    -1,
     196,   197,    -1,    -1,   155,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,    -1,   187,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,   267,    75,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,   309,    -1,    -1,   267,    -1,   155,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,   145,    -1,    -1,    -1,   309,    -1,    -1,    -1,
     187,    -1,   155,    -1,    -1,    -1,    -1,    -1,   309,   196,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,   187,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,    -1,    -1,    -1,   394,   395,
     396,   397,   398,   399,   400,    -1,    -1,   403,    -1,   405,
      -1,   145,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     267,   394,   395,   396,   397,   398,   399,   400,    -1,    -1,
     403,    -1,   405,   394,   395,   396,   397,   398,   399,   400,
      -1,    -1,   403,   187,   405,    -1,   145,    -1,    -1,    -1,
      -1,    -1,   196,   197,   267,    -1,   155,    -1,    -1,    -1,
      -1,    -1,   309,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   309,   196,   197,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,   267,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,   395,   396,
     397,   398,   399,   400,    -1,    -1,   403,    -1,   405,    -1,
      -1,    -1,    -1,    -1,    -1,   309,    75,    -1,   267,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   394,   395,   396,   397,   398,   399,   400,    -1,    -1,
     403,    -1,   405,    75,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
     309,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
     394,   395,   396,   397,   398,   399,   400,    -1,    -1,   403,
      -1,   405,    -1,   145,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,
      75,    -1,    -1,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,    -1,    -1,   403,   187,   405,    -1,   217,    -1,
      -1,    -1,    -1,    -1,   196,   197,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    75,    -1,   267,    -1,   196,   197,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
     309,    -1,   187,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,   196,   197,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   309,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    75,    -1,   267,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,   267,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,    -1,    -1,   403,   187,   405,    -1,    -1,    -1,
     309,    -1,    -1,    -1,   196,   197,    -1,    -1,    -1,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,    -1,
      -1,   403,    -1,   405,   309,   217,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    -1,   267,    -1,   196,   197,    -1,
      -1,    -1,    -1,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,   187,    -1,   403,    -1,   405,    -1,   217,    -1,
      -1,   196,   197,    75,    -1,    -1,    -1,    -1,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,   309,   403,    -1,
     405,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   267,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     309,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,    -1,
      -1,   403,    -1,   405,   309,   187,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,   394,   395,   396,   397,   398,
     399,   400,    -1,    -1,   403,    -1,   405,   145,    -1,    -1,
      -1,    -1,    75,    -1,    -1,   267,    -1,   155,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,    -1,   403,    -1,
     405,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,   187,
      -1,    75,    -1,    -1,    -1,    -1,    -1,   309,   196,   197,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    75,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,   145,    -1,    -1,   187,    -1,    -1,    -1,    -1,   267,
      75,   155,    -1,   196,   197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,   187,
      -1,   403,    -1,   405,   217,    -1,    -1,    -1,   196,   197,
      -1,    75,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   309,   196,   197,    -1,   145,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    -1,   267,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   187,   267,    -1,    -1,   309,   217,    -1,    -1,
      -1,   196,   197,    -1,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    -1,   403,    -1,   405,    -1,    -1,
      -1,   309,   217,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,    -1,   309,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,   267,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   394,   395,   396,   397,   398,   399,   400,    -1,   309,
     403,    -1,   405,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,   267,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,   309,   403,    -1,   405,    -1,    -1,
     394,   395,   396,   397,   398,   399,   400,    -1,    -1,   403,
      -1,   405,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   309,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,   145,    -1,   394,   395,   396,   397,   398,   399,
     400,    -1,   155,   403,    -1,   405,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,    -1,   403,    -1,
     405,    -1,    75,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,    75,    -1,    -1,    -1,    -1,
     394,   395,   396,   397,   398,   399,   400,    -1,    -1,   403,
      -1,   405,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,   155,    23,   267,   145,    -1,    -1,    28,    -1,
      30,    -1,    -1,    -1,    -1,   155,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   196,   197,    -1,   309,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,    78,    79,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    92,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,    -1,
      -1,   394,   395,   396,   397,   398,   399,   400,    -1,    -1,
     403,    -1,   405,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,   309,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   309,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
      -1,    -1,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   211,   212,   213,    -1,    -1,    -1,    -1,    -1,   219,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   246,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   255,    -1,    -1,    -1,    -1,
      -1,   394,   395,   396,   397,   398,   399,   400,    -1,    -1,
     403,   271,   405,    -1,   394,   395,   396,   397,   398,   399,
     400,    -1,    -1,   403,    -1,   405,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,    37,
      -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,    57,
      58,    59,    -1,   343,    62,    63,    64,    -1,    66,    -1,
      68,    69,    -1,   353,    72,    73,   356,    -1,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    -1,
      88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
     108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,   117,
     118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,   127,
      -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,   137,
      -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,   147,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,   157,
      -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,   167,
     168,   169,    -1,   171,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,   186,    -1,
     188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,   213,    -1,    -1,    -1,    -1,
     218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,   227,
     228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,   237,
      -1,    -1,   240,    -1,    -1,    -1,    -1,   245,   246,    -1,
      -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,    -1,
     258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,    -1,
     268,    -1,    -1,    -1,   272,    -1,   274,   275,    -1,    -1,
     278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,   297,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   340,   341,    -1,   343,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,   387,
      -1,   389,    -1,   391,   392,   393,    -1,    -1,    -1,    -1,
     398,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    29,    30,    31,    -1,
      -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    55,    56,    57,    58,    59,    -1,    -1,    62,
      63,    64,    -1,    66,    -1,    68,    69,    -1,    -1,    72,
      73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    -1,    88,    -1,    -1,    91,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,    -1,
      -1,   114,   115,    -1,   117,   118,   119,   120,    -1,    -1,
      -1,    -1,   125,    -1,   127,    -1,   129,    -1,    -1,    -1,
     133,    -1,    -1,   136,   137,    -1,    -1,   140,    -1,    -1,
     143,    -1,    -1,   146,   147,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,    -1,   157,    -1,   159,    -1,    -1,    -1,
     163,   164,   165,   166,   167,   168,   169,    -1,   171,   172,
      -1,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,   186,    -1,   188,    -1,    -1,   191,    -1,
     193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,   206,   207,   208,   209,   210,   211,   212,
     213,    -1,    -1,    -1,    -1,   218,    -1,   220,    -1,   222,
     223,    -1,    -1,   226,   227,   228,    -1,   230,   231,   232,
      -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,    -1,
      -1,    -1,   245,   246,    -1,    -1,    -1,    -1,    -1,    -1,
     253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,   262,
      -1,    -1,    -1,   266,    -1,   268,    -1,    -1,    -1,   272,
      -1,   274,   275,    -1,    -1,   278,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,   292,
      -1,   294,   295,   296,   297,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   307,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,   352,
     353,   354,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,
     363,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   379,    -1,   381,    -1,
      -1,   384,    -1,    -1,   387,    -1,   389,    -1,   391,   392,
     393,    -1,    -1,    -1,    -1,   398,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    29,    30,    31,    -1,    -1,    -1,    35,    -1,    37,
      -1,    39,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    55,    56,    57,
      58,    59,    -1,    -1,    62,    63,    64,    -1,    66,    -1,
      68,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    84,    85,    -1,    -1,
      88,    -1,    -1,    91,    92,    -1,    94,    -1,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,   104,    -1,    -1,   107,
     108,    -1,   110,   111,    -1,    -1,   114,   115,    -1,   117,
     118,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,   127,
      -1,   129,    -1,    -1,    -1,   133,    -1,    -1,   136,   137,
      -1,    -1,   140,    -1,    -1,   143,    -1,    -1,   146,   147,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,   157,
      -1,   159,    -1,    -1,    -1,   163,   164,   165,   166,   167,
     168,   169,    -1,   171,   172,    -1,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,   186,    -1,
     188,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,   213,    -1,    -1,    -1,    -1,
     218,    -1,   220,    -1,   222,   223,    -1,    -1,   226,   227,
     228,    -1,   230,   231,   232,    -1,    -1,   235,    -1,   237,
      -1,    -1,   240,    -1,    -1,    -1,    -1,   245,   246,    -1,
      -1,    -1,    -1,    -1,    -1,   253,   254,    -1,    -1,    -1,
     258,    -1,    -1,    -1,   262,    -1,    -1,    -1,   266,    -1,
     268,    -1,    -1,    -1,   272,    -1,   274,   275,    -1,    -1,
     278,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,   292,    -1,   294,   295,   296,   297,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   340,   341,    -1,   343,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,    -1,   363,   364,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   379,    -1,   381,    -1,    -1,   384,    -1,    -1,   387,
      -1,   389,    -1,   391,   392,   393,    -1,    -1,    -1,    -1,
     398,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    29,    30,    31,    -1,
      -1,    -1,    35,    -1,    37,    -1,    39,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    55,    56,    57,    58,    59,    -1,    -1,    62,
      63,    64,    -1,    66,    -1,    68,    69,    -1,    -1,    72,
      73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    84,    85,    -1,    -1,    88,    -1,    -1,    91,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,    -1,
      -1,   114,   115,    -1,   117,   118,   119,   120,    -1,    -1,
      -1,    -1,   125,    -1,   127,    -1,   129,    -1,    -1,    -1,
     133,    -1,    -1,   136,   137,    -1,    -1,   140,    -1,    -1,
     143,    -1,    -1,   146,   147,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,    -1,   157,    -1,   159,    -1,    -1,    -1,
     163,   164,   165,   166,   167,   168,   169,    -1,   171,   172,
      -1,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,   186,    -1,   188,    -1,    -1,   191,    -1,
     193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,   206,   207,   208,   209,   210,   211,   212,
     213,    -1,    -1,    -1,    -1,   218,    -1,   220,    -1,   222,
     223,    -1,    -1,   226,   227,   228,    -1,   230,   231,   232,
      -1,    -1,   235,    -1,   237,    -1,    -1,   240,    -1,    -1,
      -1,    -1,   245,   246,    -1,    -1,    -1,    -1,    -1,    -1,
     253,   254,    -1,    -1,    -1,   258,    -1,    -1,    -1,   262,
      -1,    -1,    -1,   266,    -1,   268,    -1,    -1,    -1,   272,
      -1,   274,   275,    -1,    -1,   278,   279,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,   290,    -1,   292,
      -1,   294,   295,   296,   297,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   307,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
     323,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,    -1,    -1,    75,    -1,    -1,    -1,    -1,   351,   352,
     353,   354,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,
     363,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    -1,    -1,    -1,   379,    -1,   381,    -1,
      -1,   384,    -1,    -1,   387,    -1,   389,    75,   391,   392,
     393,    -1,    -1,    -1,    -1,   398,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,   187,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,   196,   197,    -1,   155,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,   187,    -1,   217,    -1,    -1,    -1,    -1,
      75,    -1,   196,   197,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    75,   267,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     145,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   309,    -1,   267,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,   321,
     322,   323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   187,    -1,   145,   309,   338,   339,   340,    75,
      -1,   196,   197,    -1,   155,    -1,    -1,    -1,   350,   351,
      -1,   309,   354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   363,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,   380,   381,
      -1,    -1,    -1,    -1,    -1,   196,   197,    -1,    -1,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,    -1,
      -1,   403,    -1,    -1,    -1,    -1,   217,    -1,    -1,   145,
      -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     394,   395,   396,   397,   398,   399,   400,   385,    -1,   403,
      -1,    -1,    -1,    -1,    -1,    -1,   394,   395,   396,   397,
     398,   399,   400,    -1,    -1,   403,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,   309,    -1,   267,    -1,    -1,    -1,
     196,   197,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,   309,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
     385,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,
     395,   396,   397,   398,   399,   400,    -1,    -1,   403,    -1,
      -1,    -1,    -1,    75,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   309,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,   394,   395,   396,   397,   398,   399,   400,
      -1,    -1,   403,    -1,    -1,    -1,    -1,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    75,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,   145,    -1,    -1,    -1,    -1,
     187,    -1,    -1,    -1,    -1,   155,   145,    -1,    -1,   196,
     197,    -1,    -1,    -1,    -1,    -1,   155,    -1,   394,   395,
     396,   397,   398,   399,   400,   187,    -1,   403,    -1,    -1,
     217,    -1,    -1,    -1,   196,   197,    75,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   187,    -1,
      -1,   145,    -1,    -1,    -1,   217,    -1,   196,   197,    -1,
      -1,   155,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
     267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   197,    -1,   267,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   267,    -1,    -1,
      -1,    -1,   309,   217,    -1,    -1,    -1,    -1,   267,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   309,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,   309,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     309,    -1,    -1,   267,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,   395,   396,
     397,   398,   399,   400,    -1,   309,   403,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,
      -1,    -1,   394,   395,   396,   397,   398,   399,   400,    -1,
      -1,   403,    -1,    -1,   394,   395,   396,   397,   398,   399,
     400,    -1,    -1,   403,    -1,   394,   395,   396,   397,   398,
     399,   400,    -1,    -1,   403,    -1,    -1,    -1,    -1,    -1,
     309,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     394,   395,   396,   397,   398,   399,   400,    -1,    -1,   403,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,   394,   395,   396,   397,   398,
     399,   400,    22,    -1,   403,    25,    26,    27,    -1,    29,
      30,    31,    -1,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    -1,    66,    -1,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,    -1,   188,    -1,
      -1,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,   242,    -1,    -1,   245,   246,    -1,    -1,    -1,
      -1,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,    -1,   272,    -1,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,   341,    -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,   352,   353,   354,    -1,   356,    -1,    -1,    -1,
      -1,    -1,    -1,   363,   364,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,
      -1,   381,    -1,    -1,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    -1,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    -1,    66,    -1,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
      -1,    -1,    -1,   133,    -1,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,    -1,   153,   154,    -1,    -1,   157,    -1,   159,
      -1,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,    -1,   188,    -1,
      -1,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,   242,    -1,    -1,   245,   246,    -1,    -1,    -1,
      -1,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,    -1,   272,    -1,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,   341,    -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,   352,   353,   354,    -1,   356,    -1,    -1,    -1,
      -1,    -1,    -1,   363,   364,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,
      -1,   381,    -1,    -1,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    -1,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    -1,    66,    -1,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    76,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
      -1,    -1,    -1,   133,    -1,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,    -1,   188,    -1,
      -1,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,   242,    -1,    -1,   245,   246,    -1,    -1,    -1,
      -1,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,    -1,   272,    -1,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,   341,    -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,   352,   353,   354,    -1,   356,    -1,    -1,    -1,
      -1,    -1,    -1,   363,   364,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,
      -1,   381,    -1,    -1,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    -1,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    -1,    66,    -1,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,   124,   125,    -1,   127,    -1,   129,
      -1,    -1,    -1,   133,    -1,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,    -1,   188,    -1,
      -1,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,    -1,    -1,    -1,   245,   246,    -1,    -1,    -1,
      -1,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,    -1,   272,    -1,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,   341,    -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,   352,   353,   354,    -1,   356,    -1,    -1,    -1,
      -1,    -1,    -1,   363,   364,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,
      -1,   381,    -1,    -1,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    -1,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    -1,    66,    -1,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
      -1,    -1,    -1,   133,    -1,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,    -1,   188,    -1,
      -1,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,   242,    -1,    -1,   245,   246,    -1,    -1,    -1,
      -1,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,    -1,   272,    -1,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,   341,    -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,   352,   353,   354,    -1,   356,    -1,    -1,    -1,
      -1,    -1,    -1,   363,   364,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,
      -1,   381,    -1,    -1,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    -1,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    -1,    66,    -1,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
      -1,    -1,    -1,   133,    -1,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,    -1,   188,    -1,
      -1,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,   204,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,    -1,    -1,    -1,   245,   246,    -1,    -1,    -1,
      -1,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,    -1,   272,    -1,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,   341,    -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,   352,   353,   354,    -1,   356,    -1,    -1,    -1,
      -1,    -1,    -1,   363,   364,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,
      -1,   381,    -1,    -1,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    -1,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    -1,    66,    -1,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
      -1,    -1,    -1,   133,    -1,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,    -1,   188,    -1,
      -1,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,    -1,    -1,    -1,   245,   246,    -1,    -1,    -1,
      -1,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    -1,   268,    -1,
      -1,    -1,   272,    -1,   274,   275,    -1,    -1,   278,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,    -1,   294,   295,   296,   297,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,   341,    -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,   352,   353,   354,    -1,   356,    -1,    -1,    -1,
      -1,    -1,    -1,   363,   364,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,
      -1,   381,    -1,    -1,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    29,
      30,    31,    -1,    -1,    -1,    35,    -1,    37,    -1,    39,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      -1,    -1,    62,    63,    64,    -1,    66,    -1,    68,    69,
      -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    91,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,
      -1,   101,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,    -1,   114,   115,    -1,   117,   118,   119,
     120,    -1,    -1,    -1,    -1,   125,    -1,   127,    -1,   129,
      -1,    -1,    -1,   133,    -1,    -1,   136,   137,    -1,    -1,
     140,    -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,   157,    -1,   159,
      -1,    -1,    -1,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,   172,    -1,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   186,    -1,   188,    -1,
      -1,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,   222,   223,    -1,    -1,   226,   227,   228,    -1,
     230,   231,   232,    -1,    -1,   235,    -1,   237,    -1,    -1,
     240,    -1,    -1,    -1,    -1,   245,   246,    -1,    -1,    -1,
      -1,    -1,    -1,   253,   254,    -1,    -1,    -1,   258,    -1,
      -1,    -1,   262,    -1,    -1,    -1,   266,    23,   268,    -1,
      -1,    -1,   272,    -1,   274,   275,    -1,    33,   278,   279,
      36,    -1,    38,    -1,    40,    -1,    42,    -1,    -1,    -1,
     290,    47,   292,    -1,   294,   295,   296,   297,    -1,    55,
      -1,    -1,    -1,    -1,    60,    61,    -1,   307,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    71,    -1,    -1,    74,    -1,
      -1,    -1,    -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,   341,    -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,   352,   353,   354,    -1,   356,    -1,    -1,    -1,
      -1,    -1,   118,   363,   364,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,   379,
      -1,   381,    -1,   139,   384,    -1,    -1,   387,    -1,   389,
      -1,   391,   392,   393,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   257
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     3,     5,    23,    24,    26,    28,    31,    33,    36,
      37,    38,    41,    42,    44,    45,    47,    52,    53,    54,
      56,    57,    59,    60,    61,    62,    64,    66,    69,    70,
      71,    80,    93,   105,   106,   128,   219,   223,   225,   229,
     259,   360,   404,   413,   414,   415,   419,   485,   498,   499,
     503,   505,   507,   512,   514,   519,   521,   525,   526,   611,
     613,   619,   622,   646,   650,   661,   663,   673,   675,   677,
     683,   687,   689,   690,   691,   717,   729,   735,   736,   743,
     745,   767,   770,   771,   772,     4,     5,     6,     7,     8,
      22,    25,    26,    27,    29,    30,    31,    35,    37,    39,
      40,    41,    43,    46,    48,    51,    54,    55,    56,    57,
      58,    59,    62,    63,    64,    66,    68,    69,    72,    73,
      78,    79,    80,    81,    83,    84,    85,    88,    91,    92,
      94,    97,   101,   104,   107,   108,   110,   111,   114,   115,
     117,   118,   119,   120,   125,   127,   129,   133,   136,   137,
     140,   143,   146,   147,   153,   157,   159,   163,   164,   165,
     166,   167,   168,   169,   171,   172,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   185,   186,   188,   191,
     193,   201,   202,   206,   207,   208,   209,   210,   211,   212,
     213,   218,   220,   222,   223,   226,   227,   228,   230,   231,
     232,   235,   237,   240,   245,   246,   253,   254,   258,   262,
     266,   268,   272,   274,   275,   278,   279,   290,   292,   294,
     295,   296,   297,   307,   323,   340,   341,   343,   351,   352,
     353,   354,   356,   363,   364,   379,   381,   384,   387,   389,
     391,   392,   393,   411,   711,   712,   713,   716,   138,   494,
     238,   239,   731,   768,    48,    73,   126,   240,   256,   317,
     432,   433,   434,   444,   480,   651,   612,   139,   240,   253,
     317,   618,   678,   620,    25,    32,    42,    49,    50,    65,
      67,    69,    82,    90,    99,   121,   130,   134,   152,   158,
     160,   173,   187,   189,   190,   215,   242,   247,   248,   249,
     250,   271,   273,   275,   296,   297,   301,   308,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   324,   325,
     326,   328,   329,   330,   331,   332,   333,   334,   336,   337,
     340,   342,   344,   345,   346,   347,   348,   349,   351,   354,
     355,   356,   357,   359,   360,   361,   362,   363,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   381,   382,   383,   396,   401,   404,   407,   408,   410,
     411,   544,   545,   548,   550,   703,   705,   709,   713,   101,
     239,   731,   731,   688,   731,   684,   243,   713,   529,   530,
     664,    64,    66,    62,   245,    62,   239,   662,   731,   647,
     731,   731,   746,   731,   731,   744,   194,   719,   713,   623,
      60,     0,     3,   525,   711,   713,   713,   193,     4,   214,
     411,   239,   513,   379,   769,   243,   342,   435,   239,   433,
     253,   139,   162,   290,   659,   660,   102,   544,   644,   645,
     713,   713,   342,   617,   239,    48,    62,   107,   136,   159,
     205,   208,   222,   235,   238,   679,   680,   731,   103,   135,
     162,   625,   404,   404,   404,   404,   404,   404,   404,   404,
     544,   404,   404,   404,   404,   404,   404,   559,   560,   561,
     544,   404,   404,   404,   404,   404,   404,   703,   703,   703,
     404,   404,   404,   544,   563,   404,   404,   404,   543,   404,
     543,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   544,
     404,   404,   404,   404,   404,   404,   404,   404,   543,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   544,   544,   544,   544,   713,   154,   242,   410,
     713,   714,   713,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    75,   145,   155,   187,   196,   197,   217,
     267,   309,   394,   395,   396,   397,   398,   399,   400,   403,
     242,   411,    97,   124,   162,   694,   711,   730,   520,    48,
     508,    48,    62,   208,   685,   686,    59,   527,    74,   109,
     135,   236,   386,   387,   388,   389,   390,   391,   533,   534,
     535,    27,    33,    48,    62,   100,   125,   127,   129,   139,
     140,   143,   149,   157,   159,   185,   193,   231,   235,   238,
     239,   479,   665,   669,   722,    43,    63,   501,   502,   501,
     501,   500,   501,   616,   711,   162,   649,   506,   515,    23,
      33,    36,    38,    40,    42,    47,    55,    60,    61,    68,
      71,    74,   118,   128,   139,   206,   216,   218,   232,   257,
     747,   748,   749,   522,   504,   747,   718,   103,   626,   649,
     529,   674,   411,     9,    76,   583,   584,   737,   713,   711,
     615,   616,   416,   187,   713,   435,   713,   713,   124,   652,
     656,   657,   713,   659,   406,   192,   116,   713,   617,    88,
     264,   406,   681,   494,    74,   553,   585,   109,   553,   585,
     544,   553,   553,   553,   553,   544,   544,   544,   544,   544,
     544,   560,   307,   562,   709,   405,   544,   544,   544,   556,
     557,   556,   544,   553,   553,   378,   556,   544,   405,   544,
     405,   544,   544,   544,   544,   544,   544,   544,   544,   544,
     320,   321,   322,   323,   338,   339,   340,   350,   351,   354,
     363,   380,   381,   582,   544,   544,   544,   703,   544,   544,
     544,   582,   405,   544,   544,   544,   544,   544,   544,   544,
     544,   544,   546,   548,   405,   544,   544,   544,   544,   544,
     544,   544,    86,   151,   244,   544,   549,   556,   549,   549,
     549,   549,   549,   703,   405,   544,   405,   544,   544,   544,
     121,   160,   190,   215,   301,   602,   405,   544,   127,   157,
     231,   723,    19,   411,   544,   544,   544,   544,   544,   544,
     544,   187,   189,   544,   544,   544,   404,   548,   145,   155,
     217,   309,   544,   544,   544,   544,   547,   548,   544,   544,
     344,   544,   344,   544,   544,   544,   544,   544,   713,    48,
     157,   693,   124,   711,   732,   733,   615,   159,   615,    88,
     406,   713,   255,   773,   774,   398,   537,   538,   539,   535,
      39,   239,   159,   235,   136,   235,   155,   668,   335,   235,
      48,   238,   668,   124,   145,   667,   670,   235,   124,    95,
     207,   262,   406,   494,   615,   615,   240,   751,   238,    29,
      62,   750,   100,   752,   205,   194,   753,   192,   406,   523,
     524,   711,   615,   192,   127,   157,   231,   720,   721,   144,
     616,   627,   628,   405,   134,   242,   676,   704,   713,   713,
     713,     7,   119,   713,   738,   739,   486,   117,   178,   272,
     279,   290,   406,   516,   517,   518,   163,   164,   165,   166,
     167,   168,   169,   171,   172,   417,   418,   116,   711,   422,
     192,   656,   711,   713,   406,   654,   411,   658,   645,   711,
     615,   214,   680,   615,   682,   627,   405,   554,   551,   405,
     398,   406,   405,   405,   405,   405,   406,    76,   406,   406,
     406,   405,   406,   405,   404,   406,   405,   406,   406,   405,
     544,   558,   405,   405,   406,   405,   405,   565,   566,   405,
     406,   405,   406,   406,   406,   405,   406,   405,   406,   406,
     406,   405,   406,   406,   124,   406,   406,   405,   406,   406,
     405,   406,   406,   397,   405,   406,   406,   406,   406,   405,
     405,   405,   405,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    75,   145,   155,   187,   196,   197,   217,
     267,   309,   394,   395,   396,   397,   398,   399,   400,   403,
     405,   406,   406,   405,   406,   405,   124,   406,   406,   544,
     568,   568,   568,   124,   405,   405,   405,   405,   405,   405,
     405,   406,   405,   405,   406,   405,   405,   406,   406,   409,
     411,   411,   411,   714,   544,   713,   189,   556,   115,   589,
     404,   548,   544,   547,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    75,   145,   155,   187,   196,   197,
     217,   267,   309,   394,   395,   396,   397,   398,   399,   400,
     403,   544,   544,   411,   141,    48,   584,   406,   516,   243,
     117,   258,   290,   509,   510,   511,   686,    74,   109,   779,
     124,   144,   282,   406,   531,   532,   598,   609,   541,   544,
     707,   713,   145,   671,   711,   704,   714,   715,   335,   667,
     668,   713,   667,   711,   670,   668,   502,   404,   408,   569,
     574,   575,   243,   516,   238,   404,   760,   760,   760,   760,
     398,   711,   713,   757,   749,   406,   243,   124,   757,   406,
     245,   271,   356,   410,   713,   724,   725,   628,   624,   528,
       6,     7,     8,     9,    11,    12,    13,    14,   119,   740,
     742,   263,   586,    21,    23,    28,    30,    38,    78,    79,
      91,    92,   101,   104,   108,   111,   139,   174,   181,   198,
     202,   211,   212,   213,   219,   227,   246,   255,   271,   343,
     353,   356,   437,   438,   487,   488,   489,   594,   616,   517,
       9,     9,     9,     9,     9,     9,     9,     9,     9,   406,
     420,   251,   711,   655,   653,   411,   657,   124,   398,   713,
     614,    89,   224,   495,    47,   621,   544,   556,   405,   544,
     544,    82,   271,   274,   275,   292,   297,   302,   555,   555,
     556,   556,   544,   544,   709,   544,   544,   406,   544,   327,
     564,   544,   567,   556,   344,   344,   242,   544,   544,   556,
     242,   544,   544,   544,   556,   190,   544,   190,   544,   556,
     544,   544,   544,   556,   544,   544,   544,   544,   544,   544,
     544,   544,   187,   189,   544,   544,   544,   544,   548,   155,
     217,   309,   544,   544,   544,   544,   547,   544,   544,   344,
     544,   344,   544,   544,   544,   544,   544,   544,   544,   544,
     544,   544,   544,   124,   124,   124,   544,   190,   544,   544,
     544,   405,   242,   556,   589,    75,   544,   544,   544,   544,
     544,   544,   544,   187,   189,   544,   544,   544,   404,   548,
     145,   155,   217,   309,   544,   544,   544,   544,   547,   544,
     544,   344,   544,   344,   544,   544,   544,   544,   544,   582,
     582,   713,   242,   162,   214,   265,   734,   733,   242,   510,
     775,   569,   200,   201,   599,   538,    47,   335,   536,   609,
     532,   540,   411,   242,   124,   672,   410,    62,   668,   668,
     667,   711,   569,   713,    34,   142,   148,   184,   229,   236,
     346,   361,   406,   573,   711,   242,   713,   761,   762,   411,
     411,   243,   524,   711,   242,   124,   721,   146,   229,     9,
      19,   335,   639,   410,   714,   639,    60,   229,   261,   404,
     429,   629,   633,   774,   776,   777,   778,   741,   544,   598,
      96,    98,   126,   139,   150,   449,   451,   477,   478,   493,
     493,   493,     9,   123,   204,   478,   493,     9,     9,   639,
     640,     9,   114,     9,   149,   149,   114,     9,     9,    87,
       9,     9,     9,     9,     9,    76,   243,   497,     9,     9,
       9,   229,     9,   493,     9,   437,   406,   404,   411,   448,
     452,   710,   713,   242,   242,   242,   121,   160,   190,   215,
     301,   603,   242,   602,   602,   242,   602,   418,   404,   423,
     436,   437,   281,   291,   294,   445,   421,   260,   586,   713,
     569,   411,   658,   629,   552,   406,   405,   281,   281,   405,
     405,   405,   405,   405,   145,   405,   405,   406,   544,   405,
     544,   384,   385,   378,   405,   544,   544,   405,   405,   405,
     405,   405,   405,   406,   405,   405,   405,   405,   406,   406,
     405,   405,   405,   406,   405,   405,   406,   189,   405,   589,
     548,   544,   547,    75,   544,   544,   406,   405,   405,   335,
     405,   405,   406,   406,   544,   544,   544,   405,   406,   405,
     405,   405,   405,   544,   189,   556,   589,   404,   548,   544,
     547,    75,   544,   544,   692,   265,   157,   526,   586,   242,
     242,   600,   602,   145,    71,    76,   242,   542,   713,   398,
     713,   603,   666,   714,   264,   667,   405,   574,   148,   148,
     148,   346,   361,   648,   709,   569,   199,   576,   576,   569,
     569,   584,   405,   406,   398,   398,   715,   758,   759,   758,
     724,   153,   640,   715,   242,   356,   727,   723,   639,    74,
     102,   192,   544,   728,   430,   630,   404,   636,   641,   405,
     429,   632,   706,   707,   709,   634,   593,   594,   404,   483,
     710,   478,    93,   123,   204,   256,   450,   483,   710,   710,
     242,   150,   150,   710,   710,   603,   602,   713,   602,     9,
     602,     9,   603,   603,   595,   102,   602,   209,   210,   441,
     602,   602,   602,   711,   102,   110,   120,   268,   440,    81,
     133,   143,   147,   179,   180,   182,   439,   404,   640,     6,
     119,   188,   442,   710,   242,   489,   446,   447,   448,   449,
     713,    22,   119,   496,   216,   450,   472,   453,   411,   424,
     429,   446,   138,   360,   426,   695,   252,   404,   569,   593,
     586,   405,   544,    85,   544,   544,   544,   582,   582,   544,
     190,   544,   544,   544,   589,    75,   544,   582,   582,   544,
     544,   544,   544,   405,   405,   405,   190,   405,   556,   589,
      75,   544,   582,   582,   695,   131,   590,   610,   191,   406,
     364,   242,   713,   411,   598,   165,   668,   346,   569,   576,
     576,   406,   586,   639,   148,   148,   192,   260,   122,   138,
     259,   577,   762,   341,   221,   406,   763,   214,   220,   230,
     726,   728,   639,   404,   725,   544,   533,   637,   638,   709,
     642,   406,   404,   633,   405,   405,   406,   773,   598,   644,
     544,   150,   150,   478,   404,    38,   229,   490,   483,   495,
     242,   242,   544,   596,   708,   615,   713,   491,   405,   406,
     713,   711,    82,    83,    84,   161,   183,   186,   229,   269,
     270,   271,   274,   275,   276,   277,   278,   280,   281,   283,
     284,   285,   286,   287,   288,   291,   293,   295,   296,   297,
     298,   299,   300,   303,   304,   381,   454,   457,   458,   459,
     460,   713,   405,   405,    76,   431,   242,   481,   482,   713,
     586,   282,   601,   406,   352,   405,   385,   405,   405,   405,
     406,   406,   405,   405,   405,   544,   405,   405,   405,   405,
     406,   405,   544,   144,    87,   132,   587,    95,   696,   602,
     602,   352,   398,     9,   199,   148,   148,   709,   593,   544,
     569,   569,   544,   570,   478,   578,   578,   578,    87,    51,
     233,   237,   266,   392,   393,   755,   756,   759,   264,   764,
      94,   254,   214,   727,   242,   639,   537,   406,   639,   643,
     644,   641,   635,   633,   706,   405,   483,   481,   102,   102,
     452,   406,    77,   105,   597,   405,   454,   447,   173,   192,
     404,   473,   474,   475,   404,   404,   466,   303,   457,   458,
     271,   304,   304,   456,   404,   461,   462,   289,   455,   461,
     461,   404,   404,   466,    30,    78,   102,   187,   189,   204,
     256,   468,   469,   470,    82,   271,   305,   404,   471,   404,
     466,   404,   462,   467,   425,   436,   404,   429,   405,   406,
     597,   404,   603,   544,   405,   544,   544,   553,   556,   239,
     591,   708,   588,   593,   112,   113,   195,   241,   697,   698,
     156,   699,   242,   148,   569,   569,   639,   601,   192,   571,
     192,   572,   404,   579,   242,   356,   242,   242,   242,    75,
     754,   128,   175,   176,   177,   765,   766,   405,   728,   443,
     532,   638,   645,   405,   776,   404,   405,   275,   296,   297,
     705,   496,   708,   468,   125,   203,    36,    71,   481,   475,
     190,   190,   404,   190,   292,   302,   306,   463,   464,   465,
     404,   463,   463,   190,   190,   463,   703,   705,   189,   150,
     150,   470,   229,   190,   190,   463,   190,   463,   776,   426,
     429,   427,   482,   190,   405,   405,   406,   405,   405,   711,
     264,   406,   592,   597,   544,   598,    87,    87,   112,    87,
     698,   234,   241,   700,   701,    75,   574,   544,   544,   260,
     544,   260,   581,   713,   404,   242,   755,   194,   602,   602,
     602,   766,   536,   481,   597,   492,    89,   188,   224,   229,
     476,   476,   405,   405,   405,   484,   704,   405,   406,   465,
     484,   405,   405,   640,   405,   405,   405,   773,   597,   405,
     544,   696,    35,    58,   708,   358,   604,   704,   704,    87,
     704,    87,    87,   701,   166,   192,   404,   404,   405,   406,
     204,   580,   713,   405,   496,    72,   102,   189,   473,   405,
     406,   190,   405,   713,   471,   471,   428,   405,   699,   597,
     713,   704,   704,   704,     9,   544,   581,   581,   713,   405,
     406,   472,   704,   405,   776,   138,   702,   605,   603,   409,
     405,   405,   713,   190,   404,   631,   404,    75,   156,   405,
     632,   539,   606,   607,   608,   170,   405,   544,   405,   406,
       9,   608,   602
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
#define YYEMPTY		-2
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

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
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)           \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX	yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX	yylex (&yylval)
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
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
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

#if YYMAXDEPTH == 0
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
/*-----------------------------.
| Print this symbol on YYOUT.  |
`-----------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yysymprint (FILE* yyout, int yytype, YYSTYPE yyvalue)
#else
yysymprint (yyout, yytype, yyvalue)
    FILE* yyout;
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyout, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyout, yytoknum[yytype], yyvalue);
# endif
    }
  else
    YYFPRINTF (yyout, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyout, ")");
}
#endif /* YYDEBUG. */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yydestruct (int yytype, YYSTYPE yyvalue)
#else
yydestruct (yytype, yyvalue)
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  switch (yytype)
    {
      default:
        break;
    }
}



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif




int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of parse errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

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

  if (yyssp >= yyss + yystacksize - 1)
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
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
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

      if (yyssp >= yyss + yystacksize - 1)
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

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with.  */

  if (yychar <= 0)		/* This means end of input.  */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more.  */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

      /* We have to keep this `#if YYDEBUG', since we use variables
	 which are defined only if `YYDEBUG' is set.  */
      YYDPRINTF ((stderr, "Next token is "));
      YYDSYMPRINT ((stderr, yychar1, yylval));
      YYDPRINTF ((stderr, "\n"));
    }

  /* If the proper action on seeing token YYCHAR1 is to reduce or to
     detect an error, take that action.  */
  yyn += yychar1;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yychar1)
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
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

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



#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn - 1, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] >= 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif
  switch (yyn)
    {
        case 2:
#line 611 "sql_yacc.yy"
    {
	   THD *thd=current_thd;
	   if (!thd->bootstrap &&
	      (!(thd->lex.select_lex.options & OPTION_FOUND_COMMENT)))
	   {
	     send_error(&current_thd->net,ER_EMPTY_QUERY);
	     YYABORT;
 	   }
	   else
	   {
	     thd->lex.sql_command = SQLCOM_EMPTY_QUERY;
	   }
	}
    break;

  case 3:
#line 624 "sql_yacc.yy"
    {}
    break;

  case 42:
#line 670 "sql_yacc.yy"
    {
	  LEX *lex = Lex;
	  lex->sql_command = SQLCOM_CHANGE_MASTER;
	  bzero((char*) &lex->mi, sizeof(lex->mi));
        }
    break;

  case 43:
#line 676 "sql_yacc.yy"
    {}
    break;

  case 46:
#line 685 "sql_yacc.yy"
    {
	 Lex->mi.host = yyvsp[0].lex_str.str;
       }
    break;

  case 47:
#line 690 "sql_yacc.yy"
    {
	 Lex->mi.user = yyvsp[0].lex_str.str;
       }
    break;

  case 48:
#line 695 "sql_yacc.yy"
    {
	 Lex->mi.password = yyvsp[0].lex_str.str;
       }
    break;

  case 49:
#line 700 "sql_yacc.yy"
    {
	 Lex->mi.log_file_name = yyvsp[0].lex_str.str;
       }
    break;

  case 50:
#line 705 "sql_yacc.yy"
    {
	 Lex->mi.port = yyvsp[0].ulong_num;
       }
    break;

  case 51:
#line 710 "sql_yacc.yy"
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

  case 52:
#line 727 "sql_yacc.yy"
    {
	 Lex->mi.connect_retry = yyvsp[0].ulong_num;
       }
    break;

  case 53:
#line 732 "sql_yacc.yy"
    {
	 Lex->mi.relay_log_name = yyvsp[0].lex_str.str;
       }
    break;

  case 54:
#line 737 "sql_yacc.yy"
    {
	 Lex->mi.relay_log_pos = yyvsp[0].ulong_num;
         /* Adjust if < BIN_LOG_HEADER_SIZE (same comment as Lex->mi.pos) */
         Lex->mi.relay_log_pos = max(BIN_LOG_HEADER_SIZE, Lex->mi.relay_log_pos);
       }
    break;

  case 55:
#line 748 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_CREATE_TABLE;
	  if (!add_table_to_list(yyvsp[0].table,
				 (yyvsp[-3].num & HA_LEX_CREATE_TMP_TABLE ?
				   &tmp_table_alias : (LEX_STRING*) 0),
				 TL_OPTION_UPDATING))
	    YYABORT;
	  lex->create_list.empty();
	  lex->key_list.empty();
	  lex->col_list.empty();
	  lex->change=NullS;
	  bzero((char*) &lex->create_info,sizeof(lex->create_info));
	  lex->create_info.options=yyvsp[-3].num | yyvsp[-1].num;
	  lex->create_info.db_type= (enum db_type) lex->thd->variables.table_type;
	}
    break;

  case 56:
#line 765 "sql_yacc.yy"
    {Lex->select= &Lex->select_lex;}
    break;

  case 57:
#line 767 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_CREATE_INDEX;
	    if (!add_table_to_list(yyvsp[0].table, NULL, TL_OPTION_UPDATING))
	      YYABORT;
	    lex->create_list.empty();
	    lex->key_list.empty();
	    lex->col_list.empty();
	    lex->change=NullS;
	  }
    break;

  case 58:
#line 778 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->key_list.push_back(new Key(yyvsp[-8].key_type,yyvsp[-6].lex_str.str,lex->col_list));
	    lex->col_list.empty();
	  }
    break;

  case 59:
#line 784 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command=SQLCOM_CREATE_DB;
	    lex->name=yyvsp[0].lex_str.str;
            lex->create_info.options=yyvsp[-1].num;
	  }
    break;

  case 60:
#line 791 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command = SQLCOM_CREATE_FUNCTION;
	    lex->udf.name=yyvsp[0].lex_str.str;
	    lex->udf.name_length=yyvsp[0].lex_str.length;
	    lex->udf.type= yyvsp[-2].udf_type;
	  }
    break;

  case 61:
#line 799 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->udf.returns=(Item_result) yyvsp[-2].num;
	    lex->udf.dl=yyvsp[0].lex_str.str;
	  }
    break;

  case 62:
#line 806 "sql_yacc.yy"
    {}
    break;

  case 63:
#line 807 "sql_yacc.yy"
    {}
    break;

  case 64:
#line 810 "sql_yacc.yy"
    {}
    break;

  case 65:
#line 811 "sql_yacc.yy"
    { Select->braces= 1;}
    break;

  case 66:
#line 811 "sql_yacc.yy"
    {}
    break;

  case 67:
#line 815 "sql_yacc.yy"
    {}
    break;

  case 68:
#line 817 "sql_yacc.yy"
    { Select->braces= 0;}
    break;

  case 69:
#line 817 "sql_yacc.yy"
    {}
    break;

  case 70:
#line 819 "sql_yacc.yy"
    { Select->braces= 1;}
    break;

  case 71:
#line 819 "sql_yacc.yy"
    {}
    break;

  case 72:
#line 824 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->lock_option= (using_update_log) ? TL_READ_NO_INSERT : TL_READ;
	    if (lex->sql_command == SQLCOM_INSERT)
	      lex->sql_command= SQLCOM_INSERT_SELECT;
	    else if (lex->sql_command == SQLCOM_REPLACE)
	      lex->sql_command= SQLCOM_REPLACE_SELECT;
	    lex->select->table_list.save_and_clear(&lex->save_list);
	    mysql_init_select(lex);
          }
    break;

  case 73:
#line 835 "sql_yacc.yy"
    { Lex->select->table_list.push_front(&Lex->save_list); }
    break;

  case 74:
#line 839 "sql_yacc.yy"
    {}
    break;

  case 75:
#line 840 "sql_yacc.yy"
    {}
    break;

  case 76:
#line 843 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 77:
#line 844 "sql_yacc.yy"
    { yyval.num= yyvsp[0].num;}
    break;

  case 78:
#line 847 "sql_yacc.yy"
    { yyval.num=yyvsp[0].num; }
    break;

  case 79:
#line 848 "sql_yacc.yy"
    { yyval.num= yyvsp[-1].num | yyvsp[0].num; }
    break;

  case 80:
#line 851 "sql_yacc.yy"
    { yyval.num=HA_LEX_CREATE_TMP_TABLE; }
    break;

  case 81:
#line 854 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 82:
#line 855 "sql_yacc.yy"
    { yyval.num=HA_LEX_CREATE_IF_NOT_EXISTS; }
    break;

  case 87:
#line 866 "sql_yacc.yy"
    { Lex->create_info.db_type= yyvsp[0].db_type; }
    break;

  case 88:
#line 867 "sql_yacc.yy"
    { Lex->create_info.max_rows= yyvsp[0].ulonglong_number; Lex->create_info.used_fields|= HA_CREATE_USED_MAX_ROWS;}
    break;

  case 89:
#line 868 "sql_yacc.yy"
    { Lex->create_info.min_rows= yyvsp[0].ulonglong_number; Lex->create_info.used_fields|= HA_CREATE_USED_MIN_ROWS;}
    break;

  case 90:
#line 869 "sql_yacc.yy"
    { Lex->create_info.avg_row_length=yyvsp[0].ulong_num; Lex->create_info.used_fields|= HA_CREATE_USED_AVG_ROW_LENGTH;}
    break;

  case 91:
#line 870 "sql_yacc.yy"
    { Lex->create_info.password=yyvsp[0].lex_str.str; }
    break;

  case 92:
#line 871 "sql_yacc.yy"
    { Lex->create_info.comment=yyvsp[0].lex_str.str; }
    break;

  case 93:
#line 872 "sql_yacc.yy"
    { Lex->create_info.auto_increment_value=yyvsp[0].ulonglong_number; Lex->create_info.used_fields|= HA_CREATE_USED_AUTO;}
    break;

  case 94:
#line 873 "sql_yacc.yy"
    { Lex->create_info.table_options|= yyvsp[0].ulong_num ? HA_OPTION_PACK_KEYS : HA_OPTION_NO_PACK_KEYS; Lex->create_info.used_fields|= HA_CREATE_USED_PACK_KEYS;}
    break;

  case 95:
#line 874 "sql_yacc.yy"
    { Lex->create_info.table_options&= ~(HA_OPTION_PACK_KEYS | HA_OPTION_NO_PACK_KEYS); Lex->create_info.used_fields|= HA_CREATE_USED_PACK_KEYS;}
    break;

  case 96:
#line 875 "sql_yacc.yy"
    { Lex->create_info.table_options|= yyvsp[0].ulong_num ? HA_OPTION_CHECKSUM : HA_OPTION_NO_CHECKSUM; }
    break;

  case 97:
#line 876 "sql_yacc.yy"
    { Lex->create_info.table_options|= yyvsp[0].ulong_num ? HA_OPTION_DELAY_KEY_WRITE : HA_OPTION_NO_DELAY_KEY_WRITE; }
    break;

  case 98:
#line 877 "sql_yacc.yy"
    { Lex->create_info.row_type= yyvsp[0].row_type; }
    break;

  case 99:
#line 878 "sql_yacc.yy"
    { Lex->create_info.raid_type= yyvsp[0].ulong_num; Lex->create_info.used_fields|= HA_CREATE_USED_RAID;}
    break;

  case 100:
#line 879 "sql_yacc.yy"
    { Lex->create_info.raid_chunks= yyvsp[0].ulong_num; Lex->create_info.used_fields|= HA_CREATE_USED_RAID;}
    break;

  case 101:
#line 880 "sql_yacc.yy"
    { Lex->create_info.raid_chunksize= yyvsp[0].ulong_num*RAID_BLOCK_SIZE; Lex->create_info.used_fields|= HA_CREATE_USED_RAID;}
    break;

  case 102:
#line 882 "sql_yacc.yy"
    {
	    /* Move the union list to the merge_list */
	    LEX *lex=Lex;
	    TABLE_LIST *table_list= (TABLE_LIST*) lex->select->table_list.first;
	    lex->create_info.merge_list= lex->select->table_list;
	    lex->create_info.merge_list.elements--;
	    lex->create_info.merge_list.first= (byte*) (table_list->next);
	    lex->select->table_list.elements=1;
	    lex->select->table_list.next= (byte**) &(table_list->next);
	    table_list->next=0;
	    lex->create_info.used_fields|= HA_CREATE_USED_UNION;
	  }
    break;

  case 103:
#line 894 "sql_yacc.yy"
    {}
    break;

  case 104:
#line 895 "sql_yacc.yy"
    {}
    break;

  case 105:
#line 896 "sql_yacc.yy"
    { Lex->create_info.merge_insert_method= yyvsp[0].ulong_num; Lex->create_info.used_fields|= HA_CREATE_USED_INSERT_METHOD;}
    break;

  case 106:
#line 897 "sql_yacc.yy"
    { Lex->create_info.data_file_name= yyvsp[0].lex_str.str; }
    break;

  case 107:
#line 898 "sql_yacc.yy"
    { Lex->create_info.index_file_name= yyvsp[0].lex_str.str; }
    break;

  case 108:
#line 901 "sql_yacc.yy"
    { yyval.db_type= DB_TYPE_ISAM; }
    break;

  case 109:
#line 902 "sql_yacc.yy"
    { yyval.db_type= DB_TYPE_MYISAM; }
    break;

  case 110:
#line 903 "sql_yacc.yy"
    { yyval.db_type= DB_TYPE_MRG_MYISAM; }
    break;

  case 111:
#line 904 "sql_yacc.yy"
    { yyval.db_type= DB_TYPE_HEAP; }
    break;

  case 112:
#line 905 "sql_yacc.yy"
    { yyval.db_type= DB_TYPE_HEAP; }
    break;

  case 113:
#line 906 "sql_yacc.yy"
    { yyval.db_type= DB_TYPE_BERKELEY_DB; }
    break;

  case 114:
#line 907 "sql_yacc.yy"
    { yyval.db_type= DB_TYPE_INNODB; }
    break;

  case 115:
#line 910 "sql_yacc.yy"
    { yyval.row_type= ROW_TYPE_DEFAULT; }
    break;

  case 116:
#line 911 "sql_yacc.yy"
    { yyval.row_type= ROW_TYPE_FIXED; }
    break;

  case 117:
#line 912 "sql_yacc.yy"
    { yyval.row_type= ROW_TYPE_DYNAMIC; }
    break;

  case 118:
#line 913 "sql_yacc.yy"
    { yyval.row_type= ROW_TYPE_COMPRESSED; }
    break;

  case 119:
#line 916 "sql_yacc.yy"
    { yyval.ulong_num= RAID_TYPE_0; }
    break;

  case 120:
#line 917 "sql_yacc.yy"
    { yyval.ulong_num= RAID_TYPE_0; }
    break;

  case 121:
#line 918 "sql_yacc.yy"
    { yyval.ulong_num=yyvsp[0].ulong_num;}
    break;

  case 122:
#line 921 "sql_yacc.yy"
    { yyval.ulong_num= MERGE_INSERT_DISABLED; }
    break;

  case 123:
#line 922 "sql_yacc.yy"
    { yyval.ulong_num= MERGE_INSERT_TO_FIRST; }
    break;

  case 124:
#line 923 "sql_yacc.yy"
    { yyval.ulong_num= MERGE_INSERT_TO_LAST; }
    break;

  case 127:
#line 930 "sql_yacc.yy"
    { yyval.udf_type = UDFTYPE_FUNCTION; }
    break;

  case 128:
#line 931 "sql_yacc.yy"
    { yyval.udf_type = UDFTYPE_AGGREGATE; }
    break;

  case 129:
#line 934 "sql_yacc.yy"
    {yyval.num = (int) STRING_RESULT; }
    break;

  case 130:
#line 935 "sql_yacc.yy"
    {yyval.num = (int) REAL_RESULT; }
    break;

  case 131:
#line 936 "sql_yacc.yy"
    {yyval.num = (int) INT_RESULT; }
    break;

  case 137:
#line 951 "sql_yacc.yy"
    {
	    Lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 138:
#line 958 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->key_list.push_back(new Key(yyvsp[-4].key_type,yyvsp[-3].simple_string,lex->col_list));
	    lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 139:
#line 964 "sql_yacc.yy"
    {
	    Lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 140:
#line 968 "sql_yacc.yy"
    {
	    Lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 145:
#line 984 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->length=lex->dec=0; lex->type=0; lex->interval=0;
	   lex->default_value=0;
	 }
    break;

  case 146:
#line 990 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (add_field_to_list(yyvsp[-3].lex_str.str,
				(enum enum_field_types) yyvsp[-1].num,
				lex->length,lex->dec,lex->type,
				lex->default_value,lex->change,
				lex->interval))
	    YYABORT;
	}
    break;

  case 147:
#line 1001 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].simple_string; yyval.num=yyvsp[-2].num; }
    break;

  case 148:
#line 1002 "sql_yacc.yy"
    { yyval.num=yyvsp[-2].num; }
    break;

  case 149:
#line 1003 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_FLOAT; }
    break;

  case 150:
#line 1004 "sql_yacc.yy"
    { Lex->length=(char*) "1";
					  yyval.num=FIELD_TYPE_TINY; }
    break;

  case 151:
#line 1006 "sql_yacc.yy"
    { Lex->length=(char*) "1";
					  yyval.num=FIELD_TYPE_TINY; }
    break;

  case 152:
#line 1008 "sql_yacc.yy"
    { Lex->length=yyvsp[-2].lex_str.str;
					  yyval.num=FIELD_TYPE_STRING; }
    break;

  case 153:
#line 1010 "sql_yacc.yy"
    { Lex->length=(char*) "1";
					  yyval.num=FIELD_TYPE_STRING; }
    break;

  case 154:
#line 1012 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].lex_str.str;
					  Lex->type|=BINARY_FLAG;
					  yyval.num=FIELD_TYPE_STRING; }
    break;

  case 155:
#line 1015 "sql_yacc.yy"
    { Lex->length=yyvsp[-2].lex_str.str;
					  yyval.num=FIELD_TYPE_VAR_STRING; }
    break;

  case 156:
#line 1017 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].lex_str.str;
					  Lex->type|=BINARY_FLAG;
					  yyval.num=FIELD_TYPE_VAR_STRING; }
    break;

  case 157:
#line 1020 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_YEAR; Lex->length=yyvsp[-1].simple_string; }
    break;

  case 158:
#line 1021 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DATE; }
    break;

  case 159:
#line 1022 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_TIME; }
    break;

  case 160:
#line 1023 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_TIMESTAMP; }
    break;

  case 161:
#line 1024 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].lex_str.str;
					  yyval.num=FIELD_TYPE_TIMESTAMP; }
    break;

  case 162:
#line 1026 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DATETIME; }
    break;

  case 163:
#line 1027 "sql_yacc.yy"
    { Lex->type|=BINARY_FLAG;
					  yyval.num=FIELD_TYPE_TINY_BLOB; }
    break;

  case 164:
#line 1029 "sql_yacc.yy"
    { Lex->type|=BINARY_FLAG;
					  yyval.num=FIELD_TYPE_BLOB; }
    break;

  case 165:
#line 1031 "sql_yacc.yy"
    { Lex->type|=BINARY_FLAG;
					  yyval.num=FIELD_TYPE_MEDIUM_BLOB; }
    break;

  case 166:
#line 1033 "sql_yacc.yy"
    { Lex->type|=BINARY_FLAG;
					  yyval.num=FIELD_TYPE_LONG_BLOB; }
    break;

  case 167:
#line 1035 "sql_yacc.yy"
    { Lex->type|=BINARY_FLAG;
					  yyval.num=FIELD_TYPE_MEDIUM_BLOB; }
    break;

  case 168:
#line 1037 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_MEDIUM_BLOB; }
    break;

  case 169:
#line 1038 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_TINY_BLOB; }
    break;

  case 170:
#line 1039 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_BLOB; }
    break;

  case 171:
#line 1040 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_MEDIUM_BLOB; }
    break;

  case 172:
#line 1041 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_LONG_BLOB; }
    break;

  case 173:
#line 1043 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DECIMAL;}
    break;

  case 174:
#line 1045 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DECIMAL;}
    break;

  case 175:
#line 1046 "sql_yacc.yy"
    {Lex->interval_list.empty();}
    break;

  case 176:
#line 1047 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->interval=typelib(lex->interval_list);
	    yyval.num=FIELD_TYPE_ENUM;
	  }
    break;

  case 177:
#line 1052 "sql_yacc.yy"
    { Lex->interval_list.empty();}
    break;

  case 178:
#line 1053 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->interval=typelib(lex->interval_list);
	    yyval.num=FIELD_TYPE_SET;
	  }
    break;

  case 179:
#line 1060 "sql_yacc.yy"
    {}
    break;

  case 180:
#line 1061 "sql_yacc.yy"
    {}
    break;

  case 181:
#line 1062 "sql_yacc.yy"
    {}
    break;

  case 182:
#line 1065 "sql_yacc.yy"
    {}
    break;

  case 183:
#line 1066 "sql_yacc.yy"
    {}
    break;

  case 184:
#line 1067 "sql_yacc.yy"
    {}
    break;

  case 185:
#line 1068 "sql_yacc.yy"
    {}
    break;

  case 186:
#line 1071 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_LONG; }
    break;

  case 187:
#line 1072 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_TINY; }
    break;

  case 188:
#line 1073 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_SHORT; }
    break;

  case 189:
#line 1074 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_INT24; }
    break;

  case 190:
#line 1075 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_LONGLONG; }
    break;

  case 191:
#line 1078 "sql_yacc.yy"
    { yyval.num= current_thd->sql_mode & MODE_REAL_AS_FLOAT ?
			      FIELD_TYPE_FLOAT : FIELD_TYPE_DOUBLE; }
    break;

  case 192:
#line 1080 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DOUBLE; }
    break;

  case 193:
#line 1081 "sql_yacc.yy"
    { yyval.num=FIELD_TYPE_DOUBLE; }
    break;

  case 194:
#line 1085 "sql_yacc.yy"
    {}
    break;

  case 195:
#line 1086 "sql_yacc.yy"
    { Lex->length=yyvsp[-1].lex_str.str; }
    break;

  case 196:
#line 1087 "sql_yacc.yy"
    {}
    break;

  case 197:
#line 1091 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->length=yyvsp[-3].lex_str.str; lex->dec=yyvsp[-1].lex_str.str;
	}
    break;

  case 198:
#line 1097 "sql_yacc.yy"
    {}
    break;

  case 199:
#line 1098 "sql_yacc.yy"
    {}
    break;

  case 200:
#line 1101 "sql_yacc.yy"
    {}
    break;

  case 201:
#line 1102 "sql_yacc.yy"
    {}
    break;

  case 202:
#line 1105 "sql_yacc.yy"
    {}
    break;

  case 203:
#line 1106 "sql_yacc.yy"
    { Lex->type|= UNSIGNED_FLAG;}
    break;

  case 204:
#line 1107 "sql_yacc.yy"
    { Lex->type|= UNSIGNED_FLAG | ZEROFILL_FLAG; }
    break;

  case 205:
#line 1110 "sql_yacc.yy"
    { yyval.simple_string=(char*) 0; }
    break;

  case 206:
#line 1111 "sql_yacc.yy"
    { yyval.simple_string=yyvsp[-1].lex_str.str; }
    break;

  case 207:
#line 1114 "sql_yacc.yy"
    {}
    break;

  case 208:
#line 1115 "sql_yacc.yy"
    {}
    break;

  case 209:
#line 1118 "sql_yacc.yy"
    {}
    break;

  case 210:
#line 1119 "sql_yacc.yy"
    {}
    break;

  case 211:
#line 1122 "sql_yacc.yy"
    {}
    break;

  case 213:
#line 1126 "sql_yacc.yy"
    { Lex->type&= ~ NOT_NULL_FLAG; }
    break;

  case 214:
#line 1127 "sql_yacc.yy"
    { Lex->type|= NOT_NULL_FLAG; }
    break;

  case 215:
#line 1128 "sql_yacc.yy"
    { Lex->default_value=yyvsp[0].item; }
    break;

  case 216:
#line 1129 "sql_yacc.yy"
    { Lex->type|= AUTO_INCREMENT_FLAG | NOT_NULL_FLAG; }
    break;

  case 217:
#line 1130 "sql_yacc.yy"
    { Lex->type|= PRI_KEY_FLAG | NOT_NULL_FLAG; }
    break;

  case 218:
#line 1131 "sql_yacc.yy"
    { Lex->type|= UNIQUE_FLAG; }
    break;

  case 219:
#line 1132 "sql_yacc.yy"
    { Lex->type|= UNIQUE_KEY_FLAG; }
    break;

  case 220:
#line 1133 "sql_yacc.yy"
    {}
    break;

  case 221:
#line 1136 "sql_yacc.yy"
    {}
    break;

  case 222:
#line 1137 "sql_yacc.yy"
    { Lex->type|=BINARY_FLAG; }
    break;

  case 223:
#line 1138 "sql_yacc.yy"
    {}
    break;

  case 224:
#line 1142 "sql_yacc.yy"
    {}
    break;

  case 225:
#line 1144 "sql_yacc.yy"
    {
	    Lex->col_list.empty();		/* Alloced by sql_alloc */
	  }
    break;

  case 226:
#line 1149 "sql_yacc.yy"
    {}
    break;

  case 227:
#line 1150 "sql_yacc.yy"
    {}
    break;

  case 228:
#line 1153 "sql_yacc.yy"
    {}
    break;

  case 229:
#line 1154 "sql_yacc.yy"
    {}
    break;

  case 230:
#line 1158 "sql_yacc.yy"
    {}
    break;

  case 231:
#line 1159 "sql_yacc.yy"
    {}
    break;

  case 232:
#line 1160 "sql_yacc.yy"
    {}
    break;

  case 233:
#line 1161 "sql_yacc.yy"
    {}
    break;

  case 234:
#line 1164 "sql_yacc.yy"
    {}
    break;

  case 235:
#line 1165 "sql_yacc.yy"
    {}
    break;

  case 236:
#line 1166 "sql_yacc.yy"
    {}
    break;

  case 237:
#line 1167 "sql_yacc.yy"
    {}
    break;

  case 238:
#line 1168 "sql_yacc.yy"
    {}
    break;

  case 239:
#line 1171 "sql_yacc.yy"
    { yyval.key_type= Key::PRIMARY; }
    break;

  case 240:
#line 1172 "sql_yacc.yy"
    { yyval.key_type= Key::MULTIPLE; }
    break;

  case 241:
#line 1173 "sql_yacc.yy"
    { yyval.key_type= Key::FULLTEXT; }
    break;

  case 242:
#line 1174 "sql_yacc.yy"
    { yyval.key_type= Key::FULLTEXT; }
    break;

  case 243:
#line 1175 "sql_yacc.yy"
    { yyval.key_type= Key::UNIQUE; }
    break;

  case 244:
#line 1176 "sql_yacc.yy"
    { yyval.key_type= Key::UNIQUE; }
    break;

  case 245:
#line 1179 "sql_yacc.yy"
    {}
    break;

  case 246:
#line 1180 "sql_yacc.yy"
    {}
    break;

  case 247:
#line 1183 "sql_yacc.yy"
    {}
    break;

  case 248:
#line 1184 "sql_yacc.yy"
    {}
    break;

  case 249:
#line 1185 "sql_yacc.yy"
    {}
    break;

  case 250:
#line 1188 "sql_yacc.yy"
    { yyval.key_type= Key::MULTIPLE; }
    break;

  case 251:
#line 1189 "sql_yacc.yy"
    { yyval.key_type= Key::UNIQUE; }
    break;

  case 252:
#line 1190 "sql_yacc.yy"
    { yyval.key_type= Key::FULLTEXT; }
    break;

  case 253:
#line 1193 "sql_yacc.yy"
    { Lex->col_list.push_back(yyvsp[-1].key_part); }
    break;

  case 254:
#line 1194 "sql_yacc.yy"
    { Lex->col_list.push_back(yyvsp[-1].key_part); }
    break;

  case 255:
#line 1197 "sql_yacc.yy"
    { yyval.key_part=new key_part_spec(yyvsp[0].lex_str.str); }
    break;

  case 256:
#line 1198 "sql_yacc.yy"
    { yyval.key_part=new key_part_spec(yyvsp[-3].lex_str.str,(uint) atoi(yyvsp[-1].lex_str.str)); }
    break;

  case 257:
#line 1201 "sql_yacc.yy"
    { yyval.simple_string=(char*) 0; }
    break;

  case 258:
#line 1202 "sql_yacc.yy"
    { yyval.simple_string=yyvsp[0].lex_str.str; }
    break;

  case 259:
#line 1205 "sql_yacc.yy"
    { Lex->interval_list.push_back(yyvsp[0].string); }
    break;

  case 260:
#line 1206 "sql_yacc.yy"
    { Lex->interval_list.push_back(yyvsp[0].string); }
    break;

  case 261:
#line 1214 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_ALTER_TABLE;
	  lex->name=0;
	  if (!add_table_to_list(yyvsp[0].table, NULL, TL_OPTION_UPDATING))
	    YYABORT;
	  lex->drop_primary=0;
	  lex->create_list.empty();
	  lex->key_list.empty();
	  lex->col_list.empty();
	  lex->drop_list.empty();
	  lex->alter_list.empty();
          lex->select->order_list.elements=0;
          lex->select->order_list.first=0;
          lex->select->order_list.next= (byte**) &lex->select->order_list.first;
	  lex->select->db=lex->name=0;
    	  bzero((char*) &lex->create_info,sizeof(lex->create_info));
	  lex->create_info.db_type= DB_TYPE_DEFAULT;
	  lex->create_info.row_type= ROW_TYPE_NOT_USED;
          lex->alter_keys_onoff=LEAVE_AS_IS;
          lex->simple_alter=1;
	}
    break;

  case 262:
#line 1237 "sql_yacc.yy"
    {}
    break;

  case 266:
#line 1245 "sql_yacc.yy"
    { Lex->change=0; }
    break;

  case 267:
#line 1248 "sql_yacc.yy"
    { Lex->simple_alter=0; }
    break;

  case 268:
#line 1249 "sql_yacc.yy"
    { Lex->simple_alter=0; }
    break;

  case 269:
#line 1250 "sql_yacc.yy"
    { Lex->simple_alter=0; }
    break;

  case 270:
#line 1252 "sql_yacc.yy"
    {
	     LEX *lex=Lex;
	     lex->change= yyvsp[0].lex_str.str; lex->simple_alter=0;
	  }
    break;

  case 272:
#line 1258 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->length=lex->dec=0; lex->type=0; lex->interval=0;
	    lex->default_value=0;
            lex->simple_alter=0;
	  }
    break;

  case 273:
#line 1265 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (add_field_to_list(yyvsp[-3].lex_str.str,
				  (enum enum_field_types) yyvsp[-1].num,
				  lex->length,lex->dec,lex->type,
				  lex->default_value, yyvsp[-3].lex_str.str,
				  lex->interval))
	      YYABORT;
	  }
    break;

  case 275:
#line 1276 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->drop_list.push_back(new Alter_drop(Alter_drop::COLUMN,
					    yyvsp[-1].lex_str.str)); lex->simple_alter=0;
	  }
    break;

  case 276:
#line 1282 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->drop_primary=1; lex->simple_alter=0;
	  }
    break;

  case 277:
#line 1286 "sql_yacc.yy"
    { Lex->simple_alter=0; }
    break;

  case 278:
#line 1288 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->drop_list.push_back(new Alter_drop(Alter_drop::KEY,
						    yyvsp[0].lex_str.str));
	    lex->simple_alter=0;
	  }
    break;

  case 279:
#line 1294 "sql_yacc.yy"
    { Lex->alter_keys_onoff=DISABLE; }
    break;

  case 280:
#line 1295 "sql_yacc.yy"
    { Lex->alter_keys_onoff=ENABLE; }
    break;

  case 281:
#line 1297 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->alter_list.push_back(new Alter_column(yyvsp[-3].lex_str.str,yyvsp[0].item));
	    lex->simple_alter=0;
	  }
    break;

  case 282:
#line 1303 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->alter_list.push_back(new Alter_column(yyvsp[-2].lex_str.str,(Item*) 0));
	    lex->simple_alter=0;
	  }
    break;

  case 283:
#line 1309 "sql_yacc.yy"
    { 
	    LEX *lex=Lex;
	    lex->select->db=yyvsp[0].table->db.str;
	    lex->name= yyvsp[0].table->table.str;
	  }
    break;

  case 284:
#line 1314 "sql_yacc.yy"
    { Lex->simple_alter=0; }
    break;

  case 285:
#line 1315 "sql_yacc.yy"
    { Lex->simple_alter=0; }
    break;

  case 286:
#line 1318 "sql_yacc.yy"
    {}
    break;

  case 287:
#line 1319 "sql_yacc.yy"
    {}
    break;

  case 288:
#line 1322 "sql_yacc.yy"
    { Lex->duplicates=DUP_ERROR; }
    break;

  case 289:
#line 1323 "sql_yacc.yy"
    { Lex->duplicates=DUP_IGNORE; }
    break;

  case 290:
#line 1326 "sql_yacc.yy"
    {}
    break;

  case 291:
#line 1327 "sql_yacc.yy"
    {}
    break;

  case 292:
#line 1328 "sql_yacc.yy"
    {}
    break;

  case 293:
#line 1331 "sql_yacc.yy"
    {}
    break;

  case 294:
#line 1332 "sql_yacc.yy"
    { store_position_for_column(yyvsp[0].lex_str.str); }
    break;

  case 295:
#line 1333 "sql_yacc.yy"
    { store_position_for_column(first_keyword); }
    break;

  case 296:
#line 1336 "sql_yacc.yy"
    {}
    break;

  case 297:
#line 1337 "sql_yacc.yy"
    {}
    break;

  case 298:
#line 1338 "sql_yacc.yy"
    {}
    break;

  case 299:
#line 1339 "sql_yacc.yy"
    {}
    break;

  case 300:
#line 1346 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
           lex->sql_command = SQLCOM_SLAVE_START;
	   lex->type = 0;
         }
    break;

  case 301:
#line 1353 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
           lex->sql_command = SQLCOM_SLAVE_STOP;
	   lex->type = 0;
         }
    break;

  case 302:
#line 1360 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
           lex->sql_command = SQLCOM_SLAVE_START;
	   lex->type = 0;
         }
    break;

  case 303:
#line 1367 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
           lex->sql_command = SQLCOM_SLAVE_STOP;
	   lex->type = 0;
         }
    break;

  case 304:
#line 1374 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_BEGIN;}
    break;

  case 305:
#line 1375 "sql_yacc.yy"
    {}
    break;

  case 308:
#line 1383 "sql_yacc.yy"
    {}
    break;

  case 309:
#line 1384 "sql_yacc.yy"
    { Lex->slave_thd_opt|=SLAVE_SQL; }
    break;

  case 310:
#line 1385 "sql_yacc.yy"
    { Lex->slave_thd_opt|=SLAVE_IO; }
    break;

  case 311:
#line 1390 "sql_yacc.yy"
    {
	   Lex->sql_command = SQLCOM_RESTORE_TABLE;
	}
    break;

  case 312:
#line 1394 "sql_yacc.yy"
    {
	  Lex->backup_dir = yyvsp[0].lex_str.str;
        }
    break;

  case 313:
#line 1400 "sql_yacc.yy"
    {
	   Lex->sql_command = SQLCOM_BACKUP_TABLE;
	}
    break;

  case 314:
#line 1404 "sql_yacc.yy"
    {
	  Lex->backup_dir = yyvsp[0].lex_str.str;
        }
    break;

  case 315:
#line 1410 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->sql_command = SQLCOM_REPAIR;
	   lex->check_opt.init();
	}
    break;

  case 316:
#line 1416 "sql_yacc.yy"
    {}
    break;

  case 317:
#line 1420 "sql_yacc.yy"
    { Lex->check_opt.flags = T_MEDIUM; }
    break;

  case 318:
#line 1421 "sql_yacc.yy"
    {}
    break;

  case 319:
#line 1424 "sql_yacc.yy"
    {}
    break;

  case 320:
#line 1425 "sql_yacc.yy"
    {}
    break;

  case 321:
#line 1428 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_QUICK; }
    break;

  case 322:
#line 1429 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_EXTEND; }
    break;

  case 323:
#line 1430 "sql_yacc.yy"
    { Lex->check_opt.sql_flags|= TT_USEFRM; }
    break;

  case 324:
#line 1434 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->sql_command = SQLCOM_ANALYZE;
	   lex->check_opt.init();
	}
    break;

  case 325:
#line 1440 "sql_yacc.yy"
    {}
    break;

  case 326:
#line 1445 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->sql_command = SQLCOM_CHECK;
	   lex->check_opt.init();
	}
    break;

  case 327:
#line 1451 "sql_yacc.yy"
    {}
    break;

  case 328:
#line 1455 "sql_yacc.yy"
    { Lex->check_opt.flags = T_MEDIUM; }
    break;

  case 329:
#line 1456 "sql_yacc.yy"
    {}
    break;

  case 330:
#line 1459 "sql_yacc.yy"
    {}
    break;

  case 331:
#line 1460 "sql_yacc.yy"
    {}
    break;

  case 332:
#line 1463 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_QUICK; }
    break;

  case 333:
#line 1464 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_FAST; }
    break;

  case 334:
#line 1465 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_MEDIUM; }
    break;

  case 335:
#line 1466 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_EXTEND; }
    break;

  case 336:
#line 1467 "sql_yacc.yy"
    { Lex->check_opt.flags|= T_CHECK_ONLY_CHANGED; }
    break;

  case 337:
#line 1471 "sql_yacc.yy"
    {
	   LEX *lex=Lex;
	   lex->sql_command = SQLCOM_OPTIMIZE;
	   lex->check_opt.init();
	}
    break;

  case 338:
#line 1477 "sql_yacc.yy"
    {}
    break;

  case 339:
#line 1482 "sql_yacc.yy"
    {
	   Lex->sql_command=SQLCOM_RENAME_TABLE;
	}
    break;

  case 340:
#line 1486 "sql_yacc.yy"
    {}
    break;

  case 343:
#line 1495 "sql_yacc.yy"
    {
	   if (!add_table_to_list(yyvsp[-2].table, NULL, TL_OPTION_UPDATING, TL_IGNORE) ||
	       !add_table_to_list(yyvsp[0].table, NULL, TL_OPTION_UPDATING, TL_IGNORE))
	     YYABORT;
 	}
    break;

  case 344:
#line 1507 "sql_yacc.yy"
    { Lex->sql_command=SQLCOM_SELECT; }
    break;

  case 345:
#line 1510 "sql_yacc.yy"
    { Select->braces= 0;	}
    break;

  case 347:
#line 1512 "sql_yacc.yy"
    { Select->braces= 1;}
    break;

  case 349:
#line 1516 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->lock_option=TL_READ;
	   mysql_init_select(lex);
	}
    break;

  case 351:
#line 1524 "sql_yacc.yy"
    {}
    break;

  case 360:
#line 1542 "sql_yacc.yy"
    { Select->options|= SELECT_STRAIGHT_JOIN; }
    break;

  case 361:
#line 1544 "sql_yacc.yy"
    {
	    if (check_simple_select())
	      YYABORT;
	    Lex->lock_option= TL_READ_HIGH_PRIORITY;
	  }
    break;

  case 362:
#line 1549 "sql_yacc.yy"
    { Select->options|= SELECT_DISTINCT; }
    break;

  case 363:
#line 1550 "sql_yacc.yy"
    { Select->options|= SELECT_SMALL_RESULT; }
    break;

  case 364:
#line 1551 "sql_yacc.yy"
    { Select->options|= SELECT_BIG_RESULT; }
    break;

  case 365:
#line 1553 "sql_yacc.yy"
    {
	    if (check_simple_select())
	      YYABORT;
	    Select->options|= OPTION_BUFFER_RESULT;
	  }
    break;

  case 366:
#line 1559 "sql_yacc.yy"
    {
	    if (check_simple_select())
	      YYABORT;
	    Select->options|= OPTION_FOUND_ROWS;
	  }
    break;

  case 367:
#line 1564 "sql_yacc.yy"
    { current_thd->safe_to_cache_query=0; }
    break;

  case 368:
#line 1566 "sql_yacc.yy"
    {
	    Lex->select_lex.options|= OPTION_TO_QUERY_CACHE;
	  }
    break;

  case 369:
#line 1569 "sql_yacc.yy"
    {}
    break;

  case 371:
#line 1575 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (check_simple_select())
	      YYABORT;	
	    lex->lock_option= TL_WRITE;
	    lex->thd->safe_to_cache_query=0;
	  }
    break;

  case 372:
#line 1583 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (check_simple_select())
	      YYABORT;	
	    lex->lock_option= TL_READ_WITH_SHARED_LOCKS;
	    lex->thd->safe_to_cache_query=0;
	  }
    break;

  case 375:
#line 1596 "sql_yacc.yy"
    {
	    if (add_item_to_list(new Item_field(NULL,NULL,"*")))
	      YYABORT;
	  }
    break;

  case 376:
#line 1604 "sql_yacc.yy"
    {
	    if (add_item_to_list(yyvsp[-2].item))
	      YYABORT;
	    if (yyvsp[0].lex_str.str)
	      yyvsp[-2].item->set_name(yyvsp[0].lex_str.str);
	    else if (!yyvsp[-2].item->name)
	      yyvsp[-2].item->set_name(yyvsp[-3].simple_string,(uint) (yyvsp[-1].simple_string - yyvsp[-3].simple_string));
	  }
    break;

  case 377:
#line 1614 "sql_yacc.yy"
    { yyval.simple_string=(char*) Lex->tok_start; }
    break;

  case 378:
#line 1617 "sql_yacc.yy"
    { yyval.simple_string=(char*) Lex->tok_end; }
    break;

  case 379:
#line 1620 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 380:
#line 1621 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 381:
#line 1624 "sql_yacc.yy"
    { yyval.lex_str.str=0;}
    break;

  case 382:
#line 1625 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 383:
#line 1626 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 384:
#line 1627 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 385:
#line 1628 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 386:
#line 1631 "sql_yacc.yy"
    {}
    break;

  case 387:
#line 1632 "sql_yacc.yy"
    {}
    break;

  case 388:
#line 1635 "sql_yacc.yy"
    {yyval.item = yyvsp[0].item; }
    break;

  case 389:
#line 1636 "sql_yacc.yy"
    {yyval.item = yyvsp[0].item; }
    break;

  case 390:
#line 1641 "sql_yacc.yy"
    { yyval.item= new Item_func_in(yyvsp[-4].item,*yyvsp[-1].item_list); }
    break;

  case 391:
#line 1643 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_in(yyvsp[-5].item,*yyvsp[-1].item_list)); }
    break;

  case 392:
#line 1645 "sql_yacc.yy"
    { yyval.item= new Item_func_between(yyvsp[-4].item,yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 393:
#line 1647 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_between(yyvsp[-5].item,yyvsp[-2].item,yyvsp[0].item)); }
    break;

  case 394:
#line 1648 "sql_yacc.yy"
    { yyval.item= or_or_concat(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 395:
#line 1649 "sql_yacc.yy"
    { yyval.item= new Item_cond_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 396:
#line 1650 "sql_yacc.yy"
    { yyval.item= new Item_cond_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 397:
#line 1651 "sql_yacc.yy"
    { yyval.item= new Item_cond_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 398:
#line 1652 "sql_yacc.yy"
    { yyval.item= new Item_func_like(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].simple_string); }
    break;

  case 399:
#line 1653 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_like(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].simple_string));}
    break;

  case 400:
#line 1654 "sql_yacc.yy"
    { yyval.item= new Item_func_regex(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 401:
#line 1655 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_regex(yyvsp[-3].item,yyvsp[0].item)); }
    break;

  case 402:
#line 1656 "sql_yacc.yy"
    { yyval.item= new Item_func_isnull(yyvsp[-2].item); }
    break;

  case 403:
#line 1657 "sql_yacc.yy"
    { yyval.item= new Item_func_isnotnull(yyvsp[-3].item); }
    break;

  case 404:
#line 1658 "sql_yacc.yy"
    { yyval.item= new Item_func_eq(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 405:
#line 1659 "sql_yacc.yy"
    { yyval.item= new Item_func_equal(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 406:
#line 1660 "sql_yacc.yy"
    { yyval.item= new Item_func_ge(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 407:
#line 1661 "sql_yacc.yy"
    { yyval.item= new Item_func_gt(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 408:
#line 1662 "sql_yacc.yy"
    { yyval.item= new Item_func_le(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 409:
#line 1663 "sql_yacc.yy"
    { yyval.item= new Item_func_lt(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 410:
#line 1664 "sql_yacc.yy"
    { yyval.item= new Item_func_ne(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 411:
#line 1665 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_left(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 412:
#line 1666 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_right(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 413:
#line 1667 "sql_yacc.yy"
    { yyval.item= new Item_func_plus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 414:
#line 1668 "sql_yacc.yy"
    { yyval.item= new Item_func_minus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 415:
#line 1669 "sql_yacc.yy"
    { yyval.item= new Item_func_mul(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 416:
#line 1670 "sql_yacc.yy"
    { yyval.item= new Item_func_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 417:
#line 1671 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 418:
#line 1672 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 419:
#line 1673 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 420:
#line 1674 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 421:
#line 1676 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].interval,0); }
    break;

  case 422:
#line 1678 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].interval,1); }
    break;

  case 423:
#line 1683 "sql_yacc.yy"
    { yyval.item= new Item_func_between(yyvsp[-4].item,yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 424:
#line 1685 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_between(yyvsp[-5].item,yyvsp[-2].item,yyvsp[0].item)); }
    break;

  case 425:
#line 1686 "sql_yacc.yy"
    { yyval.item= or_or_concat(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 426:
#line 1687 "sql_yacc.yy"
    { yyval.item= new Item_cond_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 427:
#line 1688 "sql_yacc.yy"
    { yyval.item= new Item_cond_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 428:
#line 1689 "sql_yacc.yy"
    { yyval.item= new Item_cond_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 429:
#line 1690 "sql_yacc.yy"
    { yyval.item= new Item_func_like(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].simple_string); }
    break;

  case 430:
#line 1691 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_like(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].simple_string)); }
    break;

  case 431:
#line 1692 "sql_yacc.yy"
    { yyval.item= new Item_func_regex(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 432:
#line 1693 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_regex(yyvsp[-3].item,yyvsp[0].item)); }
    break;

  case 433:
#line 1694 "sql_yacc.yy"
    { yyval.item= new Item_func_isnull(yyvsp[-2].item); }
    break;

  case 434:
#line 1695 "sql_yacc.yy"
    { yyval.item= new Item_func_isnotnull(yyvsp[-3].item); }
    break;

  case 435:
#line 1696 "sql_yacc.yy"
    { yyval.item= new Item_func_eq(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 436:
#line 1697 "sql_yacc.yy"
    { yyval.item= new Item_func_equal(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 437:
#line 1698 "sql_yacc.yy"
    { yyval.item= new Item_func_ge(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 438:
#line 1699 "sql_yacc.yy"
    { yyval.item= new Item_func_gt(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 439:
#line 1700 "sql_yacc.yy"
    { yyval.item= new Item_func_le(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 440:
#line 1701 "sql_yacc.yy"
    { yyval.item= new Item_func_lt(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 441:
#line 1702 "sql_yacc.yy"
    { yyval.item= new Item_func_ne(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 442:
#line 1703 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_left(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 443:
#line 1704 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_right(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 444:
#line 1705 "sql_yacc.yy"
    { yyval.item= new Item_func_plus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 445:
#line 1706 "sql_yacc.yy"
    { yyval.item= new Item_func_minus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 446:
#line 1707 "sql_yacc.yy"
    { yyval.item= new Item_func_mul(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 447:
#line 1708 "sql_yacc.yy"
    { yyval.item= new Item_func_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 448:
#line 1709 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 449:
#line 1710 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 450:
#line 1711 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 451:
#line 1712 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 452:
#line 1714 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].interval,0); }
    break;

  case 453:
#line 1716 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].interval,1); }
    break;

  case 455:
#line 1722 "sql_yacc.yy"
    { yyval.item= new Item_func_in(yyvsp[-4].item,*yyvsp[-1].item_list); }
    break;

  case 456:
#line 1724 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_in(yyvsp[-5].item,*yyvsp[-1].item_list)); }
    break;

  case 457:
#line 1726 "sql_yacc.yy"
    { yyval.item= new Item_func_between(yyvsp[-4].item,yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 458:
#line 1728 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_between(yyvsp[-5].item,yyvsp[-2].item,yyvsp[0].item)); }
    break;

  case 459:
#line 1729 "sql_yacc.yy"
    { yyval.item= or_or_concat(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 460:
#line 1730 "sql_yacc.yy"
    { yyval.item= new Item_cond_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 461:
#line 1731 "sql_yacc.yy"
    { yyval.item= new Item_cond_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 462:
#line 1732 "sql_yacc.yy"
    { yyval.item= new Item_func_like(yyvsp[-3].item,yyvsp[-1].item,yyvsp[0].simple_string); }
    break;

  case 463:
#line 1733 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_like(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].simple_string)); }
    break;

  case 464:
#line 1734 "sql_yacc.yy"
    { yyval.item= new Item_func_regex(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 465:
#line 1735 "sql_yacc.yy"
    { yyval.item= new Item_func_not(new Item_func_regex(yyvsp[-3].item,yyvsp[0].item)); }
    break;

  case 466:
#line 1736 "sql_yacc.yy"
    { yyval.item= new Item_func_isnull(yyvsp[-2].item); }
    break;

  case 467:
#line 1737 "sql_yacc.yy"
    { yyval.item= new Item_func_isnotnull(yyvsp[-3].item); }
    break;

  case 468:
#line 1738 "sql_yacc.yy"
    { yyval.item= new Item_func_eq(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 469:
#line 1739 "sql_yacc.yy"
    { yyval.item= new Item_func_equal(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 470:
#line 1740 "sql_yacc.yy"
    { yyval.item= new Item_func_ge(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 471:
#line 1741 "sql_yacc.yy"
    { yyval.item= new Item_func_gt(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 472:
#line 1742 "sql_yacc.yy"
    { yyval.item= new Item_func_le(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 473:
#line 1743 "sql_yacc.yy"
    { yyval.item= new Item_func_lt(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 474:
#line 1744 "sql_yacc.yy"
    { yyval.item= new Item_func_ne(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 475:
#line 1745 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_left(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 476:
#line 1746 "sql_yacc.yy"
    { yyval.item= new Item_func_shift_right(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 477:
#line 1747 "sql_yacc.yy"
    { yyval.item= new Item_func_plus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 478:
#line 1748 "sql_yacc.yy"
    { yyval.item= new Item_func_minus(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 479:
#line 1749 "sql_yacc.yy"
    { yyval.item= new Item_func_mul(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 480:
#line 1750 "sql_yacc.yy"
    { yyval.item= new Item_func_div(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 481:
#line 1751 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_or(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 482:
#line 1752 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_xor(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 483:
#line 1753 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_and(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 484:
#line 1754 "sql_yacc.yy"
    { yyval.item= new Item_func_mod(yyvsp[-2].item,yyvsp[0].item); }
    break;

  case 485:
#line 1756 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].interval,0); }
    break;

  case 486:
#line 1758 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-4].item,yyvsp[-1].item,yyvsp[0].interval,1); }
    break;

  case 490:
#line 1765 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_set_user_var(yyvsp[-2].lex_str,yyvsp[0].item);
	    current_thd->safe_to_cache_query=0;
	  }
    break;

  case 491:
#line 1770 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_get_user_var(yyvsp[0].lex_str);
	    current_thd->safe_to_cache_query=0;
	  }
    break;

  case 492:
#line 1775 "sql_yacc.yy"
    {
	    if (!(yyval.item= get_system_var((enum_var_type) yyvsp[-1].num, yyvsp[0].lex_str)))
	      YYABORT;
	  }
    break;

  case 494:
#line 1780 "sql_yacc.yy"
    { yyval.item= new Item_func_neg(yyvsp[0].item); }
    break;

  case 495:
#line 1781 "sql_yacc.yy"
    { yyval.item= new Item_func_bit_neg(yyvsp[0].item); }
    break;

  case 496:
#line 1782 "sql_yacc.yy"
    { yyval.item= new Item_func_not(yyvsp[0].item); }
    break;

  case 497:
#line 1783 "sql_yacc.yy"
    { yyval.item= new Item_func_not(yyvsp[0].item); }
    break;

  case 498:
#line 1784 "sql_yacc.yy"
    { yyval.item= yyvsp[-1].item; }
    break;

  case 499:
#line 1785 "sql_yacc.yy"
    { yyval.item= yyvsp[-1].item; }
    break;

  case 500:
#line 1787 "sql_yacc.yy"
    { Select->ftfunc_list.push_back((Item_func_match *)
                   (yyval.item=new Item_func_match_nl(*yyvsp[-4].item_list,yyvsp[-1].item))); }
    break;

  case 501:
#line 1790 "sql_yacc.yy"
    { Select->ftfunc_list.push_back((Item_func_match *)
                   (yyval.item=new Item_func_match_bool(*yyvsp[-7].item_list,yyvsp[-4].item))); }
    break;

  case 502:
#line 1792 "sql_yacc.yy"
    { yyval.item= new Item_func_binary(yyvsp[0].item); }
    break;

  case 503:
#line 1793 "sql_yacc.yy"
    { yyval.item= create_func_cast(yyvsp[-3].item, yyvsp[-1].cast_type); }
    break;

  case 504:
#line 1795 "sql_yacc.yy"
    { yyval.item= new Item_func_case(* yyvsp[-2].item_list, yyvsp[-4].item, yyvsp[-1].item ); }
    break;

  case 505:
#line 1796 "sql_yacc.yy"
    { yyval.item= create_func_cast(yyvsp[-3].item, yyvsp[-1].cast_type); }
    break;

  case 506:
#line 1798 "sql_yacc.yy"
    { yyval.item= ((Item*(*)(void))(yyvsp[-2].symbol.symbol->create_func))();}
    break;

  case 507:
#line 1800 "sql_yacc.yy"
    { yyval.item= ((Item*(*)(Item*))(yyvsp[-3].symbol.symbol->create_func))(yyvsp[-1].item);}
    break;

  case 508:
#line 1802 "sql_yacc.yy"
    { yyval.item= ((Item*(*)(Item*,Item*))(yyvsp[-5].symbol.symbol->create_func))(yyvsp[-3].item,yyvsp[-1].item);}
    break;

  case 509:
#line 1804 "sql_yacc.yy"
    { yyval.item= ((Item*(*)(Item*,Item*,Item*))(yyvsp[-7].symbol.symbol->create_func))(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item);}
    break;

  case 510:
#line 1806 "sql_yacc.yy"
    { yyval.item= new Item_func_atan(yyvsp[-1].item); }
    break;

  case 511:
#line 1808 "sql_yacc.yy"
    { yyval.item= new Item_func_atan(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 512:
#line 1810 "sql_yacc.yy"
    { yyval.item= new Item_func_char(*yyvsp[-1].item_list); }
    break;

  case 513:
#line 1812 "sql_yacc.yy"
    { yyval.item= new Item_func_coalesce(* yyvsp[-1].item_list); }
    break;

  case 514:
#line 1814 "sql_yacc.yy"
    { yyval.item= new Item_func_concat(* yyvsp[-1].item_list); }
    break;

  case 515:
#line 1816 "sql_yacc.yy"
    { yyval.item= new Item_func_concat_ws(yyvsp[-3].item, *yyvsp[-1].item_list); }
    break;

  case 516:
#line 1818 "sql_yacc.yy"
    { yyval.item= new Item_func_curdate(); current_thd->safe_to_cache_query=0; }
    break;

  case 517:
#line 1820 "sql_yacc.yy"
    { yyval.item= new Item_func_curtime(); current_thd->safe_to_cache_query=0; }
    break;

  case 518:
#line 1822 "sql_yacc.yy"
    { 
	    yyval.item= new Item_func_curtime(yyvsp[-1].item); 
	    current_thd->safe_to_cache_query=0;
	  }
    break;

  case 519:
#line 1827 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-5].item,yyvsp[-2].item,yyvsp[-1].interval,0); }
    break;

  case 520:
#line 1829 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[-5].item,yyvsp[-2].item,yyvsp[-1].interval,1); }
    break;

  case 521:
#line 1831 "sql_yacc.yy"
    { 
	    yyval.item= new Item_func_database();
            current_thd->safe_to_cache_query=0; 
	  }
    break;

  case 522:
#line 1836 "sql_yacc.yy"
    { yyval.item= new Item_func_elt(yyvsp[-3].item, *yyvsp[-1].item_list); }
    break;

  case 523:
#line 1838 "sql_yacc.yy"
    { yyval.item= new Item_func_make_set(yyvsp[-3].item, *yyvsp[-1].item_list); }
    break;

  case 524:
#line 1840 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_encrypt(yyvsp[-1].item);
	    current_thd->safe_to_cache_query=0; 
	  }
    break;

  case 525:
#line 1844 "sql_yacc.yy"
    { yyval.item= new Item_func_encrypt(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 526:
#line 1846 "sql_yacc.yy"
    { yyval.item= new Item_func_decode(yyvsp[-3].item,yyvsp[-1].lex_str.str); }
    break;

  case 527:
#line 1848 "sql_yacc.yy"
    { yyval.item= new Item_func_encode(yyvsp[-3].item,yyvsp[-1].lex_str.str); }
    break;

  case 528:
#line 1850 "sql_yacc.yy"
    { yyval.item= new Item_func_des_decrypt(yyvsp[-1].item); }
    break;

  case 529:
#line 1852 "sql_yacc.yy"
    { yyval.item= new Item_func_des_decrypt(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 530:
#line 1854 "sql_yacc.yy"
    { yyval.item= new Item_func_des_encrypt(yyvsp[-1].item); }
    break;

  case 531:
#line 1856 "sql_yacc.yy"
    { yyval.item= new Item_func_des_encrypt(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 532:
#line 1858 "sql_yacc.yy"
    { yyval.item= new Item_func_export_set(yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 533:
#line 1860 "sql_yacc.yy"
    { yyval.item= new Item_func_export_set(yyvsp[-7].item, yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 534:
#line 1862 "sql_yacc.yy"
    { yyval.item= new Item_func_export_set(yyvsp[-9].item, yyvsp[-7].item, yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 535:
#line 1864 "sql_yacc.yy"
    { yyval.item= new Item_func_format(yyvsp[-3].item,atoi(yyvsp[-1].lex_str.str)); }
    break;

  case 536:
#line 1866 "sql_yacc.yy"
    { yyval.item= new Item_func_from_unixtime(yyvsp[-1].item); }
    break;

  case 537:
#line 1868 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_date_format (new Item_func_from_unixtime(yyvsp[-3].item),yyvsp[-1].item,0);
	  }
    break;

  case 538:
#line 1872 "sql_yacc.yy"
    { yyval.item= new Item_func_field(yyvsp[-3].item, *yyvsp[-1].item_list); }
    break;

  case 539:
#line 1874 "sql_yacc.yy"
    { yyval.item= new Item_func_hour(yyvsp[-1].item); }
    break;

  case 540:
#line 1876 "sql_yacc.yy"
    { yyval.item= new Item_func_if(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 541:
#line 1878 "sql_yacc.yy"
    { yyval.item= new Item_func_insert(yyvsp[-7].item,yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 542:
#line 1881 "sql_yacc.yy"
    { yyval.item= new Item_date_add_interval(yyvsp[0].item,yyvsp[-3].item,yyvsp[-2].interval,0); }
    break;

  case 543:
#line 1883 "sql_yacc.yy"
    { yyval.item= new Item_func_interval(yyvsp[-3].item,* yyvsp[-1].item_list); }
    break;

  case 544:
#line 1885 "sql_yacc.yy"
    {
	    yyval.item= new Item_int((char*) "last_insert_id()",
			     current_thd->insert_id(),21);
	    current_thd->safe_to_cache_query=0;
	  }
    break;

  case 545:
#line 1891 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_set_last_insert_id(yyvsp[-1].item);
	    current_thd->safe_to_cache_query=0;
	  }
    break;

  case 546:
#line 1896 "sql_yacc.yy"
    { yyval.item= new Item_func_left(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 547:
#line 1898 "sql_yacc.yy"
    { yyval.item= new Item_func_locate(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 548:
#line 1900 "sql_yacc.yy"
    { yyval.item= new Item_func_locate(yyvsp[-3].item,yyvsp[-5].item,yyvsp[-1].item); }
    break;

  case 549:
#line 1902 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_max(*yyvsp[-1].item_list); }
    break;

  case 550:
#line 1904 "sql_yacc.yy"
    { yyvsp[-1].item_list->push_front(yyvsp[-3].item); yyval.item= new Item_func_min(*yyvsp[-1].item_list); }
    break;

  case 551:
#line 1906 "sql_yacc.yy"
    { yyval.item= new Item_func_log(yyvsp[-1].item); }
    break;

  case 552:
#line 1908 "sql_yacc.yy"
    { yyval.item= new Item_func_log(yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 553:
#line 1910 "sql_yacc.yy"
    { 
	    yyval.item= new Item_master_pos_wait(yyvsp[-3].item, yyvsp[-1].item);
	    current_thd->safe_to_cache_query=0; 
	  }
    break;

  case 554:
#line 1915 "sql_yacc.yy"
    { 
	    yyval.item= new Item_master_pos_wait(yyvsp[-5].item, yyvsp[-3].item, yyvsp[-1].item);
	    current_thd->safe_to_cache_query=0; 
	  }
    break;

  case 555:
#line 1920 "sql_yacc.yy"
    { yyval.item= new Item_func_minute(yyvsp[-1].item); }
    break;

  case 556:
#line 1922 "sql_yacc.yy"
    { yyval.item= new Item_func_month(yyvsp[-1].item); }
    break;

  case 557:
#line 1924 "sql_yacc.yy"
    { yyval.item= new Item_func_now(); current_thd->safe_to_cache_query=0;}
    break;

  case 558:
#line 1926 "sql_yacc.yy"
    { yyval.item= new Item_func_now(yyvsp[-1].item); current_thd->safe_to_cache_query=0;}
    break;

  case 559:
#line 1928 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_password(yyvsp[-1].item);
	   }
    break;

  case 560:
#line 1932 "sql_yacc.yy"
    { yyval.item = new Item_func_locate(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 561:
#line 1934 "sql_yacc.yy"
    { yyval.item= new Item_func_rand(yyvsp[-1].item); current_thd->safe_to_cache_query=0;}
    break;

  case 562:
#line 1936 "sql_yacc.yy"
    { yyval.item= new Item_func_rand(); current_thd->safe_to_cache_query=0;}
    break;

  case 563:
#line 1938 "sql_yacc.yy"
    { yyval.item= new Item_func_replace(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 564:
#line 1940 "sql_yacc.yy"
    { yyval.item= new Item_func_right(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 565:
#line 1942 "sql_yacc.yy"
    { yyval.item= new Item_func_round(yyvsp[-1].item, new Item_int((char*)"0",0,1),0); }
    break;

  case 566:
#line 1943 "sql_yacc.yy"
    { yyval.item= new Item_func_round(yyvsp[-3].item,yyvsp[-1].item,0); }
    break;

  case 567:
#line 1945 "sql_yacc.yy"
    { yyval.item= new Item_func_second(yyvsp[-1].item); }
    break;

  case 568:
#line 1947 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 569:
#line 1949 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 570:
#line 1951 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 571:
#line 1953 "sql_yacc.yy"
    { yyval.item= new Item_func_substr(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 572:
#line 1955 "sql_yacc.yy"
    { yyval.item= new Item_func_substr_index(yyvsp[-5].item,yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 573:
#line 1957 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item,new Item_string(" ",1)); }
    break;

  case 574:
#line 1959 "sql_yacc.yy"
    { yyval.item= new Item_func_ltrim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 575:
#line 1961 "sql_yacc.yy"
    { yyval.item= new Item_func_rtrim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 576:
#line 1963 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 577:
#line 1965 "sql_yacc.yy"
    { yyval.item= new Item_func_trim(yyvsp[-1].item,yyvsp[-3].item); }
    break;

  case 578:
#line 1967 "sql_yacc.yy"
    { yyval.item= new Item_func_round(yyvsp[-3].item,yyvsp[-1].item,1); }
    break;

  case 579:
#line 1969 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_sum_udf_str(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_sum_udf_str(yyvsp[-3].udf);
	  }
    break;

  case 580:
#line 1976 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_sum_udf_float(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_sum_udf_float(yyvsp[-3].udf);
	  }
    break;

  case 581:
#line 1983 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_sum_udf_int(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_sum_udf_int(yyvsp[-3].udf);
	  }
    break;

  case 582:
#line 1990 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_func_udf_str(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_func_udf_str(yyvsp[-3].udf);
	  }
    break;

  case 583:
#line 1997 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_func_udf_float(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_func_udf_float(yyvsp[-3].udf);
	  }
    break;

  case 584:
#line 2004 "sql_yacc.yy"
    {
	    if (yyvsp[-1].item_list != NULL)
	      yyval.item = new Item_func_udf_int(yyvsp[-3].udf, *yyvsp[-1].item_list);
	    else
	      yyval.item = new Item_func_udf_int(yyvsp[-3].udf);
	  }
    break;

  case 585:
#line 2011 "sql_yacc.yy"
    { 
            yyval.item= new Item_func_unique_users(yyvsp[-7].item,atoi(yyvsp[-5].lex_str.str),atoi(yyvsp[-3].lex_str.str), * yyvsp[-1].item_list);
	  }
    break;

  case 586:
#line 2015 "sql_yacc.yy"
    {
	    yyval.item= new Item_func_unix_timestamp();
	    current_thd->safe_to_cache_query=0;
	  }
    break;

  case 587:
#line 2020 "sql_yacc.yy"
    { yyval.item= new Item_func_unix_timestamp(yyvsp[-1].item); }
    break;

  case 588:
#line 2022 "sql_yacc.yy"
    { yyval.item= new Item_func_user(); current_thd->safe_to_cache_query=0; }
    break;

  case 589:
#line 2024 "sql_yacc.yy"
    { 
	    LEX *lex=Lex;
	    yyval.item= new Item_func_week(yyvsp[-1].item,new Item_int((char*) "0", 
                                   lex->thd->variables.default_week_format,1));
	  }
    break;

  case 590:
#line 2030 "sql_yacc.yy"
    { yyval.item= new Item_func_week(yyvsp[-3].item,yyvsp[-1].item); }
    break;

  case 591:
#line 2032 "sql_yacc.yy"
    { yyval.item= new Item_func_year(yyvsp[-1].item); }
    break;

  case 592:
#line 2034 "sql_yacc.yy"
    { yyval.item= new Item_func_yearweek(yyvsp[-1].item,new Item_int((char*) "0",0,1)); }
    break;

  case 593:
#line 2036 "sql_yacc.yy"
    { yyval.item= new Item_func_yearweek(yyvsp[-3].item, yyvsp[-1].item); }
    break;

  case 594:
#line 2038 "sql_yacc.yy"
    { 
	    yyval.item=new Item_func_benchmark(yyvsp[-3].ulong_num,yyvsp[-1].item);
	    current_thd->safe_to_cache_query=0;
	  }
    break;

  case 595:
#line 2043 "sql_yacc.yy"
    { yyval.item=new Item_extract( yyvsp[-3].interval, yyvsp[-1].item); }
    break;

  case 596:
#line 2046 "sql_yacc.yy"
    { yyval.item_list= NULL; }
    break;

  case 597:
#line 2047 "sql_yacc.yy"
    { yyval.item_list= yyvsp[0].item_list;}
    break;

  case 598:
#line 2051 "sql_yacc.yy"
    { yyval.item=new Item_sum_avg(yyvsp[-1].item); }
    break;

  case 599:
#line 2053 "sql_yacc.yy"
    { yyval.item=new Item_sum_and(yyvsp[-1].item); }
    break;

  case 600:
#line 2055 "sql_yacc.yy"
    { yyval.item=new Item_sum_or(yyvsp[-1].item); }
    break;

  case 601:
#line 2057 "sql_yacc.yy"
    { yyval.item=new Item_sum_count(new Item_int((int32) 0L,1)); }
    break;

  case 602:
#line 2059 "sql_yacc.yy"
    { yyval.item=new Item_sum_count(yyvsp[-1].item); }
    break;

  case 603:
#line 2061 "sql_yacc.yy"
    { Select->in_sum_expr++; }
    break;

  case 604:
#line 2063 "sql_yacc.yy"
    { Select->in_sum_expr--; }
    break;

  case 605:
#line 2065 "sql_yacc.yy"
    { yyval.item=new Item_sum_count_distinct(* yyvsp[-2].item_list); }
    break;

  case 606:
#line 2067 "sql_yacc.yy"
    { yyval.item= new Item_sum_unique_users(yyvsp[-7].item,atoi(yyvsp[-5].lex_str.str),atoi(yyvsp[-3].lex_str.str),yyvsp[-1].item); }
    break;

  case 607:
#line 2069 "sql_yacc.yy"
    { yyval.item=new Item_sum_min(yyvsp[-1].item); }
    break;

  case 608:
#line 2071 "sql_yacc.yy"
    { yyval.item=new Item_sum_max(yyvsp[-1].item); }
    break;

  case 609:
#line 2073 "sql_yacc.yy"
    { yyval.item=new Item_sum_std(yyvsp[-1].item); }
    break;

  case 610:
#line 2075 "sql_yacc.yy"
    { yyval.item=new Item_sum_sum(yyvsp[-1].item); }
    break;

  case 611:
#line 2079 "sql_yacc.yy"
    { Select->in_sum_expr++; }
    break;

  case 612:
#line 2081 "sql_yacc.yy"
    {
	  Select->in_sum_expr--;
	  yyval.item=yyvsp[0].item;
	}
    break;

  case 613:
#line 2087 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_BINARY; }
    break;

  case 614:
#line 2088 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_CHAR; }
    break;

  case 615:
#line 2089 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_SIGNED_INT; }
    break;

  case 616:
#line 2090 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_SIGNED_INT; }
    break;

  case 617:
#line 2091 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_UNSIGNED_INT; }
    break;

  case 618:
#line 2092 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_UNSIGNED_INT; }
    break;

  case 619:
#line 2093 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_DATE; }
    break;

  case 620:
#line 2094 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_TIME; }
    break;

  case 621:
#line 2095 "sql_yacc.yy"
    { yyval.cast_type=ITEM_CAST_DATETIME; }
    break;

  case 622:
#line 2099 "sql_yacc.yy"
    { Select->expr_list.push_front(new List<Item>); }
    break;

  case 623:
#line 2101 "sql_yacc.yy"
    { yyval.item_list= Select->expr_list.pop(); }
    break;

  case 624:
#line 2104 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 625:
#line 2105 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 626:
#line 2108 "sql_yacc.yy"
    { yyval.item_list= yyvsp[0].item_list; }
    break;

  case 627:
#line 2109 "sql_yacc.yy"
    { yyval.item_list= yyvsp[-1].item_list; }
    break;

  case 628:
#line 2112 "sql_yacc.yy"
    { Select->expr_list.push_front(new List<Item>); }
    break;

  case 629:
#line 2114 "sql_yacc.yy"
    { yyval.item_list= Select->expr_list.pop(); }
    break;

  case 630:
#line 2117 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 631:
#line 2118 "sql_yacc.yy"
    { Select->expr_list.head()->push_back(yyvsp[0].item); }
    break;

  case 632:
#line 2121 "sql_yacc.yy"
    { yyval.item= NULL; }
    break;

  case 633:
#line 2122 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 634:
#line 2125 "sql_yacc.yy"
    { yyval.item= NULL; }
    break;

  case 635:
#line 2126 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item; }
    break;

  case 636:
#line 2129 "sql_yacc.yy"
    { Select->when_list.push_front(new List<Item>); }
    break;

  case 637:
#line 2131 "sql_yacc.yy"
    { yyval.item_list= Select->when_list.pop(); }
    break;

  case 638:
#line 2135 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;	    
	    sel->when_list.head()->push_back(yyvsp[-2].item);
	    sel->when_list.head()->push_back(yyvsp[0].item);
	}
    break;

  case 639:
#line 2141 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->when_list.head()->push_back(yyvsp[-2].item);
	    sel->when_list.head()->push_back(yyvsp[0].item);
	  }
    break;

  case 640:
#line 2148 "sql_yacc.yy"
    { yyval.item=new Item_string(" ",1); }
    break;

  case 641:
#line 2149 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 642:
#line 2152 "sql_yacc.yy"
    { yyval.table_list=yyvsp[-1].table_list; }
    break;

  case 643:
#line 2153 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list; }
    break;

  case 644:
#line 2154 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list; }
    break;

  case 645:
#line 2155 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list; }
    break;

  case 646:
#line 2157 "sql_yacc.yy"
    { yyval.table_list=yyvsp[0].table_list ; yyvsp[-2].table_list->next->straight=1; }
    break;

  case 647:
#line 2159 "sql_yacc.yy"
    { add_join_on(yyvsp[-2].table_list,yyvsp[0].item); yyval.table_list=yyvsp[-2].table_list; }
    break;

  case 648:
#line 2162 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->db1=yyvsp[-3].table_list->db; sel->table1=yyvsp[-3].table_list->alias;
	    sel->db2=yyvsp[-1].table_list->db; sel->table2=yyvsp[-1].table_list->alias;
	  }
    break;

  case 649:
#line 2168 "sql_yacc.yy"
    { add_join_on(yyvsp[-5].table_list,yyvsp[-1].item); yyval.table_list=yyvsp[-5].table_list; }
    break;

  case 650:
#line 2171 "sql_yacc.yy"
    { add_join_on(yyvsp[-2].table_list,yyvsp[0].item); yyvsp[-2].table_list->outer_join|=JOIN_TYPE_LEFT; yyval.table_list=yyvsp[-2].table_list; }
    break;

  case 651:
#line 2173 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->db1=yyvsp[-4].table_list->db; sel->table1=yyvsp[-4].table_list->alias;
	    sel->db2=yyvsp[0].table_list->db; sel->table2=yyvsp[0].table_list->alias;
	  }
    break;

  case 652:
#line 2179 "sql_yacc.yy"
    { add_join_on(yyvsp[-5].table_list,yyvsp[-1].item); yyvsp[-5].table_list->outer_join|=JOIN_TYPE_LEFT; yyval.table_list=yyvsp[-5].table_list; }
    break;

  case 653:
#line 2181 "sql_yacc.yy"
    {
	    add_join_natural(yyvsp[-5].table_list,yyvsp[-5].table_list->next);
	    yyvsp[-5].table_list->next->outer_join|=JOIN_TYPE_LEFT;
	    yyval.table_list=yyvsp[0].table_list;
	  }
    break;

  case 654:
#line 2187 "sql_yacc.yy"
    { add_join_on(yyvsp[-6].table_list,yyvsp[0].item); yyvsp[-6].table_list->outer_join|=JOIN_TYPE_RIGHT; yyval.table_list=yyvsp[-2].table_list; }
    break;

  case 655:
#line 2189 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->db1=yyvsp[-4].table_list->db; sel->table1=yyvsp[-4].table_list->alias;
	    sel->db2=yyvsp[0].table_list->db; sel->table2=yyvsp[0].table_list->alias;
	  }
    break;

  case 656:
#line 2195 "sql_yacc.yy"
    { add_join_on(yyvsp[-9].table_list,yyvsp[-1].item); yyvsp[-9].table_list->outer_join|=JOIN_TYPE_RIGHT; yyval.table_list=yyvsp[-5].table_list; }
    break;

  case 657:
#line 2197 "sql_yacc.yy"
    {
	    add_join_natural(yyvsp[-5].table_list->next,yyvsp[-5].table_list);
	    yyvsp[-5].table_list->outer_join|=JOIN_TYPE_RIGHT;
	    yyval.table_list=yyvsp[0].table_list;
	  }
    break;

  case 658:
#line 2203 "sql_yacc.yy"
    { add_join_natural(yyvsp[-3].table_list,yyvsp[-3].table_list->next); yyval.table_list=yyvsp[0].table_list; }
    break;

  case 659:
#line 2206 "sql_yacc.yy"
    {}
    break;

  case 660:
#line 2207 "sql_yacc.yy"
    {}
    break;

  case 661:
#line 2208 "sql_yacc.yy"
    {}
    break;

  case 662:
#line 2212 "sql_yacc.yy"
    {
	  SELECT_LEX *sel=Select;
	  sel->use_index_ptr=sel->ignore_index_ptr=0;
	  sel->table_join_options= 0;
	}
    break;

  case 663:
#line 2218 "sql_yacc.yy"
    {
	  SELECT_LEX *sel=Select;
	  if (!(yyval.table_list=add_table_to_list(yyvsp[-2].table, yyvsp[-1].lex_str_ptr, sel->table_join_options,
				     TL_UNLOCK, sel->use_index_ptr,
	                             sel->ignore_index_ptr)))
	    YYABORT;
	}
    break;

  case 664:
#line 2226 "sql_yacc.yy"
    { add_join_on(yyvsp[-3].table_list,yyvsp[-1].item); yyvsp[-3].table_list->outer_join|=JOIN_TYPE_LEFT; yyval.table_list=yyvsp[-3].table_list; }
    break;

  case 665:
#line 2229 "sql_yacc.yy"
    {}
    break;

  case 666:
#line 2230 "sql_yacc.yy"
    {}
    break;

  case 667:
#line 2233 "sql_yacc.yy"
    {}
    break;

  case 668:
#line 2235 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->use_index= *yyvsp[0].string_list;
	    sel->use_index_ptr= &sel->use_index;
	  }
    break;

  case 669:
#line 2241 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->use_index= *yyvsp[0].string_list;
	    sel->use_index_ptr= &sel->use_index;
	    sel->table_join_options|= TL_OPTION_FORCE_INDEX;
	  }
    break;

  case 670:
#line 2248 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    sel->ignore_index= *yyvsp[0].string_list;
	    sel->ignore_index_ptr= &sel->ignore_index;
	  }
    break;

  case 671:
#line 2256 "sql_yacc.yy"
    { Select->interval_list.empty(); }
    break;

  case 672:
#line 2257 "sql_yacc.yy"
    { yyval.string_list= &Select->interval_list; }
    break;

  case 673:
#line 2261 "sql_yacc.yy"
    { Select->interval_list.push_back(new String((const char*) yyvsp[0].lex_str.str,yyvsp[0].lex_str.length)); }
    break;

  case 674:
#line 2263 "sql_yacc.yy"
    { Select->interval_list.push_back(new String((const char*) yyvsp[0].lex_str.str,yyvsp[0].lex_str.length)); }
    break;

  case 675:
#line 2265 "sql_yacc.yy"
    { Select->interval_list.push_back(new String("PRIMARY",7)); }
    break;

  case 676:
#line 2269 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    if (!(yyval.item= new Item_func_eq(new Item_field(sel->db1,sel->table1, yyvsp[0].lex_str.str), new Item_field(sel->db2,sel->table2,yyvsp[0].lex_str.str))))
	      YYABORT;
	  }
    break;

  case 677:
#line 2275 "sql_yacc.yy"
    {
	    SELECT_LEX *sel=Select;
	    if (!(yyval.item= new Item_cond_and(new Item_func_eq(new Item_field(sel->db1,sel->table1,yyvsp[0].lex_str.str), new Item_field(sel->db2,sel->table2,yyvsp[0].lex_str.str)), yyvsp[-2].item)))
	      YYABORT;
	  }
    break;

  case 678:
#line 2282 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_HOUR; }
    break;

  case 679:
#line 2283 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_MINUTE; }
    break;

  case 680:
#line 2284 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY_SECOND; }
    break;

  case 681:
#line 2285 "sql_yacc.yy"
    { yyval.interval=INTERVAL_DAY; }
    break;

  case 682:
#line 2286 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR_MINUTE; }
    break;

  case 683:
#line 2287 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR_SECOND; }
    break;

  case 684:
#line 2288 "sql_yacc.yy"
    { yyval.interval=INTERVAL_HOUR; }
    break;

  case 685:
#line 2289 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MINUTE_SECOND; }
    break;

  case 686:
#line 2290 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MINUTE; }
    break;

  case 687:
#line 2291 "sql_yacc.yy"
    { yyval.interval=INTERVAL_MONTH; }
    break;

  case 688:
#line 2292 "sql_yacc.yy"
    { yyval.interval=INTERVAL_SECOND; }
    break;

  case 689:
#line 2293 "sql_yacc.yy"
    { yyval.interval=INTERVAL_YEAR_MONTH; }
    break;

  case 690:
#line 2294 "sql_yacc.yy"
    { yyval.interval=INTERVAL_YEAR; }
    break;

  case 694:
#line 2302 "sql_yacc.yy"
    { yyval.lex_str_ptr=0; }
    break;

  case 695:
#line 2304 "sql_yacc.yy"
    { yyval.lex_str_ptr= (LEX_STRING*) sql_memdup(&yyvsp[0].lex_str,sizeof(LEX_STRING)); }
    break;

  case 698:
#line 2312 "sql_yacc.yy"
    { Select->where= 0; }
    break;

  case 699:
#line 2314 "sql_yacc.yy"
    {
	    Select->where= yyvsp[0].item;
	    if (yyvsp[0].item)
	      yyvsp[0].item->top_level_item();
	  }
    break;

  case 701:
#line 2323 "sql_yacc.yy"
    { Select->create_refs=1; }
    break;

  case 702:
#line 2324 "sql_yacc.yy"
    {
	  SELECT_LEX *sel=Select;
	  sel->having= yyvsp[0].item;
	  sel->create_refs=0;
	  if (yyvsp[0].item)
	    yyvsp[0].item->top_level_item();
	}
    break;

  case 703:
#line 2334 "sql_yacc.yy"
    { yyval.simple_string= yyvsp[0].lex_str.str; }
    break;

  case 704:
#line 2335 "sql_yacc.yy"
    { yyval.simple_string= (char*) "\\"; }
    break;

  case 707:
#line 2348 "sql_yacc.yy"
    { if (add_group_to_list(yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 708:
#line 2350 "sql_yacc.yy"
    { if (add_group_to_list(yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 709:
#line 2353 "sql_yacc.yy"
    {}
    break;

  case 710:
#line 2355 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    net_printf(&lex->thd->net, ER_NOT_SUPPORTED_YET, "CUBE");
	    YYABORT;	/* To be deleted in 4.1 */
	  }
    break;

  case 711:
#line 2361 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    net_printf(&lex->thd->net, ER_NOT_SUPPORTED_YET, "ROLLUP");
	    YYABORT;	/* To be deleted in 4.1 */
	  }
    break;

  case 714:
#line 2378 "sql_yacc.yy"
    { 
	  LEX *lex=Lex;
	  if (lex->select->olap != UNSPECIFIED_OLAP_TYPE)
	  {
	    net_printf(&lex->thd->net, ER_WRONG_USAGE,
		       "CUBE/ROLLUP",
		       "ORDER BY");
	    YYABORT;
	  }
	  lex->select->sort_default=1;
	}
    break;

  case 716:
#line 2392 "sql_yacc.yy"
    { if (add_order_to_list(yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 717:
#line 2394 "sql_yacc.yy"
    { if (add_order_to_list(yyvsp[-1].item,(bool) yyvsp[0].num)) YYABORT; }
    break;

  case 718:
#line 2397 "sql_yacc.yy"
    { yyval.num =  1; }
    break;

  case 719:
#line 2398 "sql_yacc.yy"
    { yyval.num =1; }
    break;

  case 720:
#line 2399 "sql_yacc.yy"
    { yyval.num =0; }
    break;

  case 721:
#line 2403 "sql_yacc.yy"
    {}
    break;

  case 722:
#line 2405 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (lex->select->olap != UNSPECIFIED_OLAP_TYPE)
	    {
	      net_printf(&lex->thd->net, ER_WRONG_USAGE, "CUBE/ROLLUP",
		        "LIMIT");
	      YYABORT;
	    }
	  }
    break;

  case 723:
#line 2415 "sql_yacc.yy"
    {}
    break;

  case 724:
#line 2420 "sql_yacc.yy"
    {
            SELECT_LEX *sel= Select;
            sel->select_limit= yyvsp[0].ulong_num;
            sel->offset_limit= 0L;
	  }
    break;

  case 725:
#line 2426 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->select_limit= yyvsp[0].ulong_num;
	    sel->offset_limit= yyvsp[-2].ulong_num;
	  }
    break;

  case 726:
#line 2432 "sql_yacc.yy"
    {
	    SELECT_LEX *sel= Select;
	    sel->select_limit= yyvsp[-2].ulong_num;
	    sel->offset_limit= yyvsp[0].ulong_num;
	  }
    break;

  case 727:
#line 2441 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->select->select_limit= HA_POS_ERROR;
	}
    break;

  case 728:
#line 2446 "sql_yacc.yy"
    { Select->select_limit= (ha_rows) yyvsp[0].ulonglong_number; }
    break;

  case 729:
#line 2449 "sql_yacc.yy"
    { yyval.ulong_num= strtoul(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 730:
#line 2450 "sql_yacc.yy"
    { yyval.ulong_num= (ulong) strtoll(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 731:
#line 2451 "sql_yacc.yy"
    { yyval.ulong_num= (ulong) strtoull(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 732:
#line 2452 "sql_yacc.yy"
    { yyval.ulong_num= strtoul(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 733:
#line 2453 "sql_yacc.yy"
    { yyval.ulong_num= strtoul(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 734:
#line 2456 "sql_yacc.yy"
    { yyval.ulonglong_number= (ulonglong) strtoul(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 735:
#line 2457 "sql_yacc.yy"
    { yyval.ulonglong_number= strtoull(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 736:
#line 2458 "sql_yacc.yy"
    { yyval.ulonglong_number= (ulonglong) strtoll(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 737:
#line 2459 "sql_yacc.yy"
    { yyval.ulonglong_number= strtoull(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 738:
#line 2460 "sql_yacc.yy"
    { yyval.ulonglong_number= strtoull(yyvsp[0].lex_str.str,NULL,10); }
    break;

  case 740:
#line 2465 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->proc_list.elements=0;
	    lex->proc_list.first=0;
	    lex->proc_list.next= (byte**) &lex->proc_list.first;
	    if (add_proc_to_list(lex->thd, new Item_field(NULL,NULL,yyvsp[0].lex_str.str)))
	      YYABORT;
	    current_thd->safe_to_cache_query=0;
	  }
    break;

  case 742:
#line 2478 "sql_yacc.yy"
    {}
    break;

  case 743:
#line 2479 "sql_yacc.yy"
    {}
    break;

  case 746:
#line 2487 "sql_yacc.yy"
    {
	    LEX *lex= Lex;
	    if (add_proc_to_list(lex->thd, yyvsp[0].item))
	      YYABORT;
	    if (!yyvsp[0].item->name)
	      yyvsp[0].item->set_name(yyvsp[-1].simple_string,(uint) ((char*) lex->tok_end - yyvsp[-1].simple_string));
	  }
    break;

  case 747:
#line 2497 "sql_yacc.yy"
    {
	  THD *thd= current_thd;
	  thd->safe_to_cache_query= 0; 
	  if (!(thd->lex.exchange= new sql_exchange(yyvsp[0].lex_str.str,0)))
	    YYABORT;
	}
    break;

  case 749:
#line 2505 "sql_yacc.yy"
    {
	  THD *thd= current_thd;
	  thd->safe_to_cache_query= 0;
	  if (!(thd->lex.exchange= new sql_exchange(yyvsp[0].lex_str.str,1)))
	    YYABORT;
	}
    break;

  case 750:
#line 2517 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_DO;
	  if (!(lex->insert_list = new List_item))
	    YYABORT;
	}
    break;

  case 751:
#line 2524 "sql_yacc.yy"
    {}
    break;

  case 752:
#line 2533 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_DROP_TABLE;
	  lex->drop_temporary= yyvsp[-4].num;
	  lex->drop_if_exists= yyvsp[-2].num;
	}
    break;

  case 753:
#line 2539 "sql_yacc.yy"
    {}
    break;

  case 754:
#line 2540 "sql_yacc.yy"
    {
	     LEX *lex=Lex;
	     lex->sql_command= SQLCOM_DROP_INDEX;
	     lex->drop_list.empty();
	     lex->drop_list.push_back(new Alter_drop(Alter_drop::KEY,
						     yyvsp[-3].lex_str.str));
	     if (!add_table_to_list(yyvsp[-1].table, NULL, TL_OPTION_UPDATING))
	      YYABORT;
	  }
    break;

  case 755:
#line 2550 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_DROP_DB;
	    lex->drop_if_exists=yyvsp[-1].num;
	    lex->name=yyvsp[0].lex_str.str;
	 }
    break;

  case 756:
#line 2557 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command = SQLCOM_DROP_FUNCTION;
	    lex->udf.name=yyvsp[0].lex_str.str;
	  }
    break;

  case 759:
#line 2570 "sql_yacc.yy"
    { if (!add_table_to_list(yyvsp[0].table,NULL,TL_OPTION_UPDATING)) YYABORT; }
    break;

  case 760:
#line 2573 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 761:
#line 2574 "sql_yacc.yy"
    { yyval.num= 1; }
    break;

  case 762:
#line 2578 "sql_yacc.yy"
    { yyval.num= 0; }
    break;

  case 763:
#line 2579 "sql_yacc.yy"
    { yyval.num= 1; }
    break;

  case 764:
#line 2586 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_INSERT; }
    break;

  case 765:
#line 2588 "sql_yacc.yy"
    {
	  set_lock_for_tables(yyvsp[-2].lock_type);
	  Lex->select= &Lex->select_lex;
	}
    break;

  case 766:
#line 2593 "sql_yacc.yy"
    {}
    break;

  case 767:
#line 2598 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_REPLACE;
	  lex->duplicates= DUP_REPLACE;
	}
    break;

  case 768:
#line 2604 "sql_yacc.yy"
    {
	  set_lock_for_tables(yyvsp[-1].lock_type);
          Lex->select= &Lex->select_lex;
	}
    break;

  case 769:
#line 2609 "sql_yacc.yy"
    {}
    break;

  case 770:
#line 2613 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_CONCURRENT_INSERT; }
    break;

  case 771:
#line 2614 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_LOW_PRIORITY; }
    break;

  case 772:
#line 2615 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_DELAYED; }
    break;

  case 773:
#line 2616 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE; }
    break;

  case 774:
#line 2620 "sql_yacc.yy"
    { yyval.lock_type= yyvsp[0].lock_type; }
    break;

  case 775:
#line 2621 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_DELAYED; }
    break;

  case 776:
#line 2624 "sql_yacc.yy"
    {}
    break;

  case 777:
#line 2625 "sql_yacc.yy"
    {}
    break;

  case 778:
#line 2629 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->field_list.empty();
	  lex->many_values.empty();
	  lex->insert_list=0;
	}
    break;

  case 779:
#line 2637 "sql_yacc.yy"
    {}
    break;

  case 780:
#line 2638 "sql_yacc.yy"
    {}
    break;

  case 781:
#line 2639 "sql_yacc.yy"
    {}
    break;

  case 782:
#line 2641 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (!(lex->insert_list = new List_item) ||
		lex->many_values.push_back(lex->insert_list))
	      YYABORT;
	   }
    break;

  case 784:
#line 2650 "sql_yacc.yy"
    { }
    break;

  case 785:
#line 2651 "sql_yacc.yy"
    { }
    break;

  case 786:
#line 2652 "sql_yacc.yy"
    { }
    break;

  case 787:
#line 2655 "sql_yacc.yy"
    { Lex->field_list.push_back(yyvsp[0].item); }
    break;

  case 788:
#line 2656 "sql_yacc.yy"
    { Lex->field_list.push_back(yyvsp[0].item); }
    break;

  case 789:
#line 2659 "sql_yacc.yy"
    {}
    break;

  case 790:
#line 2660 "sql_yacc.yy"
    { Select->braces= 0;}
    break;

  case 791:
#line 2660 "sql_yacc.yy"
    {}
    break;

  case 792:
#line 2661 "sql_yacc.yy"
    { Select->braces= 1;}
    break;

  case 793:
#line 2661 "sql_yacc.yy"
    {}
    break;

  case 798:
#line 2675 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->field_list.push_back(yyvsp[-2].item) ||
	      lex->insert_list->push_back(yyvsp[0].item))
	    YYABORT;
	 }
    break;

  case 799:
#line 2682 "sql_yacc.yy"
    {}
    break;

  case 800:
#line 2683 "sql_yacc.yy"
    {}
    break;

  case 801:
#line 2687 "sql_yacc.yy"
    {}
    break;

  case 802:
#line 2688 "sql_yacc.yy"
    {}
    break;

  case 803:
#line 2693 "sql_yacc.yy"
    {
	    if (!(Lex->insert_list = new List_item))
	      YYABORT;
	 }
    break;

  case 804:
#line 2698 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->many_values.push_back(lex->insert_list))
	    YYABORT;
	 }
    break;

  case 805:
#line 2705 "sql_yacc.yy"
    {}
    break;

  case 807:
#line 2710 "sql_yacc.yy"
    {
	  if (Lex->insert_list->push_back(yyvsp[0].item))
	    YYABORT;
	}
    break;

  case 808:
#line 2715 "sql_yacc.yy"
    {
	    if (Lex->insert_list->push_back(yyvsp[0].item))
	      YYABORT;
	  }
    break;

  case 809:
#line 2722 "sql_yacc.yy"
    { yyval.item= yyvsp[0].item;}
    break;

  case 810:
#line 2723 "sql_yacc.yy"
    {yyval.item= new Item_default(); }
    break;

  case 811:
#line 2730 "sql_yacc.yy"
    { 
	  LEX *lex=Lex;
          lex->sql_command = SQLCOM_UPDATE;
          lex->select->order_list.elements=0;
          lex->select->order_list.first=0;
          lex->select->order_list.next= (byte**) &lex->select->order_list.first;
        }
    break;

  case 812:
#line 2739 "sql_yacc.yy"
    {
	  set_lock_for_tables(yyvsp[-7].lock_type);
	}
    break;

  case 813:
#line 2746 "sql_yacc.yy"
    {
	  if (add_item_to_list(yyvsp[-2].item) || add_value_to_list(yyvsp[0].item))
	    YYABORT;
	}
    break;

  case 814:
#line 2751 "sql_yacc.yy"
    {
	    if (add_item_to_list(yyvsp[-2].item) || add_value_to_list(yyvsp[0].item))
	      YYABORT;
	  }
    break;

  case 815:
#line 2757 "sql_yacc.yy"
    { yyval.lock_type= current_thd->update_lock_default; }
    break;

  case 816:
#line 2758 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_LOW_PRIORITY; }
    break;

  case 817:
#line 2764 "sql_yacc.yy"
    { 
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_DELETE; lex->select->options=0;
	  lex->lock_option= lex->thd->update_lock_default;
	  lex->select->order_list.elements=0;
	  lex->select->order_list.first=0;
	  lex->select->order_list.next= (byte**) &lex->select->order_list.first;
	}
    break;

  case 818:
#line 2772 "sql_yacc.yy"
    {}
    break;

  case 819:
#line 2777 "sql_yacc.yy"
    {
	  if (!add_table_to_list(yyvsp[0].table, NULL, TL_OPTION_UPDATING,
				 Lex->lock_option))
	    YYABORT;
	}
    break;

  case 820:
#line 2783 "sql_yacc.yy"
    {}
    break;

  case 821:
#line 2785 "sql_yacc.yy"
    { mysql_init_multi_delete(Lex); }
    break;

  case 823:
#line 2788 "sql_yacc.yy"
    { mysql_init_multi_delete(Lex); }
    break;

  case 824:
#line 2790 "sql_yacc.yy"
    {}
    break;

  case 825:
#line 2794 "sql_yacc.yy"
    {}
    break;

  case 826:
#line 2795 "sql_yacc.yy"
    {}
    break;

  case 827:
#line 2799 "sql_yacc.yy"
    {
	   if (!add_table_to_list(new Table_ident(yyvsp[-1].lex_str), NULL,
				  TL_OPTION_UPDATING, Lex->lock_option))
	     YYABORT;
         }
    break;

  case 828:
#line 2805 "sql_yacc.yy"
    {
	     if (!add_table_to_list(new Table_ident(yyvsp[-3].lex_str,yyvsp[-1].lex_str,0), NULL,
				    TL_OPTION_UPDATING,
				    Lex->lock_option))
	      YYABORT;
	   }
    break;

  case 829:
#line 2814 "sql_yacc.yy"
    {}
    break;

  case 830:
#line 2815 "sql_yacc.yy"
    {}
    break;

  case 831:
#line 2819 "sql_yacc.yy"
    {}
    break;

  case 832:
#line 2820 "sql_yacc.yy"
    {}
    break;

  case 833:
#line 2823 "sql_yacc.yy"
    { Select->options|= OPTION_QUICK; }
    break;

  case 834:
#line 2824 "sql_yacc.yy"
    { Lex->lock_option= TL_WRITE_LOW_PRIORITY; }
    break;

  case 835:
#line 2828 "sql_yacc.yy"
    {
	  LEX* lex = Lex;
	  lex->sql_command= SQLCOM_TRUNCATE;
	  lex->select->options=0;
	  lex->select->order_list.elements=0;
          lex->select->order_list.first=0;
          lex->select->order_list.next= (byte**) &lex->select->order_list.first;
	}
    break;

  case 838:
#line 2844 "sql_yacc.yy"
    { Lex->wild=0;}
    break;

  case 839:
#line 2845 "sql_yacc.yy"
    {}
    break;

  case 840:
#line 2850 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_DATABASES; }
    break;

  case 841:
#line 2852 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_TABLES;
	    lex->select->db= yyvsp[-1].simple_string; lex->select->options=0;
	   }
    break;

  case 842:
#line 2858 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_TABLES;
	    lex->select->options|= SELECT_DESCRIBE;
	    lex->select->db= yyvsp[-1].simple_string;
	  }
    break;

  case 843:
#line 2865 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_OPEN_TABLES;
	    lex->select->db= yyvsp[-1].simple_string;
	    lex->select->options=0;
	  }
    break;

  case 844:
#line 2872 "sql_yacc.yy"
    {
	    Lex->sql_command= SQLCOM_SHOW_FIELDS;
	    if (yyvsp[-1].simple_string)
	      yyvsp[-2].table->change_db(yyvsp[-1].simple_string);
	    if (!add_table_to_list(yyvsp[-2].table, NULL, 0))
	      YYABORT;
	  }
    break;

  case 845:
#line 2883 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_NEW_MASTER;
	    Lex->mi.log_file_name = yyvsp[-8].lex_str.str;
	    Lex->mi.pos = yyvsp[-4].ulonglong_number;
	    Lex->mi.server_id = yyvsp[0].ulong_num;
          }
    break;

  case 846:
#line 2890 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_BINLOGS;
          }
    break;

  case 847:
#line 2894 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_SLAVE_HOSTS;
          }
    break;

  case 848:
#line 2898 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command = SQLCOM_SHOW_BINLOG_EVENTS;
	    lex->select->select_limit= lex->thd->variables.select_limit;
	    lex->select->offset_limit= 0L;
          }
    break;

  case 850:
#line 2905 "sql_yacc.yy"
    {
	    Lex->sql_command= SQLCOM_SHOW_KEYS;
	    if (yyvsp[0].simple_string)
	      yyvsp[-1].table->change_db(yyvsp[0].simple_string);
	    if (!add_table_to_list(yyvsp[-1].table, NULL, 0))
	      YYABORT;
	  }
    break;

  case 851:
#line 2913 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_STATUS; }
    break;

  case 852:
#line 2915 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_SHOW_INNODB_STATUS;}
    break;

  case 853:
#line 2917 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_PROCESSLIST;}
    break;

  case 854:
#line 2919 "sql_yacc.yy"
    {
	    THD *thd= current_thd;
	    thd->lex.sql_command= SQLCOM_SHOW_VARIABLES;
	    thd->lex.option_type= (enum_var_type) yyvsp[-2].num;
	  }
    break;

  case 855:
#line 2925 "sql_yacc.yy"
    { Lex->sql_command= SQLCOM_SHOW_LOGS; }
    break;

  case 856:
#line 2927 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->sql_command= SQLCOM_SHOW_GRANTS;
	    lex->grant_user=yyvsp[0].lex_user;
	    lex->grant_user->password.str=NullS;
	  }
    break;

  case 857:
#line 2934 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_CREATE;
	    if(!add_table_to_list(yyvsp[0].table, NULL, 0))
	      YYABORT;
	  }
    break;

  case 858:
#line 2940 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_MASTER_STAT;
          }
    break;

  case 859:
#line 2944 "sql_yacc.yy"
    {
	    Lex->sql_command = SQLCOM_SHOW_SLAVE_STAT;
          }
    break;

  case 860:
#line 2949 "sql_yacc.yy"
    { yyval.simple_string= 0; }
    break;

  case 861:
#line 2950 "sql_yacc.yy"
    { yyval.simple_string= yyvsp[0].lex_str.str; }
    break;

  case 863:
#line 2954 "sql_yacc.yy"
    { Lex->wild= yyvsp[0].string; }
    break;

  case 864:
#line 2957 "sql_yacc.yy"
    { Lex->verbose=0; }
    break;

  case 865:
#line 2958 "sql_yacc.yy"
    { Lex->verbose=1; }
    break;

  case 868:
#line 2965 "sql_yacc.yy"
    { Lex->mi.log_file_name = 0; }
    break;

  case 869:
#line 2966 "sql_yacc.yy"
    { Lex->mi.log_file_name = yyvsp[0].lex_str.str; }
    break;

  case 870:
#line 2969 "sql_yacc.yy"
    { Lex->mi.pos = 4; /* skip magic number */ }
    break;

  case 871:
#line 2970 "sql_yacc.yy"
    { Lex->mi.pos = yyvsp[0].ulonglong_number; }
    break;

  case 872:
#line 2976 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->wild=0;
	  lex->verbose=0;
	  lex->sql_command=SQLCOM_SHOW_FIELDS;
	  if (!add_table_to_list(yyvsp[0].table, NULL, 0))
	    YYABORT;
	}
    break;

  case 873:
#line 2984 "sql_yacc.yy"
    {}
    break;

  case 874:
#line 2986 "sql_yacc.yy"
    { Lex->select_lex.options|= SELECT_DESCRIBE; }
    break;

  case 877:
#line 2994 "sql_yacc.yy"
    {}
    break;

  case 878:
#line 2995 "sql_yacc.yy"
    { Lex->wild= yyvsp[0].string; }
    break;

  case 879:
#line 2997 "sql_yacc.yy"
    { Lex->wild= new String((const char*) yyvsp[0].lex_str.str,yyvsp[0].lex_str.length); }
    break;

  case 880:
#line 3004 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_FLUSH; lex->type=0;
	}
    break;

  case 881:
#line 3009 "sql_yacc.yy"
    {}
    break;

  case 884:
#line 3017 "sql_yacc.yy"
    { Lex->type|= REFRESH_TABLES; }
    break;

  case 885:
#line 3017 "sql_yacc.yy"
    {}
    break;

  case 886:
#line 3018 "sql_yacc.yy"
    { Lex->type|= REFRESH_TABLES | REFRESH_READ_LOCK; }
    break;

  case 887:
#line 3019 "sql_yacc.yy"
    { Lex->type|= REFRESH_QUERY_CACHE_FREE; }
    break;

  case 888:
#line 3020 "sql_yacc.yy"
    { Lex->type|= REFRESH_HOSTS; }
    break;

  case 889:
#line 3021 "sql_yacc.yy"
    { Lex->type|= REFRESH_GRANT; }
    break;

  case 890:
#line 3022 "sql_yacc.yy"
    { Lex->type|= REFRESH_LOG; }
    break;

  case 891:
#line 3023 "sql_yacc.yy"
    { Lex->type|= REFRESH_STATUS; }
    break;

  case 892:
#line 3024 "sql_yacc.yy"
    { Lex->type|= REFRESH_SLAVE; }
    break;

  case 893:
#line 3025 "sql_yacc.yy"
    { Lex->type|= REFRESH_MASTER; }
    break;

  case 894:
#line 3026 "sql_yacc.yy"
    { Lex->type|= REFRESH_DES_KEY_FILE; }
    break;

  case 895:
#line 3027 "sql_yacc.yy"
    { Lex->type|= REFRESH_USER_RESOURCES; }
    break;

  case 896:
#line 3030 "sql_yacc.yy"
    {;}
    break;

  case 897:
#line 3031 "sql_yacc.yy"
    {;}
    break;

  case 898:
#line 3035 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_RESET; lex->type=0;
	}
    break;

  case 899:
#line 3039 "sql_yacc.yy"
    {}
    break;

  case 902:
#line 3047 "sql_yacc.yy"
    { Lex->type|= REFRESH_SLAVE; }
    break;

  case 903:
#line 3048 "sql_yacc.yy"
    { Lex->type|= REFRESH_MASTER; }
    break;

  case 904:
#line 3049 "sql_yacc.yy"
    { Lex->type|= REFRESH_QUERY_CACHE;}
    break;

  case 905:
#line 3053 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_PURGE;
	  lex->type=0;
	}
    break;

  case 906:
#line 3059 "sql_yacc.yy"
    {
	   Lex->to_log = yyvsp[0].lex_str.str;
         }
    break;

  case 907:
#line 3067 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (yyvsp[0].item->fix_fields(lex->thd,0))
	  { 
	    send_error(&lex->thd->net, ER_SET_CONSTANTS_ONLY);
	    YYABORT;
	  }
          lex->sql_command=SQLCOM_KILL;
	  lex->thread_id= (ulong) yyvsp[0].item->val_int();
	}
    break;

  case 908:
#line 3081 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command=SQLCOM_CHANGE_DB; lex->select->db= yyvsp[0].lex_str.str;
	}
    break;

  case 909:
#line 3089 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_LOAD;
	  lex->lock_option= yyvsp[-3].lock_type;
	  lex->local_file=  yyvsp[-2].num;
	  if (!(lex->exchange= new sql_exchange(yyvsp[0].lex_str.str,0)))
	    YYABORT;
	  lex->field_list.empty();
	}
    break;

  case 910:
#line 3100 "sql_yacc.yy"
    {
	  if (!add_table_to_list(yyvsp[-4].table, NULL, TL_OPTION_UPDATING))
	    YYABORT;
	}
    break;

  case 911:
#line 3106 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_LOAD_MASTER_TABLE;
	  if (!add_table_to_list(yyvsp[-2].table, NULL, TL_OPTION_UPDATING))
	    YYABORT;

        }
    break;

  case 912:
#line 3114 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_LOAD_MASTER_DATA;
        }
    break;

  case 913:
#line 3119 "sql_yacc.yy"
    { yyval.num=0;}
    break;

  case 914:
#line 3120 "sql_yacc.yy"
    { yyval.num=1;}
    break;

  case 915:
#line 3123 "sql_yacc.yy"
    { yyval.lock_type= current_thd->update_lock_default; }
    break;

  case 916:
#line 3124 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_CONCURRENT_INSERT ; }
    break;

  case 917:
#line 3125 "sql_yacc.yy"
    { yyval.lock_type= TL_WRITE_LOW_PRIORITY; }
    break;

  case 918:
#line 3129 "sql_yacc.yy"
    { Lex->duplicates=DUP_ERROR; }
    break;

  case 919:
#line 3130 "sql_yacc.yy"
    { Lex->duplicates=DUP_REPLACE; }
    break;

  case 920:
#line 3131 "sql_yacc.yy"
    { Lex->duplicates=DUP_IGNORE; }
    break;

  case 925:
#line 3142 "sql_yacc.yy"
    { Lex->exchange->field_term= yyvsp[0].string;}
    break;

  case 926:
#line 3144 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->exchange->enclosed= yyvsp[0].string;
	    lex->exchange->opt_enclosed=1;
	  }
    break;

  case 927:
#line 3149 "sql_yacc.yy"
    { Lex->exchange->enclosed= yyvsp[0].string;}
    break;

  case 928:
#line 3150 "sql_yacc.yy"
    { Lex->exchange->escaped= yyvsp[0].string;}
    break;

  case 933:
#line 3161 "sql_yacc.yy"
    { Lex->exchange->line_term= yyvsp[0].string;}
    break;

  case 934:
#line 3162 "sql_yacc.yy"
    { Lex->exchange->line_start= yyvsp[0].string;}
    break;

  case 936:
#line 3167 "sql_yacc.yy"
    { Lex->exchange->skip_lines=atol(yyvsp[-1].lex_str.str); }
    break;

  case 937:
#line 3172 "sql_yacc.yy"
    { yyval.item = new Item_string(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length); }
    break;

  case 938:
#line 3174 "sql_yacc.yy"
    { ((Item_string*) yyvsp[-1].item)->append(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length); }
    break;

  case 939:
#line 3177 "sql_yacc.yy"
    { yyval.string=  new String(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length); }
    break;

  case 940:
#line 3179 "sql_yacc.yy"
    {
	    Item *tmp = new Item_varbinary(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length);
	    yyval.string= tmp ? tmp->val_str((String*) 0) : (String*) 0;
	  }
    break;

  case 941:
#line 3185 "sql_yacc.yy"
    { yyval.item =	yyvsp[0].item; }
    break;

  case 942:
#line 3186 "sql_yacc.yy"
    { yyval.item =	new Item_int(yyvsp[0].lex_str.str, (longlong) strtol(yyvsp[0].lex_str.str, NULL, 10),yyvsp[0].lex_str.length); }
    break;

  case 943:
#line 3187 "sql_yacc.yy"
    { yyval.item =	new Item_int(yyvsp[0].lex_str.str, (longlong) strtoll(yyvsp[0].lex_str.str,NULL,10), yyvsp[0].lex_str.length); }
    break;

  case 944:
#line 3188 "sql_yacc.yy"
    { yyval.item =	new Item_uint(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length); }
    break;

  case 945:
#line 3189 "sql_yacc.yy"
    { yyval.item =	new Item_real(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length); }
    break;

  case 946:
#line 3190 "sql_yacc.yy"
    { yyval.item =	new Item_float(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length); }
    break;

  case 947:
#line 3191 "sql_yacc.yy"
    { yyval.item =	new Item_null();
			  Lex->next_state=STATE_OPERATOR_OR_IDENT;}
    break;

  case 948:
#line 3193 "sql_yacc.yy"
    { yyval.item =	new Item_varbinary(yyvsp[0].lex_str.str,yyvsp[0].lex_str.length);}
    break;

  case 949:
#line 3194 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 950:
#line 3195 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 951:
#line 3196 "sql_yacc.yy"
    { yyval.item = yyvsp[0].item; }
    break;

  case 952:
#line 3203 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 953:
#line 3204 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 954:
#line 3207 "sql_yacc.yy"
    { yyval.item = new Item_field(NullS,yyvsp[-2].lex_str.str,"*"); }
    break;

  case 955:
#line 3209 "sql_yacc.yy"
    { yyval.item = new Item_field((current_thd->client_capabilities &
   CLIENT_NO_SCHEMA ? NullS : yyvsp[-4].lex_str.str),yyvsp[-2].lex_str.str,"*"); }
    break;

  case 956:
#line 3213 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 957:
#line 3217 "sql_yacc.yy"
    {
	  SELECT_LEX *sel=Select;
	  yyval.item = !sel->create_refs || sel->in_sum_expr > 0 ? (Item*) new Item_field(NullS,NullS,yyvsp[0].lex_str.str) : (Item*) new Item_ref(NullS,NullS,yyvsp[0].lex_str.str);
	}
    break;

  case 958:
#line 3222 "sql_yacc.yy"
    {
	  SELECT_LEX *sel=Select;
	  yyval.item = !sel->create_refs || sel->in_sum_expr > 0 ? (Item*) new Item_field(NullS,yyvsp[-2].lex_str.str,yyvsp[0].lex_str.str) : (Item*) new Item_ref(NullS,yyvsp[-2].lex_str.str,yyvsp[0].lex_str.str);
	}
    break;

  case 959:
#line 3227 "sql_yacc.yy"
    {
	  SELECT_LEX *sel=Select;
	  yyval.item = !sel->create_refs || sel->in_sum_expr > 0 ? (Item*) new Item_field(NullS,yyvsp[-2].lex_str.str,yyvsp[0].lex_str.str) : (Item*) new Item_ref(NullS,yyvsp[-2].lex_str.str,yyvsp[0].lex_str.str);
	}
    break;

  case 960:
#line 3232 "sql_yacc.yy"
    {
	  SELECT_LEX *sel=Select;
	  yyval.item = !sel->create_refs || sel->in_sum_expr > 0 ? (Item*) new Item_field((current_thd->client_capabilities & CLIENT_NO_SCHEMA ? NullS :yyvsp[-4].lex_str.str),yyvsp[-2].lex_str.str,yyvsp[0].lex_str.str) : (Item*) new Item_ref((current_thd->client_capabilities & CLIENT_NO_SCHEMA ? NullS :yyvsp[-4].lex_str.str),yyvsp[-2].lex_str.str,yyvsp[0].lex_str.str);
	}
    break;

  case 961:
#line 3239 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 962:
#line 3240 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 963:
#line 3241 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 964:
#line 3244 "sql_yacc.yy"
    { yyval.table=new Table_ident(yyvsp[0].lex_str); }
    break;

  case 965:
#line 3245 "sql_yacc.yy"
    { yyval.table=new Table_ident(yyvsp[-2].lex_str,yyvsp[0].lex_str,0);}
    break;

  case 966:
#line 3246 "sql_yacc.yy"
    { yyval.table=new Table_ident(yyvsp[0].lex_str);}
    break;

  case 967:
#line 3250 "sql_yacc.yy"
    { LEX_STRING db={"",0}; yyval.table=new Table_ident(db,yyvsp[0].lex_str,0); }
    break;

  case 968:
#line 3251 "sql_yacc.yy"
    { yyval.table=new Table_ident(yyvsp[-2].lex_str,yyvsp[0].lex_str,0);}
    break;

  case 969:
#line 3255 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str; }
    break;

  case 970:
#line 3257 "sql_yacc.yy"
    {
	  LEX *lex= Lex;
	  yyval.lex_str.str= lex->thd->strmake(yyvsp[0].symbol.str,yyvsp[0].symbol.length);
	  yyval.lex_str.length=yyvsp[0].symbol.length;
	  if (lex->next_state != STATE_END)
	    lex->next_state=STATE_OPERATOR_OR_IDENT;
	}
    break;

  case 971:
#line 3267 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 972:
#line 3268 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 973:
#line 3269 "sql_yacc.yy"
    { yyval.lex_str=yyvsp[0].lex_str;}
    break;

  case 974:
#line 3273 "sql_yacc.yy"
    {
	  if (!(yyval.lex_user=(LEX_USER*) sql_alloc(sizeof(st_lex_user))))
	    YYABORT;
	  yyval.lex_user->user = yyvsp[0].lex_str; yyval.lex_user->host.str=NullS;
	  }
    break;

  case 975:
#line 3279 "sql_yacc.yy"
    {
	  if (!(yyval.lex_user=(LEX_USER*) sql_alloc(sizeof(st_lex_user))))
	      YYABORT;
	    yyval.lex_user->user = yyvsp[-2].lex_str; yyval.lex_user->host=yyvsp[0].lex_str;
	  }
    break;

  case 976:
#line 3288 "sql_yacc.yy"
    {}
    break;

  case 977:
#line 3289 "sql_yacc.yy"
    {}
    break;

  case 978:
#line 3290 "sql_yacc.yy"
    {}
    break;

  case 979:
#line 3291 "sql_yacc.yy"
    {}
    break;

  case 980:
#line 3292 "sql_yacc.yy"
    {}
    break;

  case 981:
#line 3293 "sql_yacc.yy"
    {}
    break;

  case 982:
#line 3294 "sql_yacc.yy"
    {}
    break;

  case 983:
#line 3295 "sql_yacc.yy"
    {}
    break;

  case 984:
#line 3296 "sql_yacc.yy"
    {}
    break;

  case 985:
#line 3297 "sql_yacc.yy"
    {}
    break;

  case 986:
#line 3298 "sql_yacc.yy"
    {}
    break;

  case 987:
#line 3299 "sql_yacc.yy"
    {}
    break;

  case 988:
#line 3300 "sql_yacc.yy"
    {}
    break;

  case 989:
#line 3301 "sql_yacc.yy"
    {}
    break;

  case 990:
#line 3302 "sql_yacc.yy"
    {}
    break;

  case 991:
#line 3303 "sql_yacc.yy"
    {}
    break;

  case 992:
#line 3304 "sql_yacc.yy"
    {}
    break;

  case 993:
#line 3305 "sql_yacc.yy"
    {}
    break;

  case 994:
#line 3306 "sql_yacc.yy"
    {}
    break;

  case 995:
#line 3307 "sql_yacc.yy"
    {}
    break;

  case 996:
#line 3308 "sql_yacc.yy"
    {}
    break;

  case 997:
#line 3309 "sql_yacc.yy"
    {}
    break;

  case 998:
#line 3310 "sql_yacc.yy"
    {}
    break;

  case 999:
#line 3311 "sql_yacc.yy"
    {}
    break;

  case 1000:
#line 3312 "sql_yacc.yy"
    {}
    break;

  case 1001:
#line 3313 "sql_yacc.yy"
    {}
    break;

  case 1002:
#line 3314 "sql_yacc.yy"
    {}
    break;

  case 1003:
#line 3315 "sql_yacc.yy"
    {}
    break;

  case 1004:
#line 3316 "sql_yacc.yy"
    {}
    break;

  case 1005:
#line 3317 "sql_yacc.yy"
    {}
    break;

  case 1006:
#line 3318 "sql_yacc.yy"
    {}
    break;

  case 1007:
#line 3319 "sql_yacc.yy"
    {}
    break;

  case 1008:
#line 3320 "sql_yacc.yy"
    {}
    break;

  case 1009:
#line 3321 "sql_yacc.yy"
    {}
    break;

  case 1010:
#line 3322 "sql_yacc.yy"
    {}
    break;

  case 1011:
#line 3323 "sql_yacc.yy"
    {}
    break;

  case 1012:
#line 3324 "sql_yacc.yy"
    {}
    break;

  case 1013:
#line 3325 "sql_yacc.yy"
    {}
    break;

  case 1014:
#line 3326 "sql_yacc.yy"
    {}
    break;

  case 1015:
#line 3327 "sql_yacc.yy"
    {}
    break;

  case 1016:
#line 3328 "sql_yacc.yy"
    {}
    break;

  case 1017:
#line 3329 "sql_yacc.yy"
    {}
    break;

  case 1018:
#line 3330 "sql_yacc.yy"
    {}
    break;

  case 1019:
#line 3331 "sql_yacc.yy"
    {}
    break;

  case 1020:
#line 3332 "sql_yacc.yy"
    {}
    break;

  case 1021:
#line 3333 "sql_yacc.yy"
    {}
    break;

  case 1022:
#line 3334 "sql_yacc.yy"
    {}
    break;

  case 1023:
#line 3335 "sql_yacc.yy"
    {}
    break;

  case 1024:
#line 3336 "sql_yacc.yy"
    {}
    break;

  case 1025:
#line 3337 "sql_yacc.yy"
    {}
    break;

  case 1026:
#line 3338 "sql_yacc.yy"
    {}
    break;

  case 1027:
#line 3339 "sql_yacc.yy"
    {}
    break;

  case 1028:
#line 3340 "sql_yacc.yy"
    {}
    break;

  case 1029:
#line 3341 "sql_yacc.yy"
    {}
    break;

  case 1030:
#line 3342 "sql_yacc.yy"
    {}
    break;

  case 1031:
#line 3343 "sql_yacc.yy"
    {}
    break;

  case 1032:
#line 3344 "sql_yacc.yy"
    {}
    break;

  case 1033:
#line 3345 "sql_yacc.yy"
    {}
    break;

  case 1034:
#line 3346 "sql_yacc.yy"
    {}
    break;

  case 1035:
#line 3347 "sql_yacc.yy"
    {}
    break;

  case 1036:
#line 3348 "sql_yacc.yy"
    {}
    break;

  case 1037:
#line 3349 "sql_yacc.yy"
    {}
    break;

  case 1038:
#line 3350 "sql_yacc.yy"
    {}
    break;

  case 1039:
#line 3351 "sql_yacc.yy"
    {}
    break;

  case 1040:
#line 3352 "sql_yacc.yy"
    {}
    break;

  case 1041:
#line 3353 "sql_yacc.yy"
    {}
    break;

  case 1042:
#line 3354 "sql_yacc.yy"
    {}
    break;

  case 1043:
#line 3355 "sql_yacc.yy"
    {}
    break;

  case 1044:
#line 3356 "sql_yacc.yy"
    {}
    break;

  case 1045:
#line 3357 "sql_yacc.yy"
    {}
    break;

  case 1046:
#line 3358 "sql_yacc.yy"
    {}
    break;

  case 1047:
#line 3359 "sql_yacc.yy"
    {}
    break;

  case 1048:
#line 3360 "sql_yacc.yy"
    {}
    break;

  case 1049:
#line 3361 "sql_yacc.yy"
    {}
    break;

  case 1050:
#line 3362 "sql_yacc.yy"
    {}
    break;

  case 1051:
#line 3363 "sql_yacc.yy"
    {}
    break;

  case 1052:
#line 3364 "sql_yacc.yy"
    {}
    break;

  case 1053:
#line 3365 "sql_yacc.yy"
    {}
    break;

  case 1054:
#line 3366 "sql_yacc.yy"
    {}
    break;

  case 1055:
#line 3367 "sql_yacc.yy"
    {}
    break;

  case 1056:
#line 3368 "sql_yacc.yy"
    {}
    break;

  case 1057:
#line 3369 "sql_yacc.yy"
    {}
    break;

  case 1058:
#line 3370 "sql_yacc.yy"
    {}
    break;

  case 1059:
#line 3371 "sql_yacc.yy"
    {}
    break;

  case 1060:
#line 3372 "sql_yacc.yy"
    {}
    break;

  case 1061:
#line 3373 "sql_yacc.yy"
    {}
    break;

  case 1062:
#line 3374 "sql_yacc.yy"
    {}
    break;

  case 1063:
#line 3375 "sql_yacc.yy"
    {}
    break;

  case 1064:
#line 3376 "sql_yacc.yy"
    {}
    break;

  case 1065:
#line 3377 "sql_yacc.yy"
    {}
    break;

  case 1066:
#line 3378 "sql_yacc.yy"
    {}
    break;

  case 1067:
#line 3379 "sql_yacc.yy"
    {}
    break;

  case 1068:
#line 3380 "sql_yacc.yy"
    {}
    break;

  case 1069:
#line 3381 "sql_yacc.yy"
    {}
    break;

  case 1070:
#line 3382 "sql_yacc.yy"
    {}
    break;

  case 1071:
#line 3383 "sql_yacc.yy"
    {}
    break;

  case 1072:
#line 3384 "sql_yacc.yy"
    {}
    break;

  case 1073:
#line 3385 "sql_yacc.yy"
    {}
    break;

  case 1074:
#line 3386 "sql_yacc.yy"
    {}
    break;

  case 1075:
#line 3387 "sql_yacc.yy"
    {}
    break;

  case 1076:
#line 3388 "sql_yacc.yy"
    {}
    break;

  case 1077:
#line 3389 "sql_yacc.yy"
    {}
    break;

  case 1078:
#line 3390 "sql_yacc.yy"
    {}
    break;

  case 1079:
#line 3391 "sql_yacc.yy"
    {}
    break;

  case 1080:
#line 3392 "sql_yacc.yy"
    {}
    break;

  case 1081:
#line 3393 "sql_yacc.yy"
    {}
    break;

  case 1082:
#line 3394 "sql_yacc.yy"
    {}
    break;

  case 1083:
#line 3395 "sql_yacc.yy"
    {}
    break;

  case 1084:
#line 3396 "sql_yacc.yy"
    {}
    break;

  case 1085:
#line 3397 "sql_yacc.yy"
    {}
    break;

  case 1086:
#line 3398 "sql_yacc.yy"
    {}
    break;

  case 1087:
#line 3399 "sql_yacc.yy"
    {}
    break;

  case 1088:
#line 3400 "sql_yacc.yy"
    {}
    break;

  case 1089:
#line 3401 "sql_yacc.yy"
    {}
    break;

  case 1090:
#line 3402 "sql_yacc.yy"
    {}
    break;

  case 1091:
#line 3403 "sql_yacc.yy"
    {}
    break;

  case 1092:
#line 3404 "sql_yacc.yy"
    {}
    break;

  case 1093:
#line 3405 "sql_yacc.yy"
    {}
    break;

  case 1094:
#line 3406 "sql_yacc.yy"
    {}
    break;

  case 1095:
#line 3407 "sql_yacc.yy"
    {}
    break;

  case 1096:
#line 3408 "sql_yacc.yy"
    {}
    break;

  case 1097:
#line 3409 "sql_yacc.yy"
    {}
    break;

  case 1098:
#line 3410 "sql_yacc.yy"
    {}
    break;

  case 1099:
#line 3411 "sql_yacc.yy"
    {}
    break;

  case 1100:
#line 3412 "sql_yacc.yy"
    {}
    break;

  case 1101:
#line 3413 "sql_yacc.yy"
    {}
    break;

  case 1102:
#line 3414 "sql_yacc.yy"
    {}
    break;

  case 1103:
#line 3415 "sql_yacc.yy"
    {}
    break;

  case 1104:
#line 3416 "sql_yacc.yy"
    {}
    break;

  case 1105:
#line 3417 "sql_yacc.yy"
    {}
    break;

  case 1106:
#line 3418 "sql_yacc.yy"
    {}
    break;

  case 1107:
#line 3419 "sql_yacc.yy"
    {}
    break;

  case 1108:
#line 3420 "sql_yacc.yy"
    {}
    break;

  case 1109:
#line 3421 "sql_yacc.yy"
    {}
    break;

  case 1110:
#line 3422 "sql_yacc.yy"
    {}
    break;

  case 1111:
#line 3423 "sql_yacc.yy"
    {}
    break;

  case 1112:
#line 3424 "sql_yacc.yy"
    {}
    break;

  case 1113:
#line 3425 "sql_yacc.yy"
    {}
    break;

  case 1114:
#line 3426 "sql_yacc.yy"
    {}
    break;

  case 1115:
#line 3427 "sql_yacc.yy"
    {}
    break;

  case 1116:
#line 3428 "sql_yacc.yy"
    {}
    break;

  case 1117:
#line 3429 "sql_yacc.yy"
    {}
    break;

  case 1118:
#line 3430 "sql_yacc.yy"
    {}
    break;

  case 1119:
#line 3431 "sql_yacc.yy"
    {}
    break;

  case 1120:
#line 3432 "sql_yacc.yy"
    {}
    break;

  case 1121:
#line 3433 "sql_yacc.yy"
    {}
    break;

  case 1122:
#line 3434 "sql_yacc.yy"
    {}
    break;

  case 1123:
#line 3435 "sql_yacc.yy"
    {}
    break;

  case 1124:
#line 3436 "sql_yacc.yy"
    {}
    break;

  case 1125:
#line 3437 "sql_yacc.yy"
    {}
    break;

  case 1126:
#line 3438 "sql_yacc.yy"
    {}
    break;

  case 1127:
#line 3439 "sql_yacc.yy"
    {}
    break;

  case 1128:
#line 3440 "sql_yacc.yy"
    {}
    break;

  case 1129:
#line 3441 "sql_yacc.yy"
    {}
    break;

  case 1130:
#line 3442 "sql_yacc.yy"
    {}
    break;

  case 1131:
#line 3443 "sql_yacc.yy"
    {}
    break;

  case 1132:
#line 3444 "sql_yacc.yy"
    {}
    break;

  case 1133:
#line 3450 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command= SQLCOM_SET_OPTION;
	  lex->option_type=OPT_DEFAULT;
	  lex->var_list.empty();
	}
    break;

  case 1134:
#line 3457 "sql_yacc.yy"
    {}
    break;

  case 1135:
#line 3461 "sql_yacc.yy"
    {}
    break;

  case 1136:
#line 3462 "sql_yacc.yy"
    {}
    break;

  case 1139:
#line 3469 "sql_yacc.yy"
    {}
    break;

  case 1140:
#line 3470 "sql_yacc.yy"
    { Lex->option_type= OPT_GLOBAL; }
    break;

  case 1141:
#line 3471 "sql_yacc.yy"
    { Lex->option_type= OPT_SESSION; }
    break;

  case 1142:
#line 3472 "sql_yacc.yy"
    { Lex->option_type= OPT_SESSION; }
    break;

  case 1143:
#line 3476 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1144:
#line 3477 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1145:
#line 3478 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1146:
#line 3479 "sql_yacc.yy"
    { yyval.num=OPT_GLOBAL; }
    break;

  case 1147:
#line 3483 "sql_yacc.yy"
    { yyval.num=OPT_DEFAULT; }
    break;

  case 1148:
#line 3484 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1149:
#line 3485 "sql_yacc.yy"
    { yyval.num=OPT_SESSION; }
    break;

  case 1150:
#line 3486 "sql_yacc.yy"
    { yyval.num=OPT_GLOBAL; }
    break;

  case 1151:
#line 3491 "sql_yacc.yy"
    {
	  Lex->var_list.push_back(new set_var_user(new Item_func_set_user_var(yyvsp[-2].lex_str,yyvsp[0].item)));
	}
    break;

  case 1152:
#line 3495 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->var_list.push_back(new set_var(lex->option_type, yyvsp[-2].variable, yyvsp[0].item));
	  }
    break;

  case 1153:
#line 3500 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->var_list.push_back(new set_var((enum_var_type) yyvsp[-3].num, yyvsp[-2].variable, yyvsp[0].item));
	  }
    break;

  case 1154:
#line 3505 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->var_list.push_back(new set_var(lex->option_type,
						find_sys_var("tx_isolation"),
						new Item_int((int32) yyvsp[0].tx_isolation)));
	  }
    break;

  case 1155:
#line 3512 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->var_list.push_back(new set_var(lex->option_type,
					      find_sys_var("convert_character_set"),
					      yyvsp[0].item));
	}
    break;

  case 1156:
#line 3519 "sql_yacc.yy"
    {
	    THD *thd=current_thd;
	    LEX_USER *user;
	    if (!(user=(LEX_USER*) sql_alloc(sizeof(LEX_USER))))
	      YYABORT;
	    user->host.str=0;
	    user->user.str=thd->priv_user;
	    thd->lex.var_list.push_back(new set_var_password(user, yyvsp[0].simple_string));
	  }
    break;

  case 1157:
#line 3529 "sql_yacc.yy"
    {
	    Lex->var_list.push_back(new set_var_password(yyvsp[-2].lex_user,yyvsp[0].simple_string));
	  }
    break;

  case 1158:
#line 3536 "sql_yacc.yy"
    {
	  sys_var *tmp=find_sys_var(yyvsp[0].lex_str.str, yyvsp[0].lex_str.length);
	  if (!tmp)
	    YYABORT;
	  yyval.variable=tmp;
	}
    break;

  case 1159:
#line 3545 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_READ_UNCOMMITTED; }
    break;

  case 1160:
#line 3546 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_READ_COMMITTED; }
    break;

  case 1161:
#line 3547 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_REPEATABLE_READ; }
    break;

  case 1162:
#line 3548 "sql_yacc.yy"
    { yyval.tx_isolation= ISO_SERIALIZABLE; }
    break;

  case 1163:
#line 3552 "sql_yacc.yy"
    { yyval.simple_string=yyvsp[0].lex_str.str;}
    break;

  case 1164:
#line 3554 "sql_yacc.yy"
    {
	    if (!yyvsp[-1].lex_str.length)
	      yyval.simple_string=yyvsp[-1].lex_str.str;
	    else
	    {
	      char *buff=(char*) sql_alloc(HASH_PASSWORD_LENGTH+1);
	      make_scrambled_password(buff,yyvsp[-1].lex_str.str);
	      yyval.simple_string=buff;
	    }
	  }
    break;

  case 1165:
#line 3567 "sql_yacc.yy"
    { yyval.item=yyvsp[0].item; }
    break;

  case 1166:
#line 3568 "sql_yacc.yy"
    { yyval.item=0; }
    break;

  case 1167:
#line 3569 "sql_yacc.yy"
    { yyval.item=new Item_string("ON",2); }
    break;

  case 1168:
#line 3570 "sql_yacc.yy"
    { yyval.item=new Item_string("ALL",3); }
    break;

  case 1169:
#line 3578 "sql_yacc.yy"
    {
	  Lex->sql_command=SQLCOM_LOCK_TABLES;
	}
    break;

  case 1170:
#line 3582 "sql_yacc.yy"
    {}
    break;

  case 1175:
#line 3595 "sql_yacc.yy"
    { if (!add_table_to_list(yyvsp[-2].table,yyvsp[-1].lex_str_ptr,0,(thr_lock_type) yyvsp[0].num)) YYABORT; }
    break;

  case 1176:
#line 3598 "sql_yacc.yy"
    { yyval.num=TL_READ_NO_INSERT; }
    break;

  case 1177:
#line 3599 "sql_yacc.yy"
    { yyval.num=current_thd->update_lock_default; }
    break;

  case 1178:
#line 3600 "sql_yacc.yy"
    { yyval.num=TL_WRITE_LOW_PRIORITY; }
    break;

  case 1179:
#line 3601 "sql_yacc.yy"
    { yyval.num= TL_READ; }
    break;

  case 1180:
#line 3604 "sql_yacc.yy"
    { Lex->sql_command=SQLCOM_UNLOCK_TABLES; }
    break;

  case 1181:
#line 3613 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_HA_OPEN;
	  if (!add_table_to_list(yyvsp[-2].table,yyvsp[0].lex_str_ptr,0))
	    YYABORT;
	}
    break;

  case 1182:
#line 3619 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_HA_CLOSE;
	  if (!add_table_to_list(yyvsp[-1].table,0,0))
	    YYABORT;
	}
    break;

  case 1183:
#line 3625 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_HA_READ;
	  lex->ha_rkey_mode= HA_READ_KEY_EXACT;	/* Avoid purify warnings */
	  lex->select->select_limit= 1;
	  lex->select->offset_limit= 0L;
	  if (!add_table_to_list(yyvsp[-1].table,0,0))
	    YYABORT;
        }
    break;

  case 1184:
#line 3634 "sql_yacc.yy"
    { }
    break;

  case 1185:
#line 3637 "sql_yacc.yy"
    { Lex->backup_dir= 0; }
    break;

  case 1186:
#line 3638 "sql_yacc.yy"
    { Lex->backup_dir= yyvsp[-1].lex_str.str; }
    break;

  case 1187:
#line 3641 "sql_yacc.yy"
    { Lex->ha_read_mode = RFIRST; }
    break;

  case 1188:
#line 3642 "sql_yacc.yy"
    { Lex->ha_read_mode = RNEXT;  }
    break;

  case 1189:
#line 3645 "sql_yacc.yy"
    { Lex->ha_read_mode = RFIRST; }
    break;

  case 1190:
#line 3646 "sql_yacc.yy"
    { Lex->ha_read_mode = RNEXT;  }
    break;

  case 1191:
#line 3647 "sql_yacc.yy"
    { Lex->ha_read_mode = RPREV;  }
    break;

  case 1192:
#line 3648 "sql_yacc.yy"
    { Lex->ha_read_mode = RLAST;  }
    break;

  case 1193:
#line 3650 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->ha_read_mode = RKEY;
	  lex->ha_rkey_mode=yyvsp[0].ha_rkey_mode;
	  if (!(lex->insert_list = new List_item))
	    YYABORT;
	}
    break;

  case 1194:
#line 3656 "sql_yacc.yy"
    { }
    break;

  case 1195:
#line 3659 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_KEY_EXACT;   }
    break;

  case 1196:
#line 3660 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_KEY_OR_NEXT; }
    break;

  case 1197:
#line 3661 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_KEY_OR_PREV; }
    break;

  case 1198:
#line 3662 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_AFTER_KEY;   }
    break;

  case 1199:
#line 3663 "sql_yacc.yy"
    { yyval.ha_rkey_mode=HA_READ_BEFORE_KEY;  }
    break;

  case 1200:
#line 3669 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->sql_command = SQLCOM_REVOKE;
	  lex->users_list.empty();
	  lex->columns.empty();
	  lex->grant= lex->grant_tot_col=0;
	  lex->select->db=0;
	  lex->ssl_type= SSL_TYPE_NOT_SPECIFIED;
	  lex->ssl_cipher= lex->x509_subject= lex->x509_issuer= 0;
	  bzero((char*) &lex->mqh, sizeof(lex->mqh));
	}
    break;

  case 1201:
#line 3681 "sql_yacc.yy"
    {}
    break;

  case 1202:
#line 3686 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->users_list.empty();
	  lex->columns.empty();
	  lex->sql_command = SQLCOM_GRANT;
	  lex->grant= lex->grant_tot_col= 0;
	  lex->select->db= 0;
	  lex->ssl_type= SSL_TYPE_NOT_SPECIFIED;
	  lex->ssl_cipher= lex->x509_subject= lex->x509_issuer= 0;
	  bzero((char *)&(lex->mqh),sizeof(lex->mqh));
	}
    break;

  case 1203:
#line 3699 "sql_yacc.yy"
    {}
    break;

  case 1204:
#line 3703 "sql_yacc.yy"
    {}
    break;

  case 1205:
#line 3704 "sql_yacc.yy"
    { Lex->grant = GLOBAL_ACLS;}
    break;

  case 1206:
#line 3705 "sql_yacc.yy"
    { Lex->grant = GLOBAL_ACLS;}
    break;

  case 1209:
#line 3712 "sql_yacc.yy"
    { Lex->which_columns = SELECT_ACL;}
    break;

  case 1210:
#line 3712 "sql_yacc.yy"
    {}
    break;

  case 1211:
#line 3713 "sql_yacc.yy"
    { Lex->which_columns = INSERT_ACL;}
    break;

  case 1212:
#line 3713 "sql_yacc.yy"
    {}
    break;

  case 1213:
#line 3714 "sql_yacc.yy"
    { Lex->which_columns = UPDATE_ACL; }
    break;

  case 1214:
#line 3714 "sql_yacc.yy"
    {}
    break;

  case 1215:
#line 3715 "sql_yacc.yy"
    { Lex->which_columns = REFERENCES_ACL;}
    break;

  case 1216:
#line 3715 "sql_yacc.yy"
    {}
    break;

  case 1217:
#line 3716 "sql_yacc.yy"
    { Lex->grant |= DELETE_ACL;}
    break;

  case 1218:
#line 3717 "sql_yacc.yy"
    {}
    break;

  case 1219:
#line 3718 "sql_yacc.yy"
    { Lex->grant |= INDEX_ACL;}
    break;

  case 1220:
#line 3719 "sql_yacc.yy"
    { Lex->grant |= ALTER_ACL;}
    break;

  case 1221:
#line 3720 "sql_yacc.yy"
    { Lex->grant |= CREATE_ACL;}
    break;

  case 1222:
#line 3721 "sql_yacc.yy"
    { Lex->grant |= DROP_ACL;}
    break;

  case 1223:
#line 3722 "sql_yacc.yy"
    { Lex->grant |= EXECUTE_ACL;}
    break;

  case 1224:
#line 3723 "sql_yacc.yy"
    { Lex->grant |= RELOAD_ACL;}
    break;

  case 1225:
#line 3724 "sql_yacc.yy"
    { Lex->grant |= SHUTDOWN_ACL;}
    break;

  case 1226:
#line 3725 "sql_yacc.yy"
    { Lex->grant |= PROCESS_ACL;}
    break;

  case 1227:
#line 3726 "sql_yacc.yy"
    { Lex->grant |= FILE_ACL;}
    break;

  case 1228:
#line 3727 "sql_yacc.yy"
    { Lex->grant |= GRANT_ACL;}
    break;

  case 1229:
#line 3728 "sql_yacc.yy"
    { Lex->grant |= SHOW_DB_ACL;}
    break;

  case 1230:
#line 3729 "sql_yacc.yy"
    { Lex->grant |= SUPER_ACL;}
    break;

  case 1231:
#line 3730 "sql_yacc.yy"
    { Lex->grant |= CREATE_TMP_ACL;}
    break;

  case 1232:
#line 3731 "sql_yacc.yy"
    { Lex->grant |= LOCK_TABLES_ACL; }
    break;

  case 1233:
#line 3732 "sql_yacc.yy"
    { Lex->grant |= REPL_SLAVE_ACL;}
    break;

  case 1234:
#line 3733 "sql_yacc.yy"
    { Lex->grant |= REPL_CLIENT_ACL;}
    break;

  case 1235:
#line 3738 "sql_yacc.yy"
    {}
    break;

  case 1236:
#line 3739 "sql_yacc.yy"
    {}
    break;

  case 1239:
#line 3749 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->x509_subject)
	  {
	    net_printf(&lex->thd->net,ER_DUP_ARGUMENT, "SUBJECT");
	    YYABORT;
	  }
	  lex->x509_subject=yyvsp[0].lex_str.str;
	}
    break;

  case 1240:
#line 3759 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->x509_issuer)
	  {
	    net_printf(&lex->thd->net,ER_DUP_ARGUMENT, "ISSUER");
	    YYABORT;
	  }
	  lex->x509_issuer=yyvsp[0].lex_str.str;
	}
    break;

  case 1241:
#line 3769 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->ssl_cipher)
	  {
	    net_printf(&lex->thd->net,ER_DUP_ARGUMENT, "CIPHER");
	    YYABORT;
	  }
	  lex->ssl_cipher=yyvsp[0].lex_str.str;
	}
    break;

  case 1242:
#line 3782 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->select->db=lex->thd->db;
	    if (lex->grant == GLOBAL_ACLS)
	      lex->grant = DB_ACLS & ~GRANT_ACL;
	    else if (lex->columns.elements)
	    {
	      send_error(&lex->thd->net,ER_ILLEGAL_GRANT_FOR_TABLE);
	      YYABORT;
	    }
	  }
    break;

  case 1243:
#line 3794 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->select->db = yyvsp[-2].lex_str.str;
	    if (lex->grant == GLOBAL_ACLS)
	      lex->grant = DB_ACLS & ~GRANT_ACL;
	    else if (lex->columns.elements)
	    {
	      send_error(&lex->thd->net,ER_ILLEGAL_GRANT_FOR_TABLE);
	      YYABORT;
	    }
	  }
    break;

  case 1244:
#line 3806 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    lex->select->db = NULL;
	    if (lex->grant == GLOBAL_ACLS)
	      lex->grant= GLOBAL_ACLS & ~GRANT_ACL;
	    else if (lex->columns.elements)
	    {
	      send_error(&lex->thd->net,ER_ILLEGAL_GRANT_FOR_TABLE);
	      YYABORT;
	    }
	  }
    break;

  case 1245:
#line 3818 "sql_yacc.yy"
    {
	    LEX *lex=Lex;
	    if (!add_table_to_list(yyvsp[0].table,NULL,0))
	      YYABORT;
	    if (lex->grant == GLOBAL_ACLS)
	      lex->grant =  TABLE_ACLS & ~GRANT_ACL;
	  }
    break;

  case 1246:
#line 3828 "sql_yacc.yy"
    { if (Lex->users_list.push_back(yyvsp[0].lex_user)) YYABORT;}
    break;

  case 1247:
#line 3830 "sql_yacc.yy"
    {
	    if (Lex->users_list.push_back(yyvsp[0].lex_user))
	      YYABORT;
	  }
    break;

  case 1248:
#line 3839 "sql_yacc.yy"
    {
	   yyval.lex_user=yyvsp[-3].lex_user; yyvsp[-3].lex_user->password=yyvsp[0].lex_str;
	   if (yyvsp[0].lex_str.length)
	   {
	     char *buff=(char*) sql_alloc(HASH_PASSWORD_LENGTH+1);
	     if (buff)
	     {
	       make_scrambled_password(buff,yyvsp[0].lex_str.str);
	       yyvsp[-3].lex_user->password.str=buff;
	       yyvsp[-3].lex_user->password.length=HASH_PASSWORD_LENGTH;
	     }
	  }
	}
    break;

  case 1249:
#line 3853 "sql_yacc.yy"
    { yyval.lex_user=yyvsp[-4].lex_user; yyvsp[-4].lex_user->password=yyvsp[0].lex_str ; }
    break;

  case 1250:
#line 3855 "sql_yacc.yy"
    { yyval.lex_user=yyvsp[0].lex_user; yyvsp[0].lex_user->password.str=NullS; }
    break;

  case 1251:
#line 3860 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  lex->grant |= lex->which_columns;
	}
    break;

  case 1255:
#line 3872 "sql_yacc.yy"
    {
	  String *new_str = new String((const char*) yyvsp[0].lex_str.str,yyvsp[0].lex_str.length);
	  List_iterator <LEX_COLUMN> iter(Lex->columns);
	  class LEX_COLUMN *point;
	  LEX *lex=Lex;
	  while ((point=iter++))
	  {
	    if (!my_strcasecmp(point->column.ptr(),new_str->ptr()))
		break;
	  }
	  lex->grant_tot_col|= lex->which_columns;
	  if (point)
	    point->rights |= lex->which_columns;
	  else
	    lex->columns.push_back(new LEX_COLUMN (*new_str,lex->which_columns));
	}
    break;

  case 1257:
#line 3892 "sql_yacc.yy"
    {
            Lex->ssl_type=SSL_TYPE_SPECIFIED;
          }
    break;

  case 1258:
#line 3896 "sql_yacc.yy"
    {
            Lex->ssl_type=SSL_TYPE_ANY;
          }
    break;

  case 1259:
#line 3900 "sql_yacc.yy"
    {
            Lex->ssl_type=SSL_TYPE_X509;
          }
    break;

  case 1260:
#line 3904 "sql_yacc.yy"
    {
	    Lex->ssl_type=SSL_TYPE_NONE;
	  }
    break;

  case 1261:
#line 3910 "sql_yacc.yy"
    {}
    break;

  case 1263:
#line 3914 "sql_yacc.yy"
    {}
    break;

  case 1264:
#line 3915 "sql_yacc.yy"
    {}
    break;

  case 1265:
#line 3918 "sql_yacc.yy"
    { Lex->grant |= GRANT_ACL;}
    break;

  case 1266:
#line 3920 "sql_yacc.yy"
    {
	  Lex->mqh.questions=yyvsp[0].ulong_num;
	  Lex->mqh.bits |= 1;
	}
    break;

  case 1267:
#line 3925 "sql_yacc.yy"
    {
	  Lex->mqh.updates=yyvsp[0].ulong_num;
	  Lex->mqh.bits |= 2;
	}
    break;

  case 1268:
#line 3930 "sql_yacc.yy"
    {
	  Lex->mqh.connections=yyvsp[0].ulong_num;
	  Lex->mqh.bits |= 4;
	}
    break;

  case 1269:
#line 3936 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_BEGIN;}
    break;

  case 1270:
#line 3936 "sql_yacc.yy"
    {}
    break;

  case 1271:
#line 3940 "sql_yacc.yy"
    {}
    break;

  case 1272:
#line 3941 "sql_yacc.yy"
    {;}
    break;

  case 1273:
#line 3944 "sql_yacc.yy"
    { Lex->sql_command = SQLCOM_COMMIT;}
    break;

  case 1274:
#line 3948 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_ROLLBACK;
	}
    break;

  case 1275:
#line 3952 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_ROLLBACK_TO_SAVEPOINT;
	  Lex->savepoint_name = yyvsp[0].lex_str.str;
	}
    break;

  case 1276:
#line 3958 "sql_yacc.yy"
    {
	  Lex->sql_command = SQLCOM_SAVEPOINT;
	  Lex->savepoint_name = yyvsp[0].lex_str.str;
	}
    break;

  case 1277:
#line 3969 "sql_yacc.yy"
    {}
    break;

  case 1279:
#line 3974 "sql_yacc.yy"
    {
	  LEX *lex=Lex;
	  if (lex->exchange)
	  {
	    /* Only the last SELECT can have  INTO...... */
	    net_printf(&lex->thd->net, ER_WRONG_USAGE,"UNION","INTO");
	    YYABORT;
	  }
	  if (lex->select->linkage == NOT_A_SELECT)
	  {
	    send_error(&lex->thd->net, ER_SYNTAX_ERROR);
	    YYABORT;
	  }
	  if (mysql_new_select(lex))
	    YYABORT;
	  lex->select->linkage=UNION_TYPE;
	}
    break;

  case 1280:
#line 3991 "sql_yacc.yy"
    {}
    break;

  case 1281:
#line 3995 "sql_yacc.yy"
    {}
    break;

  case 1282:
#line 3996 "sql_yacc.yy"
    {}
    break;

  case 1283:
#line 4002 "sql_yacc.yy"
    {}
    break;

  case 1284:
#line 4004 "sql_yacc.yy"
    {
    	    LEX *lex=Lex;
	    if (!lex->select->braces)
	    {
	      send_error(&lex->thd->net, ER_SYNTAX_ERROR);
	      YYABORT;
	    }
	    if (mysql_new_select(lex))
	      YYABORT;
	    mysql_init_select(lex);
	    lex->select->linkage=NOT_A_SELECT;
	    lex->select->select_limit=lex->thd->variables.select_limit;
	  }
    break;

  case 1286:
#line 4021 "sql_yacc.yy"
    {}
    break;

  case 1287:
#line 4022 "sql_yacc.yy"
    {}
    break;

  case 1288:
#line 4023 "sql_yacc.yy"
    { Lex->union_option=1; }
    break;


    }

/* Line 1016 of /usr/share/bison/yacc.c.  */
#line 15579 "sql_yacc.cc"

  yyvsp -= yylen;
  yyssp -= yylen;


#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

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
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyssp > yyss)
	    {
	      YYDPRINTF ((stderr, "Error: popping "));
	      YYDSYMPRINT ((stderr,
			    yystos[*yyssp],
			    *yyvsp));
	      YYDPRINTF ((stderr, "\n"));
	      yydestruct (yystos[*yyssp], *yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yydestruct (yychar1, yylval);
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

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

      YYDPRINTF ((stderr, "Error: popping "));
      YYDSYMPRINT ((stderr,
		    yystos[*yyssp], *yyvsp));
      YYDPRINTF ((stderr, "\n"));

      yydestruct (yystos[yystate], *yyvsp);
      yyvsp--;
      yystate = *--yyssp;


#if YYDEBUG
      if (yydebug)
	{
	  short *yyssp1 = yyss - 1;
	  YYFPRINTF (stderr, "Error: state stack now");
	  while (yyssp1 != yyssp)
	    YYFPRINTF (stderr, " %d", *++yyssp1);
	  YYFPRINTF (stderr, "\n");
	}
#endif
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


#line 609 "sql_yacc.yy"

