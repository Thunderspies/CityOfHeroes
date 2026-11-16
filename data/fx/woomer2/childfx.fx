#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Origin
End
Input	
	InpName	Target
End

LifeSpan 200

Condition
	Event
		Type	Start
		At	Origin
		Part1	:smoke.part
		Part3	:Flash.part
		Part5	:FlatFlash.part
	End


End

Condition
	Time	30
	Event
		Type	Start
		At	Origin
		ChildFx woomer2/childfx.fx
	End

End



End	


			