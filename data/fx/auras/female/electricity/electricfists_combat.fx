#############################################################
## ElectricFists_Combat.fx
#############################################################

FxInfo
Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On		Triggerbits
	Triggerbits	COMBAT
	DoMany		1

	Event
		ENAME	FistsAnchor
		Type	Local
		At	Root
		While	COMBAT
		Flags	OneAtATime
		ChildFX	:ElectricFists.fx
	End
End

#############################################################

End