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
		At	Head
		BhvrOverride
			PositionOffset 0 -.3 .5
		End
		Part1	\POWERS\FireControl\FireHandz.part
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
	Time 	10
	Event
		EName	SwordFlash2
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset 0 -.3 .5
		End
		Part1	\POWERS\FireControl\FireHandsFlash.part
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
		At	Head
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		BhvrOverride
			PositionOffset 0 -1 0
		End
		Magnet	Target
		LookAt	Target
		Part1	\POWERS\FireControl\FireHands2.part
		Part2	\POWERS\FireControl\FireBoltTail2.part
		Part3	\POWERS\FireControl\FireBoltTail.part
		Part4	\POWERS\FireControl\SparkTrail.part
		Part5	\POWERS\FireControl\FireHands2.part
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


			