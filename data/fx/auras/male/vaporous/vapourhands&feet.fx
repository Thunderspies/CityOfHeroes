#############################################################
## VapourHands.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:HandVapour01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet LarmR
		POther  LarmR
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:HandVapour01.part
		Pmagnet UarmL
	End

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet LarmL
		POther  LarmL
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:HandVapour01.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet LLegR
		POther  LLegR
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:HandVapour01.part
		Pmagnet FootL
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BodyVapour01.part
		Pmagnet LLegL
		POther  LLegL
	End
End

#############################################################

End