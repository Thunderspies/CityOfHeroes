#############################################################
## GasHead.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

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

## HEAD ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
		part	:HeadGas01.part
		Pmagnet Head
	End
End

#############################################################

End