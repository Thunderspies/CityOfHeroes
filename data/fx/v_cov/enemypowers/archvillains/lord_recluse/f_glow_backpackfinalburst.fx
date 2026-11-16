
FxInfo

##############################################################################################
######  build up stretch from last joint to tip of backpack    
######  POther node moves from last joint to tip of backpack  
######  Line particle Emitter from Base bone to POther
##############################################################################################


LifeSpan	10

##############################################################################################
######  build up stretch from base of pack to first joint  -  
##############################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	Fore_ToeTipL
		Type	Local
		At	C_Fore_ToeL
		bhvrOverride
			PositionOffset	0 2 .075
		End
#		Geom	Testing_TargetB
	End
End

##############################################################################################
######  energy build up in last arm links 
##############################################################################################

Condition
	On Time 
	Time 0

	Event
		EName	Fore_ToeTipL
		Type	Local
		At	C_Fore_ToeL

		bhvr	:B_Travel_RecluseArmTips.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	L01_Travel
		Type	Start
		At	Fore_ToeTipL
		
		bhvr	:B_Travel_RecluseEnergyLineFaster.bhvr
##		Geom	Testing_TargetB
		Magnet	Fore_ToeTipL
		LookAt	Fore_ToeTipL
	End

	Event
		EName	CB1_01_Line
		Type 	Local
		At	Fore_ToeTipL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineBiggerBURST.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLineBiggerBURST.part
###		Geom	Testing_TargetB
		POther	L01_Travel
	End

#######  

	Event
		EName	Fore_ToeTipR
		Type	Local
		At	C_Fore_ToeR
		
		bhvr	:B_Travel_RecluseArmTips.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	R01_Travel
		Type	Start
		At	Fore_ToeTipR
		
		bhvr	:B_Travel_RecluseEnergyLineFaster.bhvr
##		Geom	Testing_TargetB
		Magnet	Fore_ToeTipR
		LookAt	Fore_ToeTipR
	End

	Event
		EName	CB2_01_Line
		Type 	Local
		At	Fore_ToeTipR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineBiggerBURST.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLineBiggerBURST.part
###		Geom	Testing_TargetB
		POther	R01_Travel
	End

########
#######  second left and right crab arms 
########

	Event
		EName	FootTipL
		Type	Local
		At	C_ToeL
		
		bhvr	:B_Travel_RecluseArmTips.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	L02_Travel
		Type	Start
		At	FootTipL
		
		bhvr	:B_Travel_RecluseEnergyLineFaster.bhvr
##		Geom	Testing_TargetB
		Magnet	FootTipL
		LookAt	FootTipL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	FootTipL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineBiggerBURST.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLineBiggerBURST.part
###		Geom	Testing_TargetB
		POther	L02_Travel
	End

########

	Event
		EName	FootTipR
		Type	Local
		At	C_ToeR
		
		bhvr	:B_Travel_RecluseArmTips.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	R02_Travel
		Type	Start
		At	FootTipR
		
		bhvr	:B_Travel_RecluseEnergyLineFaster.bhvr
##		Geom	Testing_TargetB
		Magnet	FootTipR
		LookAt	FootTipR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	FootTipR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineBiggerBURST.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLineBiggerBURST.part
###		Geom	Testing_TargetB
		POther	R02_Travel
	End

########
#######  third left and right crab arms 
########

	Event
		EName	RingTipL
		Type	Local
		At	C_RingL
		bhvrOverride
			PositionOffset	-2 0 .075
		End
		
		bhvr	:B_Travel_RecluseArmTips.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	L03_Travel
		Type	Start
		At	RingTipL
		
		bhvr	:B_Travel_RecluseEnergyLineFaster.bhvr
##		Geom	Testing_TargetB
		Magnet	RingTipL
		LookAt	RingTipL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	RingTipL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineBiggerBURST.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLineBiggerBURST.part
###		Geom	Testing_TargetB
		POther	L03_Travel
	End

########

	Event
		EName	RingTipR
		Type	Local
		At	C_RingR
		bhvrOverride
			PositionOffset	2 0 .075
		End
		
#		bhvr	:B_Travel_RecluseArmTips.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	R03_Travel
		Type	Start
		At	RingTipR
		
		bhvr	:B_Travel_RecluseEnergyLineFaster.bhvr
##		Geom	Testing_TargetB
		Magnet	RingTipR
		LookAt	RingTipR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	RingTipR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineBiggerBURST.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLineBiggerBURST.part
###		Geom	Testing_TargetB
		POther	R03_Travel
	End

########
#######  fourth left and right crab arms 
########

	Event
		EName	Hind_ToeTipL
		Type	Local
		At	C_Hind_ToeL
		bhvrOverride
			PositionOffset	0 -2 .075
		End
		
		bhvr	:B_Travel_RecluseArmTips.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	L04_Travel
		Type	Start
		At	Hind_ToeTipL
		
		bhvr	:B_Travel_RecluseEnergyLineFaster.bhvr
##		Geom	Testing_TargetB
		Magnet	Hind_ToeTipL
		LookAt	Hind_ToeTipL
	End

	Event
		EName	CB1_03_Line
		Type 	Local
		At	Hind_ToeTipL
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineBiggerBURST.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLineBiggerBURST.part
###		Geom	Testing_TargetB
		POther	L04_Travel
	End

########

	Event
		EName	Hind_ToeTipR
		Type	Local
		At	C_Hind_ToeR
		bhvrOverride
			PositionOffset	0 -2 .075
		End
		
		bhvr	:B_Travel_RecluseArmTips.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	R04_Travel
		Type	Start
		At	Hind_ToeTipR
		
		bhvr	:B_Travel_RecluseEnergyLineFaster.bhvr
##		Geom	Testing_TargetB
		Magnet	Hind_ToeTipR
		LookAt	Hind_ToeTipR
	End

	Event
		EName	CB2_04_Line
		Type 	Local
		At	Hind_ToeTipR
		bhvrOverride
			HueShift	15
		End

		part1	:P_Glow_RecluseLineBiggerBURST.part
		part2	:P_GlowSparks_RecluseLine.part
		part3	:P_GlowCore_RecluseLineBiggerBURST.part
###		Geom	Testing_TargetB
		POther	R04_Travel
	End

End





End