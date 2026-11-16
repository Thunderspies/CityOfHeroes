#############################################################
## FX_SeedHamidonTentacle_01.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

Condition
	On 	Time
	Time 	0

	Event
		EName	Tentacle01
		Type	Local
		At	Hips
		Anim	FX_SeedHamidonTentacle_01
		BhvrOverride
			Scale			0.6 0.6 0.6
			PositionOffset		1.05 1.7 -1.3
			PYRrotate 		15 -45 220
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle02
		Type	Local
		At	Head
		Anim	FX_SeedHamidonTentacle_01
		BhvrOverride
			Scale			0.3 0.3 0.3
			PositionOffset		2.12 1.92 -4.3
			PYRrotate 		155 35 290
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle03
		Type	Local
		At	Hips
		Anim	FX_SeedHamidonTentacle_01
		BhvrOverride
			Scale			0.6 0.6 0.6
			PositionOffset		-1.1 1.7 -1.45
			PYRrotate 		15 225 220
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle04
		Type	Local
		At	Head
		Anim	FX_SeedHamidonTentacle_01
		BhvrOverride
			Scale			0.3 0.3 0.3
			PositionOffset		-2.12 1.87 -4.17
			PYRrotate 		170 165 290
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle05
		Type	Local
		At	Hips
		Anim	FX_SeedHamidonTentacle_01
		BhvrOverride
			Scale			0.3 0.2 0.3
			PositionOffset		1.1 1.0 0.5
			PYRrotate 		15 -45 220
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle06
		Type	Local
		At	Hips
		Anim	FX_SeedHamidonTentacle_01
		BhvrOverride
			Scale			0.35 0.35 0.45
			PositionOffset		1.6 1.2 0.12
			PYRrotate 		15 -45 220
 			PYRrotateJitter 	0 0 0
		End
	End

	Event
		EName	Tentacle07
		Type	Local
		At	Hips
		Anim	FX_SeedHamidonTentacle_01
		BhvrOverride
			Scale			0.35 0.3 0.4
			PositionOffset		-1.6 1.2 0.35
			PYRrotate 		15 200 220
 			PYRrotateJitter 	0 0 0
		End
	End
End

#############################################################

End