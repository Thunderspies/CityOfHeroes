#############################################################
## GlowArms&Head.fx
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
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyGlow01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyGlow01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyGlow01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:BodyGlow01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:FistGlow01.part
	End

	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
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