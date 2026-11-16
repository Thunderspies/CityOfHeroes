#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	WepR
End




Condition
	On 	Time
	Time 	19

	Event
		EName 	Prime8
		Type	Local 
		At	WepR
		Bhvr	behaviors/powers/coldcontrol/IceSword.bhvr
		Geom	FX_IceWeapon__Ice	
	End


Event
		EName 	Next
		Type	Local
		At	Prime8
		 AltPiv 2
		
		Part2	powers/coldcontrol/snowfall.part
		
		POther	Prime
		

	End



	Event
		EName 	Prime
		Type	Local 
		At	Prime8
		 AltPiv 1
		Part1	:icesword2.part
		Part2	:icesword1Trail.part
		Part3	:icesword1.part
		POther	Next
		#PMagnet	Next
		
	End

	
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	glowhand
		Type	Local 
		At	WepR
		Part1	:iceswordFistSumon.part

	End

End	
Condition
	On 	Time
	Time 	50

	Event	
		EName 	glowhand
		Type	Destroy


	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName 	snowburst
		Type	LOCAL
		At	WepR
		Part1	:snowburst2.part

	End
End


Condition
	On 	Time
	Time 	70

	Event
	Type	Destroy
	EName	All

	End


End


End
