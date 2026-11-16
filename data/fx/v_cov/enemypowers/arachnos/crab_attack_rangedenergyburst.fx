#########################################################
##	
##

FxInfo
LifeSpan	180
Flags		InheritAnimScale

############ Targets ################

#Input  
#	InpName	Target
#End
#
#Input  
#	InpName	Mystic
#End


###############################################################################
##################################  Shot 1  ###################################
###############################################################################
#
# this effect is made up of 8 shots, 2 from each of the 8 crab backpack arms 
# for each shot there are 4 primary events: 
#	1. "Shot1_TargetParent" - Target Parent event node with an altpiv offset 45 ft from end of gun is created 
#	2. "Shot1_Target" - Event node to become target is created at Shot1_TargetParent > altpiv 1
#	3. "Blast" - a quick flashing event for muzzel flares
#	4. "Prime" - traveling projectile to target 
#	
#	5. Alternate shots - put in to add fill to fx, this is a secondary shot 4 frames
#	   after the primary shot, from the leg two shots away.  For the most part alternate shots
#	   mirror that of the primary shot fx.
#	
#
################################ fake target 1 ######################################
##
## Setup of targets 
##

Condition
	On	Time
	Time	28					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot1_TargetParent		## Altpiv allows target to be relative to firing arm
		Type	Start
		At	C_Fore_ToeL			## firing arm in this case
		
		Geom	CrabTarget			## center target - these 5 are chosen at random to spawn target in relative random locations
		Geom	CrabTarget01			## right target
		Geom	CrabTarget02			## left target
		Geom	CrabTarget03			## high target
		Geom	CrabTarget04			## low target
	End

End

Condition
	On	Time
	Time	28					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot1_Target			## Shot target fx node to be spawned at parent altpv 1
		Type	Local
		At	Shot1_TargetParent
		 Altpiv	1
		 
		
		#Geom	TESTING_Target			## reference object used to see target position, leaving in for debugging help if needed
		Lifespan 50
	End

End


############################# Fire and projectile ###################################
##
## Firing and projectile events
##

Condition
	On 	Time
	Time 	29						## firing frame
	


	Event
		EName	Blast					## event for muzzle flare and blast flashes
		Type	Start
		At	C_Fore_ToeL

		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		Sound PoliceBotBlast 90 90 .66
		Lifespan 7
	End	
		
End


#####  Projectile  #####

Condition
	On 	Time
	Time 	31

	Event
		EName	Shot1					## projectile event, travels to target established above
		Type	Start
		At	Blast					## shot from same spawn point for blast even above

		Bhvr	:Crab_Projectile1.bhvr			## behavior i stole from doug
		Magnet	Shot1_Target
		LookAt	Shot1_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
		Part	:Crab_EnergyHeadInvert.part
		Part	:Crab_RapidEnergyCoreLocal.part
		Part	:Crab_RapidCoreLocal.part
		Part	:Crab_RapidCoreLocalInvert.part

		Part	:Crab_BotBubblesTrail.part
		Part	:Crab_BotBallzTrailDark.part
		Part	:Crab_BotBallzTrail.part
		Part	:Crab_EnergyStreakTrailStraight.part
		Lifespan 25
	End
	
End




################################ shot1 alt  ######################################
##
## alternate shot - fired from alternate leg and added to simply create more shots
##

Condition
	On 	Time
	Time 	32


	Event
		EName	Blast1_alt
		Type	Start
		At	BendMystic
		##Geom	TESTING_Target				#testing node
		Bhvr	:Crab_ArmOffset_LeftArm.bhvr

		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		#Sound	PoliceBotBlast	100 90 .98
		Lifespan 7
	End	
		
End

Condition
	On 	Time
	Time 	33

	Event
		EName	Prime
		Type	Start
		At	Blast1_alt

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Target					## this shot actually targets character
		LookAt	Target
		
		Part	:Crab_EnergyHead_NoTarget.part
		Part	:Crab_EnergyHeadInvert.part
		Part	:Crab_RapidEnergyCoreLocal.part
		Part	:Crab_RapidCoreLocal.part
		Part	:Crab_RapidCoreLocalInvert.part

		Part	:Crab_BotBubblesTrail.part
		Part	:Crab_BotBallzTrailDark.part
		Part	:Crab_BotBallzTrail.part
		Part	:Crab_EnergyStreakTrailStraight.part
		Lifespan 25
	End
	
