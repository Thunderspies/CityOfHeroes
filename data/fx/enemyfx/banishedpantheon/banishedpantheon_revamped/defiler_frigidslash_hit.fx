#
FxInfo

Lifespan 60
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
		On Time
		Time 0

		Event
			EName	HitLeft
			Type	Local
			At	Chest
			Part	:Hit_Flash_Small.part
			Part	:Hit_Particles.part
			Part	:Hit_Shockwave_Small.part
			Part	:Hit_Boils_Small.part
			Part	:Hit_Boils_Solid_Small.part
			Lifespan 10
		End

		Event
			At Chest
			Type StartPositOnly
			Part :Hit_Flash.part
		#	Part :Hit_Slash.part
			LifeSpan 10
		End
	End

End
