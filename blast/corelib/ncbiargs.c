/*   ncbiargs.c
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
* File Name:  ncbiargs.c
*
* Author:  Ostell, Schuler, Vakatov
*
* Version Creation Date:   07/15/91
*
* File Description:
*     GetArgs() for console applications 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbiargs.c,v $
* Revision 6.6  2001/07/03 20:03:11  juran
* Don't corruptly parse argv in Mac OS.
*
* Revision 6.5  2000/06/15 20:51:41  vakatov
* Use "const" in Args code
*
* Revision 6.4  1999/03/11 21:12:23  vakatov
* Get in-sync the "printf"/"scanf"'s format and args in some places
*
* Revision 6.3  1998/01/21 20:55:43  vakatov
* Nlm_GetArgs():  fixed memory leak(in the override of default str value)
*
* Revision 6.2  1997/10/29 02:43:14  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.1  1997/10/27 22:00:43  vakatov
* Check for the Bool, Int and Float arg. value validity;  reset all
* arguments if an error occured
*
* Revision 5.1  1997/07/22 19:06:56  vakatov
* Initial revision: merged GetArgs()'s from former "ncbiargs.msw" and
* "ncbimain.[msw,unx,mac,vms]"
*
* ==========================================================================
*/

#include <ncbi.h>

/*****************************************************************************
*
*   Nlm_GetArgs(ap)
*   	returns user startup arguments
*
*****************************************************************************/

