#########################################################
##	forceBubble
########################################################
FxInfo

	
#####################
##Sound
############################################








######################################################################################################

Condition
	On	Time
	Time	0

	Event	
		EName	RefTop01
		Type	Local
		At	Root
		bhvrOverride
			behavior
			scale		4 4 4
			PositionOffset	.5 22 0
			Spin		0 .02 0
		End
#		Geom	Testing_TargetB
	End

	Event
		EName	RefTopChild01
		Type	Local
		At	RefTop01
		bhvrOverride
			behavior
			PositionOffset	1 0 0
		End

#		Geom	Testing_TargetB
	End

######################################################################################################

	Event	
		EName	RefTop02
		Type	Local
		At	Root
		bhvrOverride
			behavior
			scale		4 4 4
			PositionOffset	.5 20 0
			Spin		0 -.02 0
		End
#		Geom	Testing_TargetB
	End

	Event
		EName	RefTopChild02
		Type	Local
		At	RefTop02
		bhvrOverride
			behavior
			PositionOffset	1 0 0
		End

#		Geom	Testing_TargetB
	End

######################################################################################################

	Event
		EName	Emitter01
		Type	Local
		At	Origin
		BhvrOverride
			behavior
#			Spin			0 .05 0
		End
		
		bhvr	V_COV\BaseFx\ItemsOfPower\Heart_B_ChildAdjust.bhvr
#		Geom	Testing_TargetB		
	End

	Event
		EName	AttractorParent
		Type	Local
		At	Emitter01

		BhvrOverride
			behavior
			PositionOffset		0 -1 0
#			Spin			0 .05 0
		End

#		Geom	Testing_TargetB		
	End

	Event
		EName	AttractorParent01
		Type	Local
		At	AttractorParent

		BhvrOverride
			behavior
			PositionOffset		2 0 0
			Spin			0.05 -0.01 0
		End

#		Geom	Testing_TargetB		
	End

	Event
		EName	AttractorParent02
		Type	Local
		At	AttractorParent

		BhvrOverride
			behavior
			PositionOffset		-2 0 0
			Spin			-0.05 -0.01 0
		End

#		Geom	Testing_TargetB		
	End

#########################################################################################
#########################################################################################
#########################################################################################

	Event
		EName	Attractor
		Type	Local
		At	AttractorParent02

		BhvrOverride
			behavior
			PositionOffset		0 1.5 0
		End

#		Geom	Testing_TargetB		
	End

	Event
		EName	Attractor01
		Type	Local
		At	AttractorParent01

		BhvrOverride
			behavior
			PositionOffset		0 -1.5 0
		End

#		Geom	Testing_TargetB		
	End


End

Condition
	On	Time
	time	2

	Event
		EName	Smoke
		Type	Local
		At	Emitter01
		
		bhvr	V_COV\BaseFx\ItemsOfPower\Heart_B_Adjust.bhvr
#		Part1	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke.part
#		Part2	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke01.part
		PMagnet	Attractor		
	End

	Event
		EName	Smoke01
		Type	Local
		At	Emitter01
		
		bhvr	V_COV\BaseFx\ItemsOfPower\Heart_B_Adjust.bhvr
#		Part1	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke.part
#		Part2	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke01.part
		PMagnet	Attractor01
		
	End

	Event
		EName	Smoke02
		Type	Local
		At	Emitter01

		bhvr	V_COV\BaseFx\ItemsOfPower\Heart_B_Adjust.bhvr
#		Part1	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke.part
#		Part2	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke01.part
		Part3	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTop.part
#		Part4	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTopFiller.part
		PMagnet	RefTopChild01		
	End

	Event
		EName	Smoke03
		Type	Local
		At	Emitter01

		bhvr	V_COV\BaseFx\ItemsOfPower\Heart_B_Adjust.bhvr
#		Part2	V_COV\BaseFx\ItemsOfPower\Heart_P_WispSmoke01.part
		Part3	V_COV\BaseFx\ItemsOfPower\Heart_P_WispMagTop01.part
		PMagnet	RefTopChild02		
	End
End

Condition
