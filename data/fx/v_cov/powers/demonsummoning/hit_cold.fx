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
		Part	:Explosion_Cold_Flash_Center.part
		Part	:Hit_Cold_Streaks.part
	End

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Chest
		Sound Demon_IceHit_01 100 100 .5
		Part	:Hit_Cold_Alpha.part
		Part	:Hit_Cold.part
		Part	:Hit_Cold_Mist.part
		Part	:Hit_Cold_Sparks.part
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
		ChildFX :Physics_Cold_Sparks.fx
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
		ChildFX :Physics_Ice.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End
#############################################################

End