NLM_EXTERN Nlm_Boolean Nlm_GetArgs(const char* progname,
                                   Nlm_Int2 numargs, Nlm_ArgPtr ap)
{
  static const char* types[] = {
    NULL,
    "T/F",
    "Integer",
    "Real",
    "String",
    "File In",
    "File Out",
    "Data In",
    "Data Out"
  };

  Nlm_Boolean  okay, all_default = TRUE, range;
  Nlm_Int2     i, j;
  Nlm_ArgPtr   curarg;
  Nlm_Boolean* resolved;
  Nlm_Int4     xx_argc = Nlm_GetArgc();
  char**       xx_argv = Nlm_GetArgv();

  if (!ap  ||  numargs <= 0)
    return FALSE;

  resolved = (Nlm_Boolean*) Nlm_MemNew(numargs * sizeof(Nlm_Boolean));
  if ( !resolved )
    return FALSE;

  /* reset all args */
  curarg = ap;
  for (i = 0;  i < numargs;  i++, curarg++) {
    curarg->intvalue   = 0;
    curarg->floatvalue = 0.0;
    curarg->strvalue   = NULL;
  }

  /* set defaults */
  curarg = ap;
  for (i = 0;  i < numargs;  i++, curarg++)
    {
      if ((curarg->type < ARG_BOOLEAN) ||
          (curarg->type > ARG_DATA_OUT))
        {
          ErrPostEx(SEV_ERROR, 0,0, "Invalid Arg->type in %s", curarg->prompt);
          Nlm_MemFree( resolved );
          Nlm_FreeArgs(i, ap);
          return FALSE;
        }
      curarg->intvalue = 0;
      curarg->floatvalue = 0.0;
      curarg->strvalue = NULL;
      if (curarg->defaultvalue != NULL)
        {
          resolved[i] = TRUE;
          switch (curarg->type)
            {
            case ARG_BOOLEAN:
              if (TO_UPPER(*curarg->defaultvalue) == 'T')
                curarg->intvalue = 1;
              else
                curarg->intvalue = 0;
              break;
            case ARG_INT: {
              long val;
              sscanf(curarg->defaultvalue, "%ld", &val);
              curarg->intvalue = val;
              break;
            }
            case ARG_FLOAT: {
              double val;
              sscanf(curarg->defaultvalue, "%lf", &val);
              curarg->floatvalue = val;
              break;
            }
            case ARG_STRING:
            case ARG_FILE_IN:
            case ARG_FILE_OUT:
            case ARG_DATA_IN:
            case ARG_DATA_OUT:
              curarg->strvalue = StringSave (curarg->defaultvalue);
              break;
            }
        }
      else if (curarg->optional == FALSE)
        all_default = FALSE;    /* must have some arguments */
    }

  /**** show usage if no args on command line ***/
  if (xx_argc == 1  &&  all_default)   /* no args ok */
    {
      Nlm_MemFree( resolved );
      return TRUE;
    }

  if (xx_argc == 1  ||  *(xx_argv[1]+1) == '\0')
    {
      printf("\n%s   arguments:\n\n", progname);
      curarg = ap;

      for (i = 0, j = 0;  i < numargs;  i++, j++, curarg++)
        {
          printf("  -%c  %s [%s]", curarg->tag, curarg->prompt,
                 types[curarg->type]);
          if (curarg->optional)
            printf("  Optional");
          printf("\n");
          if (curarg->defaultvalue != NULL)
            printf("    default = %s\n", curarg->defaultvalue);
          if ((curarg->from != NULL) || (curarg->to != NULL))
            {
              if ((curarg->type == ARG_DATA_IN) ||
                  (curarg->type == ARG_DATA_OUT))
                printf("    Data Type = %s\n", curarg->from);
              else
                printf("    range from %s to %s\n", 
                       (curarg->from ? curarg->from: "<NULL>"),
                       (curarg->to   ? curarg->to  : "<NULL>"));
            }
        }

      printf("\n");
      fflush (stdout);
      Nlm_MemFree( resolved );
      Nlm_FreeArgs(numargs, ap);
#ifdef COMP_MPW
      Message(MSG_OK, "To exit: ");
#endif
      return FALSE;
    }


  /* Parse the arguments */
  for (i = 1; i < xx_argc; i++)
    {
      const char* arg = xx_argv[i];
      if (*arg != '-')
        {
          ErrPostEx(SEV_ERROR, 0, 0, "Arguments must start with '-' (the offending argument #%d was: '%s')", (int)i, arg);
          Nlm_MemFree( resolved );
          Nlm_FreeArgs(numargs, ap);
          return FALSE;
        }
      arg++;
      curarg = ap;
      for (j = 0; j < numargs; j++, curarg++)
        {
          if (*arg == curarg->tag)
            break;
        }
      if (j == numargs)
        {
          ErrPostEx(SEV_ERROR, 0, 0, "Invalid argument: %s", xx_argv[i]);
          Nlm_MemFree( resolved );
          Nlm_FreeArgs(numargs, ap);
          return FALSE;
        }
      arg++;

      if (*arg == '\0')
        {
          Nlm_Boolean ok = FALSE;
          if ((i + 1) < xx_argc)  /* argument comes after space */
            {
              if (*xx_argv[i + 1] == '-')
                {
                  char tmp = *(xx_argv[i+1]+1);
                  if ((curarg->type == ARG_INT  ||  curarg->type == ARG_FLOAT)
                      &&  (tmp == '.'  ||  IS_DIGIT(tmp)))
                    ok = TRUE;
                }
              else
                ok = TRUE;

              if ( ok ) {
                i++;
                arg = xx_argv[i];
              }
            }

          if (!ok  &&  curarg->type != ARG_BOOLEAN)
            {
              ErrPostEx(SEV_ERROR, 0, 0,
                        "No argument given for %s", curarg->prompt);
              Nlm_MemFree( resolved );
              Nlm_FreeArgs(numargs, ap);
              return FALSE;
            }
        }

      resolved[j] = TRUE;
      switch (curarg->type)
        {
        case ARG_BOOLEAN:
          if (TO_UPPER(*arg) == 'T')
            curarg->intvalue = 1;
          else if (TO_UPPER(*arg) == 'F')
            curarg->intvalue = 0;
          else if (*arg == '\0')
            curarg->intvalue = 1;
          else {
            ErrPostEx(SEV_ERROR, 0, 0,
                      "%s [%s] must be one of {'T', 't', 'F', 'f'} or omitted",
                      curarg->prompt, arg);
            Nlm_MemFree( resolved );
            Nlm_FreeArgs(numargs, ap);
            return FALSE;
          }
          break;

        case ARG_INT: {
          long val;
          range = TRUE;
          if (sscanf(arg, "%ld", &val) <= 0)
            range = FALSE;
          curarg->intvalue = val;
          if (range  &&  curarg->from)
            {
              long ifrom;
              sscanf(curarg->from, "%ld", &ifrom);
              if (curarg->intvalue < ifrom)
                range = FALSE;
            }
          if (range  &&  curarg->to)
            {
              long ito;
              sscanf(curarg->to, "%ld", &ito);
              if (curarg->intvalue > ito)
                range = FALSE;
            }

          if ( !range )
            {
              ErrPostEx(SEV_ERROR, 0, 0,
                        "%s [%s] is bad or out of range [%s to %s]",
                        curarg->prompt, arg,
                        curarg->from ? curarg->from : "?",
                        curarg->to   ? curarg->to   : "?");
              Nlm_MemFree( resolved );
              Nlm_FreeArgs(numargs, ap);
              return FALSE;
            }
          break;
        }

        case ARG_FLOAT: {
          double val;
          range = TRUE;
          if (sscanf(arg, "%lf", &val) <= 0)
            range = FALSE;
          curarg->floatvalue = val;
          if (range  &&  curarg->from)
            {
              double ffrom;
              sscanf(curarg->from, "%lf", &ffrom);
              if (curarg->floatvalue < ffrom)
                range = FALSE;
            }
          if (range  &&  curarg->to)
            {
              double fto;
              sscanf(curarg->to, "%lf", &fto);
              if (curarg->floatvalue > fto)
                range = FALSE;
            }
          if ( !range )
            {
              ErrPostEx(SEV_ERROR, 0, 0,
                        "%s [%s] is bad or out of range [%s to %s]",
                        curarg->prompt, arg,
                        curarg->from ? curarg->from : "?",
                        curarg->to   ? curarg->to   : "?");
              Nlm_MemFree( resolved );
              Nlm_FreeArgs(numargs, ap);
              return FALSE;
            }
          break;
        }

        case ARG_STRING:
        case ARG_FILE_IN:
        case ARG_FILE_OUT:
        case ARG_DATA_IN:
        case ARG_DATA_OUT:
          if ( curarg->strvalue )
            MemFree(curarg->strvalue);
          curarg->strvalue = StringSave (arg);
          break;
        }    /*** end switch ****/
    }

  okay = TRUE;
  curarg = ap;
  for (i = 0;  i < numargs;  i++, curarg++)
    {
      if (!curarg->optional  &&  !resolved[i])
        {
          ErrPostEx(SEV_ERROR, 0, 0,
                    "%s was not given an argument", curarg->prompt);
          okay = FALSE;
        }
    }

  Nlm_MemFree( resolved );
  if ( !okay )
    Nlm_FreeArgs(numargs, ap);

  return okay;
}


NLM_EXTERN Nlm_Boolean Nlm_GetArgsSilent(const char* progname,
                                         Nlm_Int2 numargs, Nlm_ArgPtr ap)
{
  return Nlm_GetArgs(progname, numargs, ap);
}

