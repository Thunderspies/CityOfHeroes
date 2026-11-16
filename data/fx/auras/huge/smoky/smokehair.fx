#############################################################
## SmokeHair.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySmokeTrailSmall01.part
		POther	Hair
	End

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:HairSmoke01.part
		POther	Head
	End
End

#############################################################

End