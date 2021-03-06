//===-- MICmdCmdGdbInfo.h --------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//++
// File:		MICmdCmdGdbInfo.h
//
// Overview:	CMICmdCmdGdbInfo	interface.
//
//				To implement new MI commands derive a new command class from the command base 
//				class. To enable the new command for interpretation add the new command class
//				to the command factory. The files of relevance are:
//					MICmdCommands.cpp
//					MICmdBase.h / .cpp
//					MICmdCmd.h / .cpp
//				For an introduction to adding a new command see CMICmdCmdSupportInfoMiCmdQuery
//				command class as an example.
//
// Environment:	Compilers:	Visual C++ 12.
//							gcc (Ubuntu/Linaro 4.8.1-10ubuntu9) 4.8.1
//				Libraries:	See MIReadmetxt. 
//
// Copyright:	None.
//--

#pragma once

// Third party headers:
#include <map>

// In-house headers:
#include "MICmdBase.h"

//++ ============================================================================
// Details:	MI command class. MI commands derived from the command base class.
//			*this class implements GDB command "info".
//			The design of matching the info request to a request action (or 
//			command) is very simple. The request function which carries out
//			the task of information gathering and printing to stdout is part of
//			*this class. Should the request function become more complicated then
//			that request should really reside in a command type class. Then this
//			class instantiates a request info command for a matching request. The
//			design/code of *this class then does not then become bloated. Use a
//			lightweight version of the current MI command system.
// Gotchas:	None.
// Authors:	Illya Rudkin 05/06/2014.
// Changes:	None.
//--
class CMICmdCmdGdbInfo : public CMICmdBase
{
// Statics:
public:
	// Required by the CMICmdFactory when registering *this command
	static CMICmdBase *	CreateSelf( void );

// Methods:
public:
	/* ctor */	CMICmdCmdGdbInfo( void );

// Overridden:
public:
	// From CMICmdInvoker::ICmd
	virtual bool	Execute( void );
	virtual bool	Acknowledge( void );
	virtual bool	ParseArgs( void );
	// From CMICmnBase
	/* dtor */ virtual ~CMICmdCmdGdbInfo( void );

// Typedefs:
private:
	typedef bool (CMICmdCmdGdbInfo::*FnPrintPtr)();
	typedef	std::map< CMIUtilString, FnPrintPtr >	MapPrintFnNameToPrintFn_t;

// Methods:
private:
	bool	GetPrintFn( const CMIUtilString & vrPrintFnName, FnPrintPtr & vrwpFn ) const;
	bool	PrintFnSharedLibrary( void );

// Attributes:
private:
	const static MapPrintFnNameToPrintFn_t	ms_mapPrintFnNameToPrintFn;
	//
	const CMIUtilString	m_constStrArgNamedPrint;
	bool				m_bPrintFnRecognised;		// True = This command has a function with a name that matches the Print argument, false = not found 
	bool				m_bPrintFnSuccessful;		// True = The print function completed its task ok, false = function failed for some reason
	CMIUtilString		m_strPrintFnName;
	CMIUtilString		m_strPrintFnError;
};