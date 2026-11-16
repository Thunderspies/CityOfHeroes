#############################################################
## Binary_Body_Combat.fx
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
		Part	:LightBlobs.part
		Part	:DataBeamsUp.part
		part	:DataStringsUp01.part
		BhvrOverride
			PositionOffset	0.0 2 -0.5
		End
	End

	Event
		EName	CombatFX2
		Type	Local
		At	Root
		While	COMBAT
		Flags	OneAtATime
		Part	:LightBlobs02.part
		Part	:DataBeamsUp02.part
		part	:DataStringsUp02.part
		BhvrOverride
			PositionOffset	0 2 0.5
		End
	End

End
#############################################################
End