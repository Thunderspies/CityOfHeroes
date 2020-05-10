/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "uiEdit.h"
#include "UI/uiBox.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include "UI/sprite/sprite_font.h"
#include "win/win_init.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/components/estring.h>
#include "graphics/ttFontUtil.h"
#include <utilitieslib/utils/StringUtil.h>
#include "gameComm/wdwbase.h"
#include "UI/sprite/sprite_text.h"
#include "UI/uiClipper.h"
#include "render/renderprim.h"

#include "uiIME.h"
#include "win/input.h"
#include "dimm.h"

#pragma warning(disable:4996)

// ------------------------------------------------------------
// types
// ------------------------------------------------------------

typedef enum InputOrientation
{
    kInputOrientation_Vertical,
    kInputOrientation_Horizontal,
    kInputOrientation_Count    
} InputOrientation;

bool inputorientation_Valid( InputOrientation e )
{
    return INRANGE0(e, kInputOrientation_Count);
}

typedef enum IndicatorType
{ 
    INDICATOR_NON_IME, 
    INDICATOR_CHS, 
    INDICATOR_CHT, 
    INDICATOR_KOREAN, 
    INDICATOR_JAPANESE,
    kIndicatorType_Count
} IndicatorType;

#define MAX_UIIMESTRING_LEN 256
#define MAX_CANDLIST 9

//------------------------------------------------------------
//  Info about the IME input language
//
// for conversion mode: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/ime_1xo3.asp
// for sentence mode: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/ime_3n3n.asp
// for closing windows: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/ime_48kz.asp
// character sets: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/unicode_0a5v.asp
//----------------------------------------------------------
typedef struct uiIMEInput
{
    bool visible;
    DWORD langCharset; // aka the current code page
    HKL keyboardLayout;
    InputOrientation eOrient;
    wchar_t *indicator;
    struct IndicatorDims
    {
        // unscaled
        F32 usWd;
        F32 usHt;
    } indicatorDims;

    DWORD conversionMode; // IME_CMODE_NATIVE, IME_CMODE_KATAKANA, ...
    DWORD sentenceMode; // IME_SMODE_AUTOMATIC, IME_SMODE_NONE, ...
//     UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
// all I really need is ImmGetCompositionString(GCS_COMPREADSTR)
//     BOOL (WINAPI * CDXUTIMEEditBox::_ShowReadingWindow)( HIMC, BOOL );
// looks like I can post 'WM_IME_CONTROL' message with 'IMC_CLOSESTATUSWINDOW'
} uiIMEInput;

typedef struct uiIMEComposition
{
    bool visible;
    BYTE attributes[ MAX_UIIMESTRING_LEN ];
    DWORD clauses[ MAX_UIIMESTRING_LEN ];
//     S32 iCursorStart; // where the composition started in the UIEdit
    U32 lenStr; // length of the composition string.
} uiIMEComposition;

static void uiimecomposition_Reset(uiIMEComposition *c)
{
    if( verify( c ))
    {
        ZeroStruct(c);
        c->lenStr = 0;
    }
}


typedef enum CandidateWindowVisibility
{
    kCandidateWindowVisibility_None,
    kCandidateWindowVisibility_Reading,
    kCandidateWindowVisibility_Candidate,
    kCandidateWindowVisibility_Count
} CandidateWindowVisibility;

bool candidatewindowvisibility_Valid( CandidateWindowVisibility e )
{
    return INRANGE0(e, kCandidateWindowVisibility_Count);
}


typedef struct uiIMECandidateReading
{
    CandidateWindowVisibility visible;

    char pszCandidates[MAX_CANDLIST][MAX_UIIMESTRING_LEN];
    DWORD candidatesSize;  // number of candidates

    // --------------------
    // candidate info only

    DWORD selection; // current selection
    DWORD count; // the total number of candidates (including those not displayed).     
} uiIMECandidateReading;

static void uiimecandidate_Reset(uiIMECandidateReading *cd)
{
    if( verify( cd ))
    {
        ZeroStruct(cd);
    }
}


typedef struct uiIMEState
{
    uiIMEInput input;
    uiIMEComposition composition;
    uiIMECandidateReading candidate;
    UIEdit *pUIEdit; // the edit associated with this IME
    HIMC hImc;
} uiIMEState;


// ------------------------------------------------------------
// file global
// ------------------------------------------------------------

//------------------------------------------------------------
//  associate with IndicatorType, in utf8
// converted using: http://people.w3.org/rishida/scripts/uniview/conversion.en.html
//----------------------------------------------------------
wchar_t s_wszIndicator[5][5] = // String to draw to indicate current input locale
{
    L"A",
    L"\x7B80", //{0xE7, 0xAE, 0x80, 0},//L"\x7B80"
    L"\x7E41", //{0xE7, 0xB9, 0x81, 0},//L"\x7E41",
    L"\xAC00", //{0xEA, 0xB0, 0x80, 0},//L"\xAC00",
    L"\x3042", //{0xE3, 0x81, 0x82, 0}, //L"\x3042",
};
STATIC_ASSERT( ARRAY_SIZE( s_wszIndicator ) == kIndicatorType_Count );

// ------------------------------------------------------------
// functions
// ------------------------------------------------------------

static WORD GetPrimaryLanguage( HKL kl ) 
{ 
    return PRIMARYLANGID( LOWORD( kl ) ); 
}

static WORD GetSubLanguage( HKL kl ) 
{ 
    return SUBLANGID( LOWORD( kl ) ); 
}


//------------------------------------------------------------
//  helper for WM_INPUTLANGCHANGE
//----------------------------------------------------------
static bool s_HandleInputLangChange( uiIMEState *s, HWND hWnd, int charsetLocale, HKL idLocale )
{
    bool res = true; // not a lot to go wrong here

    if( verify( s && s->pUIEdit && hWnd ))
    {
        bool isIme = ImmIsIME( idLocale );
        UINT uLang = GetPrimaryLanguage( idLocale );
        UINT uSublang = GetSubLanguage( idLocale );
        HIMC hImc = NULL;
        
        //ab: what is this character set? is it the same as the code page?
        // yes, I think it is.
        s->input.langCharset = charsetLocale; 
        s->input.keyboardLayout = idLocale;        
        
        // get the conversion status
        hImc = ImmGetContext(hWnd);
        if( hImc)
        {
            // what is the difference between the two?
             ImmGetConversionStatus( hImc, &s->input.conversionMode, &s->input.sentenceMode );

            ImmReleaseContext( hWnd, hImc );
        }
        
        // --------------------
        // set thec string, candidate orientation, etc.

        // default to vertical for orientation
        s->input.eOrient = kInputOrientation_Vertical;

        //@todo: there are a million more conversion mode flags to pay attention to
        if( s->input.conversionMode & IME_CMODE_NATIVE )
        {
            switch ( uLang )
            {
                // Simplified Chinese
            case LANG_CHINESE:
                switch ( uSublang )
                {
                case SUBLANG_CHINESE_SIMPLIFIED:
                    s->input.indicator = s_wszIndicator[INDICATOR_CHS];
                    break;
                case SUBLANG_CHINESE_TRADITIONAL:
                    s->input.indicator = s_wszIndicator[INDICATOR_CHT];
                    break;
                default:    // unsupported sub-language
                    s->input.indicator = s_wszIndicator[INDICATOR_NON_IME];
                    break;
                }
                break;
                // Korean
            case LANG_KOREAN:
                s->input.indicator = s_wszIndicator[INDICATOR_KOREAN];

                // korean gets a horizontal orientation
                s->input.eOrient = kInputOrientation_Horizontal;
                break;
                // Japanese
            case LANG_JAPANESE:
                s->input.indicator = s_wszIndicator[INDICATOR_JAPANESE];
                break;
            default:
                // A non-IME language.  Obtain the language abbreviation
                // and store it for rendering the indicator later.
                s->input.indicator = s_wszIndicator[INDICATOR_NON_IME];
            }
        }
        else
        {
            // alpha-numeric mode, always use the 'A' indicator
            s->input.indicator = s_wszIndicator[INDICATOR_NON_IME];
        }

        
        // reset the unscaled dims of the string
        s->input.indicatorDims.usWd = 0.f;
        s->input.indicatorDims.usHt = 0.f;
        uiEditSetLeftIndent(s->pUIEdit, 0.f);

        // show the window
        s->input.visible = isIme;
    }
    
    // --------------------
    // finally

    return res;
}

