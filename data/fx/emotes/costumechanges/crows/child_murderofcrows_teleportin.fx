#############################################################
## MurderOfCrows_TeleportIn.bhvr
#############################################################

FxInfo

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On Time
	Time 0

	Event
		EName 	Prime
		Type	Start
		At	Chest
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartColor	1 1 1
			FadeInLength	20
			PositionOffset	0 20 0
			StartJitter	50 25 50
			TrackRate	2.0
			Scale		0.5 0.5 0.5
			InitialVelocityJitter	0.5 0.7 0.5
		End
		Anim	FX_Crow_flock
		LookAt	Chest
		Magnet	Chest
		Lifespan 80
	End

	Event
		EName 	Prime2
		Type	Start
		At	Chest
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartColor	1 1 1
			FadeInLength	20
			PositionOffset	0 20 0
			StartJitter	50 25 50
			TrackRate	2.0
			Scale		0.5 0.5 0.5
			InitialVelocityJitter	0.5 0.7 0.5
		End
		Anim	FX_Crow_flock
		LookAt	Chest
		Magnet	Chest
		Lifespan 80
	End

	Event
		EName 	Prime3
		Type	Start
		At	Chest
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartColor	1 1 1
			FadeInLength	20
			PositionOffset	0 20 0
			StartJitter	50 25 50
			TrackRate	2.0
			Scale		0.5 0.5 0.5
			InitialVelocityJitter	0.5 0.7 0.5
		End
		Anim	FX_Crow_flock
		LookAt	Chest
		Magnet	Chest
		Lifespan 80
	End

	Event
		EName 	Prime4
		Type	Start
		At	Chest
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartColor	1 1 1
			FadeInLength	20
			PositionOffset	0 20 0
			StartJitter	50 25 50
			TrackRate	2.0
			Scale		0.5 0.5 0.5
			InitialVelocityJitter	0.5 0.7 0.5
		End
		Anim	FX_Crow_flock
		LookAt	Chest
		Magnet	Chest
		Lifespan 80
	End

	Event
		EName 	Prime5
		Type	Start
		At	Chest
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartColor	1 1 1
			FadeInLength	20
			PositionOffset	0 20 0
			StartJitter	50 25 50
			TrackRate	2.0
			Scale		0.5 0.5 0.5
			InitialVelocityJitter	0.5 0.7 0.5
		End
		Anim	FX_Crow_flock
		LookAt	Chest
		Magnet	Chest
		Lifespan 80
	End

	Event
		EName 	Prime6
		Type	Start
		At	Chest
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartColor	1 1 1
			FadeInLength	20
			PositionOffset	0 20 0
			StartJitter	50 25 50
			TrackRate	2.0
			Scale		0.5 0.5 0.5
			InitialVelocityJitter	0.5 0.7 0.5
		End
		Anim	FX_Crow_flock
		LookAt	Chest
		Magnet	Chest
		Lifespan 80
	End

	Event
		EName 	Prime7
		Type	Start
		At	Chest
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartColor	1 1 1
			FadeInLength	20
			PositionOffset	0 20 0
			StartJitter	50 25 50
			TrackRate	2.0
			Scale		0.5 0.5 0.5
			InitialVelocityJitter	0.5 0.7 0.5
		End
		Anim	FX_Crow_flock
		LookAt	Chest
		Magnet	Chest
		Lifespan 80
	End

	Event
		EName 	Prime8
		Type	Start
		At	Chest
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartColor	1 1 1
			FadeInLength	20
			PositionOffset	0 20 0
			StartJitter	50 25 50
			TrackRate	2.0
			Scale		0.5 0.5 0.5
			InitialVelocityJitter	0.5 0.7 0.5
		End
		Anim	FX_Crow_flock
		LookAt	Chest
		Magnet	Chest
		Lifespan 80
	End
End

Condition
	On	PrimeHit

	Event
		EName 	Prime
		Type	DESTROY
	End
End

Condition
	On	Prime2Hit

	Event
		EName 	Prime2
		Type	DESTROY
	End
End

Condition
	On	Prime3Hit

	Event
		EName 	Prime3
		Type	DESTROY
	End
End

Condition
	On	Prime4Hit

	Event
		EName 	Prime4
		Type	DESTROY
	End
End

Condition
	On	Prime5Hit

	Event
		EName 	Prime5
		Type	DESTROY
	End
End

Condition
	On	Prime6Hit

	Event
		EName 	Prime6
		Type	DESTROY
	End
End

Condition
	On	Prime7Hit

	Event
		EName 	Prime7
		Type	DESTROY
	End
End

Condition
	On	Prime8Hit

	Event
		EName 	Prime8
		Type	DESTROY
	End
End

#############################################################

End