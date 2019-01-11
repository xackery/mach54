| Main Assist logic
#include mach54/config.inc

Sub MainAssistInit
    /declare MainAssist string outer
	|---Internal Variables---
	/declare MainTargetID int outer 0
/return

Sub MainLoad
    /call ConfigRead General MainAssist
    /varset MainAssist ${Macro.Return}    
/return

Sub MainLoop
    /if (${Me.Name.NotEqual[${MainAssist}]}) /return

    |Disengage Logic
	/if (${State.Equal[Combat]}) {
		/if (${Spawn[${MainTargetID}].Type.Equal[NPC]}) /return
        /bc State Idle
		/return
	}

	/if (${State.Equal[Combat]}) /return
	/if (${Target.Type.NotEqual[NPC]}) /return
	/if (${Target.PctHPs} >= 100) /return
	/if (${Target.Distance} >= 20) /return
	/if (!${Me.Combat}) /return
    /bc State Combat ${Target.ID}
/return