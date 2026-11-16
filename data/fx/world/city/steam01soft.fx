#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	time
	Time 	0
	
	Event
		#Bhvr	:smokepulse.bhvr
		
		Type	Local 
		At	Origin
		Part1	world/City/Steam01Soft.part
		#Part2	world/City/Steam02.part
		Sound Steam4_Loop 40.0 40.0 .26
	End
End

End			