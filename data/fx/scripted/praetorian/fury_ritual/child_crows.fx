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
		EName 	GoalSpin
		Type	Start
		At	Origin
		BhvrOverride
			Spin		0 0.075 0
			SpinJitter	0 0.025 0
		End
		Lifespan 80
	End

	Event
		EName 	GoalOffset
		Type	Local
		At	GoalSpin
		BhvrOverride
			PositionOffset		30 10 30
			StartJitter		10 10 10
		End
		Lifespan 80
	End

	Event
		EName 	Crow
		Type	Start
		At	Origin
		Bhvr	behaviors/fadeout.bhvr
		BhvrOverride
			StartColor		1 1 1
			FadeOutLength		10
			PositionOffset		0 3 3
		#	StartJitter		1 3 1
			Scale			0.5 0.5 0.5
			Trackrate		1.0
		End
		Anim	FX_Crow_flock
		Magnet	GoalOffset
		LookAt	GoalOffset
		Lifespan 60
	End

#	Event
#		EName 	Prime2
#		Type	Start
#		At	Chest
#		Bhvr	behaviors/fadein.bhvr
#		BhvrOverride
#			StartColor	1 1 1
#			FadeInLength	20
#			PositionOffset	0 20 0
#			StartJitter	50 25 50
#			TrackRate	2.0
#			Scale		0.5 0.5 0.5
#			InitialVelocityJitter	0.5 0.7 0.5
#		End
#		Anim	FX_Crow_flock
#		LookAt	Chest
#		Magnet	Chest
#		Lifespan 80
#	End
#
#	Event
#		EName 	Prime3
#		Type	Start
#		At	Chest
#		Bhvr	behaviors/fadein.bhvr
#		BhvrOverride
#			StartColor	1 1 1
#			FadeInLength	20
#			PositionOffset	0 20 0
#			StartJitter	50 25 50
#			TrackRate	2.0
#			Scale		0.5 0.5 0.5
#			InitialVelocityJitter	0.5 0.7 0.5
#		End
#		Anim	FX_Crow_flock
#		LookAt	Chest
#		Magnet	Chest
#		Lifespan 80
#	End
#
#	Event
#		EName 	Prime4
#		Type	Start
#		At	Chest
#		Bhvr	behaviors/fadein.bhvr
#		BhvrOverride
#			StartColor	1 1 1
#			FadeInLength	20
#			PositionOffset	0 20 0
#			StartJitter	50 25 50
#			TrackRate	2.0
#			Scale		0.5 0.5 0.5
#			InitialVelocityJitter	0.5 0.7 0.5
#		End
#		Anim	FX_Crow_flock
#		LookAt	Chest
#		Magnet	Chest
#		Lifespan 80
#	End
#
#	Event
#		EName 	Prime5
#		Type	Start
#		At	Chest
#		Bhvr	behaviors/fadein.bhvr
#		BhvrOverride
#			StartColor	1 1 1
#			FadeInLength	20
#			PositionOffset	0 20 0
#			StartJitter	50 25 50
#			TrackRate	2.0
#			Scale		0.5 0.5 0.5
#			InitialVelocityJitter	0.5 0.7 0.5
#		End
#		Anim	FX_Crow_flock
#		LookAt	Chest
#		Magnet	Chest
#		Lifespan 80
#	End
#
#	Event
#		EName 	Prime6
#		Type	Start
#		At	Chest
#		Bhvr	behaviors/fadein.bhvr
#		BhvrOverride
#			StartColor	1 1 1
#			FadeInLength	20
#			PositionOffset	0 20 0
#			StartJitter	50 25 50
#			TrackRate	2.0
#			Scale		0.5 0.5 0.5
#			InitialVelocityJitter	0.5 0.7 0.5
#		End
#		Anim	FX_Crow_flock
#		LookAt	Chest
#		Magnet	Chest
#		Lifespan 80
#	End
#
#	Event
#		EName 	Prime7
#		Type	Start
#		At	Chest
#		Bhvr	behaviors/fadein.bhvr
#		BhvrOverride
#			StartColor	1 1 1
#			FadeInLength	20
#			PositionOffset	0 20 0
#			StartJitter	50 25 50
#			TrackRate	2.0
#			Scale		0.5 0.5 0.5
#			InitialVelocityJitter	0.5 0.7 0.5
#		End
#		Anim	FX_Crow_flock
#		LookAt	Chest
#		Magnet	Chest
#		Lifespan 80
#	End
#
#	Event
#		EName 	Prime8
#		Type	Start
#		At	Chest
#		Bhvr	behaviors/fadein.bhvr
#		BhvrOverride
#			StartColor	1 1 1
#			FadeInLength	20
#			PositionOffset	0 20 0
#			StartJitter	50 25 50
#			TrackRate	2.0
#			Scale		0.5 0.5 0.5
#			InitialVelocityJitter	0.5 0.7 0.5
#		End
#		Anim	FX_Crow_flock
#		LookAt	Chest
#		Magnet	Chest
#		Lifespan 80
#	End
End

#Condition
#	On	PrimeHit
#
#	Event
#		EName 	Prime
#		Type	DESTROY
#	End
#End
#
#Condition
#	On	Prime2Hit
#
#	Event
#		EName 	Prime2
#		Type	DESTROY
#	End
#End
#
#Condition
#	On	Prime3Hit
#
#	Event
#		EName 	Prime3
#		Type	DESTROY
#	End
#End
#
#Condition
#	On	Prime4Hit
#
#	Event
#		EName 	Prime4
#		Type	DESTROY
#	End
#End
#
#Condition
#	On	Prime5Hit
#
#	Event
#		EName 	Prime5
#		Type	DESTROY
#	End
#End
#
#Condition
#	On	Prime6Hit
#
#	Event
#		EName 	Prime6
#		Type	DESTROY
#	End
#End
#
#Condition
#	On	Prime7Hit
#
#	Event
#		EName 	Prime7
#		Type	DESTROY
#	End
#End
#
#Condition
#	On	Prime8Hit
#
#	Event
#		EName 	Prime8
#		Type	DESTROY
#	End
#End

#############################################################

End