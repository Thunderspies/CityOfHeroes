#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Waist
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Waist
		Type	Posit 
		At	Root
		Part1	powers/firecontrol/Yellow2.part
		Part2	powers/firecontrol/fireringspark.part	
		
	End

End

Condition
	On 	Time
	Time 	100

	Event
		EName 	ring1
		Type	Posit 
		At	Root
		Part1	powers/firecontrol/YellowFlame01.part
			
	End


End


End			