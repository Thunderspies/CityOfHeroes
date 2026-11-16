#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	root
End

Input  
	Inpname	mystic
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

##############################################################

Condition
	On 	Time
	Time 	34

	Event
		EName	p
		Type	local	#start
		At	WepR
		Part1	:ClawStreak.part
		Sound Clawspin 100 100 1.0
	End

End

Condition
	On 	Time
	Time 	53

	Event
		EName	Streakburst
		Type	local	#start
		At	WepR
		part1	:ClawEmbers.part
		part2	:ClawEmbersBroad.part
	End

End

Condition
	On 	Time
	Time 	54

	Event
		EName	Ring
		Type	start
		At	root
		Part5	:ClawsRing.part
	End

End


Condition
	On Time 
	Time 69
	
	Event
		EName	Streakburst
		Type 	Destroy
				
	End

	Event
		EName	Ring
		Type 	Destroy
				
	End

End


Condition
	On Time 
	Time 57
	Event
		EName	p
		Type 	Destroy
				
	End
End


Condition
	On Time 
	Time 94
	
	Event
		EName	all
		Type 	Destroy
				
	End

End

End
##################################

