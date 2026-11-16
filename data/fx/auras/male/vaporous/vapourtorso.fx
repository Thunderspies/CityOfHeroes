#############################################################
## VapourTorso.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		part	:BodyVapourTrail01.part
		Pmagnet	Neck
		POther  Neck
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet UarmR
		POther  UarmR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet UarmL
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet LarmL
		POther  LarmL
	End
End

#############################################################

End