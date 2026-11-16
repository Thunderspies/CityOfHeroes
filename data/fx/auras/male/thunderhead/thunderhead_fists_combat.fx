#############################################################
## Thunderhead_Fists_Combat.fx
#############################################################

FxInfo
Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

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
		ChildFX	:Thunderhead_Fists.fx
	End
End

#############################################################

End