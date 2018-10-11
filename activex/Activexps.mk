
Activexps.dll: dlldata.obj Activex_p.obj Activex_i.obj
	link /dll /out:Activexps.dll /def:Activexps.def /entry:DllMain dlldata.obj Activex_p.obj Activex_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Activexps.dll
	@del Activexps.lib
	@del Activexps.exp
	@del dlldata.obj
	@del Activex_p.obj
	@del Activex_i.obj