// WM_IME_COMPOSITION
static bool s_HandleImeComposition( uiIMEState *s, HWND hWnd, DWORD dbcsChar, DWORD flgGcsType )
{
    bool res = true;
    HIMC hImc = ImmGetContext(hWnd);

    if( verify( s && hImc ))
    {
        uiIMEComposition * c = &s->composition;

        // --------------------
        // the result string.
        // NOTE: believe it or not, this message comes after the WM_IME_ENDCOMPOSITION msg
        if(flgGcsType & GCS_RESULTSTR)
        {
            // extract the string the user has entered.
            int stringSize;
            WCHAR* str;
            
            // Find out how long the string is so we can have an buffer ready for the input.
            stringSize = ImmGetCompositionStringW(hImc, GCS_RESULTSTR, NULL, 0);
            
            // Allocate a buffer of the appropriate length
            str = (WCHAR*)calloc(1, stringSize + sizeof(WCHAR));
            if(str){                
                UIEdit *edit = s->pUIEdit;
                LONG iStrEnd = stringSize/sizeof(WCHAR);

                ImmGetCompositionStringW(hImc, GCS_RESULTSTR, str, stringSize);
                str[iStrEnd] = 0;

                if( verify( s->pUIEdit ))
                {
                    // --------------------
                    // remove the composition string

                    uiedit_Backspace( s->pUIEdit, c->lenStr );
                
                    // --------------------
                    // insert the string

                    uiEditInsertUnicodeStr( edit, str, iStrEnd );
                }


                // --------------------
                //  we're done with this, reset and set cursor pos

                uiimecomposition_Reset(&s->composition);
                free(str);
            }
        }
        
        // --------------------
        // composition string change
        
        if( flgGcsType & GCS_COMPSTR )
        {
            LONG bytecountLen = 0;
            wchar_t *wbuf = NULL;

            // --------------------
            // get the string

            bytecountLen = ImmGetCompositionStringW( 
                hImc, 
                GCS_COMPSTR, 
                NULL, 0 );
            
            wbuf = malloc(bytecountLen + sizeof(*wbuf)); // alloc extra for NULL
            if( verify( wbuf ))
            {
                LONG iStrEnd;
                bytecountLen = ImmGetCompositionStringW( 
                    hImc, 
                    GCS_COMPSTR, 
                    wbuf,
                    bytecountLen
                    );
                iStrEnd = bytecountLen / sizeof( wbuf[0]);
                
                // ab: are the strings really not NULL terminated?
                // yes, yes they aren't.
                wbuf[iStrEnd] = 0;

                // remove the old string and
                // add the new
                if( verify( s->pUIEdit ))
                {
                    UIEdit *edit = s->pUIEdit;

                    uiedit_Backspace( edit, c->lenStr );
                    c->lenStr = iStrEnd;
                    
                    uiEditInsertUnicodeStr( edit, wbuf, iStrEnd );
                    uiEditSetUnderlineStateCurLine( edit, true, kuiEditAction_ToCursor, c->lenStr );
                }

                free(wbuf);
            }
            

            // --------------------
            // get the attributes

            bytecountLen = ImmGetCompositionStringW(
                hImc,
                GCS_COMPATTR,
                c->attributes,
                sizeof(c->attributes));

            if( verify( bytecountLen >= 0 ))
            {
                c->attributes[bytecountLen] = 0;
            }            
        }

        if( flgGcsType & GCS_COMPREADSTR )
        {
            // update the reading string
            wchar_t buf[MAX_UIIMESTRING_LEN] = {0};
            LONG bcLenStr = ImmGetCompositionStringW(
                hImc, 
                GCS_COMPREADSTR, 
                buf, 
                sizeof(buf));
            int i;
            for( i = 0x1; i & 0x1fff; i <<=    1 )
            {
                ZeroArray(buf);
                bcLenStr = ImmGetCompositionStringW(
                    hImc, 
                    i, 
                    buf, 
                    sizeof(buf));
            }

            if( verify( INRANGE0( bcLenStr, ARRAY_SIZE( buf ) )))
            {
                U32 i;
                uiIMECandidateReading *c = &s->candidate;

                c->candidatesSize = MIN(bcLenStr/sizeof(buf[0]), ARRAY_SIZE(c->pszCandidates));
                ZeroArray(c->pszCandidates);

                // copy the characters to the entries
                // for the reading string, each character is an entry.
                for( i = 0; i < c->candidatesSize; ++i ) 
                {
                    char *tmp = WideToUTF8CharConvert(buf[i]);
                    if( verify( tmp ))
                    {
                        strncpy(c->pszCandidates[i], tmp, ARRAY_SIZE(c->pszCandidates[i]) - 1 );
                    }
                }
            }

            // set the reading to visible ...
            c->visible = bcLenStr > 0 ? kCandidateWindowVisibility_Reading : 0;
        }

        if( flgGcsType & GCS_COMPCLAUSE )
        {
            LONG bytecountLen = ImmGetCompositionStringW( 
                hImc, 
                GCS_COMPCLAUSE,
                c->clauses,
                sizeof(c->clauses));

            if( verify( bytecountLen >= 0 ))
            {
                LONG oBufEnd = bytecountLen / sizeof(c->clauses[0]);
                c->clauses[oBufEnd] = 0;
            }
        }
    }

    // --------------------
    // finally

    if( hImc )
    {
        ImmReleaseContext(hWnd, hImc);
    }


    return res;
}
// WM_IME_NOTIFY:
//   IMN_OPENCANDIDATE
//   IMN_CHANGECANDIDATE
static bool s_HandleImeNotify_OpenChangeCandidate(uiIMEState *s, HIMC hImc)
{
    bool res = true;
    DWORD bcSizeCandList = 0;
    LPCANDIDATELIST lpCandList = NULL;
    uiIMECandidateReading *c;
    
    if( !verify( s && hImc ) )
    {
        return res;
    }

    c = &s->candidate;

    // --------------------
    // show candidate, hide reading

    c->visible = kCandidateWindowVisibility_Candidate;
    
    // --------------------
    // parse the candidate list
    
    bcSizeCandList = ImmGetCandidateListW(hImc, 0, NULL, 0);
    if( bcSizeCandList > 0 )
    {
        // --------------------
        // alloc the candidate list
        
        lpCandList = calloc(bcSizeCandList, 1);
        
        if( verify( lpCandList && c ))
        {
            int nPageTopIndex = 0;
            U32 i, j;
            
            ImmGetCandidateListW( hImc, 0, lpCandList, bcSizeCandList );

            // --------------------
            // get the selection


            switch ( GetPrimaryLanguage(s->input.keyboardLayout) )
            {
            case LANG_KOREAN:
            {
                // Korean never uses the indicator
                c->selection = -1;
            }
            break;
            case LANG_CHINESE:
            case LANG_JAPANESE: 
            default:
                c->selection = lpCandList->dwSelection;
            };

            // --------------------
            // candidate list

            // it turns out 'count' is the number of actual candidates
            // but candidateSize is the number per page
            // I'm going to show just the count,
            // but not standard behavior ...

            c->count = lpCandList->dwCount;
            c->candidatesSize = MIN(c->count, MAX_CANDLIST ); //MIN( lpCandList->dwPageSize, MAX_CANDLIST );
            
            if( GetPrimaryLanguage(s->input.keyboardLayout) == LANG_JAPANESE )
            {
                // Japanese IME organizes its candidate list a little
                // differently from the other IMEs.
                nPageTopIndex = ( c->selection/ c->candidatesSize ) * c->candidatesSize;
            }
            else
                nPageTopIndex = lpCandList->dwPageStart;
            
            ZeroArray(c->pszCandidates);
            
            // parse the candidate list
            for( i = nPageTopIndex, j = 0;
                 (DWORD)i < lpCandList->dwCount 
                     && j < c->candidatesSize;
                 i++, j++ )
            {
                // Initialize the candidate list strings
                char* psz = c->pszCandidates[j];
                WCHAR *pwszNewCand = NULL;
                char *convertedTmp = NULL;
                int lenTemp = 0;

                // For every candidate string entry,
                // write [index] + Space + [String] if vertical,
                // write [index] + [String] + Space if horizontal.
                *psz++ = ( '0' + ( (j + 1) % 10 ) );  // Index displayed is 1 based
//                 if( s->input.eOrient == kInputOrientation_Vertical )
//                 {
//                     *psz++ = ' ';
//                 }
                
                pwszNewCand = (LPWSTR)( (LPBYTE)lpCandList + lpCandList->dwOffset[i] );
                convertedTmp = WideToUTF8StrTempConvert( pwszNewCand, &lenTemp );

                lenTemp = MIN(lenTemp, ARRAY_SIZE( c->pszCandidates[j]) - 1);
                strncpy(psz, convertedTmp, lenTemp);
            }
        }
        
        // --------------------
        // finally
        
        if( lpCandList )
        {
            free(lpCandList);
        }
    }
    
    // --------------------
    // finally

    return res;
}


