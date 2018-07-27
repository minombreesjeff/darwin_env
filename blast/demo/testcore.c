/*  $Id: testcore.c,v 6.2 1999/11/29 20:00:37 vakatov Exp $
 * ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 *
 *
 * File Description:
 *   Test CORELIB functionality
 *
 * ---------------------------------------------------------------------------
 * $Log: testcore.c,v $
 * Revision 6.2  1999/11/29 20:00:37  vakatov
 * Added "TestParseArgs()" to test new public command-line parsing functions
 * Nlm_ParseCmdLineArguments(), Nlm_FreeCmdLineArguments()
 *
 * ===========================================================================
 */

#include <ncbi.h>


#define TEST (CTX_RESERVED+1)


/*** our prototyped functions ***/
static void BuildBS(ByteStorePtr bsp);

static void TestMessages(void);
static void TestErrors(void);
static void TestSettings(void);
static void TestMemory(void);
static void TestByteStores(void);
static void TestStrings(void);
static void TestMisc(void);
static void TestParseArgs(void);


/*** our arguments ***/
#define NUMARGS 8

Args testargs[NUMARGS] = {
    { "test Boolean",
      "T", NULL, NULL, FALSE, 'b', ARG_BOOLEAN, 0.0, 0, NULL },
    { "test Integer",
      "42", "41", "43", TRUE, 'i', ARG_INT, 0.0, 0, NULL },
    { "test Float",
      "3.14159", NULL, NULL, FALSE, 'f', ARG_FLOAT, 0.0, 0, NULL },
    { "test String",
      NULL, NULL, NULL, TRUE, 's', ARG_STRING, 0.0, 0, NULL },
    { "test File-in",
      NULL, NULL, NULL, TRUE, 'w', ARG_FILE_IN, 0.0, 0, NULL },
    { "test File-out",
      NULL, NULL, NULL, TRUE, 'x', ARG_FILE_OUT, 0.0, 0, NULL },
    { "test Data-in",
      NULL, "Fake-type", NULL, TRUE, 'y', ARG_DATA_IN, 0.0, 0, NULL },
    { "test Data-out",
      NULL, "Fake-out", NULL, TRUE, 'z', ARG_DATA_OUT, 0.0, 0, NULL }
};
  
static char* tmsg = "Test of %s",
    * fmsg = "Fail on %s",
    * omsg = "[overwrite at 50]",
    * imsg = "[inserted at 10]",
    * stest[4] = { "The", "quick", "brown", "fox" };



/*** use Int2 Main(), not main(argc, argv)  ***/
Int2 Main (void)
{
    Char lbuf[100];
    Int4 seconds;

    /*** provide opening arguments ***/
    if ( !GetArgs("TestCore 1.0", NUMARGS, testargs) )
        return 1; /*** leave if nothing happens ***/

    seconds = GetSecs();

    TestErrors();

    TestMessages();

    TestSettings();

    TestMemory();

    TestByteStores();

    TestMisc();

    TestParseArgs();

    seconds = GetSecs() - seconds;
    DayTimeStr(lbuf, TRUE, TRUE);
    Message(MSG_OK, "Today is: %s.  Test took %ld seconds.",
            lbuf, (long)seconds);

    VERIFY( FreeArgs(NUMARGS, testargs) );
    VERIFY( FreeArgs(NUMARGS, testargs) );
    return 0;
}


static void TestErrors (void)
{
    remove ("testcore.log");
    ErrSetLog ("testcore.log");

    ERRPOST((CTX_DEBUG, 0, "Test post of a debugging message"));
    ErrSetOpts (0, ERR_LOG_OFF);
    ERRPOST((CTX_DEBUG, 1,
             "Logging was not disabled as it should be"));
    ErrSetOpts (0, ERR_LOG_ON);
}


static void TestMessages (void)
{
    Message(MSG_ERROR, tmsg, "non-fatal error message");
    Message(MSG_OK, tmsg, "OK message");
    if (Message(MSG_RC, tmsg, "retry/cancel message. Hit c.") != ANS_CANCEL)
        ERRPOST((TEST, 1, "Did not get retry"));
    if (Message(MSG_ARI, tmsg, "abort/retry/ignore message. Hit a.")
        != ANS_ABORT)
        ERRPOST((TEST, 1, "Did not get abort"));
    if (! Message(MSG_YN, tmsg, "yes/no message. Hit y."))
        ERRPOST((TEST, 1, "Did not get yes"));
}


