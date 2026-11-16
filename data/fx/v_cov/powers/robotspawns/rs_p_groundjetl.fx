#########################################################
##	
##		RobotSpawn Drop
##

#########################################################

FxInfo

LifeSpan	200


#########################################################
################ left foot jet Rebound ##################
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	ReboundL
		Type	Start
		At	FootL
		Bhvr	behaviors\FX_RobotSpawn_Rebound.bhvr
		#Geom	Testing_Target
		Part2	V_COV\Powers\RobotSpawns\RS_P_flameExpandB.part
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	ReboundL
		Type	Start
		At	FootL
		Bhvr	behaviors\FX_RobotSpawn_Rebound.bhvr
		#Geom	Testing_Target
		Part1	V_COV\Powers\RobotSpawns\RS_P_flameExpandA.part
	End

End




End
















		