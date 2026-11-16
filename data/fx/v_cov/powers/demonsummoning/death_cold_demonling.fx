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
		Type	Local
		At	Chest
		Bhvr	behaviors/WarpOut_Fade.bhvr
		Flags 	AdoptParentEntity
		Part	:Explosion_Cold_Flash.part
		Part	:Explosion_Cold_Flash_Center.part
		Part	:Explosion_Cold_Streaks.part
		Part	:Explosion_Mist_Full.part
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
		Part	:Explosion_Cold_SparkCloud.part
		Part	:Explosion_Cold_Mist_Lingering.part
		Part	:Explosion_Cold_Sparks.part
		Lifespan 12
	End

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Root
		Part	:Explosion_Cold_Fire.part
		Lifespan 12
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
			StartColor	70 100 120
			Scale		10.0 10.0 10.0
			SplatFlags	ADDITIVE
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
		ChildFX :Physics_Ice.fx
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
		ChildFX :Physics_Cold_Sparks.fx
		Lifespan 120
		Lifespanjitter 	20
	End
End

#############################################################

End