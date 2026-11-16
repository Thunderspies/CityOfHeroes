#########################################################
##	Template

FxInfo

LifeSpan		20

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset_01
		Type	Local
		At	Hips
		Bhvr	:Offset_01.bhvr

	End

	Event
		EName	Body_01
		Type	Local
		At	Offset_01
		Part	:Body_Glow_Summon.part
		LifeSpan		20

	End
	
	Event
		EName	Body_01
		Type	Local
		At	Offset_01
		Part	:Body_Glow_SummonShine.part
		LifeSpan		5

	End

	Event
		EName	Inner_Beam_01
		Type	Local
		At	Offset_01
		Part	:Inner_Beam_Summon.part
		LifeSpan		20
	End

End
