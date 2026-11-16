#############################################################
## FX System Name
#############################################################

FxInfo
Lifespan 30

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		Sound maestro2 60 60 .33
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Hookup
		Type	PositOnly
		At	Chest
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
	End

	Event
		Type	Local
		At	Hookup
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		part1	:Buff_ContractingRing1.part
		Lifespan	15
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hookup
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		BhvrOverride
			PyrRotate	0 0 0
			PositionOffset	0 -3 0
		End
		part1	:Buff_SonicRings1.part
		Lifespan	15
	End

	Event
		Type	Local
		At	Hookup
		Bhvr	POWERS\SonicRessonance\SonicRing_Fade2.bhvr
		BhvrOverride
			PyrRotate	180 0 0
			PositionOffset	0 3 0
		End
		part1	:Buff_SonicRings1.part
		Lifespan	15
	End
End

#############################################################

End