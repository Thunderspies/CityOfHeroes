#############################################################
## GasEyes.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## EYES ###########################################################

Condition
	On 		Triggerbits
	Triggerbits 	COMBAT
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
End

#############################################################

End