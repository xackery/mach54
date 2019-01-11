|based on https://github.com/kodywilson/modbot

Sub LootInit
	/declare IsLooter bool outer FALSE
	/declare IsCombatLoot bool outer FALSE
	/declare LootCorpseRadius int outer 50
	|---Internal Variables---
	/declare LootRetryTimer timer outer
	|will be set to true when out of space, and will return to true once emptied
	/declare OutOfSpace bool outer FALSE
	/declare MasterCorpseList string outer
	/declare CantLootID int outer 0
/return

Sub LootLoad
	/declare mainLooter string local 
    /varset mainLooter ${Ini[mach54/mach54.ini,General,MainLooter]} 
	/bc ${mainLooter}
	/if (${mainLooter.NotEqual[${Me.Name}]}) /return
	/varset LootCorpseRadius ${Ini[mach54/mach54.ini,General,LootCorpseRadius]} 

    /varset IsLooter TRUE
    /bc I will loot items
/return

#event LootFail         	"#*#may not loot this corpse#*#"
#event LootFailLore         "#*#cannot loot this Lore Item#*#"

Sub Event_LootFail
	/bc Failed to loot
	/varset CantLootID ${Target.ID}
/return

Sub Event_LootFailLore
	/bc Failed to loot - lore item
/return

/return
Sub LootLoop
	/if (!${IsLooter}) /return
	
	/declare selectedItem string local
	/declare itemAction string local
	/declare i int local
	/declare lootList string local
	/if (!${Cast.Ready}) /return
	
	/if (!${State.Equal[Idle]}) {
		/if (!${State.Equal[Combat]}) /return
		/if (!${IsCombatLoot}) /return
		/return
	}
	/if (${LootRetryTimer}>0) /return
	/call CheckCursor
	/if (!${Me.FreeInventory}) {
		/if (${OutOfSpace}) /return
		/bc Out of inventory space, won't loot mobs
		/varset OutOfSpace TRUE
		/return
	}
	
	/if (${OutOfSpace}) /varset OutOfSpace FALSE
	/call LootMob
	/varset LootRetryTimer 2s
/return

Sub LootMob
	/declare deadCount int local ${SpawnCount[corpse radius ${LootCorpseRadius} zradius 50]}
	/declare corpseList string local
	/declare i int local
	/declare closestCorpseID int local
	/declare corpseToLootID int local
	/declare corpseCount int local 0


	/if (${deadCount}==0) /return
	/for i 1 to ${deadCount}
		/varset closestCorpseID ${NearestSpawn[${i},corpse radius ${LootCorpseRadius} zradius 50].ID}
		/if (${Spawn[${closestCorpseID}].Deity.ID}) /next i
		/varset corpseList ${corpseList}${closestCorpseID}|
        /if (!${MasterCorpseList.Find[${closestCorpseID}]}) /varset MasterCorpseList ${MasterCorpseList}${closestCorpseID}|
        /varcalc corpseCount ${corpseCount}+1
	/next i

	/for i 1 to ${deadCount}
		/varset corpseToLootID ${corpseList.Arg[${i},|]}
		/if (!${Defined[corpseLocked${corpseToLootID}]}) /declare corpseLocked${corpseToLootID} timer outer 0
		/if (${Spawn[${corpseToLootID}].Deity.ID}) /next i
		/if (!${Spawn[${corpseToLootID}].ID}) /next i
		/if (!${Spawn[${corpseToLootID}].LineOfSight}) /next i
		/if (${corpseLocked${corpseToLootID}}) /next i
		/if (${Target.ID}!=${corpseToLootID}) /target id ${corpseToLootID}
		/if (!${Me.Standing}) /stand

		/delay 3s ${Target.ID}==${corpseToLootID} && ${Me.Standing}
		/if (${Target.Distance} > 5) {
			/moveto mdist 10
			/moveto loc ${Target.Y} ${Target.X} ${Target.Z}
			/if (${Me.Mount.ID}) {
				/delay 3s ${Math.Distance[${Target.Y},${Target.X}]}<25
				/keypress back
			}
			/delay 3s ${MoveTo.Stopped} && !${Me.Moving}
			/if (${Target.ID}) /face fast
		}
		/if (${Target.Distance} < 12) {
			/call LootCorpse
			/if (!${Defined[corpseLocked${corpseToLootID}]}) /next i
			/if (${corpseLocked${corpseToLootID}}) /next i
			/if (!${MasterCorpseList.Find[${corpseToLootID}]}) /next i
			/deletevar corpseLocked${corpseToLootID}
			/varset MasterCorpseList ${MasterCorpseList,Replace[${corpseToLootID}|,]}
		}
	/next i
