#########################################################
##	Template

FxInfo


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
		Part	:Body_Glow_01.part

	End

	Event
		EName	Inner_Beam_01
		Type	Local
		At	Offset_01
		Part	:Inner_Beam_01.part

	End

	Event
		EName 	Pr
		Type	Local 
		At	Offset_01
		Part1	:Caves_dust_outpour.part
		Part2	:Caves_dust_1.part
		Sound smallrock2 40 40 .46
	End

End
