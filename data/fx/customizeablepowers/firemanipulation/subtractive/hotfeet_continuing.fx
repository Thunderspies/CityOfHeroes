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
		Part5	:Mediumfire_Smoke.part
		Part1	:Mediumfire_flame_A.part
		Part2	:Mediumfire_flame_B.part
		Part3	:Mediumfire_flame_Flat.part
		Part3	:Mediumfire_flame_Flat_Alpha.part

		Part4	:spark1.part
	End
End

#########################################################

End