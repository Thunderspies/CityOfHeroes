#############################################################
## SecurityField.fx
#############################################################

FxInfo

#############################################################
###	CYCLING BEAMS AND GLOWS	  ###########################
#############################################################

Condition
	On	Time
	Time	0


	Event
		EName	EMOffsets
		Type	StartPositOnly
		At	Root
		geom	_LaserAlcoveOffset
		Sound 5thColumnPortal_loop 200 200 0.25
	End

	Event
		Ename	EMOffset1
		Type	Local
		At	EMOffsets
		AltPiv	1
		Part	:BeamFlare.part
		Part	:BaseGlow.part
		Part	:BeamCoreBase.part

		POther	EMOffset5
		PMagnet	EMOffset5
	End

	Event
		Ename	EMOffset2
		Type	Local
		At	EMOffsets
		AltPiv	2
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
		Part	:BeamFlare.part
		Part	:BaseGlow.part
		POther	EMOffset6
		PMagnet	EMOffset6
	End

	Event
		Ename	EMOffset3
		Type	Local
		At	EMOffsets
		AltPiv	3
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
		Part	:BeamFlare.part
		Part	:BaseGlow.part
		POther	EMOffset5
		PMagnet	EMOffset5
	End

	Event
		Ename	EMOffset4
		Type	Local
		At	EMOffsets
		AltPiv	4
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
		Part	:BeamFlare.part
		Part	:BaseGlow.part
		POther	EMOffset2
		PMagnet	EMOffset2
	End



	Event
		Ename	EMOffset5
		Type	Local
		At	EMOffsets
		AltPiv	5
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
		Part	:BeamFlare.part
		Part	:BaseGlow.part
		POther	EMOffset1
		PMagnet	EMOffset1
	End



	Event
		Ename	EMOffset6
		Type	Start
		At	EMOffsets
		AltPiv	6
		Part	:BeamCoreBase.part
		Part	:BeamFlare.part
		Part	:BaseGlow.part
		POther	EMOffset3
		PMagnet	EMOffset2
	End

	Event
		Ename	EMOffset7
		Type	Start
		At	EMOffsets
		AltPiv	7
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
		Part	:BeamFlare.part
		Part	:BaseGlow.part
		POther	EMOffset3
		PMagnet	EMOffset3
	End

	Event
		Ename	EMOffset8
		Type	Start
		At	EMOffsets
		AltPiv	8
		Part	:BeamSmall.part
		Part	:BeamCoreSmall.part
		Part	:BeamCoreBase.part
		Part	:BeamFlare.part
		Part	:BaseGlow.part
		POther	EMOffset4
		PMagnet EMOffset4
	End
End