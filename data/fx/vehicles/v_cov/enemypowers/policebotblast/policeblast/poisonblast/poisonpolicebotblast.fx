#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200



#########################################################

Condition
	On 	Time
	Time 	30
	
	Event
		EName	Point
		Type	local	
		At	wepR
	
		BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Geom	heavyMace
	End	

	Event
		Type	local
		At	Point
		Altpiv	2
		
		BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EmissionPoint.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BlastStreaks.part
		Sound PoliceBotLaser2 100 100 .95
		
	End

	Event
		EName	Prime
		Type	start
		At	Point
		Altpiv	2

		Bhvr	behaviors/Quillprojectile1.bhvr
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