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
	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound 	ToV_LightningStorm_Pet_01 100 100 .8
		Sound 	ToV_LightningStorm_Pet_02 100 100 .8
		Sound 	ToV_LightningStorm_Pet_03 100 100 .8
		Sound 	ToV_LightningStorm_Pet_04 100 100 .8
		Sound 	ToV_LightningStorm_Pet_05 100 100 .8
	End

	Event
		Ename	Offset01
		Type	Start
		At	Origin
		BhvrOverride
			Startjitter	8 0 8
			PositionOffset	0 40 0
		End
	End

	Event
		Ename	Offset02
		Type	Start
		At	Origin
		BhvrOverride
			Startjitter	6 6 6
			PositionOffset	0 20 0
		End
	End

	Event
		Ename	Offset03
		Type	Local
		At	Origin

	End

#############################################################

	Event
		EName	Bolt1
		Type	Local
		At	Offset01
		Part	:ChargedBolt_Lingering2.part
		Part	:ChargedBolt_Flash.part
		Part	:ChargedBolt_Flash_Lingering.part
		Part	:Beam_Smoke.part
		Part	:ChargedBolt_Lingering.part
		LookAt	Offset02
		Pother	Offset02
		Lifespan 5
	End
End

Condition
	On	Time
	Time	1
	Event
		EName	Bolt1
		Type	Local
		At	Offset02

		Part	:ChargedBolt_Lingering2.part
		Part	:ChargedBolt_Flash.part
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur_Small2.part

		Part	:ChargedBolt_Flash_Lingering.part
		Part	:Beam_Smoke.part
		Part	:ChargedBolt_Lingering.part
		LookAt	Offset03
		Pother	Offset03
		Lifespan 2
	End
End

Condition
	On	Time
	Time	4

	Event
		EName	Bolt1
		Type	Local
		At	Offset03
		Part	:Impact_Flash.part
		Part	:Impact_ElectricRing.part
		Part	:Impact_Sparks.part
		Part	:Impact_Streaks.part
		Part	:Impact_Glow_Flat.part
		Lifespan 30
	End
End

#############################################################

End