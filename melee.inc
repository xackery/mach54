Sub MeleeInit
	|---Settings Variables---
	/declare IsMeleeAllowed bool outer FALSE
	/declare MeleeEngageDistance int outer 50
	/declare MeleeTargetPctHPs int outer 98
    |--Interal Variables---
	/declare MeleeRetryTimer timer outer
/return

Sub MeleeLoad
    /call ConfigRead Melee IsMeleeAllowed
    /varset IsMeleeAllowed ${Macro.Return}
	/call ConfigRead Melee MeleeEngageDistance
    /varset MeleeEngageDistance ${Macro.Return}
	/call ConfigRead Melee MeleeTargetPctHPs
    /varset MeleeTargetPctHPs ${Macro.Return}
/return

Sub MeleeLoop
	/if (!${Bool[${Spawn[pc ${MainAssist}]}]}) /return
	/if (${State.NotEqual[Combat]}) /return
	/if (${MeleeRetryTimer}>0) /return
	/varset MeleeRetryTimer 12s


	/if (${Spawn[${MainTargetID}].Type.NotEqual[NPC]}) /return
	/if (!${IsMeleeAllowed}) /return
	/if (${IsSitPriorityOverMelee} && ${Me.Sitting} && ${Me.PctMana}<${SitMaxManaPct}) /return

	/if (${Me.Name.Equal[${MainPuller}]}) /return
	/if (${Stick.Active} && ${State.NotEqual[Combat]} && ${State.NotEqual[Follow]}) {
		/stick off
		/return
	}
	
	/if (${Me.Name.NotEqual[${MainAssist}]} && ${Spawn[${MainTargetID}].PctHPs} > ${MeleePct}) /return


	/if (!${Cast.Ready}) /return

	/if (${Spawn[$MainTargetID].Distance} > ${MeleeEngageDistance}) /return
	/if (${Target.ID} != ${MainTargetID}) /target id ${MainTargetID}
	/if (!${Me.Combat}) /attack on
	/if (!${Stick.Active} && ${Me.Name.Equal[${MainAssist}]}) {
		/stick 10 id ${MainTargetID}
		/return
	}
	/if (!${Stick.Active}) /stick 10 behind id ${MainTargetID}
/return
