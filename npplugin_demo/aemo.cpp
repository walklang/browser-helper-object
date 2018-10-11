#include "aemo.h"

using namespace std;

#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))
#define STATIC_ASSERT(condition)                                \
    extern void np_static_assert(int arg[(condition) ? 1 : -1])

//
// static data
//

static NPNetscapeFuncs* sBrowserFuncs = NULL;

//
// npapi plugin functions
//

#ifdef XP_UNIX
NP_EXPORT(char*)
NP_GetPluginVersion()
{
  return sPluginVersion;
}
#endif

static char sMimeDescription[] = "application/x-test:tst:Test mimetype";

#if defined(XP_UNIX)
NP_EXPORT(const char*) NP_GetMIMEDescription()
#elif defined(XP_WIN) || defined(XP_OS2)
const char* NP_GetMIMEDescription()
#endif
{
  return sMimeDescription;
}

#ifdef XP_UNIX
NP_EXPORT(NPError)
NP_GetValue(void* future, NPPVariable aVariable, void* aValue) {
  switch (aVariable) {
    case NPPVpluginNameString:
      *((char**)aValue) = sPluginName;
      break;
    case NPPVpluginDescriptionString:
      *((char**)aValue) = sPluginDescription;
      break;
    default:
      return NPERR_INVALID_PARAM;
      break;
  }
  return NPERR_NO_ERROR;
}
#endif

static bool fillPluginFunctionTable(NPPluginFuncs* pFuncs)
{
  // Check the size of the provided structure based on the offset of the
  // last member we need.
  if (pFuncs->size < (offsetof(NPPluginFuncs, getsiteswithdata) + sizeof(void*)))
    return false;

  pFuncs->newp = NPP_New;
  pFuncs->destroy = NPP_Destroy;
  pFuncs->setwindow = NPP_SetWindow;
  pFuncs->newstream = NPP_NewStream;
  pFuncs->destroystream = NPP_DestroyStream;
  pFuncs->asfile = NPP_StreamAsFile;
  pFuncs->writeready = NPP_WriteReady;
  pFuncs->write = NPP_Write;
  pFuncs->print = NPP_Print;
  pFuncs->event = NPP_HandleEvent;
  pFuncs->urlnotify = NPP_URLNotify;
  pFuncs->getvalue = NPP_GetValue;
  pFuncs->setvalue = NPP_SetValue;
  pFuncs->urlredirectnotify = NPP_URLRedirectNotify;
  pFuncs->clearsitedata = NPP_ClearSiteData;
  pFuncs->getsiteswithdata = NPP_GetSitesWithData;

  return true;
}

#if defined(XP_MACOSX)
NP_EXPORT(NPError) NP_Initialize(NPNetscapeFuncs* bFuncs)
#elif defined(XP_WIN) || defined(XP_OS2)
NPError OSCALL NP_Initialize(NPNetscapeFuncs* bFuncs)
#elif defined(XP_UNIX)
NP_EXPORT(NPError) NP_Initialize(NPNetscapeFuncs* bFuncs, NPPluginFuncs* pFuncs)
#endif
{
  sBrowserFuncs = bFuncs;

#if defined(XP_UNIX) && !defined(XP_MACOSX)
  if (!fillPluginFunctionTable(pFuncs)) {
    return NPERR_INVALID_FUNCTABLE_ERROR;
  }
#endif

  return NPERR_NO_ERROR;
}

#if defined(XP_MACOSX)
NP_EXPORT(NPError) NP_GetEntryPoints(NPPluginFuncs* pFuncs)
#elif defined(XP_WIN) || defined(XP_OS2)
NPError OSCALL NP_GetEntryPoints(NPPluginFuncs* pFuncs)
#endif
#if defined(XP_MACOSX) || defined(XP_WIN) || defined(XP_OS2)
{
  if (!fillPluginFunctionTable(pFuncs)) {
    return NPERR_INVALID_FUNCTABLE_ERROR;
  }

  return NPERR_NO_ERROR;
}
#endif

#if defined(XP_UNIX)
NP_EXPORT(NPError) NP_Shutdown()
#elif defined(XP_WIN) || defined(XP_OS2)
NPError OSCALL NP_Shutdown()
#endif
{
  return NPERR_NO_ERROR;
}

NPError
NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char* argn[], char* argv[], NPSavedData* saved)
{
	return NPERR_NO_ERROR;
}

NPError
NPP_Destroy(NPP instance, NPSavedData** save)
{
	return NPERR_NO_ERROR;
}

