#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	neck
End

LifeSpan	100

##################################
Condition
	On	Time
	Time	0
		

	Event	
		Ename   targetchill
		Type	Local
		At	neck
		
		Part1	:SuffocationStreakz.part
		Part2	:suffocation.part
		Part3	:SufficationRings.part
	
	End

End



End		