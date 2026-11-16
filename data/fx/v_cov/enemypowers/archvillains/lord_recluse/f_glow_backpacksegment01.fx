
FxInfo

LifeSpan	150

##############################################################################################
######  build up stretch of line emitter from base of pack to first joint 
######  POther node moves from base to first joint of backpack.  
######  Line particle Emitter from Base bone to POther
##############################################################################################

Condition
	On Time 
	Time 0


	Event	
		EName	L01_Travel
		Type	Start
		At	C_Fore_ULegL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Fore_LlegL
		LookAt	C_Fore_LlegL
	End

	Event
		EName	CB1_01_Line
		Type 	Local
		At	C_Fore_ULegL
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
		At	C_Fore_ULegR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Fore_LlegR
		LookAt	C_Fore_LlegR
	End

	Event
		EName	CB2_01_Line
		Type 	Local
		At	C_Fore_ULegR
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
		At	C_ULegL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_LlegL
		LookAt	C_LlegL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	C_ULegL
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
		At	C_ULegR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_LlegR
		LookAt	C_LlegR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	C_ULegR
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
		At	C_UArmL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_LArmL
		LookAt	C_LArmL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	C_UArmL
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
		At	C_UArmR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_LArmR
		LookAt	C_LArmR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	C_UArmR
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
		At	C_Hind_ULegL
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Hind_LLegL
		LookAt	C_Hind_LLegL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	C_Hind_ULegL
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
		At	C_Hind_ULegR
		
		bhvr	:B_Travel_RecluseEnergyLine.bhvr
#		Geom	Testing_TargetB
		Magnet	C_Hind_LLegR
		LookAt	C_Hind_LLegR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	C_Hind_ULegR
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