#########################################################
##	Mitochondria Attack
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	Chest
End

Input  
	Inpname	Waist
End

LifeSpan	200

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	Waist
		Part1	:FireHandz.part
		LifeSpan	15
	End
End

Condition
	On	Time
	Time	16
	Event
		Type 	Local
		At 	Origin
		Sound mitoblast1 80 80 .88
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
		At	Waist
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		Magnet	Target
		LookAt	Target
		Part	:Head.part
		Part2	:FireBoltTail2.part
		Part1	:FireHands2.part
		Part3	:FireBoltTail.part
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


			