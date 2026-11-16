#########################################################
##	Lift
########################################################
FxInfo

Input  
	Inpname	chest
End

LifeSpan	100

################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	chest
		
		Part1	:TeleBoltHitClouds.part
		Part2	:TeleBoltHitStreaks.part
		
	End

End	

Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	chest
		
		Part1	:TeleBoltHitStreak.part
		Part2	:TeleBoltHitStreakz.part
	End
End


Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	chest
		#Part1	Powers/ElectricityControl/ElectricityGlow.part
		Part2	:TelectricityArch4.part
		Part3	:TelectricityArchSmall.part
	End

End

Condition
	On 	Time
	Time 	25
	Event
		EName 	HandGlow1
		Type	Destroy
				
	End

End


End
