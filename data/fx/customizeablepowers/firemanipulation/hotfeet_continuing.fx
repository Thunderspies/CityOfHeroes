#############################################################
## HotFeet_Continuing.FX
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	local
		At	root
		Part5	CustomizeablePowers\FireManipulation\Mediumfire_Smoke.part
		Part1	CustomizeablePowers\FireManipulation\Mediumfire_flame_A.part
		Part2	CustomizeablePowers\FireManipulation\Mediumfire_flame_B.part
		Part3	CustomizeablePowers\FireManipulation\Mediumfire_flame_Flat.part
		Part4	CustomizeablePowers\FireManipulation\spark1.part
	End
End

#########################################################

End