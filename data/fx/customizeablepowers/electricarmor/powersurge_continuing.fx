#############################################################
## PowerSurge_Continuing.FX
#############################################################

FxInfo

Flags DontSuppress

Lifespan 5400

## SOUNDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound electransform2 70 70 0.88
	End
End

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	Origin
		bhvr	behaviors\soundFade3.bhvr
		Sound elecarmor5_loop 70 70 0.4
		Flags	PowerLoopingSound
		Lifespan 350
	End
End

#############################################################
## GLOBALS
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
		ChildFx	:Child_EmpPulse.fx
		lifespan 240
	End

	Event
		Type	local
		At	Chest
		Part	:Haze.part
		Sound Sizzle1 60 60 0.55
	End

	Event
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:ElectricitySparkShieldRing.part
	End
End

#############################################################
## EYES AND MOUTH
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
		Part	:Pupil.part
		Part	:WhitePupil.part
	End

	Event
		Type	Local
		At	hair
		BhvrOverride
			PositionOffset		-0.15 0.2 0
		End
		Part	:Pupil.part
		Part	:WhitePupil.part
	End

	Event
		Type	Local
		At	hair
		BhvrOverride
			PositionOffset		 -0.11 -0.1 0
		End
		Part	:CreepyMouth.part
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
		Part	:ElectricityIonHaze.part
	End

	Event
		Type	Local
		At	spadR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	spadL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
	End

	Event
		Type	Local
		At	LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
	End

	Event
		Type	Local
		At	ULegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	ULegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	LLegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:ElectricityTendrils.part
	End

	Event
		Type	Local
		At	LLegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:ElectricityTendrils.part
	End

	Event
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:ElectricityTendrils.part
	End

	Event
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:ElectricityTendrils.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
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
		Part	:ElectricityControl01white.part
		Part	:ElectricityControl02.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

#############################################################

End