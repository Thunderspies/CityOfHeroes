#############################################################
## TarotCards_Attack.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On	Time
	Time	0
	
	Event
		Type Local
		At Origin
		Sound QuickReflexes_Child_01 30 30 .1
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Small.part
		POther	LArmL
		Lifespan 5
	End
End

#############################################################

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Small.part
		POther	LArmR
		Lifespan 5
	End
End

##############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Point_Small.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Small.part
		POther	WepL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Extremity_Glow_Small.part
		part	:Extremity_Ring_Small.part
		Lifespan 5
	End
End

##############################################################

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Point_Small.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Small.part
		POther	WepR
		Lifespan 5
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Extremity_Glow_Small.part
		part	:Extremity_Ring_Small.part
		Lifespan 25
	End
End

#############################################################

End