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
		Sound LambdaBlast_Close 100 100 .9
	End
End

