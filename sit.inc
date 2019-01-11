Sub SitInit
	| How much mana should I start sitting for
	/declare SitManaPct int outer 50
	| If I am under this amount and sitting is priority, I won't melee/nuke
	/declare SitMaxManaPct int outer 80
	|When state is idle, default to sitting?
	/declare IsSitDefaultIdle bool outer FALSE
	|Should I sit during combat even if I normally melee?
	/declare IsSitPriorityOverMelee bool outer FALSE
	|Should I sit during combat even if I normally nuke?
	/declare IsSitPriorityOverNuking bool outer FALSE
	|cooldown before I try sitting again (If I get hit, wait this long to try again)
	/declare SitRetryCooldown int outer 5	
    |---Internal Variables---
    /declare SitRetryTimer timer outer
/return


Sub SitLoad
| How much mana should I start sitting for
	/call ConfigRead Sit SitManaPct
    /varset SitManaPct ${Macro.Return}
	/call ConfigRead Sit SitMaxManaPct
    /varset SitMaxManaPct ${Macro.Return}
	/call ConfigRead Sit IsSitDefaultIdle
    /varset IsSitDefaultIdle ${Macro.Return}
	/call ConfigRead Sit IsSitPriorityOverMelee
    /varset IsSitPriorityOverMelee ${Macro.Return}
	/call ConfigRead Sit IsSitPriorityOverNuking
    /varset IsSitPriorityOverNuking ${Macro.Return}
    /call ConfigRead Sit SitRetryCooldown
    /varset SitRetryCooldown ${Macro.Return}
/return

Sub SitLoop		
	/if (${Me.PctMana} == 100) {
		/if (${LastState.NotEqual[Idle]} && ${State.Equal[Idle]} && ${Me.Sitting}) /sit off
		/return
	}
	/if (${Me.Sitting}) /return
	
	|Reset timer if we just changed states, we may have killed the mob hitting us
	/if (${LastState.Equal[Combat]} && ${State.Equal[Idle]}) /varset SitRetryTimer 0
	/if (${SitRetryTimer}!=0) /return

	/if (${IsSitPriorityOverMelee} && ${State.Equal[Combat]} && ${Me.PctMana}<=${SitManaPct}) {
		/echo Sitting Down During Combat
		/sit on
		/varset SitRetryTimer ${SitRetryCooldown}s
		/return
	}
	/if (${State.NotEqual[Idle]}) /return
	/if (!${IsSitDefaultIdle}) /return	
	/if (${Me.PctMana}==100) /return
	/sit on
	/varset SitRetryTimer ${SitRetryCooldown}s
/return

#event Damage "#*#YOU for#*#points of damage."

Sub Event_Damage
	/varset SitRetryTimer 12s
/return
