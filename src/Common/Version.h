// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef VERSION_H
#define VERSION_H
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
namespace QuickFAST
{
  const char[] QuickFAST_Product = "QuickFAST Version "
    "1.0"
    " Copyright (c) 2009, Object Computing, Inc.\n"
    "All Rights Reserved\n"
    "See the file license.txt for licensing information.\n";
  const long QuickFAST_Version = 0x00010000; // MMMM.mmmm {M=Major, m=minor)
}
#endif // VERSION_H
