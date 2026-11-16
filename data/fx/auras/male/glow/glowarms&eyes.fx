#############################################################
## GlowArms.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

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

## EYES ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	:EyeNodeL.bhvr
		part	:EyeGlow01.part
		part	:EyeGlow02.part
	End

	Event
		Type	Local
		At	Head
		Bhvr	:EyeNodeR.bhvr
		part	:EyeGlow01.part
		part	:EyeGlow02.part
	End
End

#############################################################

End