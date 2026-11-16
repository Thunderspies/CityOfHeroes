#############################################################
## Peacebringer - Shining Shield
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	Head
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowArm1.part
	End

	Event
		Type	Local
		At	Waist
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowLeg1.part
		POther	Waist
	End

	Event
		Type	Local
		At	ULegL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowLeg1.part
		POther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowLeg1.part
		POther	LLegR
	End

	Event
		EName	4
		Type	Local
		At	LLegL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowLeg1.part
		POther	FootL
	End

	Event
		EName	7
		Type	Local
		At	LLegR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowLeg1.part
		POther	FootR
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowArm1.part
		POther	LArmL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowArm1.part
		POther	LArmR
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowArm1.part
		POther	HandL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:Aura_GlowArm1.part
		POther	HandR
	End
End

#############################################################

End