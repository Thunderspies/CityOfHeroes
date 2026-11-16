#############################################################
## AbyssalMending_Hit.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Time
	Time	0

	Event
		EName	Spinner
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			Spin	0 0.01 0
		End
	End

	Event
		Type	Local
		At	Spinner
		Part	:Ashes.part
	End
End

#############################################################

End


