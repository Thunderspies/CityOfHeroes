
FxInfo

LifeSpan	150

##############################################################################################
######  build up stretch from second to third joint of backpack arms  
######  POther node moves from second to third joint of backpack arms.  
######  Line particle Emitter from Base bone to POther
##############################################################################################

Condition
	On Time 
	Time 0


	Event	
		EName	L01_Travel
		Type	Start
		At	C_Fore_FootL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Fore_ToeL
		LookAt	C_Fore_ToeL
	End

	Event
		EName	CB1_01_Line
		Type 	Local
		At	C_Fore_FootL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLine.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLine.part
##		Geom	Testing_TargetB
		POther	L01_Travel
	End

########  

	Event	
		EName	R01_Travel
		Type	Start
		At	C_Fore_FootR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Fore_ToeR
		LookAt	C_Fore_ToeR
	End

	Event
		EName	CB2_01_Line
		Type 	Local
		At	C_Fore_FootR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLine.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLine.part
##		Geom	Testing_TargetB
		POther	R01_Travel
	End

########
#######  second left and right crab arms 
########

	Event	
		EName	L02_Travel
		Type	Start
		At	C_FootL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_ToeL
		LookAt	C_ToeL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	C_FootL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLine.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLine.part
##		Geom	Testing_TargetB
		POther	L02_Travel
	End

########

	Event	
		EName	R02_Travel
		Type	Start
		At	C_FootR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_ToeR
		LookAt	C_ToeR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	C_FootR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLine.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLine.part
##		Geom	Testing_TargetB
		POther	R02_Travel
	End

########
#######  third left and right crab arms 
########

	Event	
		EName	L03_Travel
		Type	Start
		At	C_HandL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_RingL
		LookAt	C_RingL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	C_HandL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLine.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLine.part
##		Geom	Testing_TargetB
		POther	L03_Travel
	End

########

	Event	
		EName	R03_Travel
		Type	Start
		At	C_HandR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_RingR
		LookAt	C_RingR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	C_HandR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLine.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLine.part
##		Geom	Testing_TargetB
		POther	R03_Travel
	End

########
#######  fourth left and right crab arms 
########

	Event	
		EName	L04_Travel
		Type	Start
		At	C_Hind_FootL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Hind_ToeL
		LookAt	C_Hind_ToeL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	C_Hind_FootL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLine.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLine.part
##		Geom	Testing_TargetB
		POther	L04_Travel
	End

########


	Event	
		EName	R04_Travel
		Type	Start
		At	C_Hind_FootR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Hind_ToeR
		LookAt	C_Hind_ToeR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	C_Hind_FootR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLine.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLine.part
##		Geom	Testing_TargetB
		POther	R04_Travel
	End

End





End