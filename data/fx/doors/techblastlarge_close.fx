#########################################################
##	Door
#########################################################

FxInfo

Input
	InpName Origin
End

LifeSpan 5
Condition
	Event
		Type Local
		At origin
		Sound Door_TechBlastLarge_Close 100 100 .5
	End
End

