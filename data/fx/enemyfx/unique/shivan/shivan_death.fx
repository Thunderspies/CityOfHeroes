#############################################################
## SeedHamidon_Aura.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Condition

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	Shivan_DeathVOX_01 30 30 .8
		Sound 	Shivan_DeathVOX_02 30 30 .8
		Sound 	Shivan_DeathVOX_03 30 30 .8
	End
End

Condition
	On Time
	Time 4
	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	Shivan_Death_01 50 50 .8
		Sound 	Shivan_Death_02 50 50 .8
		Sound 	Shivan_Death_03 50 50 .8
	End
End

Condition
	On 	Time
	Time 	4


	Event
		Type	PositOnly
		At	Chest
		Part	:Splash_Flash.part
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	PositOnly
		At	Chest
		Part	:TailSplash.part
		Part	:TailSplash_Hilite.part
		Part	:Splash_TailSplatter_Hilight.part
		Part	:Splash_TailSplatter.part
		Lifespan 10
	End

	Event
		EName 	Prime
		Type	PositOnly
		At	Chest
		BhvrOverride
			StartColor	255 50 50
		End
		Part	Generic\InfernorockBits_Large.part
		Part	Generic\InfernorockBits_Glowing.part
		LifeSpan	5
	End
End

#############################################################

End