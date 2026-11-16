#############################################################
## HEADER
#############################################################

FxInfo

#Lifespan 300

#############################################################

Condition
	Repeat	14

	Event
		Type	Start
		At	WepR
		BhvrOverride
			PyrRotateJitter		15 15 15
			SpinJitter		4.0 4.0 4.0
			InitialVelocity		0.0 1.0 0.0
			InitialVelocityJitter	0.375 0.5 0.375
			Drag			0.05
			Gravity			0.02
			physics 		1
			physScale 		2.0 2.0 2.0
			physRadius 		1.8
			physGravity 		0.5
			physRestitution 	0.1
			physSFriction 	 	0.8
			physDFriction 	 	0.5
			PhysDebris		0
			PhysDensity		1.5
			FadeOutLength		60
		End
		Geom	GEO_PlayingCard_10D
		Geom	GEO_PlayingCard_2H
		Geom	GEO_PlayingCard_3S
		Geom	GEO_PlayingCard_4C
		Geom	GEO_PlayingCard_6D
		Geom	GEO_PlayingCard_7H
		Geom	GEO_PlayingCard_8C
		Geom	GEO_PlayingCard_AS
		Geom	GEO_PlayingCard_JS
		Geom	GEO_PlayingCard_KD
		Geom	GEO_PlayingCard_QH
		CEvent 		Emotes\CardTrick\Child_MagicSmoke.fx
		CThresh 	0.000001
		CDestroy 	1
	End
End

Condition
	On 	Time
	Time 	300

	Event
		EName 	All
		Type	Destroy
	End


End

#############################################################

End