#############################################################
## GasHair_Combat.fx
#############################################################

FxInfo
Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## HAIR ###########################################################

Condition
	On 		Triggerbits
	Triggerbits 	COMBAT
	DoMany		1

	Event
		EName	HeadAnchor
		Type	Local
		At	Head
		While	COMBAT
		Flags	OneAtATime
		part	:HeadGasTrail01.part
		POther	Hair
	End

	Event
		EName	HairAnchor
		Type	Local
		At	Hair
		While	COMBAT
		Flags	OneAtATime
		part	:HeadGas01.part
		POther	Head
	End
End

#############################################################

End