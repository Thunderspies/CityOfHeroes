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
		Part1	world/City/Steam01_MedSml_Sideways.part
		Part2	world/City/Steam02_MedSml_Sideways.part
		Sound steam3_loop 59 59 .2
	End
End

End			