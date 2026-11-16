#############################################################
## EMachine.fx
#############################################################

FxInfo
########################################################


#############################################################
###	CYCLING BEAMS AND GLOWS	  ###########################
#############################################################

Condition
	On	Time
	Time	0


	Event
		EName	EMOffsets
		Type	Start
		At	Root
		geom	FX_EmpowermentAltPivots
		Sound 5thColumnPortal_loop 125 125 0.1
	End

	Event
		Ename	EMOffset2
		Type	Start
		At	EMOffsets
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
	End

	Event
		Ename	EMOffset3

		Type	Start
		At	EMOffsets
		AltPiv	3
		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
	End

	Event
		Ename	EMOffset4

		Type	Start
		At	EMOffsets
		AltPiv	4
		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
	End

	Event
		Ename	EMOffset4

		Type	Start
		At	EMOffsets
		AltPiv	5
		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
	End

	Event
		Ename	EMOffset4

		Type	Start
		At	EMOffsets
		AltPiv	6
		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
	End

	Event
		Ename	EMOffset4

		Type	Start
		At	EMOffsets
		AltPiv	7
		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
	End
End