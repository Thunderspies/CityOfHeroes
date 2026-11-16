#########################################################
##	template

FxInfo

LifeSpan	100

Input
	Inpname Hips
End

#############################################################

Condition
	On Time 
	Time 11
	
	Event
		EName	Hits
		Type 	posit
		At	Hips
		
		Part3	:PistolflashStarCircle.part
		Sound flurryhits 50 50 .8
	End

End

Condition
	On Time 
	Time 84
	

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	:Speedflash.part
		Part2	:SpeedStar.part
		#Sound	hit5 70 30 .9
	End

End

	Condition
	On 	Time
	Time 	90
	Event
		EName 	flash
		Type	Destroy 

	End

End

	Condition
	On 	Time
	Time 	84
	Event
		EName 	Hits
		Type	Destroy 

	End


End


End	

	