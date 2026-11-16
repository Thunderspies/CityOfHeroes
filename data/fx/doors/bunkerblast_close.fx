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
		Sound Door_BunkerBlast_Close 100 100 .5
	End
End

