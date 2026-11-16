#############################################################
## EnvFX_SubwayDebris.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Sound
		Type	Start
		At	Origin
		Sound TrainDepot_DistantTrainFX2 500 250 .7
		Lifespan 1
	End
End

Condition
	On	Time
	Time	300

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 0
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	306

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 5
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream_Light.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	312
	Chance	0.6

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 10
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	318

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 15
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream_Light.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	324

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		Bhvr	behaviors/CameraShakeFault.bhvr
		BhvrOverride
			PositionOffset	0 0 20
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	330
	Chance	0.6

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 25
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream_Light.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	336

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 30
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	342

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 35
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream_Light.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	348
	Chance	0.6

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		Bhvr	behaviors/CameraShakeFault.bhvr
		BhvrOverride
			PositionOffset	0 0 40
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	354

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 45
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream_Light.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	360

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 50
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream_Light.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	355

	Event
		EName	CoreAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 0 55
			StartJitter	3 1 3
		End
		Part	:Offset_Sand_Stream_Light.part
		Part	:Offset_Dust_Stream.part
		Lifespan 20
	End
End

#########################################################

End