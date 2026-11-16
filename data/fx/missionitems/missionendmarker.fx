#########################################################
##	jack
##
FxInfo
	
Name 	 MissionEndMarker

Input  
	InpName	Root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Root
		Geom	MissionExitMarker
		Part1 MISSIONITEMS/missionitem_highlight.part
		#Part2 MISSIONITEMS/missionitem_highlight2.part
	End
End


End			