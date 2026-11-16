#############################################################
## FootStomp_Attack.fx
#############################################################

FxInfo
LifeSpan 60

#############################################################

Condition
	On 	Time
	Time	1
	Event
		Type	Local
		At	Root
		Sound ShivanGiant_TauntShort_01 300 150 .8
		Sound ShivanGiant_TauntShort_02 300 150 .8
		Sound ShivanGiant_TauntShort_03 300 150 .8
	End
End

Condition
	On 	Time
	Time	35
	Event
		Type	Local
		At	Origin
		Sound Hamidon_Crush_01 300 300 .9
		Sound Hamidon_Crush_02 300 300 .9
		Sound Hamidon_Crush_03 300 300 .9
	End

End

Condition
	On	Time
	Time	37

	Event
		EName	DustAnchor
		Type	StartPositOnly
		At	WepR
		BhvrOverride
			PositionOffset	0.0 30.0 0.0
		End
	End

	Event
		Type	StartPositOnly
		At	WepR
		Bhvr	behaviors/CameraShake_Strong.bhvr
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	:Dust_Circle_Lingering.part
		Part	:Dust_Shockwave_Huge.part
		Part	:Dust_Shockwave_Fill.part
		Part	:Dust_Flash.part
		Part	ENEMYFX\Praetorian\Warworks\Sparks_Flash.part
		Lifespan	8
	End

	Event
		Type	start
		At	WepR
		Bhvr	behaviors\Dust_Expanding.bhvr
		Part	:Dust_Expanding.part

		Lifespan	150
	End

	Event

		Type	start
		At	WepR
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
		At	WepR
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	ENEMYFX\Praetorian\Warworks\Sparks_Vertical.part
		Part	ENEMYFX\Praetorian\Warworks\Sparks_Long.part
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part

		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	WepR
		BhvrOverride
			PositionOffset	0.0 8.0 0.0
		End
		Part	:Dust_Pillar.part
		Part	:Dirt_Pillar.part

		POther	DustAnchor
		Lifespan	2
	End
End

#############################################################

End