End

#Condition
#	On 	PrimeHit					## killed when shot projectile hits character
#	
#	Event
#		EName 	Prime
#		Type	Destroy
#		
#
#	End
#
#End
#
#

###############################################################################
##################################  Shot 2  ###################################
###############################################################################

################################ fake target 2 ######################################

Condition
	On	Time
	Time	37					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot2_TargetParent
		Type	Start
		At	C_Fore_ToeR
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	37					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot2_Target
		Type	Local
		At	Shot2_TargetParent
		 Altpiv	1
		 
		#Geom	TESTING_Target
		Lifespan 50
	End

End


############################# Shot2 and projectile ###################################


Condition
	On 	Time
	Time 	37
	
	Event
		EName	Blast2
		Type	Start
		At	C_Fore_ToeR

		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		Sound PoliceBotBlast 90 90 .66
		Lifespan 7
	End	
		
End

Condition
	On 	Time
	Time 	38

	Event
		EName	Shot2
		Type	Start
		At	Blast2

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot2_Target
		LookAt	Shot2_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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



################################  Shot 2 Alt  ##################################

Condition
	On 	Time
	Time 	41
	
	Event
		EName	Blast2_alt
		Type	Start
		At	BendMystic

		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		#Sound	PoliceBotBlast	100 90 .98
		Lifespan 7
	End	
		
End

Condition
	On 	Time
	Time 	42

	Event
		EName	Prime1
		Type	Start
		At	Blast2_alt

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Target	
		LookAt	Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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

#Condition
#	On 	Prime1Hit
#
#	Event
#		EName 	Prime1
#		Type	Destroy
#		
#
#	End
#
#End
#



################################################################################
###################################  Shot 3  ###################################
################################################################################

################################ fake target 3 ######################################

Condition
	On	Time
	Time	45					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot3_TargetParent
		Type	Start
		At	BendMystic
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	45					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot3_Target
		Type	Local
		At	Shot3_TargetParent
		 Altpiv	1
		 
		#Geom	TESTING_Target
		Lifespan 25
	End

End


############################# Shot3 and projectile ###################################

Condition
	On 	Time
	Time 	45


	Event
		EName	Blast3
		Type	Start
		At	BendMystic
		##Geom	TESTING_Target				#testing node
		Bhvr	:Crab_ArmOffset_LeftArm.bhvr


		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		Sound PoliceBotBlast 90 90 .66
		Lifespan 7
	End	
		
End


Condition
	On 	Time
	Time 	46

	Event
		EName	Shot3
		Type	Start
		At	Blast3

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot3_Target
		LookAt	Shot3_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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



################################  Shot 3 Alt  ##################################

Condition
	On 	Time
	Time 	50
	
	Event
		EName	Blast3_alt
		Type	Start
		At	C_Fore_ToeR

		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		#Sound	PoliceBotBlast	100 90 .98
		Lifespan 7
		 
	End	
		
End


Condition
	On 	Time
	Time 	54

	Event
		EName	Prime1
		Type	Start
		At	Blast3_alt

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Target	
		LookAt	Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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


################################################################################
###################################  Shot 4  ###################################
################################################################################

################################ fake target 4 ######################################

Condition
	On	Time
	Time	54					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot4_TargetParent
		Type	Start
		At	BendMystic
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	54					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot4_Target
		Type	Local
		At	Shot4_TargetParent
		 Altpiv	1
		 
		#Geom	TESTING_Target
		Lifespan 50
	End

End


############################# Shot4 and projectile ###################################


Condition
	On 	Time
	Time 	54


	Event
		EName	Blast4
		Type	Start
		At	BendMystic
		##Geom	TESTING_Target
		Bhvr	:Crab_ArmOffset_RightArm.bhvr

		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		Sound PoliceBotBlast 90 90 .66
		Lifespan 7
	End	
		
End

Condition
	On 	Time
	Time 	55

	Event
		EName	Shot4
		Type	Start
		At	Blast4

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot4_Target
		LookAt	Shot4_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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

############################# Shot4 Alt ###################################

#####################  Shot 4 Alt Targeting  ##############################

