#########################################################
##	FireRing
##
FxInfo

LifeSpan	150	

Input  
	InpName	root
End

Input  
	Inpname	WepR
End

######################
##Hand effect
##########################
Condition
	On 	Time
	Time 	14

	Event
		EName	core
		Type	local
		At	Head
		geom	LocalPivot01
		Sound telekinetic2 80 80 .8
		
	End


	Event
		EName	core2
		Type	Local
		At	core
		  Altpiv   1
		
		part1	:PsionicTeleRingsSmall.part
		#part2	:PsionicTeleRingzSmall.part
		
	End

End

Condition
	On 	Time
	Time 	15

Event
		EName	core3
		Type	Local
		At	core
		  Altpiv   1
		
		part2	:PsionicGlow2.part
				
	
	End

End

Condition
	On 	Time
	Time	32
	
	Event
		Ename 	core
		Type	Destroy
	End

	Event
		Ename 	core2
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	110
	
	Event
		Ename 	core3
		Type	Destroy
	End

End


End			