NPError
NPP_SetWindow(NPP instance, NPWindow* window)
{
	return NPERR_NO_ERROR;
}

NPError
NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype)
{
	return NPERR_NO_ERROR;
}

NPError
NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason)
{
	return NPERR_NO_ERROR;
}

int32_t
NPP_WriteReady(NPP instance, NPStream* stream)
{
	return 0x0fffffff;
}

int32_t
NPP_Write(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer)
{
	return len;
}

void
NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname)
{

}

void
NPP_Print(NPP instance, NPPrint* platformPrint)
{
}

int16_t
NPP_HandleEvent(NPP instance, void* event)
{
	return 0;
}

void
NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData)
{

}

NPError
NPP_GetValue(NPP instance, NPPVariable variable, void* value)
{
	return NPERR_GENERIC_ERROR;
}

NPError
NPP_SetValue(NPP instance, NPNVariable variable, void* value)
{
	return NPERR_GENERIC_ERROR;
}

void
NPP_URLRedirectNotify(NPP instance, const char* url, int32_t status, void* notifyData)
{

}

NPError
NPP_ClearSiteData(const char* site, uint64_t flags, uint64_t maxAge)
{
	return NPERR_NO_ERROR;
}

char**
NPP_GetSitesWithData()
{
  return NULL;
}

//
// npapi browser functions
//

bool
NPN_SetProperty(NPP instance, NPObject* obj, NPIdentifier propertyName, const NPVariant* value)
{
  return sBrowserFuncs->setproperty(instance, obj, propertyName, value);
}

NPIdentifier
NPN_GetIntIdentifier(int32_t intid)
{
  return sBrowserFuncs->getintidentifier(intid);
}

NPIdentifier
NPN_GetStringIdentifier(const NPUTF8* name)
{
  return sBrowserFuncs->getstringidentifier(name);
}

void
NPN_GetStringIdentifiers(const NPUTF8 **names, int32_t nameCount, NPIdentifier *identifiers)
{
  return sBrowserFuncs->getstringidentifiers(names, nameCount, identifiers);
}

bool
NPN_IdentifierIsString(NPIdentifier identifier)
{
  return sBrowserFuncs->identifierisstring(identifier);
}

NPUTF8*
NPN_UTF8FromIdentifier(NPIdentifier identifier)
{
  return sBrowserFuncs->utf8fromidentifier(identifier);
}

int32_t
NPN_IntFromIdentifier(NPIdentifier identifier)
{
  return sBrowserFuncs->intfromidentifier(identifier);
}

NPError
NPN_GetValue(NPP instance, NPNVariable variable, void* value)
{
  return sBrowserFuncs->getvalue(instance, variable, value);
}

NPError
NPN_SetValue(NPP instance, NPPVariable variable, void* value)
{
  return sBrowserFuncs->setvalue(instance, variable, value);
}

void
NPN_InvalidateRect(NPP instance, NPRect* rect)
{
  sBrowserFuncs->invalidaterect(instance, rect);
}

bool
NPN_HasProperty(NPP instance, NPObject* obj, NPIdentifier propertyName)
{
  return sBrowserFuncs->hasproperty(instance, obj, propertyName);
}

NPObject*
NPN_CreateObject(NPP instance, NPClass* aClass)
{
  return sBrowserFuncs->createobject(instance, aClass);
}

bool
NPN_Invoke(NPP npp, NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
  return sBrowserFuncs->invoke(npp, obj, methodName, args, argCount, result);
}

bool
NPN_InvokeDefault(NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
  return sBrowserFuncs->invokeDefault(npp, obj, args, argCount, result);
}

bool
NPN_Construct(NPP npp, NPObject* npobj, const NPVariant* args,
	      uint32_t argCount, NPVariant* result)
{
  return sBrowserFuncs->construct(npp, npobj, args, argCount, result);
}

const char*
NPN_UserAgent(NPP instance)
{
  return sBrowserFuncs->uagent(instance);
}

NPObject*
NPN_RetainObject(NPObject* obj)
{
  return sBrowserFuncs->retainobject(obj);
}

void
NPN_ReleaseObject(NPObject* obj)
{
  return sBrowserFuncs->releaseobject(obj);
}

void*
NPN_MemAlloc(uint32_t size)
{
  return sBrowserFuncs->memalloc(size);
}

char*
NPN_StrDup(const char* str)
{
  return strcpy((char*)sBrowserFuncs->memalloc(strlen(str) + 1), str);
}

void
NPN_MemFree(void* ptr)
{
  return sBrowserFuncs->memfree(ptr);
}

