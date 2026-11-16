#############################################################
## MaceMastery_PoisonousBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	36

	Event
		EName	Point
		Type	local
		At	wepR
		BHVR	V_COV\Weapons\maceBlastEmissionPoint.bhvr
		Geom	heavyMace
	End

	Event
		Type	local
		At	wepR
		BHVR	V_COV\Weapons\maceBlastEmissionPoint.bhvr
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EmissionPoint.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BlastStreaks.part
		Sound PoliceBotLaser2 100 100 .95
		LifeSpan 2
	End

	Event
		EName	Prime
		Type	start
		At	Point
		Altpiv	2
		Bhvr	behaviors\Quillprojectile1.bhvr
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\EnergyHead.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\EnergyHeadInvert.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\RapidEnergyCoreLocal.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\RapidCoreLocal.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\RapidCoreLocalInvert.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\BotTrail.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\BotTrailSpecks.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\BotBubblesTrail.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\BotBallzTrail.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\EnergyStreakTrailStraight.part
		Part1	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\PoisonBlast\BotTrail2.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	ALL
		Type	Destroy
	End
End

#############################################################

End