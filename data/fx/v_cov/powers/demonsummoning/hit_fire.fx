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
		Sound Demon_CastHit_01 100 100 .8
		Sound Demon_CastHit_02 100 100 .8
		Sound Demon_CastHit_03 100 100 .8
		Part	:Explosion_Flash.part
		Part	:Explosion_Flash_Center.part
		Part	:Hit_Streaks.part
	End

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Chest
		Sound Demon_FireHit_01 100 100 .75
		Part	:Hit_Fire_Alpha.part
		Part	:Hit_Fire.part
		Part	:Hit_Smoke.part
		Part	:Hit_Sparks.part
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	3
	Repeat 	12

	Event
	#	HardwareOnly
		Type 	Start
		At 	Chest
		ChildFX :Physics_Sparks.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End

Condition
	On 	Time
	Time 	2
	Repeat 	5

	Event
	#	HardwareOnly
		Type 	Start
		At 	Chest
		ChildFX :Physics_Fire.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End
#############################################################

End