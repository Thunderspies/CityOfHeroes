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
	Time 	26
	
	Event
		Type	Local
		At 	origin
		Sound PoliceBotBlast2 100 100 .8
	End
End

Condition
	On 	Time
	Time 	36
	
	Event
		EName	Point
		Type	local	
		At	wepR
	
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Geom	heavyMace
	End	

	Event
		Type	local
		At	Point
		Altpiv	2
	
		#BHVR	V_COV/Weapons/maceBlastEmissionPoint.bhvr
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EmissionPoint.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BlastStreaks.part
		
		LifeSpan	2
	End

	Event
		EName	Prime
		Type	start
		At	Point
		Altpiv	2

		Bhvr	:Projectile1.bhvr
		Magnet	Target
		LookAt	Target
		
		
		Part	:EnergyHead.part
		Part	:EnergyHeadInvert.part
		Part	:RapidEnergyCoreLocal.part
		Part	:RapidCoreLocal.part
		Part	:RapidCoreLocalInvert.part
		
		Part	:RapidEnergyCoreGlow.part

		Part	:BotTrail.part
		Part	:BotTrailSpecks.part
		Part	:BotBubblesTrail.part
		Part	:BotBallzTrail.part
		Part	:EnergyStreakTrailStraight.part
		Part1	:BotTrail2.part
		#Sound	PoliceBotLaser2 100 90 .95
		
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