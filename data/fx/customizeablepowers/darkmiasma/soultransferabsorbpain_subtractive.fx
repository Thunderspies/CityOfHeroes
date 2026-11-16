#############################################################
## SoulTransferAbsorbPain_Subtractive.fx
#############################################################

FxInfo
Lifespan 150

#########################################################

Input
	Inpname	Hips
End

#########################################################


Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime2
		Type	start
		At	origin

		Bhvr	CustomizeablePowers\DarkMiasma\SoulTransfer1.bhvr

		part	CustomizeablePowers\DarkMiasma\aFearMist.part

		Magnet	Target
		Lookat	Target

	End

	Event
		EName 	Prime
		Type	start
		At	origin

		Bhvr	CustomizeablePowers\DarkMiasma\SoulTransfer1.bhvr

		Part	CustomizeablePowers\DarkMiasma\PetrifingPuddleTrail_Subtractive.Part
		Part	CustomizeablePowers\DarkMiasma\aFearTenticles_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\aFearTenticles2_Subtractive.part
		part	CustomizeablePowers\DarkMiasma\Blast2.part
		part	CustomizeablePowers\DarkMiasma\aFearMist2.part
		Part	CustomizeablePowers\DarkMiasma\aFearTenticles.part
		Part	CustomizeablePowers\DarkMiasma\aFearTenticles2.part


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
		Part	CustomizeablePowers\DarkMiasma\ChillOfNightTendril_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\ChillOfNightTendril.part
		part	CustomizeablePowers\DarkMiasma\HealingHitMist.part
		part	CustomizeablePowers\DarkMiasma\DefenceHitMist2_Subtractive.part
		part	CustomizeablePowers\DarkMiasma\DefenceHitMist2.part
		Part	CustomizeablePowers\DarkMiasma\HealingGlow.part
		LifeSpan	120

	End

	Event
		ENAME   Tendrills
		Type	Local
		At	Target
		Part1	CustomizeablePowers\DarkMiasma\LightRays.part

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

		Part	CustomizeablePowers\DarkMiasma\SoulTransferRings.part
		part	CustomizeablePowers\DarkMiasma\DamageHitMist.part
		part	CustomizeablePowers\DarkMiasma\DefenceHitMist2.part
		sound	soultrans2 80 30 .85

		#LifeSpan	50
	End

	Event
		ENAME   Tendrills
		Type	start
		At	chest
		Part	CustomizeablePowers\DarkMiasma\ChillOfNightTendril_Subtractive.part
		Part	CustomizeablePowers\DarkMiasma\ChillOfNightTendril.part

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
		Part	CustomizeablePowers\DarkMiasma\SoulTransferRingFinal.part

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
		Part	CustomizeablePowers\DarkMiasma\SoulTransferRing1.part
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
		Part	CustomizeablePowers\DarkMiasma\SoulTransferRing1.part
		LifeSpan	30

	End


End


End
##################################

