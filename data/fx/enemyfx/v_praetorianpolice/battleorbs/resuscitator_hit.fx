#############################################################
## PlasmaticStunner_Attack.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	StartPositOnly
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Rings_Static_Ground.part
		Part	:Glows_Static_Ground.part
		Part	:Glows_Ground.part
		POther	Neck
		Lifespan 10
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	Neck
		Lifespan 60
	End

	Event
		Type	Local
		At	Neck
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	UArmL
		Lifespan 60
	End

	Event
		Type	Local
		At	Neck
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	UArmR
		Lifespan 60
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	LarmL
		Lifespan 60
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	LarmR
		Lifespan 60
	End

	Event
		Type	Local
		At	LarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	WepL
		Lifespan 60
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	WepR
		Lifespan 60
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	ULegL
		Lifespan 60
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	ULegR
		Lifespan 60
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	LLegL
		Lifespan 60
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	LLegR
		Lifespan 60
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	FootR
		Lifespan 60
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glows_Static_Body.part
		POther	FootL
		Lifespan 60
	End

#############################################################

End