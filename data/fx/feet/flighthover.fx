#########################################################
## 	Flight
#########################################################

FxInfo

Input
	InpName Origin
End

LifeSpan 30

Condition
	Event
		Type Local
		At origin
		Sound FlightHover1 40.0 40.0 .15
		Sound FlightHover2 40.0 40.0 .15
		Sound FlightHover3 40.0 40.0 .15
	End
	# Event
		# Type Local
		# At Origin
		# while fly
		# Sound FlightHover_Loop 40 40 .025
		# bhvr	behaviors/Sound/SoundIn05.bhvr
	# End
End