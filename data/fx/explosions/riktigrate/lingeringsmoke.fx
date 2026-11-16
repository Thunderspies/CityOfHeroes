#############################################################
## FX System Name
#############################################################

FxInfo  

#LifeSpan	1100

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Smoke
		Type	Start 
		At	Root
		part	:LingeringSmoke1.part
		part	:LingeringSmoke2.part
		#part	:LingeringSmoke3.part
	End
End

#############################################################

End			