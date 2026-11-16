
FxInfo

Lifespan	300

#####  Snakes  #######################################

Condition
	On	Time
	Time	0

	Event
		EName	T_C_01
		Type	create		
		Inherit	Position Scale
		At	T_Root
		BhvrOverride
			StartJitter	2 0 2
			PositionOffset	5 0 5
		End
		
	End

	Event
		EName	T_L_01
		Type	create		
		Inherit	Position Scale
		At	T_Root
		BhvrOverride
			StartJitter	5 0 2
			PositionOffset	15 0 5

		End
	End

	Event
		EName	T_R_01
		Type	create		
		Inherit	Position Scale
		At	T_Root
		BhvrOverride
			StartJitter	2 0 5
			PositionOffset	5 0 15
		End
		
	End

	Event
		EName	T_L_02
		Type	create		
		Inherit	Position Scale
		At	T_Root
		BhvrOverride
			StartJitter	5 0 2
			PositionOffset	-15 0 -5
		End
	End

	Event
		EName	T_R_02
		Type	create		
		Inherit	Position Scale
		Update	Position
		At	T_Root
		BhvrOverride
			StartJitter	2 0 5
			PositionOffset	-5 0 -15
		End
		
	End
	
	Event
		EName	T_C_02
		Type	create		
		Inherit	Position Scale
		At	T_Root
		BhvrOverride
			StartJitter	2 0 2
			PositionOffset	-5 0 -5
		End
		
	End
End

###########################  Single Snake At Target  ################################

Condition
	On	Time
	Time	5

	Event
		EName	Prime
		Type	create		
		Inherit	Position Rotation
		At	Root
		
		BhvrOverride
			TrackRate	.5
			Scale		.65 .65 .65	#1 1 .1
			StartJitter	5.5 0.0 5.5
			StartJitter		5.5 0.0 5.5
#			FadeInLength		130
#			FadeOutLength		160
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake				#Anim refers to Entype filename
		
		Magnet	T_R_01

	End

End

Condition
	On	Time
	Time	6

	Event
		EName	Prime1
		Type	create		
		Inherit	Position Rotation
		At	Root
		
		BhvrOverride
			TrackRate	.55
			Scale		.65 .65 .65	#1 1 .1
			StartJitter	5.5 0.0 5.5
			StartJitter		5.5 0.0 5.5
#			FadeInLength		130
#			FadeOutLength		160
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake				#Anim refers to Entype filename
		
		Magnet	T_C_01

	End

End

Condition
	On	Time
	Time	7

	Event
		EName	Prime2
		Type	create		
		Inherit	Position Rotation
		At	Root
		
		BhvrOverride
			TrackRate	.6
			Scale		.65 .65 .65	#1 1 .1
			StartJitter	5.5 0.0 5.5
			StartJitter		5.5 0.0 5.5
#			FadeInLength		130
#			FadeOutLength		160
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake				#Anim refers to Entype filename
		
		Magnet	T_L_01

	End

End

Condition
	On	Time
	Time	5

	Event
		EName	Prime3
		Type	create		
		Inherit	Position Rotation
		At	Root
		
		BhvrOverride
			TrackRate	.56
			Scale		.65 .65 .65	#1 1 .1
			StartJitter	5.5 0.0 5.5
			StartJitter		5.5 0.0 5.5
#			FadeInLength		130
#			FadeOutLength		160
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake				#Anim refers to Entype filename
		
		Magnet	T_R_02

	End

End

Condition
	On	Time
	Time	2

	Event
		EName	Prime4
		Type	create		
		Inherit	Position Rotation
		At	Root
		
		BhvrOverride
			TrackRate	.45
			Scale		.65 .65 .65	#1 1 .1
			StartJitter	5.5 0.0 5.5
			StartJitter		5.5 0.0 5.5
#			FadeInLength		130
#			FadeOutLength		160
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake				#Anim refers to Entype filename
		
		Magnet	T_L_01

	End

End

Condition
	On	Time
	Time	2

	Event
		EName	Prime5
		Type	create		
		Inherit	Position Rotation
		At	Root
		
		BhvrOverride
			TrackRate	.45
			Scale		.65 .65 .65	#1 1 .1
			StartJitter	5.5 0.0 5.5
			StartJitter		5.5 0.0 5.5
#			FadeInLength		130
#			FadeOutLength		160
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake				#Anim refers to Entype filename
		
		Magnet	T_R_01

	End

End

Condition
	On	Time
	Time	7

	Event
		EName	Prime6
		Type	create		
		Inherit	Position Rotation
		At	Root
		
		BhvrOverride
			TrackRate	.65
			Scale		.65 .65 .65	#1 1 .1
			StartJitter	5.5 0.0 5.5
			StartJitter		5.5 0.0 5.5
#			FadeInLength		130
#			FadeOutLength		160
		End

		bhvr	:B_SnakeSummonFade.bhvr
		Anim	FX_Snake				#Anim refers to Entype filename
		
		Magnet	T_C_02

	End

End
###########################  Kill Snake 01  ################################

Condition
	On	PrimeHit
		
	Event
		EName	Prime
		Type	Destroy
	End

End

Condition
	On	Prime1Hit
		
	Event
		EName	Prime1
		Type	Destroy
	End

End

Condition
	On	Prime2Hit
		
	Event
		EName	Prime2
		Type	Destroy
	End

End

Condition
	On	Prime3Hit
		
	Event
		EName	Prime3
		Type	Destroy
	End

End

Condition
	On	Prime4Hit
		
	Event
		EName	Prime4
		Type	Destroy
	End

End

Condition
	On	Prime5Hit
		
	Event
		EName	Prime5
		Type	Destroy
	End

End

Condition
	On	Prime6Hit
		
	Event
		EName	Prime6
		Type	Destroy
	End

End

End