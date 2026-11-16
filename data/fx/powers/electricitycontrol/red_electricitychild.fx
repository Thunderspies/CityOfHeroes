#############################################################
## Red_ElectricityChild.fx
#############################################################

FxInfo

Flags DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound electransform2 70 70 0.88
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		ChildFx	V_COV\Powers\ElectricalMelee\EmpPulse.fx
		lifespan 240
	End

	Event
		Type	local
		At	Chest
		Part	V_COV\Powers\ElectricalMelee\Red_Haze.part
		Sound Sizzle1 60 60 0.55
	End

	Event
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\ElectricityControl\Red_ElectricitySparkShieldRing.part
	End
End

#############################################################

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	Origin
		Sound elecarmor5_loop 70 70 0.4
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 350
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	hair
		BhvrOverride
			PositionOffset		0.15 0.2 0
		End
		Part	POWERS\ElectricityControl\RedPupil.part
		Part	POWERS\ElectricityControl\WhitePupil.part
	End

	Event
		Type	Local
		At	hair
		BhvrOverride
			PositionOffset		-0.15 0.2 0
		End
		Part	POWERS\ElectricityControl\RedPupil.part
		Part	POWERS\ElectricityControl\WhitePupil.part
	End

	Event
		Type	Local
		At	hair
		BhvrOverride
			PositionOffset		 -0.11 -0.1 0
		End
		Part	POWERS\ElectricityControl\CreepyMouth.part
	End
End

#############################################################
## HAZY SMOKE
#############################################################

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
	End

	Event
		Type	Local
		At	spadR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
		Part	POWERS\ElectricityControl\Red_ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	spadL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
		Part	POWERS\ElectricityControl\Red_ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
	End

	Event
		Type	Local
		At	LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
	End

	Event
		Type	Local
		At	ULegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
		Part	POWERS\ElectricityControl\Red_ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	ULegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
		Part	POWERS\ElectricityControl\Red_ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	LLegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
		Part	POWERS\ElectricityControl\Red_ElectricityTendrils.part
	End

	Event
		Type	Local
		At	LLegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
		Part	POWERS\ElectricityControl\Red_ElectricityTendrils.part
	End

	Event
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
		Part	POWERS\ElectricityControl\Red_ElectricityTendrils.part
	End

	Event
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityIonHaze.part
		Part	POWERS\ElectricityControl\Red_ElectricityTendrils.part
	End
End

#############################################################
## RANDOM ELECTRICAL FLASHES
#############################################################

Condition
	On 	cycle
	Time 	3
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	Hair
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	4
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	UarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	UarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End


Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	ULegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	UlegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	LlegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	LlegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part	POWERS\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	POWERS\ElectricityControl\Red_ElectricityArchContinuing2z.part
		lifespan 3
	End
End

#############################################################

End