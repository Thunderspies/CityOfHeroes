#############################################################
## TendrilFeet.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## FEET ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyTendrils01.part
		part	:BodyTendrilsTrailSmall01.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyTendrils01.part
		part	:BodyTendrilsTrailSmall01.part
		Pmagnet FootL
		POther  FootL
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FistTendrils01.part
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FistTendrils01.part
	End

	Event
		Type	Local
		At	ToeR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FistTendrils01.part
	End

	Event
		Type	Local
		At	ToeL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FistTendrils01.part
	End
End

#############################################################

End