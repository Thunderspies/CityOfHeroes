#############################################################
## Judgment_Ion.fx
#############################################################

FxInfo
LifeSpan	80

#############################################################

Input
	Inpname	Hips
End

#############################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound Abomination_Howl_01 150 150 .8
		Sound Abomination_Howl_02 150 150 .8
		Sound Abomination_Howl_03 150 150 .8
		Sound Abomination_Howl_04 150 150 .8
	End
End


Condition
	On 	Time
	Time	15
	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	Thunder_01 100 50 .9
		Sound 	Thunder_04 100 50 .9
		Sound 	Thunder_05 100 50 .9
		Sound 	Thunder_06 100 50 .9
	End
End

Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	Sewer_ElectricBurstMediumHigh_Loop 100 100 .5
		lifespan 20
	End
End


Condition
	On	Time
	Time	20

	Event
		Ename	Offset01
		Type	Start
		At	Origin
		BhvrOverride
			Startjitter	4 0 4
			PositionOffset	0 60 0
		End
	End

	Event
		Ename	Offset02
		Type	Start
		At	Origin
		BhvrOverride
			Startjitter	3 2 3
			PositionOffset	0 30 0
		End
	End

	Event
		Ename	Offset03
		Type	Local
		At	Origin

	End

	Event
		Ename	ChestAnchor
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0.5 1.0
		End
	End

#############################################################

	Event
		EName	Bolt1
		Type	Local
		At	Offset01
		Part	ENEMYFX\Praetorian\Talons\ChargedBolt_Flash.part
		Part	ENEMYFX\Praetorian\Talons\Beam_Bolt_Blur_Small.part
		Part	ENEMYFX\Praetorian\Talons\Beam_Bolt_Blur_Small2.part
		Part	ENEMYFX\Praetorian\Talons\Beam_Smoke_Small.part
		LookAt	Offset02
		Pother	Offset02
		Lifespan 5
	End
End

Condition
	On	Time
	Time	21

	Event
		EName	Bolt1
		Type	Local
		At	Offset02
		Part	ENEMYFX\Praetorian\Talons\ChargedBolt_Flash.part
		Part	ENEMYFX\Praetorian\Talons\Beam_Bolt_Blur_Small.part
		Part	ENEMYFX\Praetorian\Talons\Beam_Bolt_Blur_Small2.part
		Part	ENEMYFX\Praetorian\Talons\ChargedBolt_Flash.part
		Part	ENEMYFX\Praetorian\Talons\Beam_Smoke_Small.part
		LookAt	ChestAnchor
		Pother	ChestAnchor
		Lifespan 2
	End
End

Condition
	On	Time
	Time	22

	Event
		Type	Local
		At	ChestAnchor
		Part	ENEMYFX\Praetorian\Talons\Impact_Flash.part
		Part	ENEMYFX\Praetorian\Talons\Impact_Streaks.part
		Part	:Sparks_Burst.part
		Part	:Flash_Burst.part
		Part	:Electricity_Burst.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	24

	Event
		Type	Local
		At	UArmL
		Part	:Electricity_Arms.part
		Lifespan 5
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		Part	:Electricity_Arms.part
		Lifespan 5
		POther	LArmR

	End
End

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	LArmL
		Part	:Electricity_Arms.part
		Lifespan 30
		POther	HandL
	End

	Event
		Type	Local
		At	LArmR
		Part	:Electricity_Arms.part
		Lifespan 30
		POther	HandR

	End
End

Condition
	On	Time
	Time	33

	Event
		Type	Local
		At	WepR
		Part	:Electricity_Hands.part
		Part	:Electricity_Hands_Bright.part

		Lifespan 30
	End


	Event
		Type	Local
		At	WepL
		Part	:Electricity_Hands.part
		Part	:Electricity_Hands_Bright.part

		Lifespan 30
	End
End


#############################################################

End