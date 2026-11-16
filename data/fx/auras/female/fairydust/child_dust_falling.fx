#############################################################
## FairyDust_Body.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		BhvrOverride
			StartJitter	0.5 0.5 0.5
			InitialVelocityJitter	0.0 -0.075 0.0
			Gravity		0.004
		End
		Part	:Dust_Falling.part
		Lifespan	20
	End

End

#########################################################

End