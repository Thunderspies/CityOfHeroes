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
		Sound Door_Tech_LobbyDoors_Close 50 50 .5
	End
End

