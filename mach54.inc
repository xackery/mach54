
#include mach54/cure.inc
#include mach54/dead.inc
#include mach54/heal.inc
#include mach54/load.inc
#include mach54/loot.inc
#include mach54/main.inc
#include mach54/melee.inc
#include mach54/nuke.inc
#include mach54/config.inc
#include mach54/sit.inc
#include mach54/slow.inc


Sub MainInit
    |---Internal Variables---
	/declare State string outer Idle
	/declare LastState string outer
	
    /call ConfigInit
	/call CureInit
	/call HealInit    
    /call LoadInit
    /call LootInit
	/call MeleeInit    
	/call NukeInit
	/call SettingsInit
	/call SitInit
	/call SlowInit
/return

Sub MainLoad
    /call ConfigLoad
	/call CureLoad
	/call HealLoad
    /call LootLoad
	/call MeleeLoad    
	/call NukeLoad
	/call SettingsLoad
	/call SitLoad
	/call SlowLoad
/return

#event State "[#1#] State #2# #3#"
#event State "<#1#> State #2# #3#"

Sub Event_State(string line, string sender, string state, string stateArg1)
    | Ignore messages if they aren't from assist or puller
    /if (!${sender.Equal[${MainAssist}]} && !${sender.Equal[${MainPuller}]}) /return
    | Ignore messages if the sender isn't in same zone
    /if (!${Spawn[pc ${sender}]}) /return

    /varset State ${state}
    /if (${State.Equal[Combat]) /varset MainTargetID stateArg1
    /if (${State.Equal[Follow]}) {
        /if (${stateArg1.Equal[]}) /stick id ${Spawn[pc ${sender}].ID}
        /if (${stateArg1.NotEqual[]}) /stick id ${Spawn[pc ${stateArg1}].ID}
    }
/return

Sub MainLoop
:Loop
	/doevents
	/varset LastState ${State}
	|Order is based on priority

    /call ConfigLoop
	/call HealLoop
	/call AssistLoop
	/call SlowLoop
	/call MeleeLoop	
	/call NukeLoop
	/call CureLoop
	/call SitLoop
	/delay 1s
	/goto :Loop
/return