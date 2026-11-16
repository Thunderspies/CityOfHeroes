
#########################################################
#####  Note: All Crab Arm fx even names are based on a left/Right (L or R) and 1 through 4  
#####        - Left and right are character relative
#####        - numbers 1 through 4 count from center top arm (1) around to bottom arm (4). 
#####          so the arm that is on the right side of the body, and second to the bottom will
#####          be identified as R3
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
		EName	R1_Start
		Type	Local
		At	C_Fore_ULegR
		bhvrOverride
			PositionOffset	0 .25 0
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R1_02
		Type	Local
		At	C_Fore_LLegR
		bhvrOverride
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R1_03
		Type	Local
		At	C_Fore_FootR
		bhvrOverride
			PYRRotate	0 0 0 
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	R1_04
		Type	Local
		At	C_Fore_ToeR
		bhvrOverride
			PositionOffset	0 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R1_End
		Type	Local
		At	C_Fore_ToeR
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
		EName	Trail_R1_01
		Type	Local
		At	R1_Start
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R1_02
		PMagnet	R1_02

	End

	Event
		EName	Trail_R1_02
		Type	Local
		At	R1_02
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R1_03
		PMagnet	R1_03

	End

	Event
		EName	Trail_R1_03
		Type	Local
		At	R1_03
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R1_04
		PMagnet	R1_04

	End

	Event
		EName	Trail_R1_04
		Type	Local
		At	R1_04
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R1_End
		PMagnet	R1_End

	End

	Event
		EName	Trail_R1_02_Joint
		Type	Local
		At	R1_02
		
		part2	:P_Streak_RecluseKnockUpMove01.part
	End

	Event
		EName	Trail_R1_03_Joint
		Type	Local
		At	R1_03
		
		part2	:P_Streak_RecluseKnockUpMove01.part
	End
End


#########################################################################################
####################  Set up streak emitter positions - Crab Arm L2  ####################
#########################################################################################


Condition
	On	Time
	Time	0
	
	Event
		EName	R2_Start
		Type	Local
		At	C_ULegR
		bhvrOverride
			PositionOffset	0 .25 0
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R2_02
		Type	Local
		At	C_LLegR
		bhvrOverride
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R2_03
		Type	Local
		At	C_FootR
		bhvrOverride
			PYRRotate	0 0 0 
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	R2_04
		Type	Local
		At	C_ToeR
		bhvrOverride
			PositionOffset	0 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R2_End
		Type	Local
		At	C_ToeR
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
		EName	Trail_R2_01
		Type	Local
		At	R2_Start
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R2_02
		PMagnet	R2_02

	End

	Event
		EName	Trail_R2_02
		Type	Local
		At	R2_02
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R2_03
		PMagnet	R2_03

	End

	Event
		EName	Trail_R2_03
		Type	Local
		At	R2_03
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R2_04
		PMagnet	R2_04

	End

	Event
		EName	Trail_R2_04
		Type	Local
		At	R2_04
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R2_End
		PMagnet	R2_End

	End

	Event
		EName	Trail_R2_02_Joint
		Type	Local
		At	R2_02
		
		part2	:P_Streak_RecluseKnockUpMove01.part
	End

	Event
		EName	Trail_R2_03_Joint
		Type	Local
		At	R2_03
		
		part2	:P_Streak_RecluseKnockUpMove01.part
	End
End

#########################################################################################
####################  Set up streak emitter positions - Crab Arm L3  ####################
#########################################################################################


Condition
	On	Time
	Time	0
	
	Event
		EName	R3_Start
		Type	Local
		At	C_UArmR
		bhvrOverride
			PositionOffset	0 .25 0
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R3_02
		Type	Local
		At	C_LArmR
		bhvrOverride
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R3_03
		Type	Local
		At	C_HandR
		bhvrOverride
			PYRRotate	0 0 0 
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	R3_04
		Type	Local
		At	C_RingR
		bhvrOverride
			PositionOffset	0 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R3_End
		Type	Local
		At	C_RingR
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
		EName	Trail_R3_01
		Type	Local
		At	R3_Start
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R3_02
		PMagnet	R3_02

	End

	Event
		EName	Trail_R3_02
		Type	Local
		At	R3_02
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R3_03
		PMagnet	R3_03

	End

	Event
		EName	Trail_R3_03
		Type	Local
		At	R3_03
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R3_04
		PMagnet	R3_04

	End

	Event
		EName	Trail_R3_04
		Type	Local
		At	R3_04
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R3_End
		PMagnet	R3_End

	End

	Event
		EName	Trail_R3_02_Joint
		Type	Local
		At	R3_02
		
		part2	:P_Streak_RecluseKnockUpMove01.part
	End

	Event
		EName	Trail_R3_03_Joint
		Type	Local
		At	R3_03
		
		part2	:P_Streak_RecluseKnockUpMove01.part
	End
End

#########################################################################################
####################  Set up streak emitter positions - Crab Arm L4  ####################
#########################################################################################


Condition
	On	Time
	Time	0
	
	Event
		EName	R4_Start
		Type	Local
		At	C_Hind_ULegR
		bhvrOverride
			PositionOffset	0 .25 0
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R4_02
		Type	Local
		At	C_Hind_LLegR
		bhvrOverride
			PYRRotate	0 0 0
			Scale		.1 1 1
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R4_03
		Type	Local
		At	C_Hind_FootR
		bhvrOverride
			PYRRotate	0 0 0 
		End

#		Geom	Testing_TargetB
	End
	
	Event
		EName	R4_04
		Type	Local
		At	C_Hind_ToeR
		bhvrOverride
			PositionOffset	0 0 0
			PYRRotate	0 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	R4_End
		Type	Local
		At	C_Hind_ToeR
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
		EName	Trail_R4_01
		Type	Local
		At	R4_Start
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R4_02
		PMagnet	R4_02

	End

	Event
		EName	Trail_R4_02
		Type	Local
		At	R4_02
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R4_03
		PMagnet	R4_03

	End

	Event
		EName	Trail_R4_03
		Type	Local
		At	R4_03
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R4_04
		PMagnet	R4_04

	End

	Event
		EName	Trail_R4_04
		Type	Local
		At	R4_04
		
		part1	:P_Streak_RecluseKnockUpMove.part
		part2	:P_Streak_RecluseKnockUpMove01.part
		POther	R4_End
		PMagnet	R4_End

	End

	Event
		EName	Trail_R4_02_Joint
		Type	Local
		At	R4_02
		
		part2	:P_Streak_RecluseKnockUpMove01.part
	End

	Event
		EName	Trail_R4_03_Joint
		Type	Local
		At	R4_03
		
		part2	:P_Streak_RecluseKnockUpMove01.part
	End
End




End	

	