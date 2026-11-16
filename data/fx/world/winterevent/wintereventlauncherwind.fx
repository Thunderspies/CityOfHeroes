
FxInfo

#########################################################################
##Wind Geoms
############################

		

Condition
	On 	Time
	Time 	80
Random	1

	Event
		EName   Wind
		Type	start 
		At	origin

		Bhvr	:WindGeom2.bhvr

#		BhvrOverride
#			
##			InitialVelocity			0.0 0.6 0.0
##			InitialVelocityJitter		0.0 0.3 0.0
#
##			Spin				0.0 .05 0.0
##			SpinJitter			0.0 0.1 0.0
#
##			Alpha			70
#
#
#		End

		geom	FX_TmpCtrlWisp01
		LifeSpan	40
	End

	Event
		EName   WindFast
		Type	start 
		At	origin

		Bhvr	:WindGeom.bhvr

#		BhvrOverride
#			
##			InitialVelocity			0.0 0.6 0.0
##			InitialVelocityJitter		0.0 0.3 0.0
#
##			Spin				0.0 .05 0.0
##			SpinJitter			0.0 0.1 0.0
#
##			Alpha			40
#
#		End

		geom	FX_TmpCtrlWisp01
		LifeSpan	40
	End
End

#Condition
#	On 	Time
#	Time 	10
#	Event
#		EName   WindFirst
#		Type	start 
#		At	origin
#
#		BhvrOverride
#			
#			StartJitter			0.0 0.0 0.0
#			InitialVelocity			0.0 0.2 0.0
#			InitialVelocityJitter		0.0 0.0 0.0
#
#			Spin				0.0 1.7 0.0
#			SpinJitter			0.0 0.0 0.0
#
#			FadeInLength			20
#			FadeOutLength			50
#
#
#			Scale			.45 .45 .45
#			ScaleRate		1.03 1.005 1.03
#			EndScale		13.65 1.65 13.65
#
#			TrackRate			0.1
#			TrackMethod			0
#
#			AnimScale			1.0
#
#			LifeSpan			-1
#
#		End
#
#		geom	FX_TmpCtrlWisp01
#		LifeSpan	150
#		
#	End
#End
#
End	


			