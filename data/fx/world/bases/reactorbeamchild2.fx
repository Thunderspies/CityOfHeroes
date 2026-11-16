#########################################################
##	Villain Base Portal
#########################################################

FxInfo

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound EntrancePortal1_loop 80 80 .58
	End
End



Condition
	On 	Cycle
	Time 	210	#155
	#Chance	0.9

#	Event
#		EName	VerticalOffset
#		Type	Local
#		At	Origin
#		Bhvr	WORLD\Bases\HelixOffset2.bhvr
#		BhvrOverride
#			PositionOffset 0.0 10.0 0.0
#		End
#		Lifespan 200
#	End


	Event
		EName 	Close
		Type	Start
		Bhvr	:HelixSpinner.bhvr
		BhvrOverride
			PositionOffset	0.0 80.0 0.0
		End
		part	:CenterRings.part
#		Pother	VerticalOffset
		Lifespan 200
	End

	Event
		EName 	closeHelix1
		Type	Local
		At	Close
		Bhvr	:HelixOffset.bhvr
		part	:HelixGlow.part
		Lifespan	140
	End

	Event
		EName 	closeHelix2
		Type	Local
		At	Close
		Bhvr	WORLD\Bases\HelixOffset2.bhvr
		part	WORLD\Bases\HelixGlow.part
		Lifespan	140
	End

End

#Condition
#	On 	Cycle
#	Time 	190
#	Chance	0.8
#
#	Event
#		EName 	gravWell
#		Type	Local
#		Magnet	helix2
#		part	V_COV\World\Bases\Portal\PortalMist02.part
#		part	V_COV\World\Bases\Portal\PortalMist01.part
#		part	V_COV\World\Bases\Portal\PortalMist02.part
#
#		Lifespan	5
#	End
#
#End


#Condition
#	On 	Cycle
#	Time 	45
#	Chance	0.5
#
#	Event
#		EName 	Arcs
#		Type	Start
##		part	V_COV\World\Bases\Portal\PortalLightningArcs.part
#		Lifespan	20
#	End
#End
#

########### Second Helix ##################


#Condition
#	On 	Cycle
#	Time 	175
#	Chance	0.9
##
##	Event
##		EName 	Start
##		Type	Start
##		part	V_COV\World\Bases\Portal\PortalRing04.part
##		Lifespan	30
##	End
#
#	Event
#		EName 	Open
#		Type	Start
#		Bhvr	V_COV\World\Bases\Portal\PortalRiftOpener.bhvr
#		Lifespan	30
#	End
#
#	Event
#		EName 	openHelix1
#		Type	Local
#		At	Open
#		Bhvr	WORLD\Bases\HelixOffset.bhvr
#		part	WORLD\Bases\HelixGlow.part
#		Lifespan	90
#	End
#
#	Event
#		EName 	openHelix2
#		Type	Local
#		At	open
#		Bhvr	WORLD\Bases\HelixOffset2.bhvr
#		part	WORLD\Bases\HelixGlow.part
#		Lifespan	90
#	End
#
#End


######### PORTAL MIST ##########
#Condition
#	On 	Time 	#Cycle
#	Time 	0 	#135
#	#Chance	0.6
#
#	Event
#		EName 	flash
#		Type	Start
#		part	V_COV\World\Bases\Portal\PortalRift01.part
#		part	V_COV\World\Bases\Portal\PortalRift02.part
#		part	V_COV\World\Bases\Portal\PortalRift03.part
#		part	V_COV\World\Bases\Portal\PortalRift04.part
#
#		Lifespan	5
#	End
#
#End


####################################
######## OUTER LINES ###############
####################################


Condition
	On	Time
	Time 	0

	Event
		EName 	Spinner
		Type	Start
		At	Root
		Bhvr	V_COV\World\Bases\Portal\PortalRiftSpinner.bhvr

	End

	Event
		EName 	helix2
		Type	Local
		At	Spinner
		Bhvr	behaviors\PortalRiftOffset3.bhvr
		BhvrOverride
			PositionOffset 0.0 90.0 9.0
		End
		Part	:LightBeamsVertical.part  # (squarish light beams)
		Part	:HelixGlow2.part
	End

#	Event
#		EName 	helix2
#		Type	Local
#		At	Spinner
#		Bhvr	behaviors\PortalRiftOffset3.bhvr
#		BhvrOverride
#			PositionOffset 0.0 90.0 9.0
#			Gravity	.004
#		End
#		Part	:HelixGlow2.part
#	End

End