#############################################################
## Smash_Activate.fx
#############################################################

FxInfo
LifeSpan 100

#############################################################

Condition 
	On Time
	Time 10
	Event
		Type Local
		At Hips
		Sound WW_GroundSlam_Attack 200 200 .5
	End
End
		
Condition
	On	Time
	Time	84

	Event
		EName	DustAnchor
		Type	StartPositOnly
		At	Mystic
		BhvrOverride
			PositionOffset	0.0 30.0 0.0
		End
	End

	Event
		Type	StartPositOnly
		At	Mystic
		Bhvr	behaviors/CameraShake01.bhvr
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Dust_Circle_Large.part
		Part	:Dust_Circle_Lingering.part
		Part	:Dust_Shockwave_Large.part
		Part	:Dust_Shockwave_Fill.part
		Part	:Dust_Flash.part
		Part	:Sparks_Flash.part
		Lifespan	8
	End

	Event
		Type	start
		At	Mystic
		Bhvr	behaviors\Splat_Cracks.bhvr
		BhvrOverride
			Alpha		255
			PositionOffset	0 0.05 0
			Scale		20.0 20.0 20.0
			PyrRotate	0 180 0
		End
		Splat	ImpactCracks.tga
		Lifespan	150
	End

	Event
		Type	StartPositOnly
		At	Mystic
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Sparks_Vertical.part
		Part	:Sparks_Long.part
		Part	:Rocks.part

		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	Mystic
		BhvrOverride
			PositionOffset	0.0 8.0 0.0
		End
		Part	:Dust_Pillar.part
		Part	:Dirt_Pillar.part

		POther	DustAnchor
		Lifespan	5
	End
End

#############################################################

End
