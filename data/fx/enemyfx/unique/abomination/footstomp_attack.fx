#############################################################
## FootStomp_Attack.fx
#############################################################

FxInfo
LifeSpan 100

#############################################################

Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound WW_MoveServo_01 150 150 .5
		Sound WW_MoveServo_02 150 150 .5
		Sound WW_MoveServo_03 150 150 .5
	End
End

Condition
	On 	Time
	Time	34
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FF_01 200 200 .65
		Sound WW_FF_02 200 200 .65
		Sound WW_FF_03 200 200 .65
		Sound WW_FF_04 200 200 .65
		Sound WW_FF_05 200 200 .65
		Sound WW_FF_06 200 200 .65
		Sound WW_FF_07 200 200 .65
	End
End

Condition
	On	Time
	Time	31

	Event
		Type	StartPositOnly
		At	FootR
		Bhvr	behaviors/CameraShake01.bhvr
		BhvrOverride
			PositionOffset	0.0 -4.5 0.0
		End
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Circle_Lingering.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Shockwave_Large.part
		Part	:Dust_Shockwave_Huge.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Shockwave_Fill.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Flash.part
		Part	ENEMYFX\Praetorian\Warworks\Sparks_Flash.part
		Lifespan	8
	End

	Event
		Type	start
		At	FootR
		Bhvr	behaviors\Dust_Expanding.bhvr
		Part	:Dust_Expanding.part

		Lifespan	150
	End

	Event

		Type	start
		At	FootR
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
		At	FootR
		BhvrOverride
			PositionOffset	0.0 -6.5 0.0
		End
		Part	ENEMYFX\Praetorian\Warworks\Sparks_Vertical.part
		Part	ENEMYFX\Praetorian\Warworks\Sparks_Long.part
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part

		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	FootR
		BhvrOverride
			PositionOffset	0.0 3.5 0.0
		End
		Part	ENEMYFX\Praetorian\Warworks\Dust_Pillar.part
		Part	ENEMYFX\Praetorian\Warworks\Dirt_Pillar.part

		POther	DustAnchor
		Lifespan	5
	End
End

#############################################################

End
