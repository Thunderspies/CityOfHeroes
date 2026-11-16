#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		Time
	Time 		0
	DoMany		1

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	1.0 0.0 0.0
		End
		Part	:Slime_track_Fist.part
		Part	:Slime_Track_Stationary_Fist.part
	End
End


#########################################################

End