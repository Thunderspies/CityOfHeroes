#############################################################
## FusionGrenade_Hit.fx
#############################################################

FxInfo
LifeSpan 340

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		# Sound WW_ChemGrenade_Hit 200 200 1
	End

	Event
		Type	Local
		At	Root
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
		Lifespan 4
	End

	Event
		Type	Local
		At	Root
		Part	:Explosion_Streaks_Thin_Small.part
		Part	:Explosion_Sparks_Small.part
		Part	:Explosion_Smoke_Additive_Yellow.part
		Part	:Explosion_Smoke_Lingering.part
		Part	:Explosion_Rocks.part
		Part	:Explosion_Smoke_Thin.part
		Part	:Explosion_Fire_Rings_Small.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Root
		Part	:Coals_Flat_Additive.part
		Lifespan 300
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:BurnPatch_Sparks.part
		Lifespan 300
	End

End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Explosion_Fire.part
		Part	:Explosion_Fire_Base.part
		Lifespan 300
	End

	Event
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	5
			FadeOutLength	40
			StartColor	20 20 20
			Scale		40.0 10.0 40.0
		End
		Splat	BlurredOilSplatDark.tga
		Lifespan 320
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			PositionOffset	0.0 0.1 0.0
			Alpha		255
			FadeInLength	5
			FadeOutLength	40
			StartColor	20 20 20
			Scale		40.0 10.0 40.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 340
	End
End

Condition
	On	Time
	Time	0
	Repeat	15
	Event
		Type	Local
		At	Root
		ChildFX	:Physics_Sparks.fx
		Lifespan 60
	End
End

Condition
	On	Time
	Time	0
	Repeat	5

	Event
		Type	Local
		At	Root
		ChildFX	:Physics_Rocks.fx
		Lifespan 90
	End
End
#############################################################

End