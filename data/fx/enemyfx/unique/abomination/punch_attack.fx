#############################################################
## GroundPound_Attack.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################


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
	Time	30
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
	Event
		Type	StartPositOnly
		At	Mystic
		Bhvr	behaviors/CameraShake01.bhvr
		Part	ENEMYFX\Praetorian\Warworks\Sparks_Flash.part
		Part	ENEMYFX\Praetorian\Warworks\Rocks.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Circle_Lingering.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Shockwave_Fill.part
		Part	ENEMYFX\Praetorian\Warworks\Dust_Flash.part
		Part	:Sparks_Flash.part
		Lifespan	8
	End
End

#############################################################

End
