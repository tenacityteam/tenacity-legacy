/**********************************************************************

  Audacity: A Digital Audio Editor

  Demo.h

  James Crook

**********************************************************************/

#pragma once

#include "AudacityCommand.h"
#include "../SampleFormat.h"

class ShuttleGui;

class DemoCommand final : public AudacityCommand
{
public:
   static const ComponentInterfaceSymbol Symbol;

   // ComponentInterface overrides
   ComponentInterfaceSymbol GetSymbol() override {return Symbol;};
   TranslatableString GetDescription() override {return XO("Does the demo action.");};
   bool DefineParams( ShuttleParams & S ) override;
   void PopulateOrExchange(ShuttleGui & S) override;
   bool Apply(const CommandContext & context) override;

   // AudacityCommand overrides
   ManualPageID ManualPage() override {return L"Extra_Menu:_Scriptables_I";}

private:
   double delay;
   double decay;
};
