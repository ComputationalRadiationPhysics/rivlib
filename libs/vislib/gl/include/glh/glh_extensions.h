/* Comments:

   The trick with GLH_EXT_SINGLE_FILE is that you need to have it defined in
   exactly one cpp file because it piggy-backs the function implementations in
   the header.  You don't want multiple implementations defined or the linker
   gets mad, but one file must have the implementations in it or the linker
   gets mad for different reasons.
   
     The goal was to avoid having this helper require a separate cpp file.  One
   thing you could do is just have a glh_extensions.cpp that did
   
   #define GLH_EXT_SINGLE_FILE
   #include <glh_extensions.h>
  
   and make it the only file that ever defines GLH_EXT_SINGLE_FILE.
*/

#ifndef GLH_EXTENSIONS
#define GLH_EXTENSIONS

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
# include <windows.h>
#endif
/*
#ifdef MACOS
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#undef GL_VERSION_1_2
#undef GL_VERSION_1_3

#undef __glext_h_
*/

#define CHECK_MEMORY(ptr) \
    if (NULL == ptr) { \
        printf("Error allocating memory in file %s, line %d\n", __FILE__, __LINE__); \
        exit(-1); \
    }

#ifdef GLH_EXT_SINGLE_FILE
#  define GLH_EXTENSIONS_SINGLE_FILE  /* have to do this because glh_genext.h unsets GLH_EXT_SINGLE_FILE */
#endif

#if (defined(WIN32) || defined(UNIX))
#include "glh_genext.h"
#elif defined(MACOS)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include "GL/glext.h"
#endif
#ifdef WIN32
# include <GL/GL.h>
# include <GL/wglext.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GLH_EXTENSIONS_SINGLE_FILE
static char *unsupportedExts = NULL;
static char *sysExts = NULL;
#ifndef GL_SHADER_CONSISTENT_NV
#define GL_SHADER_CONSISTENT_NV           0x86DD
#endif
#ifndef GL_TEXTURE_SHADER_NV
#define GL_TEXTURE_SHADER_NV              0x86DE
#endif
#ifndef GL_SHADER_OPERATION_NV
#define GL_SHADER_OPERATION_NV            0x86DF
#endif


static int parseVersionString(const char *str, 
                              long &major, long &minor, long &rev, 
                              char delimiter)
{
    major = minor = rev = 0;

    if (!str)
        return GL_FALSE;

    char *p = NULL;
    major = strtol(str, &p, 10);
    if (p == str)
        return GL_FALSE; // no digits found, strange...
    if ((p[0] == delimiter) && (strlen(p) > 1)) // go for minor version
    {
        minor = strtol(&p[1], &p, 10);
        if ((p[0] == delimiter) && (strlen(p) > 1)) // go for revision
        {
            rev = strtol(&p[1], &p, 10);
        }
    }
    return GL_TRUE;
}


static int ExtensionExists(const char* extName, const char* sysExts)
{
    // circumvent memory leak by first checking for OpenGL version 
    // and then allocating

    // version check
    if (strncmp(extName, "GL_VERSION_", 11) == 0)
    {
        long int majorGL, minorGL, revGL;
        long int majorRequest, minorRequest, revRequest;
        const char *versionGL = (const char*)glGetString(GL_VERSION);

        if (!parseVersionString(versionGL, majorGL, minorGL, revGL, '.'))
            return GL_FALSE;
        if (!parseVersionString(&extName[11], 
                                majorRequest, minorRequest, revRequest, '_'))
            return GL_FALSE;

        if ((majorRequest < majorGL)
            || ((majorRequest == majorGL) && (minorRequest < minorGL))
            || ((majorRequest == majorGL) && (minorRequest == minorGL) 
                && (revRequest <= revGL)))
            return GL_TRUE;
        else
            return GL_FALSE;
    }


#if (_MSC_VER >= 1400)
    size_t padExtNameLen = strlen(extName) + 2;
    char *padExtName = (char*)malloc(padExtNameLen);
    strcpy_s(padExtName, padExtNameLen, extName);
    strcat_s(padExtName, padExtNameLen, " ");
#else
    char *padExtName = (char*)malloc(strlen(extName) + 2);
    strcat(strcpy(padExtName, extName), " ");
#endif
    if (strstr(sysExts, padExtName)) {
        free(padExtName);
        return GL_TRUE;
    } else {
        free(padExtName);
        return GL_FALSE;
    }
}

static const char* EatWhiteSpace(const char *str)
{
    for (; *str && (' ' == *str || '\t' == *str || '\n' == *str); str++);
    return str;
}

static const char* EatNonWhiteSpace(const char *str)
{
    for (; *str && (' ' != *str && '\t' != *str && '\n' != *str); str++);
    return str;
}

#if !defined(WIN32) && !defined(__USE_SVID) && !defined(__USE_BSD) && !defined(__USE_XOPEN_EXTENDED)
static char *strdup(const char *s)
{
	char *ret;

	if (!s) {
		return NULL;
	}

	if (! (ret = (char *)malloc(strlen(s) + 1))) {
		fprintf(stderr, "not enough memory for string duplicate\n");
		exit(1);
	}

	strcpy(ret, s);

	return ret;
}
#endif

