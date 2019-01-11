#include mach54/heal.inc
#include mach54/loot.inc
#include mach54/main.inc
#include mach54/melee.inc
#include mach54/nuke.inc
#include mach54/config.inc
#include mach54/sit.inc


Sub MachInit
    |---Internal Variables---
	
    /call ConfigInit
	/call HealInit
    /call LootInit
	/call MainInit
	/call MeleeInit    
	/call NukeInit
	/call SitInit
/return

Sub MachLoad
    /call ConfigLoad
	/call HealLoad
    /call LootLoad
	/call MeleeLoad  
	/call MainLoad
	/call NukeLoad
	/call SitLoad
/return

#event State "[#1#] State #2#"
#event State "<#1#> State #2#"
#event StateArg "[#1#] State #2# #3#"
#event StateArg "<#1#> State #2# #3#"

Sub Event_State(string line, string sender, string state)
	/if (${state.Find[ ]}) /return
	| Ignore messages if they aren't from assist or puller
    /if (!${sender.Equal[${MainAssist}]} && !${sender.Equal[${MainPuller}]}) /return
    | Ignore messages if the sender isn't in same zone
	/if (!${Bool[${Spawn[pc ${sender}]}]}) /return
    /varset State ${state}
    /if (${State.Equal[Follow]}) {
        /if (!${stateArg1} && ${sender.NotEqual[${Me.Name}]}) /stick id ${Spawn[pc ${sender}].ID}
    }
/return

Sub Event_StateArg(string line, string sender, string state, string stateArg1)
	| Ignore messages if they aren't from assist or puller
    /if (!${sender.Equal[${MainAssist}]} && !${sender.Equal[${MainPuller}]}) /return
    | Ignore messages if the sender isn't in same zone
	/if (!${Bool[${Spawn[pc ${sender}]}]}) /return
    /varset State ${state}
    /if (${State.Equal[Combat]}) {
		|/varset MainTargetID ${Int[${stateArg1}]}
		/varset MainTargetID ${stateArg1}		
		/if (${Spawn[${MainTargetID}].Distance} > 200) /return
		/target id ${MainTargetID}
		/if (${MainAssist.Equal[${Me.Name}]}) /killthis
	}
    /if (${State.Equal[Follow]}) {
        /if (${stateArg1} && ${stateArg1.NotEqual[${Me.Name}]}) /stick id ${Spawn[pc ${stateArg1}].ID}
    }
/return

Sub MachLoop
	|Order is based on priority
    /call ConfigLoop
	/call HealLoop
	/call LootLoop
	/call MainLoop
	/call MeleeLoop	
	/call NukeLoop
	/call SitLoop
/return