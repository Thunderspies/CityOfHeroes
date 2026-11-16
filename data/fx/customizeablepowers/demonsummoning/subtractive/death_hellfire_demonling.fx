#############################################################
## Death_Fire_Demonling.fx
#############################################################

FxInfo
LifeSpan 190

#############################################################

Condition
	On 	Time
	Time 	11

	Event
		EName 	HitFX
		Type	Start
		At	Chest
		Flags 	AdoptParentEntity
		Part	:Explosion_Hellfire_Flash.part
		Part	:Explosion_Flash_Center.part
		Part	:Explosion_Hellfire_Streaks.part
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	13

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Hips
		Part	:Explosion_Hellfire_SparkCloud.part
		Part	:Explosion_Hellfire_Sparks.part
		Lifespan 8
	End

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Root
		Part	:Explosion_Hellfire_Alpha.part
		Part	:Explosion_Hellfire.part
		Part	:Explosion_Hellfire_Smoke_Lingering.part
		Part	:Explosion_Hellfire_Smoke_Lingering_Full.part
		Lifespan 8
	End

	Event
		EName	Sigil
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	5
			FadeOutLength	40
			StartColor	25 25 25
			PrimaryTint	10
			SecondaryTint	5
			Scale		10.0 10.0 10.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 190
	End

End

Condition
	On 	Time
	Time 	13
	Repeat 	2

	Event
	#	HardwareOnly
		Type 	Start
		At 	Chest
		ChildFX :Physics_Hellfire.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End

Condition
	On 	Time
	Time 	14
	Repeat 	12

	Event
	#	HardwareOnly
		Type 	Start
		At 	Chest
		ChildFX :Physics_Hellfire_Sparks.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End

#############################################################

End