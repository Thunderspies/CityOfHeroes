#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Root
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	RoachGroupSmall
		Type	Posit 
		At	Root
		Part1	World/Nature/Fish.part
		#Part2	powers/firecontrol/fireringspark.part	
		
	End


End

End			