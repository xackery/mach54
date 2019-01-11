| Config handles config reloading and calling

Sub ConfigInit
/return

Sub ConfigLoad
/return

Sub ConfigLoop    
/return

Sub ConfigRead(string category, string key)
	/declare value string local

	/varset value ${Ini[${Me.Name},${category},${key}]}
	/if (${value.NotEqual[]}) /return ${value}

	/varset value ${Ini[${Me.Class.ShortName},${category},${key}]}
	/if (${value.NotEqual[]}) /return ${value}

	/varset value ${Ini[default.ini,${category},${key}]}
	/if (${value.NotEqual[]}) /return ${value}
/return ${value}