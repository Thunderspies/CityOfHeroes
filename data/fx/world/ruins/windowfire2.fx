#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		#Sound	Bigboomexplo 90 70 .7
		#Sound	ExploBurst 90 70 .88
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part1	:WindowFireFlame_A.part
		Part2	:WindowFireFlame_B.part
		#Part4	:spark2.part
		Part5	:Window_Smoke.part
		#Sound	torch2_loop 97 19 .44
	End
End

End			