#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part	:Nucleus_Glow_Large.part
		Part	:Nucleus_Glow_Core.part
	End

	Event
		Type	Local
		At	Root
		Part	:Healing_Ring.part
	End
End

Condition
	On 	Cycle
	Time 	12

	Event
		Type	Local
		At	Chest
		ChildFX	:CHILD_Electron.fx
		Lifespan 40
	End
End

#############################################################

End