// ----------------------------------------
// the global state

static uiIMEState s_IMEState = {0};


//================================================================================
//  Lifted from directX sample C++/Common/DXUTgui.cpp
// this section is madness
//================================================================================

//#if 0
#define CHT_IMEFILENAME1    "TINTLGNT.IME" // New Phonetic
#define CHT_IMEFILENAME2    "CINTLGNT.IME" // New Chang Jie
#define CHT_IMEFILENAME3    "MSTCIPHA.IME" // Phonetic 5.1
#define CHS_IMEFILENAME1    "PINTLGNT.IME" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    "MSSCIPYA.IME" // MSPY3 for OfficeXP

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor ) \
    ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )    // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )    // New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )    // New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )    // New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )    // New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )    // New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )    // New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
#define IMEID_CHS_VER41    ( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )    // MSPY1.5    // SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42    ( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )    // MSPY2    // Win2k/WinME
#define IMEID_CHS_VER53    ( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )    // MSPY3    // WinXP


//--------------------------------------------------------------------------------------
//    GetImeId( UINT uIndex )
//        returns 
//    returned value:
//    0: In the following cases
//        - Non Chinese IME input locale
//        - Older Chinese IME
//        - Other error cases
//
//    Othewise:
//      When uIndex is 0 (default)
//            bit 31-24:    Major version
//            bit 23-16:    Minor version
//            bit 15-0:    Language ID
//        When uIndex is 1
//            pVerFixedInfo->dwFileVersionLS
//
//    Use IMEID_VER and IMEID_LANG macro to extract version and language information.
//    

// We define the locale-invariant ID ourselves since it doesn't exist prior to WinXP
// For more information, see the CompareString() reference.
#define LCID_INVARIANT MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT)

static DWORD GetImeId( UINT uIndex )
{
    static HKL hklPrev = 0;
    static DWORD dwID[2] = { 0, 0 };  // Cache the result
    
    DWORD   dwVerSize;
    DWORD   dwVerHandle;
    LPVOID  lpVerBuffer;
    LPVOID  lpVerData;
    UINT    cbVerData;
    char    szTmp[1024];

    if( uIndex >= sizeof( dwID ) / sizeof( dwID[0] ) )
        return 0;

    if( hklPrev == s_IMEState.input.keyboardLayout )
        return dwID[uIndex];

    hklPrev = s_IMEState.input.keyboardLayout; //s_hklCurrent;  // Save for the next invocation

    // Check if we are using an older Chinese IME
    if( !( ( s_IMEState.input.keyboardLayout == _CHT_HKL ) || ( s_IMEState.input.keyboardLayout == _CHT_HKL2 ) || ( s_IMEState.input.keyboardLayout == _CHS_HKL ) ) )
    {
        dwID[0] = dwID[1] = 0;
        return dwID[uIndex];
    }

    // Obtain the IME file name
    if ( !ImmGetIMEFileNameA( s_IMEState.input.keyboardLayout, szTmp, ( sizeof(szTmp) / sizeof(szTmp[0]) ) - 1 ) )
    {
        dwID[0] = dwID[1] = 0;
        return dwID[uIndex];
    }

    // Check for IME that doesn't implement reading string API
    {
        if( ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME2, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME3, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME2, -1 ) != CSTR_EQUAL ) )
        {
            dwID[0] = dwID[1] = 0;
            return dwID[uIndex];
        }
    }

    dwVerSize = GetFileVersionInfoSizeA( szTmp, &dwVerHandle );
    if( dwVerSize )
    {
        lpVerBuffer = HeapAlloc( GetProcessHeap(), 0, dwVerSize );
        if( lpVerBuffer )
        {
            if( GetFileVersionInfoA( szTmp, dwVerHandle, dwVerSize, lpVerBuffer ) )
            {
                if( VerQueryValueA( lpVerBuffer, "\\", &lpVerData, &cbVerData ) )
                {
                    DWORD dwVer = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionMS;
                    dwVer = ( dwVer & 0x00ff0000 ) << 8 | ( dwVer & 0x000000ff ) << 16;
                    if( 
                        ( LOWORD(s_IMEState.input.keyboardLayout) == LANG_CHT &&
                          ( dwVer == MAKEIMEVERSION(4, 2) || 
                            dwVer == MAKEIMEVERSION(4, 3) || 
                            dwVer == MAKEIMEVERSION(4, 4) || 
                            dwVer == MAKEIMEVERSION(5, 0) ||
                            dwVer == MAKEIMEVERSION(5, 1) ||
                            dwVer == MAKEIMEVERSION(5, 2) ||
                            dwVer == MAKEIMEVERSION(6, 0) ) )
                        ||
                        ( LOWORD(s_IMEState.input.keyboardLayout) == LANG_CHS &&
                          ( dwVer == MAKEIMEVERSION(4, 1) ||
                            dwVer == MAKEIMEVERSION(4, 2) ||
                            dwVer == MAKEIMEVERSION(5, 3) ) )
                      )
                    {
                        dwID[0] = dwVer | LOWORD(s_IMEState.input.keyboardLayout);
                        dwID[1] = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS;
                    }
                }
            }
            HeapFree( GetProcessHeap(), 0, lpVerBuffer );
        }
    }

    return dwID[uIndex];
}

