#############################################################
## GasHead.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## EYES ###########################################################

Condition
	On		Triggerbits
	Triggerbits	Combat
	DoMany		1

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		While	COMBAT
		Flags	OneAtATime
		Geom	CircleOfThorns
	End

	Event
		EName	EyeLAnchor
		Type	Local
		At	EyeAnchor
		AltPiv	1
		While	COMBAT
		Flags	OneAtATime
		part	:EyeGlow01.part
		part	:EyeGas01.part
	End

	Event
		EName	EyeRAnchor
		Type	Local
		At	EyeAnchor
		AltPiv	2
		While	COMBAT
		Flags	OneAtATime
		part	:EyeGlow01.part
		part	:EyeGas01.part
		part	:HeadGasTrail01.part
	End

	Event
		EName	HeadAnchor
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
		While	COMBAT
		Flags	OneAtATime
		part	:HeadGas01.part
		Pmagnet Head
	End
End

#############################################################

End