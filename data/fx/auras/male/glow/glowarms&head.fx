#############################################################
## GlowArms.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## ARMS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmR
		part	:BodyGlow01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	HandR
		part	:BodyGlow01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		part	:BodyGlow01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandL
		part	:BodyGlow01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandR
		part	:FistGlow01.part
	End

	Event
		Type	Local
		At	HandL
		part	:FistGlow01.part
	End
End

## HEAD ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		part	:HeadGlow01.part
		Pmagnet Head
	End
End

#############################################################

End