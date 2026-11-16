#############################################################
## GroundPound_Attack.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################


Condition
	On 	Time
	Time	25
	Event
		Type	Local
		At	Root
		Sound 	Sewer_ElectricBurstHigh_Loop 100 100 .8
		lifespan 55
	End
End


Condition
	On 	Time
	Time	0
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
	Time	30
	Event
		Type	Local
		At	Root
		Sound WW_MoveServo_01 100 100 .7
		Sound WW_MoveServo_02 100 100 .7
		Sound WW_MoveServo_03 100 100 .7
	End
End

Condition
	On 	Time
	Time	50
	Event
		Type	Local
		At	Root
		Sound WW_MoveServo_01 100 100 .7
		Sound WW_MoveServo_02 100 100 .7
		Sound WW_MoveServo_03 100 100 .7
	End
End

Condition
	On 	Time
	Time	70
	Event
		Type	Local
		At	Root
		Sound WW_MoveServo_01 100 100 .7
		Sound WW_MoveServo_02 100 100 .7
		Sound WW_MoveServo_03 100 100 .7
	End
End
Condition
	On 	Time
	Time	20
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
	Time	0


	Event
		EName	GroundAnchorL
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-0.5 0.5 4
		End
	End

	Event
		EName	GroundAnchorR
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	1.0 0.5 4
		End
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	StartPositOnly
		At	GroundAnchorL #WepL
		Bhvr	behaviors/CameraShake01.bhvr
		Part	ENEMYFX\Praetorian\Warworks\Sparks_Flash.part
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Circle_Lingering.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Shockwave_Fill.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Flash.part
		Part	:Sparks_Flash.part
		Part	:Electricity_Shockwave.part
		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	GroundAnchorL #WepL
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Sparks_Vertical.part
		Part	:Sparks_Long.part
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	:Overload_Electric.part
		Lifespan	8
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:GroundPOund_Electricity_CHILD.fx
		Lifespan	150
	End

End

Condition
	On	Time
	Time	40

	Event
		Type	StartPositOnly
		At	GroundAnchorR #WepR
		Bhvr	behaviors/CameraShake01.bhvr
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Circle_Lingering.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Shockwave_Fill.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Flash.part
		Part	:Sparks_Flash.part
		Part	:Electricity_Shockwave.part
		Lifespan	8
	End

	Event
		Type	StartPositOnly
		At	GroundAnchorR #WepR
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Sparks_Vertical.part
		Part	:Sparks_Long.part
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	:Overload_Electric.part
		Lifespan	8
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:GroundPOund_Electricity_CHILD.fx
		Lifespan	150
	End
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FFClose_01 75 75 .8
		Sound WW_FFClose_02 75 75 .8
		Sound WW_FFClose_03 75 75 .8
		Sound WW_FFClose_04 75 75 .8
		Sound WW_FFClose_05 75 75 .8
		Sound WW_FFClose_06 75 75 .8
		Sound WW_FFClose_07 75 75 .8
	End
End

Condition
	On	Time
	Time	61

	Event
		Type	StartPositOnly
		At	GroundAnchorL #WepL
		Bhvr	behaviors/CameraShake01.bhvr
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Shockwave_Fill.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Flash.part
		Part	:Sparks_Flash.part
		Part	:Electricity_Shockwave.part
		Lifespan	8
	End

	Event

		Type	start
		At	GroundAnchorL #WepL
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
		At	GroundAnchorL #WepL
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Sparks_Vertical.part
		Part	:Sparks_Long.part
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	:Overload_Electric.part
		Lifespan	8
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:GroundPOund_Electricity_CHILD.fx
		Lifespan	150
	End
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FFClose_01 75 75 .7
		Sound WW_FFClose_02 75 75 .7
		Sound WW_FFClose_03 75 75 .7
		Sound WW_FFClose_04 75 75 .7
		Sound WW_FFClose_05 75 75 .7
		Sound WW_FFClose_06 75 75 .7
		Sound WW_FFClose_07 75 75 .7
	End
End

Condition
	On	Time
	Time	80

	Event
		Type	StartPositOnly
		At	GroundAnchorR #WepR
		Bhvr	behaviors/CameraShake01.bhvr
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Shockwave_Fill.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Flash.part
		Part	:Sparks_Flash.part
		Part	:Electricity_Shockwave.part
		Lifespan	8
	End

	Event

		Type	start
		At	GroundAnchorR #WepR
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
		At	GroundAnchorR #WepR
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Sparks_Vertical.part
		Part	:Sparks_Long.part
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	:Overload_Electric.part
		Lifespan	8
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:GroundPOund_Electricity_CHILD.fx
		Lifespan	150
	End
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound WW_FFClose_01 75 75 .8
		Sound WW_FFClose_02 75 75 .8
		Sound WW_FFClose_03 75 75 .8
		Sound WW_FFClose_04 75 75 .8
		Sound WW_FFClose_05 75 75 .8
		Sound WW_FFClose_06 75 75 .8
		Sound WW_FFClose_07 75 75 .8
	End
End

#############################################################

End
