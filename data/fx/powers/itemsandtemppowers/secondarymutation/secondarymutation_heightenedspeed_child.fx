#############################################################
## TarotCards_Attack.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type Local
		At Origin
		Sound HeightenedSpeed_Child_01 30 30 .1
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Small.part
		POther	LLegL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	3
	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Small.part
		POther	LLegR
		Lifespan 5
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Point_Small.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	9

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Point_Small.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Small.part
		POther	FootL
		Lifespan 5
	End

	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Extremity_Glow_Small.part
		part	:Extremity_Ring_Small.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Small.part
		POther	FootR
		Lifespan 5
	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Extremity_Glow_Small.part
		part	:Extremity_Ring_Small.part
		Lifespan 5
	End
End

#############################################################

End