uint32_t
NPN_ScheduleTimer(NPP instance, uint32_t interval, NPBool repeat, void (*timerFunc)(NPP npp, uint32_t timerID))
{
  return sBrowserFuncs->scheduletimer(instance, interval, repeat, timerFunc);
}

void
NPN_UnscheduleTimer(NPP instance, uint32_t timerID)
{
  return sBrowserFuncs->unscheduletimer(instance, timerID);
}

void
NPN_ReleaseVariantValue(NPVariant *variant)
{
  return sBrowserFuncs->releasevariantvalue(variant);
}

NPError
NPN_GetURLNotify(NPP instance, const char* url, const char* target, void* notifyData)
{
  return sBrowserFuncs->geturlnotify(instance, url, target, notifyData);
}

NPError
NPN_GetURL(NPP instance, const char* url, const char* target)
{
  return sBrowserFuncs->geturl(instance, url, target);
}

NPError
NPN_RequestRead(NPStream* stream, NPByteRange* rangeList)
{
  return sBrowserFuncs->requestread(stream, rangeList);
}

NPError
NPN_PostURLNotify(NPP instance, const char* url, 
                  const char* target, uint32_t len, 
                  const char* buf, NPBool file, void* notifyData)
{
  return sBrowserFuncs->posturlnotify(instance, url, target, len, buf, file, notifyData);
}

NPError 
NPN_PostURL(NPP instance, const char *url,
                    const char *target, uint32_t len,
                    const char *buf, NPBool file)
{
  return sBrowserFuncs->posturl(instance, url, target, len, buf, file);
}

NPError
NPN_DestroyStream(NPP instance, NPStream* stream, NPError reason)
{
  return sBrowserFuncs->destroystream(instance, stream, reason);
}

NPError
NPN_NewStream(NPP instance, 
              NPMIMEType  type, 
              const char* target,
              NPStream**  stream)
{
  return sBrowserFuncs->newstream(instance, type, target, stream);
}

int32_t
NPN_Write(NPP instance,
          NPStream* stream,
          int32_t len,
          void* buf)
{
  return sBrowserFuncs->write(instance, stream, len, buf);
}

bool
NPN_Enumerate(NPP instance,
              NPObject *npobj,
              NPIdentifier **identifiers,
              uint32_t *identifierCount)
{
  return sBrowserFuncs->enumerate(instance, npobj, identifiers, 
      identifierCount);
}

bool
NPN_GetProperty(NPP instance,
                NPObject *npobj,
                NPIdentifier propertyName,
                NPVariant *result)
{
  return sBrowserFuncs->getproperty(instance, npobj, propertyName, result);
}

bool
NPN_Evaluate(NPP instance, NPObject *npobj, NPString *script, NPVariant *result)
{
  return sBrowserFuncs->evaluate(instance, npobj, script, result);
}

void
NPN_SetException(NPObject *npobj, const NPUTF8 *message)
{
  return sBrowserFuncs->setexception(npobj, message);
}

NPBool
NPN_ConvertPoint(NPP instance, double sourceX, double sourceY, NPCoordinateSpace sourceSpace, double *destX, double *destY, NPCoordinateSpace destSpace)
{
  return sBrowserFuncs->convertpoint(instance, sourceX, sourceY, sourceSpace, destX, destY, destSpace);
}

NPError
NPN_SetValueForURL(NPP instance, NPNURLVariable variable, const char *url, const char *value, uint32_t len)
{
  return sBrowserFuncs->setvalueforurl(instance, variable, url, value, len);
}

NPError
NPN_GetValueForURL(NPP instance, NPNURLVariable variable, const char *url, char **value, uint32_t *len)
{
  return sBrowserFuncs->getvalueforurl(instance, variable, url, value, len);
}

NPError
NPN_GetAuthenticationInfo(NPP instance,
                          const char *protocol,
                          const char *host, int32_t port,
                          const char *scheme,
                          const char *realm,
                          char **username, uint32_t *ulen,
                          char **password,
                          uint32_t *plen)
{
  return sBrowserFuncs->getauthenticationinfo(instance, protocol, host, port, scheme, realm,
      username, ulen, password, plen);
}

void
NPN_PluginThreadAsyncCall(NPP plugin, void (*func)(void*), void* userdata)
{
  return sBrowserFuncs->pluginthreadasynccall(plugin, func, userdata);
}

void
NPN_URLRedirectResponse(NPP instance, void* notifyData, NPBool allow)
{
  return sBrowserFuncs->urlredirectresponse(instance, notifyData, allow);
}
