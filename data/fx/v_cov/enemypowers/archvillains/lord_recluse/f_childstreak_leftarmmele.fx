
#########################################################
#####
#####  Note: All Crab Arm fx even names are based on a left/Right (L or R) and 1 through 4  
#####        - Left and right are character relative
#####        - numbers 1 through 4 count from center top arm (1) around to bottom arm (4). 
#####          clockwise on right, counter clockwise on left, 
#####
########################################################################################


FxInfo

Lifespan 30

#########################################################################################
####################  Set up streak emitter positions - Crab Arm L1  ####################
#########################################################################################


Condition
	On	Time
	Time	0
	
	Event
		EName	L1_Start
		Type	Local
		At	C_Fore_UlegL
		bhvrOverride
			PositionOffset	0 .25 0
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L1_02
		Type	Local
		At	C_Fore_LlegL
		bhvrOverride
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L1_03
		Type	Local
		At	C_Fore_FootL
		bhvrOverride
			PYRRotate	0 0 0 
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	L1_04
		Type	Local
		At	C_Fore_ToeL
		bhvrOverride
			PositionOffset	0 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L1_End
		Type	Local
		At	C_Fore_ToeL
		bhvrOverride
			PositionOffset	0 2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End
End

##############  Particle Emitters  ################

Condition
	On	Time
	Time	3
	
	Event
		EName	Trail_L1_01
		Type	Local
		At	L1_Start
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L1_02
		PMagnet	L1_02

	End

	Event
		EName	Trail_L1_02
		Type	Local
		At	L1_02
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L1_03
		PMagnet	L1_03

	End

	Event
		EName	Trail_L1_03
		Type	Local
		At	L1_03
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L1_04
		PMagnet	L1_04

	End

	Event
		EName	Trail_L1_04
		Type	Local
		At	L1_04
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L1_End
		PMagnet	L1_End

	End

	Event
		EName	Trail_L1_02_Joint
		Type	Local
		At	L1_02
		
		part2	:P_Streak_RecluseClawMove01.part
	End

	Event
		EName	Trail_L1_03_Joint
		Type	Local
		At	L1_03
		
		part2	:P_Streak_RecluseClawMove01.part
	End
End


#########################################################################################
####################  Set up streak emitter positions - Crab Arm L2  ####################
#########################################################################################


Condition
	On	Time
	Time	0
	
	Event
		EName	L2_Start
		Type	Local
		At	C_ULegL
		bhvrOverride
			PositionOffset	0 .25 0
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L2_02
		Type	Local
		At	C_LlegL
		bhvrOverride
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L2_03
		Type	Local
		At	C_FootL
		bhvrOverride
			PYRRotate	0 0 0 
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	L2_04
		Type	Local
		At	C_ToeL
		bhvrOverride
			PositionOffset	0 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L2_End
		Type	Local
		At	C_ToeL
		bhvrOverride
			PositionOffset	0 2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End
End

##############  Particle Emitters  ################

Condition
	On	Time
	Time	2
	
	Event
		EName	Trail_L2_01
		Type	Local
		At	L2_Start
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L2_02
		PMagnet	L2_02

	End

	Event
		EName	Trail_L2_02
		Type	Local
		At	L2_02
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L2_03
		PMagnet	L2_03

	End

	Event
		EName	Trail_L2_03
		Type	Local
		At	L2_03
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L2_04
		PMagnet	L2_04

	End

	Event
		EName	Trail_L2_04
		Type	Local
		At	L2_04
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L2_End
		PMagnet	L2_End

	End

	Event
		EName	Trail_L2_02_Joint
		Type	Local
		At	L2_02
		
		part2	:P_Streak_RecluseClawMove01.part
	End

	Event
		EName	Trail_L2_03_Joint
		Type	Local
		At	L2_03
		
		part2	:P_Streak_RecluseClawMove01.part
	End
End

#########################################################################################
####################  Set up streak emitter positions - Crab Arm L3  ####################
#########################################################################################


Condition
	On	Time
	Time	0
	
	Event
		EName	L3_Start
		Type	Local
		At	C_UArmL
		bhvrOverride
			PositionOffset	0 .25 0
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L3_02
		Type	Local
		At	C_LArmL
		bhvrOverride
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L3_03
		Type	Local
		At	C_HandL
		bhvrOverride
			PYRRotate	0 0 0 
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	L3_04
		Type	Local
		At	C_RingL
		bhvrOverride
			PositionOffset	0 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L3_End
		Type	Local
		At	C_RingL
		bhvrOverride
			PositionOffset	0 2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End
End

##############  Particle Emitters  ################

Condition
	On	Time
	Time	1
	
	Event
		EName	Trail_L3_01
		Type	Local
		At	L3_Start
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L3_02
		PMagnet	L3_02

	End

	Event
		EName	Trail_L3_02
		Type	Local
		At	L3_02
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L3_03
		PMagnet	L3_03

	End

	Event
		EName	Trail_L3_03
		Type	Local
		At	L3_03
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L3_04
		PMagnet	L3_04

	End

	Event
		EName	Trail_L3_04
		Type	Local
		At	L3_04
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L3_End
		PMagnet	L3_End

	End

	Event
		EName	Trail_L3_02_Joint
		Type	Local
		At	L3_02
		
		part2	:P_Streak_RecluseClawMove01.part
	End

	Event
		EName	Trail_L3_03_Joint
		Type	Local
		At	L3_03
		
		part2	:P_Streak_RecluseClawMove01.part
	End
End

#########################################################################################
####################  Set up streak emitter positions - Crab Arm L4  ####################
#########################################################################################


Condition
	On	Time
	Time	0
	
	Event
		EName	L4_Start
		Type	Local
		At	C_Hind_ULegL
		bhvrOverride
			PositionOffset	0 .25 0
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L4_02
		Type	Local
		At	C_Hind_LLegL
		bhvrOverride
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L4_03
		Type	Local
		At	C_Hind_FootL
		bhvrOverride
			PYRRotate	0 0 0 
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	L4_04
		Type	Local
		At	C_Hind_ToeL
		bhvrOverride
			PositionOffset	0 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	L4_End
		Type	Local
		At	C_Hind_ToeL
		bhvrOverride
			PositionOffset	0 2 .1
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End
End

##############  Particle Emitters  ################

Condition
	On	Time
	Time	0
	
	Event
		EName	Trail_L4_01
		Type	Local
		At	L4_Start
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L4_02
		PMagnet	L4_02

	End

	Event
		EName	Trail_L4_02
		Type	Local
		At	L4_02
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L4_03
		PMagnet	L4_03

	End

	Event
		EName	Trail_L4_03
		Type	Local
		At	L4_03
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L4_04
		PMagnet	L4_04

	End

	Event
		EName	Trail_L4_04
		Type	Local
		At	L4_04
		
		part1	:P_Streak_RecluseClawMove.part
		part2	:P_Streak_RecluseClawMove01.part
		POther	L4_End
		PMagnet	L4_End

	End

	Event
		EName	Trail_L4_02_Joint
		Type	Local
		At	L4_02
		
		part2	:P_Streak_RecluseClawMove01.part
	End

	Event
		EName	Trail_L4_03_Joint
		Type	Local
		At	L4_03
		
		part2	:P_Streak_RecluseClawMove01.part
	End
End




End	

	