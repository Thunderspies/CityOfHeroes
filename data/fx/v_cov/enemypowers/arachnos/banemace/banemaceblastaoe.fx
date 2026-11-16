#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300
#########################################################

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
		Sound PoliceBotBlast3 100 100 .98
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
		Type	start
		At	T_Chest
		Part1	POWERS\MartialArts\MartialArtsHit.part
		Part2	POWERS\MartialArts\MartialArtsHit1.part
		Part3	POWERS\MartialArts\MartialArtsHit2.part
		Part4	POWERS\MartialArts\MartialArtsHitRaysSmall.part
		Sound banedisruptorhit 80 80 .8
		LifeSpan	3
	End
	
	Event
		EName 	Rings
		Type	start
		At	T_Root

		BHVR	Behaviors/RootToChestadjustment.bhvr
		Part	:HitRingflashthin.part
		Part	:HitRingflashthin2.part
		Part	:HitRingflash.part
		LifeSpan	20
	End
	
	Event
		EName   R
		Type	local
		At	Rings
		Geom	Centerdummy
		Bhvr	:Inferno.bhvr
	End


	Event
		Type	Local 
		At	R
		altpiv	1
		bhvr    :Infernobubble.bhvr
		Part	:aoeDarkEnergy.part
		Part	:aoeLightEnergy.part
		Part1	:aoeEnergy.part
		Part	:aoeEnergyDots.part
	End		

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

End				