/*
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "TextBoundaries.h"

#include <WebKitApollo/WebKit.h>

#include "StringImpl.h"
#include "TextBreakIterator.h"


#if PLATFORM(APOLLO_UNIX)
#include <wctype.h>
#endif 

namespace WebKitApollo { extern WebKitAPIHostFunctions* g_HostFunctions; }

namespace WebCore {

int findNextWordFromIndex(const UChar* chars, int len, int position, bool forward)
{
    TextBreakIterator* it = wordBreakIterator(chars, len);

    if (forward) 
    {
        position = textBreakFollowing(it, position);
        while (position != -1) 
        {
            // We stop searching when the character preceeding the break
            // is alphanumeric.
            if (position < len && iswalpha(chars[position - 1]))
                return position;

            position = textBreakFollowing(it, position);
        }

        return len;
    } 
    else 
    {
        position = textBreakPreceding(it, position);
        while (position != -1) 
        {
            // We stop searching when the character following the break
            // is alphanumeric.
            if (position > 0 && iswalpha(chars[position]))
                return position;

            position = textBreakPreceding(it, position);
        }

        return 0;
    }
}

void findWordBoundary(const UChar* chars, int len, int position, int* start, int* end)
{
    TextBreakIterator* it = wordBreakIterator(chars, len);
    *end = textBreakFollowing(it, position);
    if (*end < 0)
        *end = WebKitApollo::g_HostFunctions->textBreakLast(reinterpret_cast<void*>(it));
    *start = WebKitApollo::g_HostFunctions->textBreakPrev(reinterpret_cast<void*>(it));
}

} // namespace WebCore
