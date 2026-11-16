#########################################################
##	PL_CollapseTable
########################################################
FxInfo
Flags	InheritAnimScale

Condition
	On	Time
	Time	0
	Event
		EName 	Table
		Type	Start
		At	Mystic
		
		#Bhvr	Behaviors\FX_CollapseTable.bhvr
		Geom	FX_CollapseTable	

	End
End


Condition
	On	Time
	Time	0
	Event
		EName 	CupMiddle
		Type	Local
		At	Mystic
		
		#Bhvr	Behaviors\FX_CollapseTable_Games.bhvr
		Geom	Cup_3CardMonty	

	End

	Event
		EName 	CupLeft
		Type	Local
		At	WepL
		
		#Bhvr	Behaviors\FX_CollapseTable_Games.bhvr
		Geom	Cup_3CardMonty	

	End

	Event
		EName 	CupLeft
		Type	Local
		At	WepR
		
		#Bhvr	Behaviors\FX_CollapseTable_Games01.bhvr
		Geom	Cup_3CardMonty	

	End
End

End
##################################

