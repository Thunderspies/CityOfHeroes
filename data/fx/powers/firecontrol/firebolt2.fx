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

LifeSpan	30

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		Sound Fireball1 100.0 100.0 .8
	
	End


End


Condition
	On 	Time
	Time 	10
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:FireHandsFlash.part
	
	End

End


Condition
	On 	Time
	Time 	24

	Event
		EName	SwordFlash2
		Type	Destroy
		
	
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
		At	Origin
		bhvr	behaviors/fireRain.bhvr
		Part1	:FireHands2.part
		Part2	:FireBoltTail2.part
		Part3	:FireBoltTail.part
		Part4	:SparkTrail.part
		Part5	:FireHands2.part
	End


End

Condition
	On	PrimeHit
		
		
	
	Event
		Ename	Prime
		Type	Destroy
	End

End

Condition
	On	Time
	Time	30
		
		
	Event
		Ename	all
		Type	Destroy
	End

End

End	


			