Condition
	On	Time
	Time	54					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot4_alt_TargetParent
		Type	Start
		At	C_Fore_ToeL
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	54					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot4_alt_Target
		Type	Local
		At	Shot4_alt_TargetParent
		 Altpiv	1
		 
		##Geom	TESTING_Target
		Lifespan 50
	End

End

###################  shot 4 alt firing  ########################


Condition
	On 	Time
	Time 	58


	Event
		EName	Blast4_alt
		Type	Start
		At	C_Fore_ToeL
		##Geom	TESTING_Target
		Bhvr	:Crab_ArmOffset_RightArm.bhvr

		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		#Sound	PoliceBotBlast	100 90 .98
		Lifespan 7
	End	
		
End

Condition
	On 	Time
	Time 	59

	Event
		EName	Shot4_alt
		Type	Start
		At	Blast4_alt

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot4_alt_Target
		LookAt	Shot4_alt_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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

################################################################################
###################################  Shot 5  ###################################
################################################################################

################################ fake target 5 ######################################

Condition
	On	Time
	Time	63					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot5_TargetParent
		Type	Start
		At	C_Fore_ToeL
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	63					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot5_Target
		Type	Local
		At	Shot5_TargetParent
		 Altpiv	1
		 
		#Geom	TESTING_Target
		Lifespan 50
	End

End


############################# Shot5 and projectile ###################################


Condition
	On 	Time
	Time 	63


	Event
		EName	Blast5
		Type	Start
		At	C_Fore_ToeL

		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		Sound PoliceBotBlast 90 90 .66
		Lifespan 7
	End	
		
End


Condition
	On 	Time
	Time 	64

	Event
		EName	Shot5
		Type	Start
		At	Blast5

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot5_Target
		LookAt	Shot5_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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


################################ shot 5 alt  ######################################

Condition
	On 	Time
	Time 	67


	Event
		EName	Blast5_alt
		Type	Start
		At	BendMystic
		##Geom	TESTING_Target				#testing node
		Bhvr	:Crab_ArmOffset_LeftArm.bhvr


		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		#Sound	PoliceBotBlast	100 90 .98
		Lifespan 7
	End	
		
End

Condition
	On 	Time
	Time 	68

	Event
		EName	Shot5_alt
		Type	Start
		At	Blast5_alt

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Target	
		LookAt	Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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

Condition
	On 	PrimeHit
	
	Event
		EName 	Shot5_alt
		Type	Destroy
		

	End

End


###############################################################################
##################################  Shot 6  ###################################
###############################################################################

################################ fake target 6 ######################################

Condition
	On	Time
	Time	72					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot6_TargetParent
		Type	Start
		At	C_Fore_ToeR
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	72					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot6_Target
		Type	Local
		At	Shot6_TargetParent
		 Altpiv	1
		 
		#Geom	TESTING_Target
		Lifespan 50
	End

End


############################# Shot6 and projectile ###################################

Condition
	On 	Time
	Time 	72


	Event
		EName	Blast6
		Type	Start
		At	C_Fore_ToeR

		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		Sound PoliceBotBlast 90 90 .66
		Lifespan 7
	End	
		
End


Condition
	On 	Time
	Time 	73

	Event
		EName	Shot6
		Type	Start
		At	Blast6

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot6_Target
		LookAt	Shot6_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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

################################## Shot6 alt ##########################################

Condition
	On	Time
	Time	76					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot6_alt_TargetParent
		Type	Start
		At	BendMystic
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	76					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot6_alt_Target
		Type	Local
		At	Shot6_alt_TargetParent
		 Altpiv	1
		 
		#Geom	TESTING_Target
		Lifespan 50
	End

End


####  shot 6 fire and projectiles ####

Condition
	On 	Time
	Time 	76


	Event
		EName	Blast6_alt
		Type	Start
		At	BendMystic

		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		#Sound	PoliceBotBlast	100 90 .98
		Lifespan 7
	End	
		
End


Condition
	On 	Time
	Time 	77

	Event
		EName	Shot6_alt
		Type	Start
		At	Blast6_alt

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot6_alt_Target
		LookAt	Shot6_alt_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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



################################################################################
##################################  Shot 7  ###################################
###############################################################################

################################ fake target 7 ######################################