static void GetPrivateReadingString(HWND hWnd, uiIMEState *s )
{
    static INPUTCONTEXT* (WINAPI * _ImmLockIMC)( HIMC ) = NULL;
    static INPUTCONTEXT* (WINAPI * _ImmUnlockIMC)( HIMC ) = NULL;
    static LPVOID (WINAPI * _ImmLockIMCC)( HIMCC );
    static LPVOID (WINAPI * _ImmUnlockIMCC)( HIMCC );

    DWORD dwId = GetImeId(0);
    HIMC hImc;
    DWORD dwReadingStrLen = 0;
    DWORD dwErr = 0;
    WCHAR *wstr = 0;
    bool bUnicodeIme = false;  // Whether the IME context component is Unicode.
    INPUTCONTEXT *lpIC = NULL;
    uiIMECandidateReading *c = NULL;
    hImc = ImmGetContext( hWnd );

    if( !verify(dwId && hImc && s))
    {
        return;
    }

    c = &s_IMEState.candidate;
    {
        // IMEs that doesn't implement Reading String API
        LPBYTE p = 0;
        if( !_ImmLockIMC )
        {
            WCHAR wszPath[MAX_PATH+1];
            HINSTANCE hDllImm32 = NULL;
            if( !GetSystemDirectoryW( wszPath, MAX_PATH+1 ) )
                return;
            lstrcatW( wszPath, L"\\imm32.dll" );
            hDllImm32 = LoadLibraryW( wszPath );
            if( hDllImm32 )
            {
                *(FARPROC*)&_ImmLockIMC = GetProcAddress( hDllImm32, "ImmLockIMC" );
                *(FARPROC*)&_ImmUnlockIMC = GetProcAddress( hDllImm32, "ImmUnlockIMC" );
                *(FARPROC*)&_ImmLockIMCC = GetProcAddress( hDllImm32, "ImmLockIMCC" );
                *(FARPROC*)&_ImmUnlockIMCC = GetProcAddress( hDllImm32, "ImmUnlockIMCC" );
            }

            FreeLibrary( hDllImm32 );
        }

        if( !verify( _ImmLockIMC && _ImmUnlockIMC ))
        {
            return;
        }

        lpIC = _ImmLockIMC( hImc );
        
        switch( dwId )
        {
            case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
            case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
            case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP
                p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 24 );
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)( p + 7 * 4 + 32 * 4 );
                dwErr = *(DWORD *)( p + 8 * 4 + 32 * 4 );
                wstr = (WCHAR *)( p + 56 );
                bUnicodeIme = true;
                break;

            case IMEID_CHT_VER50: // 5.0.x.x // WinME
                p = *(LPBYTE *)( (LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 3 * 4 );
                if( !p ) break;
                p = *(LPBYTE *)( (LPBYTE)p + 1*4 + 5*4 + 4*2 );
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16);
                dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 + 1*4);
                wstr = (WCHAR *)(p + 1*4 + (16*2+2*4) + 5*4);
                bUnicodeIme = false;
                break;

            case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
            case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
            case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
                p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 4);
                if( !p ) break;
                p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4);
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2);
                dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2 + 1*4);
                wstr  = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
                bUnicodeIme = true;
                break;

            // the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
            case IMEID_CHS_VER41:
            {
                int nOffset;
                nOffset = ( GetImeId( 1 ) >= 0x00000002 ) ? 8 : 7;

                p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + nOffset * 4);
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)(p + 7*4 + 16*2*4);
                dwErr = *(DWORD *)(p + 8*4 + 16*2*4);
                dwErr = min( dwErr, dwReadingStrLen );
                wstr = (WCHAR *)(p + 6*4 + 16*2*1);
                bUnicodeIme = true;
                break;
            }

            case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
            {
                OSVERSIONINFOW osi;
                int nTcharSize;
                osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
                GetVersionExW( &osi );

                nTcharSize = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? sizeof(WCHAR) : sizeof(char);
                p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 1*4 + 1*4 + 6*4);
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize);
                dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize + 1*4);
                wstr  = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
                bUnicodeIme = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? true : false;
            }
        }   // switch
    }

    // Copy the reading string to the candidate list first
//     s_CandList.awszCandidate[0][0] = 0;
//     s_CandList.awszCandidate[1][0] = 0;
//     s_CandList.awszCandidate[2][0] = 0;
//     s_CandList.awszCandidate[3][0] = 0;
//     s_CandList.dwCount = dwReadingStrLen;
//     s_CandList.dwSelection = (DWORD)-1; // do not select any char
    ZeroArray( c->pszCandidates );
    c->selection = (DWORD)-1;

    if( bUnicodeIme )
    {
        UINT i;
        c->candidatesSize = dwReadingStrLen;
        for( i = 0; i < dwReadingStrLen; ++i ) // dwlen > 0, if known IME
        {
            if( dwErr <= i && c->selection == (DWORD)-1 )
            {
                // select error char
                //s_CandList.dwSelection = i;
                c->selection = i;
            }

            strncpy( c->pszCandidates[i], WideToUTF8CharConvert(wstr[i]), ARRAY_SIZE(c->pszCandidates[i]) );
        }
        
    }
    else
    {
        char *p = (char *)wstr;
        DWORD i = 0;
        c->candidatesSize = 0;
        while( i < dwReadingStrLen )
        {
            char *nextStart = UTF8GetNextCharacter(p+i);
            int strwidth = nextStart - (p + i);
            if( dwErr <= i && c->selection == (DWORD)-1 )
            {
                c->selection = i;
            }
            strncpy( c->pszCandidates[i], p+i, strwidth );
            c->candidatesSize++;
            i += MAX(strwidth, 1); // always move forward at least 1
        }
    }
    _ImmUnlockIMCC( lpIC->hPrivate );
    _ImmUnlockIMC( hImc );
    ImmReleaseContext( hWnd, hImc );

    // Copy the string to the reading string buffer
    if( c->candidatesSize > 0 )
    {
        c->visible = kCandidateWindowVisibility_Reading;
    }
    else
    {
        c->visible = kCandidateWindowVisibility_None;
    }
}

//#endif 

