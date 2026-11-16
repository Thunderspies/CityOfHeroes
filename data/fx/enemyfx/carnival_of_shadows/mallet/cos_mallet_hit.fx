#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	FootL
End

Input  
	Inpname	FootR
End

Input  
	InpName	UlegR
End

Input  
	InpName	UlegL
End

Input  
	InpName	LlegL
End

Input  
	InpName	LlegR
End

Input  
	Inpname	mystic
End

lifespan 100

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	flash
		Type	start
		At	FootR  ##mystic
		Part1	:AOEflash.part
		Part2	:AOEflash2.part
		Part3	:FootStompStreaks.part
		Part4	:LightBurst.part
		Part5	:LightBurstFlat.part
		Sound atomicblast2 70 70 .5
		Bhvr	Behaviors/CameraShake01.bhvr
	End

End

Condition
	On 	Time
	Time 	0
	Event
		EName	flash2
		Type	start
		At	FootR   ##mystic
		Sound PunchHit2 70.0 70.0 .7
		Part2	:AOEflash3.part
	
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	Streak1
		Type	Destroy
		
	End
	
	Event
		EName	Streak2
		Type	Destroy
		
	End

	Event
		EName	Streak5
		Type	Destroy
		
	End

	Event
		EName	Streak6
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	42

	Event
		EName	Streak3
		Type	Destroy
		
	End

	Event
		EName	Streak4
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	100

	Event
		EName	all
		Type	Destroy
		
	End

End


End
#########################################################

