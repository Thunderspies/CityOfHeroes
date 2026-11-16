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
		Ename	Offset01
		Type	Start
		At	Origin
		BhvrOverride
			Startjitter	8 0 8
			PositionOffset	0 30 0
		End
	End

	Event
		Ename	Offset02
		Type	Start
		At	Origin
		BhvrOverride
			Startjitter	6 2 6
			PositionOffset	0 15 0
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
		Part	:ChargedBolt_Flash.part
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Beam_Smoke_Small.part
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
		Part	:ChargedBolt_Flash.part
		Part	:Beam_Bolt_Blur_Small.part
		Part	:Beam_Bolt_Blur_Small2.part
		Part	:Beam_Smoke_Small.part
		LookAt	Chest
		Pother	Chest
		Lifespan 2
	End
End


#############################################################

End