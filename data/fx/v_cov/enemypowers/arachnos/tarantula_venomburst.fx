#########################################################
##	Tarantula Venom Bolt
##
#########################################################################################
########	Notes:
########	- Fire Prime1 a few frames before Prime 
########	- Fire Prime with attached particle emitters, spinning offset emiter, 
########	and a burst at firing blast origin
########	- Prime 1 hits killing just before prime hits to provide for an explosion
########	just prior to Prime impact, or visible impact
########	- If miss projectile explodes using parameters above
########
#########################################################################################

FxInfo
	
LifeSpan	105

###############################################################################
##############################  P R I M E  ##########################  01  ####
###############################################################################

########################################################################################
########################################################################################
##########             #############              #############              ###########
##########  SHOT ONE   #############   OF THREE   #############   SHOT ONE   ###########
##########             #############              #############              ###########
########################################################################################
########################################################################################



Condition
	On	Time
	Time	6
	Event
		Type	Local
		At	Chest
		Sound TarantulaSpit 90 90 .94
	End
End


Condition
	On	Time
	Time	6
	Event
		Type	Local
		At	Chest
		Sound ppenergytrans 90 90 .8
	End
End


Condition
	On 	Time
	Time 	35
	
	Event
		Type	Local
		At 	origin
		Sound poisonpalmblast 100 100 .7
	End
End
################################################################################
###########################  Emitter points  ###################################
################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitterPos
		Type	Local
		At	Belt
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBoltEmitter.bhvr
##		Geom	Testing_Target
	End

	Event
		EName	Child
		Type	Local
		At	Belt

		ChildFX	V_COV\EnemyPowers\Arachnos\Tarantula_VBurst_Child.fx
	End

	Event
		EName	ChildFront
		Type	Local
		At	Belt

		ChildFX	V_COV\EnemyPowers\Arachnos\Tarantula_VBurst_ChildFront.fx
	End
End

##################################################################################
####################  P R I M E   decoy for early splat ################  01  ####
##################################################################################

Condition
	On	Time
	Time	32

	Event
		EName	Prime1
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstTravel.bhvr
##		Geom	Testing_Target
		Magnet	T_Chest
		LookAt	T_Chest
	End
End

#################################################################################
###########################  P R I M E   B U R S T  ###################  01  ####
#################################################################################

Condition
	On	Time
	Time	38

	Event
		EName	PrimeSplat
		Type	Start
		At	EmitterPos
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst01.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst.part
		#PMagnet	T_chest
	End

#################################################################################
################################  P R I M E  ##########################  01  ####
#################################################################################

	Event
		EName	Prime
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstTravel.bhvr
##		Geom	Testing_Target
		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName	PrimeOff	# offset node that orbits projectile
		Type	Local
		At	Prime
	
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstOffSet.bhvr
##		Geom	Testing_Target
	End

######################################################################  01  ####

	Event
		EName	PrimeBall
		Type	Local
		At	Prime
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstTail.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst02.part
		PMagnet	Prime
		LookAt	Prime
	End

	Event
		EName	PrimeBall01
		Type	Local
		At	PrimeOff
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst02.part
		PMagnet	PrimeBall
	End

######################################################################  01  ####

	Event
		EName	PrimeDrips
		Type	Local
		At	Prime
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltDrips.part
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltDrips01.part
		Lifespan 20
		PMagnet	T_chest
	End

End


#################################################################################
#################################  Killaz  ############################  01  ####
#################################################################################

Condition
	On	Prime1Hit

	Event
		EName	splat
		Type	PositOnly
		At	prime

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact02.part
		PMagnet	T_Chest
		Lifespan 15
	End

	Event
		EName	splatCloud
		Type	PositOnly
		At	prime

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstCloud.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstBubbles.part
		PMagnet	T_Chest
		Lifespan 15
	End

	Event
		EName	Prime1
		Type	Destroy
	End

	Event
		EName	PrimeDrips
		Type	Destroy
	End

	Event
		EName	PrimeBall
		Type	Destroy
	End

	Event
		EName	PrimeBall01
		Type	Destroy
	End
End

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

###################################################################################################################################################################
###################################################################################################################################################################
###################################################################################################################################################################
###################################################################################################################################################################
###################################################################################################################################################################
###################################################################################################################################################################
###################################################################################################################################################################
###################################################################################################################################################################
###################################################################################################################################################################
###################################################################################################################################################################

###########################################################################################
##########################################################################################
############             #############              #############              ###########
############  SHOT TWO   #############   OF THREE   #############   SHOT TWO   ###########
############             #############              #############              ###########
##########################################################################################
##########################################################################################

#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	TargetAlt02
		Type	Start
		At	T_Chest

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstTargOffset.bhvr
##		Geom	Testing_Target
	End
End

#################################################################################
###################  P R I M E   decoy for early splat ################  02  ####
#################################################################################

Condition
	On	Time
	Time	37

	Event
		EName	Prime4
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstTravel.bhvr
##		Geom	Testing_Target
		Magnet	TargetAlt02
		LookAt	TargetAlt02
	End
End

#################################################################################
###########################  P R I M E   B U R S T  ###################  02  ####
#################################################################################

