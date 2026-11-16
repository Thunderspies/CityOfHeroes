#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End  

Input  
	InpName	head
End    

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	head
		Bhvr	behaviors/MorterLobbprojectile.bhvr
		Geom	FX_Locknar
		Magnet	Target
		LookAt	Target
		Part1	:InfectOthersStreak.part
		Part2	:RadiationHandElectrons.part
		Part3	:MorterStreakGlows.part
		Part4	:MorterStreakRings.part
		
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