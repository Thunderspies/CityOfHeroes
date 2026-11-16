#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	neck
End

Input  
	InpName	head
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
		
		Part1	:AirSuffocationStreakz.part
		Part2	:AirSuffocation.part
		Part3	:AirSufficationRings.part
		Part4	:AirNumbersign.part
		Part5	:AirStarsign.part
	End

	Event	
		Ename   targetchill
		Type	Local
		At	neck
		
		Part1	:AirPercentsign.part
		Part2	:AirQuestionMark.part
		#Part3	:AirSuffocationlocal.part
	End

	Event	
		Ename   targetchill
		Type	Local
		At	head
		
		#Part1	:AirPercentsign.part
		#Part2	:AirQuestionMark.part
		Part3	:AirSuffocationlocal.part
	End

End



End		