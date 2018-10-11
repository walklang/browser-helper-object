#pragma once
#include "npfrmwkbase.h"
class CFrmwkPlugin :
	public nsPluginInstanceBase
{
public:
	CFrmwkPlugin(NPP pNPInstance);
	~CFrmwkPlugin();

	NPBool init(NPWindow* pNPWindow); 
	void shut();
	NPBool isInitialized();
#ifdef ENABLE_SCRIPT_OBJECT
	NPObject* m_pScriptableObject;

	//for scriptable plugins
	virtual NPObject *GetScriptableObject();
#else
	virtual NPObject* GetScriptableObject(){return NULL;}
#endif

private:
	NPP m_pNPInstance;
	NPBool m_bInitialized;

};

#ifdef ENABLE_SCRIPT_OBJECT
class CScriptPluginObject:
	public nsScriptObjectBase
{
public:
	static NPIdentifier bar_id;
	static NPIdentifier func_id;
	static NPObject *sWindowObj;

	CScriptPluginObject(NPP npp):nsScriptObjectBase(npp){}
	~CScriptPluginObject(){};

	virtual bool HasProperty(NPIdentifier name);
	virtual bool HasMethod(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result);
};
#endif
