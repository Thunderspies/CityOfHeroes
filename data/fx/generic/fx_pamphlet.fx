#########################################################
##	PL_Pamphlet stack of paper
########################################################
FxInfo

Condition
	On	Time
	Time	0
	Event
		EName 	Stack
		Type	Local
		At	WepL
		Geom	WepL_StackOfPaper

	End

	Event
		EName 	Sheet
		Type	Local
		At	WepR
		Geom	WepR_SheetOfPaper

	End


End

End
##################################

