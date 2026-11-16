#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan	1800

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset
		Type	Local 
		At	Root
		part	:LingeringSmoke1.part
		part	:LingeringSmoke2.part
		part	:LingeringSmoke3.part
	End
End

#############################################################

End