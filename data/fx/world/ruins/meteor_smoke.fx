#############################################################
## Meteor_Smoke.fx
#############################################################

FxInfo
LifeSpan 0
Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Part	:Meteor_Smoke.part
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Root
		Part	:Meteor_Smoke2.part
	End
End


#############################################################

End