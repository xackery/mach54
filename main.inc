| Main logic
#include mach54/main_assist.inc
#include mach54/main_puller.inc

Sub MainInit
    /call MainAssistInit
    /call MainPullerInit
/return

Sub MainLoad
    /call MainAssistLoad
    /call MainPullerLoad
/return

Sub MainLoop
    /if (${State.Equal[Idle]} && ${Stick.Active}) /stick off
    /call MainAssistLoop
    /call MainPullerLoop
/return