#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	Hips
End

Flags    InheritAlpha


Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	Hips
		Part1	:flys.part
		Part	:Locusts.part
		Part	:FireFlys.part
		Part	:bees.part
		Part	:beesCoreMist.part
		Part	:beesCoreMist2.part
		Sound swarm2_loop 46 46 .32

	End
End



End			