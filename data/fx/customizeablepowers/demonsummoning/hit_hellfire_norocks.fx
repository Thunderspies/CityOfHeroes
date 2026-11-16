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
		Part	:Explosion_Hellfire_Flash.part
		Part	:Explosion_Flash_Center.part
		Part	:Hit_Hellfire_Streaks.part
	End

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Chest
		Part	:Hit_Hellfire_Alpha.part
		Part	:Hit_Hellfire.part
		Part	:Hit_Hellfire_Smoke.part
		Part	:Hit_Hellfire_Sparks.part
		Lifespan 5
	End
End

#############################################################

End