#############################################################
## SoulTransferAbsorbPain.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Root
End

#############################################################



Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime2
		Type	start
		At	origin

		Bhvr	behaviors/SoulTransfer1.bhvr

		part1	Powers/DarknessControl/aFearMist.part

		Magnet	Target
		Lookat	Target

	End

	Event
		EName 	Prime
		Type	start
		At	origin

		Bhvr	behaviors/SoulTransfer1.bhvr

		Part5	Powers/DarknessControl/PetrifingPuddleTrail.Part
		part1	Powers/Darknesscontrol/Blast2.part
		part2	Powers/DarknessControl/aFearMist2.part
		Part3	Powers/DarknessControl/aFearTenticles.part
		Part4	Powers/DarknessControl/aFearTenticles2.part


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
		Part1	Powers/DarknessControl/ChillOfNightTendril.part
		part2	Powers/DarknessControl/HealingHitMist.part
		part3	Powers/DarknessControl/DefenceHitMist2.part
		Part4	Powers/DarknessControl/HealingGlow.part
		LifeSpan	120

	End

	Event
		ENAME   Tendrills
		Type	Local
		At	Target
		Part1	Powers/DarknessControl/LightRays.part

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

		Part1	Powers/DarknessControl/SoulTransferRings.part
		part2	Powers/DarknessControl/DamageHitMist.part
		part3	Powers/DarknessControl/DefenceHitMist2.part
		sound	soultrans2 80 30 .85

		#LifeSpan	50
	End

	Event
		ENAME   Tendrills
		Type	start
		At	chest
		Part1	Powers/DarknessControl/ChillOfNightTendril.part


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
		Part1	Powers/DarknessControl/SoulTransferRingFinal.part
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
		Part1	Powers/DarknessControl/SoulTransferRing1.part
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
		Part1	Powers/DarknessControl/SoulTransferRing1.part
		LifeSpan	30

	End


End

Condition
	On 	Time
	Time 	150

	Event
		EName	all
		Type	Destroy

	End


End



End
##################################

