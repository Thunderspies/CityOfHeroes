#############################################################
## Resurection.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Left
		Type	Local
		At	wepL
		Part1	Powers\Ability\Stamina2.part
		Part2	Powers\Ability\StaminaSmall2.part
		Part3	Powers\Ability\AbilityDots.part
		Part4	Powers\Ability\AbilitySparklingDots.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound heal6 70 70 .8
		Lifespan 80
	End

	Event
		EName	Right
		Type	Local
		At	wepR
		Part1	Powers\Ability\Stamina2.part
		Part2	Powers\Ability\StaminaSmall2.part
		Part3	Powers\Ability\AbilityDots.part
		Part4	Powers\Ability\AbilitySparklingDots.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
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
		Bhvr	Behaviors\teleportscale.bhvr
		Anim	FX_teleportbase
		Lifespan 120
	End

	Event
		EName 	BottleRocket02
		Type	Local
		At	BottleRocketHookup
		AnimPiv Waist
		Part1	Powers\Teleport\RessurectionBottleRocketSparks.part
		Part2	Powers\Teleport\BottleRocketHead.part
		Lifespan 120
	End

	Event
		EName 	BottleRocket03
		Type	Local
		At	BottleRocketHookup
		AnimPiv Neck
		Part1	Powers\Teleport\RessurectionBottleRocketSparks.part
		Part2	Powers\Teleport\BottleRocketHead.part
		Lifespan 120
	End

	Event
		EName 	BottleRocket04
		Type	Local
		At	BottleRocketHookup
		AnimPiv Head
		Part1	Powers\Teleport\RessurectionBottleRocketSparks.part
		Part2	Powers\Teleport\BottleRocketHead.part
		Lifespan 120
	End

	Event
		EName	Outerglow
		Type	Local
		At	T_Root
		Part1	Powers\Teleport\Sparklies.part
		Part2	Powers\Teleport\RingSparklies.part
		Part3	Powers\Teleport\SharpRingSparklies.part
		Part4	Powers\Teleport\streaks.part
		Part5	Powers\Teleport\Glowstreaks.part
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
		part1	Powers\Ability\StaminaRingUp.part
		#Sound	heal1 70 25 .8
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
		part1	Powers\Ability\StaminaRingUp.part
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
		part1	Powers\Ability\StaminaRingUp.part
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
		part1	POWERS\Healing\HealingBodyGlows.part
		part2	POWERS\Healing\HealingBodyGlowsFlat.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_ULEGL
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet Knee1
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LLEGL
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet T_FootL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_FootL
		part1	POWERS\Healing\HealingBodyGlows.part
		part2	POWERS\Healing\HealingBodyGlowsFlat.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_ULEGR
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet Kneer
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LLEGR
		part1	POWERS\Healing\HealingMotionGlows.part
		PMagnet T_FootR
		Lifespan 165
	End

	Event
		Type	Local
		At	T_Hair
		part2	POWERS\Healing\HealingHeadGlows.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_UARML
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LARML
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_wepL
		part1	POWERS\Healing\HealingArmGlows.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_UARMR
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LARMR
		part1	POWERS\Healing\HealingArmMotionGlows.part
		part2	POWERS\Healing\HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_wepR
		part1	POWERS\Healing\HealingArmGlows.part
		Lifespan 165
	End
End


Condition
	On	Time
	Time	80

	Event
		Type	Local
		At	T_Hips
		part2	POWERS\Healing\LightRays.part
		lifespan 50
	End
End

Condition
	On	Time
	Time	100

	Event
		Type	Local
		At	T_Hips
		part1	POWERS\Healing\HealingGlow.part
		Lifespan 165
	End
End

#############################################################

End