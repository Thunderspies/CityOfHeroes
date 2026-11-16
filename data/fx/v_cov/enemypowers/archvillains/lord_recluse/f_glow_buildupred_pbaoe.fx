#########################################################
##	Recluse Ranged Cone Attack
##############################################################################################
##############################################################################################
######   Defs: BC = Energy Ball Core; BE = Energy Ball Core Extent Targets
##############################################################################################
##############################################################################################

FxInfo

LifeSpan	90

##############################################################################################
######  Sound
##############################################################################################

Condition
	On	Time
	Time	7
	
	Event
		EName	EnergyBallCore
		Type	Local
		At	Root
		Sound Powerup2 70 70 .8
	End
End

##############################################################################################
######  Ball Core hands Start
##############################################################################################

Condition
	On	Time
	Time	1

	Event
		EName	Child_HandR
		Type	Local
		At	HandR
		bhvrOverride
			PositionOffset	.25 0 0.02
		End
		ChildFX :F_ChildGlow_RecluseRedHand.fx
		Lifespan 85
	End

	Event
		EName	Child_HandL
		Type	Local
		At	HandL
		bhvrOverride
			PositionOffset	-.25 0 0.02
		End
		ChildFX :F_ChildGlow_RecluseRedHand.fx
		Lifespan 100
	End
End

##############################################################################################
######  build up stretch from hand to elbowt    LOWER ARMs
##############################################################################################

Condition
	On Time 
	Time 3 
	
	Event
		EName	ForeArm_LineL
		Type 	Local
		At	HandL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineHuman.part
		part2	:P_GlowSparks_RecluseLineHuman.part
		part3	:P_GlowCore_RecluseLineHuman.part
#		Geom	Testing_TargetB
		Lifespan  75
		POther	LarmL
	End

	Event
		EName	ForeArm_LineR
		Type 	Local
		At	HandR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineHuman.part
		part2	:P_GlowSparks_RecluseLineHuman.part
		part3	:P_GlowCore_RecluseLineHuman.part
#		Geom	Testing_TargetB
		Lifespan  65
		POther	LarmR
	End
	
End

##############################################################################################
######  Ball Core Elbow 
##############################################################################################

Condition
	On	Time
	Time	5

	Event
		EName	Child_ElbowR
		Type	Local
		At	LarmR
		ChildFX :F_ChildGlow_RecluseRedHand.fx
		Lifespan  45
	End

	Event
		EName	Child_ElbowL
		Type	Local
		At	LarmL
		ChildFX :F_ChildGlow_RecluseRedHand.fx
		Lifespan  50
	End
End

##############################################################################################
######  build up stretch from elbow to upper arm   -     Upper Arms
##############################################################################################

Condition
	On Time 
	Time 8
	
	Event
		EName	Bicep_LineL
		Type 	Local
		At	LArmL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineHuman.part
		part2	:P_GlowSparks_RecluseLineHuman.part
		part3	:P_GlowCore_RecluseLineHuman.part
#		Geom	Testing_TargetB
		Lifespan  30
		POther	UArmL
	End

	Event
		EName	Bicep_LineR
		Type 	Local
		At	LArmR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineHuman.part
		part2	:P_GlowSparks_RecluseLineHuman.part
		part3	:P_GlowCore_RecluseLineHuman.part
#		Geom	Testing_TargetB
		Lifespan  32
		POther	UArmR
	End	
End

##############################################################################################
######  Ball Core Shoulder 
##############################################################################################

Condition
	On	Time
	Time	10

	Event
		EName	Child_ShoulderR
		Type	Local
		At	UarmR
		ChildFX :F_ChildGlow_RecluseRedHand.fx
		Lifespan  25
	End

	Event
		EName	Child_ShoulderL
		Type	Local
		At	UarmL
		ChildFX :F_ChildGlow_RecluseRedHand.fx
		Lifespan  25
	End
End

##############################################################################################
######  Ball Core BackPackBase 
##############################################################################################

Condition
	On	Time
	Time	12

	Event
		EName	Child_CB1_01
		Type	Local
		At	C_Hips
		bhvrOverride
			PositionOffset	0 0 -.2
		End
		ChildFX :F_ChildGlow_RecluseRedBig.fx
		Lifespan  10
	End

	Event
		EName	Child_CB2_01
		Type	Local
		At	C_Hips
		bhvrOverride
			PositionOffset	0 0 -0.2
		End
		ChildFX :F_ChildGlow_RecluseRedBig.fx
		Lifespan  10
	End

End

##############################################################################################
##############################################################################################
#######  start buildup of crab backpack arms

##############################################################################################
#######  build up stretch from base of pack to first joint  -  
###############################################################################################

Condition
	On	Time
	Time	15

	Event
		EName	CB_Child01
		Type	Local
		At	C_Hips

		ChildFX	:F_Glow_BackpackSegment01_PBAoE.fx
	End
End

#######   Knuckle 01 child glow fx and prime to trigger it 

Condition
	On	Time
	Time	13

	Event	
		EName	Prime1
		Type	Start
		At	C_Fore_ULegL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Fore_LlegL
		LookAt	C_Fore_LlegL
	End
End

##############################################################################################
######  build up stretch from first joint to second joint on the arms    
##############################################################################################

Condition
	On	Prime1Hit

	Event
		EName	CB_Child01
		Type	Local
		At	C_Hips

		ChildFX	:F_Glow_BackpackSegment02.fx
	End
End

#######   Knuckle 02 child glow fx and prime to trigger it 

Condition
	On	Prime1Hit

	Event	
		EName	Prime2
		Type	Start
		At	C_Fore_LLegL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Fore_FootL
		LookAt	C_Fore_FootL
	End
End

#############################################################################################
#####  build up stretch from second joint to third joint on the crab arms    
#############################################################################################

Condition
	On	Prime2Hit

	Event
		EName	CB_Child01
		Type	Local
		At	C_Hips

		ChildFX	:F_Glow_BackpackSegment03.fx
	End
End


#############################################################################################
#####  build up stretch from last joint to tip    
#############################################################################################

Condition
	On	Prime2Hit

	Event
		EName	CB_Child01
		Type	Local
		At	C_Hips

		ChildFX	:F_Glow_BackpackSegment04.fx
	End
End

End				