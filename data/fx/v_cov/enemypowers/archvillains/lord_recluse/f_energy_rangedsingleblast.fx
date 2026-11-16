#########################################################
##	energy_Ball
##

#########################################################

FxInfo

LifeSpan	90

Condition
	On	Time
	Time	0

	Event
		EName	Emitter
		Type	Local
		At	C_Fore_ToeR
		BhvrOverride
			PositionOffset	0 1.25 .05
		End

#		Geom	Testing_TargetB
	End
End

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
		At	Emitter

		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_EnergyFlareDark.part		#blast black flares		
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_EnergyFlareRed.part		#blast cone red 
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_EnergyFlareWhite.part		#blast cone inner white
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_EnergyFlareStreaks.part
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

		Bhvr	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\B_Energy_ProjectileTravel.bhvr
		Magnet	Target
		LookAt	Target
		
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_EnergyHead.part
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_EnergyHeadInvert.part
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_RapidEnergyCoreLocal.part
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_RapidCoreLocal.part
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_RapidCoreLocalInvert.part
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_BotBubblesTrail.part
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_BotBallzTrailDark.part
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_BotBallzTrail.part
		Part	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\P_Blast_EnergyStreakTrailStraight.part
		Lifespan 25
	End
	
End


Condition
	On	PrimeHit

	Event
		EName	ChildBurst
		Type	Local
		At	Prime
		Flags	AdoptParentEntity

		ChildFX	V_COV\EnemyPowers\ArchVillains\Lord_Recluse\F_Child_EnergyBlastHit.fx
	End
End

End