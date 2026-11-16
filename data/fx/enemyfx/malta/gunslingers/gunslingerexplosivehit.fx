#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	hips
End



LifeSpan	100

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	hips
		Sound gunslingerexplo2 80 80 .78
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	hips
		#Part1	Powers/FireControl/FireBallTargetSteam.part
		#Part2	Powers/FireControl/FireBallTargetFlame2.part
		Part3	Powers/FireControl/FireBallSpark.part
		
		Lifespan 5
	End

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	powers/firecontrol/FireWeaponExplosion.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
	End

End

Condition
	On 	Time
	Time 	6
	Event
		EName 	flash
		Type	Destroy 

	End


End

Condition
	On 	Time
	Time 	100

	Event
		EName 	All
		Type	Destroy 
	End


End


End			