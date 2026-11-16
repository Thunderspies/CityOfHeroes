#############################################################
## GasFists.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## FISTS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	HandR
		part	:FistGas01.part
		part	:BodyGas01.part
		part	:HeadGasTrail01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	HandL
		part	:FistGas01.part
		part	:BodyGas01.part
		part	:HeadGasTrail01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	LarmR
		part	:BodyGas01.part
		Pmagnet UarmR
		POther  UarmR
	End

	Event
		Type	Local
		At	LarmL
		part	:BodyGas01.part
		Pmagnet UarmL
		POther  UarmL
	End
End

#############################################################

End