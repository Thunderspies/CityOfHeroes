#############################################################
## GasHair.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## HAIR ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		part	:HeadGasTrail01.part
		POther	Hair
	End

	Event
		Type	Local
		At	Hair
		part	:HeadGas01.part
		POther	Head
	End
End

#############################################################

End