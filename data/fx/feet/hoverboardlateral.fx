#########################################################
## 	Flight
#########################################################

FxInfo

Input
	InpName Origin
End

#LifeSpan 100

Condition
	Event
		Type Local
		At origin
		Sound HoverboardLateral1 100.0 50.0 .18
		Sound HoverboardLateral2 100.0 50.0 .18
		Sound HoverboardLateral3 100.0 50.0 .18
	End
	Event
		Type Local
		At Origin
		while STEPRIGHT FLY
		Sound HoverboardFly_Loop 40 40 .1
		bhvr	behaviors/Sound/SoundIn3.bhvr
	End
	Event
		Type Local
		At Origin
		while STEPLEFT FLY
		Sound HoverboardFly_Loop 40 40 .1
		bhvr	behaviors/Sound/SoundIn3.bhvr
	End
End
Condition
	On Time
	Time 15
	Event
		Type Local
		At Origin
		while STEPLEFT FLY
		Sound FlightWind_Loop 40 40 .1
		bhvr	behaviors/Sound/SoundIn5.bhvr
	End
		Event
		Type Local
		At Origin
		while STEPRIGHT FLY
		Sound FlightWind_Loop 40 40 .04
		bhvr	behaviors/Sound/SoundIn5.bhvr
	End
End
