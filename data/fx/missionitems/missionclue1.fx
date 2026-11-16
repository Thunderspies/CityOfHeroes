#########################################################
##	jack
##
FxInfo
	
Name 	 MissionClue1

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
		#Geom	Item_MissionClue1
		Part1 MISSIONITEMS/missionitem_highlight.part
	End
End

End			