Condition
	On	Time
	Time	80					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot7_TargetParent
		Type	Start
		At	BendMystic
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	80					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot7_Target
		Type	Local
		At	Shot7_TargetParent
		 Altpiv	1
		 
		#Geom	TESTING_Target
		Lifespan 50
	End

End


############################# Shot7 and projectile ###################################

Condition
	On 	Time
	Time 	80


	Event
		EName	Blast7
		Type	Start
		At	BendMystic
		Bhvr	:Crab_ArmOffset_LeftArm.bhvr


		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		Sound PoliceBotBlast 90 90 .66
		Lifespan 7
	End	
		
End


Condition
	On 	Time
	Time 	81

	Event
		EName	Shot7
		Type	Start
		At	Blast7

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot7_Target
		LookAt	Shot7_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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


############################# Shot7 alt ###################################

Condition
	On 	Time
	Time 	84


	Event
		EName	Blast7_alt
		Type	Start
		At	C_Fore_ToeL
		Bhvr	:Crab_ArmOffset_LeftArm.bhvr


		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		#Sound	PoliceBotBlast	100 90 .98
		Lifespan 7
	End	
		
End


Condition
	On 	Time
	Time 	85

	Event
		EName	Shot7_alt
		Type	Start
		At	Blast7_alt

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Target
		LookAt	Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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

Condition
	On 	PrimeHit

	Event
		EName 	Shot7_alt
		Type	Destroy
	End

End



##############################################################################
##################################  Shot 8  ###################################
###############################################################################

################################ fake target 8 ######################################

Condition
	On	Time
	Time	89					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot8_TargetParent
		Type	Start
		At	BendMystic
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	89					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot8_Target
		Type	Local
		At	Shot8_TargetParent
		 Altpiv	1
		 
		#Geom	TESTING_Target
		Lifespan 50
	End

End


############################# Shot8 and projectile ###################################

Condition
	On 	Time
	Time 	89


	Event
		EName	Blast8
		Type	Start
		At	BendMystic
		Bhvr	:Crab_ArmOffset_RightArm.bhvr


		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		Sound PoliceBotBlast 90 90 .66
		Lifespan 7
	End	
		
End

Condition
	On 	Time
	Time 	90

	Event
		EName	Shot8
		Type	Start
		At	Blast8

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot8_Target
		LookAt	Shot8_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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


############################# Shot8 alt ###################################

################################ fake target 8 alt ########################

Condition
	On	Time
	Time	88					## Event that creates geom node with offset altpiv for target

	Event
		Ename	Shot8_alt_TargetParent
		Type	Start
		At	C_Fore_ToeR
		
		Geom	CrabTarget
		Geom	CrabTarget01
		Geom	CrabTarget02
		Geom	CrabTarget03
		Geom	CrabTarget04
	End

End

Condition
	On	Time
	Time	88					## Event node placed at altpiv from above to be used as target

	Event
		Ename	Shot8_alt_Target
		Type	Local
		At	Shot8_alt_TargetParent
		 Altpiv	1
		 
		#Geom	TESTING_Target
		Lifespan 50
	End

End

### shots ###

Condition
	On 	Time
	Time 	93


	Event
		EName	Blast8_alt
		Type	Start
		At	C_Fore_ToeR
		Bhvr	:Crab_ArmOffset_RightArm.bhvr


		#Bhvr	V_COV\EnemyPowers\Arachnos\Arachnos_GunFlash.bhvr
		#BHVR	V_COV/Weapons/maceBlastOffset.bhvr
		Part	:Crab_EnergyFlareDark.part		#blast black flares		
		Part	:Crab_EnergyFlareRed.part		#blast cone red 
		Part	:Crab_EnergyFlareWhite.part		#blast cone inner white
		Part	:Crab_EnergyFlareStreaks.part
		Sound PoliceBotBlast3 90 90 .8
		Lifespan 7
	End	
		
End

Condition
	On 	Time
	Time 	94

	Event
		EName	Shot8_alt
		Type	Start
		At	Blast8_alt

		Bhvr	:Crab_Projectile1.bhvr
		Magnet	Shot8_alt_Target
		LookAt	Shot8_alt_Target
		
		Part	:Crab_EnergyHead_NoTarget.part
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
		Lifespan 30
	End
	
End


#####################################################



End				