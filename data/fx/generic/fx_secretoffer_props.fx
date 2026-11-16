#############################################################
## FX_SecretOffer_Props.fx
#############################################################

FXInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	RightFlask
		Type	Local
		At	WepR
		BhvrOverride
			PositionOFfset	0 0 -0.2
			PYRrotate	-90 0 0
		End
		Geom	GEO_ErlenmeyerFlask_01
	End


	Event
		Ename	RightFlaskOffset
		Type	Local
		At	RightFlask
		AltPiv	1

		Part	Emotes\TestTubes\Smoke_Trail.part
	End


End

########################################################

End

