
FxInfo

LifeSpan	150

##############################################################################################
######  build up stretch from first joint of pack to second joint  -  
######  POther node moves from first to second joint of backpack arms.  
######  Line particle Emitter from Base bone to POther
##############################################################################################

Condition
	On Time 
	Time 0


	Event	
		EName	L01_Travel
		Type	Start
		At	C_Fore_LLegL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Fore_FootL
		LookAt	C_Fore_FootL
	End

	Event
		EName	CB1_01_Line
		Type 	Local
		At	C_Fore_LLegL
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
		At	C_Fore_LLegR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Fore_FootR
		LookAt	C_Fore_FootR
	End

	Event
		EName	CB2_01_Line
		Type 	Local
		At	C_Fore_LLegR
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
		At	C_LLegL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_FootL
		LookAt	C_FootL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	C_LLegL
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
		At	C_LLegR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_FootR
		LookAt	C_FootR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	C_LLegR
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
		At	C_LArmL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_HandL
		LookAt	C_HandL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	C_LArmL
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
		At	C_LArmR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_HandR
		LookAt	C_HandR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	C_LArmR
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
		At	C_Hind_LLegL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Hind_FootL
		LookAt	C_Hind_FootL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	C_Hind_LLegL
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
		At	C_Hind_LLegR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Hind_FootR
		LookAt	C_Hind_FootR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	C_Hind_LLegR
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