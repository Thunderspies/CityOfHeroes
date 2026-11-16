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

	Event
		EName 	CupMiddle
		Type	Local
		At	Mystic
		
		#Bhvr	Behaviors\FX_CollapseTable_Games.bhvr
		Geom	Cup_3CardMonty	
		Lifespan 38
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

############## swaps ##################

Condition
	On	Time
	Time	35
	Event
		EName 	CupLeft01
		Type	Start
		At	Wepr
		
		#Bhvr	Behaviors\FX_CollapseTable_Games.bhvr
		Geom	Cup_3CardMonty	
		Lifespan 18
	End

End

Condition
	On	Time
	Time	45
	Event
		EName 	CupMid01
		Type	Start
		At	Mystic
		
		#Bhvr	Behaviors\FX_CollapseTable_Games.bhvr
		Geom	Cup_3CardMonty	
		Lifespan 49
	End

End

Condition
	On	Time
	Time	100
	Event
		EName 	CupMid02
		Type	Start
		At	Mystic
		
		#Bhvr	Behaviors\FX_CollapseTable_Games.bhvr
		Geom	Cup_3CardMonty	
		#Lifespan 36
	End

End

Condition
	On	Time
	Time	91
	Event
		EName 	CupLeft02
		Type	Start
		At	Wepr
		
		#Bhvr	Behaviors\FX_CollapseTable_Games.bhvr
		Geom	Cup_3CardMonty	
		Lifespan 18
	End

End
##################################