//------------------------------------------------------------
//  from international_ime microsoft example
//----------------------------------------------------------
static void s_SetCandidateWindowPos(uiIMEState * s, HWND hWnd, LONG x_cursor, LONG y_cursor) 
{
    HIMC        hIMC;
    CANDIDATEFORM Candidate;

    if ( hIMC = ImmGetContext(hWnd) ) 
    {        
        Candidate.dwIndex = 0;
        Candidate.dwStyle = CFS_FORCE_POSITION;

        if ( GetPrimaryLanguage( s->input.keyboardLayout ) == LANG_JAPANESE) 
        {
            Candidate.ptCurrentPos.x = x_cursor;
        }
        else
        {
            Candidate.ptCurrentPos.x = x_cursor;
        }
        Candidate.ptCurrentPos.y = y_cursor;
        ImmSetCandidateWindow(hIMC, &Candidate);
        ImmReleaseContext(hWnd,hIMC);
    }
}

// ================================================================================
// end MS madness section
// ================================================================================

// just let the ime api know where the caret is
static void SetCompositionWindowPos(HWND hWnd, F32 x, F32 y) 
{
    HIMC        hIMC;
    COMPOSITIONFORM Composition;

    if ( hIMC = ImmGetContext(hWnd) ) 
    {
        // Set composition window position near caret position
        Composition.dwStyle = CFS_POINT;
        Composition.ptCurrentPos.x = x;
        Composition.ptCurrentPos.y = y;
        ImmSetCompositionWindow(hIMC, &Composition);
        ImmReleaseContext(hWnd,hIMC);
    }
}

static bool s_HandleImeNotify( uiIMEState * s, HWND hWnd, DWORD imnCmd, DWORD data)
{
    bool res = true;
    HIMC hImc = ImmGetContext(hWnd);

    if( verify( s && hImc ))
    {
        uiIMECandidateReading *c = &s->candidate;
        
        switch ( imnCmd )
        {
        case IMN_SETOPENSTATUS:
            {
                // forward this to input lang change
                res = s_HandleInputLangChange( s, hWnd, GetACP(), GetKeyboardLayout( 0 ) );
            }
            break;
        case IMN_OPENCANDIDATE:
        case IMN_CHANGECANDIDATE:
            // candidate string is about to change
            res = s_HandleImeNotify_OpenChangeCandidate( s, hImc );
            break;
        case IMN_CLOSECANDIDATE:
            {
                s->candidate.visible = kCandidateWindowVisibility_None;
            }
            break;
        case IMN_PRIVATE:
        {            
            // --------------------
            // kooky plan: hack into memory and see if that works

            {
                GetPrivateReadingString( hWnd, &s_IMEState );
            }
                
            // --------------------
            // kooky plan 1: set the window position and use the windows window

//             if(0)    
//             {
//                 UIEdit const *edit = s->pUIEdit;
//                 //edit->cursor.lastXPos, edit->bounds.y
//                 s_SetCandidateWindowPos( s, hWnd, edit->cursor.lastXPos + edit->bounds.x, edit->bounds.y ); // hacking this in for now
//                 printf("cursor lastxpos:%d\n",edit->cursor.lastXPos);
                
//                 // 'handle' the message if we don't have 
//                 // an edit control active
//                 //res = false; //s->composing;
//                 //printf(res ? "intercepting private\n" : "");
//             }
            
            // --------------------
            // not so kooky plan: try getting the reading string using the published api
            // doesn't work for some reason though.

//             if(0)
//             {
//                 // update the reading string
//                 wchar_t buf[MAX_UIIMESTRING_LEN] = {0};
//                 LONG bcLenStr = ImmGetCompositionStringW(
//                     hImc, 
//                     GCS_RESULTREADSTR, 
//                     buf, 
//                     sizeof(buf));
                
//                 if( verify( INRANGE0( bcLenStr, ARRAY_SIZE( buf ) )))
//                 {
//                     U32 i;
                    
                    
//                     c->candidatesSize = MIN(bcLenStr/sizeof(buf[0]), ARRAY_SIZE(c->pszCandidates));
//                     ZeroArray(c->pszCandidates);
                    
//                     // copy the characters to the entries
//                     // for the reading string, each character is an entry.
//                     for( i = 0; i < c->candidatesSize; ++i ) 
//                     {
//                         char *tmp = WideToUTF8CharConvert(buf[i]);
//                         if( verify( tmp ))
//                         {
//                             strncpy(c->pszCandidates[i], tmp, ARRAY_SIZE(c->pszCandidates[i]) - 1 );
//                         }
//                     }
//                 }
                
//                 // set the reading to visible ...
//                 c->visible = bcLenStr > 0 ? kCandidateWindowVisibility_Reading : 0;
//             }
        }    
        break;
        default:
            // do nothing
            break;
        };
    }
    

    // --------------------
    // finally

    if( hImc )
    {
        ImmReleaseContext(hWnd, hImc);
    }

    return res;
}

