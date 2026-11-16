#############################################################
## FifthCol_Portal.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offsets
		Type	Start
		At	Root
		geom	GEO_5thColumnPortalOffsets
		Sound 5thColumnPortal_loop 200 200 0.25
	End

#############################################################
###	PORTAL		  ###################################
#############################################################

	Event
		EName	Offset
		Type	Start
		At	Offsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
		Part	:Oval.part
		Part	:Oval_Fill_Alpha.part
		Part	:Oval_Shrinking.part
		Part	:Core_Tendrils.part
	End


#############################################################
###	ELECTRIC BEAMS	  ###################################
#############################################################

	Event
		EName	Offset2
		Type	Start
		At	Offsets
		AltPiv	2
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam2.part
		Part	:Embers_Traveling.part
		PMagnet	Offset
	End

	Event
		EName	Offset3
		Type	Start
		At	Offsets
		AltPiv	3
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam2.part
		PMagnet	Offset
	End

	Event
		EName	Offset4
		Type	Start
		At	Offsets
		AltPiv	4
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam2.part
		Part	:Embers_Traveling.part
		PMagnet	Offset
	End


	Event
		EName	Offset5
		Type	Start
		At	Offsets
		AltPiv	5
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam2.part
		PMagnet	Offset
	End

	Event
		EName	Offset6
		Type	Start
		At	Offsets
		AltPiv	6
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam.part
		Part	:Embers_Traveling.part
		PMagnet	Offset
	End

	Event
		EName	Offset7
		Type	Start
		At	Offsets
		AltPiv	7
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam.part
		PMagnet	Offset
	End

	Event
		Ename	Offset8
		Type	Start
		At	Offsets
		AltPiv	8
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam2.part
		Part	:Embers_Traveling.part
		PMagnet	Offset
	End

	Event
		Ename	Offset9
		Type	Start
		At	Offsets
		AltPiv	9
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam2.part
		PMagnet	Offset
	End

	Event
		Ename	Offset10
		Type	Start
		At	Offsets
		AltPiv	10
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam2.part
		Part	:Embers_Traveling.part
		PMagnet	Offset
	End


	Event
		Ename	Offset11
		Type	Start
		At	Offsets
		AltPiv	11
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam2.part
		PMagnet	Offset
	End

	Event
		Ename	Offset12
		Type	Start
		At	Offsets
		AltPiv	12
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam.part
		Part	:Embers_Traveling.part
		PMagnet	Offset
	End

	Event
		Ename	Offset13
		Type	Start
		At	Offsets
		AltPiv	13
		Part	:SmallBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:ElectricBeam.part
		PMagnet	Offset
	End
End

#############################################################
###	CYCLING BEAMS AND GLOWS	  ###########################
#############################################################

Condition
	On	Cycle
	Time	27

	Event
		Type	Start
		At	Offsets
		AltPiv	2
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	9
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20
	End
End

Condition
	On	Cycle
	Time	48

	Event
		Type	Start
		At	Offsets
		AltPiv	10
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	4
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20

	End

	Event
		Type	Start
		At	Offsets
		AltPiv	12
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20

	End


End

Condition
	On	Cycle
	Time	75

	Event
		Type	Start
		At	Offsets
		AltPiv	6
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	8
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	11
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20
	End
End


Condition
	On	Cycle
	Time	40

	Event
		Type	Start
		At	Offsets
		AltPiv	3
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	5
		Part	:ElectricBeamQuick.part
		Part	:GlowQuick.part
		PMagnet	Offset
		Lifespan 20

	End
End

#############################################################

End