#############################################################
## Wealthy_Head.fx
#############################################################

FxInfo

###########################################################

Condition
	On		Triggerbits
	Triggerbits	COMBAT
	DoMany		1

	Event
		EName	CombatFX
		Type	Local
		At	Root
		While	COMBAT
		Flags	OneAtATime
		ChildFX	:Microbial_FistR.fx
	End
End

###########################################################

End
