#########################################################
##	Robot Spawn Assemble Big Bang
##


FxInfo
	
lifespan	150

##########################################################################
###########################  Explode Center  #############################
##########################################################################

Condition

	On Time 
	Time 0
	
	Event
		EName 	Pos
		Type	Start 
		At	Chest
#		Geom	RootToChestAdjustment
		
	End
	
End

##########################################################################
###########################  Explode Sounds  #############################
##########################################################################




##########################################################################
###########################  Explode Center  #############################
##########################################################################

Condition

	On 	Time
	Time 	0

	Event
		EName   RadiusFxScale
		Type	local
		At	Pos
		
		Bhvr	Behaviors\FX_RobotSpawn_blastCore.bhvr
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		
		bhvr    behaviors/FX_RobotSpawn_BlastScale.bhvr
#		Part2	ENEMYFX\DevouringEarth\Hamidon\Power_Nucleus_AE_Blast\IrradiateYellowFlame01.part
#		Part3	ENEMYFX\DevouringEarth\Hamidon\Power_Nucleus_AE_Blast\IrradiateRings.part
		Part4	V_COV\Powers\RobotSpawns\RS_P_BlastSpikes.part
		
		
	End		


End

Condition
	On	Time
	Time	3
	Event
		EName 	BlastCore
		Type	Local 
		At	RadiusFxScale
		
		bhvr    behaviors/FX_RobotSpawn_BlastScale.bhvr
		Part1	V_COV\Powers\RobotSpawns\RS_P_RadiateCloud.part
	End
End




##########################################################################
##########################  Expanding Rings  #############################
##########################################################################

Condition
	On 	Time
	Time 	3
	Event
		EName 	Prime8
		Type	StartPositOnly 
		At	BlastCore
		
		bhvr	Behaviors\FX_RobotSpawn_blastring.bhvr
		Part1   V_COV\Powers\RobotSpawns\RS_P_RadiateRing.part
		Part2   V_COV\Powers\RobotSpawns\RS_P_RadiateRing2.part
	End

End

Condition
	On 	Time
	Time 	4
	Event
		EName 	Prime9
		Type	StartPositOnly 
		At	BlastCore
		
		bhvr	Behaviors\FX_RobotSpawn_blastring.bhvr
#		Part1   V_COV\Powers\RobotSpawns\RS_P_RadiateRing.part
#		Part2   V_COV\Powers\RobotSpawns\RS_P_RadiateRing2.part

	End

End

##########################################################
################### Ground Impact ########################
##########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin

		childFX	:FX_RS_FeetLand.fx
	End
End


##########################################################
################### Ground Impact ########################
##########################################################


End





##########################################################################
##########################################################################
##########################################################################
##########################################################################
##########################################################################





















