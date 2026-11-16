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
		At	Head
		Part	:Nucleus_Glow.part
	End

	Event
		Type	Local
		At	WepL
		Part	:Nucleus_Glow_Hand.part
	End

	Event
		Type	Local
		At	WepR
		Part	:Nucleus_Glow_Hand.part
	End
End

Condition
	On 	Cycle
	Time 	13

	Event
		Type	Local
		At	Chest
		ChildFX	:CHILD_Electron_Head.fx
		Lifespan 40
	End

	Event
		Ename	ElectronWEPL
		Type	Local
		At	WepL
		ChildFX	:CHILD_Electron_Fists.fx
		Lifespan 40
	End

	Event
		Ename	ElectronWEPR
		Type	Local
		At	WepR
		ChildFX	:CHILD_Electron_Fists.fx
		Lifespan 40
	End
End

#############################################################

End