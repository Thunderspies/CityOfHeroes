#############################################################
## Child_SteamJet_Small.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		Part	:Body_Glow_Core.part
		Lifespan 	10
	End
End


Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Ripples.part
		POther	LArmL
		Lifespan 	10

	End

End

Condition
	On	Time
	Time	10



	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Ripples.part
		POther	LArmR
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	13



	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Ripples.part
		POther	WepL
		Lifespan 	10

	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Hand_Glow.part
		Part	:Hand_Ripples.part
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Ripples.part
		POther	WepR
		Lifespan 	10

	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Hand_Glow.part
		Part	:Hand_Ripples.part
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Leg_Ripples.part
		POther	LLegR
		Lifespan 	30

	End

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:BeastlyRage_Eyes.fx
		POther	LLegL
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	14

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Leg_Ripples.part
		POther	LLegL
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	17


	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Leg_Glow_BG.part
		Part	:Leg_Ripples.part
		POther	FootR
		Lifespan 	10
	End
End

Condition
	On	Time
	Time	19

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Leg_Glow_BG.part
		Part	:Leg_Ripples.part
		POther	FootL
		Lifespan 	10

	End

End

Condition
	On	Time
	Time	23

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan 	30

	End
End

##############################################################

End