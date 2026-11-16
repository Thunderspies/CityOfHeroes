#############################################################
## Fleas_Fists_Combat.fx
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
		ChildFX	:Fleas_Fists.fx
	End
End

#############################################################

End