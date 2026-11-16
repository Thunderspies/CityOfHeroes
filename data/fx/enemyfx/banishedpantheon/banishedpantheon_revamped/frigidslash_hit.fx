#
FxInfo
Lifespan 10

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPB_FrigidSlash_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0
	Random	1

	Event
		EName	HitLeft
		Type	Local
		At	LLegL
		Part	:Hit_Flash_Small.part
		Part	:Hit_Particles.part
		Part	:Hit_Shockwave_Small.part
		Part	:Hit_Boils_Small.part
		Part	:Hit_Boils_Solid_Small.part
		Lifespan 10
	End

	Event
		EName	HitRight
		Type	Local
		At	LLegR
		Part	:Hit_Flash_Small.part
		Part	:Hit_Particles.part
		Part	:Hit_Shockwave_Small.part
		Part	:Hit_Boils_Small.part
		Part	:Hit_Boils_Solid_Small.part
		Lifespan 10
	End

End

###########################################################

End