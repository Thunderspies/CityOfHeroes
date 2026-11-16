#############################################################
## SmokeArms.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	LarmRNode
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySmokeTrailSmall01.part
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:UarmSmoke01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySmokeTrailSmall01.part
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:UarmSmoke01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySmoke01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySmoke01.part
	End

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodySmoke01.part
	End
End

#############################################################

End