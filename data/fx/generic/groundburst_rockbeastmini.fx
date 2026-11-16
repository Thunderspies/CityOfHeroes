#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Mystic
End


Input  
	Inpname	Chest
End


####################################

Lifespan 60

Condition
	On 	Time
	Time 	1

	Event
		EName 	GroundBurst
		Type	Local 
		At	Hips
		Part1	POWERS\SuperStrength\superStrengthDust1.part
		part2	POWERS\SuperStrength\superStrengthDust2.part
		Part3	POWERS\EarthControl\WeaponRocks05.part
		Part4	POWERS\EarthControl\WeaponRocks05.part
		Sound Groundburst 80 80 .96
	End


End

Condition
	On 	Time
	Time 	3

	Event
		EName 	ChestDust
		Type	Local 
		At	Chest
		Part1	POWERS\EarthControl\StalagtiteDustHit.part
		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
	End


End

Condition
	On 	Time
	Time	50
	
	Event
		EName 	ChestDust
		Type	Destroy
		

	End
	

End

Condition
	On 	Time
	Time	40
	
	Event
		EName 	GroundBurst
		Type	Destroy
		

	End
	

End


End
