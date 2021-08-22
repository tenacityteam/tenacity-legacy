/**********************************************************************

   Audacity - A Digital Audio Editor
   Copyright 1999-2009 Audacity Team
   File License: wxWidgets

   Dan Horgan

******************************************************************//**

\file CommandHandler.h
\brief Contains declarations for the CommandHandler class.

*//******************************************************************/

#pragma once

#include <memory>
class AudacityApp;
class AudacityProject;
class AppCommandEvent;
class CommandContext;

class CommandHandler
{
   public:
      CommandHandler();
      ~CommandHandler();

      // Whenever a command is received, process it.
      void OnReceiveCommand(AppCommandEvent &event);
};
