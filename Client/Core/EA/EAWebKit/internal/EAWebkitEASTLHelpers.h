/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// EAWebKitEASTLHelpers.h
// By Arpit Baldeva (Isolated Paul Pedriana's code in EAWebkit.h)
///////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EAWEBKITEASTLHELPERS_H
#define EAWEBKIT_EAWEBKITEASTLHELPERS_H


///////////////////////////////////////////////////////////////////////
// The purpose of this file is to isolate the EASTL related code from EAWebkit.h.
// This is required for LGPL compliance.
///////////////////////////////////////////////////////////////////////

#include <EABase/eabase.h>
#include <EASTL/list.h>
#include <EASTL/fixed_string.h>
#include <EASTL/fixed_map.h>
#include <EASTL/vector.h>
#include <EASTL/hash_map.h>
#include <EAWebKit/EAWebkitAllocator.h> //For EASTLAllocator
#include <EAWebKit/EAWebKitJavascriptValue.h>

#if EAWEBKIT_THROW_BUILD_ERROR
#error This file should be included only in a dll build
#endif

namespace EA
{
    namespace WebKit
    {
		class EASTLFixedString8Wrapper;
		class EASTLFixedString16Wrapper;
		class EASTLHeaderMapWrapper;
        class EASTLVectorJavaScriptValueWrapper;
        class EASTLJavascriptValueHashMapWrapper;
        class EASTLJavascriptValueHashMapIteratorWrapper;
			
		///////////////////////////////////////////////////////////////////////
		// EAWebKit strings
		///////////////////////////////////////////////////////////////////////

		typedef eastl::fixed_string<char16_t, 256, true, EASTLAllocator> FixedString16_256;
		typedef eastl::fixed_string<char8_t,  256, true, EASTLAllocator> FixedString8_256;

		typedef eastl::fixed_string<char16_t, 128, true, EASTLAllocator> FixedString16_128;
		typedef eastl::fixed_string<char8_t, 128, true, EASTLAllocator> FixedString8_128;
		
		typedef eastl::fixed_string<char16_t, 64, true, EASTLAllocator> FixedString16_64;
		typedef eastl::fixed_string<char8_t, 64, true, EASTLAllocator> FixedString8_64;

		typedef eastl::fixed_string<char16_t, 32, true, EASTLAllocator> FixedString16_32;
		typedef eastl::fixed_string<char8_t,  32, true, EASTLAllocator> FixedString8_32;

        typedef eastl::vector<JavascriptValue, EASTLAllocator> VectorJavaScriptValue;
        typedef eastl::hash_map<FixedString16_32, JavascriptValue, eastl::string_hash<FixedString16_32>, eastl::equal_to<FixedString16_32>, EASTLAllocator> HashMapJavaScriptValue;

		void ConvertToString8 (const FixedString16_256& s16, FixedString8_256&  s8);
		void ConvertToString8 (const FixedString16_128& s16, FixedString8_128&  s8);
		void ConvertToString8 (const FixedString16_64& s16, FixedString8_64&  s8);
		void ConvertToString8 (const FixedString16_32& s16, FixedString8_32&  s8);
		
		void ConvertToString16(const FixedString8_256&  s8,  FixedString16_256& s16);
		void ConvertToString16(const FixedString8_128&  s8,  FixedString16_128& s16);
		void ConvertToString16(const FixedString8_64&  s8,  FixedString16_64& s16);
		void ConvertToString16(const FixedString8_32&  s8,  FixedString16_32& s16);

		//Note by Arpit Baldeva: Would rather overload above functions than create the visible templates for an accidental code bloat 
		//and nuisance of specifying type with each use.
#if 0 
		template<typename StringType16_N, typename StringType8_N>
		void ConvertToString8_Internal(const StringType16_N& s16, StringType8_N& s8);

		template<typename StringType8_N, typename StringType16_N>
		void ConvertToString16_Internal(const StringType8_N& s8, StringType16_N& s16);
#endif
		//Does not belong here. Copy here only to help in port.

		///////////////////////////////////////////////////////////////////////
		// HeaderMap 
		///////////////////////////////////////////////////////////////////////

		//Change the HeaderMap to have 64 chars than 256 by default.
		//It may result in some allocations(usually for value in the key-value pair) at run-time but we are doing thousands of allocations anyway.

		// Used for HTTP header entries.
		struct fstr_iless : public eastl::binary_function<FixedString16_64, FixedString16_64, bool>
		{
			bool operator()(const FixedString16_64& a, const FixedString16_64& b) const { return (a.comparei(b) < 0); }
		};

		// Used for HeaderMap::find_as() calls.
		struct str_iless : public eastl::binary_function<FixedString16_64, const char16_t*, bool>
		{
			bool operator()(const FixedString16_64& a, const char16_t*      b) const { return (a.comparei(b) < 0); }
			bool operator()(const char16_t*      b, const FixedString16_64& a) const { return (a.comparei(b) > 0); }
		};

		typedef eastl::fixed_multimap<FixedString16_64, FixedString16_64, 8, true, fstr_iless, EASTLAllocator> HeaderMap;
        		
		FixedString8_128*	GetFixedString(const EASTLFixedString8Wrapper& wrapper);
		FixedString16_128*	GetFixedString(const EASTLFixedString16Wrapper& wrapper);
		HeaderMap*			GetHeaderMap(const EASTLHeaderMapWrapper& wrapper);
        VectorJavaScriptValue* GetVectorJavaScriptValue(const EASTLVectorJavaScriptValueWrapper& wrapper);
        HashMapJavaScriptValue* GetHashMap(const EASTLJavascriptValueHashMapWrapper& wrapper);
        HashMapJavaScriptValue::iterator* GetHashMapIterator(const EASTLJavascriptValueHashMapIteratorWrapper& wrapper);

		//#define GetFixedString(wrapper) (reinterpret_cast<EA::WebKit::FixedString8_256*>((wrapper).GetImpl()))
		//#define GetFixedString(wrapper) (reinterpret_cast<EA::WebKit::FixedString16_256*>((wrapper).GetImpl()))
		//#define GetHeaderMap(wrapper) (reinterpret_cast<EA::WebKit::HeaderMap*>((wrapper).GetImpl()))

    }
}

#endif // Header include guard
