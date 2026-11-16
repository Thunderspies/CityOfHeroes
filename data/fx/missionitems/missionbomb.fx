#########################################################
##	jack
##
FxInfo
	
Name 	 MissionBomb

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
		#Geom	Item_MissionBomb1
		Part1 MISSIONITEMS/missionitem_highlight.part
	End

End	

End		