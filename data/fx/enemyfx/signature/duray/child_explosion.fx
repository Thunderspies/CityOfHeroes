#############################################################
## Inferno.fx
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	StartPositOnly
		At	Origin
		Bhvr	behaviors/CameraShake02.bhvr
		BhvrOverride
			PositionOffset	0 -3.5 0

		End
		Part	:Dirt_Splatter.part
		Part	:Flash_Glow_Large.part
		Part	:Flash_Sparks.part
		Part	:Dust_Spread.part
		Part	:Sparks_Impact.part
		POther	DustAnchor
		Lifespan	5
	End

End

Condition
	On	Time
	Time	0

	Event
		Type	StartPositOnly
		At	Origin
		bhvr    behaviors/Braz/ExpandRingFast.bhvr
		BhvrOverride
			PositionOffset	0 -3.5 0
		End
		Part	:Dust_Shockwave3.part
		Part	:Dust_Pillar_Large.part
		Lifespan 2
	End

	Event
		Type	StartPositOnly
		At	Root

		Bhvr	behaviors\Braz\Splat_ExplosiveResidue.bhvr
		BhvrOverride
			Alpha		255
			StartColor	200 200 200
			Scale		15.0 40.0 15.0
		End
		Splat	ExplosiveResidue.tga
		Lifespan 160
	End

End

Condition
	On	Time
	Time	5

	Event
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0 -3.5 0
		End
		Part	:Dust_Lingering_Cloud.part
		Lifespan 40
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0 -3.5 0
		End

		Part	:Dust_Pillar_Large_Secondary.part
		Part	:Dirt_Splatter.part
		Part	:Rock_Chunks.part

		POther	DustAnchor
		Lifespan	5
	End
	Event
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0 10 0
		End
		Part	:Dust_Falling.part
	End

	Event
		Type	StartPositOnly
		At	Origin

		#Part	:Dust_RockBreak.part

		Lifespan	5
	End
End
#############################################################

End