static void imeMsgPrint(UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    static DWORD cnt = 0;
    cnt++;
#define HDR() printf("%d:\t", cnt)
    switch(uMsg)
    {
    case WM_INPUTLANGCHANGE: 
        HDR();
        {
            printf("WM_INPUTLANGCHANGE\n");
        }
        break;
    case WM_IME_CHAR: 
        HDR();
        printf("WM_IME_CHAR\n");
        break;
    case WM_IME_COMPOSITION: 
        HDR();
        printf("WM_IME_COMPOSITION wParam(0x%x) ", wParam);
        
        {
            struct GCSPairs
            {
                DWORD id;
                char const *str;
            } ids[] = 
                {
                    
                    { GCS_COMPATTR, "GCS_COMPATTR" },
                    { GCS_COMPCLAUSE, "GCS_COMPCLAUSE" },
                    { GCS_COMPREADSTR, "GCS_COMPREADSTR" },
                    { GCS_COMPREADATTR, "GCS_COMPREADATTR" },
                    { GCS_COMPREADCLAUSE, "GCS_COMPREADCLAUSE" },
                    { GCS_COMPSTR, "GCS_COMPSTR" },
                    { GCS_CURSORPOS, "GCS_CURSORPOS" },
                    { GCS_DELTASTART, "GCS_DELTASTART" },
                    { GCS_RESULTCLAUSE, "GCS_RESULTCLAUSE" },
                    { GCS_RESULTREADCLAUSE, "GCS_RESULTREADCLAUSE" },
                    { GCS_RESULTREADSTR, "GCS_RESULTREADSTR" },
                    { GCS_RESULTSTR, "GCS_RESULTSTR" },
                };
            int i;
            
            printf("lParam(");
            for( i = 0; i < ARRAY_SIZE( ids ); ++i ) 
            {
                if( ids[i].id & lParam )
                {
                    printf("%s,", ids[i].str);
                }
            }
            printf(")\n");
        }

        break;
    case WM_IME_COMPOSITIONFULL: 
        HDR();
        {
            printf("WM_IME_COMPOSITIONFULL\n");
        }
        break;
    case WM_IME_CONTROL:
        HDR();
        {
            struct IdStrPairs
            {
                int id;
                char const *str;
            } idStrPairs[] = {
                {IMC_CLOSESTATUSWINDOW,"IMC_CLOSESTATUSWINDOW"},
                {IMC_GETCANDIDATEPOS,"IMC_GETCANDIDATEPOS"},
                {IMC_GETCOMPOSITIONFONT,"IMC_GETCOMPOSITIONFONT"},
                {IMC_GETCOMPOSITIONWINDOW,"IMC_GETCOMPOSITIONWINDOW"},
                {IMC_GETSTATUSWINDOWPOS,"IMC_GETSTATUSWINDOWPOS"},
                {IMC_OPENSTATUSWINDOW,"IMC_OPENSTATUSWINDOW"},
                {IMC_SETCANDIDATEPOS,"IMC_SETCANDIDATEPOS"},
                {IMC_SETCOMPOSITIONFONT,"IMC_SETCOMPOSITIONFONT"},
                {IMC_SETCOMPOSITIONWINDOW,"IMC_SETCOMPOSITIONWINDOW"},
                {IMC_SETSTATUSWINDOWPOS ,"IMC_SETSTATUSWINDOWPOS "},
            };    
            int i;

            printf("WM_IME_CONTROL:");

            for(i = 0; i < ARRAY_SIZE( idStrPairs ); ++i)
            {
                if( wParam == idStrPairs[i].id )
                {
                    printf("wParam(%s)", idStrPairs[i].str);
                    break;
                }
            }
            printf("\n");
        }
        break;
    case WM_IME_SELECT:
        HDR();
        {
            printf("WM_IME_SELECT selected(%s) locale(%d)", wParam ? "true" : "false", lParam);
        }
        break;
    case WM_IME_SETCONTEXT:
        HDR();
        {
            struct IdStrPairs
            {
                int id;
                char const *str;
            } idStrPairs[] = {
                {ISC_SHOWUICANDIDATEWINDOW, "ISC_SHOWUICANDIDATEWINDOW"},
                {ISC_SHOWUICOMPOSITIONWINDOW,"ISC_SHOWUICOMPOSITIONWINDOW"},
                {ISC_SHOWUIGUIDELINE,"ISC_SHOWUIGUIDELINE"},
                {ISC_SHOWUICANDIDATEWINDOW,"ISC_SHOWUICANDIDATEWINDOW 0"},
                {ISC_SHOWUICANDIDATEWINDOW << 1,"ISC_SHOWUICANDIDATEWINDOW 1"},
                {ISC_SHOWUICANDIDATEWINDOW << 2,"ISC_SHOWUICANDIDATEWINDOW 2"},
                {ISC_SHOWUICANDIDATEWINDOW << 3,"ISC_SHOWUICANDIDATEWINDOW 3"},
                {ISC_SHOWUIALL,"ISC_SHOWUIALL"},
            };    
            int i;

            printf("WM_IME_SETCONTEXT: active(%s) display flags(", wParam ? "true" : "false");

            for(i = 0; i < ARRAY_SIZE( idStrPairs ); ++i)
            {
                if( lParam & idStrPairs[i].id )
                {
                    printf("%s,", idStrPairs[i].str);
                }
            }

            printf(")\n");
        }
        break;
    case WM_IME_KEYDOWN:
        HDR();
        printf("WM_IME_KEYDOWN\n");
        break;
    case WM_IME_KEYUP:
        HDR();
        printf("WM_IME_KEYUP\n");
        break;
    case WM_IME_NOTIFY:
        if( wParam != IMN_SETCANDIDATEPOS )
        {
            HDR();
            printf("WM_IME_NOTIFY: ");
            {
                struct IMENotify
                { 
                    int id;
                    char *str;
                } imes[] = 
                {
                    {IMN_CHANGECANDIDATE,"IMN_CHANGECANDIDATE"},
                    {IMN_CLOSECANDIDATE,"IMN_CLOSECANDIDATE"},
                    {IMN_CLOSESTATUSWINDOW,"IMN_CLOSESTATUSWINDOW"},
                    {IMN_GUIDELINE,"IMN_GUIDELINE"},
                    {IMN_OPENCANDIDATE,"IMN_OPENCANDIDATE"},
                    {IMN_OPENSTATUSWINDOW,"IMN_OPENSTATUSWINDOW"},
                    {IMN_SETCANDIDATEPOS,"IMN_SETCANDIDATEPOS"},
                    {IMN_SETCOMPOSITIONFONT,"IMN_SETCOMPOSITIONFONT"},
                    {IMN_SETCOMPOSITIONWINDOW,"IMN_SETCOMPOSITIONWINDOW"},
                    {IMN_SETCONVERSIONMODE,"IMN_SETCONVERSIONMODE"},
                    {IMN_SETOPENSTATUS,"IMN_SETOPENSTATUS"},
                    {IMN_SETSENTENCEMODE,"IMN_SETSENTENCEMODE"},
                    {IMN_SETSTATUSWINDOWPOS ,"IMN_SETSTATUSWINDOWPOS "},
                    {IMN_PRIVATE,"IMN_PRIVATE"},
                };
                int i;
                for( i = 0; i < ARRAY_SIZE( imes ); ++i ) 
                {
                    if( imes[i].id == wParam)
                    {
                        char *extra = NULL;
                        if( wParam == IMN_SETCONVERSIONMODE )
                        {
                            struct IMECMode
                            {
                                int mask;
                                char *id;
                            } cmodes[] = {
                                { IME_CMODE_CHARCODE, "IME_CMODE_CHARCODE" },
                                { IME_CMODE_EUDC, "IME_CMODE_EUDC" },
                                { IME_CMODE_FIXED, "IME_CMODE_FIXED" },
                                { IME_CMODE_FULLSHAPE, "IME_CMODE_FULLSHAPE" },
                                { IME_CMODE_HANJACONVERT, "IME_CMODE_HANJACONVERT" },
                                { IME_CMODE_KATAKANA, "IME_CMODE_KATAKANA" },
                                { IME_CMODE_NATIVE, "IME_CMODE_NATIVE" },
                                { IME_CMODE_NOCONVERSION, "IME_CMODE_NOCONVERSION" },
                                { IME_CMODE_ROMAN, "IME_CMODE_ROMAN" },
                                { IME_CMODE_SOFTKBD, "IME_CMODE_SOFTKBD" },
                                { IME_CMODE_SYMBOL, "IME_CMODE_SYMBOL" },
                            };
                            char *str = NULL;
                            int j;
                            DWORD cstat, sstat; // converson and senetence modes
                            ImmGetConversionStatus( ImmGetContext(hwnd), &cstat, &sstat );
                            for(j = 0; j < ARRAY_SIZE(cmodes); ++j)
                            {
                                if( cmodes[j].mask & cstat )
                                {
                                    estrPrintf( &extra, "%s,", cmodes[j].id );
                                }
                            }
                        }

                        printf("\t%s(%s)\n",imes[i].str, extra ? extra : "");
                        estrDestroy(&extra);
                        break;
                    }
                }

                //if( i == ARRAY_SIZE(imes) )
                //{
                //    printf("\t'%d' wparam not found\n", wParam);
                //}
            }
        }
        break;
    case WM_IME_REQUEST:
        HDR();
        printf("WM_IME_REQUEST: ");
        {
            struct IMERequest
            { 
                int id;
                char *str;
            } imers[] = 
            {
                {IMR_CANDIDATEWINDOW,"IMR_CANDIDATEWINDOW"},
                {IMR_COMPOSITIONFONT,"IMR_COMPOSITIONFONT"},
                {IMR_COMPOSITIONWINDOW,"IMR_COMPOSITIONWINDOW"},
                {IMR_CONFIRMRECONVERTSTRING,"IMR_CONFIRMRECONVERTSTRING"},
                {IMR_DOCUMENTFEED,"IMR_DOCUMENTFEED"},
                {IMR_QUERYCHARPOSITION,"IMR_QUERYCHARPOSITION"},
                {IMR_RECONVERTSTRING ,"IMR_RECONVERTSTRING "},
            };
            int i;
            for( i = 0; i < ARRAY_SIZE( imers ); ++i ) 
            {
                if( imers[i].id == wParam )
                {
                    printf("\t%s\n",imers[i].str);
                    break;
                }
            }

            if( i == ARRAY_SIZE(imers) )
            {
                printf("\t0x%x (unknown)\n", wParam);
            }
        }
        break;
    case WM_IME_STARTCOMPOSITION:
        HDR();
        printf("WM_IME_STARTCOMPOSITION\n");;
        
        break;
    case WM_IME_ENDCOMPOSITION:
        HDR();
        printf("WM_IME_ENDCOMPOSITION\n");        
        break;    
    default:
        cnt--;
        break;
    };
#undef HDR
}

