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
		Sound Whoosh8 40.0 40.0 .18
		Sound Whoosh14 40.0 40.0 .18
		Sound Whoosh15 40.0 40.0 .18
		# Sound FlightLateral2 40.0 40.0 .15
		# Sound FlightLateral3 40.0 40.0 .15
	End
	Event
		Type Local
		At Origin
		while STEPRIGHT FLY
		Sound FlightFlyNew_Loop 40 40 .06
		#Sound FlightHover_Loop 40 40 .06
		bhvr	behaviors/Sound/SoundIn3.bhvr
	End
	Event
		Type Local
		At Origin
		while STEPLEFT FLY
		Sound FlightFlyNew_Loop 40 40 .06
		#Sound FlightHover_Loop 40 40 .06
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
