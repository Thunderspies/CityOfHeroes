#############################################################
## Books_1.fx
#############################################################

FxInfo

Flags InheritAlpha

########  book   ##########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	WepR_book_1
		Geom	WepR_book_2
		Geom	WepR_book_3
	End
End

#############################################################

End