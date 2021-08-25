/**********************************************************************

   Audacity - A Digital Audio Editor
   Copyright 1999-2018 Audacity Team
   File License: wxWidgets

   Dan Horgan

******************************************************************//**

\file ScriptCommandRelay.h
\brief Contains declarations for ScriptCommandRelay

*//*******************************************************************/

#ifndef __SCRIPT_COMMAND_RELAY__
#define __SCRIPT_COMMAND_RELAY__



#include <memory>

class wxString;

typedef int(*tpExecScriptServerFunc)(wxString * pIn, wxString * pOut);
typedef int(*tpRegScriptServerFunc)(tpExecScriptServerFunc pFn);

class TENACITY_DLL_API ScriptCommandRelay
{
public:
   static void StartScriptServer(tpRegScriptServerFunc scriptFn);
};

// The void * return is actually a Lisp LVAL and will be cast to such as needed.
extern void * ExecForLisp( char * pIn );
extern void * nyq_make_opaque_string( int size, unsigned char *src );

#endif /* End of include guard: __SCRIPT_COMMAND_RELAY__ */
