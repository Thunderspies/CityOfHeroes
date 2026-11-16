#############################################################
## Meteor_Pebble_Hits.fx
#############################################################

FxInfo
#############################################################

Condition
	On		Time
	Time		0

	Event
		Ename	PebbleHits
		Type	Local
		At	Origin
		Part	:Meteor_Pebble_Hits.part
		Part	:Meteor_Pebble_Dust.part
	End
End

#############################################################
End
