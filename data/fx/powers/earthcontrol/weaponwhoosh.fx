#########################################################
##	KickStreaks
########################################################
FxInfo

Input  
	Inpname	Origin
End

LifeSpan	20

Condition
	On 	Time
	Time 	13

	Event
		Type	local
		At	Origin
		Sound stonewhoosh 100.0 100.0 .9
		LifeSpan	20
	End

End



##################################

