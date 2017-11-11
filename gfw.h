#ifndef __GFRAMEWORK__H
#define __GFRAMEWORK__H
/****************************************************************************************/
/*****************************G-Framework (General Purpose Framework)********************
 *                  File: gfw.h
 *                  Description: Main Framework header file.
 *                  Author: Daniel Rios.
 *     LICENSE:
 *       Copyright (c) <2017> <Daniel Rios>
 *
 *       This software is provided 'as-is', without any express or implied
 *       warranty. In no event will the authors be held liable for any damages
 *       arising from the use of this software.
 *
 *       Permission is granted to anyone to use this software for any purpose,
 *       including commercial applications, and to alter it and redistribute it
 *       freely, subject to the following restrictions:
 *
 *       1. The origin of this software must not be misrepresented; you must not
 *          claim that you wrote the original software. If you use this software
 *          in a product, an acknowledgment in the product documentation would be
 *          appreciated but is not required.
 *       2. Altered source versions must be plainly marked as such, and must not be
 *          misrepresented as being the original software.
 *       3. This notice may not be removed or altered from any source distribution.
 *
******************************************************************************************/


// Strings
#include <gString/gStringList.h>
// Dynamic Data Structures
#include <gArray/gListBase.h>
#include <gAlignedLists//gAlignedPtrList.h>
#include <gAlignedLists//gAlignedList.h>
#include <gVariant/gVariantList.h>
#include <gMemory/gBaseArray.h>
#include <gTree/gTree.h>
#include <gLinkedList/gtLinkedList.h>
#include <gHashMap/gHashMap.h>
#include <gTable/gAlignedTable.h>
#include <gTable/gAlignedPtrTable.h>
#include <gMemory/gSharedPointer.h>
//Math
#include <gMath/gMath.h>
#include <gMath/gVector2.h>
#include <gMath/gVector3.h>
#include <gMath/gPolygon.h>
#include <gMath/gDimension.h>
//Parser utilities
#include <gLexer/gLexer.h>
#include <gXMLParser/gXMLParser.h>
#include <gCsl/gCsl.h>
//Files
#include <gFile/gFile.h>
// Dynamic libraries
#include <gLibrary/gLibrary.h>
//Time and timers
#include <gTimers/gSystemTimer.h>
#include <gTimers/gVirtualTimer.h>
#include <gTime/gTime.h>
//Threads
#include <gThreads/gThread.h>
#include <gThreads/gSemaphore.h>
//Network
#ifdef INCLUDE_GUDA
#include <gUda/gUda.h>
#endif
#include <gUrl/gUrl.h>
//Programing utilities
#include <gBase/gFunction.h>
//Additional utilities
#include <gBinPacker/gBpTools.h>
#include <gBinPacker/gGuillotine3D.h>
#endif
