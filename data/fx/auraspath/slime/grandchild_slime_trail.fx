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
		Part	:Slime_track.part
		Part	:Slime_Track_Stationary.part
	End
End


#########################################################

End