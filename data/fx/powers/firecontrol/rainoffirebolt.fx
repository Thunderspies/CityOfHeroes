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

LifeSpan	1500

Condition

	On 	Time
	Time 	10

	Event
		Type Local
		At origin
		Sound Firerain 100.0 100.0 .33
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
	#	Part1	:FireHandz.part
	End


End


Condition
	On 	Time
	Time 	10
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
	#	Part1	:FireHandsFlash.part
	
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
		bhvr	behaviors/RainOfFireBolt.bhvr
		Part1	:FireHands2.part
		Part2	:FireBoltTail2.part
		Part3	:FireBoltTail.part
		Part4	:SparkTrail.part
		Part5	:FireHands2.part

		CEvent 	:RainOfFireBoltBurst.fx
		CThresh 	1
		CDestroy 	1
	End


End

Condition
	On	PrimeHit
		
		
	
	Event
		Ename	Prime
		Type	Destroy
	End

End


End	


			