static void TestSettings (void)
{
    char buffer[6];

    if ( ! SetAppParam ("junk", "test", "key", "value"))
        ERRPOST((TEST, 1, fmsg, "SetAppParam"));

    GetAppParam ("junk", "test", "key", NULL, buffer, sizeof buffer);
    if (strcmp(buffer, "value"))
        ERRPOST((TEST, 1, fmsg, "GetAppParam"));

    GetAppParam ("junk", "test", "foo", "foo", buffer, sizeof buffer);
    if (strcmp(buffer, "foo"))
        ERRPOST((TEST, 1, "GetAppParam: default value not returned"));

    if ( ! TransientSetAppParam ("junk", "test", "foo", "foo2"))
        ERRPOST((TEST, 1, "TransientSetAppParam"));
    GetAppParam ("junk", "test", "foo", "foo", buffer, sizeof buffer);
    if (strcmp(buffer, "foo2"))
        ERRPOST((TEST, 1,
                 "GetAppParam: TransientSetAppParam value not returned"));
    if ( ! TransientSetAppParam ("junk", "test", NULL, NULL))
        ERRPOST((TEST, 1, "TransientSetAppParam section"));

    GetAppParam ("junk", "test", "key", "foo", buffer, sizeof buffer);
    if (strcmp(buffer, "foo"))
        ERRPOST((TEST, 1, fmsg, "GetAppParam on empty section"));

    if ( ! SetAppParam ("junk", "test", "key", "val2"))
        ERRPOST((TEST, 1, fmsg, "SetAppParam"));

    GetAppParam ("junk", "test", "key", NULL, buffer, sizeof buffer);
    if (strcmp(buffer, "val2"))
        ERRPOST((TEST, 1, fmsg, "GetAppParam on new value"));
}


static void TestMemory (void)
{
    CharPtr cpnt;
    Handle hand;
    Uint2 x = 0xdead;
    unsigned char *y;

    if (sizeof(Int1) != 1) {
        ERRPOST((TEST, 1, fmsg,
                 "Int1 is really %d bytes long!", sizeof(Int1)));
    }

    if (sizeof(Int2) != 2) {
        ERRPOST((TEST, 1, fmsg,
                 "Int2 is really %d bytes long!", sizeof(Int2)));
    }

    if (sizeof(Int4) != 4) {
        ERRPOST((TEST, 1, fmsg,
                 "Int4 is really %d bytes long!", sizeof(Int4)));
    }

    if (sizeof(x) == 2) {
        y = (unsigned char*) &x;
#ifdef IS_LITTLE_ENDIAN
        if (y[0] == 0xde && y[1] == 0xad) {
            ERRPOST((TEST, 1, fmsg,
                     "Declared little-endian mach. appears to be big-endian"));
        }
#endif /* IS_LITTLE_ENDIAN */
#ifdef IS_BIG_ENDIAN
        if (y[1] == 0xde && y[0] == 0xad) {
            ERRPOST((TEST, 1, fmsg,
                     "Declared big-endian mach. appears to be little-endian"));
        }
#endif /* IS_BIG_ENDIAN */
    }

    /*** test some memory ***/
    if ((hand = HandNew((size_t)60000)) == NULL)
        ERRPOST((TEST, 1, fmsg, "HandNew"));
    if ((cpnt = (CharPtr) HandLock(hand)) == NULL)
        ERRPOST((TEST, 1, fmsg, "HandLock"));
    HandUnlock(hand);
    HandFree(hand);

    if ((cpnt = (CharPtr) MemNew((size_t)60000)) == NULL)
        ERRPOST((TEST, 1, fmsg, "MemNew"));
    else
        MemFree(cpnt);
}


