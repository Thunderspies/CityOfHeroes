#############################################################
## ShadowyPresence_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	1

	Event
		EName	DarkLight
		Type	SetLight
		BHVR	:LightPulse_Dark.bhvr
		Lifespan 90
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
		pOther	Neck
		sound	ShadowyPresence_Continuous_Loop 20 15 0.15
	End

#############################################################

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
		pOther	LArmL
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
		pOther	HandL
	End

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	LArmR
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
		pOther	HandR
	End

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
	End

#############################################################

	Event
		Type	Local
		At	ULegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	LLegL
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	FootL
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	LLegR
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	FootR
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
	End
End

#############################################################

End