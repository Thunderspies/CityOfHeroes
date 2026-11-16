#############################################################
## Beaker.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	80

	Event
		EName 	Beaker
		Type	Start
		At	Mystic
		BhvrOverride
			InitialVelocity		0.0 -1.5 0.0
			SpinJitter		0.05 0.05 0.05
			Physics			1
			physRadius 		1.0
		End
		CEvent 	:Child_BeakerShatter.fx
#		CThresh 	0.001
#		CDestroy 	1
		Lifespan 88
	End
End

#############################################################

End