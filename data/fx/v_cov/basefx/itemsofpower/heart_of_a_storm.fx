#########################################################
##	forceBubble
########################################################
FxInfo

Condition
	On	Time
	Time	0

	Event
		EName	Ref
		Type	Local
		At	Root
		bhvrOverride
			behavior
			PositionOffset	0 7.5 0
		End

#		Part	:Heart_Aura_Glow.part
		Part	:Heart_Aura_Glow2.part
		Part	:Heart_Aura_Smoke.part
#		Part	:Heart_Aura_GlowGreen.part
#		Part	:Heart_Aura_Sparks.part
	End

	Event
		EName	Ref
		Type	Local
		At	Root
		bhvrOverride
			behavior
			scale		7 7 7
			PositionOffset	-1 0 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	RefTop
		Type	Local
		At	Root
		bhvrOverride
			behavior
			scale		4 4 4
			PositionOffset	-.75 25 0
		End

#		Geom	Testing_TargetB
	End

	Event
		EName	Rig01
		Type	Local
		At	Root
		bhvrOverride
			behavior
			PositionOffset	-.75 0 0
		End

		Geom	GEO_Heart_FXRig01
	End

	Event
		EName	Rig02
		Type	Local
		At	Root
		bhvrOverride
			behavior
			PositionOffset	-.75 0 0
		End

		Geom	GEO_Heart_FXRig02
	End
End

######################################################################################################
###############################  Lower Gas Emitters  #################################################
######################################################################################################


Condition
	On	Time
	Time	1

	Event
		EName	Rig01_C01
		Type	Local
		At	Rig01
		  AltPiv 1

		ChildFX	V_COV\BaseFx\ItemsOfPower\Heart_ChildSmoke.fx
#		Geom	Testing_TargetB
	End

	Event
		EName	Glow_C01
		Type	Local
		At	Rig01_C01
		bhvrOverride
			behavior
			PositionOffset		-.1 -.27 0
		End

#		Geom	Testing_TargetB
		Part4	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTopFiller.part
	End
End

######################################################################################################

Condition
	On	Time
	Time	5

	Event
		EName	Rig01_C02
		Type	Local
		At	Rig01
		  AltPiv 2

		ChildFX	V_COV\BaseFx\ItemsOfPower\Heart_ChildSmoke.fx
#		Geom	Testing_TargetB
	End

	Event
		EName	C02_Target01
		Type	Local
		At	Rig01_C02
		bhvrOverride
			behavior
			PositionOffset		0.1 -0.27 -1.0
		End

#		Geom	Testing_Targetb
	End

	Event
		EName	Glow_C02
		Type	Local
		At	Rig01_C02
		bhvrOverride
			behavior
			PositionOffset		-.25 -.27 .5
		End

#		Geom	Testing_TargetB
		Part4	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTopFiller01.part
		POther	C02_Target01
	End
End

######################################################################################################

Condition
	On	Time
	Time	12

	Event
		EName	Rig01_C03
		Type	Local
		At	Rig01
		  AltPiv 3

		ChildFX	V_COV\BaseFx\ItemsOfPower\Heart_ChildSmoke.fx
#		Geom	Testing_TargetB
	End

	Event
		EName	Glow_C03
		Type	Local
		At	Rig01_C03
		bhvrOverride
			behavior
			PositionOffset		0.1 0.1 .10
		End

#		Geom	Testing_TargetB
		Part4	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTopFiller.part
	End
End

######################################################################################################

Condition
	On	Time
	Time	17

	Event
		EName	Rig01_C04
		Type	Local
		At	Rig01
		  AltPiv 4

		ChildFX	V_COV\BaseFx\ItemsOfPower\Heart_ChildSmoke.fx
#		Geom	Testing_TargetB
	End

	Event
		EName	C04_Target01
		Type	Local
		At	Rig01_C04
		bhvrOverride
			behavior
			PositionOffset		0.1 0.1 -1.5
		End

#		Geom	Testing_Targetb
	End

	Event
		EName	Glow_C04
		Type	Local
		At	Rig01_C04
		bhvrOverride
			behavior
			PositionOffset		-.65 0.1 .65
		End

#		Geom	Testing_TargetB
		Part4	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTopFiller01.part
		POther	C04_Target01
	End
End

######################################################################################################
###############################  Upper Gas Emitters  #################################################
######################################################################################################

