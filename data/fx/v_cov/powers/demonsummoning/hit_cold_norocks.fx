#############################################################
## Death_Fire_Demonling.fx
#############################################################

FxInfo
LifeSpan 190

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	HitFX
		Type	Start
		At	Chest
		Part	:Explosion_Cold_Flash_Center.part
		Part	:Hit_Cold_Streaks.part
	End

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Chest
		Part	:Hit_Cold_Alpha.part
		Part	:Hit_Cold.part
		Part	:Hit_Cold_Mist.part
		Part	:Hit_Cold_Sparks.part
		Lifespan 5
	End
End

#############################################################

End