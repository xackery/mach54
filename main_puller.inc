| Main logic

Sub MainPullerInit
	/declare MainPuller string outer
    |---Internal Variables---
/return

Sub MainPullerLoad
    /call ConfigRead General MainPuller
    /varset MainPuller ${Macro.Return}
/return

Sub MainPullerLoop

/return