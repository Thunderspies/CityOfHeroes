#############################################################
## EnvFX_SubwayDebris_Light_Child.fx
#############################################################

FxInfo

#############################################################



Condition
	On	Time
	Time	300

	Event
		EName	CoreAnchor1
		Type	Start
		At	Origin
		BhvrOverride
			StartJitter	3 1 5
		End
		Part	:Offset_Sand_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	306

	Event
		EName	CoreAnchor2
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 5
			StartJitter	3 1 5
		End
		Part	:Offset_Dust_Stream_Light.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	312
	Chance	.6

	Event
		EName	CoreAnchor3
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 10
			StartJitter	3 1 5
		End
		Part	:Offset_Sand_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	318

	Event
		EName	CoreAnchor4
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 15
			StartJitter	3 1 5
		End
		Part	:Offset_Sand_Stream.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	324

	Event
		EName	CoreAnchor5
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 20
			StartJitter	3 1 5
		End
		Part	:Offset_Dust_Stream_Light.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	330
	Chance	.6

	Event
		EName	CoreAnchor6
		Type	Start
		At	Origin
		Bhvr	behaviors/CameraShake_Subway_Subtle.bhvr
		BhvrOverride
			PositionOffset	0 0 25
			StartJitter	3 1 5
		End
		Part	:Offset_Sand_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	336

	Event
		EName	CoreAnchor7
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 30
			StartJitter	3 1 5
		End
		Part	:Offset_Sand_Stream_Light.part
		Part	:Offset_Dust_Stream_Light.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	342
	Chance	.6

	Event
		EName	CoreAnchor8
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 35
			StartJitter	3 1 5
		End
		Part	:Offset_Sand_Stream_Light.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	348

	Event
		EName	CoreAnchor9
		Type	Start
		At	Origin
		Bhvr	behaviors/CameraShake_Subway_Subtle.bhvr
		BhvrOverride
			PositionOffset	0 0 40
			StartJitter	3 1 5
		End
		Part	:Offset_Sand_Stream_Light.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	354

	Event
		EName	CoreAnchor10
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 45
			StartJitter	3 1 5
		End
		Part	:Offset_Sand_Stream_Light.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	360

	Event
		EName	CoreAnchor11
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 50
			StartJitter	3 1 5
		End
		Part	:Offset_Dust_Stream.part
		Part	:Offset_Sand_Stream_Light.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	366

	Event
		EName	CoreAnchor12
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 55
			StartJitter	3 1 5
		End
		Part	:Offset_Sand_Stream_Light.part
		Lifespan 20
	End
End
#########################################################

End