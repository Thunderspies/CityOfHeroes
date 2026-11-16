#
FxInfo
Lifespan 6

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPB_FrigidQuills_Hit_01 100 100 .8
	End
End
Condition
	On	Time
	Time	0

	Random	1

	Event
		EName	HitChest
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
		Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part
		Part	:Hit_Boils_Small.part
		Lifespan 10
	End

	Event
		EName	HitLeftLeg
		Type	Local
		At	LLegL
		Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
		Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part

		Part	:Hit_Boils_Small.part
		Lifespan 10
	End


	Event
		EName	HitLeftArm
		Type	Local
		At	LArmR
		Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
		Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part
		Part	:Hit_Boils_Small.part
		Lifespan 10
	End

End

Condition
	On	Cycle
	Time	3
	Random	1

	Event
		EName	HitRightArm
		Type	Local
		At	LArmL
		Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
		Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part

		Part	:Hit_Boils_Small.part
		Lifespan 10
	End

	Event
		EName	HitRightLeg
		Type	Local
		At	LLegR
		Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
		Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part

		Part	:Hit_Boils_Small.part
		Lifespan 10
	End
End

###########################################################

End