/return

Sub LootCorpse
	/declare corpseTotalItems int local
	/declare i int local
	/declare selectedItem string local
	/declare itemAction string local

	/varset CantLootID 0
	/loot
	/delay 3s ${Corpse.Open}
	/doevents LootFail
	/delay 3s ${Corpse.Items}
	/if (!${Corpse.Open} || !${Corpse.Items} || ${CantLootID}>0) {
		/bc Corpse wouldn't open.. hmm
		/varset LootRetryTimer 6s
		/return
	}
	/if (${Corpse.Items} == 0) /goto :LootFinish
:LootLag
	/varset corpseTotalItems ${Corpse.Items}
	/echo ${corpseTotalItems}
	/if (${corpseTotalItems} != ${Corpse.Items}) /goto :LootLag
	/for i 1 to ${corpseTotalItems}
		/if (!${Corpse.Item[${i}].ID}) /next i

		/varset itemAction ${Ini["mach54/mach54.ini","Loot","${selectedItem}"]}
		/if (${itemAction.Length}==0) /next i

		/varset selectedItem ${If[${Corpse.Item[${i}].Name.Find[,]},${Corpse.Item[${i}].Name.Arg[1,,]}${Corpse.Item[${i}].Name.Arg[2,,]},${Corpse.Item[${i}].Name}]}

		/echo ${selectedItem} ${itemAction}
		|Destroy
		/if (${itemAction.Equal[Destroy]}) {

    		|/if (!${QuestOnly}) /call LootItem ${i} Destroy left
			/bc ${selectedItem} Destroy ${Corpse.Item[${i}].BuyPrice}
    		/next i
  		}

		|Keep
		/if (${itemAction.Equal[Keep]}) {
			/bc ${selectedItem} Keep ${Corpse.Item[${i}].BuyPrice}
			/call LootItem ${i} Keep right
			/next i
		}

		|Announce
		/if (${itemAction.Equal[Announce]}) {
			/bc ${selectedItem} dropped from corpse id ${Corpse.ID} ${Corpse.Item[${i}].BuyPrice}
			/next i
		}
		/bc ${selectedItem} Keep (default) ${Corpse.Item[${i}].BuyPrice}
		/call LootItem ${i} Keep right
	/next i
:LootFinish
	/nomodkey /notify LootWnd LW_DoneButton leftmouseup
	/delay 5s !${Corpse.Open}
	/if (${Corpse.Open}) /goto :LootFinish
/return

Sub LootItem(int i,DoWhat,WhichButton)
    /if (${Debugloot}) /echo DEBUGLOOT Enter Sub LootItem ${i} ${DoWhat} ${WhichButton}
    /declare CorpseItemID int local ${Corpse.Item[${i}].ID}
    /nomodkey /shift /itemnotify ${Corpse.Item[${i}].InvSlot} ${WhichButton}mouseup
    /delay 5s ${Window[ConfirmationDialogBox].Open} || !${Corpse.Item[${i}].NoDrop}
    /if (${Window[ConfirmationDialogBox].Open}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
    /delay 5s ${Cursor.ID} || ${WhichButton.NotEqual[left]}
    /if (${DoWhat.Equal[Destroy]} && ${Cursor.ID}==${CorpseItemID}) /destroy
    /delay 5s !${Corpse.Item[${i}].ID}
    /if (${Cursor.ID}) /call CheckCursor
    /if (${Debugloot}) /echo DEBUGLOOT Leave Sub LootItem
/return

Sub CheckCursor
    :AutoInv
    /if (!${Cursor.ID}) /return
    /if (${Cursor.NoDrop} && !${Me.FreeInventory}) /return
    /timed 5 /autoinventory
    /delay 2s !${Cursor.ID}
    /goto :AutoInv
/return