static void TestByteStores (void)
{
    CharPtr cpnt = NULL;
    ByteStorePtr bsp;
    Char lbuf[100];
    Int2 ctr;

    if ((bsp = BSNew(60000)) == NULL)
        ERRPOST((TEST, 1, fmsg, "BSNew"));

    /*** test ByteStore ***/
    BuildBS(bsp);                        /*** fill 60k ***/
    if (BSLen(bsp) != 60000)
        Message(MSG_ERROR, fmsg, "BSLen");

    BSSeek(bsp, 31980, SEEK_SET);
    if (BSTell(bsp) != 31980)
        Message(MSG_ERROR, fmsg, "BSTell");
    MemFill(lbuf, '\0', 100);      /*** BSRead does not add '\0' at end */
    BSRead(bsp, lbuf, 50);
    Message(MSG_OK, "BS50  [%s]", lbuf);        /*** show 31980-32029 ***/

    BSSeek(bsp, 31990, SEEK_SET);
    BSDelete(bsp, 20);                          /*** show delete at 31990 **/
    BSSeek(bsp, 31980, SEEK_SET);
    MemFill(lbuf, '\0', 100);      /*** BSRead does not add '\0' at end */
    BSRead(bsp, lbuf, 50);
    Message(MSG_OK, "BSdel [%s]", lbuf);

    BSSeek(bsp, 50, SEEK_SET);                 /*** show overwrite at 50 **/
    BSWrite(bsp, omsg, StringLen(omsg));
    BSSeek(bsp,40, SEEK_SET);
    MemFill(lbuf, '\0', 100);      /*** BSRead does not add '\0' at end */
    BSRead(bsp, lbuf, 50);
    Message(MSG_OK, "BSow [%s]", lbuf);

    BSSeek(bsp, 10, SEEK_SET);                 /*** show insert at 10 **/
    BSInsert(bsp, imsg, StringLen(imsg));
    BSSeek(bsp, 0, SEEK_SET);
    MemFill(lbuf, '\0', 100);      /*** BSRead does not add '\0' at end */
    BSRead(bsp, lbuf, 50);
    Message(MSG_OK, "BS i [%s]", lbuf);

    BSSeek(bsp, 0, SEEK_SET);                  /*** show getbyte ***/
    MemFill(lbuf, '\0', 100);      /*** BSRead does not add '\0' at end */
    for (ctr = 0; ctr < 50; ctr++)
        lbuf[ctr] = (Byte) BSGetByte(bsp);
    Message(MSG_OK, "BS g [%s]", lbuf);

    /*** check freeing of memory ***/
    cpnt = (CharPtr) MemFree(cpnt);
    cpnt = (CharPtr) BSMerge(bsp, NULL);       /*** check merge ***/
    MemFill(lbuf, '\0', 100);
    MemCopy(lbuf, cpnt, 50);
    Message(MSG_OK, "BS m [%s]", lbuf);
    bsp = BSFree(bsp);             /*** free the ByteStore */
    cpnt = (CharPtr) MemFree(cpnt);

    /*** check add ***/
    bsp = BSNew(0);                /*** no initial storage allocated ***/
    for (ctr = 0; ctr < 4; ctr++)
        BSWrite(bsp, stest[ctr], StringLen(stest[ctr]));
    MemFill(lbuf, '\0', 100);      /*** merge doesn't add '\0' to previous */
    /*   storage **/
    BSMerge(bsp, lbuf);            /*** merge to previous storage ***/
    Message(MSG_OK, "BS add [%s]", lbuf);
    bsp = BSFree(bsp);
}


/*****************************************************************************
*
*   BuildBS(bsp);
*
*****************************************************************************/

static void BuildBS (ByteStorePtr bsp)
{
    Char lbuf[20], tbuf[20];
    Int4 value;

    StringCpy(lbuf, "....:....|");
    for (value = 0; value < 60000; value += 10)
        {
            sprintf(tbuf, "%ld", value);
            MemCopy(lbuf, tbuf, StringLen(tbuf));
            BSWrite(bsp, lbuf, 10);
        }
    return;
}


static void TestStrings (void)
{
    CharPtr cpnt;
    Char lbuf[100];
    Int2 ctr;
    /*** some string functions ***/
    for (ctr = 0; ctr < 4; ctr++) {
        if (StringCmp("brown", stest[ctr])) {
            if (ctr == 2)
                Message(MSG_ERROR, fmsg, "StringCmp");
        }
        else if (ctr != 2) {
            Message(MSG_ERROR, fmsg, "StringCmp2");
        }
    }

    for (ctr = 0; ctr < 4; ctr++) {
        if (StringICmp("Brown", stest[ctr])) {
            if (ctr == 2)
                Message(MSG_ERROR, fmsg, "StringICmp");
        }
        else if (ctr != 2) {
            Message(MSG_ERROR, fmsg, "StringICmp2");
        }
    }

    for (ctr = 0; ctr < 4; ctr++) {
        if (StringNCmp("brow", stest[ctr], 4)) {
            if (ctr == 2)
                Message(MSG_ERROR, fmsg, "StringNCmp");
        }
        else if (ctr != 2) {
            Message(MSG_ERROR, fmsg, "StringNCmp2");
        }
    }

    for (ctr = 0; ctr < 4; ctr++) {
        if (StringNICmp("Brow", stest[ctr], 4)) {
            if (ctr == 2)
                Message(MSG_ERROR, fmsg, "StringNICmp");
        }
        else if (ctr != 2) {
            Message(MSG_ERROR, fmsg, "StringNICmp2");
        }
    }

    MemFill(lbuf, '\0', 100);
    for (ctr = 0; ctr < 4; ctr++) {
        StringCat(lbuf, stest[ctr]);
    }
    Message(MSG_OK, "StringCat [%s]", lbuf);

    MemFill(lbuf, '\0', 100);
    for (ctr = 0; ctr < 4; ctr++) {
        StringNCat(lbuf, stest[ctr], 4);
    }
    Message(MSG_OK, "StringNCat 4 [%s]", lbuf);

    cpnt = StringSave(lbuf);
    StringCpy(lbuf, cpnt);   /* copy to local buf for MSWindows */
    Message(MSG_OK, "StringSave [%s]", lbuf);
    cpnt = (CharPtr) MemFree(cpnt);
}