bool uiIME_MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    bool res = false;

    // debug help
    //imeMsgPrint(uMsg, wParam, lParam);

    switch ( uMsg ) // HANDLE_MSG
    {
        //ab: this is probably the right way to ignore IME, but in the
        //'if it aint broke' dept: the ArenaNet guy thought manually
        //disabling IME was better...  
        //
        //if bugs start happening with IME, maybe give this a try.  if
        //you uncomment this, make sure to comment out the two
        //ImmAssociateContext calls in uiIME_OnFocus
//     case WM_INPUTLANGCHANGEREQUEST:
//     {
//         // special behavior: only allow if we have an edit, but if
//         // we have an edit, pass to defwindowproc
//         if( s_IMEState.pUIEdit )
//         {
//             DefWindowProc (hWnd, uMsg, wParam, lParam);
//         }
//         res = true;
//     }
//     break;
    case WM_INPUTLANGCHANGE:
    {
        res = !s_IMEState.pUIEdit
            || s_HandleInputLangChange( &s_IMEState, hWnd, wParam, (HKL)lParam );
    }
    break;
    case WM_IME_CHAR:
        // lifted verbatim from win_init.c...
        verify(0); // I just don't think this gets called
    break;
    case WM_IME_COMPOSITION:
    {
        res = !s_IMEState.pUIEdit
            || s_HandleImeComposition( &s_IMEState, hWnd, wParam, lParam );
    }
    break;
    case WM_IME_COMPOSITIONFULL:
    {
        verify(0&&"should never get this, get someone if you do.");
    }
    break;
    case WM_IME_CONTROL:
    {
        verify(0&&"should never get this, get someone if you do.");
    }
    break;
    case WM_IME_KEYDOWN:
    {
        // ignore
    }
    break;
    case WM_IME_KEYUP:
    {
        // ignore
    }
    break;
    case WM_IME_NOTIFY:
    {
        res = !s_IMEState.pUIEdit
            || s_HandleImeNotify(&s_IMEState, hWnd, wParam, lParam );
        //printf("WM_IME_NOTIFY %x %x\n", wParam, lParam);
    }
    break;
    case WM_IME_REQUEST:
    {
        // ignore
    }
    break;
    case WM_IME_SELECT:
    {
        // do I do something here?
    }
    break;
    case WM_IME_SETCONTEXT:
    {
        // ignored
    }
    break;
    case WM_IME_STARTCOMPOSITION:
    {
        // clear the composition string
        if( s_IMEState.pUIEdit )
        {
            s_IMEState.composition.visible = true;
            s_IMEState.composition.lenStr = 0;
        }
        res = true;
    }
    break;
    case WM_IME_ENDCOMPOSITION:
    {
        // not sure on the details here...
        if( s_IMEState.pUIEdit )
        {
            UIEdit *edit = s_IMEState.pUIEdit;
            uiIMEComposition * c = &s_IMEState.composition;

            // --------------------
            // clean up candidate and composition information

            // hide the candidate and composition feedback
            s_IMEState.candidate.visible = kCandidateWindowVisibility_None;
            s_IMEState.composition.visible = false;

            // don't reset composition here, the cursor pos is needed for the result string.
            uiimecandidate_Reset(&s_IMEState.candidate);

            // clear the underline
            if( verify( s_IMEState.pUIEdit ))
            {
                uiEditSetUnderlineStateCurLine( s_IMEState.pUIEdit, false, 0, 0 );
            }
        }
        res = true;
    }
    break;
    default:
        // unhandled, just skip
        break;
    };

    // --------------------
    // finally

    return res;
}

// ------------------------------------------------------------
// rendering 

void uiIMEIndicatorRender(F32 z_ind, F32 sc, int color, int bcolor)
{
    uiIMEInput *il = &s_IMEState.input;
    
    // --------------------
    // render the indicator
    
    if( s_IMEState.input.visible && verify( il->indicator && s_IMEState.pUIEdit ))
    {
        TTDrawContext *fnt = &game_9;
        F32 wd_ind = 0.f;
        F32 ht_ind = 0.f;
        CBox dims = {0};

        // recalc the indicator dims
        if( s_IMEState.input.indicatorDims.usWd == 0.f )
        {
            CBox dims = {0};
            char *tmp = WideToUTF8StrTempConvert(il->indicator, NULL);


            // note: could cache these dims
            if(verify(tmp))
            {
                F32 usWd;
                str_dims( fnt, 1.f, 1.f, false, &dims, tmp );
                usWd = dims.right-dims.left+PIX3*2;
                s_IMEState.input.indicatorDims.usWd = usWd;
                s_IMEState.input.indicatorDims.usHt = dims.bottom-dims.top+PIX3*2;

                // set the indent for the edit
                uiEditSetLeftIndent(s_IMEState.pUIEdit, sc*(usWd+2) );
            }
        }

        wd_ind = il->indicatorDims.usWd*sc;
        ht_ind = il->indicatorDims.usHt*sc;

        {
            F32 x_ind = s_IMEState.pUIEdit->boundsPage.x + 1*sc;
            F32 y_ind = s_IMEState.pUIEdit->boundsPage.y + 3*sc;
            char *tmp = WideToUTF8StrTempConvert(il->indicator, NULL ); // could cache this...

            // draw frame
            drawFlatFrame( PIX3, R10, x_ind, y_ind, z_ind, 
                wd_ind, ht_ind, sc, color, bcolor );

            // draw the text
            font(fnt);

            // set the color based on the conversion mode
            //         if( il->conversionMode & IME_CMODE_NATIVE )
            //         {
                         font_color(CLR_WHITE, CLR_WHITE);
            //         }
            //         else
            //         {
            //             font_color(CLR_BLACK, CLR_BLACK);
            //         }



            if(verify(tmp))
            {
                prnt( x_ind + PIX3*sc, y_ind + (s_IMEState.input.indicatorDims.usHt - PIX3)*sc, z_ind, sc, sc, tmp);
            }
        }
        
    }
}


