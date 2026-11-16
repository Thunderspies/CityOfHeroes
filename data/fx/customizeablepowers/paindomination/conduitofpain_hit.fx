#############################################################
## Resurection.fx
#############################################################

FxInfo

LifeSpan 300

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Left
		Type	Local
		At	wepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PainDomination\Stamina2.part
		Part2	CustomizeablePowers\PainDomination\StaminaSmall2.part
		Part3	CustomizeablePowers\PainDomination\AbilityDots.part
		Part4	CustomizeablePowers\PainDomination\AbilitySparklingDots.part
		Sound heal6 70 70 .8
		Lifespan 80
	End

	Event
		EName	Right
		Type	Local
		At	wepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\PainDomination\Stamina2.part
		Part2	CustomizeablePowers\PainDomination\StaminaSmall2.part
		Part3	CustomizeablePowers\PainDomination\AbilityDots.part
		Part4	CustomizeablePowers\PainDomination\AbilitySparklingDots.part
		Lifespan 80
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	BottleRocketHookup
		Type	Local
		At	T_Root
		Bhvr	CustomizeablePowers\PainDomination\teleportscale.bhvr
		Anim	FX_teleportbase
		Lifespan 120
	End

	Event
		EName 	BottleRocket02
		Type	Local
		At	BottleRocketHookup
		AnimPiv Waist
		Part1	CustomizeablePowers\PainDomination\RessurectionBottleRocketSparks.part
		Part2	CustomizeablePowers\PainDomination\BottleRocketHead.part
		Lifespan 120
	End

	Event
		EName 	BottleRocket03
		Type	Local
		At	BottleRocketHookup
		AnimPiv Neck
		Part1	CustomizeablePowers\PainDomination\RessurectionBottleRocketSparks.part
		Part2	CustomizeablePowers\PainDomination\BottleRocketHead.part
		Lifespan 120
	End

	Event
		EName 	BottleRocket04
		Type	Local
		At	BottleRocketHookup
		AnimPiv Head
		Part1	CustomizeablePowers\PainDomination\RessurectionBottleRocketSparks.part
		Part2	CustomizeablePowers\PainDomination\BottleRocketHead.part
		Lifespan 120
	End

	Event
		EName	Outerglow
		Type	Local
		At	T_Root
		Part1	CustomizeablePowers\PainDomination\Sparklies.part
		Part2	CustomizeablePowers\PainDomination\RingSparklies.part
		Part3	CustomizeablePowers\PainDomination\SharpRingSparklies.part
		Part4	CustomizeablePowers\PainDomination\streaks.part
		Part5	CustomizeablePowers\PainDomination\Glowstreaks.part
		Lifespan 165
	End
End

#############################################################

Condition
	On	Time
	Time	28

	Event
		EName	Ring
		Type	Local
		At	T_Root
		part1	CustomizeablePowers\PainDomination\StaminaRingUp.part
		Lifespan 120
	End
End

Condition
	On	Time
	Time	43

	Event
		EName	Ring1
		Type	Local
		At	T_Root
		part1	CustomizeablePowers\PainDomination\StaminaRingUp.part
		Lifespan 105
	End
End

Condition
	On	Time
	Time	58

	Event
		EName	Ring2
		Type	Local
		At	T_Root
		part1	CustomizeablePowers\PainDomination\StaminaRingUp.part
		Lifespan 90
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	T_FootR
		part1	CustomizeablePowers\PainDomination\HealingBodyGlows.part
		part2	CustomizeablePowers\PainDomination\HealingBodyGlowsFlat.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_ULEGL
		part1	CustomizeablePowers\PainDomination\HealingMotionGlows.part
		PMagnet Knee1
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LLEGL
		part1	CustomizeablePowers\PainDomination\HealingMotionGlows.part
		PMagnet T_FootL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_FootL
		part1	CustomizeablePowers\PainDomination\HealingBodyGlows.part
		part2	CustomizeablePowers\PainDomination\HealingBodyGlowsFlat.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_ULEGR
		part1	CustomizeablePowers\PainDomination\HealingMotionGlows.part
		PMagnet Kneer
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LLEGR
		part1	CustomizeablePowers\PainDomination\HealingMotionGlows.part
		PMagnet T_FootR
		Lifespan 165
	End

	Event
		Type	Local
		At	T_Hair
		part2	CustomizeablePowers\PainDomination\HealingHeadGlows.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_UARML
		part1	CustomizeablePowers\PainDomination\HealingArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LARML
		part1	CustomizeablePowers\PainDomination\HealingArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_wepL
		part1	CustomizeablePowers\PainDomination\HealingArmGlows.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_UARMR
		part1	CustomizeablePowers\PainDomination\HealingArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LARMR
		part1	CustomizeablePowers\PainDomination\HealingArmMotionGlows.part
		part2	CustomizeablePowers\PainDomination\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_wepR
		part1	CustomizeablePowers\PainDomination\HealingArmGlows.part
		Lifespan 165
	End
End


Condition
	On	Time
	Time	80

	Event
		Type	Local
		At	T_Hips
		part2	CustomizeablePowers\PainDomination\LightRays.part
		lifespan 50
	End
End

Condition
	On	Time
	Time	100

	Event
		Type	Local
		At	T_Hips
		part1	CustomizeablePowers\PainDomination\HealingGlow.part
		Lifespan 165
	End
End

#############################################################

End