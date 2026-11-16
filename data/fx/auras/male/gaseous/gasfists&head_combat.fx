#############################################################
## GasFists.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## FISTS ###########################################################

Condition
	On		Triggerbits
	Triggerbits	COMBAT
	DoMany		1

	Event
		EName	WepRAnchor
		Type	Local
		At	WepR
		While	COMBAT
		Flags	OneAtATime
		part	:FistGas01.part
		part	:BodyGas01.part
		part	:HeadGasTrail01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		EName	WepLAnchor
		Type	Local
		At	WepL
		While	COMBAT
		Flags	OneAtATime
		part	:FistGas01.part
		part	:BodyGas01.part
		part	:HeadGasTrail01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		EName	LArmRAnchor
		Type	Local
		At	LarmR
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet UarmR
		POther  UarmR
	End

	Event
		EName	LArmLAnchor
		Type	Local
		At	LarmL
		While	COMBAT
		Flags	OneAtATime
		part	:BodyGas01.part
		Pmagnet UarmL
		POther  UarmL
	End

## EYES ###########################################################

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

## HEAD ###########################################################

	Event
		EName	HeadAnchor
		Type	Local
		At	Head
		While	COMBAT
		Flags	OneAtATime
		Bhvr	:HeadOffset1.bhvr
		part	:HeadGas01.part
		Pmagnet Head
	End
End

#############################################################

End