#############################################################
## AnguishingCry_Attack.fx
#############################################################

FxInfo

LifeSpan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Left
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PainDomination\Stamina2.part
		Part2	CustomizeablePowers\PainDomination\StaminaSmall2.part
		Part3	CustomizeablePowers\PainDomination\AbilityDots.part
		Part4	CustomizeablePowers\PainDomination\AbilitySparklingDots.part
		Lifespan 45
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PainDomination\Stamina2.part
		Part2	CustomizeablePowers\PainDomination\StaminaSmall2.part
		Part3	CustomizeablePowers\PainDomination\AbilityDots.part
		Part4	CustomizeablePowers\PainDomination\AbilitySparklingDots.part
		Lifespan 45
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	ChestOffset
		Type	local
		At	Root
		Geom	RootToChestAdjustment
	End

	Event
		Type	start
		At	ChestOffset
		Altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\PainDomination\AnguishingCry_Ring1.part
		Part	CustomizeablePowers\PainDomination\AnguishingCry_Ring2.part
		LifeSpan 18
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	local
		At	ChestOffset
		Altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	CustomizeablePowers\PainDomination\AnguishingCry_Ring_Burst1.part
		Part	CustomizeablePowers\PainDomination\AnguishingCry_Ring_Burst2.part
	End
End

#############################################################

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	Root
		BhvrOverride
			Shake          0.625
			ShakeFallOff   0.015
			ShakeRadius    80.0
		End
	End
End

Condition
	On	Time
	Time	17

	Event
		Ename	ExplosiveForce
		Type	Posit
		At	Hips
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Lifespan 30
	End
End

#############################################################

End