Condition
	On	Time
	Time	43

	Event
		EName	PrimeSplat02
		Type	Start
		At	EmitterPos
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst01.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst.part
		#PMagnet	T_chest
	End

#################################################################################
################################  P R I M E  ##########################  02  ####
#################################################################################

	Event
		EName	Prime2
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstTravel.bhvr
##		Geom	Testing_Target
		Magnet	TargetAlt02
		LookAt	TargetAlt02
	End

	Event
		EName	PrimeOff02	# offset node that orbits projectile
		Type	Local
		At	Prime2
	
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstOffSet.bhvr
##		Geom	Testing_Target
	End

######################################################################  02  ####

	Event
		EName	PrimeBall02
		Type	Local
		At	Prime2
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstTail.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst02.part
		PMagnet	Prime2
		LookAt	Prime2
	End

	Event
		EName	PrimeBall04
		Type	Local
		At	PrimeOff02
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst02.part
		PMagnet	PrimeBall02
	End

######################################################################  02  ####

	Event
		EName	PrimeDrips02
		Type	Local
		At	Prime2
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltDrips.part
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltDrips01.part
		Lifespan 20
		PMagnet	TargetAlt02
	End

End


#################################################################################
#################################  Killaz  ############################  02  ####
#################################################################################

Condition
	On	Prime4Hit

	Event
		EName	splat02
		Type	PositOnly
		At	prime2

		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact02.part
		PMagnet	TargetAlt02
		Lifespan 15
	End

	Event
		EName	splatCloud02
		Type	PositOnly
		At	prime2

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstCloud.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstBubbles.part
		PMagnet	TargetAlt02
		Lifespan 15
	End


	Event
		EName	Prime4
		Type	Destroy
	End
End

Condition
	On	Prime4Hit

	Event
		EName	PrimeDrips02
		Type	Destroy
	End

	Event
		EName	PrimeBall02
		Type	Destroy
	End

	Event
		EName	PrimeBall04
		Type	Destroy
	End
End

##################################################################################################################################################################
##################################################################################################################################################################
##################################################################################################################################################################
##################################################################################################################################################################
##################################################################################################################################################################
##################################################################################################################################################################
##################################################################################################################################################################
##################################################################################################################################################################
##################################################################################################################################################################
##################################################################################################################################################################

###########################################################################################
###########################################################################################
#############             #############              #############              ###########
############# SHOT THREE  #############   OF THREE   #############  SHOT THREE  ###########
#############             #############              #############              ###########
###########################################################################################
###########################################################################################

#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	TargetAlt03
		Type	Start
		At	T_Chest

		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstTargOffset01.bhvr
##		Geom	Testing_Target
	End
End

#################################################################################
###################  P R I M E   decoy for early splat ################  03  ####
#################################################################################

Condition
	On	Time
	Time	42

	Event
		EName	Prime5
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstTravel.bhvr
#		Geom	Testing_Target
		Magnet	TargetAlt03
		LookAt	TargetAlt03
	End
End

#################################################################################
###########################  P R I M E   B U R S T  ###################  03  ####
#################################################################################

Condition
	On	Time
	Time	48

	Event
		EName	PrimeSplat03
		Type	Start
		At	EmitterPos
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst01.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltBurst.part
		#PMagnet	T_chest
	End

#################################################################################
################################  P R I M E  ##########################  03  ####
#################################################################################

	Event
		EName	Prime3
		Type	Start
		At	EmitterPos
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstTravel.bhvr
#		Geom	Testing_Target
		Magnet	TargetAlt03
		LookAt	TargetAlt03
	End

	Event
		EName	PrimeOff03	# offset node that orbits projectile
		Type	Local
		At	Prime3
	
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_VBurstOffSet.bhvr
##		Geom	Testing_Target
	End

######################################################################  03  ####

	Event
		EName	PrimeBall03
		Type	Local
		At	Prime3
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstTail.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst02.part
		PMagnet	Prime3
		LookAt	Prime3
	End

	Event
		EName	PrimeBall05
		Type	Local
		At	PrimeOff03
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurst02.part
		PMagnet	PrimeBall03
	End

######################################################################  03  ####

	Event
		EName	PrimeDrips03
		Type	Local
		At	Prime3
		
		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltDrips.part
#		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBoltDrips01.part
		Lifespan 20
		PMagnet	TargetAlt03
	End

End


#################################################################################
#################################  Killaz  ############################  03  ####
#################################################################################

Condition
	On	Prime5Hit

	Event
		EName	splat03
		Type	PositOnly
		At	prime3

		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact01.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact.part
		Part3	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstImpact02.part
		PMagnet	TargetAlt03
		Lifespan 15
	End

	Event
		EName	splatCloud03
		Type	PositOnly
		At	prime3

		Part1	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstCloud.part
		Part2	V_COV\EnemyPowers\Arachnos\Tarantula_P_VBurstBubbles.part
		PMagnet	TargetAlt03
		Lifespan 15
	End

	Event
		EName	Prime5
		Type	Destroy
	End
End

Condition
	On	Prime5Hit

	Event
		EName	PrimeDrips03
		Type	Destroy
	End

	Event
		EName	PrimeBall03
		Type	Destroy
	End

	Event
		EName	PrimeBall05
		Type	Destroy
	End
End

End			