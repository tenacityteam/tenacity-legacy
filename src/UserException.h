/*!
 @file UserException.h
 @brief An AudacityException with no visible message

 Created by Paul Licameli on 11/27/16.

*/

#pragma once

#include "AudacityException.h"

 //! Can be thrown when user cancels operations, as with a progress dialog.  Delayed handler does nothing
/*! This class does not inherit from MessageBoxException. */
class TENACITY_DLL_API UserException final : public AudacityException
{
public:
   UserException() {}

   ~UserException() override;

   void DelayedHandlerAction() override;
};
