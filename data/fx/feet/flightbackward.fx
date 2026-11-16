#########################################################
## 	Flight
#########################################################

FxInfo

Input
	InpName Origin
End

#LifeSpan 100

Condition
	On Time
	Time 0
	Event
		Type Local
		At origin
		Sound Whoosh18 40.0 40.0 .25
		#Sound Whoosh3 40.0 40.0 .25
		# Sound FlightBackward 40.0 40.0 .25
	End
	Event
		Type Local
		At Origin
		while BACKWARD FLY
		Sound FlightFlyNew_Loop 40 40 .07
		#Sound FlightHover_Loop 40 40 .09
		bhvr	behaviors/Sound/SoundIn3.bhvr
	End
End
Condition
	On Time
	Time 15
	Event
		Type Local
		At Origin
		While BACKWARD FLY
		Sound FlightWind_Loop 40 40 .1
		bhvr	behaviors/Sound/SoundIn5.bhvr
	End
End