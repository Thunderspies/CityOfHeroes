#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Mystic
End


Input  
	Inpname	FootL
End

Input  
	Inpname	FootR
End

####################################

Lifespan 300

Condition
	On 	Time
	Time 	45

	Event
		EName 	GroundBurst_right
		Type	Local 
		At	Mystic
		Part1	POWERS\SuperStrength\superStrengthDust1.part
		part2	POWERS\SuperStrength\superStrengthDust2.part
		Part3	POWERS\EarthControl\WeaponRocks05.part
		Part4	POWERS\EarthControl\WeaponRocks05.part
	End


End

Condition
	On 	Time
	Time 	45

	Event
		EName 	FootDust_right
		Type	Local 
		At	FootR
		Part1	POWERS\EarthControl\StalagtiteDustHit.part
		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
	End


End

Condition
	On 	Time
	Time	50
	
	Event
		EName 	GroundBurst_right
		Type	Destroy
		

	End
	

End

Condition
	On 	Time
	Time	50
	
	Event
		EName 	FootDust_right
		Type	Destroy
		

	End
	

End


Condition
	On 	Time
	Time 	140

	Event
		EName 	GroundBurst_left
		Type	Local 
		At	Mystic
		Part1	POWERS\SuperStrength\superStrengthDust1.part
		part2	POWERS\SuperStrength\superStrengthDust2.part
		Part3	POWERS\EarthControl\WeaponRocks05.part
		Part4	POWERS\EarthControl\WeaponRocks05.part
	End


End

Condition
	On 	Time
	Time 	140

	Event
		EName 	FootDust_left
		Type	Local 
		At	FootL
		Part1	POWERS\EarthControl\StalagtiteDustHit.part
		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
	End


End

Condition
	On 	Time
	Time	145
	
	Event
		EName 	GroundBurst_left
		Type	Destroy
		

	End
	

End

Condition
	On 	Time
	Time	145
	
	Event
		EName 	FootDust_left
		Type	Destroy
		

	End
	

End




End
