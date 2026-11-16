#############################################################
## TendrilFists.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## FISTS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:UarmTendrils01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:UarmTendrils01.part
		Pmagnet UarmL
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyTendrilsTrailSmall01.part
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:UarmTendrils01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyTendrilsTrailSmall01.part
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:UarmTendrils01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyTendrils01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyTendrils01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FistTendrils01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FistTendrils01.part
		Pmagnet UarmL
	End
End

## EYES ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Bhvr	behaviors/GenericParticleFade.bhvr
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		part	:EyeGlow01.part
		part	:EyeTendrils01.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		part	:EyeGlow01.part
		part	:EyeTendrils01.part
	End
End

## HAIR ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Neck
		part	:HairTendrils01.part
		POther	Hair
	End

	Event
		Type	Local
		At	Eyes
		part	:HairTendrils01.part
		part	:BodyTendrilsTrailSmall01.part
		POther	Head
	End
End

#############################################################

End