static void TestMisc (void)
{
    /*** test ncbistd.h ***/
    if (MIN(0.8, 1.0) != 0.8)
        Message(MSG_ERROR, fmsg, "MIN");
    if (MAX(0.8, 1.0) != 1.0)
        Message(MSG_ERROR, fmsg, "MAX");
    if (ABS(-2) != 2)
        Message(MSG_ERROR, fmsg, "ABS");
    if (ROUNDUP(24, 5) != 25)
        Message(MSG_ERROR, fmsg, "ROUNDUP");
}


static void TestParseArgs(void)
{
    int i;
    typedef struct {
        const char* prog_name;
        const char* cmd_line;
    } STestParseArgs;

    static const STestParseArgs x_test[] = {
        {  0,  0  },
        { "",  0  },
        {  0,  "" },
        { "",  "" },
        { 0, "a bb ccc" },
        { "", "-aaa bbb -ccc +ddd eee" },
        { "MyProgName1", "aa bb - +" },
        { "MyProgName 2", "aa \"bb cc\"" },
        { 0, "'aa' 'bb cc' \"ee dd\"" },
        { 0, "'\"aa' 'bb\" cc' \"ee ' dd\"" }
    };

    ErrSetMessageLevel(SEV_MIN);
    ErrSetLogLevel(SEV_MIN);

    ErrPostEx(SEV_INFO, 0,0, "[BEGIN] TEST of Nlm_ParseCmdLineArguments()");
    for (i = 0;  i < DIM(x_test);  i++) {
        int    x_argc, j;
        char** x_argv;
        ErrPostEx(SEV_INFO, 0,0, "Program: `%s'",
                x_test[i].prog_name ? x_test[i].prog_name : "<NULL>");
        ErrPostEx(SEV_INFO, 0,0, "CmdLine: `%s'",
                x_test[i].cmd_line  ? x_test[i].cmd_line  : "<NULL>");

        /* ProcessQuotes */
        VERIFY( ParseCmdLineArguments
                (x_test[i].prog_name, x_test[i].cmd_line, &x_argc, &x_argv,
                 eProcessQuotes) );
        ErrPostEx(SEV_INFO, 0,0, "argc = %d", x_argc);
        for (j = 0;  j < x_argc;  j++) {
            ASSERT( x_argv[j] );
            ErrPostEx(SEV_INFO, 0,0,
                      "ProcessQuotes::argv[%d] = %s", j, x_argv[j]);
        }
        ASSERT( !x_argv[x_argc] );
        FreeCmdLineArguments(x_argv);

        /* IgnoreQuotes */
        VERIFY( ParseCmdLineArguments
                (x_test[i].prog_name, x_test[i].cmd_line, &x_argc, &x_argv,
                 eIgnoreQuotes) );
        ErrPostEx(SEV_INFO, 0,0, "argc = %d", x_argc);
        for (j = 0;  j < x_argc;  j++) {
            ASSERT( x_argv[j] );
            ErrPostEx(SEV_INFO, 0,0,
                      "IgnoreQuotes::argv[%d] = %s", j, x_argv[j]);
        }
        ASSERT( !x_argv[x_argc] );
        FreeCmdLineArguments(x_argv);
    }
    ErrPostEx(SEV_INFO, 0,0, "[END] TEST of Nlm_ParseCmdLineArguments()");
}
