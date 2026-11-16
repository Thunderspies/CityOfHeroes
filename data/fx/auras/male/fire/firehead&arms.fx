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

## ARMS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	HandR
		part	:Fiery01.part
		part	:FieryFist01.part
		Pmagnet HandR
	End

	Event
		Type	Local
		At	HandL
		part	:Fiery01.part
		part	:FieryFist01.part
		Pmagnet HandL
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part	:FireFlameBody01.part
		part	:FireEmbersBody01.part
		part	:FireCrawlBody01.part
		part	:FireBaseBody01.part
		Pmagnet LarmL
		POther  LarmL
	End
End

#############################################################

End