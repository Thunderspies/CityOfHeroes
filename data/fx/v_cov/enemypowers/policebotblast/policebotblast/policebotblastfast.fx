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
	On	Time
	Time	10

	Event
		EName	Prime
		Type	start
		At	eyes

		Bhvr	behaviors/projectile3.bhvr
		Magnet	Target
		LookAt	Target
		
		
		Part	:EnergyHead.part
		Part	:EnergyHeadInvert.part
		Part	:RapidEnergyCoreLocal.part
		Part	:RapidCoreLocal.part
		Part	:RapidCoreLocalInvert.part

		Part	:BotTrail.part
		Part	:BotTrailSpecks.part
		Part	:BotBubblesTrail.part
		Part	:BotBallzTrail.part
		Part	:EnergyStreakTrailStraight.part
		Part1	:BotTrail2.part
		Sound PoliceBotLaser2 100 100 .95
		
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