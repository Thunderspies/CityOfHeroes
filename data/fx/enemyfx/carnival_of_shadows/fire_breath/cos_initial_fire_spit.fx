#########################################################
##	
##

FxInfo


Input  
	InpName	head
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Lifespan	150

Condition
	On 	Time  
	Time 	0
	
	Event
		EName 	TorchFire
		Type	Local 
		At	WepR
		GEOM	Staff_06
	
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	TorchFire
		Altpiv   1		
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Part3	:TorchsparksA.part
		Sound Torch1_loop 22.0 22.0 1.0
		
	End
	Event
		EName	Torch
		Type	Local
		At	WepR
		Geom	GEO_WepR_Staff_07
		Lifespan	150
		
	End
End

Condition
	On 	Time
	Time 	51
	Event
		EName	Prime
		Type	Start
		At	Torchfire
		Altpiv   1
		Bhvr	:FireSpit.bhvr
		Part1	:FireBallHead.part
		Part2	:FireBallTail.part
		Part3	:FireBallStreak.part
		Part4	:SparkTrail.part
		Magnet	Target
		LookAt	Target
		Sound arcoflame 100 100 .88
		Lifespan	80
		
	End

End

Condition
	On	PrimeHit		
	Event
		Ename	FireBallHit
		Type	local
		At	target

		Part1	:BigFireExplosion.part
		Part2	:FireBallHitSpark.part
	End

	Event
		Ename	All
		Type	Destroy
	End

End

End				