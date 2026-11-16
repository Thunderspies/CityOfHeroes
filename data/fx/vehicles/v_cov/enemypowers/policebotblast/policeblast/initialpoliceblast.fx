#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200


#########################################################

Condition
	On 	Time
	Time 	40
	
	Event
		EName	Point
		Type	local	
		At	wepR
	
		BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Geom	heavyMace
	End	

	Event
		EName	Prime
		Type	local
		At	Point
		Altpiv	2
		
		BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EmissionPoint.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BlastStreaks.part
		
	End

	Event
		EName	Prime1
		Type	start
		At	Point
		Altpiv	2

		Bhvr	:Projectile1.bhvr
		Magnet	Target
		LookAt	Target
		
		Part	:EnergyHead.part
		Part	:EnergyHeadInvert.part
		Part	:EnergyHeadDark.part
		Part	:EnergyHeadDarkInvert.part
		Part	:RapidEnergyCoreLocal.part
		Part	:RapidCoreLocal.part
		Part	:RapidCoreLocalInvert.part


		Part	:BotTrail.part
		Part	:BotTrailSpecks.part
		Part	:BotBubblesTrail.part
		Part	:BotBallzTrailDark.part
		Part	:BotBallzTrail.part
		Part	:EnergyStreakTrailStraight.part
		Part	:BotTrail2.part
		
	End

	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End
End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
		

	End
End

End				