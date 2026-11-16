#########################################################
##	KickStreaks
########################################################
FxInfo

Input  
	Inpname	Origin
End

LifeSpan	30

Condition
	On 	Time
	Time 	9

	Event
		Type	local
		At	Origin
		Sound stonewhoosh2 100.0 100.0 .9
		LifeSpan	30
	End

End



##################################

