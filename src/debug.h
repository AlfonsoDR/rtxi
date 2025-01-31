/*
	 The Real-Time eXperiment Interface (RTXI)
	 Copyright (C) 2011 Georgia Institute of Technology, University of Utah, Weill Cornell Medical College

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef DEBUG_H
#define DEBUG_H

#include <boost/stacktrace.hpp>
#include <execinfo.h>
#include <stdarg.h>
#include <string>
#include <memory>
#include <iostream>

#if XENOMAI
#include <rtdk.h>
#endif

//! Prints a backtrace to standard error.
void PRINT_BACKTRACE(void);

void ERROR_MSG(const std::string& errmsg, ...);

#endif /* DEBUG_H */
