#############################################################
## Child_SteamJet_Small.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

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
	Time	2



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
	Time	4

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
	Time	5



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
	Time	7

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
	Time	15

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan 	30

	End



##############################################################

End