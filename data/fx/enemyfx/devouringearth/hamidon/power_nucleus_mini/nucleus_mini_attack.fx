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
	Inpname	Waist
End

LifeSpan	200


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	Waist
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:Nucleus_Attack_Head_Inner.part
		Part3	:Nucleus_Attack_Body.part
		Part4	:Nucleus_Attack_Squiggle.part
		Part1	:Nucleus_Attack_Head.part
		Sound HamidonMiniFire 80 80 .7
		
	End
End

Condition
	On	PrimeHit
		
		
	
	Event
		Ename	NucleusAttack
		Type	Destroy
	End
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


			