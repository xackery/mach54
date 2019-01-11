Sub NukeInit
	|---Settings Variables---
	/declare EffectiveNuke string outer
	/declare EffectiveNukePct int outer 60 
	/declare EffectiveNukeRange int outer 100
	/declare EffectiveNukeManaPct int outer 90
	/declare EffectiveNukeCooldown int outer 6
	/declare EffectiveNukeTargetHPPct int outer 80	

	/declare FinisherNuke string outer
	/declare FinisherNukePct int outer 10 
	/declare FinisherNukeRange int outer 100 
	/declare FinisherNukeManaPct int outer 50
    /declare FinisherNukeCooldown int outer 6
	/declare FinisherNukeTargetHPPct int outer 18
	

	|---Internal Variables---	
	/declare EffectiveNukeCooldownTimer timer outer
/return

Sub NukeLoad
    /call ConfigRead Nuke EffectiveNuke
    /varset EffectiveNuke ${Macro.Return}
	/call ConfigRead Nuke EffectiveNukePct
    /varset EffectiveNukePct ${Macro.Return}
	/call ConfigRead Nuke EffectiveNukeRange
    /varset EffectiveNukeRange ${Macro.Return}
	/call ConfigRead Nuke EffectiveNukeManaPct
    /varset EffectiveNukeManaPct ${Macro.Return}
	/call ConfigRead Nuke EffectiveNukeCooldown
    /varset EffectiveNukeCooldown ${Macro.Return}
	/call ConfigRead Nuke EffectiveNukeTargetHPPct
    /varset EffectiveNukeTargetHPPct ${Macro.Return}
    /call ConfigRead Nuke FinisherNuke
    /varset FinisherNuke ${Macro.Return}
	/call ConfigRead Nuke FinisherNukePct
    /varset FinisherNukePct ${Macro.Return}
	/call ConfigRead Nuke FinisherNukeRange
    /varset FinisherNukeRange ${Macro.Return}
	/call ConfigRead Nuke FinisherNukeManaPct
    /varset FinisherNukeManaPct ${Macro.Return}
    /call ConfigRead Nuke FinisherNukeCooldown
    /varset FinisherNukeCooldown ${Macro.Return}
	/call ConfigRead Nuke FinisherNukeTargetHPPct
    /varset FinisherNukeTargetHPPct ${Macro.Return}
/return

Sub NukeLoop
	/if (!${Bool[${Spawn[pc ${MainAssist}]}]}) /return
	/if (${Me.PctMana} < 5) /return

	| MainTarget isn't an NPC, return
	/if (${Spawn[${MainTargetID}].Type.NotEqual[NPC]}) /return

	| Don't do nuke logic if finisher nuke isn't set
	/if (${FinisherNuke.Length}==0) /return
	| Don't bother nuking if not in combat
	/if (${State.NotEqual[Combat]}) /return

	| Don't nuke if I'm medding and not hit my threshold
	/if (${IsSitPriorityOverNuking} && ${Me.PctMana}<${SitMaxManaPct}) /return

	

	|How many nuke candidates are there
	/declare nukeCandidateCount int 0 
	|Which spell is going to be used
	/declare currentNukeSpell string 
	|Who is the target for the spell
	/declare currentNukeTarget string 
	|How hurt is this target
	/declare currentNukeTargetPct int 100 
	|What type of nuke
	/declare currentNukeType string 

	|Iterator selector
	/declare selector string 
	|Iterator
	/declare i int 0 


| Iterate group members
|	/for i 0 to ${Group.Members}
|		/varset selector ${Group.Member[${i}]}
|		
|		|Finisher Nukeing		
|		/if (${Bool[${Spawn[pc ${selector}]}]} && ${Spawn[pc ${selector}].Distance} < ${FinisherNukeRange} && ${Spawn[pc ${selector}].PctHPs} < ${FinisherNukePct} && ${Spawn[pc ${selector}].PctHPs} < ${currentNukeTargetPct}) {
|			/varcalc nukeCandidateCount ${nukeCount}+1
|			/varset currentNukeTarget ${selector}
|			/varset currentNukeSpell ${FinisherNuke}
|			/varset currentNukeTargetPct ${Spawn[pc ${selector}].PctHPs}
|			/varset currentNukeType Finisher
|		}
|
|		|Effective Nukeing		
|		/if (${Bool[${Spawn[pc ${selector}]}]} && ${Spawn[pc ${selector}].Distance} < ${EffectiveNukeRange} && ${Spawn[pc ${selector}].PctHPs} < ${EffectiveNukePct} && ${Spawn[pc ${selector}].PctHPs} < ${currentNukeTargetPct}) {
|			/echo Effective
|			/varcalc nukeCandidateCount ${nukeCount}+1
|			/varset currentNukeTarget ${selector}
|			/varset currentNukeSpell ${EffectiveNuke}
|			/varset currentNukeTargetPct ${Spawn[pc ${selector}].PctHPs}
|			/varset currentNukeType Effective
|		}
|	/next i

	/if (${Me.PctMana} >= ${FinisherNukeManaPct} && ${Cast.Ready} && ${currentNukeSpell.Length}==0 && ${Spawn[${MainTargetID}].PctHPs} < ${FinisherNukeTargetHPPct}) {
		/varset currentNukeSpell ${FinisherNuke}
		/varset currentNukeType Finisher
	}
	
	/if (${Me.PctMana} >= ${EffectiveNukeManaPct} && ${EffectiveNukeCooldownTimer}==0 && ${Cast.Ready} && ${currentNukeSpell.Length}==0 && ${Spawn[${MainTargetID}].PctHPs} < ${EffectiveNukeTargetHPPct}) {
		/varset currentNukeSpell ${EffectiveNuke}
		/varset currentNukeType Effective
		/varset EffectiveNukeCooldownTimer ${EffectiveNukeCooldown}s
	}

	/if (${currentNukeSpell.Length}>0 && ${Cast.Ready}) {
		/bc ${currentNukeType} ${currentNukeSpell} -> ${Spawn[${MainTargetID}].Name} (${Spawn[${MainTargetID}].PctHPs}% HP) ${nukeCandidateCount}A		
		/casting "${currentNukeSpell}" -targetid|${MainTargetID} -maxtries|5
	}
/return