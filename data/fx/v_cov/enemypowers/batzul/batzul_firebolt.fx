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
		Part1	:FireHandz.part
		#Sound	Fireball1 100.0 30.0 .66
	
	End


End

Condition
	On 	Time
	Time  	8
	Event
		Type	Local
		At	Origin
		Sound Fireball1 60.0 60.0 .33
	End

End


Condition
	On 	Time
	Time 	12
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:FireHandsFlash.part
		#Sound	Fireblastfly 80 30 .9
	
	End

End

Condition
	On	Time
	Time	16
	Event
		Type 	Local
		At 	Origin
		Sound Fireblastfly 80 80 .9
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
	Time 	16

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
		Bhvr	:BatZul_Fire_Projectile.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:FireHands2.part
		Part2	:FireBoltTail2.part
		Part3	:FireBoltTail.part
		Part4	:SparkTrail.part
		Part5	:FireHands2.part

			
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		12
			PhysForcePower		150
			PhysForcePowerJitter	25
		End
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
	Time	100
		
		
	Event
		Ename	all
		Type	Destroy
	End

End

End	


			