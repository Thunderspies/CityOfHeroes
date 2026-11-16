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
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Leg_Glow_Heal.part

		POther	FootR
		Lifespan 	10
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Leg_Glow_Heal.part

		POther	FootL
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Leg_Glow_Heal.part
		POther	LLegR
		Lifespan 	30

	End

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Leg_Glow_Heal.part
		POther	LLegL
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Spiral_Explode_Small.part
		Part	:Spiral_Explode_Small_Alpha.part
		Part	:Chest_Glow_Heal.part
		Lifespan 	10
	End
End

Condition
	On	Time
	Time	16

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Glow_Heal.part

		POther	LArmL
		Lifespan 	10
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Glow_Heal.part

		POther	LArmR
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Glow_Heal.part

		POther	WepL
		Lifespan 	10

	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Glow_Heal.part
		POther	WepR
		Lifespan 	10

	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Arm_Glow_Heal.part
		Lifespan 	10
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Arm_Glow_Heal.part
		Lifespan 	10
	End

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Leg_Glow.part
		Part	:Arm_Glow_Heal.part
		Lifespan 	10
	End
End



##############################################################

End