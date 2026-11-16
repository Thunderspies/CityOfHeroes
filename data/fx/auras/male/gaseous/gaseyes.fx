#############################################################
## GasEyes.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## EYES ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	:EyeNodeL.bhvr
		part	:EyeGlow01.part
		part	:EyeGas01.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:EyeNodeR.bhvr
		part	:EyeGlow01.part
		part	:EyeGas01.part
		part	:HeadGasTrail01.part
	End
End

#############################################################

End