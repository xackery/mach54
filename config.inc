| Config handles config reloading and calling

Sub ConfigInit
/return

Sub ConfigLoad
/return

Sub ConfigLoop
/return

Sub ConfigRead(string category, string key)
	/declare value string local
	/varset value ${Ini[mach54/${Me.Name}.ini,${category},${key}]}
	/if (${value.NotEqual[]}) /return ${value}

	/varset value ${Ini[${mach54/Me.Class.ShortName}.ini,${category},${key}]}
	/if (${value.NotEqual[]}) /return ${value}

	/varset value ${Ini[mach54/default.ini,${category},${key}]}
	/if (${value.NotEqual[]}) /return ${value}
/return ${value}