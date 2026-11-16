###########################################################

FxInfo

###########################################################

Condition
	On	Cycle
	Time	5
	Random	1
Event
		EName	HitChest
		Type	Local
		At	Chest
	#	Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
	#	Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part
		Part	:Hit_Boils_Small.part
		Lifespan 10
	End

	Event
		EName	HitLeftArm
		Type	Local
		At	LArmR
	#	Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
	#	Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part
		Part	:Hit_Boils_Small.part
		Lifespan 10
	End

	Event
		EName	HitRightArm
		Type	Local
		At	LArmL
	#	Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
	#	Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part

		Part	:Hit_Boils_Small.part
		Lifespan 10
	End

	Event
		EName	HitLeftLeg
		Type	Local
		At	LLegL
	#	Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
	#	Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part

		Part	:Hit_Boils_Small.part
		Lifespan 10
	End

	Event
		EName	HitRightLeg
		Type	Local
		At	LLegR
	#	Part	:Hit_Flash_Small.part
		Part	:Hit_Particles_Small.part
	#	Part	:Hit_Shockwave_Small.part
		Part	:Hit_Sparks.part

		Part	:Hit_Boils_Small.part
		Lifespan 10
	End
End

###########################################################

End