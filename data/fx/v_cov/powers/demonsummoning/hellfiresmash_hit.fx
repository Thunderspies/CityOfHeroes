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
		Part	:Explosion_Hellfire_Flash_Big.part
		Part	:Explosion_Hellfire_Embers_Big.part
		Part	:Explosion_Flash_Center.part
		Part	:Hit_Hellfire_Streaks.part
	End

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Chest
		Part	:Hit_Hellfire_Alpha.part
		Part	:Hit_Hellfire.part
		Part	:Hit_Hellfire.part
		Part	:Hit_Hellfire_Smoke.part
		Part	:Hit_Hellfire_Sparks.part
		Part	:Hit_Hellfire_Sparks_Lingering.part

		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	2
	Repeat 	4

	Event
	#	HardwareOnly
		Type 	Start
		At 	Chest
		ChildFX :Physics_Hellfire.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End

#############################################################

End