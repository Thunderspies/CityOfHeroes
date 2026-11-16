#############################################################
## SoulTransferAbsorbPain_Subtractive.fx
#############################################################

FxInfo
Lifespan 150

########################################################

Input
	Inpname	Hips
End

########################################################

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime2
		Type	start
		At	origin

		Bhvr	CustomizeablePowers\DarkArmor\SoulTransfer1.bhvr

		part1	CustomizeablePowers\DarkArmor\aFearMist.part

		Magnet	Target
		Lookat	Target

	End

	Event
		EName 	Prime
		Type	start
		At	origin

		Bhvr	CustomizeablePowers\DarkArmor\SoulTransfer1.bhvr

		Part	CustomizeablePowers\DarkArmor\PetrifingPuddleTrail_Subtractive.Part
		Part3	CustomizeablePowers\DarkArmor\aFearTenticles_Subtractive.part
		Part4	CustomizeablePowers\DarkArmor\aFearTenticles2_Subtractive.part
		part1	CustomizeablePowers\DarkArmor\Blast2.part
		part2	CustomizeablePowers\DarkArmor\aFearMist2.part
		Part3	CustomizeablePowers\DarkArmor\aFearTenticles.part
		Part4	CustomizeablePowers\DarkArmor\aFearTenticles2.part


		Magnet	Target
		Lookat	Target

	End



End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy

	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		ENAME   Tendrills
		Type	Local
		At	Target
		Part	CustomizeablePowers\DarkArmor\ChillOfNightTendril_Subtractive.part
		Part1	CustomizeablePowers\DarkArmor\ChillOfNightTendril.part
		part2	CustomizeablePowers\DarkArmor\HealingHitMist.part
		part3	CustomizeablePowers\DarkArmor\DefenceHitMist2_Subtractive.part
		part3	CustomizeablePowers\DarkArmor\DefenceHitMist2.part
		Part4	CustomizeablePowers\DarkArmor\HealingGlow.part
		LifeSpan	120

	End

	Event
		ENAME   Tendrills
		Type	Local
		At	Target
		Part1	CustomizeablePowers\DarkArmor\LightRays.part

		LifeSpan	50

	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName	dummy
		Type	Local
		At	chest

		Part1	CustomizeablePowers\DarkArmor\SoulTransferRings.part
		part2	CustomizeablePowers\DarkArmor\DamageHitMist.part
		part3	CustomizeablePowers\DarkArmor\DefenceHitMist2.part
		sound	soultrans2 80 30 .85

		#LifeSpan	50
	End

	Event
		ENAME   Tendrills
		Type	start
		At	chest
		Part1	CustomizeablePowers\DarkArmor\ChillOfNightTendril_Subtractive.part
		Part1	CustomizeablePowers\DarkArmor\ChillOfNightTendril.part


	End

End

Condition
	On 	cycle
	Time 	18
	Chance	.6

	Event
		EName	dummy2
		Type	Local
		At	Root
		Geom	RootToChestAdjustment
		LifeSpan	50
	End

	Event
		EName	dummy5
		Type	start
		At	dummy2
		altpiv	1
		Part1	CustomizeablePowers\DarkArmor\SoulTransferRingFinal.part
		LifeSpan	50
	End


End

Condition
	On 	cycle
	Time 	14
	Chance	.3


	Event
		EName	dummy3
		Type	Local
		At	chest
		Part1	CustomizeablePowers\DarkArmor\SoulTransferRing1.part
		LifeSpan	20

	End


End

Condition
	On 	cycle
	Time 	28
	Chance	.6

	Event
		EName	dummy4
		Type	Local
		At	chest
		Part1	CustomizeablePowers\DarkArmor\SoulTransferRing1.part
		LifeSpan	30

	End


End

##################################

End

