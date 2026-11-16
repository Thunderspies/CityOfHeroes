#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Origin
End

LifeSpan	100

##################################
Condition
	On	Time
	Time	0
		

	Event	
		Ename   targetchill
		Type	Local
		At	Origin
		Part1	powers/telekinesis/LiftTarget.part
	End
	

End



End		