#include "FrmwkPlugin.h"

#define PLUGIN_VERSION     "1.0.0.1"
#define MIME_TYPES_HANDLED  "application/x-frmwk-demo"
#define PLUGIN_NAME         "Test Plug-in"
#define MIME_TYPES_DESCRIPTION  MIME_TYPES_HANDLED"::"PLUGIN_NAME
#define PLUGIN_DESCRIPTION  PLUGIN_NAME " (demo plugin for our own NPAPI framework)" 

#ifdef XP_UNIX
char* nsPluginInstanceBase::GetPluginVersion()
{
	return PLUGIN_VERSION;
}

NP_EXPORT(NPError) nsPluginInstanceBase::GetValue(NPPVariable aVariable, void *aValue)
{
	switch (aVariable) {
	case NPPVpluginNameString:
		*((char**)aValue) = PLUGIN_NAME;
		break;
	case NPPVpluginDescriptionString:
		*((char**)aValue) = PLUGIN_DESCRIPTION;
		break;
	default:
		return NPERR_INVALID_PARAM;
		break;
	}
	return NPERR_NO_ERROR;
}
#endif

#if defined(XP_UNIX)
NP_EXPORT(const char*) nsPluginInstanceBase::GetMIMEDescription()
#elif defined(XP_WIN) || defined(XP_OS2)
const char* nsPluginInstanceBase::GetMIMEDescription()
#endif
{
	return MIME_TYPES_HANDLED;
}

NPError nsPluginInstanceBase::PluginInitialize()
{
	return NPERR_NO_ERROR;
}

void nsPluginInstanceBase::PluginShutdown()
{
}

nsPluginInstanceBase * nsPluginInstanceBase::NewPluginInstance(nsPluginCreateData * aCreateDataStruct)
{
	if(!aCreateDataStruct)
		return NULL;

	CFrmwkPlugin * plugin = new CFrmwkPlugin(aCreateDataStruct->instance);
	return plugin;
}

void nsPluginInstanceBase::DestroyPluginInstance(nsPluginInstanceBase * aPlugin)
{
	if(aPlugin)
		delete (CFrmwkPlugin *)aPlugin;
}


CFrmwkPlugin::CFrmwkPlugin(NPP npp):nsPluginInstanceBase(),
	m_pNPInstance(npp),
	m_bInitialized(FALSE)
{
#ifdef ENABLE_SCRIPT_OBJECT
	m_pScriptableObject=NULL;
	CScriptPluginObject::bar_id = NPN_GetStringIdentifier("bar");
	CScriptPluginObject::func_id = NPN_GetStringIdentifier("func");
	NPN_GetValue(m_pNPInstance, NPNVWindowNPObject, &(CScriptPluginObject::sWindowObj));
#endif
}

NPBool CFrmwkPlugin::init(NPWindow* pNPWindow)
{
	m_bInitialized = TRUE;
	return TRUE;
}

void CFrmwkPlugin::shut()
{
	m_bInitialized = FALSE;
}

NPBool CFrmwkPlugin::isInitialized()
{
	return m_bInitialized;
}

CFrmwkPlugin::~CFrmwkPlugin()
{
#ifdef ENABLE_SCRIPT_OBJECT
	if (m_pScriptableObject)
		NPN_ReleaseObject(m_pScriptableObject);
#endif
}


#ifdef ENABLE_SCRIPT_OBJECT
NPObject *nsScriptObjectBase::AllocateScriptPluginObject(NPP npp, NPClass *aClass)
{
	return (NPObject*)new CScriptPluginObject(npp);
}

NPObject* CFrmwkPlugin::GetScriptableObject()
{
	if (!m_pScriptableObject) {
		m_pScriptableObject = NPN_CreateObject(m_pNPInstance,	&CScriptPluginObject::nsScriptObjectClass);
	}

	if (m_pScriptableObject) {
		NPN_RetainObject(m_pScriptableObject);
	}

	return m_pScriptableObject;
}

NPIdentifier CScriptPluginObject::bar_id;
NPIdentifier CScriptPluginObject::func_id;
NPObject * CScriptPluginObject::sWindowObj;

bool CScriptPluginObject::HasMethod(NPIdentifier name)
{
	return name == func_id;
}

bool CScriptPluginObject::HasProperty(NPIdentifier name)
{
	return name == bar_id;
}

bool CScriptPluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
	return NPN_GetProperty(m_npp,sWindowObj,name,result);
}

bool CScriptPluginObject::SetProperty(NPIdentifier name, const NPVariant *value)
{
	return NPN_SetProperty(m_npp,sWindowObj,name,value);
}

bool CScriptPluginObject::Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	if (name==func_id)
	{
		MessageBox(NULL,_T("func"),_T(""),0);
	}
	return true;
}

bool CScriptPluginObject::InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return true;
}
#endif
