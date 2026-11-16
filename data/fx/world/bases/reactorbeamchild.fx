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
		#Sound	EntrancePortal1_loop 80 70 .58
	End
End



Condition
	On 	Cycle
	Time 	130	#155



	Event
		EName 	Close
		Type	Start
		Bhvr	:HelixSpinner.bhvr
		BhvrOverride
			PositionOffset	0.0 80.0 0.0
		End
		Pother	VerticalOffset
		Lifespan 125
	End

	Event
		EName 	closeHelix2
		Type	Local
		At	Close
		Bhvr	WORLD\Bases\HelixOffset2.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -3
		End
		part	WORLD\Bases\DataRectangles.part
		Lifespan	120
	End

	Event
		EName 	closeHelix3
		Type	Local
		At	Close
		Bhvr	WORLD\Bases\HelixOffset2.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 6
			Spin		0.0 0.3 0.0

		End
		part	WORLD\Bases\DataRectangles.part
		Lifespan	120
	End

End



####################################
######## OUTER LINES ###############
####################################


Condition
	On	Time
	Time 	0


	Event
		EName 	helix2
		Type	Local
		At	Origin
		Bhvr	behaviors\PortalRiftOffset3.bhvr
		BhvrOverride
			PositionOffset 0.0 90.0 0.0
		End
		Part	:DataBeams.part  # (squarish light beams)
	End

	Event
		EName 	helix2
		Type	Local
		At	Origin
		Part	:DataPullouts.part
	End


End