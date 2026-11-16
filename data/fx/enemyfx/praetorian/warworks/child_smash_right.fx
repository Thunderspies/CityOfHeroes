#############################################################
## Smash_Activate.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################


Condition
	On	Time
	Time	46

	Event
		EName	DustAnchor
		Type	StartPositOnly
		At	WepR
		BhvrOverride
			PositionOffset	0.0 30.0 0.0
		End
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
		Bhvr	behaviors/CameraShake01.bhvr

		BhvrOverride
			PositionOffset	0.0 -2.5 0.0
		End
		Part	:Dust_Circle.part
		Part	:Dust_Circle_Lingering.part
		Part	:Dust_Shockwave.part
		Part	:Dust_Shockwave_Fill.part
		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	Mystic
		Bhvr	behaviors/CameraShake01.bhvr
		Part	:Dust_Flash.part
		Part	:Sparks_Flash.part
		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	Mystic
		BhvrOverride
			PositionOffset	0.0 -2.0 0.0
		End

		Part	:Sparks_Long.part
		Part	:Rocks.part

		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	WepR
		Part	:Dust_Pillar.part
		Part	:Dirt_Pillar.part
		POther	DustAnchor
		Lifespan	5
	End
End

#############################################################

End
