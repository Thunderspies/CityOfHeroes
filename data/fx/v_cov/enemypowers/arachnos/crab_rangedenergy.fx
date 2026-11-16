#########################################################
##	energy_Ball
##

#########################################################

FxInfo

LifeSpan	90


###############  Sound  ##################


Condition	#Sounds
	On 	Time
	Time 	22

	Event
		Type	local
		Sound PoliceBotBlast2 100 100 .98
	End	
End


############## Pulse 1  ###################

Condition	
	On 	Time
	Time 	28

	Event
		EName	Pulse1
		Type	Start
		At	C_Fore_ToeR

		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		#Sound	PoliceBotBlast2	100 90 .98
		Lifespan 12
	End	
End



Condition
	On 	Time
	Time 	30

	Event
		EName	Prime
		Type	start
		At	Pulse1

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Target
		LookAt	Target
		
		Part	:Crab_EnergyHead.part
		Part	:Crab_EnergyHeadInvert.part
##		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EnergyHeadDark.part
##		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\EnergyHeadDarkInvert.part
		Part	:Crab_RapidEnergyCoreLocal.part
		Part	:Crab_RapidCoreLocal.part
		Part	:Crab_RapidCoreLocalInvert.part


#		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BotTrail.part
# 		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BotTrailSpecks.part
		Part	:Crab_BotBubblesTrail.part
		Part	:Crab_BotBallzTrailDark.part
		Part	:Crab_BotBallzTrail.part
		Part	:Crab_EnergyStreakTrailStraight.part
#		Part	V_COV\EnemyPowers\PoliceBotBlast\PoliceBlast\BotTrail2.part
		Lifespan 25
	End
	
End



		