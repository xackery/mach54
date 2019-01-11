#EVENT lowmana "Insufficient Mana to cast this spell!"

SUB EVENT_lowmana
/return

Sub HealInit
	/declare EffectiveHealSpellName string outer
	/declare EffectiveHealGem int outer
	/declare EffectiveHealTargetPctHPs int outer
	/declare EffectiveHealRange int outer

	/declare EmergencyHealSpellName string outer
	/declare EmergencyHealGem int outer
	/declare EmergencyHealTargetPctHPs int outer
	/declare EmergencyHealRange int outer
/return

Sub HealLoad
	/call ConfigRead Heal EffectiveHeal
    /varset MainAssist ${Macro.Return}

	/call ConfigRead Heal EffectiveHealSpellName
	/varset EffectiveHealSpellName ${Macro.Return}
	/call ConfigRead Heal EffectiveHealGem
	/varset EffectiveHealGem ${Macro.Return}
	/call ConfigRead Heal EffectiveHealTargetPctHPs
	/varset EffectiveHealTargetPctHPs ${Macro.Return}
	/call ConfigRead Heal EffectiveHealRange
	/varset EffectiveHealRange ${Macro.Return}

	/call ConfigRead Heal EmergencyHealSpellName
	/varset EmergencyHealSpellName ${Macro.Return}
	/call ConfigRead Heal EmergencyHealGem
	/varset EmergencyHealGem ${Macro.Return}
	/call ConfigRead Heal EmergencyHealTargetPctHPs
	/varset EmergencyHealTargetPctHPs ${Macro.Return}
	/call ConfigRead Heal EmergencyHealRange
	/varset EmergencyHealRange ${Macro.Return}
/return

Sub HealLoop
	/if (${Me.PctMana} < 8) /return
	| Don't do heal logic if emergency heal isn't set
	/if (${EmergencyHealSpellName.Length}==0) /return

	|How many heal candidates are there
	/declare healCandidateCount int 0 
	|Which spell is going to be used
	/declare currentHealSpell string 
	|Who is the target for the spell
	/declare currentHealTarget string 
	|How hurt is this target
	/declare currentHealTargetPct int 100 
	|What type of heal
	/declare currentHealType string 

	|Iterator selector
	/declare selector string 
	|Iterator
	/declare i int 0 


	| Iterate group members
	/for i 0 to ${Group.Members}
		/varset selector ${Group.Member[${i}]}
		
		|Emergency Healing		
		/if (${Bool[${Spawn[pc ${selector}]}]} && ${Spawn[pc ${selector}].Distance} < ${EmergencyHealRange} && ${Spawn[pc ${selector}].PctHPs} < ${EmergencyHealPct} && ${Spawn[pc ${selector}].PctHPs} < ${currentHealTargetPct}) {
			/varcalc healCandidateCount ${healCount}+1
			/varset currentHealTarget ${selector}
			/varset currentHealSpell ${EmergencyHealSpellName}
			/varset currentHealTargetPct ${Spawn[pc ${selector}].PctHPs}
			/varset currentHealType Emergency
		}

		|Effective Healing		
		/if (${Bool[${Spawn[pc ${selector}]}]} && ${Spawn[pc ${selector}].Distance} < ${EffectiveHealRange} && ${Spawn[pc ${selector}].PctHPs} < ${EffectiveHealPct} && ${Spawn[pc ${selector}].PctHPs} < ${currentHealTargetPct}) {
			/echo Effective
			/varcalc healCandidateCount ${healCount}+1
			/varset currentHealTarget ${selector}
			/varset currentHealSpell ${EffectiveHealSpellName}
			/varset currentHealTargetPct ${Spawn[pc ${selector}].PctHPs}
			/varset currentHealType Effective
		}
	/next i

	/if (${currentHealSpell.Length}>0 && ${Cast.Ready}) {
		/bc ${currentHealType} ${currentHealSpell} -> ${currentHealTarget} (${Spawn[pc ${currentHealTarget}].PctHPs}% HP) ${healCandidateCount}A
		
		/casting "${currentHealSpell}" -targetid|${Spawn[pc ${currentHealTarget}].ID} -maxtries|5
	}
/return