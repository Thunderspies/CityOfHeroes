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
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound heal6 70 70 .8
		Lifespan 80
	End

	Event
		EName	Right
		Type	Local
		At	wepR
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
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
		Bhvr	CustomizeablePowers\Empathy\teleportscale.bhvr
		Anim	FX_teleportbase
		Lifespan 120
	End

	Event
		EName 	BottleRocket02
		Type	Local
		At	BottleRocketHookup
		AnimPiv Waist
		Part1	:RessurectionBottleRocketSparks.part
		Part2	:BottleRocketHead.part
		Lifespan 120
	End

	Event
		EName 	BottleRocket03
		Type	Local
		At	BottleRocketHookup
		AnimPiv Neck
		Part1	:RessurectionBottleRocketSparks.part
		Part2	:BottleRocketHead.part
		Lifespan 120
	End

	Event
		EName 	BottleRocket04
		Type	Local
		At	BottleRocketHookup
		AnimPiv Head
		Part1	:RessurectionBottleRocketSparks.part
		Part2	:BottleRocketHead.part
		Lifespan 120
	End

	Event
		EName	Outerglow
		Type	Local
		At	T_Root
		Part1	:Sparklies.part
		Part2	:RingSparklies.part
		Part3	:SharpRingSparklies.part
		Part4	:streaks.part
		Part5	:Glowstreaks.part
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
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part
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
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part
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
		part1	CustomizeablePowers\Empathy\StaminaRingUp.part
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
		part1	:HealingBodyGlows.part
		part2	:HealingBodyGlowsFlat.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_ULEGL
		part1	:HealingMotionGlows.part
		PMagnet Knee1
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LLEGL
		part1	:HealingMotionGlows.part
		PMagnet T_FootL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_FootL
		part1	:HealingBodyGlows.part
		part2	:HealingBodyGlowsFlat.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_ULEGR
		part1	:HealingMotionGlows.part
		PMagnet Kneer
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LLEGR
		part1	:HealingMotionGlows.part
		PMagnet T_FootR
		Lifespan 165
	End

	Event
		Type	Local
		At	T_Hair
		part2	:HealingHeadGlows.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_UARML
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LARML
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_wepL
		part1	:HealingArmGlows.part
		Lifespan 165
	End

	Event
		Type	Local
		At	T_UARMR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet ElbowL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_LARMR
		part1	:HealingArmMotionGlows.part
		part2	:HealingArmMotionGlows2.part
		PMagnet HandL
		Lifespan 165
	End

	Event
		Type	Local
		At	T_wepR
		part1	:HealingArmGlows.part
		Lifespan 165
	End
End


Condition
	On	Time
	Time	80

	Event
		Type	Local
		At	T_Hips
		part2	:LightRays.part
		lifespan 50
	End
End

Condition
	On	Time
	Time	100

	Event
		Type	Local
		At	T_Hips
		part1	:HealingGlow.part
		Lifespan 165
	End
End

#############################################################

End