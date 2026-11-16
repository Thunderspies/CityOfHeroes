#########################################################
## 	Flight
#########################################################

FxInfo

Input
	InpName Origin
End

#LifeSpan 30

Condition
	# Event
		# Type Local
		# At Origin
		# while forward
		# Sound FlightFly_Loop 40 40 .09
		# bhvr	behaviors/Sound/SoundIn05.bhvr
	# End
End