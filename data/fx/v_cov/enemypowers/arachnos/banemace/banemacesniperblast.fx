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
	Time 	0

	Event
		EName	Point
		Type	local
		At	wepR
		Geom	heavyMace
	End

	Event
		Type	local
		At	Point
		Altpiv	2
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EmissionPoint.part
		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BlastStreaks.part
		Sound PoliceBotBlast3 100 100 .98
		LifeSpan	2
	End

	Event
		EName	Prime
		Type	start
		At	Point
		Altpiv	2

		Bhvr	:Projectile1.bhvr
		BhvrOverride
			TrackRate	7.5
		End
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