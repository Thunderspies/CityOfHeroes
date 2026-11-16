#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	hips
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	hips
		Part1	powers/firecontrol/FireWeaponExplosion.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
	End

	Event
		EName 	Fire
		Type	local
		At	hips
		Part1	powers/firecontrol/FireBallTargetSteam.part
		Part2	powers/firecontrol/FireBallTargetFlame2.part
		Part3	powers/firecontrol/FireBallSpark.part	
		Sound FireExplo 100.0 100.0 .8
	End

End


Condition
	On 	time
	Time 	0
	Repeat  12
	RepeatJitter	3

	Event 
		EName 	Sparkz
		Type 	Local
		At 	Chest
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End


Condition
	On 	Time
	Time 	15
	Event
		EName 	flash
		Type	Destroy 

	End


End

Condition
	On 	Time
	Time 	100
	Event
		EName 	Fire
		Type	Destroy 

	End

	Event
		EName 	all
		Type	Destroy 

	End

End

End			