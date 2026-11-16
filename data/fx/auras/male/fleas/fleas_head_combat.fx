#############################################################
## Fleas_Head_Combat.fx
#############################################################

FxInfo
Flags DontSuppress

#############################################################

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
		ChildFX	:Fleas_Head.fx
	End
End

#############################################################

End