Condition
	On	Time
	Time	22

	Event
		EName	Rig02_C01
		Type	Local
		At	Rig02
		  AltPiv 1
		bhvrOverride
			behavior
			PositionOffset		0 -.2 -.2
			Scale			.1 .1 .1
		End

#		ChildFX	V_COV\BaseFx\ItemsOfPower\Heart_ChildSmoke.fx
		Part4	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTopFiller.part
#		Geom	Testing_TargetB
	End
End
#
Condition
	On	Time
	Time	24

	Event
		EName	Rig02_C02
		Type	Local
		At	Rig02
		  AltPiv 2
		bhvrOverride
			behavior
			PositionOffset		0 0 -.2
			Scale			.1 .1 .1
		End

#		ChildFX	V_COV\BaseFx\ItemsOfPower\Heart_ChildSmoke.fx
		Part4	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTopFiller.part
#		Geom	Testing_TargetB
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	Rig02_C03
		Type	Local
		At	Rig02
		  AltPiv 3
		bhvrOverride
			behavior
			PositionOffset		0 0 -.1
			Scale			.1 .1 .1
		End

#		ChildFX	V_COV\BaseFx\ItemsOfPower\Heart_ChildSmoke.fx
		Part4	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTopFiller.part
#		Geom	Testing_TargetB
	End
End

Condition
	On	Time
	Time	35

	Event
		EName	Rig02_C04
		Type	Local
		At	Rig02
		  AltPiv 4
		bhvrOverride
			behavior
			PositionOffset		0 0 -.1
			Scale			.1 .1 .1
		End


#		ChildFX	V_COV\BaseFx\ItemsOfPower\Heart_ChildSmoke.fx
		Part4	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTopFiller.part
#		Geom	Testing_TargetB
	End
End

######################################################################################################
#################################  Overall Gas glow  #################################################
######################################################################################################

Condition
	On	Time
	Time	1

	Event
		EName	Glow
		Type	Local
		At	Rig01
		BhvrOverride
			behavior
			positionOffset	0 8.5 0
		End

#		Part1	V_COV\BaseFx\ItemsOfPower\Heart_P_Glow.part
		#Geom	Testing_TargetB
		PMagnet	RefTop
	End
End


#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	Emitter01
#		Type	Local
#		At	Root
#
#		BhvrOverride
#			behavior
#			PositionOffset		0 4 2
#		End
#
#		Geom	Testing_TargetB
#	End
#
#	Event
#		EName	AttractorParent
#		Type	Local
#		At	Emitter01
#
#		BhvrOverride
#			behavior
#			PositionOffset		0 3 0
#			Spin			0 .05 0
#		End
#
##		Geom	Testing_TargetB
#	End
#
#	Event
#		EName	AttractorParent01
#		Type	Local
#		At	AttractorParent
#
#		BhvrOverride
#			behavior
#			PositionOffset		2 0 0
#			Spin			0.05 -0.01 0
#		End
#
##		Geom	Testing_TargetB
#	End
#
#	Event
#		EName	AttractorParent02
#		Type	Local
#		At	AttractorParent
#
#		BhvrOverride
#			behavior
#			PositionOffset		-2 0 0
#			Spin			-0.05 -0.01 0
#		End
#
##		Geom	Testing_TargetB
#	End
#
##########################################################################################
##########################################################################################
##########################################################################################
#
#	Event
#		EName	Attractor
#		Type	Local
#		At	AttractorParent02
#
#		BhvrOverride
#			behavior
#			PositionOffset		0 1.5 0
#		End
#
##		Geom	Testing_TargetB
#	End
#
#	Event
#		EName	Attractor01
#		Type	Local
#		At	AttractorParent01
#
#		BhvrOverride
#			behavior
#			PositionOffset		0 -1.5 0
#		End
#
##		Geom	Testing_TargetB
#	End
#
#
#End
#
#Condition
#	On	Time
#	time	2
#
#	Event
#		EName	Smoke
#		Type	Local
#		At	Emitter01
#
#		Part1	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke.part
#		Part2	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke01.part
#		PMagnet	Attractor
#
#	End
#
#	Event
#		EName	Smoke01
#		Type	Local
#		At	Emitter01
#
#		Part1	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke.part
#		Part2	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke01.part
#		PMagnet	Attractor01
#
#	End
#
#	Event
#		EName	Smoke02
#		Type	Local
#		At	Emitter01
#
#		Part1	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke.part
#		Part2	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke01.part
#		PMagnet	AttractorParent
#
#	End
#End
#
#End
#