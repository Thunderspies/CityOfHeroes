#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	Head
End


#########################################################

Condition
	On 	Time
	Time 	30
	

	Event
		EName	Prime
		Type	start
		At	head

		Bhvr	behaviors/Quillprojectile6.bhvr
		Magnet	Target
		LookAt	Target
		
		
		Part	:SmallEnergyHead.part
		Part	:SmallEnergyHeadInvert.part
		Part	:SmallRapidEnergyCoreLocal.part

		Part	:SmallBotBubblesTrail.part
		Part	:SmallBotBallzTrail.part
		Part	:SmallEnergyStreakTrailStraight.part
		
	End

	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

End				