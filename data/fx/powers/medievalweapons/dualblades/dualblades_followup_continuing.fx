#############################################################
## FX System Name
#############################################################

FxInfo

Lifespan 120

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	POWERS\FollowUpGlow.part
		Part2	POWERS\FollowUpSpecks2.part
		Sound clawfollow3 100 100 .8
		Lifespan	25
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part2	POWERS\FollowUpRing01.part
		Part3	POWERS\FollowUpRing01a.part
	End
End

Condition
	On	Time
	Time	25

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part2	POWERS\FollowUpRingSlow.part
		Part3	POWERS\FollowUpRing03.part
		Part4	POWERS\FollowUpRing03a.part
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part3	POWERS\FollowUpRing02.part
		Part4	POWERS\FollowUpRing02a.part
	End
End

## BODY AURA ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowSmall1.part
		part	:DualBlades_FollowUp_GlowSmall2.part
		POther  LArmR
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowSmall1.part
		part	:DualBlades_FollowUp_GlowSmall2.part
		POther  LArmL
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowSmall1.part
		part	:DualBlades_FollowUp_GlowSmall2.part
		POther  UArmR
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowSmall1.part
		part	:DualBlades_FollowUp_GlowSmall2.part
		POther  UArmL
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowMedium1.part
		part	:DualBlades_FollowUp_GlowMedium2.part
		POther  ULegR
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowMedium1.part
		part	:DualBlades_FollowUp_GlowMedium2.part
		POther  ULegL
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowSmall1.part
		part	:DualBlades_FollowUp_GlowSmall2.part
		POther  Head
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowSmall1.part
		part	:DualBlades_FollowUp_GlowSmall2.part
		POther  Head
	End
End

Condition
	On	Time
	Time	9

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowSmall1.part
		part	:DualBlades_FollowUp_GlowSmall2.part
		POther  Hair
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowMedium1.part
		part	:DualBlades_FollowUp_GlowMedium2.part
		POther  LLegR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowMedium1.part
		part	:DualBlades_FollowUp_GlowMedium2.part
		POther  LLegL
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	LLegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowSmall1.part
		part	:DualBlades_FollowUp_GlowSmall2.part
		POther  FootR
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:DualBlades_FollowUp_GlowSmall1.part
		part	:DualBlades_FollowUp_GlowSmall2.part
		POther  FootL
	End
End

#############################################################

End