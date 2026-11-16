#########################################################
##	Lift
########################################################
FxInfo

Input  
	Inpname	Origin
End

LifeSpan	100

################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part1	powers/telekinesis/LiftPlayer.part
		#Part2	powers/powerpunch/powerpunch2.part
		#Part3	powers/powerpunch/powerpunch3.part
	End

End	





End
