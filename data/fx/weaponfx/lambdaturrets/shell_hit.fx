#############################################################
## Turret_MuzzleFlash.fx
#############################################################

FxInfo
LifeSpan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors\CameraShake01Subtle.bhvr
		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	:Explosion_Flash_Large.part
		Part	:Explosion_Shockwave_Small.part
		Part	:Explosion_Streaks_Small.part
		Part	:Explosion_Fire_Rings_Large.part
		Part	:Explosion_Flash_Large_Flat.part
		Part	:Explosion_Glow_Lingering_Flat.part
		Part	:Explosion_Shockwave_Flat.part
		Part	:Explosion_Glow_Flat.part
		Sound LambdaTurret_Shell_Hit_01 300 300 .8
		Lifespan 4
	End

	Event
		Type	Local
		At	Origin
		Part	:Explosion_Streaks_Thin_Small.part
		Part	:Explosion_Sparks_Small.part
		Part	:Explosion_Smoke_Additive_Yellow.part
		Part	:Explosion_Smoke_Lingering.part
		Part	:Explosion_Smoke.part
		Part	:Explosion_Rocks.part
		Part	:Explosion_Smoke_Thin.part
		Part	:Explosion_Fire_Rings_Small.part
		Part	:Explosion_Flash.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Origin
		Part	:Coals_Flat_Additive.part
		Lifespan 1
	End

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:BurnPatch_Sparks.part
		Lifespan 30
	End
	Event
		Type	StartPositOnly
		At	Origin
		Bhvr	behaviors/CameraShake01.bhvr

		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Dust_Circle.part
		Part	:Dust_Circle_Lingering.part
		Part	:Dust_Shockwave.part
		Part	:Dust_Shockwave_Fill.part
		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 8.0 0.0
		End
		Part	:Dust_Pillar.part
		Part	:Dirt_Pillar.part

		POther	DustAnchor
		Lifespan	5
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	StartPositOnly
		At	Origin
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	5
			FadeOutLength	80
			StartColor	20 20 20
			Scale		40.0 10.0 40.0
		End
		Splat	BlurredOilSplatDark.tga
		Lifespan 80
	End

	Event
		Type	StartPositOnly
		At	Origin
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			PositionOffset	0.0 0.1 0.0
			Alpha		200
			FadeInLength	5
			FadeOutLength	80
			StartColor	20 20 20
			Scale		40.0 10.0 40.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 100
	End



End