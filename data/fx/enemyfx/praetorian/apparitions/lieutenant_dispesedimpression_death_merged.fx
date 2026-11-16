#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	Apparitions_Death_01 150 100 .9
		Sound 	Apparitions_Death_02 150 100 .9
		Sound 	Apparitions_Death_03 150 100 .9
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Explosion_Smoke_Death_Lieu_Merged.part
		Part	:Explosion_Smoke_Death_Minion_Merged.part
		Part	:Body_Explode_Flash_Merged.part
		Part	:Body_Explode_Shockwave_Merged.part
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
		ChildFX	:Minion_ScatteredSpirit_Explode_Merged.fx
		Lifespan 5
	End
End

#############################################################

End