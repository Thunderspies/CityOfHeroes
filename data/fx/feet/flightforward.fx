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
		Sound Whoosh3 40.0 40.0 .25
		Sound Whoosh16 40.0 40.0 .25
		Sound Whoosh17 40.0 40.0 .25
		# Sound FlightForward1 40.0 40.0 .25
		# Sound FlightForward2 40.0 40.0 .25
		# Sound FlightForward3 40.0 40.0 .25
	End
	Event
		Type Local
		At Origin
		while FORWARD FLY
		Sound FlightFlyNew_Loop 40 40 .14
		bhvr	behaviors/Sound/SoundIn3.bhvr
	End
	Event
		Type Local
		At Origin
		while JUMP FLY
		Sound FlightFlyNew_Loop 40 40 .14
		#Sound FlightFly_Loop 40 40 .09
		bhvr	behaviors/Sound/SoundIn3.bhvr
	End


End
Condition
	On Time
	Time 15
	Event
		Type Local
		At Origin
		While FORWARD FLY
		Sound FlightWind_Loop 40 40 .15
		bhvr	behaviors/Sound/SoundIn5.bhvr
	End
		Event
		Type Local
		At Origin
		While JUMP FLY
		Sound FlightWind_Loop 40 40 .15
		bhvr	behaviors/Sound/SoundIn5.bhvr
	End
End
	