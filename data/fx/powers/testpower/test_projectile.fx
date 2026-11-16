#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	WepR
End

LifeSpan	200

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
	
		#Sound	Fireball1 100.0 30.0 .8
	
	End


End


Condition
	On 	Time
	Time 	45

	Event
		EName	InitialSwordFire
		Type	Destroy
		
	
	End

End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:FireBallHead.part
		Part2	:FireBallTail.part
		Part3	:FireBallStreak.part
	
	End

End

Condition
	On 	Time
	Time 	200

	Event
		Ename	All
		Type	Destroy
	End

End

End	


			