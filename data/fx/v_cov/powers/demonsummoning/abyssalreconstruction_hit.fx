#############################################################
## RuneCircle_Enchant.fx
#############################################################

FxInfo
Lifespan 200

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	HitFX
		Type	PositOnly
		At	Root
		Sound Demon_AbyssalRecon_Hit 100 100 .9
		Part	:Heal_Shockwave_Shrinking.part
		Part	:Heal_Shockwave_Shrinking_Fast.part

		Lifespan 12
	End
End

Condition
	On	Time
	Time	10

	Event
		EName 	HitFX
		Type	PositOnly
		At	Root
		Part	:AbyssalReconstruction_Fire_Geyser.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	14

	Event
		EName 	HitFX
		Type	PositOnly
		At	Root
		Part	:AbyssalReconstruction_Sparks_Upward.part
		Part	:AbyssalReconstruction_Smoke_Geyser.part
		Lifespan 7
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers\Ability\StaminaRingUp.part
		#Sound	heal1 70 25 .8
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers\Ability\StaminaRingUp.part
	End
End

Condition
	On	Time
	Time	25

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	Powers\Ability\StaminaRingUp.part
	End
End

#########################################################

End