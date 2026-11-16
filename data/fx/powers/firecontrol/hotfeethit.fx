#########################################################
## Fire Control - Hot Feet Hit FX
#########################################################

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
		Part1	:Mediumfire_flame_A.part
		Part2	:Mediumfire_flame_B.part
		Part3	:Mediumfire_flame_Flat.part
		Part4	WORLD/Fire/spark1.part
		Part5	:Mediumfire_Smoke.part
	End
End

#########################################################

End