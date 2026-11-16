#########################################################
##
##

FxInfo

LifeSpan  200
Flags	InheritAnimScale

###############################################################################
##################################   Spit   ###################################
###############################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TestGeom01
		Type	Local
		At	C_FootR
		bhvrOverride
			PositionOffset	0 1.65 .075

		End

		#Geom	Testing_TargetB
	End

#	Event
#		EName	TestGeom02
#		Type	Local
#		At	C_FootR
#
#		Geom	Testing_TargetB
#	End
#
#	Event
#		EName	TestGeom03
#		Type	Local
#		At	C_LlegR
#
#		Geom	Testing_TargetB
#	End
#
#	Event
#		EName	TestGeom04
#		Type	Local
#		At	C_UlegR
#
#		Geom	Testing_TargetB
#	End

End

End