#########################################################
##	Lightning Bolt
##
FxInfo

lifespan	60

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

Input  
	Inpname	mystic
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime2
		Type	Local 
		At	WepR
		Part1	powers/powerpunch/powerpunch1.part
		Part2	powers/powerpunch/powerpunch2.part
		Part3	powers/powerpunch/powerpunch3.part
		Part4	powers/powerpunch/powerpunch2.part
		Part5   powers/powerpunch/powerpunchGlow.part
	End

	Event
		EName 	Prime4
		Type	Local 
		At	WepR
		
		Part3   powers/powerpunch/powerpunchRings.part

	End
End


Condition
	On 	Time
	Time 	36

	Event
		EName 	Prime
		Type	Local 
		At	mystic
		Part1	powers/powerpunch/HitRingFlat.part
		Part2	powers/powerpunch/HitRingThinFlat.part
		
	End

#####################################################################################

End


Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime3
		Type	Destroy
	End

	Event
		EName 	Prime4
		Type	Destroy
	End

End

End		