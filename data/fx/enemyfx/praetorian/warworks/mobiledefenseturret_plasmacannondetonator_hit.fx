#############################################################
## MobileDefenseTurret_PlasmaCannonDetonator_Attack.fx
#############################################################

FxInfo
LifeSpan 120

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Hips
		BhvrOverride
			PositionOffset	0.0 5.4 0.0
		End
		Part	:Explosion_Plasma_Flash.part
		Part	:Explosion_Plasma_Glow.part
		Part	:Explosion_Plasma_Shockwave.part
		Part	:Explosion_Plasma_Glow_Lingering.part
		Part	:Explosion_Flash_Streaks.part
		Lifespan 3
	End


	Event
		Type	Start
		At	Root
		Bhvr	behaviors\CameraShake03.bhvr

		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:Explosion_Plasma_Shockwave_Ground.part
		Part	:Explosion_Plasma_Glow_Ground.part
		Lifespan 3
	End

End

Condition
	On	Time
	Time	1

	Event
		Type	Start
		At	Root
		Part	:Explosion_Plasma_Fire.part
		Part	:Explosion_Plasma_Fire_Fill.part
		Part	:Explosion_Plasma_Fire_Ground.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	3

	Event
		EName	Sigil
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	5
			FadeOutLength	60
			StartColor	20 20 20
			Scale		40.0 10.0 40.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 120
	End

	Event
		Type	StartPositOnly
		At	Hips
		Part	:Plasma_Smoke_Lingering.part
		Part	:Plasma_Smoke.part
		Lifespan 1
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Start
		At	Hips
		Part	:Explosion_Plasma_Sparks_Lingering.part
		Part	:Explosion_Plasma_Sparks2_Lingering.part

		Lifespan 5
	End

End

#############################################################

End