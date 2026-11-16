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
		Bhvr	behaviors/WarpOut_Fade.bhvr
		Flags 	AdoptParentEntity
		Part	:Explosion_Flash.part
		Part	:Explosion_Flash_Center.part
		Part	:Explosion_Streaks.part
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
		Part	:Explosion_SparkCloud.part
		Part	:Explosion_Smoke_Lingering.part
		Part	:Explosion_Sparks.part
		Lifespan 8
	End

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Root
		Part	:Explosion_Smoke_Alpha.part
		Part	:Explosion_Fire.part
		Part	:Explosion_Smoke_Lingering_Full.part
		Part	:Explosion_Sparks.part
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
			StartColor	20 20 20
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
		ChildFX :Physics_Fire.fx
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
		ChildFX :Physics_Sparks.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End


#############################################################

End