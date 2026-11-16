#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

Input  
	InpName	root
End


##################################
Condition
	On	Time
	Time	0
		

	Event	
		Ename   targetchill
		Type	Local
		At	root
		
		Part1	:liftRings.part
		Part2	:liftSparks.part
		Part3	:LargeOuterliftRings.part
		Part4	:LiftSparklies.part
	End

End



End		