GLH_EXTERN int glh_init_extensions(const char *origReqExts)
{
    /* Length of requested extensions string */
    size_t reqExtsLen;
    char *reqExts;
    /* Ptr for individual extensions within reqExts */
    char *reqExt;
    int success = GL_TRUE;
    /* build space-padded extension string */
    if (NULL == sysExts) {
        const char *extensions = (const char*)glGetString(GL_EXTENSIONS);
        size_t sysExtsLen = strlen(extensions);
        const char *winsys_extensions = "";     
        size_t winsysExtsLen = 0;
#if defined(WIN32)
        {
            PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = 0;
            wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
            if(wglGetExtensionsStringARB)
            {
                winsys_extensions = wglGetExtensionsStringARB(wglGetCurrentDC());
                winsysExtsLen = strlen(winsys_extensions);
            }
        }
#elif defined(UNIX)
        {

          winsys_extensions = glXQueryExtensionsString (glXGetCurrentDisplay(),DefaultScreen(glXGetCurrentDisplay())) ;
          winsysExtsLen = strlen (winsys_extensions);
        }
#endif
        /* Add 2 bytes, one for padding space, one for terminating NULL */
#if (_MSC_VER >= 1400)
        size_t sysExtsAllLen = sysExtsLen + winsysExtsLen + 3;
#endif
        sysExts = (char*)malloc(sysExtsLen + winsysExtsLen + 3);
        CHECK_MEMORY(sysExts);
#if (_MSC_VER >= 1400)
        strcpy_s(sysExts, sysExtsAllLen, extensions);
#else
        strcpy(sysExts, extensions);
#endif
        sysExts[sysExtsLen] = ' ';
        sysExts[sysExtsLen + 1] = 0;
#if (_MSC_VER >= 1400)
        strcat_s(sysExts, sysExtsAllLen, winsys_extensions);
#else
        strcat(sysExts, winsys_extensions);
#endif
        sysExts[sysExtsLen + 1 + winsysExtsLen] = ' ';
        sysExts[sysExtsLen + 1 + winsysExtsLen + 1] = 0;
    }

    if (NULL == origReqExts)
        return GL_TRUE;

#if (_MSC_VER >= 1400)
    reqExts = _strdup(origReqExts);
#else /* (_MSC_VER >= 1400) */
    reqExts = strdup(origReqExts);
#endif /* (_MSC_VER >= 1400) */ 
    reqExtsLen = strlen(reqExts);

#if (_MSC_VER >= 1400)
    size_t unsupportedExtsLen = 0;
#endif
    if (NULL == unsupportedExts) {
#if (_MSC_VER >= 1400)
        unsupportedExtsLen = reqExtsLen + 2;
#endif
        unsupportedExts = (char*)malloc(reqExtsLen + 2);
    } else if (reqExtsLen > strlen(unsupportedExts)) {
#if (_MSC_VER >= 1400)
        unsupportedExtsLen = reqExtsLen + 2;
#endif
        unsupportedExts = (char*)realloc(unsupportedExts, reqExtsLen + 2);
    }
    CHECK_MEMORY(unsupportedExts);
    *unsupportedExts = 0;

    /* Parse requested extension list */
    for (reqExt = reqExts;
        (reqExt = (char*)EatWhiteSpace(reqExt)) && *reqExt;
        reqExt = (char*)EatNonWhiteSpace(reqExt))
    {
        char *extEnd = (char*)EatNonWhiteSpace(reqExt);
        char saveChar = *extEnd;
        *extEnd = (char)0;

#if (defined(WIN32) || defined(UNIX))
        if (!ExtensionExists(reqExt, sysExts) || !glh_init_extension(reqExt)) {
#elif defined(MACOS)
        if (!ExtensionExists(reqExt, sysExts)) {    /* don't try to get function pointers if on MacOS */
#endif
            /* add reqExt to end of unsupportedExts */
#if (_MSC_VER >= 1400)
            strcat_s(unsupportedExts, unsupportedExtsLen, reqExt);
            strcat_s(unsupportedExts, unsupportedExtsLen, " ");
#else
            strcat(unsupportedExts, reqExt);
            strcat(unsupportedExts, " ");
#endif
            success = GL_FALSE;
        }
        *extEnd = saveChar;
    }
    free(reqExts);
    return success;
}

GLH_EXTERN const char* glh_get_unsupported_extensions()
{
    return (const char*)unsupportedExts;
}

GLH_EXTERN void glh_shutdown_extensions()
{
    if (unsupportedExts)
    {
        free(unsupportedExts);
        unsupportedExts = NULL;
    }
    if (sysExts)
    {
        free(sysExts);
        sysExts = NULL;
    }
}

GLH_EXTERN int glh_extension_supported(const char *extension)
{
    static const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;
    
    /* Extension names should not have spaces. */
    where = (GLubyte *) strchr(extension, ' ');
    if (where || *extension == '\0')
      return 0;
    
    if (!extensions)
      extensions = glGetString(GL_EXTENSIONS);

    /* It takes a bit of care to be fool-proof about parsing the
       OpenGL extensions string.  Don't be fooled by sub-strings,
       etc.
    */
    start = extensions;
    for (;;) 
    {
        where = (GLubyte *) strstr((const char *) start, extension);
        if (!where)
            break;
        terminator = where + strlen(extension);
        if (where == start || *(where - 1) == ' ') 
        {
            if (*terminator == ' ' || *terminator == '\0') 
            {
                return 1;
            }
        }
        start = terminator;
    }
    return 0;
}

#else
int glh_init_extensions(const char *origReqExts);
const char* glh_get_unsupported_extensions();
void glh_shutdown_extensions();
int glh_extension_supported(const char *extension);
#endif /* GLH_EXT_SINGLE_FILE */

#ifdef __cplusplus
}
#endif

#endif /* GLH_EXTENSIONS */