static void s_renderComposition( TTDrawContext *fnt, F32 x_comp, F32 y_comp, F32 z_comp, F32 sc )
{
    // I don't have to render this any longer. it goes directly
    // into the UIEdit

//    uiIMEComposition *c = &s_IMEState.composition;
//    if( c->visible && verify( fnt ) )
//    {
//        F32 dy_fnt = str_height( fnt, sc, sc, false, c->buf );
//        // just draw the composition
//        font(fnt);
//        font_color(CLR_WHITE, CLR_WHITE);
//        prnt( x_comp, y_comp + dy_fnt, z_comp, sc, sc, c->buf );
//    }    
}


//------------------------------------------------------------
//  The candidate window pops up over where the user is currently editing
//----------------------------------------------------------
static void s_renderCandidates( TTDrawContext *fnt, UIBox const *dimsParent, F32 x_candidate, F32 y_candidate, F32 z_candidate, F32 sc, int color, int bcolor )
{
    uiIMECandidateReading *c = &s_IMEState.candidate;
    if( c->visible && verify(dimsParent && fnt ) )
    {
        UIBox dims = {0};
        U32 i;
        F32 dx_txtpad = PIX3*sc;
        F32 dy_txtpad = 0;
        UIBox relPosTxts[MAX_CANDLIST] = {0};
        bool verticalLayout = s_IMEState.input.eOrient == kInputOrientation_Vertical;

        // --------------------
        // first calc the dims

        // in-bounds the page size (shouldn't happen, ever)
        if( !verify( c->candidatesSize <= ARRAY_SIZE( relPosTxts )))
        {
            c->candidatesSize = ARRAY_SIZE( relPosTxts );
        }

        // the text
        // NOTE: this code depends closely on the rendering code below
        for( i = 0; i < c->candidatesSize; ++i ) 
        {
            CBox bx = {0};
            F32 ht;
            F32 wd;
            
            str_dims(fnt, sc, sc, false, &bx, c->pszCandidates[i]);
            ht = bx.bottom - bx.top;
            wd = bx.right - bx.left;
            
            relPosTxts[i].width = wd;
            relPosTxts[i].height = ht;

            relPosTxts[i].x = dx_txtpad;
            relPosTxts[i].y = dy_txtpad;

            if( verticalLayout )
            {
                relPosTxts[i].y = dims.height;
                dims.height += ht + dy_txtpad;
                dims.width = MAX( dims.width, wd + 2*PIX4*sc );
            }
            else
            {
                relPosTxts[i].x = dims.width;
                dims.height = MAX( dims.height, ht + PIX4*sc );
                dims.width += wd + dx_txtpad;
            }
        }

        // --------------------
        // position

        dims.x = x_candidate;
        dims.y = y_candidate - dims.height;

        // in-bounds the window
        uiBoxMakeInbounds( dimsParent, &dims );

        // --------------------
        // render
        
        {
            F32 z_cur = z_candidate + 1;
            F32 z_txt = z_candidate + 2;            
            
            // draw frame
            uiDrawBox( &dims, z_candidate, color, bcolor );
            
            // draw the strings
            font(fnt);
            font_color(CLR_WHITE,CLR_WHITE);
            
            // draw the text
            // NOTE: depends closely on dims finding code above
            for( i = 0; i < c->candidatesSize; ++i ) 
            {
                UIBox *dp = &relPosTxts[i];
                F32 x_txt = dims.x + dp->x;

                // if vertical, just give each one enough room to fit itself
                // for horizontal, make sure all draw from the buttom of the box
                F32 dyAdj = verticalLayout ? dp->height : dims.height - PIX3*sc;
                F32 y_txt = dims.y + dp->y + dyAdj;
                
                // render text
                // ab: using %s to prevent translation
                prnt( x_txt, y_txt, z_txt, sc, sc, "%s", c->pszCandidates[i] );
                
                // draw the selection
                if( i == c->selection )
                {
                    UIBox tmp = *dp;
                    tmp.x = dims.x;
                    tmp.y = y_txt - dp->height;

                    // if vertical, use the box width so it fits snugly
                    // else, just the width of the indicator itself
                    tmp.width = verticalLayout ? dims.width : dp->width;

                    uiDrawBox(&tmp, z_cur, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
                }
            }
        }
    }
}

void uiIME_RenderOnUIEdit(UIEdit *edit)
{
    if( verify( edit ) && s_IMEState.pUIEdit == edit )
    {
        int borderColor = CLR_NORMAL_FOREGROUND;
        int backColor = CLR_NORMAL_BACKGROUND;
        F32 sc = edit->textScale;
        F32 x = edit->cursor.xDrawn + PIX3*sc;
        F32 y = edit->boundsText.y;
        F32 z = edit->z + 20;
        TTDrawContext *fnt = edit->font;
        UIBox dimsParent = {0};
        int dx, dy;

        // get the parent window for in-bounding
        windowClientSize(&dx, &dy);
        dimsParent.width = dx;
        dimsParent.height = dy;

        // regardless of current clip, need unbounded clipper.
        clipperPush(NULL);
        {
            //s_renderComposition( fnt, x, y, z, sc );
            s_renderCandidates( fnt, &dimsParent, x, y, z, sc, borderColor, backColor);
            uiIMEIndicatorRender( z, sc, borderColor, backColor );
        }
        clipperPop();
    }
}


void uiIME_OnLoseFocus(UIEdit *edit)
{
    HIMC hImc = ImmGetContext(hwnd);

    // if handle is null, then IME is not enabled
    if( hImc )
    {
        // testing: lost focus from an edit that never gained focus
        verify( edit == s_IMEState.pUIEdit );

        // not sure on the details here...
        s_IMEState.candidate.visible = kCandidateWindowVisibility_None;
        s_IMEState.composition.visible = false;

        uiimecomposition_Reset(&s_IMEState.composition);
        uiimecandidate_Reset(&s_IMEState.candidate);

        if( verify( s_IMEState.pUIEdit ))
        {
            uiEditSetUnderlineStateCurLine( s_IMEState.pUIEdit, false, 0, 0 );
            uiEditSetLeftIndent( s_IMEState.pUIEdit, 0.f );
        }

        s_IMEState.pUIEdit = NULL;

        // close the IME
        s_IMEState.hImc = ImmAssociateContext( hwnd, NULL );
    }
}

void uiIME_OnFocus(UIEdit *edit)
{
    HIMC hImc = s_IMEState.hImc;

    if( !hImc )
    {
        hImc = ImmGetContext(hwnd);
    }

    if( hImc && verify( edit ))
    {
        // if IME input is allowed, associate the context
        if( !edit->nonAsciiBlocked )
        {
            // open the IME
            s_IMEState.pUIEdit = edit;
            s_IMEState.composition.lenStr = 0;

            // tell indicator to re-calc its dims
            s_IMEState.input.indicatorDims.usWd = 0.f; 

            ImmAssociateContext( hwnd, hImc );
        }
        else if( !s_IMEState.hImc )
        {
            // prevent IME
            s_IMEState.hImc = ImmAssociateContext( hwnd, NULL );
        }
    }
}

