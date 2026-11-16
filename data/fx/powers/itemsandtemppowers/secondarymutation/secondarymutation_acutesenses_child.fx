#############################################################
## TarotCards_Attack.fx
#############################################################

FxInfo
Lifespan 10

#############################################################

Condition
	On	Time
	Time	0
	
	Event
		Type Local
		At Origin
		Sound AcuteSenses_Child_01 30 30 .1
	End

	Event
		Type	Local
		At	Head
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part	:Leg_Glow_Small.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Head
		Bhvr 	behaviors/GenericParticleFade.bhvr
		part	:Extremity_Glow_Small.part
		part	:Extremity_Ring_Small.part
		Lifespan 5
	End
End


#############################################################

End