#############################################################
## FireHead.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

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
		part	:EyeGlow01.part
		part	:EyeGlow02.part
		part	:FireEyesLeft01.part
		part	:FireEyesLeft01.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:EyeGlow01.part
		part	:EyeGlow02.part
		part	:FireEyesRight01.part
		part	:FireEyesRight01.part
	End
End

## HEAD ###########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Fire1
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
		part	:FireFlame01.part
		part	:FireEmbers01.part
		part	:FireCrawl01.part
		part	:FireBase01.part
		Pmagnet Head
	End
End

#############################################################

End