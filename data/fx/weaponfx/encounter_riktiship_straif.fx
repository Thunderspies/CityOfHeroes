#########################################################
##	
##

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End


Input  
	InpName	WepR
End

Input  
	InpName	T1_L
End

Input  
	InpName	T2_L
End

Input  
	InpName	T3_L
End



 

Flags    InheritAlpha

##################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Flash
		Type	local
		At	WepR
		Part1	:BurstStreaks.part
		Part2	:BurstEmbers.part
		
	End

End

Condition
	On 	Time
	Time 	1

	Event
		EName 	Prime
		Type	Local 
		At	T1_L
		Part1	:RiktiBlasterBullet.part

		
	End

End


########################################################


Condition
	On 	Time
	Time	60

	Event
		Ename 	all
		Type	Destroy
	End

End

End				