#########################################################
##	Fireweapon
########################################################
FxInfo

#####################################################################################
##################################  Right 01 Emit  ##################################
#####################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	EmitterL01
		Type	local
		At	Chest
		BhvrOverride
			behavior
			PositionOffset		-2.325 -.4 -.225
		End

		bhvr	ENEMYFX\V_LongBow\LB_B_SkiffEmitR1.bhvr
#		Geom	Testing_TargetB
	End

End

#####################################################################################
##################################  Right 01 Jet  ###################################
#####################################################################################

Condition
	On	Time
	Time	1

	Event
		EName 	JetL
		Type	local
		At	EmitterL01		
		
#		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsEmit.part
		Part2	ENEMYFX\V_LongBow\LB_P_SkiffThrustCore.part
		Part3	ENEMYFX\V_LongBow\LB_P_SkiffThrustSource.part
#		Part3	ENEMYFX\V_LongBow\LB_P_ThrustFlare.part	
	End

	Event
		EName	L01_a
		Type	Local
		At	EmitterL01
		BhvrOverride
			Behavior
			PositionOffset 0 -.75 0
		End

		Part1	ENEMYFX\V_LongBow\LB_P_SkiffThrustCloud.part
	End
End

#####################################################################################
##################################  Left  01 Emit  ##################################
#####################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	EmitterR01
		Type	local
		At	Chest
		
		bhvr	ENEMYFX\V_LongBow\LB_B_SkiffEmitR1.bhvr
#		Geom	Testing_TargetB
	End
End

#####################################################################################
##################################  Left  01 Jet  ###################################
#####################################################################################

Condition
	On	Time
	Time	1

	Event
		EName 	JetR
		Type	local
		At	EmitterR01		
		
#		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsEmit.part
		Part2	ENEMYFX\V_LongBow\LB_P_SkiffThrustCore.part
		Part3	ENEMYFX\V_LongBow\LB_P_SkiffThrustSource.part

#		Part3	ENEMYFX\V_LongBow\LB_P_ThrustFlare.part
		Sound glow7_loop 70 70 .66
	End

	Event
		EName	FlareRim
		Type	Local
		At	EmitterR01
		BhvrOverride
			Behavior
			PositionOffset 0 -.75 0
		End

		Part1	ENEMYFX\V_LongBow\LB_P_SkiffThrustCloud.part
	End
End

#####################################################################################
##################################  Right 02 Emit  ##################################
#####################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	EmitterR02
		Type	local
		At	Chest
		BhvrOverride
			behavior
			PositionOffset		4.6 -.95 -.65
			Scale			1 1 1
		End
		
		bhvr	ENEMYFX\V_LongBow\LB_B_SkiffEmitR1.bhvr
#		Geom	Testing_TargetB
	End
End

#####################################################################################
##################################  Right 02 Jet  ###################################
####################################################################################
Condition
	On	Time
	Time	1

	Event
		EName 	JetR02
		Type	local
		At	EmitterR02		
		
#		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsEmit.part
		Part2	ENEMYFX\V_LongBow\LB_P_SkiffThrustCore02.part
		Part3	ENEMYFX\V_LongBow\LB_P_SkiffThrustSource.part
#		Part3	ENEMYFX\V_LongBow\LB_P_ThrustFlare.part	
	End

	Event
		EName	R02_a
		Type	Local
		At	EmitterR02
		BhvrOverride
			Behavior
			PositionOffset 0 -.85 0
		End

		Part1	ENEMYFX\V_LongBow\LB_P_SkiffThrustCloud02.part
	End
End

#####################################################################################
##################################  Left  02 Emit  ##################################
#####################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	EmitterL02
		Type	local
		At	Chest
		BhvrOverride
			behavior
			PositionOffset		-4.6 -.95 -.65
			Scale			1 1 1
		End
		
		bhvr	ENEMYFX\V_LongBow\LB_B_SkiffEmitR1.bhvr
#		Geom	Testing_TargetB
	End
End

####################################################################################
##################################  Left 02 Jet  ###################################
####################################################################################
Condition
	On	Time
	Time	1

	Event
		EName 	JetL02
		Type	local
		At	EmitterL02		
		
#		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsEmit.part
		Part2	ENEMYFX\V_LongBow\LB_P_SkiffThrustCore02.part
		Part3	ENEMYFX\V_LongBow\LB_P_SkiffThrustSource.part
#		Part3	ENEMYFX\V_LongBow\LB_P_ThrustFlare.part	
	End

	Event
		EName	L02_a
		Type	Local
		At	EmitterL02
		BhvrOverride
			Behavior
			PositionOffset 0 -.85 0
		End

		Part1	ENEMYFX\V_LongBow\LB_P_SkiffThrustCloud02.part
	End
End

#####################################################################################
##################################     Testing     ##################################
#####################################################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Test
#		Type	local
#		At	Chest
#		BhvrOverride
#			behavior
#			PositionOffset		0 -5 -5
#			Scale			1 1 1
#		End
#		
#		bhvr	ENEMYFX\V_LongBow\LB_B_SkiffEmitR1.bhvr
##		Geom	Testing_TargetB
#	End
#End
#
#Condition
#	On	Time
#	Time	1
#
#	Event
#		EName 	JetR
#		Type	local
#		At	Test		
#		
#		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsEmit.part
#		Part2	ENEMYFX\V_LongBow\LB_P_ThrustFlareCore.part
#		Part3	ENEMYFX\V_LongBow\LB_P_ThrustFlare.part
#		Sound	glow7_loop 70 65 .66	
#	End
#
#	Event
#		EName	FlareRim
#		Type	Local
#		At	Test
#		BhvrOverride
#			Behavior
#			PositionOffset 0 -.15 0
#		End
#
#		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRings.part
#	End
#
#	Event
#		EName	FlareRim
#		Type	Local
#		At	Test
#		BhvrOverride
#			Behavior
#			PositionOffset 0 -.5 0
#		End
#
#		Part1	ENEMYFX\V_LongBow\LB_P_Smoke.part
#	End
#	
#End




End