#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo

#############################################################

#Condition
#	On	Time
#	Time	35
#
#	Event
#		Ename	PebbleHits
#		Type	Local
#		At	Origin
#		ChildFX	:Meteor_Pebble_Hits.fx
#		Lifespan 40
#	End
#End

Condition
	On	Time
	Time	0
	Chance	0.7

	Event
		Ename	pebble1
		Type	Local
		At	Origin
		ChildFX	:Meteor_Attack_MiniGRANDCHILD.fx
	End
End

Condition
	On	Time
	Time	5

	Event
		Ename	pebble2
		Type	Local
		At	Origin
		ChildFX	:Meteor_Attack_MiniGRANDCHILD.fx
	End
End

Condition
	On	Time
	Time	20
	Chance	0.9

	Event
		Ename	pebble2
		Type	Local
		At	Origin
		ChildFX	:Meteor_Attack_MiniGRANDCHILD.fx
		ChildFX	:Meteor_Attack_MiniGRANDCHILD.fx
		Lifespan 100
	End
End

Condition
	On	Time
	Time	40
	Chance	0.9

	Event
		Ename	pebble2
		Type	Local
		At	Origin
		ChildFX	:Meteor_Attack_MiniGRANDCHILD.fx
		ChildFX	:Meteor_Attack_MiniGRANDCHILD.fx
		ChildFX	:Meteor_Pebble_Hits.fx
	End
End

Condition
	On	Time
	Time	60
	Chance	0.9

	Event
		Ename	pebble2
		Type	Local
		At	Origin
		ChildFX	:Meteor_Attack_MiniGRANDCHILD.fx
	End
End

Condition
	On	Time
	Time	80
	Chance	0.9

	Event
		Ename	pebble2
		Type	Local
		At	Origin
		ChildFX	:Meteor_Attack_MiniGRANDCHILD.fx
	End
End

Condition
	On	Time
	Time	100
	Chance	0.9

	Event
		Ename	pebble2
		Type	Local
		At	Origin
		ChildFX	:Meteor_Attack_MiniGRANDCHILD.fx
	End
End

#############################################################

End

