#############################################################
## FX_CorruptedSeerTentacle_01.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

Condition
	On 	Time
	Time 	0

	Event
		EName	Tentacle01
		Type	Local
		At	Head
		Anim	FX_CorruptedSeerTentacle_01
		BhvrOverride
			Scale			1.55 1.25 1.55
			PositionOffset		0.3 0.35 -0.2
			PYRrotate 		-18 0.0 0
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle02
		Type	Local
		At	Head
		Anim	FX_CorruptedSeerTentacle_02
		BhvrOverride
			Scale			0.9 0.9 0.9
			PositionOffset		0.15 0.25 -0.2
			PYRrotate 		-18 0.0 0
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle03
		Type	Local
		At	Head
		Anim	FX_CorruptedSeerTentacle_03
		BhvrOverride
			Scale			0.9 0.9 0.9
			PositionOffset		-0.1 0.075 -0.1
			PYRrotate 		-12 0 0
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle04
		Type	Local
		At	Head
		Anim	FX_CorruptedSeerTentacle_04
		BhvrOverride
			Scale			0.5 0.5 0.8
			PositionOffset		0.14 0.4 0
			PYRrotate 		0 0 0
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle05
		Type	Local
		At	Head
		Anim	FX_CorruptedSeerTentacle_05
		BhvrOverride
			Scale			0.5 0.5 0.8
			PositionOffset		0.10 0.35 -0.4
			PYRrotate 		-10 0 10
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle06
		Type	Local
		At	Head
		Anim	FX_CorruptedSeerTentacle_06
		BhvrOverride
			Scale			0.4 0.4 0.2
			PositionOffset		-0.01 0.4 -0.55
			PYRrotate 		-24 0.0 4
 			PYRrotateJitter 	0 0 0
		End
	End
End

#############################################################

End