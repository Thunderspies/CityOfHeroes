#############################################################
## Plasma_FistsAndEyes_Huge.fx
#############################################################

FxInfo
Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

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
		ChildFX	:Plasma_FistsAndEyes_Huge.fx
	End
End

#############################################################

End